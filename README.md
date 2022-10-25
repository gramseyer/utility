# Collection of utility classes and a wrapper around lmdb

## Notes:

The AX_PKGCONFIG_SUBDIR macro doesn't support 
statically linkable libraries.  You may need to add lmdb as a linker flag (i.e. -llmdb -L location) to wherever lmdb is installed, when building something
that uses libutility.a but does not use lmdb.
For some reason, building with -O3 -flto seems to build a library that is statically linkable.


