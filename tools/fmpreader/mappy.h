/*
    fmpreader - tile meta data reader for mappy FMP files.
    Copyright (C) 2022  Jordan Niethe

	Derived from MappyGL
	Copyright (C) 2000 Robin Burrows <mappy@tilemap.co.uk>

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

#define MER_NONE 0 /* All the horrible things that can go wrong */
#define MER_OUTOFMEM 1
#define MER_MAPLOADERROR 2
#define MER_NOOPEN 3
#define MER_WRONGMAPVER 4

/* structure defs */
typedef struct { /* Structure for data blocks */
	int bg, fg; /* number of graphic */
	int fg2, fg3; /* more overlay blocks */
	unsigned int user1, user2; /* user long data */
	unsigned short user3, user4; /* user short data */
	unsigned char user5, user6, user7; /* user byte data */
	unsigned char tl : 1; /* bits for collision detection */
	unsigned char tr : 1;
	unsigned char bl : 1;
	unsigned char br : 1;
	unsigned char trigger : 1; /* transparency */
	unsigned char unused1 : 1;
	unsigned char unused2 : 1;
	unsigned char unused3 : 1;
} BLKSTR;

typedef struct { /* Animation control structure */
	int antype; /* Type of anim, AN_? */
	int andelay; /* Frames to go before next frame */
	int ancount; /* Counter, decs each frame, till 0, then resets to andelay */
	int anuser; /* User info */
	int ancuroff; /* current offset in list (mapanimseqpt) */
	int anstartoff; /* start of blkstr offsets list, AFTER ref. blkstr offset */
	int anendoff; /* end of blkstr offsets list */
} ANISTR;

int MapLoad(char *);
int MapDecode(unsigned char *);
void MapFreeMem(void);
BLKSTR *MapGetBlock(int, int);

extern int mapwidth, mapheight;
extern int mapblockwidth, mapblockheight, mapblockdepth;
extern int mapnumblockstr, mapnumblockgfx, mapnumanimstr;
extern int *mappt;
extern int **maparraypt;
