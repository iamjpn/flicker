#
#    fmpreader - tile meta data reader for mappy FMP files.
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

fmpreader: main.o mappy.o mappy.h
	$(CC) $(CFLAGS) main.o mappy.o -o fmpreader

.PHONY clean:
clean:
	rm -f fmpreader *.o
