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

struct actor actors[TACTORS];

void
clearactors(void) {
	char i;

	for (i = 0; i < TACTORS; i++) {
		actors[i].status = STATUSFREE;
	}
}

void
pauseactors(void) {
	char i;

	for (i = 0; i < NACTORS; i++) {
		actors[i].status &= ~STATUSACTIVE;
	}
}

void
unpauseactors(void) {
	char i;

	for (i = 0; i < NACTORS; i++) {
		if (actors[i].status & STATUSUSED) {
			actors[i].status |= STATUSACTIVE;
		}
	}
}

struct actor *
createactor(void) {
	int i;

	for (i = 0; i < NACTORS; i++) {
		if (!actors[i].status) {
			//memset(actors + i, 0, sizeof(struct actor));
			actors[i].status = STATUSUSED | STATUSACTIVE;
			actors[i].next = NULL;
			return &actors[i];
		}
	}
	return 0;
}

void
update(struct actor *ap) {
	switch (ap->id) {
	case PLAYERID:
		updateplayer(ap);
		break;
	case BULLETID:
		updatebullet(ap);
		break;
	case BOMBID:
		updatebomb(ap);
		break;
	case FRUITID:
		break;
	case DEADID:
		updatedead(ap);
		break;
	}
}

int nsprites;
void
draw(struct actor *ap) {
	int pattern, offset;
	char ctrl;
	char shift;

	if (!ap->status)
		return;

	spr_set(nsprites++);
	spr_x(FIXTOINT(ap->x));
	spr_y(FIXTOINT(ap->y));
	spr_pal(ap->pal);
	spr_pri(1);

	if (ap->w == 16) {
		shift = 1;
		ctrl = 0x00;
	} else if (ap->w == 32) {
		ctrl = 0x01;
		shift = 2;
	}

	if (ap->h == 16) {
		ctrl |= 0x00;
		shift *= 1;
	} else if (ap->h == 32) {
		ctrl |= 0x10;
		shift *= 2;
	} else if (ap->h == 64) {
		ctrl |= 0x30;
		shift *= 4;
	}

	if (ap->flags & FLAGSLEFT)
		ctrl |= FLIP_X;
	spr_ctrl(FLIP_MAS | SIZE_MAS, ctrl);

	pattern = ap->pattern << 6;
	if (ap->animation) {
		offset = ap->animation[ap->frame];
		offset *= shift;
		offset <<= 6;
		pattern += offset;
	}
	pattern += SPRVRAM;
	spr_pattern(pattern);
}

void
startdrawing() {
	nsprites = 0;
}

void
finishdrawing() {
	char i;

	for (i = nsprites; i < 63; i++) {
		spr_hide(i);
	}
}

void
animate(struct actor *ap) {
	char done, nextf, nextd, frameduration, step;

	done = 0;
	if (ap->animation != oldanimation) {
		ap->frame = 0;
		ap->framei = 0;
		ap->flags &= ~FLAGSCYCLED;
	}

	draw(ap);

	if (!ap->animation)
		goto out;

	if (ap->animation == playerflyanimation) {
		if (ap->vy < -0x30)
			step = 7;
		else if (ap->vy < -0x20)
			step = 5;
		else if (ap->vy < -0x10)
			step = 3;
		else
			step = 2;
	} else {
		step = 1;
	}

	frameduration = ap->animation[ap->frame + 1];

	if (ap->framei < frameduration) {
		ap->framei += step;
	} else {
		ap->frame += 2;
		ap->framei = 0;
	}

	nextf = ap->animation[ap->frame];
	nextd = ap->animation[ap->frame + 1];
	if (nextf == 0 && nextd == 0) {
		ap->frame = 0;
		ap->framei = 0;
		done = 1;
	}

	if (nextf == 0 && nextd == 0xff) {
		ap->animation = 0;
		done = 1;
	}

out:
	if (done)
		ap->flags |= FLAGSCYCLED;
	else
		ap->flags &= ~FLAGSCYCLED;
}

void
updatedead(struct actor *ap) {
	if (ap->flags & FLAGSCYCLED) {
		ap->status = STATUSFREE;
		if (ap->parent) {
			controllernotify(ap);
		}
	}
}

char
ymove(struct actor *ap) {
	char r;
	int v;

	r = NILT;
	v = ap->vy;

	/* Moving up */
	if (v < 0) {
		while (v < 0 && (r = ucollision(ap)) == NILT) {
			ap->y -= INTTOFIX(1);
			v += INTTOFIX(1);
		}
	}

	/* Moving down */
	if (v > 0) {
		while (v > 0 && (r = dcollision(ap)) == NILT) {
			ap->y += INTTOFIX(1);
			v -= INTTOFIX(1);
		}
	}
	return r;
}

char
xmove(struct actor *ap) {
	char r;
	int v;

	v = ap->vx;
	/* Moving left */
	if (v < 0) {
		while (v < 0 && (r = lcollision(ap)) == NILT) {
			ap->x -= INTTOFIX(1);
			v += INTTOFIX(1);
		}
	}

	/* Moving right */
	if (v > 0) {
		while (v > 0 && (r = rcollision(ap)) == NILT) {
			ap->x += INTTOFIX(1);
			v -= INTTOFIX(1);
		}
	}

	return r;
}

void
initactors(char i) {
	struct actor *ap;
	char *p;

	switch (i) {
	case STATEPLAY:
		p = level001actors;
		break;
	default:
		return;
	}
	while (*p) {
		ap = createactor();
		ap->id = *p++;
		switch (ap->id) {
		case PLAYERID:
			initplayer(ap);
			break;
		}
		ap->x = INTTOFIX(*p++ * 16);
		ap->y = INTTOFIX(*p++ * 16);
	}
}
