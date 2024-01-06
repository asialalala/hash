decrypt : main.o scouting.o incAndDef.h scouting.h global.h
	@echo "main"
	gcc -pedantic -Wall -o Output/decrypt.out Output/main.o Output/scouting.o -lssl -lcrypto -pthread

main.o : scouting.o incAndDef.h scouting.h global.h
	@echo "main.o"
	gcc -pedantic -Wall -c main.c -o Output/main.o 

scouting.o : incAndDef.h scouting.h global.h
	@echo "scouting.o"
	gcc -pedantic -Wall -c scouting.c -o Output/scouting.o

clean:
	rm -f Output/decrypt.out Output/scouting.o Output/main.o