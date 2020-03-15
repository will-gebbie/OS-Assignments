#pragma once
#include <string>
using namespace std;

// Global Variable for number of characters. Use const over define for c++ style.
const int nchars = 27;

class dictentry
{
public:
    // isEndofWord is true if THIS node represents the the last character of a completed word.
    bool isEndofWord;

    // next is a pointer to the next characters (dictentries) of the
    // words that branch off of the THIS node (character).
    dictentry* next[nchars];

    // Default new entry constructor, initializes member variables.
    dictentry() {
        this->isEndofWord = false;

        for (int i = 0; i < nchars; i++) {
            this->next[i] = nullptr;
        }
    }

    string find(string word);
    string insert(string characters);
};
