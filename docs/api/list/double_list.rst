===================
Doubly Linked Lists
===================

To use the doubly linked list implementation, create an instance of the ``double_list`` type using ``dl_create()``.  You can then use the data management functions to add and remove elements from the list.  You can also use the functional data operations to transform, reduce, or filter the data in the list.

.. literalinclude:: ../../examples/revabs_dll.c

Creation and Destruction
------------------------
.. doxygenfunction:: dl_create
.. doxygenfunction:: dl_destroy

Data Management
---------------
.. doxygenfunction:: dl_empty
.. doxygenfunction:: dl_size
.. doxygenfunction:: dl_push_head
.. doxygenfunction:: dl_push_tail
.. doxygenfunction:: dl_pop_head
.. doxygenfunction:: dl_pop_tail
.. doxygenfunction:: dl_elem
.. doxygenfunction:: dl_insert
.. doxygenfunction:: dl_delete
.. doxygenfunction:: dl_remove
.. doxygenfunction:: dl_fetch

Higher Order Functions
----------------------
.. doxygenfunction:: dl_map
.. doxygenfunction:: dl_foldr
.. doxygenfunction:: dl_foldl
.. doxygenfunction:: dl_any
.. doxygenfunction:: dl_all
.. doxygenfunction:: dl_filter
.. doxygenfunction:: dl_drop_while
.. doxygenfunction:: dl_take_while

Debugging
---------
.. doxygenfunction:: dl_dump
