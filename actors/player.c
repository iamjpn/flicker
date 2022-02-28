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
updateplayer(struct actor *ap) {
	char t;
	int v;
	/* Set the velocities */

	if (ap->state & PALIVE) {
		if (buttons & JOY_I) {
			if (!(ap->state & PJUMPED)) {
				ap->state |= PJUMPED;
				playsound(SOUNDJUMP);
			}
			ap->vy = max(-0x40, ap->vy - 0x8);
		} else {
			ap->state &= ~PJUMPED;
			ap->vy = min(0x10, ap->vy + 0x8);
		}

		if (buttons & JOY_LEFT) {
			ap->vx = max(-0x40, ap->vx - 0x8);
			ap->flags |= FLAGSLEFT;
		} else if (buttons & JOY_RIGHT) {
			ap->vx = min(0x40, ap->vx + 0x8);
			ap->flags &= ~FLAGSLEFT;
		} else if (ap->vx < 0)
			ap->vx = min(0, ap->vx + 0x08);
		else if (ap->vx > 0)
			ap->vx = max(ap->vx - 0x08, 0);

		t = ymove(ap);

		if (ap->vy > 0 && t == GROUNDT) {
			if (ap->vx)
				ap->animation = playerwalkanimation;
			else
				ap->animation = 0;
		} else {
			ap->animation = playerflyanimation;
		}

		xmove(ap);
		if (buttons & JOY_II)
			createbullet(ap);
	}

	if (ap->state & PHURT) {
		playnoise(0x100);
		ap->animation = playerhurtanimation;
		if (ap->flags & FLAGSCYCLED)
			ap->state &= ~PHURT;
	}

	if (ap->state & PDEAD) {
		playnoise(0x100);
		ap->animation = playerdeadanimation;
		if (ap->flags & FLAGSCYCLED) {
			gameover = 1;
		}
	}
}

void
initplayer(struct actor *ap) {
	ap->id = PLAYERID;
	ap->parent = NULL;
	ap->x = 0;
	ap->y = 0;
	ap->state = PALIVE;
	ap->h = 32;
	ap->w = 32;
	ap->vx = 0;
	ap->vy = 0;
	ap->pal = 2;
	ap->pattern = (playerspr - playerspr) >> 6;
	ap->flags = 0;
	ap->frame = 0;
	ap->framei = 0;
	ap->animation = 0;
	ap->wpad = INTTOFIX(26);
	ap->hpad = INTTOFIX(16);
}
