.. FOCS coding style guide
   Copyright (C) 2018 Quytelda Kahja

   This file is part of focs.

   focs is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   focs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with focs.  If not, see <http://www.gnu.org/licenses/>.

=======================
Coding Style Guidelines
=======================

The coding style guidelines for FOCS are very similar to the coding style guidelines for the Linux Kernel, with a few notable exceptions.  If you cannot find the information you are looking for in this document, try checking the Linux Kernel Coding Style Guidelines at https://www.kernel.org/doc/html/v4.16/process/coding-style.html, and use those guidelines unless otherwise specified.

Indentation
-----------
FOCS uses tabs for indentation, with a tab width of 8 characters.  8 characters may seem like a lot, but it is helpful in highlighting overly complex code - if your code starts to wrap way too far to the right, it should probably be refactored.  A good guideline is that your code shouldn't normally surpass 3 levels of indentation.

Alignment
---------
Because alignment doesn't always work with just tabs, stick to the rule "use tabs for indentation and spaces for alignment."  This means that when you are working at indentation level 2 (2 tabs), and you need to align further to the right, add two tabs, then continue to use spaces to line up the code as necessary.

Line Length
-----------
To prevent code from becoming overly complex, line length is restricted to 80 characters.  Generally, older terminal devices and most terminal emulators have a default line length of 80 characters, so lines longer than this will become wrapped or disappear off the end of the screen, which is ugly and unhelpful.  If your code has lines longer than 80 characters, you should refactor it by renaming functions and variables with shorter names, reducing function chaining, or breaking up calculation into multiple steps.

Sometimes there will be cases where lines over the character limit cannot be shortened without dramatically hurting the readability or utility of the code; in this case, readability and utility take precedance, and the line will just have to remain too long.

Naming
------
FOCS uses snake case for all symbol names.  Camel case is shorter, but more difficult to read.

Try to give symbols names that are both as descriptive and succint as possible.  The use of common variable naming conventions is encouraged with the following exceptions:

Hungarian Notation
^^^^^^^^^^^^^^^^^^
C is a strongly typed language.  You should be able to figure out what the types of your variables are without inserting it into the name.

"_t" Notation
^^^^^^^^^^^^^
It is sometimes common practice in C programs to name typedef'd structs with names ending in "_t".  Do not typedef structs in FOCS, and do not use the "_t" affix on struct names.  For primitive data types, it is not recommended that you use typedefs, but if need compells you, the "_t" suffix is acceptable to remain consistent with ISO C99 standard library types like `uint8_t`, `size_t`, etc.  If you use a typedef for function pointers, the typedef should end in either "_fn" (for abstract function types) or "_dop" for data structure specific operations.

Function Structure
------------------
Functions should be structured using the following template (optional keywords and symbols are in square brackets ``[]``)::

  [static] [inline] <return_type> function_name(<arg1_type> <arg1>,
                                                <arg2_type> <arg2>)
  {
          success = do_something(...);
	  if(!success)
	          goto exit;

	  do_something_else(...);

  [exit:]
	  return [value];
  }

Note that the opening curly brace (``{``) for a function definition occurs on the line *after* the function signature.  This is different from how FOCS handles other statements that use curly braces, but it makes the function signature easier to separate from the implementation.

``goto`` statements are useful to provide common exit sections for functions that might terminate in several different ways, such as under error conditions.  This prevents the programmer from having to copy past function cleanup and exit routines to every exit branch in the function.  However, the use of a goto label is optional, and it shouldn't be used if the function is not complex enough to require it.  If a common exit label is needed, it should be named ``exit``.  Other ``goto`` labels used to provide common exit points which preceed ``exit`` should be name ``exit_<something else>``.

Don't write functions on one line like so::

  int myfunction(int a) { return (a + 1); }

If your function looks like this, it would be better off as a macro, probably.

Conditionals
------------

Conditionals statements should be written with first curly brace on the same line as the ``if``, ``else if`` or ``else`` statement they are associated with.  ``else`` or ``else if`` statements should appear on the same line as the closing curly brace of the previous block, like so::

  if(<condition>) {
          do_this(...);
	  do_that(...);
  } else if(<condition>) {
          do_this(...);
  } else {
          do_that(...);
  }

Note that there should be a space between curly braces and the statements the preceed/follow, but there is no space between the keyword ``if`` and it's conditional.  If all the branches in the conditional statement are single lines, the curly braces may be ommited, like so::

  if(<condition>)
          func_one(...);
  else if(<condition>)
          func_two(...);
  else
          func_three(...);

This saves spaces and increases readability.
