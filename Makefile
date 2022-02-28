#
#    Forest Flicker
#    Copyright (C) 2022  Jordan Niethe
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, write to the Free Software Foundation, Inc.,
#    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

CC := huc
AS := pceas
INC := /usr/include/pce
CFLAGS :=
ASEPRITE := aseprite

SOURCE = main.c sound.c actor.c controller.c levels/level001.c animations.c \
		 collisions.c actors/player.c actors/bomb.c actors/bullet.c \
		 controllers/bomb.c controllers/fruit.c controllers/heart.c \
		 actors/ghost.c
HEADERS = picturemap.h flicker.h proto.h levels/levelend.h levels/levelstart.h \
		  sprites.h levels/level001collisions.h
SPRITES = sprites/player.pcx sprites/bullet.pcx sprites/bee.pcx \
		  sprites/fruit.pcx sprites/heart.pcx sprites/heart.pcx \
		  sprites/backgroundtiles.pcx
BACKGROUNDS = backgrounds/backgroundstart.pcx backgrounds/backgroundgameover.pcx

flicker: $(SOURCE) $(HEADERS) $(SPRITES) $(BACKGROUNDS)
	PCE_INCLUDE=$(INC) PCE_PCEAS=$(AS) $(CC) $(CLAGS) $(SOURCE)
	mv main.pce flicker.pce

levels/%collisions.h: tools/fmpreader/fmpreader
	$(shell tools/fmpreader/fmpreader maps/$*.FMP user1 | \
	  tools/carray const char level001collisions > $@)

picturemap.h:
	$(shell ./tools/pictureindices |\
	  ./tools/carray const char picturemap > picturemap.h)

sprites/%.pcx: resources/%.ase
	$(ASEPRITE) -b --sheet $@ --sheet-type vertical $< > /dev/null

backgrounds/%.pcx: resources/%.ase
	$(ASEPRITE) -b --sheet $@ --sheet-type horizontal $< > /dev/null

tools/fmpreader/fmpreader:
	make -C tools/fmpreader CC=gcc CFLAGS=

.PHONY format:
format:
	./tools/format

.PHONY play:
play:
	mednafen flicker.pce

.PHONY clean:
clean:
	rm -f main.s main.lst main.sym flicker.pce
	make -C tools/fmpreader clean

