#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <getopt.h>

#include "debug.h"
#include "string.h"
#include "printFormat.h"
#include "helper.h"
#include <readline/readline.h>
#include <readline/history.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


/*
 * "Imprimer" printer spooler.
 */
#define MAX_LENGTH 80


int main(int argc, char *argv[])
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //My Code



    int loop = 1;


    CONVERSIONPROGRAM matrixOfConversions[64][64]= {0};
    //CONVERSIONPROGRAM* arrayOfConversions =malloc(sizeof(CONVERSIONPROGRAM)*5);
    int numConversionPrograms =0;

    CONVERSIONPROGRAM ** pathway = (CONVERSIONPROGRAM**)malloc(sizeof(CONVERSIONPROGRAM*)*5);


    char **arrayOfTypes = (char**)malloc(sizeof(char*)*5);

    int numTypes=0;


    int numArgs;



    // //Array of printers;
    PRINTER arrayOfPrinters[MAX_PRINTERS];
    int numPrinters=0;

    JOBSTRUCT* headOfListOfJobs=NULL;
    int numJobs =0;

    //PRINTERSET setOfPrinters=0;


    int batchMode = 0;// 0 is interactive mode. 1 is batch mode.

    if(argc==3 && strcmp(argv[1],"-i")==0){
        batchMode =1;
    }

    FILE* file = NULL;

    //int outPutMode =0;


    while(loop){
        char **arrayOfArguments=malloc(sizeof(char*)*5);
        char* input = malloc(sizeof(char)*100);
        numArgs=0;

        if(batchMode==0){
        input = readline("imp>");
        add_history(input); // Adds history to the terminal.
        }
        else{
            if(file == NULL){
                file = fopen(argv[2],"r");
            }
            if(file!=NULL){
                if(fgets(input,100,file) != NULL){

                }
                else{
                    batchMode = 0;
                }
            }
            else{
                char buffer[100];
                char* temp = "file not found.";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
                batchMode=0;
                input = readline("imp>");
            }

        }


        char* word;
        while ((word = strtok(input, "\t\n "))) {
            if(numArgs>=5){
                arrayOfArguments = realloc(arrayOfArguments,sizeof(char*) * (numArgs+1));
            }
            arrayOfArguments[numArgs] = word;
            numArgs++;
            input = NULL;
        }

        if(numArgs>0){

        if(strcmp(arrayOfArguments[0],"help")==0){
            if(numArgs==1){
                printf("%s\n",HELPMENU);
            }
            else{
                char buffer[100];
                char* temp = "too many arguments for help command.";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
            }
        }


        else if(strcmp(arrayOfArguments[0],"quit")==0){ //quit input detected
            if(numArgs==1){
                exit(0);
            }
            else{
                char buffer[100];
                char* temp = "too many arguments for quit comand.";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
            }
        }

        else if(strcmp(arrayOfArguments[0],"type")==0){
             if(numArgs==2){
                if(numTypes>=5){
                    arrayOfTypes=realloc(arrayOfTypes,sizeof(char*)*(numTypes+1));
                }
                arrayOfTypes[numTypes] = arrayOfArguments[1];
                numTypes++;
                printf("Sucessfully added type: %s\n",arrayOfTypes[numTypes-1]);
            }
            else{
                char buffer[100];
                char* temp = "type command accepts the following - type  [file_type]";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
            }
        }


        else if(strcmp(arrayOfArguments[0],"printer")==0){
            if(numArgs==3){
                if(numPrinters<32){
                    if(checkType(arrayOfArguments[2],arrayOfTypes, numTypes)==1){
                        if(checkPrinter(arrayOfArguments[1],arrayOfPrinters, numPrinters)==0){
                            addPrinter(arrayOfArguments[1],arrayOfArguments[2],arrayOfPrinters,numPrinters);
                            char buffer[200];
                            printf("%s\n",imp_format_printer_status(&arrayOfPrinters[numPrinters],buffer,200));
                            numPrinters++;

                            printf("Sucessfully added %s that prints %s files.\n", arrayOfArguments[1],arrayOfArguments[2]);
                        }
                        else{
                            char buffer[100];
                            char* temp = "printer with name already exists.";
                            printf("%s\n",imp_format_error_message(temp, buffer, 100));
                        }
                    }
                    else {
                        char buffer[100];
                        char* temp = "Program not declared to accept type specified. Use type [type] to add.";
                        printf("%s\n",imp_format_error_message(temp, buffer, 100));
                    }
                }
                else{
                    char buffer[100];
                    char* temp = "number of printers cannot be more than 32.";
                    printf("%s\n",imp_format_error_message(temp, buffer, 100));
                }

            }
            else{
                char buffer[100];
                char* temp = "printer command accepts the following - printer [printer_name][file_type].";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
            }
        }





        else if(strcmp(arrayOfArguments[0],"conversion")==0){
            int errorFound =0;
            int flag=0;
            if(numArgs == 4){
                if((getIndex(arrayOfTypes,arrayOfArguments[1],numTypes) != -1 )&& (getIndex(arrayOfTypes,arrayOfArguments[2],numTypes) != -1)){
                    addConversion(arrayOfArguments[1], arrayOfArguments[2], arrayOfArguments[3], matrixOfConversions, NULL, 0, getIndex(arrayOfTypes,arrayOfArguments[1],numTypes), getIndex(arrayOfTypes,arrayOfArguments[2],numTypes));
                    numConversionPrograms++;
                }

                else{
                    errorFound=1;
                    char buffer[100];
                    char* temp = "please declare file type before using a conversion that involes that type. ";
                    printf("%s\n",imp_format_error_message(temp, buffer, 100));
                }
            }
            else if(numArgs > 4 ){
                char* tempArguments[numArgs - 4];
                int tempCounter=4;
                for(int i =0;i<numArgs-4;i++){
                    tempArguments[i] = arrayOfArguments[tempCounter];
                    tempCounter++;
                }
                if((getIndex(arrayOfTypes,arrayOfArguments[1],numTypes) != -1 )&& (getIndex(arrayOfTypes,arrayOfArguments[2],numTypes) != -1)){
                    addConversion(arrayOfArguments[1], arrayOfArguments[2], arrayOfArguments[3], matrixOfConversions, tempArguments, numArgs-4,getIndex(arrayOfTypes,arrayOfArguments[1],numTypes), getIndex(arrayOfTypes,arrayOfArguments[2],numTypes));
                    numConversionPrograms++;
                }
                else{
                    errorFound=1;
                    char buffer[100];
                    char* temp = "please declare file type before using a conversion that involes that type. ";
                    printf("%s\n",imp_format_error_message(temp, buffer, 100));
                }
            }

            else if(numArgs < 4){
                errorFound=1;
            }



            if(errorFound ==0){
                JOBSTRUCT *pointer = headOfListOfJobs;
                for(int i =0;i<numJobs;i++){
                    if((pointer)->job->status == QUEUED){
                        getAvailablePrinter(arrayOfPrinters,numPrinters,&pointer,matrixOfConversions,arrayOfTypes,numTypes,&pathway);
                        if((pointer)->job->status==RUNNING){
                            flag =1;
                            break;
                        }

                    }
                    (pointer)= (pointer)->next;
                }

                if(flag ==1){
                        //fork;
                        //pointer
                        //THIS IS NOT REACHED
                    }
            }



            else{
               errorFound=1;
               char buffer[100];
                char* temp = "conversion command accepts the following - conversion [file1] [file2] [conversionProgram] [Optional:args...]";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
            }
        }




        else if(strcmp(arrayOfArguments[0],"printers")==0){ //printers input detected
            if(numArgs==1){
                printf("Printing Current Printers:\n");
                for(int i=0;i<numPrinters;i++){
                    char buffer[sizeof(arrayOfPrinters)];
                    printf("%s\n",imp_format_printer_status(&(arrayOfPrinters[i]),buffer,sizeof(arrayOfPrinters)));
                }
            }
            else{
                char buffer[100];
                char* temp = "error: Too many arguments for printers command.";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
            }
        }


        else if(strcmp(arrayOfArguments[0],"jobs")==0){ //jobs input detected
            if(numArgs==1){
                printf("Printing Current Jobs:\n");
                for(int i=0;i<numJobs;i++){
                    char buffer[(getSizeOfJob(*getJobAtIndex(headOfListOfJobs,i)))*2];
                    printf("%s\n",imp_format_job_status(getJobAtIndex(headOfListOfJobs,i),buffer,(getSizeOfJob(*getJobAtIndex(headOfListOfJobs,i)))*2));
                }

            }
            else{
                char buffer[100];
                char* temp = "Too many arguements for jobs command.";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
            }
        }


        else if(strcmp(arrayOfArguments[0],"print")==0){
            if(numArgs==2){ // No arguments
                char* type = strrchr(arrayOfArguments[1],'.');
                if(type != NULL){
                type=type+1;
                    if(checkType(type,arrayOfTypes,numTypes)==1){
                        char* eligiblePrinterNames[numPrinters];
                        for(int i =0;i<numPrinters;i++)
                            eligiblePrinterNames[i]=arrayOfArguments[i];

                        JOBSTRUCT* addedJob= addNewJob(arrayOfArguments[1],numJobs,&headOfListOfJobs,eligiblePrinterNames,0,numPrinters,arrayOfPrinters);
                        numJobs++;
                        char nextbuffer[200];
                        printf("%s\n",imp_format_job_status((addedJob)->job, nextbuffer, 200));

                        getAvailablePrinter(arrayOfPrinters,numPrinters,&addedJob,matrixOfConversions,arrayOfTypes,numTypes,&pathway);




                    }
                    else{
                        char buffer[100];
                     char* temp = "type not found.";
                        printf("%s\n",imp_format_error_message(temp, buffer, 100));
                    }
                }
                else{
                    char buffer[100];
                char* temp = "file_name must end with a  .format.";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
                }


            }
            else if(numArgs>2){ // There are arguments
                char* type = strrchr(arrayOfArguments[1],'.');
                 if (type != NULL){
                type=type+1;
                    if(checkType(type,arrayOfTypes,numTypes)==1){
                        char* eligiblePrinterNames[numArgs-2];
                        int temp = 2;
                        int numEligiblePrinters=0;
                        for(int i=0;i<numArgs-2;i++){
                            if(checkPrinter(arrayOfArguments[temp],arrayOfPrinters,numPrinters)==1){
                                eligiblePrinterNames[i]=arrayOfArguments[temp];
                                temp++;
                                numEligiblePrinters++;
                            }
                            else{
                                char buffer[100];
                                char* temp = "printer does not exist.";
                                printf("%s\n",imp_format_error_message(temp, buffer, 100));

                                break;
                            }
                        }



                        if(numEligiblePrinters == numArgs-2){ // If all printers listed are valid
                             JOBSTRUCT* addedJob = addNewJob(arrayOfArguments[1],numJobs,&headOfListOfJobs,eligiblePrinterNames,numArgs-2,numPrinters,arrayOfPrinters);
                             numJobs++;

                            getAvailablePrinter(arrayOfPrinters,numPrinters,&addedJob,matrixOfConversions,arrayOfTypes,numTypes,&pathway);


                        }
                        else{

                            char buffer[100];
                            char* temp = "printers listed not found.";
                            printf("%s\n",imp_format_error_message(temp, buffer, 100));

                        }

                    }
                    else{
                        char buffer[100];
                        char* temp = "type not found.";
                        printf("%s\n",imp_format_error_message(temp, buffer, 100));
                    }

                }
                else{
                    char buffer[100];
                    char* temp = "file_name must end with a .format. Exmaple: file.txt";
                    printf("%s\n",imp_format_error_message(temp, buffer, 100));
                }
            }
            else{
                char buffer[100];
                char* temp = " print command accepts the following - print [file_name] [Optional:printer1 printer 2...]";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
            }
        }




        else if(strcmp(arrayOfArguments[0],"disable")==0){
            int printerIndex=0;
            int foundPrinter=0;
            if(numArgs==2){
                for(int i =0; i<numPrinters;i++){
                    if(strcmp(arrayOfArguments[1],arrayOfPrinters[i].name)==0){
                        arrayOfPrinters[i].enabled=0;
                        foundPrinter=1;
                        printerIndex=i;
                    }
                }

                if(foundPrinter==0){
                    char buffer[100];
                    char* temp = "printer not found";
                    printf("%s\n",imp_format_error_message(temp, buffer, 100));
                    continue;
                }




                char buffer[200];
                printf("%s\n",imp_format_printer_status(&arrayOfPrinters[printerIndex],buffer,200));
            }
        }




        else if(strcmp(arrayOfArguments[0],"cancel")==0){
            if(numArgs==2){
                if(atoi(arrayOfArguments[1])<=numJobs){
                (getJobAtIndex(headOfListOfJobs,atoi(arrayOfArguments[1])))->status = ABORTED;

                if((getJobAtIndex(headOfListOfJobs,atoi(arrayOfArguments[1])))->chosen_printer != NULL ){
                   (getJobAtIndex(headOfListOfJobs,atoi(arrayOfArguments[1])))->chosen_printer->busy = 0;
                    char buffer[100];
                    printf("%s\n",imp_format_printer_status((getJobAtIndex(headOfListOfJobs,atoi(arrayOfArguments[1])))->chosen_printer,buffer,100));

                }

                char nextbuffer[200];
                printf("%s\n",imp_format_job_status((getJobAtIndex(headOfListOfJobs,atoi(arrayOfArguments[1]))), nextbuffer, 200));
                }

                else{
                    char buffer[100];
                    char* temp = "cancel must take in a valid argument";
                    printf("%s\n",imp_format_error_message(temp, buffer, 100));
                    continue;
                }
            }
            else{
                char buffer[100];
                char* temp = "cancel must arguments.";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
                continue;
            }
        }





        else if(strcmp(arrayOfArguments[0],"enable")==0){
            int errorFound=0;
            int flag =0;
            int foundPrinter=0;
            int printerIndex =0;
            if(numArgs==2){
                for(int i =0; i<numPrinters;i++){
                    if(strcmp(arrayOfArguments[1],arrayOfPrinters[i].name)==0){
                        arrayOfPrinters[i].enabled=1;
                        foundPrinter=1;
                        printerIndex = i;

                    }
                }


                if(foundPrinter==0){
                    char buffer[100];
                    char* temp = "printer not found";
                    printf("%s\n",imp_format_error_message(temp, buffer, 100));
                    continue;
                }

                char buffer[200];
                printf("%s\n",imp_format_printer_status(&arrayOfPrinters[printerIndex],buffer,200));

                if(errorFound ==0){
                     JOBSTRUCT *pointer = headOfListOfJobs;
                for(int i =0;i<numJobs;i++){
                    if((pointer)->job->status == QUEUED){
                        getAvailablePrinter(arrayOfPrinters,numPrinters,&pointer,matrixOfConversions,arrayOfTypes,numTypes,&pathway);
                        if((pointer)->job->status==RUNNING){
                            flag =1;
                            break;
                        }

                    }
                    (pointer)= (pointer)->next;
                }

                if(flag ==1){
                        //fork;
                        //pointer
                        //THIS IS NOT REACHED
                    }
                }

            }
            else{
                char buffer[100];
                char* temp = "enable command accepts the following - enable [printer_name]";
                printf("%s\n",imp_format_error_message(temp, buffer, 100));
            }
        }

        else{
            char buffer[100];
            char* temp = " For a list of commands, please type help";
            printf("%s\n",imp_format_error_message(temp, buffer, 100));
        }


        free(arrayOfArguments);
        }
    }




    //FREE EVERYTHING HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    //Free readline input
    //Free all values inside jobs, including job
    // Free values inside of each printer (They are malloced). including printer





    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    char optval;
    while(optind < argc) {
	if((optval = getopt(argc, argv, "")) != -1) {
	    switch(optval) {
	    case '?':
		fprintf(stderr, "Usage: %s [-i <cmd_file>] [-o <out_file>]\n", argv[0]);
		exit(EXIT_FAILURE);
		break;
	    default:
		break;
	    }
	}
    }
    exit(EXIT_SUCCESS);
}


void addPrinter(char* name, char* type, PRINTER* arrayOfPrinters, int numPrinters){
    PRINTER printer;
    printer.id=numPrinters;//UNSURE OF WHAT PRINTER_SET IS
    printer.name = name;
    printer.type = type;
    printer.enabled = 0;
    printer.busy = 0;

    arrayOfPrinters[numPrinters] = printer;
}

void addConversion(char*file1, char* file2, char* name, CONVERSIONPROGRAM matrixOfConversions[][64],char **arguments, int numArgs, int pos1, int pos2){
    CONVERSIONPROGRAM conversion;
    conversion.name = name;
    conversion.fileType1 = file1;
    conversion.fileType2 = file2;

    conversion.arguments = arguments;
    conversion.numArguments = numArgs;

    if(matrixOfConversions[pos1][pos2].name == NULL)
      matrixOfConversions[pos1][pos2]= conversion;
    else{
        char buffer[100];
        char* temp = "conversion already exists.";
        printf("%s\n",imp_format_error_message(temp, buffer, 100));
    }
}

int getIndex(char** arrayOfTypes, char* type, int numTypes){
    for(int i =0;i<numTypes;i++){
        if(strcmp(type,arrayOfTypes[i])==0){
            return i;
        }
    }
    return -1;
}


int BFS (CONVERSIONPROGRAM matrixOfConversions[][64], char* input, char* target, char** arrayOfTypes, int numTypes, CONVERSIONPROGRAM*** pathway){
   CONVERSIONPROGRAM* lastConversion = malloc(sizeof(CONVERSIONPROGRAM*));

   lastConversion = 0;

   int convsersionsInPathway=0;
    for(int row =0;row<numTypes;row++){
        for(int col=0;col<numTypes;col++){
            matrixOfConversions[row][col].visited=0;
            matrixOfConversions[row][col].parent=NULL;
        }
    }

    struct Queue *queue = createQueue();
    int inputIndex = getIndex(arrayOfTypes,input, numTypes);
    for(int col=0;col<numTypes;col++){
        if(matrixOfConversions[inputIndex][col].name != 0){
            matrixOfConversions[inputIndex][col].visited=1;
            enQueue(queue,&(matrixOfConversions[inputIndex][col]));
        }
    }

    while(isEmptyQueue(queue)!= 1){
        CONVERSIONPROGRAM* pointer = deQueue(queue);
        if(strcmp(pointer->fileType2,target)==0){
            lastConversion = pointer;
            break;
        }

    int newIndex = getIndex(arrayOfTypes,pointer->fileType2,numTypes);
        for(int col=0;col<numTypes;col++){
            if(matrixOfConversions[newIndex][col].name!=0){
                if(matrixOfConversions[newIndex][col].visited != 1){
                    matrixOfConversions[newIndex][col].visited=1;
                    matrixOfConversions[newIndex][col].parent = pointer;
                    enQueue(queue,&(matrixOfConversions[newIndex][col]));
                }
            }
        }
    }


    if(lastConversion==0){
        free(lastConversion);
        free(queue);
        return 0;
    }


    while(lastConversion){
        if(convsersionsInPathway >= 5){
            pathway =realloc(pathway, sizeof(CONVERSIONPROGRAM*)*(convsersionsInPathway+1));
        }
        (*pathway)[convsersionsInPathway] = lastConversion;
        convsersionsInPathway++;
        lastConversion = lastConversion->parent;
    }


    int start =0;
    int end = convsersionsInPathway-1;
    CONVERSIONPROGRAM* temp;
    while (start < end)
    {
        temp = (*pathway)[start];
        (*pathway)[start] = (*pathway)[end];
        (*pathway)[end] = temp;
        start++;
        end--;
    }

    free(lastConversion);
    free(queue);
    return convsersionsInPathway;
}

struct QNode* newNode(CONVERSIONPROGRAM* key){
    struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->key = key;
    temp->next = NULL;
    return temp;

}

struct Queue *createQueue(){
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;

}
void enQueue(struct Queue *q, CONVERSIONPROGRAM* key){
    struct QNode *temp = newNode(key);
    if (q->rear == NULL)
    {
       q->front = q->rear = temp;
       return;
    }
    q->rear->next = temp;
    q->rear = temp;
}
CONVERSIONPROGRAM*deQueue(struct Queue *q){
    if (q->front == NULL)
       return NULL;
    struct QNode *temp = q->front;
    CONVERSIONPROGRAM* pointerOfResult = temp->key;
    q->front = q->front->next;
    if (q->front == NULL)
       q->rear = NULL;
    free(temp);
    return pointerOfResult;
}



int isEmptyQueue(struct Queue *queue){
    if(queue->rear==NULL)
        return 1;
    return 0;
}

void printMatrix(CONVERSIONPROGRAM matrixOfConversions[][64], int numTypes){
    for(int row =0; row<numTypes; row++){
        for(int col=0;col<numTypes;col++){
            printf("[%s], ",matrixOfConversions[row][col].name);
            printf("[%s, ",matrixOfConversions[row][col].fileType1);
            printf("[%s]]          ",matrixOfConversions[row][col].fileType2);
        }
        printf("\n\n\n");
    }
}

//Returns 1 if there is already a type with that name.
int checkType(char* type, char** arrayOfTypes, int numTypes){
    for(int i =0; i<numTypes;i++){
        if(strcmp(type,arrayOfTypes[i])==0){
            return 1;
        }
    }
    return 0;
}

//Returns 1 if there is already a printer with that name.
int checkPrinter(char* name, PRINTER* arrayOfPrinters, int numPrinters){
    for(int i =0;i<numPrinters;i++){
        if(strcmp(name,arrayOfPrinters[i].name)==0){
            return 1;
        }
    }
    return 0;
}

void printEligiblePrinters(char* eligiblePrinters[], int numEligiblePrinters ){
    for(int i =0;i<numEligiblePrinters;i++){
        printf("%s \n",eligiblePrinters[i]);
    }
}

JOBSTRUCT* addNewJob(char* fileName, int numJobs, JOBSTRUCT** head, char* arrayOfArguments[], int numArgs, int numPrinters, PRINTER arrayOfPrinters[]){
    //char* file;
    char* type = strrchr(fileName,'.');
    type=type+1;


    JOB* newJob = malloc(sizeof(JOB));
    newJob->jobid = numJobs;
    newJob->status = QUEUED;
    newJob->pgid = 0;
    newJob->file_name = fileName;
    newJob->file_type = type;
    if(numArgs==0){
        newJob->eligible_printers = ANY_PRINTER;
    }
    else{
        newJob->eligible_printers = getSetOfEligiblePrinters(numArgs,arrayOfArguments,numPrinters,arrayOfPrinters);
    }



    newJob->chosen_printer = NULL;//THIS WILL HAVE TO BE IMPLEMENTED USING THE ALGORITHM.
    gettimeofday(&newJob->creation_time,NULL);
    gettimeofday(&newJob->change_time,NULL);



    JOBSTRUCT* jobStruct  = malloc(sizeof(JOBSTRUCT));
    jobStruct-> job = newJob;
    jobStruct->next = NULL;
    jobStruct ->prev = NULL;

    addNode(head,numJobs,&jobStruct);





    return jobStruct;
}



//CONVERSIONPROGRAM matrixOfConversions[][64], char* input, char* target, char** arrayOfTypes, int numTypes, CONVERSIONPROGRAM*** pathway
void getAvailablePrinter(PRINTER arrayOfPrinters[],int numPrinters, JOBSTRUCT** jobstruct, CONVERSIONPROGRAM matrixOfConversions[][64],char** arrayOfTypes, int numTypes, CONVERSIONPROGRAM*** pathway){
    if((*jobstruct)->job->status == QUEUED){
        PRINTER_SET tempSet =(*jobstruct)->job->eligible_printers;
        int selector = 0x1;
        int flag =0;
        int connectedSuccessfully = 0; //0 = Not connected. 1 = directly connected, 2 =  connected using conversions
        PRINTER * selectedPrinter;

        //Check to see if there exists an avaiable printer that can print the job directly.
        if(flag ==0){
            for(int i=0;i<numPrinters;i++){
                if(((selector & tempSet) ==1)){
                    if((arrayOfPrinters[i].enabled ==1)&& (arrayOfPrinters[i].busy == 0)){
                        if(strcmp(arrayOfPrinters[i].type, (*jobstruct)->job->file_type)==0){
                            (*jobstruct)->job->chosen_printer = &arrayOfPrinters[i];
                            (*jobstruct)-> job->status = RUNNING;
                            gettimeofday(&((*jobstruct)->job->change_time),NULL);
                            arrayOfPrinters[i].busy=1;
                            selectedPrinter = &(arrayOfPrinters[i]);
                            flag =1;
                            connectedSuccessfully=1;
                            char buffer[200];
                            printf("%s\n",imp_format_printer_status(&arrayOfPrinters[i],buffer,200));
                            char nextbuffer[200];
                            printf("%s\n",imp_format_job_status((*jobstruct)->job, nextbuffer, 200));

                        }
                        selector = selector << (i+1);
                    }
                }
            }
        }

        //Checks to see if there exists an avaialbe printer with a  conversion pipeline for the job type.
        if(flag ==0){
        selector = 0x1;
            for(int i =0; i<numPrinters; i++){
                if(((selector & tempSet)) == 1){
                    if((arrayOfPrinters[i].enabled == 1) && (arrayOfPrinters[i].busy == 0)){
                        int x = BFS(matrixOfConversions,(*jobstruct)->job->file_type,arrayOfPrinters[i].type,arrayOfTypes,numTypes,pathway);
                        if(x != 0){ //BPS found a valid pathway.
                            (*jobstruct)->  numConversions = x;
                            (*jobstruct)->  pathway = pathway;
                            (*jobstruct)->  job->chosen_printer = &arrayOfPrinters[i];
                            (*jobstruct)->  job->status = RUNNING;
                            gettimeofday(&((*jobstruct)->job->change_time),NULL);
                            arrayOfPrinters[i].busy = 1;
                            selectedPrinter = &(arrayOfPrinters[i]);
                            connectedSuccessfully = 2;
                            char buffer[200];
                            printf("%s\n",imp_format_printer_status(&arrayOfPrinters[i],buffer,200));
                            char nextbuffer[200];
                            printf("%s\n",imp_format_job_status((*jobstruct)->job, nextbuffer, 200));



                        }
                        selector = selector <<(i+1);
                    }
                }
            }
        }

//Direct Connection
        if(connectedSuccessfully == 1){
            pid_t master;
            if((master = fork())== 0){
                pid_t masterid = getpid();
                (*jobstruct)->job->pgid = masterid;
                pid_t childProcess1;

                if((childProcess1=fork()) == 0){
                    setpgid(masterid,getpid());
                    char* argv[] =  {"/bin/cat",(*jobstruct)->job->file_name, NULL};
                    char* envp[] = {NULL};
                    execve("/bin/cat", argv,envp);
                    exit(1);

                }
                else{
                    waitpid(childProcess1,NULL,0);
                }

            }
            else{
                waitpid(master,NULL,0);
                exit(0);
            }

            (*selectedPrinter).busy = 0;
            (*jobstruct)->job->status = COMPLETED;
        }
//Not direct connection
        if(connectedSuccessfully == 2){
            enum fileEnd{READ, WRITE};
            pid_t master;
            if((master=fork()) == 0){
                // int pipeA[2];
                // int pipeB[2];
                // int numPipes = (*jobstruct)->numConversions;
                // int commandNum = numPipes +1;










            }
            else{
                waitpid(master,NULL,0);
                exit(0);
            }
        }





    }
}














//CHecks name and adjusts bits to fill that requirement. EX: ...0101 would mean printer 0, and 2 are eligible.
PRINTER_SET getSetOfEligiblePrinters(int numArgs, char* args[], int numPrinters, PRINTER arrayOfPrinters[]){
    PRINTER_SET  temp = 0;
    for (int i =0;i<numArgs;i++){
        for(int position=0;position<numPrinters;position++){
            if(strcmp(arrayOfPrinters[position].name,args[i])==0){
                temp|=(0x1<<arrayOfPrinters[position].id);
            }
        }
    }
    return temp;
}


size_t getSizeOfJob(JOB job){
    size_t x = 0;
    x = x + sizeof(job.jobid);
    x = x + sizeof(job.status);
    x = x + sizeof(job.pgid);
    x = x + sizeof(job.file_name);
    x = x + sizeof(job.file_type);
    x = x + sizeof(job.eligible_printers);
    x = x +sizeof(job.chosen_printer);
    x = x + sizeof(job.creation_time);
    x = x + sizeof(job.change_time);
    x = x + sizeof(job.other_info);

    return x;
}


void addNode(JOBSTRUCT** head, int numJobs, JOBSTRUCT** newNode){
    if (*head==NULL){
        *head = *newNode;
    }
    else{
        JOBSTRUCT** pointer = head;
        for(int i =0; i<numJobs-1;i++){
            pointer = &(*pointer)->next;
        }
        (*pointer)->next = *newNode;
        (*newNode)->prev = *pointer;
    }
}


JOB* getJobAtIndex(JOBSTRUCT* head, int index){
    JOBSTRUCT* temp = head;
    JOB* temp2=NULL;
    for(int i =0;i<index;i++){
        temp = temp ->next;
    }
    if(temp != NULL)
    temp2 = temp->job;
    return temp2;
}

JOBSTRUCT* getStructAtIndex(JOBSTRUCT* head, int index){
    JOBSTRUCT* temp = head;
    for(int i =0;i<index;i++){
        temp = temp ->next;
    }

    return temp;
}





