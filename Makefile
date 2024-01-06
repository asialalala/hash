decrypt : main.o scouting.o createWordsTabs.o incAndDef.h scouting.h createWordsTabs.h global.h 
	@echo "main"
	gcc -pedantic -Wall -o Output/decrypt.out Output/main.o Output/scouting.o Output/createWordsTabs.o -lssl -lcrypto -pthread

main.o : scouting.o createWordsTabs.o incAndDef.h scouting.h global.h createWordsTabs.h
	@echo "main.o"
	gcc -pedantic -Wall -c main.c -o Output/main.o 

createWordsTabs.o : incAndDef.h global.h createWordsTabs.h 
	@echo "createWordsTabs.o"
	gcc -pedantic -Wall -c createWordsTabs.c -o Output/createWordsTabs.o

scouting.o : incAndDef.h scouting.h global.h
	@echo "scouting.o"
	gcc -pedantic -Wall -c scouting.c -o Output/scouting.o

clean:
	rm -f Output/decrypt.out Output/scouting.o Output/main.o Output/createWordsTabs.o