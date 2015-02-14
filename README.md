# etchasketch-stepper
Code for a Stepper Motor and Arduino based Etch-a-Sketch automation rig.

This code is for an experimental and hacky Etch-a-Sketch automation project. The MATLAB/Octave functions convert an image file into a long vector of directions to move at (manual input may be required). The directions created can be stuck onto some provided Arduino code to control the stepper motors.

## Required Software/Hardware

- A DRV8805 or similar based dual single pole stepper motor controller
- An Arduino, if you want to use my Arduino sketch.
- Some mechanical rig that couples stepper motor shaft to movement of the etch-a-sketch knobs. 
- Octave or MATLAB. Confirmed to work on Octave and the student edition of MATLAB. 

## Usage

In the `mfiles` folder, you will see `stepperinstructions.m` and `instructions2array.m`.

`stepperinstructions.m` accepts three arguments and outputs one argument.

`stepperinstructions(im, starty, startx)` where `im` is an image array of dimension 3, typically read from something like `imread` with black lines to be traced. `starty` and `startx` specify where the stepping should start. This should typically be where the "penstrokes" begin. 

If the script encounters a pixel which has multiple other pixels adjacent to it, it will ask you to pick which pixel to advance next. Use `qwe ; asd ; zxc` keys to specify which pixel you want to advance, with `q` being the pixel to the upper left corner, and so on. 

If your image has an unfortunate pixel junction in a way that the program can't come back, you will need to modify the image to work with this program.

Once you get the raw instructions in the form of 1,2,3,...,9, you can convert that to a C-style compile-time array declaration using `instructions2array.m`. Just input the instructions into the m-file, and your first output will be a string that has a C-style declaration, and the second output will be the number of elements, in case you need it.

You can then put those numbers into your Arduino sketch or whatever uc you will be using.
