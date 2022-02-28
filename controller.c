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

struct controller controllers[NCONTROLLERS];

void
clearcontrollers(void) {
	char i;

	for (i = 0; i < NCONTROLLERS; i++) {
		controllers[i].childid = NOID;
		controllers[i].childlist = NULL;
		controllers[i].status = 0;
	}
}

void
pausecontrollers(void) {
	char i;

	for (i = 0; i < NCONTROLLERS; i++) {
		controllers[i].status = 0;
	}
}

void
unpausecontrollers(void) {
	char i;

	for (i = 0; i < NCONTROLLERS; i++) {
		if (controllers[i].childid != NOID)
			controllers[i].status = STATUSACTIVE;
	}
}

void
updatecontroller(struct controller *cp) {
	if (cp->timer > 0) {
		cp->timer--;
	}

	switch (cp->childid) {
	case BOMBID:
		updatebombcontroller(cp);
		break;
	case FRUITID:
		updatefruitcontroller(cp);
		break;
	case BULLETID:
		break;
	case HEARTID:
		updateheartcontroller(cp);
		break;
	}
}

void
controllernotify(struct actor *ap) {
	ap->parent->nchildren--;
	ap->parent->timer += 40;
}

void
initcontrollers(char i) {
	struct controller *cp;
	char *p;

	cp = controllers;
	switch (i) {
	case STATEPLAY:
		p = level001fixtures;
		break;
	default:
		return;
	}
	while (*p) {
		cp->childid = *p++;
		cp->status = STATUSACTIVE;
		cp->nchildren = 0;
		cp->maxchildren = *p++;
		cp->ncoords = *p++;
		cp->coordlist = p;
		cp->timer = 0;
		cp->i = 0;
		cp->childlist = NULL;
		p = p + cp->ncoords * 2;
		if (cp->childid == BULLETID) {
			bulletcontroller = cp;
		}
		cp++;
	}
}