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
updateheartcontroller(struct controller *cp) {
	int i;
	struct actor *ap;
	if (cp->nchildren == nlives)
		return;

	cp->nchildren = nlives;
	ap = (struct actor *)cp->childlist;

	while (ap) {
		ap->status = STATUSFREE;
		ap = ap->next;
	}

	cp->childlist = NULL;
	for (i = 0; i < nlives; i++) {
		ap = createghost();

		ap->id = HEARTID;
		ap->parent = cp;
		ap->hpad = 0;
		ap->x = INTTOFIX((11 + i) * 16);
		ap->y = 0;
		ap->h = 16;
		ap->w = 16;
		ap->vx = 0;
		ap->vy = 0;
		ap->pal = 2;
		ap->pattern = (heartspr - playerspr) >> 6;
		ap->flags = 0;
		ap->animation = NULL;
		ap->framei = 0;
		ap->frame = 0;

		ap->next = (struct actor *)cp->childlist;

		cp->childlist = (char *)ap;
	}
}