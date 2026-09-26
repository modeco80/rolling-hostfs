#!/bin/env python3

import tempfile
import subprocess

from elftools.elf.elffile import ELFFile
from elftools.elf.sections import Section, SymbolTableSection

GCC_PATH = '../../tools/cc/2.96-ee-001003-1/'

__all__ = [ 'ElfWrapper' ]

def alignBy(data: bytes, alignment: int) -> bytes:
	current_length = len(data)
	padding_needed = (alignment - (current_length % alignment)) % alignment
	padded_data = data + b'\x00' * padding_needed
	return padded_data

# Class which manages ELF file reading, as a slightly
# higher-level wrapper over the pyelftools library.
class ElfWrapper:
	_elfFile: ELFFile
	_symbolTableSection: SymbolTableSection
	_textSection: Section

	def __init__(self, elfFileName: str):
		self._elfFileName = elfFileName
		self._elfFile = ELFFile(open(elfFileName, 'rb'))
		self._symbolTableSection = self._elfFile.get_section_by_name('.symtab')
		self._textSection = self._elfFile.get_section_by_name('.text')
		if not self._symbolTableSection:
			raise ValueError('ELF does not have a symbol table? WTF')
		if not isinstance(self._symbolTableSection, SymbolTableSection):
			raise ValueError('not a symbol table section? What?')

	# Returns the address (st_value) of a symbol in the ELF.
	def symbol(self, symbolName: str) -> int:
		# FIXME: better algorithm
		for symbol in self._symbolTableSection.iter_symbols():
			if symbol.name == symbolName:
				return symbol['st_value']

	# Makes a program core.
	def makeCore(self):
		data = None
		# I don't love this, but... hey, it works..
		with tempfile.NamedTemporaryFile(prefix="mkpnach") as tmpFile:
			result = subprocess.run(f'{GCC_PATH}/bin/ee-objcopy -O binary {self._elfFileName} {tmpFile.name}', shell=True, check=True)
			data = tmpFile.read()
		return data



