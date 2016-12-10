#ifndef READIN_UTIL
#define READIN_UTIL
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "common.hpp"

using namespace std;

void readin(char* file);
void get_next_line(ifstream& source, string line, stringstream& stream);
#endif
