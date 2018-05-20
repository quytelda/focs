===================
Singly Linked Lists
===================

To use the singly linked list implementation, create an instance of the ``single_list`` type using ``sl_create()``.  You can then use the data management functions to add and remove elements from the list.  You can also use the functional data operations to transform, reduce, or filter the data in the list.

.. literalinclude:: ../../examples/revabs_sll.c

.. doxygenstruct:: single_list

Creation and Destruction
------------------------
.. doxygenfunction:: sl_create
.. doxygenfunction:: sl_destroy

Data Management
---------------
.. doxygenfunction:: sl_empty
.. doxygenfunction:: sl_push_head
.. doxygenfunction:: sl_push_tail
.. doxygenfunction:: sl_pop_head
.. doxygenfunction:: sl_pop_tail
.. doxygenfunction:: sl_elem
.. doxygenfunction:: sl_insert
.. doxygenfunction:: sl_delete
.. doxygenfunction:: sl_remove
.. doxygenfunction:: sl_fetch

Higher Order Functions
----------------------
.. doxygenfunction:: sl_map
.. doxygenfunction:: sl_foldr
.. doxygenfunction:: sl_foldl
.. doxygenfunction:: sl_any
.. doxygenfunction:: sl_all
.. doxygenfunction:: sl_filter
.. doxygenfunction:: sl_drop_while
.. doxygenfunction:: sl_take_while

Debugging
---------
.. doxygenfunction:: sl_dump
