# focs
Functional Open C Structures - C Data Structures Library

## What is FOCS?
FOCS is a data structure library implemented in C, designed to provide common data structures to C programs.  Additionally, FOCS aims to provide a functional programming bent to these data structures providing common functional utilities like `map` and `foldr/foldl` that can be used with any of these data structures.

## Obtaining FOCS
FOCS is very new and still under heavy development; there have not yet been any versioned releases.  To obtain a copy of focs, either clone or download the master branch of the focs repository (located at [https://github.com/quytelda/focs.git]).  For example:
```
$ git clone https://github.com/quytelda/focs.git
Cloning into 'focs'...
...
$ cd focs
```

FOCS can be built using the `make` utility, which will generate the binary `libfocs.so`.
```
$ make # run in the top-level repository directory
gcc -c -std=c99 -I include -fpic -Wall -pedantic -lpthread  -o src/list/double_list.o src/list/double_list.c
gcc -c -std=c99 -I include -fpic -Wall -pedantic -lpthread  -o src/list/ring_buffer.o src/list/ring_buffer.c
gcc -c -std=c99 -I include -fpic -Wall -pedantic -lpthread  -o src/sync/rwlock.o src/sync/rwlock.c
...
gcc -shared -o libfocs.so -std=c99 -I include -fpic -Wall -pedantic src/list/double_list.o src/list/ring_buffer.o src/sync/rwlock.o
$ ls
global.mk  include  libfocs.so  Makefile  src  test
```

To install this library, simply run `make install` as root (or `sudo make install` as a regular user).  This will copy the library to `/usr/lib` and update the linker cache; it will also install the neccessary headers to `/usr/include`.  To change the installation prefix, which is by default `/usr`, you can optionally set the environment variable `PREFIX`.  If you need to uninstall the library, you can use the same steps running `make uninstall`:
```
$ sudo make install
install --owner=0 --group=0 --mode=644 libfocs.so  /usr/lib
ldconfig
cp include/focs.h  /usr/include
cp include/metadata.h  /usr/include
cp -R include/list  /usr/include
cp -R include/sync  /usr/include

$ sudo make uninstall
rm -f  /usr/lib/libfocs.so
ldconfig
rm -f  /usr/include/focs.h
rm -f  /usr/include/metadata.h
rm -rf  /usr/include/list
rm -rf  /usr/include/sync
```
or, alternatively:
```
# PREFIX=/usr/local make install
install --owner=0 --group=0 --mode=644 libfocs.so  /usr/local/lib
ldconfig
cp include/focs.h  /usr/local/include
cp include/metadata.h  /usr/local/include
cp -R include/list  /usr/local/include
cp -R include/sync  /usr/local/include

# PREFIX=/usr/local make uninstall
rm -f  /usr/local/lib/libfocs.so
ldconfig
rm -f  /usr/local/include/focs.h
rm -f  /usr/local/include/metadata.h
rm -rf  /usr/local/include/list
rm -rf  /usr/local/include/sync
```
