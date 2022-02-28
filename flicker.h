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

#define true 1
#define false 0

#define NULL 0

#define SPRVRAM 0x5000

#define NOID 0
#define PLAYERID 1
#define BULLETID 2
#define ROCKETID 3
#define BOMBID 4
#define FRUITID 5
#define DEADID 6
#define HEARTID 0x10
#define NIDS 7

/* Sound */
#define NOISECHANNEL 4
#define SOUNDCHANNEL 2
#define MELODYCHANNEL 0
#define BASSCHANNEL 1

#define C3 0x357
#define F3 0x281
#define G3 0x23b
#define C4 0x1ac
#define D4 0x17d
#define E4 0x153
#define F4 0x140
#define G4 0x11d
#define A4 0x0fe
#define B4 0x0e3
#define C5 0x0d6
#define G5 0x085

#define CROTCHET 30
#define MINIM 60
#define SEMIBREVE 120

#define SOUNDJUMP 0
#define SOUNDSHOOT 1
#define SOUNDFRUIT 2

struct controller {
	char status;
	char childid;
	char nchildren;
	char maxchildren;
	char flags;
	char ncoords;
	char i;
	char *coordlist;
	char timer;
	void *childlist;
};

struct actor {
	char status; /* Fundemental state */
	char flags; /* Visible state */
	char state; /* Internal state machine */
	char id;

	char h;
	char w;
	char wpad;
	char hpad;
	int x;
	int y;
	int vx;
	int vy;

	struct actor *next; /* List */

	char pal;
	char pattern;
	char frame;
	char framei;
	char *animation;
	struct controller *parent;
};

#define STATUSFREE 0
#define STATUSUSED 1
#define STATUSACTIVE 2
#define STATUSPAUSE 4

#define FLAGSLEFT 1
#define FLAGSCYCLED 2

#define STATESTART 1
#define STATEPLAY 2
#define STATEEND 3
#define STATEPAUSE 4

extern char gameover;
extern int buttons;
extern char nlives;
extern int score;

extern char playerspr[32];
extern char bombspr[1];
extern char bulletspr[4];
extern char fruitspr[2];
extern char heartspr[1];

#define NACTORS 0x6
#define NGHOSTS 0x5
#define TACTORS (NACTORS + NGHOSTS)
extern struct actor actors[];

#define NCONTROLLERS 4

extern char level001actors[];

#define max(a, b) (((a)) > ((b)) ? (a) : (b))
#define min(a, b) (((a)) < ((b)) ? (a) : (b))
#define abs(a) (((a) >= 0) ? (a) : (-(a)))

#define FIXSHIFT 4
#define FIXTOINT(x) ((x) >> FIXSHIFT)
#define INTTOFIX(x) ((x) << FIXSHIFT)

extern struct controller controllers[];
extern const int level001map[];
extern const int level001collisions[];
extern const int level001tiles[];
extern const int level001pal[];
extern const int level001fixtures[];

extern struct controller *bulletcontroller;

extern const char playerhurtanimation[];
extern const char playerwalkanimation[];
extern const char playerdeadanimation[];
extern const char playerflyanimation[];
extern const char bulletanimation[];
extern const char bulletdeadanimation[];
extern const char fruitanimation[];
extern const char bombanimation[];
extern const char bombdeadanimation[];

extern char *oldanimation;

#define NILT 0x00
#define GROUNDT 0x01

#define MAPW 16
#define MAPH 14

extern char *collisions;

#define PALIVE 1
#define PHURT 2
#define PJUMPED 4
#define PDEAD 16

#define BALIVE 0x01
#define BDEAD 0x02
