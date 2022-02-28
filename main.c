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

#include "sprites.h"
#include "picturemap.h"
#include "levels/levelstart.h"
#include "levels/levelend.h"
#include "sound.h"

char gamestate;
char nlives;
char gameover;
char *oldanimation;
char statedebounce;
struct controller *bulletcontroller;
int buttons;
int score;

void
initroom(char i) {
	switch (i) {
	case STATEPLAY:
		collisions = level001collisions;

		set_font_pal(0);
		set_font_color(6, 0);
		load_default_font();

		set_map_data(level001map, 16, 14);
		set_tile_data(level001tiles);
		load_tile(0x1000);
		load_map(0, 0, 0, 0, 16, 14);
		load_palette(0, level001pal, 1);
		break;
	case STATESTART:
		collisions = NULL;

		set_map_data(picturemap, 16, 14);
		set_tile_data(levelstarttiles);
		load_tile(0x1000);
		load_map(0, 0, 0, 0, 16, 14);
		load_palette(0, level001pal, 1);
		break;
	case STATEEND:
		collisions = NULL;

		set_map_data(picturemap, 16, 14);
		set_tile_data(levelendtiles);
		load_tile(0x1000);
		load_map(0, 0, 0, 0, 16, 14);
		load_palette(0, level001pal, 1);
		break;
	}
}

void
loadstage(char i) {
	initroom(i);
	clearactors();
	initactors(i);
	clearcontrollers();
	initcontrollers(i);
}

void
beginplay(void) {
	struct actor *a;

	nlives = 5;
	score = 0;
	gameover = 0;
	statedebounce = 0;

	gamestate = STATEPLAY;
	loadstage(gamestate);
}

void
updateplay(void) {
	struct actor *ep;

	put_string("Score:", 3, 1);
	put_number(score, 3, 10, 1);

	if (buttons & JOY_RUN) {
		if (statedebounce > 10) {
			pauseactors();
			pausecontrollers();
			gamestate = STATEPAUSE;
		}
	}

	if (gameover) {
		gamestate = STATEEND;
		loadstage(gamestate);
	}
}

void
updatestart(void) {
	//put_string("Press Run", 11, 4);
	if (buttons & JOY_RUN) {
		if (statedebounce > 30) {
			beginplay();
		}
	}
}

void
updateend(void) {
	if (buttons & JOY_RUN) {
		if (statedebounce > 30) {
			gamestate = STATESTART;
			loadstage(gamestate);
		}
	}
}

void
updatepause(void) {
	if (buttons & JOY_RUN) {
		if (statedebounce > 10) {
			unpauseactors();
			unpausecontrollers();
			gamestate = STATEPLAY;
		}
	}
}

char oldgamestate;
void
updateroom(void) {
	oldgamestate = gamestate;
	switch (gamestate) {
	case STATESTART:
		updatestart();
		break;
	case STATEPLAY:
		updateplay();
		break;
	case STATEEND:
		updateend();
		break;
	case STATEPAUSE:
		updatepause();
		break;
	}
	if (gamestate != oldgamestate)
		statedebounce = 0;
	else
		statedebounce++;
}

main() {
	struct actor *ap;
	struct controller *cp;

	set_screen_size(SCR_SIZE_32x32);
	init_satb();

	clearactors();

	load_vram(SPRVRAM + (bulletspr - playerspr), bulletspr,
		  bulletspr_end - bulletspr);
	load_vram(SPRVRAM + (fruitspr - playerspr), fruitspr,
		  fruitspr_end - fruitspr);
	load_vram(SPRVRAM + (bombspr - playerspr), bombspr,
		  bombspr_end - bombspr);
	load_vram(SPRVRAM + (playerspr - playerspr), playerspr,
		  playerspr_end - playerspr);
	load_vram(SPRVRAM + (heartspr - playerspr), heartspr,
		  heartspr_end - heartspr);
	load_palette(17, playerpal, 1);
	load_palette(18, spritepal, 1);

	oldgamestate = STATESTART;
	statedebounce = 30;
	gamestate = STATESTART;
	loadstage(gamestate);

	satb_update();

	initsound();
	initmusic(MELODYCHANNEL, twinkletwinkle);
	initmusic(BASSCHANNEL, twinkletwinklebass);
	initnoise();
	while (1) {
		soundtick();
		musictick(MELODYCHANNEL, twinkletwinkle,
			  sizeof(twinkletwinkle));
		musictick(BASSCHANNEL, twinkletwinklebass,
			  sizeof(twinkletwinklebass));
		noisetick();
		buttons = joy(0);
		startdrawing();
		updateroom();
		for (cp = controllers; cp < controllers + NCONTROLLERS; cp++) {
			if (cp->status & STATUSACTIVE)
				updatecontroller(cp);
		}
		for (ap = actors; ap < actors + NACTORS; ap++) {
			if (ap->status & STATUSACTIVE) {
				oldanimation = ap->animation;
				update(ap);
				animate(ap);
			}
		}
		for (ap = actors + NACTORS; ap < actors + TACTORS; ap++) {
			if (ap->status & STATUSACTIVE) {
				oldanimation = ap->animation;
				update(ap);
				animate(ap);
			}
		}
		checkcols();
		finishdrawing();
		vsync();
		satb_update();
	}
}
