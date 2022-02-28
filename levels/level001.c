/*
    Forest Flicker
    Copyright (C) 2022  Jordan Niethe

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "huc.h"
#include "flicker.h"
#include "proto.h"

#inctile_ex(level001tiles, "sprites/backgroundtiles.pcx", 0, 0, 6, 1, 0);

#incbin(level001map, "maps/level001.FMP");

#incpal(level001pal, "sprites/backgroundtiles.pcx");

#include "levels/level001collisions.h"

/*
	id,
	max,
	ncords
	coords
*/
// clang-format off
const char level001fixtures[] = {
	BOMBID,
	3,
	2,
	1, 3,
	14, 3,
	FRUITID,
	1,
	3,
	4, 8, 
	11, 5, 
	12, 12, 
	BULLETID,
	1,
	1,
	0, 0,
	HEARTID,
	0,
	1,
	0, 0,
	NOID
};
// clang-format on

/*
	id,
	coords
*/
// clang-format off
const char level001actors[] = {
	PLAYERID,
	2,
	11,
	NOID;
};
// clang-format on
