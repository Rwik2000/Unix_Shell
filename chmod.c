#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
int chmod_model(int argc,char *argv[]){
        char *mode=argv[1];
        char *path;
        int exit_status=0;
        if(argc<3){
                printf("Usage: mkdir [mode] [path...]");
                exit_status=1;
        }else{  
                for(int i=2;i<argc;i++){
                        path=argv[i];
                        int i;
                        int k = strtol(mode, 0, 8);
                        if (chmod (path,k) < 0)
                        {
                                perror("chmod() ");
                                exit_status=1;
                        }
                }
        }
        if(exit_status==0){
                printf("CHMOD done!\n");
        }else{
                printf("Error in Chmod\n");
        }
        return exit_status;        
}
