#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int mkdir_model(int argc,char *argv[]){
        int exit_status=0;
        char *path;
        struct stat st;
        // Iterating over user's input(s).
        for(int i=1;i<argc;i++){
                path=argv[i];
                // making the directory.
                if (mkdir(path,0777)==0) 
                        printf("Directory created %s\n",path); 
                else { 
                        perror("mkdir() "); 
                        exit_status=1; 
                } 
        }
        return exit_status;
}
