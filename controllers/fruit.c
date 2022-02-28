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

void
updatefruitcontroller(struct controller *cp) {
	struct actor *ap;

	if (cp->nchildren < cp->maxchildren && cp->timer == 0) {
		ap = createactor();
		ap->id = FRUITID;
		ap->parent = cp;
		ap->hpad = 8;
		ap->wpad = 8;
		ap->x = INTTOFIX(cp->coordlist[cp->i * 2] * 16);
		ap->y = INTTOFIX(cp->coordlist[cp->i * 2 + 1] * 16);
		ap->h = 16;
		ap->w = 16;
		ap->vx = 0;
		ap->vy = 0;
		ap->pal = 1;
		ap->pattern = (fruitspr - playerspr) >> 6;
		ap->flags = 0;
		ap->animation = fruitanimation;
		ap->framei = 0;
		ap->frame = 0;

		cp->nchildren++;
		cp->timer = 50;
		cp->i = (cp->i + 1) % cp->ncoords;
	}
}
