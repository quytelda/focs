============
Ring Buffers
============

To use the ring buffer implementation, create an instance of the ring_buffer type, and initialize it using ``rb_create()``.  You can then use the data management functions to add and remove data blocks from the buffer.  You can also use the functional data operations to transform, reduce, or filter the data in the buffer as if it were a list.

Creation and Destruction
------------------------
.. doxygenfunction:: rb_create
.. doxygenfunction:: rb_destroy

Data Management
---------------
.. doxygenfunction:: rb_size
.. doxygenfunction:: rb_push_head
.. doxygenfunction:: rb_push_tail
.. doxygenfunction:: rb_pop_head
.. doxygenfunction:: rb_pop_tail
.. doxygenfunction:: rb_insert
.. doxygenfunction:: rb_delete
.. doxygenfunction:: rb_remove
.. doxygenfunction:: rb_fetch

Functional Utilities
--------------------
.. doxygenfunction:: rb_empty
.. doxygenfunction:: rb_contains
.. doxygenfunction:: rb_map
.. doxygenfunction:: rb_foldr
.. doxygenfunction:: rb_foldl
.. doxygenfunction:: rb_any
.. doxygenfunction:: rb_all
.. doxygenfunction:: rb_filter
.. doxygenfunction:: rb_drop_while
.. doxygenfunction:: rb_take_while
