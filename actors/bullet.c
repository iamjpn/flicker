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
createbullet(struct actor *a) {
	struct actor *bp;

	if (bulletcontroller->nchildren)
		return;

	bulletcontroller->nchildren++;
	bp = createactor();
	bp->id = BULLETID;
	bp->h = 16;
	bp->w = 16;
	bp->parent = bulletcontroller;
	if (a->flags & FLAGSLEFT) {
		bp->x = a->x - INTTOFIX(bp->w) + a->wpad;
		bp->vx = -INTTOFIX(4);
	} else {
		bp->x = a->x + INTTOFIX(a->w) - a->wpad;
		bp->vx = INTTOFIX(4);
	}
	bp->y = a->y + INTTOFIX(8);
	bp->vy = 0;
	bp->pal = 1;
	/* 80 */
	bp->pattern = (bulletspr - playerspr) >> 6;
	bp->flags = a->flags & FLAGSLEFT;
	bp->framei = 0;
	bp->frame = 0;
	bp->animation = bulletanimation;
	bp->wpad = 0;
	bp->hpad = 0;

	if (lcollision(bp) != NILT || rcollision(bp) != NILT) {
		bp->status = STATUSFREE;
		bp->parent->nchildren--;
	}
	playsound(SOUNDSHOOT);
}

void
updatebullet(struct actor *a) {
	char t;
	t = xmove(a);
	if (t != NILT) {
		playnoise(0x03);
		a->id = DEADID;
		a->animation = bulletdeadanimation;
	}
}
