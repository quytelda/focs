============
Ring Buffers
============

To use the ring buffer implementation, create an instance of the ``ring_buffer`` type using ``rb_create()``.  You can then use the data management functions to add and remove data blocks from the buffer.  You can also use the functional data operations to transform, reduce, or filter the data in the buffer as if it were a list.

Creation and Destruction
------------------------
.. doxygenfunction:: rb_create
.. doxygenfunction:: rb_destroy

Data Management
---------------
.. doxygenfunction:: rb_empty
.. doxygenfunction:: rb_full
.. doxygenfunction:: rb_size
.. doxygenfunction:: rb_pop_head
.. doxygenfunction:: rb_pop_tail
.. doxygenfunction:: rb_push_head
.. doxygenfunction:: rb_push_tail
.. doxygenfunction:: rb_elem
.. doxygenfunction:: rb_insert
.. doxygenfunction:: rb_fetch
.. doxygenfunction:: rb_delete
.. doxygenfunction:: rb_remove
.. doxygenfunction:: rb_reverse

Iterator Macros
---------------
.. doxygendefine:: ring_buffer_foreach_i
.. doxygendefine:: ring_buffer_foreach
.. doxygendefine:: ring_buffer_foreach_i_rev
.. doxygendefine:: ring_buffer_foreach_rev

Higher Order Functions
----------------------
.. doxygenfunction:: rb_map
.. doxygenfunction:: rb_foldr
.. doxygenfunction:: rb_foldl
.. doxygenfunction:: rb_any
.. doxygenfunction:: rb_all
.. doxygenfunction:: rb_filter
.. doxygenfunction:: rb_drop_while
.. doxygenfunction:: rb_take_while

Debugging
---------
.. doxygenfunction:: rb_dump
