#ifndef SCOUTING_H
#define SCOUTING_H

#include "incAndDef.h"
#include "global.h"

void bytes2md5(const char *data, int len, char *md5buf);
bool compareHash(char * gess, char * pass, long passID);
void basicScounting(char ** tab, int wordID, long prodNr, long passID);
void prefixScounting(char ** tab, int wordID, long prodNr, long passID);
void postfixScounting(char ** tab, int wordID, long prodNr, long passID);
// void postfixAndPrefixScounting(char ** tab, int wordID);
void* scouting(void *arg); // funkcja producenta
#endif
