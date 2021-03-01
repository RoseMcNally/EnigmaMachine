/* This file contains the member function definitions 
   for the Enigma class */

#include "Enigma.hpp"
#include "Plugboard.hpp"
#include "Reflector.hpp"
#include "Rotor.hpp"
#include "errors.h"
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

Enigma::Enigma() :
  plugboard_(Plugboard()),
  reflector_(Reflector()),
  rotor_array_(nullptr),
  number_of_rotors_(0) {}

Enigma::~Enigma()
{
  delete [] rotor_array_;
}

int Enigma::setUp(int number_of_files,
		  char const* const* const configuration_files)
{
  number_of_rotors_ = number_of_files - 3;

  int plugboard_error = plugboard_.setUp(configuration_files[0]);
  if (plugboard_error != NO_ERROR) {
    return plugboard_error;
  }

  int reflector_error = reflector_.setUp(configuration_files[1]);
  if (reflector_error != NO_ERROR) {
    return reflector_error;
  }

  if (number_of_rotors_ == 0) {
    return NO_ERROR;
  } else {
    rotor_array_ = new Rotor[number_of_rotors_];

    for (int i = 0; i < number_of_rotors_; i++) {
      int rotor_error = rotor_array_[i].setUp(configuration_files[i + 2]);
      if (rotor_error != NO_ERROR) {
	return rotor_error;
      }
    }

    int position_error =
      positionRotors(configuration_files[number_of_files - 1]);
    if (position_error != NO_ERROR) {
      return position_error;
    }
  }
  
  return NO_ERROR;
}

char Enigma::code(char letter)
{
  int letter_index = static_cast<int>(letter) - ASCII_A;
  
  rotateRotors();

  letter_index = plugboard_.getPlugboardLetter(letter_index);
  
  for (int i = number_of_rotors_; i > 0; i--) {
    letter_index = rotor_array_[i - 1].getForwardRotorLetter(letter_index);
  }
  
  letter_index = reflector_.getReflectorLetter(letter_index);
  
  for (int i = 0; i < number_of_rotors_; i++) {
    letter_index = rotor_array_[i].getBackwardRotorLetter(letter_index);
  }
  
  letter_index = plugboard_.getPlugboardLetter(letter_index);    

  return static_cast<char>(letter_index + ASCII_A);  
}

int Enigma::positionRotors(char const* const input_file_name)
{
  ifstream in(input_file_name);
  if (in.fail()) {
    cerr << "Error opening rotor position file " << input_file_name << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }

  in >> ws;
  if (in.peek() == EOF) {
    cerr << "No starting position for rotor 0 in rotor position file ";
    cerr << input_file_name << endl;
    in.close();
    return NO_ROTOR_STARTING_POSITION;
  } else {
    auto positions = new int[number_of_rotors_];
    int position_error = readRotorPositions(positions, in, input_file_name);

    in.close();
    
    if (position_error != NO_ERROR) {
      delete positions;
      return position_error;
    }
    
    for (int j = 0; j < number_of_rotors_; j++) {
      rotor_array_[j].rotate(positions[j]);
    }
    
    delete positions;
  }
  
  return NO_ERROR;
}

int Enigma::readRotorPositions(int* const positions, ifstream& in,
			       char const* const file_name) const
{
  string number_string;
  in >> number_string;

  int i = 0;
  for (; i < number_of_rotors_ && in; i++) {
    
    int numeric_error = checkNumeric(number_string, file_name);
    if (numeric_error != NO_ERROR) {
      return numeric_error;
    }
    
    positions[i] = atoi(number_string.c_str());

    int index_error = checkIndex(positions[i], file_name);
    if (index_error != NO_ERROR) {
      return index_error;
    }
    
    in >> number_string;
  }

  if (i != number_of_rotors_) {
    cerr << "No starting position for rotor " << (i - 1);
    cerr << " in rotor position file " << file_name << endl;
    return NO_ROTOR_STARTING_POSITION;
  } if (in) {
    cerr << "Too many rotor starting positions in position file ";
    cerr << file_name << endl;
    return NO_ROTOR_STARTING_POSITION;
  }
  
  return NO_ERROR;
}

int Enigma::checkNumeric(string number_string,
			 char const* const file_name) const
{
  for (int i = 0; number_string[i]; i++) {
    if (number_string[i] < ASCII_ZERO || number_string[i] > ASCII_NINE) {
      cerr << "Non-numeric character in rotor position file ";
      cerr << file_name << endl;
      return NON_NUMERIC_CHARACTER;
    }
  }
  return NO_ERROR;
}

int Enigma::checkIndex(int letter_index, char const* const file_name) const
{
  if (letter_index < A_INDEX || letter_index > Z_INDEX) {
    cerr << "Invalid index in rotor position file " << file_name << endl;
    return INVALID_INDEX;
  }
  return NO_ERROR;
}

void Enigma::rotateRotors()
{
  if (rotor_array_ != nullptr) {
    rotor_array_[number_of_rotors_ - 1].rotateUp();

    bool is_still_rotating = true;
    
    for (int i = number_of_rotors_ - 1; i > 0 && is_still_rotating; i--) {
      is_still_rotating = rotateSingleRotor(i);
    }
  }
}

bool Enigma::rotateSingleRotor(int rotor_index)
{
  for (int i = 0; i < rotor_array_[rotor_index].getNumberOfNotches(); i++) {
    if (rotor_array_[rotor_index].getNotch(i) ==
	rotor_array_[rotor_index].getTopLetter()) {
      rotor_array_[rotor_index - 1].rotateUp();
      return true;
    }
  }
  return false;
}
