#pragma once

#include <fstream>
#include <iostream>
#include "DataBlock.h"

DataBlock* ReadFile(const std::string& filename);
DataBlock* ReadFile(const char* filename);