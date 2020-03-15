#include "byutr.h"
#include "level.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <climits>
#include <cstdlib>
#include <cmath>

#define addrsize 32
#define invalid -1
#define updated 1
#define notupdated 0
#define BADFILE -5
#define BADARGS -6

unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift) {
    unsigned int indexOfPage;
    indexOfPage = (LogicalAddress & Mask) >> Shift;
    return indexOfPage;
}

int PageInsert(pagetable* pt, unsigned int LogicalAddress, unsigned int Frame) {
    int i;
    int currIndex;
    int j;

    level* currLevel = pt->RootNodePtr;
    level** levelArrPtr = nullptr;
    map* leaves = nullptr;
    for (i = 0; i < pt->LevelCount; i++) {
        currIndex = LogicalToPage(LogicalAddress, pt->BitmaskAry[i], pt->ShiftAry[i]);
        // If inferior level, go to next level given address
        if (currLevel->CurrentDepth < pt->LevelCount - 1) {
            // Creates new pointer array if it does not already exist
            levelArrPtr = currLevel->NextLevelPtr;
            if (levelArrPtr == nullptr) {
                levelArrPtr = new level * [pt->EntryCount[i]];
                for (j = 0; j < pt->EntryCount[i]; j++) {
                    levelArrPtr[j] = nullptr;
                }
                currLevel->NextLevelPtr = levelArrPtr;
            }
            // If level index exists, then we just continue
            if (currLevel->NextLevelPtr[currIndex] != nullptr) {
                currLevel = currLevel->NextLevelPtr[currIndex];
            }
            // If not, then we populate the new level
            else {
                currLevel->NextLevelPtr[currIndex] = new level(*pt, i + 1);
                currLevel = currLevel->NextLevelPtr[currIndex];
            }
        }
        // If leaf level, put frame number into map
        else {
            leaves = currLevel->MapPtr;
            if (leaves != nullptr) {
                if (leaves->FrameNumbers[currIndex] == invalid) {
                    leaves->FrameNumbers[currIndex] = Frame;
                    return updated;
                }
                else {
                    return notupdated;
                }
            }
            else {
                leaves = new map(pt->EntryCount[i]);
                currLevel->MapPtr = leaves;
                leaves->FrameNumbers[currIndex] = Frame;
                return updated;
            }
        }
    }
}

int PageLookup(pagetable* pt, unsigned int LogicalAddress) {
    int i;
    int currIndex;
    level* currLevel = pt->RootNodePtr;
    map* leaves = nullptr;
    for (i = 0; i < pt->LevelCount; i++) {
        currIndex = LogicalToPage(LogicalAddress, pt->BitmaskAry[i], pt->ShiftAry[i]);
        if (currLevel->CurrentDepth < pt->LevelCount - 1) {
            if (currLevel->NextLevelPtr == nullptr) {
                return invalid;
            }
            else {
                if (currLevel->NextLevelPtr[currIndex] == nullptr) {
                    return invalid;
                }
                else {
                    currLevel = currLevel->NextLevelPtr[currIndex];
                }
            }
        }
        else {
            leaves = currLevel->MapPtr;
            if (leaves == nullptr) {
                return invalid;
            }
            else {
                if (leaves->FrameNumbers[currIndex] == invalid)
                    return invalid;
                else {
                    return leaves->FrameNumbers[currIndex];
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    // Declarations

    int numAddresses = INT_MAX;
    bool addressFlag = false;
    FILE* frameIndices = nullptr;
    bool filePresent = false;
    FILE* trace = nullptr;
    int LevelCount;

    int Option;
    while ((Option = getopt(argc, argv, "n:p:t")) != -1) {
        switch (Option) {
        case 'n': /* Number of addresses to process */
        // optarg will contain the string following -n
            numAddresses = atoi(optarg);
            break;
        case 'p': /* produce map of pages */
        // optarg contains name of page file…
            frameIndices = fopen(optarg, "w");
            filePresent = true;
            break;
        case 't': /* Show address translation */
        // No argument this time, just set a flag
            addressFlag = true;
            break;
        default:
            printf("You messed up in your arguments somewhere!\n");
            exit(BADARGS); // BADARGS is an error # defined in a header
        }
    }
    // Assign trace file to the first arg that is not an option
    trace = fopen(argv[optind], "r");
    if (!trace) {
        printf("Trace file is not valid!\n");
        exit(BADFILE);
    }
    optind++;
    // Loop through the rest of the args for the number of bits for each level
    int i = optind;
    int op_offset = optind;
    for (; optind < argc; optind++) {
        LevelCount++;
    }
    // Create an array containing the number of bits for each level
    unsigned int* Bits = new unsigned int[LevelCount];
    for (i; i < argc; i++) {
        Bits[i - op_offset] = (unsigned int)atoi(argv[i]);
    }
    // Create a pagetable and set the num bits for each level
    pagetable* pt = new pagetable(LevelCount);
    pt->Bits = Bits;

    // Populate BitMaskAry, ShiftAry, and EntryArt
    unsigned int* BitMaskAry = new unsigned int[LevelCount];
    int* ShiftAry = new int[LevelCount];
    int* EntryCount = new int[LevelCount];
    int sumexp = 0;
    unsigned int currMask;
    for (i = 0; i < LevelCount; i++) {
        sumexp += pt->Bits[i];
        ShiftAry[i] = addrsize - sumexp;
        EntryCount[i] = (int)pow(2, pt->Bits[i]);
        currMask = (unsigned int)(pow(2, pt->Bits[i]) - 1);
        currMask = currMask << (ShiftAry[i]);
        BitMaskAry[i] = currMask;
    }
    // Set pagetable member variable ptrs
    pt->BitmaskAry = BitMaskAry;
    pt->ShiftAry = ShiftAry;
    pt->EntryCount = EntryCount;

    // Initialize Root Level
    level* RootLevel = new level(*pt, 0);
    pt->RootNodePtr = RootLevel;

    // Start of Address processing
    p2AddrTr* trace_item = new p2AddrTr;
    int addrCounter = 0;
    bool done = false;

    int totalbits;
    unsigned int offsetMask;
    //Make Offset Mask
    for (i = 0; i < LevelCount; i++) {
        totalbits += Bits[i];
    }
    offsetMask = (unsigned int)pow(2, addrsize - totalbits) - 1;
    int framecounter = 0;
    while (addrCounter < numAddresses && !done) {
        // Grab next address
        int bytesread = NextAddress(trace, trace_item);
        done = bytesread == 0;
        if (!done) {
            int framenum;
            int frameupdate;
            int offset;
            unsigned int physAddr;
            // Insert all the addresses and corresponding frame numbers
            frameupdate = PageInsert(pt, trace_item->addr, framecounter);
            // If there is -t, translate and print to stdout
            if (addressFlag) {
                framenum = PageLookup(pt, trace_item->addr);
                // Calculate offset and concatenate with the framenumber
                offset = trace_item->addr & offsetMask;
                physAddr = (framenum << addrsize - totalbits) + offset;
                // Print translation
                printf("%08x -> %08x\n", trace_item->addr, physAddr);
            }
            // If the a frame got inserted, update the frame number
            if (frameupdate) {
                framecounter++;
            }
            addrCounter++;
        }
    }
    // End of Address Processing 
    // If there is -p, tree traversal and print to order to the file input
    if (filePresent) {
        unsigned int framenum;
        unsigned int la;
        //From page 0 - how many bits are used for page numbers
        for (la = 0; la < (unsigned int)pow(2, totalbits); la++) {
            // Have to shift la to account for offset
            framenum = PageLookup(pt, (la << addrsize - totalbits));
            // If the framenum is invalid, do not print to file
            if (framenum != invalid) {
                fprintf(frameIndices, "%08x -> %08x\n", la, framenum);
            }
        }
    }
    // Close Files
    fclose(trace);
    if (frameIndices != nullptr) {
        fclose(frameIndices);
    }
}