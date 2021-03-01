#ifndef REFLECTOR_H
#define REFLECTOR_H

/* The Reflector class contains a single Wiring object, which
   contains the Reflector mappings. 
   The reflector mapping consists of 13 pairs of letters, where the
   letters in the pair map to eachother. 
   Letters cannot map to themselves. */

#include "Wiring.hpp"
#include "constants.h"
#include <fstream>
#include <string>

class Reflector
{
public:
  /* Function to initialise Reflector object to have all letters mapping
   to themselves. */
  Reflector();
  
  /* Function to set up Reflector object so that all letters are paired.
     The pairs are read from the configuration file. 
     input_file_name is a pointer to a c-string containing
     the name of the configuration file.
     The function checks the configuration file is well formed
     and returns an error code corresponding to those in the
     'errors.h' file. */
  int setUp(char const* const input_file_name);

  /* Function to return the output letter index that the input 
     letter index maps to. */
  int getReflectorLetter(int input_letter) const;
  
private:
  Wiring wiring_;

  /* Function to check and extract reflector input from configuration file.
     connections is an empty 13x2 array which is filled up with 
     the mappings given in the configuration file. 
     in is the input file stream connected to the configuration file. 
     file_name is a pointer to a c-string containing the name of the
     configuration file. 
     The function returns an error code corresponding to those in 'errors.h' */
  int readReflectorInput(int connections[ALPHABET_LENGTH / 2][2],
			 std::ifstream& in,
			  char const* const file_name) const;

  /* Function to check if reflector input is a numeric character.
     number_string is the string to be checked.
     file_name is a pointer to a c-string containing the name of the
     configuration file. 
     The function returns an error code corresponding to those in 'errors.h' */
  int checkNumeric(std::string number_string,
		   char const* const file_name) const;

  /* Function to check if reflector input is a valid index between 
     0 and 25.
     letter_index is the number to be checked.
     file_name is a pointer to a c-string containing the name of the
     configuration file. 
     The function returns an error code corresponding to those in 'errors.h' */
  int checkIndex(int letter_index, char const* const file_name) const;

  /* Function to check if the reflector input is a repeat of a 
     previous one.
     connections is a two-dimensional array containing the mappings
     which have been read from the configuration file so far.
     size is the number of mapping pairs which have been read in so far.
     file_name is a pointer to a c-string containing the name of the
     configuration file. 
     The function returns an error code corresponding to those in 'errors.h' */
  int checkRepeat(int const connections[][2], int size,
		  char const* const file_name) const;
  
};

#endif
