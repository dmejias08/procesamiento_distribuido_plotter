#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <vector>

using namespace std;

int serial_port=-1;

vector<vector<float>> getMax(vector<int> histograma){
    vector<float> normalizado=vector<float>(histograma.size());
    vector<float> maxValues=vector<float>();
    int total=0;

    for (size_t i = 0; i < normalizado.size(); i++){
        total+=histograma[i];
    }

    float maxvalue=0;
    for (size_t i = 0; i < normalizado.size(); i++){
        normalizado[i]=histograma[i]/total;
        if (normalizado[i]>normalizado[maxvalue])
        {
            maxvalue=i;
        }
        
    }

    maxValues.push_back(maxvalue);

    for (size_t i = 0; i < 4; i++)
    {
        maxvalue=0;   
        for (size_t i = 0; i < normalizado.size(); i++){
            if (normalizado[maxValues.size()-1]>normalizado[i] && normalizado[maxvalue]<normalizado[i])
            {
             maxvalue=i;   
            }
        }
        maxValues.push_back(maxvalue);
    }
    return {normalizado,maxValues};
}

int draw(vector<int> datos) {
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

    std::string message;
    vector<char> recieved(255);

    std::cout << "Enter a character to send to the Arduino: ";
    std::cin >> message;

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
    

    close(serial_port);
    return 0;
}
int main(){
    draw(vector<int>(256));
}