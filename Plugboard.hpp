#ifndef PLUGBOARD_H
#define PLUGBOARD_H

/* The Plugboard class contains a single Wiring object, which
   contains the Plugboard mappings. 
   Plugboard letters can map to themselves, or two letters can 
   be paired and map to eachother. */

#include "Wiring.hpp"
#include "constants.h"
#include <fstream>
#include <string>

class Plugboard
{
 public:
  /* Function to initialise Plugboard object to have
     all letters mapping to themselves. */
  Plugboard();

  /* Function to set up Plugboard object with mappings given
     in the configuration file. 
     input_file_name is a pointer to a c-string containing
     the name of the configuration file.
     The function checks the configuration file is well formed
     and returns an error code corresponding to those in the
     'errors.h' file. */
  int setUp(char const* const input_file_name);

  /* Function to return the output letter index that the input letter
     index maps to. */
  int getPlugboardLetter(int input_letter) const;
  
 private:
  Wiring wiring_;

    /* Function to check and extract plugbaord input from configuration file. 
     size is an integer which counts the number of pairs of mappings in the 
     configuration file. It must be set to zero before the function is called.
     connections is an empty 13x2 array which is filled up with 
     the mappings given in the configuration file.
     in is the input file stream connected to the configuration file. 
     file_name is a pointer to a c-string containing the name of the
     configuration file. 
     The function returns an error code corresponding to those in 'errors.h' */
  int readPlugboardInput(int& size,
			 int connections[ALPHABET_LENGTH / 2][2],
			 std::ifstream& in,
			 char const* const file_name) const;

  /* Function to check if plugboard input is a numeric character.
     number_string is the string to be checked.
     file_name is a pointer to a c-string containing the name of the
     configuration file. 
     The function returns an error code corresponding to those in 'errors.h' */
  int checkNumeric(std::string number_string,
		   char const* const file_name) const;

  /* Function to check if plugboard input is a valid index between 
     0 and 25.
     letter_index is the number to be checked.
     file_name is a pointer to a c-string containing the name of the
     configuration file. 
     The function returns an error code corresponding to those in 'errors.h' */
  int checkIndex(int letter_index, char const* const file_name) const;
  
  /* Function to check if the plugboard input is a repeat of a 
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
