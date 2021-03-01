/* This file contains the member function definitions 
   for the Wiring class */

#include "Wiring.hpp"
#include "constants.h"

using namespace std;

Wiring::Wiring()
{
  for (int i = 0; i < ALPHABET_LENGTH; i++){
    mapping_[i] = i;
  }
}

void Wiring::setUp(int const wires_to_swap[ALPHABET_LENGTH])
{
  for (int i = 0; i < ALPHABET_LENGTH; i++){
    mapping_[i] = wires_to_swap[i];
  }
}

void Wiring::setUp(int const wires_to_swap[][2], int size)
{
  for (int i = 0; i < size; i++){
    mapping_[wires_to_swap[i][0]] = wires_to_swap[i][1];
    mapping_[wires_to_swap[i][1]] = wires_to_swap[i][0];
  }
}

int Wiring::getOutputLetter(int input_letter) const
{
  return mapping_[input_letter];
}

void Wiring::setOutputLetter(int input_letter, int output_letter)
{
  mapping_[input_letter] = output_letter;
}

