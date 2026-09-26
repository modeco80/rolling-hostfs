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

/* PS2 SDK */

/* FIO */
sceOpen = 0x002bb748;
sceClose = 0x002bb9d0;
sceRead = 0x002bbd88;
sceLSeek = 0x002bbb50;


/* Rolling */

main = 0x00146c80;
mainTrampoline = 0x00146c88;


/* Wad manager */

Wad_InstallFileSystem = 0x001404d0;
Wad_Mount = 0x001407c8;
Wad_Unmount = 0x01407f0;

Wad_fopen = 0x001416a8;
Wad_fclose = 0x00141760;
Wad_fexist = 0x00141680;
Wad_fflush = 0x00141918;

Wad_fseek = 0x00141800;

Wad_fgets = 0x0013f200;
Wad_fputs = 0x001418d8;

Wad_fread = 0x00141798;

Wad_ReadAll = 0x0013f950;
Wad_ReadAllInto = 0x00140830;

/* Wad manager extern variables */

Wad_ReadAllFileSize = 0x004aeaa0;
Wad_ReadCount = 0x004aeaac;
