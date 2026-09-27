#!/bin/env python3

import struct
import sys
import os

sys.path.append(os.path.abspath(os.path.dirname(__file__)))

# modules we depend on
from utils.mips import Mips
from utils.elf import ElfWrapper
from utils.ldparse import LdScript

import lief
import pyxdelta

ELF32_EHDR_FMT = "<16sHHIIIIIHHHHHH"
ELF32_PHDR_FMT = "<IIIIIIII"

ELF32_EHDR_SIZE = struct.calcsize(ELF32_EHDR_FMT)
ELF32_PHDR_SIZE = struct.calcsize(ELF32_PHDR_FMT)

PT_LOAD = 1

PF_X = 1
PF_W = 2
PF_R = 4

def alignUp(value: int, align: int) -> int:
	return (value + align - 1) & ~(align - 1)

def patchInjectCore(original: bytes, patchCore: bytes, patchVirtualLoad: int) -> bytes:
	# Parse the ELF header.
	ehdr = list(struct.unpack_from(ELF32_EHDR_FMT, original, 0))

	e_ident = ehdr[0]
	e_phoff = ehdr[5]
	e_phentsize = ehdr[9]
	e_phnum = ehdr[10]

	# Sanity checks.
	if e_ident[:4] != b"\x7fELF":
		raise ValueError("stupidass")

	if e_ident[4] != 1:
		raise ValueError("Not an ELF32. What are we doing here")

	if e_ident[5] != 1:
		raise ValueError("ok you're just being silly on purpose now")

	if e_phentsize != ELF32_PHDR_SIZE:
		raise ValueError(
			f"Incorrect program-header size of {e_phentsize:#x} in ELF file"
		)

	old_phdr_end = e_phoff + e_phnum * e_phentsize
	new_phdr_end = e_phoff + (e_phnum + 1) * e_phentsize

	# Find the first PT_LOAD PHDR.
	first_load_offset = None
	for i in range(e_phnum):
		phoff = e_phoff + i * e_phentsize

		ph = struct.unpack_from(
			ELF32_PHDR_FMT,
			original,
			phoff,
		)

		p_type = ph[0]
		p_offset = ph[1]

		if p_type == PT_LOAD:
			first_load_offset = p_offset
			break

	# Make sure there is actually enough space to expand the PHDR table
	# in-place, and give up early if there is not.
	if new_phdr_end > first_load_offset:
		raise ValueError(
			"Cannot expand the PHDR table in-place: "
			f"Need through {new_phdr_end:#x}, "
			f"and the first PT_LOAD begins at {first_load_offset:#x}"
		)

	align = 0x1000
	vaddr_offset = patchVirtualLoad % align

	blob_offset = len(original)

	# Round the file end up while preserving the low bits of the virtual address.
	if blob_offset % align <= vaddr_offset:
		blob_offset += vaddr_offset - (blob_offset % align)
	else:
		blob_offset += align + vaddr_offset - (blob_offset % align)

	# Create the new Elf32_Phdr
	patch_phdr = struct.pack(
		ELF32_PHDR_FMT,
		PT_LOAD,					# p_type
		blob_offset,				# p_offset
		patchVirtualLoad,			# p_vaddr
		patchVirtualLoad,			# p_paddr
		len(patchCore),				# p_filesz
		len(patchCore),				# p_memsz
		PF_R | PF_W | PF_X,			# p_flags
		align,						# p_align
	)

	result = bytearray(original)
	struct.pack_into("<H", result, 0x2c, e_phnum + 1)

	# Write the additional PHDR immediately after the existing PHDR.
	result[old_phdr_end:new_phdr_end] = patch_phdr

	if len(result) < blob_offset:
		result.extend(b"\x00" * (blob_offset - len(result)))

	# Append the actual code/data.
	result.extend(patchCore)
	return bytes(result)

# The fun begins...
def main():
	global REGION
	REGION = sys.argv[1]

	# Open the ELF file so we can get at symbols in it.
	# Open the version-specific LD file so we can grab other symbols we need too
	patchElf = ElfWrapper(f'bin/rolling_hostfs_{REGION}.elf')
	ldScript = LdScript.new(f'../rollapi/ld/{REGION}.ld')

	# This trampoline is written to the start of main().
	MAIN_STATIC_TRAMPOLINE = Mips.j(patchElf.symbol('_start')) + Mips.nop()

	# Create the patch core.
	patchCore = patchElf.makeCore()
	patchCoreEntry = patchElf.symbol('BLOB_LOAD_ADDRESS')

	# Read the original ELF file into memory.
	with open(f'../../elf/rolling_{REGION}.elf', 'rb') as f:
		origCode = f.read()

	# Patch the ELF to inject the patch core as a new PT_LOAD header.
	# We do this outside of LIEF because LIEF can't do this in place, and it's
	# out-of-place moves sections, which is untenable.
	patched = patchInjectCore(origCode, patchCore, patchCoreEntry)

	# Once the patch core is injeted, we can use LIEF to perform the rest of our patches.
	rollingLief = lief.parse(patched)

	origEnd = rollingLief.get_int_from_virtual_address(ldScript.symbol('__HOOK_sbrk_break'), 4)
	adjustedEnd = alignUp(patchCoreEntry + len(patchCore), 0x1000)

	print(f'Original _end: 0x{origEnd:08x}, patch adjusted: 0x{adjustedEnd:08x}')

	def patchString(addr: int, newString: str):
		encoded = newString.encode('ascii') + b'\x00'
		rollingLief.patch_address(addr, list(encoded))
		print(f'Patched string at 0x{addr:08x}')

	def patchEraseString(addr: int):
		rollingLief.patch_address(addr, [0])
		print(f'Erased string at 0x{addr:08x}')

	# Patch the program break to be ahead of the core, so that memory allocations do not break us.
	# Once done, patch the start of main() to jump to the core entry point.
	rollingLief.patch_address(ldScript.symbol('__HOOK_sbrk_break'), adjustedEnd)
	rollingLief.patch_address(ldScript.symbol('main'), list(MAIN_STATIC_TRAMPOLINE))

	# Patch IOP module path strings for hostfs
	patchString(ldScript.symbol('ModulePath'), 'host0:modules/%s.irx')
	patchString(ldScript.symbol('ModuleRebootPackagePath'), 'host0:modules/ioprp255.img')

	# Patch music path format string and wipe ISO version suffix
	patchString(ldScript.symbol('MusicPath'), 'host0:music/%s')
	patchEraseString(ldScript.symbol('MusicCdSuffix'))

	# Wipe movie prefix and ISO version suffix
	patchEraseString(ldScript.symbol('MoviePrefix'))
	patchEraseString(ldScript.symbol('MovieCdSuffix'))

	# Write the patched ELF file to disk.
	rollingLief.write(f'../../elf/rolling_{REGION}_hostfs_patched.elf')

	# Make the xdelta patch.
	pyxdelta.run(f'../../elf/rolling_{REGION}.elf', f'../../elf/rolling_{REGION}_hostfs_patched.elf', f'../../rolling_{REGION}_hostfs_patch.xdelta')


if __name__ == '__main__':
	main()
