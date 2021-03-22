#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait */
#include <stdio.h>     /* printf */
#include <unistd.h>    /* _exit, fork, execvp */
#include <fcntl.h> 	/*open*/
#include <stdlib.h>
#include "myutils.h"
#define BSIZE 80
#define NCMDS 10

//gcc -pthread -o main final.c myutils.c

typedef struct cmd_struct{
	char cmd[BSIZE];
	char args[NCMDS][BSIZE];
	int nargs;
	int is_concurrent;

} Cmd_type;



int main (int argc, char *argv[]){

	//counters
	int ncmds = 0; // command counter
	int is_command = 1; // command flag
	int nparams = 0; //parameters counter
	
	Cmd_type cmds[NCMDS];
	char buffer[BSIZE],ch_end;
	int fin = open("cmds_all.txt",O_RDWR);
	int in_num; // input number
	//check if the file exist
	if (fin < 0){
		printf("File does not exist\n");
		exit(1);
	}
	//WRITTING AND STORING THE FILE
		while(read_split(fin,buffer,BSIZE,&ch_end) > 0){
			if(strlen(buffer) > 0){ //check if the buff is not a whitespace
				if(is_command == 1){ //check if the string is a command
					sscanf(buffer,"%s",cmds[ncmds].cmd);
					is_command = 0;
				}else{ //if is not a command
					sscanf(buffer,"%s",cmds[ncmds].args[nparams]);
					nparams++;
				}

			}
			if(ch_end == '&'){ //checking if a parameter is & to set concurrency
					cmds[ncmds].is_concurrent = 1;
					sscanf(&ch_end,"%s",cmds[ncmds].args[nparams]);
					nparams++;
					}
			if (ch_end == '\n'){ //if it reaches a new command line, we reset the variables
				cmds[ncmds].nargs = nparams;
				ncmds++;
				is_command=1; 
				nparams = 0; 
			}
		
	}
	//STARTING THE SHELL 
	while(1){
		write(0,"MyShell>",strlen("MyShell>"));
		read_split(1,buffer,80,&ch_end);
		
		// ------- LIST: it prints all commands -------
		if(strcmp(buffer,"list") == 0){
			for (int i=0;i<NCMDS;i++ ){
				if(*cmds[i].cmd != '\0'){ //iterate until the pointer points to an empy space
					printf("%d:%s",i+1,cmds[i].cmd);
					for(int j=0;j<cmds[i].nargs;j++){ //print al arguments if it has
						printf(" %s",cmds[i].args[j]);
					}
					printf("\n");
					
				}else break;
			}
		//------- EXIT:we exit the program.----------
		}else if(strcmp(buffer,"exit") == 0){
		break;
		//-------NUMBER: we execute the command in this line -----------
		}else{
			in_num = 0;
			sscanf(buffer,"%d",&in_num); // store the input into a variable
			if(in_num > 0 && in_num < ncmds+1){ //check if it's a valid imput
			
				//transform data struct into malloc to apply the execvp 
				in_num--; //decrease number for the acces: line1 = cmd[0] 
				char* current_cmd[10];
				current_cmd[0] = malloc(80);
				strcpy(current_cmd[0],cmds[in_num].cmd); //store command to first space
				int current_nargs = cmds[in_num].nargs;
				for (int i = 0; i<current_nargs;i++){   // store the arguments in the next spaces
					current_cmd[i+1] = malloc(80);
					// the execvp does not catch & so we put null and execute concurrently later
					if(strcmp(cmds[in_num].args[i],"&") == 0){
						current_cmd[current_nargs]=NULL; 
						break;
					}
					strcpy(current_cmd[i+1],cmds[in_num].args[i]);
				}
				current_cmd[current_nargs+1]=NULL;
				
				//creating process
				int pid = fork();
					if(pid == 0){
						execvp(current_cmd[0],&current_cmd[0]);
						_exit(0);
					}if(cmds[in_num].is_concurrent == 1){
						write(1,"doing concurrently\n",strlen("doing concurrently\n"));
					}else{
						wait(NULL);
					}
			}else{
				printf("Please insert: exit, list or a number between 1 and and %d\n",ncmds);
			}
		
		} 
	}

}
