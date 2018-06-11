# Helloworld

Hello World! example for Nintendo 64, using NuSystem.

## Build

Requires MS Windows XP, Nintendo 64 SDK 5.0.

```
set ROOT=c:\ultra
set gccdir=c:\ultra\gcc
set PATH=c:\ultra\gcc\mipse\bin;c:\ultra\usr\sbin
set gccsw=-mips3 -mgp32 -mfp32 -funsigned-char -D_LANGUAGE_C -D_ULTRA64 -D__EXTENSIONS__
set n64align=on
set GCC_CELF=ON
make
```