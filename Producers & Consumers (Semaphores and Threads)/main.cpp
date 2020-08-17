#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "consumer.h"
#include "producer.h"

#define NOTSET 0
#define BADARGS 5
#define NEGINPUT 7
#define NUMOUTPUT 100
#define RESTRICTION 3

using namespace std;

struct prod_args
{
    producer *prod;
    sem_t *access;
    sem_t *underflow;
    sem_t *overflow;
    sem_t *restriction;
};

struct cons_args
{
    consumer *cons;
    int *total;
    sem_t *access;
    sem_t *underflow;
    sem_t *overflow;
    sem_t *restriction;
};

struct report_args
{
    producer *prod1;
    producer *prod2;
    consumer *cons1;
    consumer *cons2;
};

void *producerLoop(void *vargs)
{
    prod_args *args = (prod_args *)vargs;

    while (args->prod->getBelt()->getTotalAdded() < 100)
    {
        // Start semaphore for only 3 escargots at a time
        if (args->prod->getName() == string(RESTRICTIONNAME))
            sem_wait(args->restriction);
        sem_wait(args->overflow);
        sem_wait(args->access);
        // Make a candy!
        args->prod->make();
        // Output for producer
        cout << "Belt: " << args->prod->getBelt()->getFrogs() << " frogs + "
             << args->prod->getBelt()->getEscargots() << " escargots = "
             << args->prod->getBelt()->getFrogs() + args->prod->getBelt()->getEscargots()
             << ". produced: " << args->prod->getBelt()->getTotalAdded() << "\t"
             << "Added " << args->prod->getName()
             << "." << endl;
        sem_post(args->access);
        sem_post(args->underflow);
        usleep(args->prod->getTime() * 1000);
    }
}

void *consumerLoop(void *vargs)
{
    string itemConsumed;
    cons_args *args = (cons_args *)vargs;

    while (args->cons->getBelt()->getTotalRemoved() < 100)
    {
        sem_wait(args->underflow);
        sem_wait(args->access);
        // Grabs candy off of the queue
        itemConsumed = args->cons->grab();
        // Let the escargot producer know that it can produce
        if (itemConsumed == string(RESTRICTIONNAME))
            sem_post(args->restriction);
        // Output for consumer
        cout << "Belt: " << args->cons->getBelt()->getFrogs() << " frogs + "
             << args->cons->getBelt()->getEscargots() << " escargots = "
             << args->cons->getBelt()->getFrogs() + args->cons->getBelt()->getEscargots()
             << ". produced: " << args->cons->getBelt()->getTotalAdded() << "\t"
             << args->cons->getName() << " consumed " << itemConsumed << "." << endl;

        sem_post(args->access);
        sem_post(args->overflow);
        usleep(args->cons->getTime() * 1000);
    }
}

report_args *createThreads(int prod1Time, int prod2Time, int cons1Time, int cons2Time)
{
    // Create Objects
    belt *mizzo = new belt(10);
    producer *crunchyFrog = new producer("crunchy frog bite", prod1Time, mizzo);
    producer *escargotSucker = new producer("escargot sucker", prod2Time, mizzo);
    consumer *lucy = new consumer("Lucy", cons1Time, mizzo);
    consumer *ethel = new consumer("Ethel", cons2Time, mizzo);
    // Initialize Semaphores, overflow controls number of item on belt,
    // underflow makes it so that consumers cannot eat if there is no
    // items on the belt, and restriction controls whether escargots
    // can be produced or not.
    sem_t accessSem, underflowSem, overflowSem, restrictionSem;
    sem_init(&accessSem, 0, 1);
    sem_init(&underflowSem, 0, 0);
    sem_init(&overflowSem, 0, mizzo->getCapacity());
    sem_init(&restrictionSem, 0, RESTRICTION);

    // Initialize Structs for pthread_create() function args
    prod_args *frogBites = (prod_args *)malloc(sizeof(prod_args));
    frogBites->prod = crunchyFrog;
    frogBites->access = &accessSem;
    frogBites->underflow = &underflowSem;
    frogBites->overflow = &overflowSem;

    prod_args *escargotSuckers = (prod_args *)malloc(sizeof(prod_args));
    escargotSuckers->prod = escargotSucker;
    escargotSuckers->access = &accessSem;
    escargotSuckers->underflow = &underflowSem;
    escargotSuckers->overflow = &overflowSem;
    escargotSuckers->restriction = &restrictionSem;

    cons_args *lucyArgs = (cons_args *)malloc(sizeof(cons_args));
    lucyArgs->cons = lucy;
    lucyArgs->access = &accessSem;
    lucyArgs->underflow = &underflowSem;
    lucyArgs->overflow = &overflowSem;
    lucyArgs->restriction = &restrictionSem;

    cons_args *ethelArgs = (cons_args *)malloc(sizeof(cons_args));
    ethelArgs->cons = ethel;
    ethelArgs->access = &accessSem;
    ethelArgs->underflow = &underflowSem;
    ethelArgs->overflow = &overflowSem;
    ethelArgs->restriction = &restrictionSem;

    // Start Threads
    pthread_t prod1,
        prod2, cons1, cons2;

    pthread_create(&prod1, NULL, producerLoop, frogBites);
    pthread_create(&prod2, NULL, producerLoop, escargotSuckers);
    pthread_create(&cons1, NULL, consumerLoop, lucyArgs);
    pthread_create(&cons2, NULL, consumerLoop, ethelArgs);

    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(cons1, NULL);
    pthread_join(cons2, NULL);

    // Pass in resulting classes into stats struct to display end statistics
    report_args *stats = (report_args *)malloc(sizeof(report_args));
    stats->prod1 = crunchyFrog;
    stats->prod2 = escargotSucker;
    stats->cons1 = lucy;
    stats->cons2 = ethel;

    return stats;
}

void produceReport(report_args *args)
{

    int totalFrogBites = args->prod1->getTotalProduced();
    int totalEscargotSuckers = args->prod2->getTotalProduced();
    int lucyFrogBites = args->cons1->getFrogs();
    int lucyEscargotSuckers = args->cons1->getEscargots();
    int ethelFrogBites = args->cons2->getFrogs();
    int ethelEscargotSuckers = args->cons2->getEscargots();

    cout << endl
         << endl;

    cout << "PRODUCTION REPORT" << endl
         << "----------------------------------------" << endl
         << "crunchy frog bite producer generated " << totalFrogBites
         << " candies" << endl
         << "escargot sucker producer generated "
         << totalEscargotSuckers << " candies" << endl
         << "Lucy consumed " << lucyFrogBites << " crunchy frog bites + "
         << lucyEscargotSuckers << " escargot suckers = "
         << lucyFrogBites + lucyEscargotSuckers << endl
         << "Ethel consumed " << ethelFrogBites << " crunchy frog bites + "
         << ethelEscargotSuckers << " escargot suckers = "
         << ethelFrogBites + ethelEscargotSuckers << endl;
}

int main(int argc, char **argv)
{
    // Declare variables (iniitalized by argument handler)
    int timeFB = NOTSET;
    int timeES = NOTSET;
    int inBoxLucy = NOTSET;
    int inBoxEthel = NOTSET;

    // Handle Arguments, Initialize variables
    int Option;
    while ((Option = getopt(argc, argv, "E:L:f:e:")) != -1)
    {
        switch (Option)
        {
        case 'E': /* Number of addresses to process */
                  // optarg will contain the string following -n
            inBoxEthel = atoi(optarg);
            if (inBoxEthel < 0)
            {
                cout << "Input must be positive!" << endl;
                exit(NEGINPUT);
            }
            break;
        case 'L': /* produce map of pages */
                  // optarg contains name of page file�
            inBoxLucy = atoi(optarg);
            if (inBoxLucy < 0)
            {
                cout << "Input must be positive!" << endl;
                exit(NEGINPUT);
            }
            break;
        case 'f': /* Show address translation */
                  // No argument this time, just set a flag
            timeFB = atoi(optarg);
            if (timeFB < 0)
            {
                cout << "Input must be positive!" << endl;
                exit(NEGINPUT);
            }
            break;
        case 'e':

            timeES = atoi(optarg);
            break;
            if (timeES < 0)
            {
                cout << "Input must be positive!" << endl;
                exit(NEGINPUT);
            }
        default:
            printf("You messed up in your arguments somewhere!\n");
            exit(BADARGS); // BADARGS is an error # defined in a header
        }
    }

    // Create threads, start production/consumption process
    report_args *args = createThreads(timeFB, timeES, inBoxLucy, inBoxEthel);

    // Prints out statistics after the process has been completes
    produceReport(args);

    exit(0);
}