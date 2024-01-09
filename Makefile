decrypt : main.o scouting.o createWordsTabs.o manage.o incAndDef.h scouting.h createWordsTabs.h global.h manage.h
	@echo "main"
	gcc -pedantic -Wall -o Output/decrypt.out Output/main.o Output/scouting.o Output/createWordsTabs.o Output/manage.o -lssl -lcrypto -pthread

main.o : manage.o scouting.o createWordsTabs.o incAndDef.h scouting.h global.h createWordsTabs.h manage.h
	@echo "main.o"
	gcc -pedantic -Wall -c main.c -o Output/main.o 

createWordsTabs.o : incAndDef.h global.h createWordsTabs.h 
	@echo "createWordsTabs.o"
	gcc -pedantic -Wall -c createWordsTabs.c -o Output/createWordsTabs.o

scouting.o : incAndDef.h scouting.h global.h
	@echo "scouting.o"
	gcc -pedantic -Wall -c scouting.c -o Output/scouting.o

manage.o : incAndDef.h global.h manage.h
	@echo "manage.o"
	gcc -pedantic -Wall -c manage.c -o Output/manage.o

clean:
	rm -f Output/decrypt.out Output/scouting.o Output/main.o Output/createWordsTabs.o Output/manage.o