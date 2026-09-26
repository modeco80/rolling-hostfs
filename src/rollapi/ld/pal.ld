/* rolling symbols */

/* C library */
strlen = 0x002e1bb0;
memcpy = 0x002df89c;
memset = 0x002dfa50;
sprintf = 0x002e1378;
vsprintf = 0x002e5a90;

malloc = 0x002ded20;
free = 0x002ded48;


/* Hook specific things */
__HOOK_sbrk_break = 0x0048d03c;

/* original value of the Rolling ELF's _end symbol, as captured in the program break adress */
__HOOK_ORIG_END = 0x004fc02c;


/* The fun stuff */

main = 0x00146c80;
mainTrampoline = 0x00146c88;


/* Wad */

Wad_fopen = 0x001416a8;
