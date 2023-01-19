CC=gcc
WFLAGS=-Wall -Wextra -std=c++17


compileAllC:
	$(CC) -Wall -Wextra -std=c99 *.c -o ash 

compileAllCPP:
	$(CC) $(WFLAGS) *.cpp -o ash -lstdc++


OneFile: 
	$(CC) $(WFLAGS) $(filter-out $@ , $(MAKECMDGOALS)) -o a.out -lstdc++


clean:
	rm -f *.o
