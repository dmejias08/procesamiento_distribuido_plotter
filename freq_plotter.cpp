#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "biblioteca.h"

using namespace std;

int main(int argc, char** argv) {

    string vector_result_text = argv[1];

    vector<int> result;
    stringstream ss(vector_result_text);
    string token;
    
    while (getline(ss, token, ',')) {
        result.push_back(std::stoi(token));
    }

    for (int j = 0; j < result.size(); ++j) {
            printf("valor %d: %d\n", j, result[j]);        
    }

    Plot(result);
}