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

extern void playsound(char);
extern void soundtick();
extern void playsound(char);
extern void noisetick();
extern void musictick(char, char *, char);

extern void initactors(char);
extern struct actor *createactor(void);
extern void unpauseactors(void);
extern void pauseactors(void);
extern void clearactors(void);
extern void update(struct actor *);
extern void draw(struct actor *);
extern void startdrawing(void);
extern void finishdrawing(void);
extern void animate(struct actor *);
extern void updatedead(struct actor *);
extern char ymove(struct actor *);
extern char xmove(struct actor *);

extern char ucollision(struct actor *a);
extern char dcollision(struct actor *a);
extern char lcollision(struct actor *a);
extern char rcollision(struct actor *a);
extern void checkcols(void);

extern void updatecontroller(struct controller *);
extern void controllernotify(struct actor *);

extern void updateplayer(struct actor *);
extern void initplayer(struct actor *);

extern void updatebomb(struct actor *);

extern void createbullet(struct actor *);
extern void updatebullet(struct actor *);

extern struct actor *createghost(void);

extern void updatebombcontroller(struct controller *);
extern void updatebombcontroller(struct controller *);