#ifndef SCOUTING_H
#define SCOUTING_H

#include "incAndDef.h"
#include "global.h"

void bytes2md5(const char *data, int len, char *md5buf);
void compareHash(char * gess, char * pass);
void basicScounting();
void prefixScounting();
void postfixScounting();
void postfixAndPrefixScounting();
#endif
