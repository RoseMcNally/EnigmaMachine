#ifndef ROTOR_H
#define ROTOR_H

/* The Rotor class contains three Wiring objects, a pointer and an integer.
   forward_wiring_ contains the mappings when moving from the plugboard to
   the reflector.
   backward_wiring_ contains the mappings when moving from the reflector
   to the plugboard. 
   These are different because the letters do not have to be paired, any 
   letter can map to any letter, as long as every letter is mapped to and 
   from exactly once.
   letter_tracker_ contains the positions of each letter as the rotor rotates.
   notch_array_ points to an integer array which contains the notch positions
   for the rotor. 
   number_of_notches_ is the number of notches that the rotor has.*/

#include "Wiring.hpp"
#include "constants.h"
#include <fstream>
#include <string>

class Rotor
{
 public:
  /* Function to initialise Rotor object to have all letters
     mapping to themselves, and no notches.*/
  Rotor();

  /* Destructor. */
  ~Rotor();

  /* Function to set up Rotor object with mappings and notches given
     in the configuration file. 
     input_file_name is a pointer to a c-string containing
     the name of the configuration file.
     The function checks the configuration file is well formed
     and returns an error code corresponding to those in the
     'errors.h' file. */
  int setUp(char const* const input_file_name);

  /* Function to return the output letter index that the rotor maps the 
     input letter index to in the forward direction. */
  int getForwardRotorLetter(int input_letter) const;

  /* Function to return the output letter index that the rotor maps the 
     input letter index to in the backward direction. */
  int getBackwardRotorLetter(int input_letter) const;

  /* Function to return letter at the absolute A position. */
  int getTopLetter() const;

  /* Function to get number of notches. */
  int getNumberOfNotches() const;

  /* Function to get notch in notch array element accessed by index. */
  int getNotch(int index) const;

  /* Function to rotate rotor 'up' by 1 position (position 1 moves to 
     position 0). */
  void rotateUp();

  /* Function to rotate rotor by number of letters given. */
  void rotate(int amount_to_rotate_by);


 private:
  Wiring forward_wiring_;
  Wiring backward_wiring_;
  Wiring letter_tracker_;
  int* notch_array_;
  int number_of_notches_;

  /* Function to check and extract rotor input from configuration file. 
     connections is an empty 26 element array which is filled up with 
     the mappings given in the configuration file.
     dummy_notch_array is an empty 26 element array which is filled up
     with the notch positions given in the configuration file.
     in is the input file stream connected to the configuration file. 
     file_name is a pointer to a c-string containing the name of the
     configuration file. 
     The function returns an error code corresponding to those in 'errors.h' */
  int readRotorInput(int connections[ALPHABET_LENGTH],
		     int dummy_notch_array[ALPHABET_LENGTH],
		     std::ifstream& in,
		     char const* const file_name);

  /* Function to check if rotor input is a numeric character.
     number_string is the string to be checked.
     file_name is a pointer to a c-string containing the name of the
     configuration file. 
     is_notch is a boolean which should be set to true if a notch input
     is being checked. 
     The function returns an error code corresponding to those in 'errors.h' */
  int checkNumeric(std::string number_string,
		   char const* const file_name,
		   bool is_notch = false) const;

  /* Function to check if rotor input is a valid index between 
     0 and 25.
     letter_index is the number to be checked.
     file_name is a pointer to a c-string containing the name of the
     configuration file. 
     is_notch is a boolean which should be set to true if a notch input
     is being checked. 
     The function returns an error code corresponding to those in 'errors.h' */
  int checkIndex(int letter_index, char const* const file_name,
		 bool notch = false) const;

  /* Function to check if the rotor input is a repeat of a 
     previous one.
     connections is an array containing the inputs
     which have been read from the configuration file so far.
     size is the number of mapping pairs which have been read in so far.
     file_name is a pointer to a c-string containing the name of the
     configuration file.
     is_notch is a boolean which should be set to true if a notch input
     is being checked. 
     The function returns an error code corresponding to those in 'errors.h' */
  int checkRepeat(int const connections[],
		  int size,
		  char const* const file_name,
		  bool notch = false) const;

  /* Function to convert forward mapping to equivalent backward mapping. 
   forward_connections is a 26 element array containing the rotor mappings
   read from the configuration file. 
   backward_connections is an empty 26 element array which will be filled
   with the backward mappings. */ 
  void convertForwardToBackward(int const forward_connections[ALPHABET_LENGTH],
				int backward_connections[ALPHABET_LENGTH]) const;
};

#endif
