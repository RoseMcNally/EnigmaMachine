/* This file contains the member function definitions 
   for the Plugboard class */

#include "Plugboard.hpp"
#include "Wiring.hpp"
#include "errors.h"
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

Plugboard::Plugboard() :
  wiring_(Wiring()) {}

int Plugboard::setUp(char const* const input_file_name)
{
  ifstream in(input_file_name);
  if (in.fail()) {
    cerr << "Error opening plugboard file " << input_file_name << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }

  in >> ws;
  if (in.peek() == EOF) {
    in.close();
    return NO_ERROR;
  } else {
    int size = 0;
    int connections[ALPHABET_LENGTH / 2][2];
    int plugboard_error = readPlugboardInput(size, connections,
					     in, input_file_name);
    
    in.close();

    if (plugboard_error != NO_ERROR) {
      return plugboard_error;
    }
    
    wiring_.setUp(connections, size);
  }
  
  return NO_ERROR;
}

int Plugboard::getPlugboardLetter(int input_letter) const
{
  return wiring_.getOutputLetter(input_letter);
}

int Plugboard::readPlugboardInput(int& size,
				  int connections[ALPHABET_LENGTH / 2][2],
				  ifstream& in,
				  char const* const file_name) const
{
  string first_number, second_number;
  in >> first_number >> second_number;
  bool in_is_open = true; // Every loop, this bool will be set to true if
                          // the first_number read in successfully.

  for (; in && size < ALPHABET_LENGTH / 2; size++)
    {
      int numeric_error = checkNumeric(first_number, file_name);
      if (numeric_error != NO_ERROR) {
	return numeric_error;
      }
      numeric_error = checkNumeric(second_number, file_name);
      if (numeric_error != NO_ERROR) {
	return numeric_error;
      }
      
      connections[size][0] = atoi(first_number.c_str());
      connections[size][1] = atoi(second_number.c_str());

      if (connections[size][0] == connections[size][1]) {
	cerr << "Invalid mapping of " << connections[size][0] << " to ";
	cerr << connections[size][1] << " in plugboard file ";
	cerr << file_name << endl;
	return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
      }
      
      int index_error = checkIndex(connections[size][0], file_name);
      if (index_error != NO_ERROR) {
	return index_error;
      }
      index_error = checkIndex(connections[size][1], file_name);
      if (index_error != NO_ERROR) {
	return index_error;
      }
      
      int repeat_error = checkRepeat(connections, size, file_name);
      if (repeat_error != NO_ERROR) {
	return repeat_error;
      }
      
      in >> first_number;
      in_is_open = (in) ? true : false;
      in >> second_number;
    }

  if (in_is_open) { // Indicates an odd number of mappings or
                    // too many mappings in configuration file 
    cerr << "Incorrect number of parameters in plugboard file ";
    cerr << file_name << endl;
    return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
  }
  
  return NO_ERROR;
}

int Plugboard::checkNumeric(string number_string,
			    char const* const file_name) const
{
  for (int i = 0; number_string[i]; i++) {
    if (number_string[i] < ASCII_ZERO || number_string[i] > ASCII_NINE) {
      cerr << "Non-numeric character in plugboard file " << file_name << endl;
      return NON_NUMERIC_CHARACTER;
    }
  }
  return NO_ERROR;
}

int Plugboard::checkIndex(int letter_index,
			  char const* const file_name) const
{
  if (letter_index < A_INDEX || letter_index > Z_INDEX) {
    cerr << "Invalid index in plugboard file " << file_name << endl;
    return INVALID_INDEX;
  }
  return NO_ERROR;
}

int Plugboard::checkRepeat(int const connections[][2], int size,
			   char const* const file_name) const
{
  for (int i = 0; i < size; i++) { // iterates over rows
    for (int j = 0; j < 2; j++) { // iterates over columns of final row
      for (int k = 0; k < 2; k++) { // iterates over columns of comparison row
	if (connections[size][j] == connections[i][k]) {
	  cerr << "Invalid mapping of " << connections[size][(j + 1) % 2];
	  cerr << " to " << connections[size][j] << " (";
	  cerr << connections[size][j] << " is already mapped to from ";
	  cerr << connections[i][(k + 1) % 2] << ") in plugboard file ";
	  cerr  << file_name << endl;
	  return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
	}
      }
    }
  }
  return NO_ERROR;   
}
