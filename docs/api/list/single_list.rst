===================
Singly Linked Lists
===================

To use the doubly linked list implementation, create a pointer to a ``struct single_list``, and initialize it using ``sl_alloc()``.  You can then use the data management functions to add and remove elements from the list.  You can also use the functional data operations to transform, reduce, or filter the data in the list.

.. literalinclude:: ../../examples/revabs_sll.c

.. doxygenstruct:: single_list

Creation and Destruction
------------------------
.. doxygenfunction:: sl_alloc
.. doxygenfunction:: sl_destroy

Data Management
---------------
.. doxygenfunction:: sl_push_head
.. doxygenfunction:: sl_push_tail
.. doxygenfunction:: sl_pop_head
.. doxygenfunction:: sl_pop_tail
.. doxygenfunction:: sl_insert
.. doxygenfunction:: sl_delete
.. doxygenfunction:: sl_remove
.. doxygenfunction:: sl_fetch

Functional Utilities
--------------------
.. doxygenfunction:: sl_empty
.. doxygenfunction:: sl_elem
.. doxygenfunction:: sl_map
.. doxygenfunction:: sl_foldr
.. doxygenfunction:: sl_foldl
.. doxygenfunction:: sl_any
.. doxygenfunction:: sl_all
.. doxygenfunction:: sl_filter
.. doxygenfunction:: sl_drop_while
.. doxygenfunction:: sl_take_while
