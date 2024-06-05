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

int drawManual(int *serial_port);

int sendToArduino(std::string message, int *serial_port);

void closePort(int *serial_port);

vector<vector<float>> Plot(vector<int> in, int *serial_port);

#endif
