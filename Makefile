main: 
	gcc -pedantic -Wall main.c -o decrypt.out -lssl -lcrypto -pthread

clean:
	rm -f decrypt.out