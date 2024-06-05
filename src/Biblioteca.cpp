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

template <typename T> std::string vectorToString(std::vector<T> vec) {
    std::stringstream ss;
    ss<<"0,";
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
    float maxvalue=0;
    for (size_t i = 0; i < normalizado.size(); i++){
        normalizado[i]=static_cast<float>(histograma[i]);
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
    int total=0;
    for (size_t i = 0; i < maxValues.size(); i++){
        total+=histograma[maxValues[i]];
    }
    for (size_t i = 0; i < normalizado.size(); i++){
        normalizado[i]=normalizado[i]/total;
    }
    return {normalizado,maxValues};
}


//static int serial_port=-1;


int drawManual(int *serial_port) {
    // Change the port name to match your system
    const char* port_name = "/dev/ttyARDUINO0";

    // Open the serial port
    if (*serial_port<0)
    {
        *serial_port = open(port_name, O_RDWR);
    }
    

    if (*serial_port < 0) {
        std::cerr << "Error opening serial port: " << strerror(errno) << std::endl;
        return 1;
    }

    std::string message;
    vector<char> recieved(255);

    std::cout << "Enter a character to send to the Arduino: ";
    std::cin >> message;

    // Write the message to the serial port
    int write_result = write(*serial_port, message.c_str(), message.size());
    if (write_result < 0) {
        std::cerr << "Error writing to serial port: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Sent: " << message << std::endl;
    }
    
    int read_result = read(*serial_port,recieved.data(),255);
    if (read_result < 0) {
        std::cerr << "Error Reading to serial port: " << strerror(errno) << std::endl;
    } else {
        std::cout << "readed: " << recieved.data() << std::endl;
    }
    return 0;
}


int sendToArduino(std::string message, int *serial_port) {
    // Change the port name to match your system
    const char* port_name = "/dev/ttyARDUINO0";

    // Open the serial port
    if (*serial_port<0)
    {
        *serial_port = open(port_name, O_RDWR);
    }
    

    if (*serial_port < 0) {
        std::cerr << "Error opening serial port: " << strerror(errno) << std::endl;
        return 1;
    }

    vector<char> recieved(255);

    std::string message_1;
    std::cout << "Press any key to continue: ";
    std::cin >> message_1;

    // Write the message to the serial port
    int write_result = write(*serial_port, message.c_str(), message.size());
    if (write_result < 0) {
        std::cerr << "Error writing to serial port: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Sent: " << message << std::endl;
    }
    
    int read_result = read(*serial_port,recieved.data(),255);
    if (read_result < 0) {
        std::cerr << "Error Reading to serial port: " << strerror(errno) << std::endl;
    } else {
        std::cout << "readed: " << recieved.data() << std::endl;
    }    

    return 0;
}
void closePort(int *serial_port){
    close(*serial_port);
}


vector<vector<float>> Plot(vector<int> in, int *serial_port){
    //obtener lista de posicion de maximos y la frecuencia normalizada
    //Valores[0] =>  histograma normalizado
    //Valores[1] =>  lista de posiciones
    auto valores=getMax(in);
    //Obtener los 5 a plotear
    vector<float> frecuenciasMaximos=vector<float>(5);
    for (size_t i = 0; i < frecuenciasMaximos.size(); i++){
        frecuenciasMaximos[i]=valores[0][valores[1][(int)i]];
    }
    //Crear el string
    auto parsedString =vectorToString(frecuenciasMaximos);
    //Mandar al ARDUINO
    sendToArduino(parsedString, serial_port);
    cout<<parsedString<<endl;
    return {valores[1],frecuenciasMaximos};
}
/*
int main(){
//Setear valores
vector<int> lista=vector<int>(7);
lista[0]=0;
lista[1]=1;
lista[2]=2;
lista[3]=3;
lista[4]=4;
lista[5]=3;
lista[6]=7;
//obtener lista de posicion de maximos y la frecuencia normalizada
//Valores[0] => lista de posiciones
//Valores[1] => histograma normalizado
auto valores=getMax(lista);
//Obtener los 5 a plotear
vector<float> frecuenciasMaximos=vector<float>(5);
for (size_t i = 0; i < frecuenciasMaximos.size(); i++){
	frecuenciasMaximos[i]=valores[0][valores[1][(int)i]];
}
cout<<vectorToString(valores[1])<<endl;
cout<<vectorToString(valores[0])<<endl;
//Crear el string
auto parsedString =vectorToString(frecuenciasMaximos);
//Mandar al ARDUINO
sendToArduino(parsedString);//No tengo el arduino correcto
cout<<parsedString<<endl;
}*/
