//kevin Johnson 101077070
// Jack Hendry 101079591 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
struct for Process control block
PCB are created for each intruction with unique PID to identify
*/
typedef struct PCB {
	int PID;
	int ARIVAL_TIME;
	int TOTAL_CPU_TIME;
	int I_O_Fq;
	int I_O_Duration;
	int TOTAL_FREQ;
    int PCBno;
	int oldState;
    int state;
//	struct PCB *last;
} PCB_t;

/*
get total number processes from input file 
*/
int lineCounterInFile(FILE** fileCount){
    int noLines = 0;
	while(!feof(*fileCount)){
    		char singleLine[100]; 
    		fgets(singleLine, 100, *fileCount);
    		noLines++;
  	}
    return noLines;
}

/*
initalizes all the PCB's 
based on the input file instructions 
*/
void intializeProcesses(FILE ** fPolonger, struct PCB * ptr, int noLines){
    int numberOfLines =0; 
    while(!feof(*fPolonger)){ // if file polonger hasn't reached end keep running
        
        char singleLine[100];  //stores single line of the text
    
        fgets(singleLine, 100, *fPolonger);  //gets single 

		// assigns variable based the delimitter (" ") in the input fie 

		// assigns the processes PID number to the structs PID number
        ptr[numberOfLines].PID = atoi(strtok(singleLine, " "));  
        printf("PID: %d\n", ptr[numberOfLines].PID);
     
        // assigns the processes Arrival time to the structs Arrival time 
        ptr[numberOfLines].ARIVAL_TIME = atoi(strtok(NULL, " "));
        printf("Arrival Time:  %d\n", ptr[numberOfLines].ARIVAL_TIME);

		// assigns the processes Total CPU Time to the structs Total CPU Time 
        ptr[numberOfLines].TOTAL_CPU_TIME = atoi(strtok(NULL, " ")); 
        printf("Total CPU Time: %d\n", ptr[numberOfLines].TOTAL_CPU_TIME);

		// assigns the processes I/O Frequency to the structs I/O Frequency 
        ptr[numberOfLines].I_O_Fq = atoi(strtok(NULL, " "));
        printf("I/O Frequency: %d\n", ptr[numberOfLines].I_O_Fq);

		// assigns the processes I/O Duration to the structs I/O Duartion
        ptr[numberOfLines].I_O_Duration = atoi(strtok(NULL, " "));
        printf("I/O Duration: %d\n", ptr[numberOfLines].I_O_Duration);
        
        // assigns line on which the processes was on the input file. 
        ptr[numberOfLines].PCBno = numberOfLines; 
        printf("Process Number: %d\n",ptr[numberOfLines].PCBno);

		// assigns processes with an intial state of ready 
        ptr[numberOfLines].state = 0; 
        printf("State: %d\n\n",ptr[numberOfLines].state);
		
        numberOfLines++;
    } 
}

/*
Ensures all of the processes from input file has a unique PID number
*/
bool uniquePID(PCB_t * p,int lines){
	
	for(int i = 0; i<lines; i++){
		for(int j=i+1; j<lines; j++)
        {
            /* If duplicate found then increment count by 1 */
            if(p[i].PID == p[j].PID)
            {
                printf("ERROR : Process Numbers: %d , %d have the same PID\n", p[i].PCBno, p[j].PCBno);
				return false;
            }
        }
	}
	printf("No Duplicate PID !!!!\n \nCurrent Time    PID     Previous State        New State \n");
	return true; 
}
/*
converts the integer states to their corresponding String 
*/
char *intToString(int state){
	//char state2str [10];
	if (state == 0){
		return "READY   ";
	}
	else if(state == 1){
		return "RUNNING";
	}else if (state == 2){
		return "WAITING";
	}
	else {
		return "TERMINATED";
	}
}
/*
Records of transitions from the state diagram are documented in a common txt file 
*/
void output(FILE * fptr, int currTime, int PID, int oldState, int newState){
	
	static int timmy = 0; // ensures that Titles for output file only happens 
	fptr = fopen("output.txt", "a");
	if(timmy ==0){
		fprintf(fptr,"\n\nCurrent Time\t\t\t PID\t\t\t Previous State\t\t\t New State"); 
		timmy++; 
	}
	char outputLine[50]; 
	// double check to make sure we finish all processes
	if(fptr==NULL){
		printf("Finished all processes"); 
		exit(1); 
	}
	/*
	prints to output file: current time, PID number , 
	the previous state & the new state converted to a string
	*/
	fprintf(fptr, "\n%d\t\t \t\t%d\t \t\t%s \t\t\t %s",currTime,PID,intToString(oldState),intToString(newState)); 
	fclose(fptr); 

}



/*
Checks to see if all the PCBs are terminated 
*/
bool terminated(PCB_t *ptr,int lines){
	bool flag = true;    
   	for (int i = 0; i<lines; i++){
    	if (!(ptr[i].state == 3)){
		 	flag = false; 
	 	}
   }
   return flag;
}

/*
Checks to see if any PCB are RUNNING 
*/
bool running(PCB_t *ptr,int lines){
	bool flag = false;    
   	for (int i = 0; i<lines; i++){
    	if (ptr[i].state == 1){
		 	flag = true; 
	 	}
   }
   return flag;
}
/*
used to change the state of the PCB based on time 
array of states is used to represent states
method works by wrapping everything in a while loop whose parameter checks if all PCBs are terminated
the for loop goes through the PCBs constantly to change the states 
PCBs change states based on clk 
ARRIVAL_TIME is synced to the clk 
based on that the PCBs  IO freq, IO duration and CPU time are used to change states along with states of other PCBs
*/
void stateDriver(PCB_t* ptr, int lines, int clk){

    // states: 0 = ready, 1 = running, 2 = waiting, 3 = terminated
	const int states[] = {0, 1, 2, 3};
	FILE * fptr;   
	fptr = fopen("output.txt", "a");      

	if(uniquePID(ptr,lines)){  
		while(!(terminated(ptr,lines))){
			clk++;     //global clock used to simulate seconds
			//if statement to switch from READY to RUNNING
			for (int i = 0; i < lines; i++){
				if (ptr[i].state == states[0]){ // checking if state is == 0 (ready)
					if ((ptr[i].ARIVAL_TIME) <= clk){          //checks if arrival time is less than or equal to clk
						if (!running(ptr,lines)){			 //checks to see if any PCBs are running      
							ptr[i].state = states[1];           //switches state from READY TO RUNNING
							ptr[i].oldState = states[0];		//switches old state from NULL to READY
							output(fptr,clk,ptr[i].PID , ptr[i].oldState, ptr[i].state); 			//outputs to the output.txt			
							printf("%d\t\t", clk);
							printf("\t\t%d\t", ptr[i].PID);	
							printf("\t\t%d \t\t\t\t %d\n", ptr[i].oldState, ptr[i].state);
							ptr[i].ARIVAL_TIME = clk;                 //updates arrival time when the PCB has changed states all the way back to running
						}	
					} 	
				// checks if state is RUNNING and decides to move to TERMINATED or WAITING
				}
				if (ptr[i].state == states[1]){ 
					//moves from RUNNING to TERMINATED
					//arrival time is updated in READY to RUNNING
					//total frequcny is a running counter of time the IO freq has run for
					//(clk-arrival time) time count from 1-19
					//used to see if TERMINATED before freqency reaches total
					if((clk)-(ptr[i].ARIVAL_TIME)+(ptr[i].TOTAL_FREQ) >= ptr[i].TOTAL_CPU_TIME) {
							ptr[i].state = states[3];     //changing states 
							ptr[i].oldState = states[1];
							output(fptr,clk,ptr[i].PID,ptr[i].oldState, ptr[i].state);
							printf("%d\t\t", clk);
							printf("\t\t%d\t", ptr[i].PID);	
							printf("\t\t%d \t\t\t\t %d\n", ptr[i].oldState, ptr[i].state);
							if(i == (lines- 1) | ptr[i+1].state == states[3]){   //decrement clk by one to get rid of delay 
								clk = clk -  1;
							}
					//moves from RUNNING to WAITING 
					}else if((ptr[i].I_O_Fq + ptr[i].ARIVAL_TIME) == clk){
						ptr[i].TOTAL_FREQ = ptr[i].TOTAL_FREQ + ptr[i].I_O_Fq;  //if waited the whole IO freq adds it to the total freq  
						ptr[i].state = states[2];         //changes state
						ptr[i].oldState = states[1];
						output(fptr,clk,ptr[i].PID,ptr[i].oldState, ptr[i].state);
						printf("%d\t\t", clk);
						printf("\t\t%d\t", ptr[i].PID);	
						printf("\t\t%d \t\t\t\t %d\n", ptr[i].oldState, ptr[i].state);
						if(i == (lines- 1) | ptr[i+1].state == states[3]){    //decrement clk by one to get rid of delay
							clk = clk -  1;
						}
					}
				//moves from WAITING to READY
				//check to see if arrival time + IO freq+ IO duration equals clk then goes back to ready
				}else if ((ptr[i].state == states[2]) & ((ptr[i].I_O_Fq+ptr[i].ARIVAL_TIME +ptr[i].I_O_Duration)  == clk)){ 
					ptr[i].state = states[0];   		//changes states
					ptr[i].oldState = states[2];
					output(fptr,clk,ptr[i].PID,ptr[i].oldState, ptr[i].state);
					printf("%d\t\t", clk);
					printf("\t\t%d\t", ptr[i].PID);	
					printf("\t\t%d \t\t\t\t %d\n", ptr[i].oldState, ptr[i].state);
					if(i == (lines- 1) | ptr[i+1].state == states[3]){		//decrement clk by one to get rid of delay
							clk = clk -  1;
					}
				} 
			}
		} 
		printf("\n All processes have been terminated !!!"); // all state have been termianted
	
	} 
}
/*
	main function
	thoughout the code integer values are used to represent the states 
	0 = READY
	1 = RUNNING
	2 = WAITING
	3 = TERMINATED
*/
int main() {

  FILE * fPolonger; // polongs to a file
  FILE * FileCount; // polongs to a file
	
  fPolonger = fopen("input.txt", "r"); // opens up notepad, reads inputted file 
  FileCount = fopen("input.txt", "r");
  int noOfLinesInFile = lineCounterInFile(&fPolonger);  //used to find numebr of lines so we can instantiate the number of PCB's
  int clock =0; //global clk
  

  PCB_t *ptr = malloc(noOfLinesInFile * sizeof(PCB_t));          //allocates memory on heap for PCBs based on amount of process'
  intializeProcesses(&FileCount,ptr, noOfLinesInFile);		//initializes all PCBs based on process'

  stateDriver(ptr,noOfLinesInFile, clock);					//changes states of PCBs
return 0; 
}
