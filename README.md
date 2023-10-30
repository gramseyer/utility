# Collection of utility classes and a wrapper around lmdb

The easiest way to add this to a project is as a git submodule, including
into the project the Makefile.am.fragment.

One can also build this as a library -- the autoconf code to do so exists,
but this is not recommended.

## Notes:

The AX_PKGCONFIG_SUBDIR macro doesn't support 
statically linkable libraries.  
You may need to add lmdb as a linker flag (i.e. -llmdb -L location) to wherever lmdb is installed, when building something
that uses libutility.a but does not use lmdb.


