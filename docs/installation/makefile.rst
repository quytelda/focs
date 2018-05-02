.. highlight :: sh

==============================
Building FOCS Using a Makefile
==============================

FOCS can be built using the ``make`` utility, which will generate the binary ``libfocs.so``::

  $ make # run from the project's top-level directory
  ...
  gcc -shared -o libfocs.so -std=c99 -I include -fpic -Wall -pedantic ...

To install this library, simply run ``make install`` as root (or ``sudo make install`` as a regular user). This will copy the library to ``/usr/lib`` and update the linker cache; it will also install the neccessary headers to ``/usr/include``. To change the installation prefix, which is by default ``/usr``, you can optionally set the environment variable ``PREFIX``. If you need to uninstall the library, you can use the same steps running ``make uninstall``::

  user@host$ sudo make install
  install --owner=0 --group=0 --mode=644 libfocs.so  /usr/lib
  ldconfig
  cp include/focs.h  /usr/include
  cp -R include/focs  /usr/include
  cp -R include/list  /usr/include
  cp -R include/sync  /usr/include

  user@host$ sudo make uninstall
  rm -f  /usr/lib/libfocs.so
  ldconfig
  rm -f  /usr/include/focs.h
  rm -rf  /usr/include/focs
  rm -rf  /usr/include/list
  rm -rf  /usr/include/sync

or, alternatively to install to ``/usr/local``::

  user@host$ su
  Password:
  root@host$ PREFIX=/usr/local make install
  install --owner=0 --group=0 --mode=644 libfocs.so  /usr/local/lib
  ldconfig
  cp include/focs.h  /usr/local/include
  cp -R include/focs  /usr/local/include
  cp -R include/list  /usr/local/include
  cp -R include/sync  /usr/local/include
  
  root@host$ PREFIX=/usr/local make uninstall
  rm -f  /usr/local/lib/libfocs.so
  ldconfig
  rm -f  /usr/local/include/focs.h
  rm -rf  /usr/local/include/focs
  rm -rf  /usr/local/include/list
  rm -rf  /usr/local/include/sync
