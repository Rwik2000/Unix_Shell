#include<stdio.h> 
#include<unistd.h> 
#include<stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include<string.h> 
#include <fcntl.h> 
#include <sys/wait.h>
#define PATH_MAX 4096

int grep_model(int argc,char *argv[]) 
{ 
    int exit_status=0;
    struct stat st; 
    FILE * req_file;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int empty_offset=0;
    char *check_word;
    
// empty offset corresponds to the offset to be made in argv to ensure proper iteration over the files.
// if user specifies " " as the string to search, argv[1] becomes " and argv[2] becomes ". Thus in this case,
// iteration must begin from 3rd argument i.e. 2+empty_offset.
    if((strcmp(argv[1],"\"")==0 && strcmp(argv[2],"\""))==0){
        empty_offset=1;
        check_word=" ";
    }else if(strcmp(argv[1],"\"\"")==0){
        check_word=" ";
    }
    else{
        check_word=argv[1];
    }

    if(argc>=3+empty_offset) 
    {   
        
        for(int i=2+empty_offset;i<argc;i++){
            
            if(stat(argv[i],&st)==0){                
                
                if((req_file = fopen(argv[i], "r"))!=NULL){                   
                    while ((read = getline(&line, &len, req_file)) != -1) {
                        if(strstr(line,check_word)!=NULL){
                            if(argc>=3){
                                if(argc>3+empty_offset){
                                    printf("%s : ",argv[i]); 
                                }
                            printf("%s",line);               
                            }
                        }
                    }
                }else{
                    perror("grep() ");
                    exit_status=1;
                }            
            }
            else{
                perror("grep: ");
                exit_status=1;
            }
            printf("\n");
        }
    }
    else if(argc==2+empty_offset){
        while(1){
            char string[PATH_MAX];
            fgets(string, PATH_MAX, stdin );
                if(strstr(string,argv[1])!=NULL){
                    printf("%s\n",string);               
                }
            }
        }
    else if(argc==1)
    { 
        printf("wgrep: searchterm [file ...]\n"); 
        exit_status=1; 
    } 
    return exit_status;

} 
