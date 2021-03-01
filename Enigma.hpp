#ifndef ENIGMA_H
#define ENIGMA_H

/* The Enigma class contains a Plugboard object, a Reflector 
   object, a pointer to a Rotor object and and integer.
   plugboard_ contains the plugboard mappings.
   reflector_ contains the reflector mappings.
   rotor_array_ is a pointer that can point to an array
   of Rotor objects, depending on how many rotors are input.
   number_of_rotors_ is the number_of_rotors in the Enigma 
   object. */

#include "Plugboard.hpp"
#include "Reflector.hpp"
#include "Rotor.hpp"
#include <fstream>
#include <string>

class Enigma
{
public:
  /* Function to initialise blank enigma machine where
     all the plugboard letters map to themselves,
     all the reflector letters map to themselves
     and there are no rotors. */
  Enigma();
  
  /* Destructor. */
  ~Enigma();

  /* Function to set up enigma machine with each component being
     set up have the mappings given in the configurations files.
     number_of_files determines how many rotors there will be 
     and must be greater than or equal to 3.
     configuration_files is a pointer to an array of pointers, each of
     which point to c-strings. Each c-string is the name of a configuration 
     file. The order of the names must be 'plugboard file' 'reflector file'
     ['rotor file']* 'position file'.
     The function returns an error code corresponding to those in 'errors.h' */
  int setUp(int number_of_files, char const* const* const configuration_files);
  
  /* Function to encode or decode a letter. */
  char code(char letter); 
  
private:
  Plugboard plugboard_;
  Reflector reflector_;
  Rotor* rotor_array_;
  int number_of_rotors_;

  /* Function to position rotors in their starting positions.
     input_file_name is a pointer to a c-string containing the 
     name of the configuration file. 
     The function returns an error code corresponding to those in 'errors.h' */
  int positionRotors(char const* const input_file_name);

  /* Function to check and extract rotor positions from configuration file.
     positions is a pointer to an empty integer array which will be filled up
     with the rotor positions as they are read from the file. 
     in is the input file stream connected to the configuration file. 
     file_name is a pointer to a c-string containing the name of the 
     configuration file.
     The function returns an error code corresponding to those in 'errors.h' */
  int readRotorPositions(int* const positions, std::ifstream& in,
			 char const* const file_name) const;

  /* Function to check if rotor position is a numeric character.
     number_string is the string to be checked.
     file_name is a pointer to a c-string containing the name of the
     configuration file. 
     The function returns an error code corresponding to those in 'errors.h' */
  int checkNumeric(std::string number_string,
		   char const* const file_name) const;

  /* Function to check if rotor position is a valid index between 
     0 and 25.
     letter_index is the number to be checked.
     file_name is a pointer to a c-string containing the name of the
     configuration file.  
     The function returns an error code corresponding to those in 'errors.h' */
  int checkIndex(int letter_index, char const* const file_name) const;

  /* Function to rotate rotors in rotor_array_. */
  void rotateRotors();

  /* Function to check if any notch on the current rotor matches the letter at
     the top of the rotor. If it does, the next rotor is rotated and true is 
     returned, false otherwise.
     rotor_index is the index into the rotor_array_ which contains the rotor 
     to be checked, not the rotor to be rotated. */
  bool rotateSingleRotor(int rotor_index);
};

#endif
