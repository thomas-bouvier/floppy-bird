# floppy-bird

## Game

### Installation

#### For Windows (with CodeBlocks) :

Download SDL2 for mingw (in development libraries) from https://www.libsdl.org/download-2.0.php

Extract the folder i686-w64-mingw.

Download SDL2_ttf, SDL2_mixer and SDL2_image the same way.

Create an environment variable named SDLPATH, that contains the path to the SDL folder.

Don't forget to add the necessary .dll files in your project directory (SDL2.dll, SDL2_ttf.dll, SDL2_mixer.dll, SDL2_image.dll, libfreetype-6.dll and libpng16-16.dll)

Now you can compile and run the project with the custom Makefile provided.

#### For Linux :

Install the SDL2 :

	apt-get install libsdl2-dev

Extract the archive and cd to the folder that got extracted. Then :

	./configure
	make
	sudo make install
	
Also install the complementary libraries SDL2_mixer, SDL2_ttf and SDL2_image

	apt-get install libsdl2-ttf-dev
	apt-get install libsdl2-mixer-dev
	apt-get install libsdl2-image-dev

Now you can compile and run the project with the Makefile provided with the sources.

### Configuration

Two configuration file is provided in game/src/conf.

One is made for Windows with Codeblocks, the other for Linux, but feel free to modify these paths if you change anything.

## Robot

### Hardware

The robot is made from 3D printed parts. All parts (STL files) can be found in robot/cad_files/stl_files.

To build the robot, you need the following parts :
* Boitier camera x1
* Pied camera x2
* Support stylet x1
* support_ipad x1
* Raspberry pi 3 x1
* Pi camera V2.0 x1
* Stylus x1
* Servomotor SG90s x1
* M3 screw 22mm x2
* M3 nut x2

**Printing parameters :** 
* Material : PLA
* Layer height : 0,2mm
* Supports : yes

### Configuration

The raspberry pi needs some libraries to get all the programs running properly. Here are the main steps to install the libraries :

**1st step : update and upgrade the firmware**

    sudo apt-get update && apt-get upgrade -y

**2nd step : instalation of the librairies**

    sudo apt-get install -y cmake git gcc g++ libx11-dev libxt-dev libxext-dev libgraphicsmagick1-dev libcv-dev libhighgui-dev

**3rd step : clone userland repository**

    mkdir -p ~/git/raspberrypi
    cd ~/git/raspberrypi
    git clone https://github.com/raspberrypi/userland.git
    cd userland
    ./buildme

**4th step : clone raspicam_cv repository** (allows us to control the Pi camera within the programs)

    mkdir -p ~/git
    cd ~/git
    git clone https://github.com/robidouille/robidouille.git
    cd robidouille/raspicam_cv
    mkdir objs
    make


