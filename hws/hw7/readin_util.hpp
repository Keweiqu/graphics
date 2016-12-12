#ifndef READIN_UTIL
#define READIN_UTIL
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "common.hpp"
#include "extern.hpp"
#include "Camera.hpp"
using namespace std;

void readin(char* file);
void get_next_line(ifstream& source, string line, stringstream& stream);
void readin_debugg_output();
#endif
