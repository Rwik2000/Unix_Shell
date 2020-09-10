#include<stdio.h>
#include<unistd.h> 
#include<stdlib.h> 
#include<fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>

// To read all the lines of all the files specified by the user.
int cat_model(int argc,char *argv[])
{ 
    int exit_status=0;
    struct stat st;
    FILE * req_file;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    if(argc<2){
        // Usage error.
        printf("Usage: cat [filepaths...]\n");
        exit_status=1;
    }else{
        for (int i= 1; i < argc; i++) {  
            // Checking if specified file exists.   
            if(stat(argv[i],&st)==0){   
                // Opening the specified file in read format.  
                req_file = fopen(argv[i],"r");
                if(req_file==NULL){
                    // If fopen returns NULL pointer.
                    printf("cat(): Couldn't open %s file",basename(argv[i]));
                    exit_status=1;
                    continue;
                }            
                // Iterating through all the lines.
                if(argc>2){
                    printf("File: %s\n",argv[i]);
                }                
                while ((read = getline(&line, &len, req_file)) != -1){
                    printf("%s",line);                    
                }                                                       
            }else{
                perror("cat(): ");
                exit_status=1;
            }
            printf("\n");
        }
    }
    if(exit_status==0){
        printf("\nCAT done!!\n");
    }
    return exit_status;
}
