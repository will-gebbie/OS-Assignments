#include "dictionary.h"
using namespace std;

string dictentry::find(string word)
{
    dictentry* currentNode = this;
    int dictIndex;

    // Check for badchars and dont insert if there are any
    for (int i = 0; i < word.length(); i++) {
        if ((word[i] < 65 && word[i] != 39) || word[i] > 90 && word[i] < 97 || word[i] > 122) {
            return "BADCHARS";
        }
    }

    for (int i = 0; i < word.length(); i++) {
        // Apostrophe = index 0 of next pointers
        if (word[i] == 39) {
            dictIndex = 0;
        }
        // Convert Capital to lowercase then to range 1 - 26
        else if (word[i] > 64 && word[i] < 91) {
            dictIndex = word[i] + 32 - 96;
        }
        else {
            dictIndex = word[i] - 96;
        }

        // Go through the dictionary to see if it is a word or not
        currentNode = currentNode->next[dictIndex];

        if (currentNode == nullptr)
            return "MISSPELLED";
    }

    // If the last node marks the end of a word return "Correct"
    if (currentNode->isEndofWord == true) {
        return "CORRECT";
    }
    else {
        return "MISSPELLED";
    }
}

string dictentry::insert(string characters)
{
    dictentry* currentNode = this;
    bool exists = true;
    int dictIndex;

    // Check for badchars and dont insert if there are any
    for (int i = 0; i < characters.length(); i++) {
        if ((characters[i] < 65 && characters[i] != 39) || characters[i] > 90 && characters[i] < 97 || characters[i] > 122) {
            return "BADCHARS";
        }
    }

    for (int i = 0; i < characters.length(); i++) {
        // Apostrophe = index 0 of next pointers
        if (characters[i] == 39) {
            dictIndex = 0;
        }
        // Convert Capital to lowercase then to range 0 - 26
        else if (characters[i] > 64 && characters[i] < 91) {
            dictIndex = characters[i] + 32 - 96;
        }
        else {
            dictIndex = characters[i] - 96;
        }

        if (currentNode->next[dictIndex] == nullptr) {
            currentNode->next[dictIndex] = new dictentry();
            exists = false;
        }

        currentNode = currentNode->next[dictIndex];
    }

    currentNode->isEndofWord = true;

    if (exists) {
        return "EXISTS";
    }
    else {
        return "ADDED";
    }
}