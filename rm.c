#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define MAX_CHAR 4096

// Recursive Deletion is used to delete directories and thier sub-directories.
void recursive_deletion(char *dest_dir){
    
    DIR *dir;
    struct stat st;
    struct dirent *entry;
    char current_path[MAX_CHAR];
    // Changing to destination directory.
    chdir(dest_dir);
    getcwd(current_path,sizeof(current_path));
    // Opening current Directory.
    dir=opendir(current_path);
    // printf("%s\n",current_path);
    // Iterating over the files/directories of the opened directory.
    while((entry=readdir(dir))!=NULL){
        
        char *final_path_name=malloc(strlen(current_path)+1+strlen(entry->d_name)+1);
        
        strcpy(final_path_name,current_path);
        strcat(final_path_name,"/");
        strcat(final_path_name,entry->d_name);
        int check;
        stat(final_path_name,&st);
        // checking if directory is to be deleted or a file.
        if(S_ISDIR(st.st_mode)){
            // Deletion of a sub-directory of the main directory.

            //  . : Current directory and .. : Precious directory. These two are also shown by
            // readdir. And hence we don't delete these directories.
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")){
                continue;
            }else{
                // Calling recursive deletion again on a sub-folder.
                recursive_deletion(entry->d_name);
                chdir(current_path);
                remove(entry->d_name);
            }                    
        }else{
            // Deleting the file.
            remove(entry->d_name);
        }
    }
}
int rm_model(int argc,char *argv[]) {
    int exit_status=0;
    struct stat st;
    DIR *dir;
    struct dirent *entry;
    char current_path[MAX_CHAR];
    getcwd(current_path,sizeof(current_path));
    // Checking if the user has used -r flag.
    if(strcmp(argv[1],"-r")==0){
        if(argc>2){
            for(int i=2;i<argc;i++){
                // Checking if the paths specified by the users exist.
                if(stat(argv[i], &st)!=0){
                    perror("rm()  ");
                    exit_status=1;
                    continue;                
                }
                // Removing using recursion if user has specified a directory.
                if(S_ISDIR(st.st_mode)){
                    recursive_deletion(argv[i]);
                    chdir(current_path);
                    if(remove(argv[i])==0)
                    {
                        printf("Deleted directories and sub directories!!!!\n");
                    }else{
                        perror("rm()  ");
                        exit_status=1;
                    }
                    
                }else{
                    // Removing files.
                    if(remove(argv[i])==0){
                        printf("Deleted file!!\n");
                    }else{
                        perror("rm()  ");
                        exit_status=1;
                    }
                }                
            }
        }else{
            printf("Please Specify the filename/Directory to be deleted after -r flag\n");
            printf("Usage: rm [flag] paths... \n");
            exit_status=1;
        }
    }else{
        if(argc>1){
            // if user hasn not specified flag -r.
            for(int i=1;i<argc;i++){
                // Checking if user input exists.
                if(stat(argv[i], &st)!=0){
                    perror("rm() ");
                    exit_status=1;
                    continue;                
                }
                // Checking if use input is a file or a directory
                if(S_ISDIR(st.st_mode)){
                    printf("Usage: rm -r [directory] to delete the directory ");
                }else{
                    // Removing the specified files.
                    if(remove(argv[i])==0){
                        printf("Deleted File!!\n");
                    }else{
                        perror("rm() ");
                        exit_status=1;
                    }
                }                
            }
        }else{
            printf("Usage: rm [flag] paths... \n");
            exit_status=1;
        }
    }
    
    
    return exit_status;  
}
