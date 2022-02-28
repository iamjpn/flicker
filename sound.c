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

const unsigned char *psgch = 0x800;
const unsigned char *psgbal = 0x801;
const unsigned char *psgfreqlo = 0x802;
const unsigned char *psgfreqhi = 0x803;
const unsigned char *psgctrl = 0x804;
const unsigned char *psgchbal = 0x805;
const unsigned char *psgdata = 0x806;
const unsigned char *psgnoise = 0x807;
const unsigned char *psglfofreq = 0x808;
const unsigned char *psglfoctrl = 0x809;

const char sine[] = { 18, 22, 24, 26, 28, 28, 30, 30, 30, 30, 28,
		      28, 26, 24, 22, 18, 12, 8,  6,  4,  2,  2,
		      0,  0,  0,  0,  2,  2,  4,  6,  8,  12 };

char musicis[2];
char durations[2];
char bpms[2];

/*
	80 B/M
	1 1/3 / S

	0.00133 / ms

	one beat == 750 mseconds

	each vsync ==  1 / 60 seconds
		   ~= 16 milliseconds

	count 46 vsyncs
*/

void
initmusic(char channel, char *s) {
	char i;

	*psgch = channel;

	*psgctrl = 1 << 6;
	*psgctrl = 0;
	for (i = 0; i < 32; i++) {
		*psgdata = sine[i];
	}

	musicis[channel] = 0;
	*psgfreqlo = s[musicis[channel]++];
	*psgfreqhi = s[musicis[channel]++];
	durations[channel] = s[musicis[channel]++];
	musicis[channel]++;
	bpms[channel]++;

	*psgchbal = 0xee;

	*psgbal = 0xff;

	*psgctrl = 1 << 7 | 31;
}

void
initnoise() {
	*psgch = NOISECHANNEL;

	*psgctrl = 0x1f;

	*psgchbal = 0x00;

	*psgnoise = 0x82;
	*psgctrl = 1 << 7 | 31;
}

char noisetimer;
void
noisetick() {
	if (noisetimer == 0) {
		*psgch = NOISECHANNEL;
		*psgchbal = 0x00;
		return;
	}
	noisetimer--;
}

void
playnoise(char x) {
	noisetimer = x;

	*psgch = NOISECHANNEL;
	*psgchbal = 0xaa;
}

void
musictick(char channel, char *s, char len) {
	char rem;
	*psgch = channel;

	bpms[channel]++;
	rem = durations[channel] - bpms[channel];
	switch (rem) {
	case 0:
		*psgfreqlo = s[musicis[channel]++];
		*psgfreqhi = s[musicis[channel]++];
		durations[channel] = s[musicis[channel]++];
		musicis[channel]++;
		*psgctrl = 1 << 7 | 31;
		musicis[channel] = musicis[channel] % len;
		bpms[channel] = 0;
		break;
	case 3:
		*psgctrl = 1 << 7 | 30;
		break;
	case 2:
		*psgctrl = 1 << 7 | 29;
		break;
	case 1:
		*psgctrl = 1 << 7 | 28;
		break;
	}
}

char soundcount;
char soundi;
char *soundbuffer;

const int jumpsound[] = { C3 };

const int shootsound[] = { C5 };

const int shootfruit[] = { G5 };

char lofreq;
void
playsound(char sound) {
	*psgch = SOUNDCHANNEL;
	*psgctrl = 1 << 7 | 31;
	switch (sound) {
	case SOUNDJUMP:
		soundbuffer = jumpsound;
		break;
	case SOUNDSHOOT:
		soundbuffer = shootsound;
		break;
	case SOUNDFRUIT:
		soundbuffer = shootfruit;
		break;
	}
	lofreq = soundbuffer[0];
	*psgfreqlo = soundbuffer[0];
	*psgfreqhi = soundbuffer[1];
	soundcount = 0;
}

void
initsound() {
	char i;
	*psgch = SOUNDCHANNEL;

	*psgctrl = 1 << 6;
	*psgctrl = 0;
	for (i = 0; i < 32; i++) {
		*psgdata = sine[i];
	}

	soundbuffer = NULL;

	*psgchbal = 0xff;

	*psgbal = 0xff;
}

void
soundtick() {
	char *s;
	*psgch = SOUNDCHANNEL;

	if (!soundbuffer)
		return;

	soundcount++;
	*psgfreqlo = --lofreq;
	if (soundcount == 8) {
		soundcount = 0;
		soundbuffer == NULL;
		*psgctrl = 0;
	}
}