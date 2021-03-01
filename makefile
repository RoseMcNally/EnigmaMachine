enigma: Wiring.o Plugboard.o Reflector.o Rotor.o Enigma.o main.o
	g++ -Wall -Wextra -g Wiring.o Plugboard.o Reflector.o Rotor.o Enigma.o main.o -o enigma

Wiring.o: Wiring.cpp Wiring.hpp errors.h
	g++ -c -Wall -Wextra -g Wiring.cpp -o Wiring.o

Plugboard.o: Plugboard.cpp Plugboard.hpp Wiring.hpp errors.h
	g++ -c -Wall -Wextra -g Plugboard.cpp -o Plugboard.o

Reflector.o: Reflector.cpp Reflector.hpp Wiring.hpp errors.h
	g++ -c -Wall -Wextra -g Reflector.cpp -o Reflector.o

Rotor.o: Rotor.cpp Rotor.hpp Wiring.hpp errors.h
	g++ -c -Wall -Wextra -g Rotor.cpp -o Rotor.o

Enigma.o: Enigma.cpp Enigma.hpp Plugboard.hpp Rotor.hpp Reflector.hpp errors.h
	g++ -c -Wall -Wextra -g Enigma.cpp -o Enigma.o

main.o: main.cpp Enigma.hpp errors.h
	g++ -c -Wall -Wextra -g main.cpp -o main.o

clean:
	rm *.o enigma
