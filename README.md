# floppy-bird

## Game

### Installation

#### For Windows (with CodeBlocks) :

Take care to install SDL version 2. 

In order to compile with CodeBlocks, you have to add a PATH environement variable, that leads to the SDL2 folder.

Then open the project with Codeblocks (.cbp file), build, compile and run.

#### For Linux :

Install the SDL2 :

	apt-get install libsdl2-dev

Extract the archive and cd to the folder that got extracted. Then :

	./configure
	make
	sudo make install

Now you can compile and run the project with the Makefile provided with the sources.

### Configuration

A configuration file is provided in game/src/conf.

The paths for the game (level, sprites...) are made by default for CodeBlocks, but feel free to modify these paths if you use an other configuration.

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


