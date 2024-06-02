#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <vector>

using namespace std;

int configured=0;

int GetArduinoPort(void)
{
    int fd = open("/dev/ttyUSB1", O_RDONLY | O_NOCTTY);
    if (fd == -1)
    {
        /* Could not open the port. */
        perror("open_port: Unable to open /dev/ttyACM0 - ");
    }

    char buffer[32];
    int n = read(fd, buffer, sizeof(buffer));
    if (n < 0)
        fputs("read failed!\n", stderr);
    return (fd);
}
int configurePort(int serial_port,int baud_rate = B9600){
    // Configure the serial port
    struct termios tty;
    if (tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Error getting terminal attributes: " << strerror(errno) << std::endl;
        close(serial_port);
        return 0;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE;  // Clear all bits that set the data size
    tty.c_cflag |= CS8;     // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received
    tty.c_cc[VMIN] = 0;

    cfsetispeed(&tty, baud_rate);
    cfsetospeed(&tty, baud_rate);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cerr << "Error setting terminal attributes: " << strerror(errno) << std::endl;
        close(serial_port);
        return 0;
    }
    return 1;
}


int main() {
    // Change the port name to match your system
    const char* port_name = "/dev/ttyARDUINO0";

    // Open the serial port
    int serial_port = open(port_name, O_RDWR);

    if (serial_port < 0) {
        std::cerr << "Error opening serial port: " << strerror(errno) << std::endl;
        return 1;
    }


    if (configured==0)
    {
        //configured= configurePort(serial_port);
    }
    



    std::string message;
    vector<char> recieved(255);
    while (true) {
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
    }

    close(serial_port);
    return 0;
}
