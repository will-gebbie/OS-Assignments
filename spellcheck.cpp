#include "dictionary.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <cstring>

using namespace std;

dictentry *createDictTree(ifstream &file) {
    dictentry *root = new dictentry();
    string line;

    // Parse each line of file and assign it to the string line variable
    if (file) {
        while (getline(file, line)) {
            // Insert the word into the dictionary tree
            string result = root->insert(line);
        }
    }

    return root;
}

void compareWithDict(dictentry *dictTree, ifstream &file) {

    const char* separators = "\n\r !\"#$%&()*+,-./0123456789:;<=>?@[\\]^_`{|}~";
    string line;

    if (file) {
        while (getline(file, line)) {

            // Convert string into c string for strtok method.
            char* line_c = new char[line.length() + 1];
            strcpy(line_c, line.c_str());

            // Pull one word at a time
            char* token = (char*)strtok(line_c, separators);

            while (token != NULL) {

                // Convert the token back to a string
                string inputStr = string(token);
                // Find the word in the dictionary
                string result = dictTree->find(inputStr);
                
                // If the word is MISSPELLED, output it to std out
                if (result.compare("MISSPELLED") == 0) {
                    cout << inputStr << endl;
                }

                token = strtok(NULL, separators);
            }
        }
    }
}

int main(int argc, char** argv) {
    // Take the args (files) passed and open them for reading.
    ifstream checkSpell;
    ifstream dictSource;

    checkSpell.open(argv[1]);
    dictSource.open(argv[2]);

    // Error Handling for bad args passed
    try {
        // Too many args:
        if (argc != 3) {
            throw runtime_error("Too few or too many arguments. There must be 2 files.");
        }
        // Invalid files:
        else if (!checkSpell || !dictSource) {
            throw runtime_error("One of the files does not exist! Please check spelling or filepath.");
        }
        // Main Program
        else {

            dictentry *dictTree = createDictTree(dictSource);
            compareWithDict(dictTree, checkSpell);

            // Close the files that were opened
            checkSpell.close();
            dictSource.close();
        }
    }
    catch (exception & e) {
        cerr << "The program could not run correctly because of: " << e.what();
    }

    return 0;
}