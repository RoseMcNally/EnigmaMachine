/* This file contains the member function definitions 
   for the Reflector class */

#include "Reflector.hpp"
#include "Wiring.hpp"
#include "errors.h"
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

Reflector::Reflector() :
  wiring_(Wiring()) {}

int Reflector::setUp(char const* const input_file_name)
{
  ifstream in(input_file_name);
  if (in.fail()) {
    cerr << "Error opening reflector file " << input_file_name << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }

  in >> ws;
  if (in.peek() == EOF) {
    cerr << "Reflector parameter file " << input_file_name;
    cerr << " is empty." << endl;
    in.close();
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  } else {
    int connections[ALPHABET_LENGTH / 2][2];
    int reflector_error = readReflectorInput(connections, in,
					      input_file_name);

    in.close();
    
    if (reflector_error != NO_ERROR) {
      return reflector_error;
    }
    
    wiring_.setUp(connections, ALPHABET_LENGTH / 2);
  }
  
  return NO_ERROR;
}

int Reflector::getReflectorLetter(int input_letter) const
{
  return wiring_.getOutputLetter(input_letter);
}

int Reflector::readReflectorInput(int connections[ALPHABET_LENGTH / 2][2],
				  ifstream& in,
				  char const* const file_name) const
{
  string first_number, second_number;
  in >> first_number >> second_number;
  bool in_is_open = true; // Every loop, this bool will be set to true if
                          // the first_number read in successfully.
  
  int i = 0;
  for (; i < ALPHABET_LENGTH / 2 && in; i++)
    {
      int numeric_error = checkNumeric(first_number, file_name);
      if (numeric_error != NO_ERROR) {
	return numeric_error;
      }
      numeric_error = checkNumeric(second_number, file_name);
      if (numeric_error != NO_ERROR) {
	return numeric_error;
      }

      connections[i][0] = atoi(first_number.c_str());
      connections[i][1] = atoi(second_number.c_str());

      if (connections[i][0] == connections[i][1]) {
	cerr << "Invalid mapping of " << connections[i][0] << " to ";
	cerr << connections[i][1] << " in reflector file " << file_name << endl;
	return INVALID_REFLECTOR_MAPPING;
      }
      
      int index_error = checkIndex(connections[i][0], file_name);
      if (index_error != NO_ERROR) {
	return index_error;
      }
      index_error = checkIndex(connections[i][1], file_name);
      if (index_error != NO_ERROR) {
	return index_error;
      }
      int repeat_error = checkRepeat(connections, i, file_name);
      if (repeat_error != NO_ERROR) {
	return repeat_error;
      }
      
      in >> first_number;
      in_is_open = (in) ? true : false;
      in >> second_number;
    }

  if (!(i == ALPHABET_LENGTH / 2 && !in_is_open)) { 
    if (i == ALPHABET_LENGTH / 2) {
      cerr << "Too many parameters in reflector file " << file_name << endl;
    } else {
      if (in_is_open) {
	cerr << "Incorrect (odd) number of parameters in reflector file ";
	cerr << file_name << endl;
      } else {
	cerr << "Insufficient number of mappings in reflector file ";
	cerr << file_name << endl;
      }
    }
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS; 
  }
  
  return NO_ERROR;
}

int Reflector::checkNumeric(string number_string,
			    char const* const file_name) const
{
  for (int i = 0; number_string[i]; i++) {
    if (number_string[i] < ASCII_ZERO || number_string[i] > ASCII_NINE) {
      cerr << "Non-numeric character in reflector file " << file_name << endl;
      return NON_NUMERIC_CHARACTER;
    }
  }
  return NO_ERROR;
}

int Reflector::checkIndex(int letter_index,
			  char const* const file_name) const
{
  if (letter_index < A_INDEX || letter_index > Z_INDEX) {
    cerr << "Invalid index in reflector file " << file_name << endl;
    return INVALID_INDEX;
  }
  return NO_ERROR;
}

int Reflector::checkRepeat(int const connections[][2], int size,
			   char const* const file_name) const
{
  for (int i = 0; i < size; i++) { // Iterate over rows
    for (int j = 0; j < 2; j++) { // Iterate over columns in final row
      for (int k = 0; k < 2; k++) { // Iterate over columns in comparison row
	if (connections[size][j] == connections[i][k]) {
	  cerr << "Invalid mapping of " << connections[size][(j + 1) % 2];
	  cerr << " to " << connections[size][j] << " (";
	  cerr << connections[size][j] << " is already mapped to from ";
	  cerr << connections[i][(k + 1) % 2] << ") in reflector file ";
	  cerr  << file_name << endl;
	  return INVALID_REFLECTOR_MAPPING;
	}
      }
    }
  }
  return NO_ERROR;
}
