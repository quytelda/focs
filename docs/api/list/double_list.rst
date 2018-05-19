===================
Doubly Linked Lists
===================

To use the doubly linked list implementation, create a pointer to a ``struct double_list``, and initialize it using ``dl_alloc()``.  You can then use the data management functions to add and remove elements from the list.  You can also use the functional data operations to transform, reduce, or filter the data in the list.

.. literalinclude:: ../../examples/revabs_dll.c

.. doxygenstruct:: double_list

Creation and Destruction
------------------------
.. doxygenfunction:: dl_alloc
.. doxygenfunction:: dl_destroy

Data Management
---------------
.. doxygenfunction:: dl_push_head
.. doxygenfunction:: dl_push_tail
.. doxygenfunction:: dl_pop_head
.. doxygenfunction:: dl_pop_tail
.. doxygenfunction:: dl_insert
.. doxygenfunction:: dl_delete
.. doxygenfunction:: dl_remove
.. doxygenfunction:: dl_fetch

Functional Utilities
--------------------
.. doxygenfunction:: dl_empty
.. doxygenfunction:: dl_elem
.. doxygenfunction:: dl_map
.. doxygenfunction:: dl_foldr
.. doxygenfunction:: dl_foldl
.. doxygenfunction:: dl_any
.. doxygenfunction:: dl_all
.. doxygenfunction:: dl_filter
.. doxygenfunction:: dl_drop_while
.. doxygenfunction:: dl_take_while
