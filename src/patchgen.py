#!/bin/env python3
import mkpatch
import pyxdelta
import sys
import os
from mkpatch.patch import PatchStep, PatchContext

class StringPatches(PatchStep):
	def runStep(self, ctx: PatchContext):
		ctx.patchString(ctx.ldScript.symbol('ModulePath'), 'host0:modules/%s.irx')
		ctx.patchString(ctx.ldScript.symbol('ModuleRebootPackagePath'), 'host0:modules/ioprp255.img')

		# Patch music path format string and wipe ISO version suffix
		ctx.patchString(ctx.ldScript.symbol('MusicPath'), 'host0:music/%s')
		ctx.patchEraseString(ctx.ldScript.symbol('MusicCdSuffix'))

		# Wipe movie prefix and ISO version suffix
		ctx.patchEraseString(ctx.ldScript.symbol('MoviePrefix'))
		ctx.patchEraseString(ctx.ldScript.symbol('MovieCdSuffix'))

REGION = sys.argv[1]

ORIGINAL_ELF_PATH = f'../elf/rolling_{REGION}.elf'
PATCHED_ELF_PATH = f'../elf/rolling_{REGION}_hostfs_patched.elf'
XDELTA_PATCH_PATH = sys.argv[2] # Provided by build system now :)

def main():
	# set up mkpatch
	args = mkpatch.Arguments()
	args.patchInjectKind = mkpatch.PatchInjectCoreKind.INJECT_NEW_PHDR
	args.patchElfPath = f'bin/rolling_hostfs_{REGION}.elf'
	args.originalElfPath = ORIGINAL_ELF_PATH
	args.apiLdScriptPath = f'../rollapi/ld/{REGION}.ld'

	# Add steps and run it!
	args.patchSteps.append(StringPatches())
	binary = mkpatch.main(args)

	# Write the patched ELF file and make the xdelta patch.
	binary.write(PATCHED_ELF_PATH)
	pyxdelta.run(ORIGINAL_ELF_PATH, PATCHED_ELF_PATH, XDELTA_PATCH_PATH)

if __name__ == "__main__":
	main()
