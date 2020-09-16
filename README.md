# sleep-timer

A project to make an IoT sleep timing device.
This is the the device code.

## Device

This code is writted to work on a [Particle Argon](https://store.particle.io/collections/wifi/products/argon), but should also work on other Particle boards.

![Argon Board](https://github.com/jaydlawrence/sleep-timer/blob/master/media/argon-board.png)

## Compiling the project

When you're ready to compile the project, make sure you have the correct Particle device target selected and run `particle compile <platform>` in the CLI or click the Compile button in the Desktop IDE. The following files in your project folder will be sent to the compile service:

- Everything in the `/src` folder, including the `.ino` application file
- The `project.properties` file for the project
- Any libraries stored under `lib/<libraryname>/src`
- Images etc used in the readme are stored under `/media` but have no affect on functionality

For development, the [Particle Workbench](https://docs.particle.io/workbench/) plugin for Visual Studio is recommended.
It has a user interface that allows for selecting target device, installing plugins, compiling and flashing all with a few button clicks.

To get this project running on a particle board:

- Clone project locally
- Select target board using the workbench plugin
- Compile code for target
- Flash the code to device

## Wiring

This is an example from an earlier version of this project with less blue LEDs

![Picture of wiring on dev board](https://github.com/jaydlawrence/sleep-timer/blob/master/media/dev-board.jpg)

The LEDs for this project are hooked up to digital pins 0 to 6.
The blue LEDs are on 0 to 5 and the sun LED(s) on pin 6.
Pin 0 and 1 are currently paired and used for when the device is put into sleep timing mode.
From there, pins 2 to 5 are lit as the sleep timer progresses until the sleep time is over and the sun on pin 6 is lit.

Each LED was wired from the respective pin through a 47 Ohm resistor, then through the LED and back to the ground terminal on the board.
Alternatively, each LED can be connected directly to the board on the Anode side and then on the way to ground, they can all be connected through a single resistor as below.


This is how I wired it up and it saved on some resistors.
The downside is the more LEDs are on, the less bright each LED will be, but in practice, the difference is negligible.

![Wiring diagram](https://github.com/jaydlawrence/sleep-timer/blob/master/media/board-layout.png)

From there you can add it to a container of some sort.
