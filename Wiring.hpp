#ifndef WIRING_H
#define WIRING_H

/* The wiring class contains an integer array with 26 elements. 
   The elements contain the numbers 0 to 25 corresponding to 
   the zero-based indices into the alphabet.
   The element index represents the input letter.
   The integer contained in the element represents the output 
   letter. */

#include "constants.h"

class Wiring
{
public:
  /* Function to initialise Wiring object to have
     all letters map to themselves. */
  Wiring();
  
  /* Function to set up Wiring object with
     letters mapping to those given in the input array. 
     To use when setting up rotors. 
     The input array must contain every number from 0 to 25
     exactly once. The position of the number in the array
     corresponds the input letter index and the number itself
     corresponds to the output letter index. */
  void setUp(int const wires_to_swap[ALPHABET_LENGTH]);

  /* Function to set up Wiring object with
     letters mapping to those given in the input array.
     To use when setting up plugboards and reflectors. 
     The input array must contain distinct numbers from 
     0 to 25. Each row contains a pair of numbers
     which will be mapped to eachother. */
  void setUp(int const wires_to_swap[][2], int size);

  /* Function to return the output letter index that the 
     input letter index maps to. */
  int getOutputLetter(int input_letter) const;

  /* Function to set the output letter index that the 
     input letter index maps to. */
  void setOutputLetter(int input_letter, int output_letter);
  
private:
  int mapping_[ALPHABET_LENGTH];

};

#endif
