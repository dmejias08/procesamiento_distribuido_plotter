#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
using namespace std;

string encrypted_file_path ="EncryptedData.txt";

int main(void)
{
  char filepath[256];
  cout << "Enter file name: ";
  scanf("%256s", filepath);
  string input_text_file = filepath;
  ifstream input_file(input_text_file);
  
  if(!input_file){
    cerr << "Error: Unable to open input file." << endl;
    return -1;
  }

  cout<<"File opened\n";
  string text((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
  int text_lenght = text.length();
  
  std::ofstream encrypted_file;
  encrypted_file.open(encrypted_file_path, std::ios_base::out);
  encrypted_file.close();
  encrypted_file.open(encrypted_file_path, std::ios_base::app);
  cout << "Encrypting..." << endl;
  for (int i = 0;i<text_lenght;i++){
    encrypted_file << char(toascii(text[i])+122);
  }
  cout << "Encrypted" << endl;

  //system("make");
  return 0;
}