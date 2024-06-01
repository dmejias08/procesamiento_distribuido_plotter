#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <mpi.h>

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
        if (isalpha(c)) {
            c = tolower(remove_accent(c));
            frequency_map[c]++;
        }
    }

    return frequency_map;
}

int main(int argc, char** argv) {
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

    if (rank == 0) {
        string text((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
        int text_length = text.length();
        int fraction_size = text_length / size;
        int extra_characters = text_length % size;

        // Broadcast fraction size to all processes
        MPI_Bcast(&fraction_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Send text fractions to other processes
        for (int i = 1; i < size; ++i) {
            int start = i * fraction_size;
            int fraction_length = fraction_size + (i == size - 1 ? extra_characters : 0);
            string text_fraction = text.substr(start, fraction_length);
            MPI_Send(text_fraction.c_str(), fraction_length, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    } else {
        int fraction_length;
        // Receive fraction size from root process
        MPI_Bcast(&fraction_length, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Receive text fraction from root process
        string text_fraction(fraction_length, '\0');
        MPI_Recv(&text_fraction[0], fraction_length, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Count letter frequency in the text fraction
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
        vector<int> global_frequency_data(256, 0); // Initialize vector to hold global frequency data
        for (int i = 1; i < size; ++i) {
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
        }
    }

    MPI_Finalize();
    return 0;
}