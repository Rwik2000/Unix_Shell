#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#define PATH_MAX 4096

// Function to move a file from initial to final directory
int mv_swap(int num_args,char **argv,int index){
    int exit_status=0;
    char dest_fd[PATH_MAX];
    // Creating final path name of the file to be moved.
    strcpy(dest_fd,argv[num_args-1]);
    strcat(dest_fd,"/");
    // Extracting filename from the initial path specified using basename().
    char *initial_filename=basename(argv[index]);
    strcat(dest_fd,initial_filename);
    // changing the location to new location using rename().
    if(rename(argv[index],dest_fd)==-1){
        perror("mv() ");
        exit_status=1;
    }
    printf("%s Moved to Specified Directory!!\n",initial_filename);
    return exit_status;    
}


int mv_model(int argc, char *argv[]){
    struct stat st;
    int num_arg=argc;
    int exit_status=0;
	if(argc < 3){
		printf("Usage: mv [filePath] [destinationPath]");
        exit_status=1;
	}else if(argc==3){
        // Moving a single file

        // checking if first argument by user exits.
        if(stat(argv[1], &st)==0){

            // Checking if the first argument is a directory or a file.
            // If a directory is specified as the first argument, Usage
            // error is thrown.
            if(S_ISDIR(st.st_mode)){
                printf("'%s' mv can ONLY change path of non-directory\n",argv[1]);
                exit_status=1;
            }
            else{
                // Checking if second argument exists.
                stat(argv[2],&st);
                // if(stat(argv[2], &st)){
                // Checking if second argument is a file or a directory.
                    if(S_ISDIR(st.st_mode)){
                        exit_status=mv_swap(num_arg,argv,1);
                    
                    }else{
                        if(stat(argv[2], &st)==0){
                            remove(argv[2]);
                        }
                        if(rename(argv[1],argv[2])==-1){
                            perror("mv() ");
                            exit_status=1;
                        }
                    }
                // }
            }
        }else{
            perror("mv()");
            exit_status=1;            
        }
    }else{
        // Multiple files are specified, then code will run in this block.
        // argc>3.

        // Checking if final argument exists and is a directory.
        if(stat(argv[argc-1], &st)==0){
            if(S_ISDIR(st.st_mode)){                
                for(int i=1;i<argc-1;i++){
                    if(stat(argv[i],&st)==0){
                        
                        if(S_ISDIR(st.st_mode)){
                            printf("'%s' mv can ONLY change path of non-directory\n",argv[i]);
                            exit_status=1;
                        }else{
                            mv_swap(num_arg,argv,i);
                        }
                    }
                }
                
            }else{                
                perror("mv() ");
                exit_status=1;                
            }
        }else{
            perror("mv() ");
            exit_status=1;
        }
    }   
    if(exit_status==0){
        printf("Moved!!\n");
    }
    return exit_status;
}
