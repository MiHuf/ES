/*
 * test.c
 *
 * Copyright 2015 Michael Hufschmidt <michael@Burrone_Linux_2>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */


#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv) {
  uint32_t x = 49154;
  uint32_t y = 0xC002;
  uint32_t z = 0b010 | 0b10 << 13 | 0b1 << 15;
  printf("x = %8d = %8x  \r\n", x, x);
  printf("y = %8d = %8x  \r\n", y, y);
  printf("z = %8d = %8x  \r\n", z, z);
	return 0;
}

