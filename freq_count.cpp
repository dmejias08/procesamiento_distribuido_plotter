#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <mpi.h>

using namespace std;

/*
mpic++ -o freq_count freq_count.cpp
mpirun -np 4 ./freq_count Quijote.txt
*/

// Function to map accented characters to their English alphabet counterparts
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

// Function to count letter frequency in a text fraction, treating uppercase and lowercase as equal
map<char, int> count_letter_frequency(const string& text_fraction) {
    map<char, int> frequency_map;

    // Count frequency of each letter, treating uppercase and lowercase as equal
    for (char c : text_fraction) {
        if (isalpha(c)) {
            c = tolower(remove_accent(c)); // Convert to lowercase and remove accent
            frequency_map[c]++;
        }
    }

    return frequency_map;
}

int main(int argc, char** argv) {
    // Initialize MPI
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

    string input_text_file = argv[1];

    ifstream input_file(input_text_file);
    if (!input_file) {
        cerr << "Error: Unable to open input file." << endl;
        MPI_Finalize();
        return -1;
    }

    // Read the entire text into a string
    string text((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());

    int text_length = text.length();
    int fraction_size = text_length / size;
    int extra_characters = text_length % size;
    printf("fraction_size: %d, extra_characters: %d \n",fraction_size, extra_characters);

    // Calculate the size of text fraction for each node
    int start = rank * fraction_size;
    int fraction_length = fraction_size + (rank == size - 1 ? extra_characters : 0);

    // Extract the text fraction for this node
    string text_fraction = text.substr(start, fraction_length);
    printf(text_fraction);

    // Count letter frequency in the text fraction
    map<char, int> local_frequency = count_letter_frequency(text_fraction);

    // Send local frequency data to the root node
    if (rank != 0) {
        vector<int> frequency_data(256); // Initialize a vector to hold frequency data
        for (const auto& pair : local_frequency) {
            frequency_data[pair.first] = pair.second;
        }
        MPI_Send(frequency_data.data(), 256, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        // Root node gathers frequency data from all nodes
        vector<int> global_frequency_data(256, 0); // Initialize vector to hold global frequency data
        for (int i = 0; i < size; ++i) {
            if (i == 0) {
                for (const auto& pair : local_frequency) {
                    global_frequency_data[pair.first] = pair.second;
                }
            } else {
                vector<int> received_frequency_data(256); // Vector to receive frequency data
                MPI_Recv(received_frequency_data.data(), 256, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // Combine received frequency data with global frequency data
                for (int j = 0; j < 256; ++j) {
                    global_frequency_data[j] += received_frequency_data[j];
                }
                
            }
        }

        // Print the global frequency of each letter, treating uppercase and lowercase as equal
        for (char c = 'a'; c <= 'z'; ++c) {
            cout << "'" << c << "': " << global_frequency_data[c] + global_frequency_data[toupper(c)] << endl;
        }


    }

    // Finalize MPI
    MPI_Finalize();
    return 0;
}