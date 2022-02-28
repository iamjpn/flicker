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

char *collisions;

int
ind(int x, int y) {
	return ((y * MAPW) + x);
}

char
ucollision(struct actor *ap) {
	int y, x;

	y = (FIXTOINT(ap->y) - 1) / 16;
	if (ap->w == 32)
		x = (FIXTOINT(ap->x) + 16) / 16;
	else
		x = FIXTOINT(ap->x) / 16;

	if (collisions[ind(x, y)] != NILT)
		return collisions[ind(x, y)];
	x = (FIXTOINT(ap->x) + 16 - 2) / 16;
	if (collisions[ind(x, y)] != NILT)
		return collisions[ind(x, y)];
	if (ap->w == 32) {
		x = (FIXTOINT(ap->x) + 16) / 16;
		if (collisions[ind(x, y)] != NILT)
			return collisions[ind(x, y)];

		x = (FIXTOINT(ap->x) + 32 - 16 - 2) / 16;
		if (collisions[ind(x, y)] != NILT)
			return collisions[ind(x, y)];
	}

	return NILT;
}

char
dcollision(struct actor *ap) {
	int y, x;

	y = (FIXTOINT(ap->y) + ap->h) / 16;
	if (ap->w == 32)
		x = (FIXTOINT(ap->x) + 16) / 16;
	else
		x = FIXTOINT(ap->x) / 16;

	if (collisions[ind(x, y)] != NILT)
		return collisions[ind(x, y)];

	x = (FIXTOINT(ap->x) + 16 - 2) / 16;
	if (collisions[ind(x, y)] != NILT)
		return collisions[ind(x, y)];

	if (ap->w == 32) {
		x = (FIXTOINT(ap->x) + 16) / 16;
		if (collisions[ind(x, y)] != NILT)
			return collisions[ind(x, y)];

		x = (FIXTOINT(ap->x) + 32 - 16 - 2) / 16;
		if (collisions[ind(x, y)] != NILT)
			return collisions[ind(x, y)];
	}

	return NILT;
}

char
lcollision(struct actor *ap) {
	int y, x;

	y = FIXTOINT(ap->y) / 16;
	if (ap->w == 32)
		x = (FIXTOINT(ap->x) + 8 - 1) / 16;
	else
		x = (FIXTOINT(ap->x) - 1) / 16;

	if (collisions[ind(x, y)] != NILT)
		return collisions[ind(x, y)];
	y = (FIXTOINT(ap->y) + 16 - 2) / 16;
	if (collisions[ind(x, y)] != NILT)
		return collisions[ind(x, y)];
	if (ap->h == 32) {
		y = (FIXTOINT(ap->y) + 16) / 16;
		if (collisions[ind(x, y)] != NILT)
			return collisions[ind(x, y)];

		y = (FIXTOINT(ap->y) + 32 - 2) / 16;
		if (collisions[ind(x, y)] != NILT)
			return collisions[ind(x, y)];
	}
	return NILT;
}

/* need to fix this... */
char
rcollision(struct actor *ap) {
	int y, x;

	y = FIXTOINT(ap->y) / 16;
	if (ap->w == 32)
		x = (FIXTOINT(ap->x) + ap->w - 8 - 1) / 16;
	else
		x = (FIXTOINT(ap->x) + ap->w - 1) / 16;
	if (collisions[ind(x, y)] != NILT)
		return collisions[ind(x, y)];
	y = (FIXTOINT(ap->y) + 16 - 2) / 16;
	if (collisions[ind(x, y)] != NILT)
		return collisions[ind(x, y)];
	if (ap->h == 32) {
		y = (FIXTOINT(ap->y) + 16) / 16;
		if (collisions[ind(x, y)] != NILT)
			return collisions[ind(x, y)];

		y = (FIXTOINT(ap->y) + 32 - 2) / 16;
		if (collisions[ind(x, y)] != NILT)
			return collisions[ind(x, y)];
	}
	return NILT;
}

char
checkactorcol(struct actor *ap, struct actor *bp) {
	char x, y;
	x = (bp->x + bp->wpad >= ap->x + ap->wpad) &&
	    (bp->x + bp->wpad <= ap->x - ap->wpad + INTTOFIX(ap->w));
	x |= (ap->x + ap->wpad >= bp->x + bp->wpad) &&
	     (ap->x + ap->wpad <= bp->x + -bp->wpad + INTTOFIX(bp->w));
	y = (bp->y + bp->hpad >= ap->y + ap->hpad) &&
	    (bp->y + bp->hpad <= ap->y - ap->hpad + INTTOFIX(ap->h));
	y |= (ap->y + ap->hpad >= bp->y + bp->hpad) &&
	     (ap->y + ap->hpad <= bp->y - bp->hpad + INTTOFIX(bp->h));
	return x && y;
}

#define NOCX 0

#define PLAYERTOBOMBCX 1
#define PLAYERTOFRUITCX 2

#define BOMBTOPLAYERCX 3
#define BOMBTOBULLETCX 3

#define BULLETTOPLAYERCX 4

#define FRUITTOPLAYERCX 5

// clang-format off
const char collisiontable[NIDS * NIDS] = {
	0,0,0,0,0,0,0,	/* 0) NOID */
	0,0,0,0,PLAYERTOBOMBCX,PLAYERTOFRUITCX,0,	/* 1) PLAYERID */
	0,BULLETTOPLAYERCX,0,0,0,0,0,	/* 2) BULLETID */
	0,0,0,0,0,0,0,	/* 3) ROCKETID */
	0,BOMBTOPLAYERCX,BOMBTOBULLETCX,0,0,0,0,	/* 4) BOMBID */
	0,FRUITTOPLAYERCX,0,0,0,0,0,	/* 5) FRUITID */
	0,0,0,0,0,0,0	/* 6) DEADID */
};
// clang-format on

void
xcol(struct actor *ap, struct actor *bp) {
	char colid;

	colid = collisiontable[ap->id * NIDS + bp->id];
	switch (colid) {
	case PLAYERTOBOMBCX:
		if (ap->state & (PHURT | PDEAD))
			return;
		ap->state |= PHURT;
		nlives &&nlives--;
		if (!nlives) {
			ap->state = PDEAD;
		}
		break;
	case PLAYERTOFRUITCX:
		playsound(SOUNDFRUIT);
		score++;
		break;

	case BULLETTOPLAYERCX:
		ap->status = STATUSFREE;
		ap->parent->nchildren--;
		break;

	case BOMBTOPLAYERCX:
	case BOMBTOBULLETCX:
		ap->state = BDEAD;
		break;

	case FRUITTOPLAYERCX:
		ap->status = STATUSFREE;
		ap->parent->nchildren--;
		ap->parent->timer = 0x020;
	}
}

void
checkcols(void) {
	struct actor *ip, *jp;

	for (ip = actors; ip < actors + NACTORS; ip++) {
		if (!(ip->status & STATUSACTIVE)) {
			continue;
		}
		for (jp = actors; jp < actors + NACTORS; jp++) {
			if (!(jp->status & STATUSACTIVE)) {
				continue;
			}
			if (ip == jp) {
				continue;
			}
			if (!collisiontable[(ip->id * NIDS) + jp->id]) {
				continue;
			}
			if (checkactorcol(ip, jp)) {
				xcol(ip, jp);
			}
		}
	}
}