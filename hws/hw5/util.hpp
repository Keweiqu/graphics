#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <fstream>
#include <sstream>
#include "common.hpp"
#include "meshManager.hpp"
using namespace std;

int readFile(meshManager* mesh, int argc, char* argv[]);
#endif
