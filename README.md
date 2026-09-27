# Rolling (2003) HostFS Patch

This is a HostFS patch for Rolling. It allows you to run the game on HostFS, but it also allows you to run the game with an unpacked WAD. This makes modding much easier.

# Usage

Head over to [Releases](https://github.com/modeco80/rolling-hostfs/releases) and grab the latest xdelta patch.

Use your xdelta patcher of choice to patch the original ELF from the game ISO.

Extract the WAD. There are a couple options for this ([the original kind of garbage python extractor I wrote a few years ago](https://gist.github.com/modeco80/85daf2f20624eccbdd0f6f64517237c9), and a rewrite I am working on in C++), but it doesn't matter what you use as long as the WAD is extracted correctly.

Place the patched ELF file where you extracted the WAD.

Copy the `PMOVIES`, `MUSIC`, and `MODULES` from the original ISO image to where you extracted the WAD.

Launch PCSX2, and add the extracted WAD directory to search paths. Make sure to enable the Host Filesystem in PCSX2 as well.

Launch the ELF from the PCSX2 game list.

...

Profit?

# Building

You need:

- Python
- Pipenv
- ee-gcc `2.96-ee-001003-1` (place it in `tools/cc/2.96-ee-001003-1`)
- The original Rolling ELF

Make an `elf` directory in the root of the repository, and copy the original Rolling ELF to `elf/rolling_pal.elf`.

Install python dependencies and enter the pipenv managed virtualenv:
```
pipenv install
pipenv shell
```

Build the patch. It will be pressent at the root of the repository as `rolling_pal_hostfs_patch.xdelta`.

```
make
```


