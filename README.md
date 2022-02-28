Forest Flicker
==============



https://user-images.githubusercontent.com/39333880/212460405-fe8daa85-90e9-456c-9e74-87d7ac806e46.mp4




Forest Flicker is a single screen platformer for the NEC PC Engine.

Collect stars while dodging and shooting enemies.

It uses the HuC toolchain with a few modifications. In a few places
a few places the code gets a little strained trying to work with HuC.
The engine is flexible enough that adding more levels and characters
should be quite simple.

Game Controls
--------

* Run: Begin the game / Pause
* I Button: Fly
* II Button: Shoot


Usage
-----

`flicker.pce` can be played with PC Engine emulator or on real
hardware with a flashcart.

Building
--------

Levels are edited with the [Mappy](https://tilemap.co.uk/mappy.php)
map editor.

Sprites are edited with [Aseprite](https://www.aseprite.org). It is
also used to create sprite sheets for HuC.

Flicker is built with the [HuC](https://github.com/iamjpn/huc) toolchain.

To build the flicker.pce ROM:

	$ make AS=../huc/bin/pceas \
	  CC=../huc/bin/huc \
	  INC=../huc/include/pce \
	  ASEPRITE=/Applications/Aseprite.app/Contents/MacOS/aseprite

