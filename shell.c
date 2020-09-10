#include "HeaderFiles.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define MAX_CHAR 4096

// This function executes using execvp().
int exec_builtin(int argc,char *argv[]);

// To calculate the number of arguments given by the user.
int num_args(char *argument);

// This function is to check the user's input and redirect the input
// to appropriate sub-functions.
int myshell_loop(int argc,char *argv[]);

// This function is used to execute the function in the foreground 
// or background according to user input i.e. whether the last input
// is "&". 
int myshell_execute(int argc,char *argv[],int amp_check);

// Function to take user input.
char *user_input(char userInput[]);

// The user input is tokenized.
char **tokenize(int argc,char* argv[],char *userInput);

void main()
{
        // variable to check whether to exit a shell.
        int checkShell=0;
        int exit_status=0;
        while(checkShell!=1)
        {
                char userInput[MAX_CHAR];
                user_input(userInput);
                int argc=num_args(userInput);
                int amp_check=0;
                char *argv[argc];
                printf("\n");
                
                // To check if user has given a command or just pressed enter.
                if (strcmp(userInput,"\n") == 0)
                        continue;
                else
                        userInput[strcspn(userInput,"\n")]='\0';

                // Exiting from the shell using "exit" command.
                if(strcmp(userInput,"exit") == 0)
                {
                        printf("\nExiting... \n############# THANK YOU ###############\n");
                        checkShell=1;
                        continue;
                }
                tokenize(argc,argv,userInput);
                
                // Checking & in user's input and removing it from arguments. 
                if(strcmp(argv[argc-1],"&")==0){
                        argv[argc-1]=NULL;
                        argc=argc-1;
                        amp_check=1;
                }                

                exit_status=myshell_execute(argc,argv,amp_check);
                if(exit_status!=0){
                        exit_status=1;
                }    
                printf("EXIT_STATUS: %d\n",exit_status);            
        }
        exit(EXIT_SUCCESS);
}

int exec_builtin(int argc,char *argv[]){
        int exit_status=0;
        int status;
        pid_t pid;
        char *exec_arg[argc+1];
        for(int i=0;i<argc;i++){
                exec_arg[i]=argv[i];
        }
        // Last element of exec_arg is kept as NULL to use it in execvp().
        exec_arg[argc]=NULL;
        pid = fork();
        if (pid<0){
                // If fork fails.
                printf("Fork() failed!");
                exit_status=1;
        }
        else if (pid == 0) {
                // execvp to execute the functions which are not hard-coded.
                if (execvp(exec_arg[0], exec_arg) == -1) {
                        printf("Error in executing the command. Please check the command!\n ");
                        
                }
        }
        else {
                // waits for the child process to complete.
                waitpid(pid,&status,0);
               
        }
        exit_status=WEXITSTATUS(status);

        return exit_status;
}

int num_args(char *argument){
        int i = 0;
        char *result=malloc(strlen(argument)+1);
        strcpy(result,argument);
        char *p = strtok (result, " ");
        int number=0;
        while (p != NULL)
        {
                number=number+1;
                p = strtok (NULL, " ");
        }
        return number;
}

int myshell_loop(int argc,char *argv[]){
        pid_t return_pid;
        int exit_status=0;
        if(strcmp(argv[0],"cd")==0){ 
                // changing directory.
                exit_status=cd_model(argc,argv);
        }else if(strcmp(argv[0],"ls")==0){
                // listing files and directory in a particular location.
                exit_status=ls_model(argc,argv);
        }else if(strcmp(argv[0],"mv")==0){
                // moving files/renaming file(s) from one directory to the other.
                exit_status=mv_model(argc,argv);
        }else if(strcmp(argv[0],"cat")==0){
                // printing the lines of files(s) specifed.
                exit_status=cat_model(argc,argv);
        }else if(strcmp(argv[0],"grep")==0){
                // Checking if a word exists in a file and printing the entire line 
                // containing the word.
                exit_status=grep_model(argc,argv);
        }else if(strcmp(argv[0],"pwd")==0) {
                // printing the current directory.
                char path[MAX_CHAR];
                getcwd(path, sizeof(path));                        
                printf("%s\n",path);
        }else if(strcmp(argv[0],"rm")==0){
                // removing a file/files.
                // Use -r flag to remove a directory i.e. all the files and directories in it.
                exit_status=rm_model(argc,argv);
        }else if(strcmp(argv[0],"mkdir")==0){
                // Making a directory at a particular location.
                exit_status=mkdir_model(argc,argv);
        }else if(strcmp(argv[0],"cp")==0){
                // Copying a file from one directory to other.
                // Use -r to copy directory to a location. 
                exit_status=cp_model(argc,argv);
        }else if(strcmp(argv[0],"chmod")==0){
                // Changing permissions.
                exit_status=chmod_model(argc,argv);
        }
        // Running all the other commands.
        else{   
                exit_status=exec_builtin(argc,argv);
        }
        return exit_status;
}

int myshell_execute(int argc,char *argv[],int amp_check){
        // If statement signifies the process will run in the foreground.
        // amp_check is ampersand(&) check. 
        int exit_status;
        if(amp_check==0){
                exit_status=myshell_loop(argc,argv);
        }

        // If & is present in the user input, the process is forked i.e. a child
        // process is run separatel and user gets back the shell.
        else{
                printf("Starting [%s] process in the background\n",argv[0]);
                int rc=fork();
                int status;
                if(rc<0){
                        fprintf(stderr,"Could not run process in background : '%s'",strerror(errno));
                        exit(1);
                }
                else if(rc==0)
                {
                        exit_status=myshell_loop(argc,argv);
                        exit(0);
                }
        }
        return exit_status;
}

char *user_input(char userInput[]){

        char cwd[MAX_CHAR];
        printf("\nRwikShell...");
        getcwd(cwd,sizeof(cwd));
        printf(" %s >>>$ ",cwd);
        fgets(userInput,100,stdin);
        return userInput;
}

char **tokenize(int argc,char* argv[],char *userInput){
        char buffer[MAX_CHAR];
        int i=0;
        strcpy(buffer, userInput);
        char *token = strtok(buffer," ");
        while(token)
        {
                argv[i]=token; 
                i++;
                token = strtok(NULL, " ");
        }
        return argv;
}


