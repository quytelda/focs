/* array.h - C Array API
 * Copyright (C) 2018 Quytelda Kahja
 *
 * This file is part of focs.
 *
 * focs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * focs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with focs.  If not, see <http://www.gnu.org/licenses/>.
 */

 /**
  * Determine the number of elements stored in a C array.
  * @param array The array to check (must be statically sized)
  *
  * @return This function shall return the number of elements in `array`.  This
  * represents the total capacity of `array`, not how many values are currently
  * stored or initialized.
  */
#define array_size(array) (sizeof(array) / sizeof(array[0]))

/**
 * Advance through an array element by element with a counter.
 * @param array   The ring buffer to iterate over
 * @param current A pointer that will point to the current element
 *
 * array_foreach_i() should be used like a for loop; for example:
 * ```
 * size_t i;
 * struct data_type * current;
 * array_foreach_i(array, current, i) {
 *         if(some_condition(i)) {
 *                 do_something(current);
 *         }
 * }
 * ```
 * The loop will work whether `current` is a void pointer or a pointer to a type
 * with the same size as (or theoretically less than) the array's elements.
 */
#define array_foreach_i(array, current, i) \
	for(i = 0, current = array[0];     \
	    i < array_size(array);         \
	    i++, current = array[i])

/**
 * Advance through an array element by element.
 * @param array   The array to iterate over
 * @param current A pointer that will point to the current element
 *
 * array_foreach() should be used like a for loop; for example:
 * ```
 * struct data_type * current;
 * array_foreach(array, current) {
 *         do_something(current);
 * }
 * ```
 * The loop will work whether `current` is a void pointer or a pointer to a type
 * with the same size as (or theoretically less than) the array's elements.
 */
#define array_foreach(array, current)       \
	size_t _i;                          \
	array_foreach_i(array, current, _i)
