#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include "DataBlock.h"

DataBlock* ReadFile(const std::string& filename);
DataBlock* ReadFile(const char* filename);
std::string itos(int i);
