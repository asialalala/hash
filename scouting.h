#ifndef SCOUTING_H
#define SCOUTING_H

#include "incAndDef.h"
#include "global.h"

void bytes2md5(const char *data, int len, char *md5buf);
long compareHash(char * gess, char * pass);
void basicScounting(char ** tab, int wordID);
void prefixScounting(char ** tab, int wordID);
void postfixScounting(char ** tab, int wordID);
void postfixAndPrefixScounting(char ** tab, int wordID);
void* scouting(void *arg); // funkcja producenta
#endif
