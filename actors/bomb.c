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
updatebomb(struct actor *a) {
	char t;

	if (a->state & BALIVE) {
		t = xmove(a);
		if (t != NILT)
			a->vx *= -1;
		t = ymove(a);
		if (t != NILT)
			a->vy *= -1;
	}
	if (a->vx < 0)
		a->flags |= FLAGSLEFT;
	else
		a->flags &= ~FLAGSLEFT;
	if (a->state & BDEAD) {
		playnoise(0x5);
		a->id = DEADID;
		a->animation = bombdeadanimation;
	}
}
