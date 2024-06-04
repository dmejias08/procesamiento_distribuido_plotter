#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <mpi.h>
#include <string.h>

using namespace std;

char remove_accent(char c) {
    switch (c) {
        case 'á': case 'à': case 'â': case 'ä': return 'a';
        case 'é': case 'è': case 'ê': case 'ë': return 'e';
        case 'í': case 'ì': case 'î': case 'ï': return 'i';
        case 'ó': case 'ò': case 'ô': case 'ö': return 'o';
        case 'ú': case 'ù': case 'û': case 'ü': return 'u';
        case 'ñ': return 'n';
        default: return c;
    }
}

map<char, int> count_letter_frequency(const string& text_fraction) {
    map<char, int> frequency_map;

    for (char c : text_fraction) {
        c = char(c-122);
        if (isalpha(c)) {
            c = tolower(remove_accent(c));
            //printf("El caracter es: %c\n", c);
            frequency_map[c]++;
        }
    }

    return frequency_map;
}

int main(int argc, char** argv) {
    string result;
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            cerr << "Usage: " << argv[0] << " <input_text_file>" << endl;
        }
        MPI_Finalize();
        return -1;
    }

    

    if (rank == 0) {
        string input_text_file = argv[1];

        ifstream input_file(input_text_file);

        if (!input_file) {
            cerr << "Error: Unable to open input file." << endl;
            MPI_Finalize();
            return -1;
        }

        printf("Estoy en master, este es mi size %d\n", size);
        string text((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
        int text_length = text.length();
        printf("Text size %d\n", text_length);
        int fraction_size = text_length / (size-1);
        int extra_characters = text_length - fraction_size * (size-1);

        /// Send fraction size to all processes
        for (int i = 1; i < size; i++) {
            int start = (i-1) * fraction_size;
            int fraction_length = fraction_size + (i == size - 1 ? extra_characters : 0);
            string text_fraction = text.substr(start, fraction_length);

            printf("fraction_length desde master %d\n", fraction_length);
            MPI_Send(&fraction_length, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(text_fraction.c_str(), fraction_length, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    } 
    else {
        int fraction_length;
        // Receive fraction size from root process
        MPI_Recv(&fraction_length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("fraction_length desde nodo %d: %d\n", rank, fraction_length);

        // Dynamically allocate memory for text_fraction
        char* text_fraction_buffer = new char[fraction_length + 1]; // +1 for null terminator
        MPI_Recv(text_fraction_buffer, fraction_length, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        text_fraction_buffer[fraction_length] = '\0'; // Add null terminator

        // Assign received data to string
        string text_fraction(text_fraction_buffer);
        delete[] text_fraction_buffer; // Deallocate buffer

        // Count letter frequency in the text fraction
        printf("chars to count\n");
        map<char, int> local_frequency = count_letter_frequency(text_fraction);

        // Send local frequency data to the root node after encryption
        vector<int> frequency_data(256); // Initialize a vector to hold frequency data
        for (const auto& pair : local_frequency) {
            frequency_data[pair.first] = pair.second;
        }
        MPI_Send(frequency_data.data(), 256, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    

    // Root node gathers frequency data from all nodes
    if (rank == 0) {
       printf("Hola desde master estoy recibiendo los datos\n");
        // Root node gathers frequency data from all nodes
        vector<int> global_frequency_data(256, 0); // Initialize vector to hold global frequency data
        for (int i = 1; i < size; i++) {
            vector<int> received_frequency_data(256); // Vector to receive frequency data
            MPI_Recv(received_frequency_data.data(), 256, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // Combine received frequency data with global frequency data
            for (int j = 0; j < 256; ++j) {
                global_frequency_data[j] += received_frequency_data[j];
            }
        }

        
        // Print the global frequency of each letter, treating uppercase and lowercase as equal
        for (char c = 'a'; c <= 'z'; ++c) {
            cout << "'" << c << "': " << global_frequency_data[c] + global_frequency_data[toupper(c)] << endl;
            result.append(to_string(global_frequency_data[c] + global_frequency_data[toupper(c)]));
            result.append(",");
            //result.push_back(global_frequency_data[c] + global_frequency_data[toupper(c)]);
        }
	string command = "/home/nodo/operativos/plotter " + result;
    	cout<<command<<endl;
    	system(command.c_str());
    }

    MPI_Finalize();
    return 0;
}
