#include "imprimer.h"

// typedef struct jobnode {
//     struct jobnode *next;
//     JOB* job;
// } JOBNODE;


typedef struct conversionProgram{
    char* name;
    char* fileType1;
    char* fileType2;
    int numArguments;
    char** arguments;
    struct conversionProgram* parent;
    int visited;

} CONVERSIONPROGRAM;

typedef struct jobstruct{
    JOB* job;
    int numConversions;
    CONVERSIONPROGRAM*** pathway;
    struct jobstruct *next;
    struct jobstruct *prev;
}JOBSTRUCT;



void addConversion(char* file1 ,char*file2,char* name, CONVERSIONPROGRAM  matrixOfConversions[][64], char** arguments, int numArguments, int pos1, int pos2);

void addPrinter(char* name, char* type, PRINTER* arrayOfPrinters, int numPrinters);

JOBSTRUCT* addNewJob(char* fileName, int numJobs, struct jobstruct** head, char* arrayOfArguments[], int numArgs, int numPrinters, PRINTER arrayOfPrinters[]);

void printConversionPrograms(CONVERSIONPROGRAM** program, int numConversionPrograms);

int checkType(char* type, char** arrayOfTypes, int numTypes);

int checkPrinter(char* name, PRINTER* arrayOfPrinters, int numPrinters);

void printEligiblePrinters(char* eligiblePrinters[], int numEligiblePrinters );

PRINTER_SET getSetOfEligiblePrinters(int numArgs, char* args[], int numPrinters, PRINTER arrayOfPrinters[]);

void getAvailablePrinter(PRINTER arrayOfPrinters[],int numPrinters, JOBSTRUCT** job, CONVERSIONPROGRAM matrixOfConversions[][64],char** arrayOfTypes, int numTypes, CONVERSIONPROGRAM*** pathway);

size_t getSizeOfJob(JOB job);

int getIndex(char** arrayOfTypes, char* type, int numTypes);

void printMatrix(CONVERSIONPROGRAM matrixOfConversions[][64], int numTypes);

int BFS (CONVERSIONPROGRAM matrixOfConversions[][64], char* input, char* target, char** arrayOfTypes, int numTypes, CONVERSIONPROGRAM*** pathway);

struct QNode{
    CONVERSIONPROGRAM* key;
    struct QNode *next;
};

struct Queue{
    struct QNode *front, *rear;
};

CONVERSIONPROGRAM* deQueue(struct Queue *q);
void enQueue(struct Queue *q, CONVERSIONPROGRAM* key);
struct Queue *createQueue();
struct QNode* newNode(CONVERSIONPROGRAM*key);
int isEmptyQueue(struct Queue *queue);


void addNode(JOBSTRUCT** head, int numJobs, JOBSTRUCT** newNode);

JOB* getJobAtIndex(JOBSTRUCT* head, int index);

JOBSTRUCT* getStructAtIndex(JOBSTRUCT* head, int index);

int createNewProcess();