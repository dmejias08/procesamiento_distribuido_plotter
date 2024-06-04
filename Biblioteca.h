#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>

using namespace std;
template <typename T> std::string vectorToString(std::vector<T> vec);

vector<vector<float>> getMax(vector<int> histograma);

int drawManual();

int sendToArduino(std::string message);

void closePort();

void Plot(vector<int> in);

#endif
