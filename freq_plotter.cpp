#include "Biblioteca.h"
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

static int serial_port=-1;
template <typename T> std::string vectorToString(std::vector<T> vec) {
    std::stringstream ss;
    for (size_t i = 0; i < vec.size(); ++i) {
        ss << vec[i];
        if (i != vec.size() - 1) {
            ss << ",";
        }
    }
    ss << ",255,";
    return ss.str();
}


vector<vector<float>> getMax(vector<int> histograma){
    vector<float> normalizado=vector<float>(histograma.size());
    vector<float> maxValues=vector<float>();
    int total=0;
    for (size_t i = 0; i < normalizado.size(); i++){
        total+=histograma[i];
    }
    float maxvalue=0;
    for (size_t i = 0; i < normalizado.size(); i++){
        normalizado[i]=static_cast<float>(histograma[i])/total;
        if (normalizado[i]>normalizado[maxvalue])
        {
            maxvalue=i;
        }
        
    }
    float nextMaxValue=0;
    maxValues.push_back(maxvalue);
    for (size_t i = 0; i < 4; i++)
    {
        maxvalue=nextMaxValue;
        nextMaxValue=0;
        for (size_t i = 0; i < normalizado.size(); i++){
            

            if (normalizado[i]>normalizado[maxvalue])
            {
		    if(std::find(maxValues.begin(), maxValues.end(), i) == maxValues.end()){
	             maxvalue=i;}
            }else{
	     nextMaxValue=i; 
	    }
        }
        maxValues.push_back(maxvalue);
    }
    return {normalizado,maxValues};
}

int sendToArduino(std::string message) {
    // Change the port name to match your system
    const char* port_name = "/dev/ttyARDUINO0";

    // Open the serial port
    if (serial_port<0)
    {
        serial_port = open(port_name, O_RDWR);
    }
    

    if (serial_port < 0) {
        std::cerr << "Error opening serial port: " << strerror(errno) << std::endl;
        return 1;
    }

    vector<char> recieved(255);
    std::string message_1;

    std::cout << "Enter a character to send to the Arduino: ";
    std::cin >> message_1;

    // Write the message to the serial port
    int write_result = write(serial_port, message.c_str(), message.size());
    if (write_result < 0) {
        std::cerr << "Error writing to serial port: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Sent: " << message << std::endl;
    }
    
    int read_result = read(serial_port,recieved.data(),255);
    if (read_result < 0) {
        std::cerr << "Error Reading to serial port: " << strerror(errno) << std::endl;
    } else {
        std::cout << "readed: " << recieved.data() << std::endl;
    }    

    return 0;
}

int main(int argc, char** argv) {

    string vector_result_text = argv[1];

    vector<int> result_freq;
    stringstream ss(vector_result_text);
    string token;
    
    while (getline(ss, token, ',')) {
        result_freq.push_back(std::stoi(token));
    }

    for (int j = 0; j < result_freq.size(); ++j) {
            printf("valor %d: %d\n", j, result_freq[j]);        
    }

    auto valores = getMax(result_freq);
    vector<float> frecuenciasMaximos = vector<float>(5);
    
    for (size_t i =0;i<frecuenciasMaximos.size();i++) {
        frecuenciasMaximos[i] = valores[0][valores[1][(int)i]];
    }

    auto parsedString = vectorToString(frecuenciasMaximos);
    sendToArduino(parsedString);
    cout<<parsedString<<endl;
    return 0;
}