# EnigmaMachine

C++ project to build an Engima Machine for the command line.

### Usage

Download the code and compile the executable according to the makefile instructions. 

Call the main function by running `enigma plugboard-file reflector-file (<rotor-file>)* rotor-positions`. Each command line argument should refer to a file in the data files provided. For example:

```
enigma plugboards/II.pb reflectors/IV.rf rotors/VI.rot rotors/II.rot rotors/II.rot rotors/III.pos
```

Play around with the files :) You can include as many or as few rotors as you like, and you can make your own data files too!

Also, check out the header files ot see how the model is designed.
