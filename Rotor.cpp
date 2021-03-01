/* This file contains the member function definitions 
   for the Rotor class */

#include "Rotor.hpp"
#include "Wiring.hpp"
#include "errors.h"
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

Rotor::Rotor() :
  forward_wiring_(Wiring()),
  backward_wiring_(Wiring()),
  letter_tracker_(Wiring()),
  notch_array_(nullptr),
  number_of_notches_(0) {}

Rotor::~Rotor()
{
  delete [] notch_array_;
}

int Rotor::setUp(char const* const input_file_name)
{
  ifstream in(input_file_name);
  if (in.fail()) {
    cerr << "Error opening rotor file " << input_file_name << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  
  in >> ws;
  if (in.peek() == EOF) {
    cerr << "Rotor mapping file " << input_file_name << " is empty." << endl;
    in.close();
    return INVALID_ROTOR_MAPPING;
  } else {
    int forward_connections[ALPHABET_LENGTH];
    int backward_connections[ALPHABET_LENGTH];
    int dummy_notch_array[ALPHABET_LENGTH];
    int rotor_error = readRotorInput(forward_connections, dummy_notch_array,
				     in, input_file_name);

    in.close();
    
    if (rotor_error != NO_ERROR) {
      return rotor_error;
    }
    
    forward_wiring_.setUp(forward_connections);
    convertForwardToBackward(forward_connections, backward_connections);
    backward_wiring_.setUp(backward_connections);

    notch_array_ = new int[number_of_notches_];
    for (int i = 0; i < number_of_notches_; i++) {
      notch_array_[i] = dummy_notch_array[i];
    }

  }
  return NO_ERROR;
}

int Rotor::getForwardRotorLetter(int input_letter) const
{
  return forward_wiring_.getOutputLetter(input_letter);
}

int Rotor::getBackwardRotorLetter(int input_letter) const
{
  return backward_wiring_.getOutputLetter(input_letter);
}

int Rotor::getTopLetter() const
{
 return letter_tracker_.getOutputLetter(0);
}

int Rotor::getNumberOfNotches() const
{
  return number_of_notches_;
}

int Rotor::getNotch(int index) const
{
  return notch_array_[index];
}

void Rotor::rotateUp()
{
  // Store the first mappings to be moved to the end of the arrays.
  // The values are also decreased by 1 (except 0 which is set to 25)
  int first_forward_temp = (forward_wiring_.getOutputLetter(A_INDEX)
			    - 1 + ALPHABET_LENGTH) % ALPHABET_LENGTH;
  int first_backward_temp = (backward_wiring_.getOutputLetter(A_INDEX)
			     - 1 + ALPHABET_LENGTH) % ALPHABET_LENGTH;
  int first_letter_temp = letter_tracker_.getOutputLetter(A_INDEX);

  // Move every mapping up one and decrease the value it maps to by 1
  for (int i = 0; i < ALPHABET_LENGTH - 1; i++) {
    int forward_temp = (forward_wiring_.getOutputLetter(i + 1)
			- 1 + ALPHABET_LENGTH) % ALPHABET_LENGTH;
    int backward_temp = (backward_wiring_.getOutputLetter(i + 1)
			 - 1 + ALPHABET_LENGTH) % ALPHABET_LENGTH;
    int letter_temp = letter_tracker_.getOutputLetter(i + 1);
    forward_wiring_.setOutputLetter(i, forward_temp);
    backward_wiring_.setOutputLetter(i, backward_temp);
    letter_tracker_.setOutputLetter(i, letter_temp);
  }

  forward_wiring_.setOutputLetter(Z_INDEX, first_forward_temp);
  backward_wiring_.setOutputLetter(Z_INDEX, first_backward_temp);
  letter_tracker_.setOutputLetter(Z_INDEX, first_letter_temp);
}

void Rotor::rotate(int amount_to_rotate_by)
{
  for (int i = 0; i < amount_to_rotate_by; i++) {
    rotateUp();
  }
}

int Rotor::readRotorInput(int connections[ALPHABET_LENGTH],
			  int dummy_notch_array[ALPHABET_LENGTH],
			  ifstream& in, char const* const file_name)
{
  string number;
  in >> number;

  int i = 0;
  for (; i < ALPHABET_LENGTH && in; i++)
    {
      int numeric_error = checkNumeric(number, file_name);
      if (numeric_error != NO_ERROR) {
	return numeric_error;
      }
      connections[i] = atoi(number.c_str());
      int index_error = checkIndex(connections[i], file_name);
      if (index_error != NO_ERROR) {
	return index_error;
      }
      int repeat_error = checkRepeat(connections, i, file_name);
      if (repeat_error != NO_ERROR) {
	return repeat_error;
      }
      in >> number;
    }

  if (i < ALPHABET_LENGTH) {
    cerr << "Not all inputs mapped in rotor file " << file_name << endl;
    return INVALID_ROTOR_MAPPING;
  }
  if (!in) {
    cerr << "No rotor notch provided in rotor file " << file_name << endl;
    return INVALID_ROTOR_MAPPING;
  }

  i = 0;
  for (; i < ALPHABET_LENGTH && in; i++)
    {
      int numeric_error = checkNumeric(number, file_name, true);
      if (numeric_error != NO_ERROR) {
	return numeric_error;
      }
      dummy_notch_array[i] = atoi(number.c_str());
      int index_error = checkIndex(dummy_notch_array[i], file_name, true);
      if (index_error != NO_ERROR) {
	return index_error;
      }
      int repeat_error = checkRepeat(dummy_notch_array, i, file_name, true);
      if (repeat_error != NO_ERROR) {
	return repeat_error;
      }
      in >> number;
    }

  if (i == ALPHABET_LENGTH && in) {
    cerr << "Too many rotor notches provided in rotor file ";
    cerr << file_name << endl;
    return INVALID_ROTOR_MAPPING;
  }

  number_of_notches_ = i;
      
  return NO_ERROR;
}

int Rotor::checkNumeric(string number_string,
			char const* const file_name,
			bool is_notch) const
{
  for (int i = 0; number_string[i]; i++) {
    if (number_string[i] < ASCII_ZERO || number_string[i] > ASCII_NINE) {
      string component = (is_notch) ? "notch" : "mapping";
      cerr << "Non-numeric character for " << component << " in rotor file ";
      cerr << file_name << endl;
      return NON_NUMERIC_CHARACTER;
    }
  }
  return NO_ERROR;
}

int Rotor::checkIndex(int letter_index, char const* const file_name,
		      bool is_notch) const
{
  if (letter_index < A_INDEX || letter_index > Z_INDEX) {
    string component = (is_notch) ? "notch" : "mapping";
    cerr << "Invalid index for " << component << " in rotor file ";
    cerr << file_name << endl;
    return INVALID_INDEX;
  }
  return NO_ERROR;
}

int Rotor::checkRepeat(int const connections[], int size,
		       char const* const file_name, bool is_notch) const
{
  for (int i = 0; i < size; i++) {
    if (connections[size] == connections[i]) {
      if (is_notch) {
	cerr << "Invalid notch position in rotor file " << file_name;
	cerr << ", cannot have two notches on the same letter." << endl;
      } else {
	cerr << "Invalid mapping of input " << size << " to output ";
	cerr << connections[size] << " (output " << connections[i];
	cerr << " is already mapped to from input " << i;
	cerr << ") in rotor file " << file_name << endl;
      }
      return INVALID_ROTOR_MAPPING;
    }    
  }
  return NO_ERROR;
}

void Rotor::convertForwardToBackward(
    int const forward_connections[ALPHABET_LENGTH],
    int backward_connections[ALPHABET_LENGTH]) const
{
  for (int i = 0; i < ALPHABET_LENGTH; i++) {
    int input_index = forward_connections[i];
    backward_connections[input_index] = i;
  }
}
