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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mappy.h"

/* globals */
int maperror = MER_NONE;
int mapislsb;
int mapwidth, mapheight;
int mapblockwidth, mapblockheight, mapblockdepth;
int mapblockstrsize, mapnumblockstr, mapnumblockgfx, mapnumanimstr;
unsigned char *mapcmappt = NULL;
BLKSTR *mapblockstrpt = NULL;
ANISTR *mapanimstrpt = NULL;
int *mapanimseqpt = NULL;
int *mappt = NULL;
int **maparraypt = NULL;
int *mapmappt[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
int **mapmaparraypt[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

/* end of globals */

BLKSTR *
MapGetBlock(int x, int y) {
	int *mymappt;

	if (maparraypt != NULL) {
		mymappt = maparraypt[y] + x;
	} else {
		mymappt = mappt;
		mymappt += x;
		mymappt += y * mapwidth;
	}
	if (*mymappt >= 0)
		return mapblockstrpt + mymappt[0];
	else
		return mapblockstrpt +
		       mapanimseqpt[mapanimstrpt[(-*mymappt) - 1].ancuroff];
}

int
MapGetchksz(unsigned char *locpt) {
	return ((((int)(locpt[0])) << 24) | (((int)(locpt[1])) << 16) |
		(((int)(locpt[2])) << 8) | ((int)(locpt[3])));
}

int
MapGetshort(unsigned char *locpt) {
	if (mapislsb)
		return ((((int)(locpt[1])) << 8) | ((int)(locpt[0])));
	else
		return ((((int)(locpt[0])) << 8) | ((int)(locpt[1])));
}

int
MapGetlong(unsigned char *locpt) {
	if (mapislsb)
		return ((((int)(locpt[3])) << 24) | (((int)(locpt[2])) << 16) |
			(((int)(locpt[1])) << 8) | ((int)(locpt[0])));
	else
		return ((((int)(locpt[0])) << 24) | (((int)(locpt[1])) << 16) |
			(((int)(locpt[2])) << 8) | ((int)(locpt[3])));
}

void
MapFreeMem(void) {
	int i;

	if (mapcmappt != NULL) {
		free(mapcmappt);
		mapcmappt = NULL;
	}
	if (mapblockstrpt != NULL) {
		free(mapblockstrpt);
		mapblockstrpt = NULL;
	}
	if (mapanimstrpt != NULL) {
		free(mapanimstrpt);
		mapanimstrpt = NULL;
	}
	if (mapanimseqpt != NULL) {
		free(mapanimseqpt);
		mapanimseqpt = NULL;
	}

	for (i = 0; i < 8; i++) {
		if (mapmappt[i] != NULL) {
			free(mapmappt[i]);
			mapmappt[i] = NULL;
		}
	}
	for (i = 0; i < 8; i++) {
		if (mapmaparraypt[i] != NULL) {
			free(mapmaparraypt[i]);
			mapmaparraypt[i] = NULL;
		}
	}
	mappt = NULL;
	maparraypt = NULL;
}

int
MapDecodeMPHD(unsigned char *mdatpt) {
	mdatpt += 8;
	if (mdatpt[0] > 0) {
		maperror = MER_WRONGMAPVER;
		return -1;
	}
	if (mdatpt[2] == 1)
		mapislsb = 1;
	else
		mapislsb = 0;

	mapwidth = (int)MapGetshort(mdatpt + 4);
	mapheight = (int)MapGetshort(mdatpt + 6);
	mapblockwidth = (int)MapGetshort(mdatpt + 12);
	mapblockheight = (int)MapGetshort(mdatpt + 14);
	mapblockdepth = (int)MapGetshort(mdatpt + 16);
	mapblockstrsize = (int)MapGetshort(mdatpt + 18);
	mapnumblockstr = (int)MapGetshort(mdatpt + 20);
	mapnumblockgfx = (int)MapGetshort(mdatpt + 22);
	return 0;
}

int
MapDecodeCMAP(unsigned char *mdatpt) {
	mapcmappt = malloc(MapGetchksz(mdatpt + 4));
	if (mapcmappt == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}

	memcpy(mapcmappt, mdatpt + 8, MapGetchksz(mdatpt + 4));
	return 0;
}

int
MapDecodeBKDT(unsigned char *mdatpt) {
	int i, j;
	BLKSTR *myblkstrpt;

	mapblockstrpt = malloc(mapnumblockstr * (sizeof(BLKSTR)));
	if (mapblockstrpt == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}

	mdatpt += 8;
	myblkstrpt = mapblockstrpt;
	j = mapblockwidth * mapblockheight * ((mapblockdepth + 1) / 8);
	for (i = 0; i < mapnumblockstr; i++) {
		myblkstrpt->bg = (MapGetlong(mdatpt) / j);
		myblkstrpt->fg = (MapGetlong(mdatpt + 4) / j);
		myblkstrpt->fg2 = (MapGetlong(mdatpt + 8) / j);
		myblkstrpt->fg3 = (MapGetlong(mdatpt + 12) / j);
		myblkstrpt->user1 = (MapGetlong(mdatpt + 16));
		myblkstrpt->user2 = (MapGetlong(mdatpt + 20));
		myblkstrpt->user3 = (MapGetshort(mdatpt + 24));
		myblkstrpt->user4 = (MapGetshort(mdatpt + 26));
		myblkstrpt->user5 = mdatpt[28];
		myblkstrpt->user6 = mdatpt[29];
		myblkstrpt->user7 = mdatpt[30];
		if (mdatpt[31] & 0x01)
			myblkstrpt->tl = 1;
		else
			myblkstrpt->tl = 0;
		if (mdatpt[31] & 0x02)
			myblkstrpt->tr = 1;
		else
			myblkstrpt->tr = 0;
		if (mdatpt[31] & 0x04)
			myblkstrpt->bl = 1;
		else
			myblkstrpt->bl = 0;
		if (mdatpt[31] & 0x08)
			myblkstrpt->br = 1;
		else
			myblkstrpt->br = 0;
		if (mdatpt[31] & 0x10)
			myblkstrpt->trigger = 1;
		else
			myblkstrpt->trigger = 0;
		if (mdatpt[31] & 0x20)
			myblkstrpt->unused1 = 1;
		else
			myblkstrpt->unused1 = 0;
		if (mdatpt[31] & 0x40)
			myblkstrpt->unused2 = 1;
		else
			myblkstrpt->unused2 = 0;
		if (mdatpt[31] & 0x80)
			myblkstrpt->unused3 = 1;
		else
			myblkstrpt->unused3 = 0;

		mdatpt += mapblockstrsize;
		myblkstrpt++;
	}

	return 0;
}

int
MapDecodeANDT(unsigned char *mdatpt) {
	int i, j;
	ANISTR *myanistrpt;
	unsigned char *countpt;
	int *myseqpt;

	j = MapGetchksz(mdatpt + 4);
	mdatpt += 8;
	countpt = (mdatpt + j) - 16;
	mapnumanimstr = 0;
	while (((char *)countpt)[0] != -1) {
		mapnumanimstr++;
		countpt -= 16;
	}

	mapanimstrpt = malloc(mapnumanimstr * (sizeof(ANISTR)));
	if (mapanimstrpt == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	mapanimseqpt = malloc(((countpt - mdatpt) / 4) * sizeof(int));
	if (mapanimseqpt == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}

	myseqpt = mapanimseqpt;
	while (mdatpt != countpt) {
		myseqpt[0] = (MapGetlong(mdatpt) / 32);
		myseqpt++;
		mdatpt += 4;
	}

	mdatpt += (mapnumanimstr * 16);
	myanistrpt = mapanimstrpt;
	for (i = 0; i < mapnumanimstr; i++) {
		myanistrpt->antype = (int)mdatpt[0];
		if (myanistrpt->antype == 255)
			myanistrpt->antype = -1;
		myanistrpt->andelay = (int)mdatpt[1];
		myanistrpt->ancount = (int)mdatpt[2];
		myanistrpt->anuser = (int)mdatpt[3];

		myanistrpt->ancuroff = (int)((j + MapGetlong(mdatpt + 4)) / 4);
		myanistrpt->anstartoff =
			(int)((j + MapGetlong(mdatpt + 8)) / 4);
		myanistrpt->anendoff = (int)((j + MapGetlong(mdatpt + 12)) / 4);

		myanistrpt++;
		mdatpt -= 16;
	}

	return 0;
}

int
MapDecodeBGFX(unsigned char *mdatpt) {
	return 0;
}

int
MapDecodeLayer(unsigned char *mdatpt, int lnum) {
	int i, j;
	int *mymappt;

	mapmappt[lnum] = malloc(mapwidth * mapheight * sizeof(int));
	if (mapmappt[lnum] == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}

	mdatpt += 8;
	mymappt = mapmappt[lnum];
	for (j = 0; j < mapheight; j++) {
		for (i = 0; i < mapwidth; i++) {
			*mymappt = (int)MapGetshort(mdatpt);
			if (*mymappt > 0x7FFF) {
				*mymappt -= 0x10000;
				*mymappt /= 16;
			} else {
				*mymappt /= 32;
			}
			mdatpt += 2;
			mymappt++;
		}
	}

	if (lnum == 0)
		mappt = mapmappt[lnum];
	return 0;
}

int
MapDecode(unsigned char *fmappt) {
	unsigned char *fmappospt;
	int mapfilesize;

	MapFreeMem();
	fmappospt = fmappt;
	if (strncmp((const char *)fmappt, "FORM", 4)) {
		maperror = MER_MAPLOADERROR;
		return -1;
	}

	mapfilesize = MapGetchksz(fmappt + 4);

	if (strncmp((const char *)fmappt + 8, "FMAP", 4)) {
		maperror = MER_MAPLOADERROR;
		return -1;
	}

	fmappospt += 12;
	mapfilesize -= 4;
	while (mapfilesize > 0) {
		if (!strncmp((const char *)fmappospt, "MPHD", 4))
			MapDecodeMPHD(fmappospt);
		if (!strncmp((const char *)fmappospt, "CMAP", 4))
			MapDecodeCMAP(fmappospt);
		if (!strncmp((const char *)fmappospt, "BKDT", 4))
			MapDecodeBKDT(fmappospt);
		if (!strncmp((const char *)fmappospt, "ANDT", 4))
			MapDecodeANDT(fmappospt);
		if (!strncmp((const char *)fmappospt, "BGFX", 4))
			MapDecodeBGFX(fmappospt);
		if (!strncmp((const char *)fmappospt, "BODY", 4))
			MapDecodeLayer(fmappospt, 0);
		if (!strncmp((const char *)fmappospt, "LYR1", 4))
			MapDecodeLayer(fmappospt, 1);
		if (!strncmp((const char *)fmappospt, "LYR2", 4))
			MapDecodeLayer(fmappospt, 2);
		if (!strncmp((const char *)fmappospt, "LYR3", 4))
			MapDecodeLayer(fmappospt, 3);
		if (!strncmp((const char *)fmappospt, "LYR4", 4))
			MapDecodeLayer(fmappospt, 4);
		if (!strncmp((const char *)fmappospt, "LYR5", 4))
			MapDecodeLayer(fmappospt, 5);
		if (!strncmp((const char *)fmappospt, "LYR6", 4))
			MapDecodeLayer(fmappospt, 6);
		if (!strncmp((const char *)fmappospt, "LYR7", 4))
			MapDecodeLayer(fmappospt, 7);

		mapfilesize -= 8;
		mapfilesize -= MapGetchksz(fmappospt + 4);
		fmappospt += MapGetchksz(fmappospt + 4);
		fmappospt += 8;

		if (maperror != MER_NONE) {
			MapFreeMem();
			return -1;
		}
	}
	return 0;
}

int
MapLoad(char *mapname) {
	int mretval;
	FILE *fpt;
	char idtag[4];
	unsigned char tempc;
	unsigned char *fmappt;
	int mapfilesize;

	maperror = MER_NONE;
	fpt = fopen(mapname, "rb");
	if (fpt == NULL) {
		maperror = MER_NOOPEN;
		return -1;
	}

	mapfilesize = 0;
	maperror = MER_MAPLOADERROR;
	if (fread(&idtag[0], 1, 1, fpt) == 1) {
		if (fread(&idtag[1], 1, 1, fpt) == 1) {
			if (fread(&idtag[2], 1, 1, fpt) == 1) {
				if (fread(&idtag[3], 1, 1, fpt) == 1) {
					if (fread(&tempc, 1, 1, fpt) == 1) {
						mapfilesize =
							(((int)tempc) << 24);
						if (fread(&tempc, 1, 1, fpt) ==
						    1) {
							mapfilesize |=
								(((int)tempc)
								 << 16);
							if (fread(&tempc, 1, 1,
								  fpt) == 1) {
								mapfilesize |=
									(((int)tempc)
									 << 8);
								if (fread(&tempc,
									  1, 1,
									  fpt) ==
								    1) {
									mapfilesize |=
										((int)tempc);
									mapfilesize +=
										8;
									if (!strncmp(
										    idtag,
										    "FORM",
										    4))
										maperror =
											MER_NONE;
								}
							}
						}
					}
				}
			}
		}
	}

	if (maperror != MER_NONE) {
		fclose(fpt);
		return -1;
	}

	fmappt = malloc(mapfilesize);
	if (fmappt == NULL) {
		fclose(fpt);
		maperror = MER_OUTOFMEM;
		return -1;
	}

	fseek(fpt, 0, SEEK_SET);
	if (fread(fmappt, 1, mapfilesize, fpt) != mapfilesize) {
		fclose(fpt);
		free(fmappt);
		maperror = MER_MAPLOADERROR;
		return -1;
	}
	fclose(fpt);

	mretval = MapDecode(fmappt);
	free(fmappt);
	return mretval;
}
