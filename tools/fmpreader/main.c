/*
    fmpreader - tile meta data reader for mappy FMP files.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mappy.h"

void
usage(void) {
	fprintf(stderr, "usage: fmpreader file user[1,2,3,4,5]|flag[1,2,3]\n");
	exit(1);
}

int
main(int argc, char **argv) {
	if (argc != 3) {
		usage();
	}

	MapLoad(argv[1]);

	for (int i = 0; i < mapheight; i++) {
		for (int j = 0; j < mapwidth; j++) {
			BLKSTR *b;

			b = MapGetBlock(j, i);
			if (strcmp(argv[2], "user1") == 0)
				printf("%d ", b->user1);
			else if (strcmp(argv[2], "user2") == 0)
				printf("%d ", b->user2);
			else if (strcmp(argv[2], "user3") == 0)
				printf("%d ", b->user3);
			else if (strcmp(argv[2], "user4") == 0)
				printf("%d ", b->user4);
			else if (strcmp(argv[2], "user5") == 0)
				printf("%d ", b->user5);
			else if (strcmp(argv[2], "flag1") == 0)
				printf("%d ", b->unused1);
			else if (strcmp(argv[2], "flag2") == 0)
				printf("%d ", b->unused2);
			else if (strcmp(argv[2], "flag3") == 0)
				printf("%d ", b->unused3);
			else
				usage();
		}
		printf("\n");
	}
	return 0;
}
