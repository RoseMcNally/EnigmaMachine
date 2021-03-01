#include "Enigma.hpp"
#include "errors.h"
#include "constants.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
  if (argc < 4) {
    cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>)*";
    cerr << " rotor-positions" << endl;
    return INSUFFICIENT_NUMBER_OF_PARAMETERS;
  }
    
  auto enigma = Enigma();
  int error_code = enigma.setUp(argc - 1, argv + 1);
  if (error_code != NO_ERROR) {
    return error_code;
  }

  char next;
  cin >> ws;
  cin.get(next);
  while (next >= ASCII_A && next <= ASCII_Z && cin) {
    cout << enigma.code(next);
    cin >> ws;
    cin.get(next);
  }

  if (next < ASCII_A || next > ASCII_Z) {
    cerr << next << " is not a valid input character (input characters must be";
    cerr << " upper case letters A-Z)!" << endl;
    return INVALID_INPUT_CHARACTER;
  }

  return NO_ERROR;  
}

