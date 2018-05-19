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
