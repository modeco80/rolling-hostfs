
import regex as re
from typing import TypedDict

comment_regex = re.compile(r'\/\*[\s\S]*?\*\/')

__all__ = [ 'LdScript' ]

# Hilariously simple .ld script parser. Only handles comments, and
# symbol (X = 0xY;) declarations. Does not support variables or
# complicated expressions.
class LdScript:
	_symbolMap: dict[str, int]

	def __init__(self):
		self._symbolMap = dict()

	# Opens and parses
	@staticmethod
	def new(path: str) -> LdScript:
		script = LdScript()
		script.parse(path)
		return script

	def parse(self, path: str):
		with open(path, 'r') as ldFile:
			self.parseString(ldFile.read())

	def parseString(self, ldScript: str):
		for line in ldScript.splitlines():
			stripped = line.strip();
			if len(stripped) == 0:
				continue
			if comment_regex.match(stripped) is not None:
				continue
			if not stripped.endswith(';'):
				raise ValueError('INVALID LINE')
			stripped = stripped.replace(' ', '')[:-1]

			items = stripped.split('=')

			if items[0] in self._symbolMap:
				raise ValueError(f'Invalid ld file: symbol {items[0]} is listed twice')

			# Add item to list
			self._symbolMap[items[0]] = int(items[1], 0)

	def symbol(self, name: str) -> int|None:
		if not name in self._symbolMap:
			return None
		return self._symbolMap[name]

