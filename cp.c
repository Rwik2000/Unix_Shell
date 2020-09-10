#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#define PATH_MAX 4096

// Function to copy files from source to destination.
int cp_swap_file(char *init_path,char *dest){
    int exit_status=0;
    // Opening source file using READ.
    FILE *initial=fopen(init_path,"r");
    // Opening/Creating destination file using WRITE. Note: existing lines in the file will be truncated.
    FILE *final=fopen(dest,"w");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    if(initial==NULL) { 
        printf("cp: cannot open file\n"); 
        exit_status=1; 
    }else{
        while ((read = getline(&line, &len, initial)) != -1) {
            fputs(line,final);                   
    }
    fclose(initial);
    fclose(final);

    }
    return exit_status;
}

// Function to copying a directory (only sub-files and NOT sub- directories) to destination.
int cp_swap_directory(char *init_path,char *dest,char *dir_name){
    int exit_status=0;
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    char *current_path;
    char *dest_path;
    char *dest_dirname;
    dest_dirname=malloc(PATH_MAX+1);
    dest_path=malloc(PATH_MAX+1);
    current_path=malloc(PATH_MAX+1);
    strcpy(dest_dirname,dest);

    strcat(dest_dirname,"/");
    strcat(dest_dirname,basename(init_path));
    // Making the directory of the same name in the destination directory.
    mkdir(dest_dirname,0777);
    int i=0;
    if ((dir = opendir(init_path)) != NULL) {
        // Iterating over the contents of the directory.
        while ((ent = readdir(dir)) != NULL) {
            char *init_path_file=malloc(strlen(init_path)+strlen(ent->d_name)+2);
            strcpy(init_path_file,init_path);
            strcat(init_path_file,"/");
            strcat(init_path_file,ent->d_name);
            stat(init_path_file, &st);
            // Skipping the subdirectories.
            if(S_ISDIR(st.st_mode)){
                continue;
            }else{                
                // Copying files to new directory.
                strcpy(current_path,init_path);
                strcat(current_path,"/");
                strcat(current_path,ent->d_name);
                strcpy(dest_path,dest_dirname);
                strcat(dest_path,"/");
                strcat(dest_path,ent->d_name);
                // Calling cp_swap_file to copy file from source to destination.
                exit_status=cp_swap_file(current_path,dest_path);
            }        
        }
    }else{
        perror("cp() ");
        exit_status=1;
    }
    return exit_status;
}

// Main Function
int cp_model(int argc, char *argv[]){
    int exit_status=0;
    struct stat st;
    int num_arg=argc;
    int mode_offset=0;
	if(argc < 3){
		printf("Usage: cp [mode] [filePath] [destinationPath]\n");
        exit_status=1;
	}else if(argc==3){
        // Three arguments imply -r flag will not be used, If used, error will be thrown.
        
        // Checking if file to be copied exists. 
        if(stat(argv[1], &st)==0 && strcmp(argv[1],"-r")!=0){
            // If first input path is a directory. Usage error will be thrown.
            if(S_ISDIR(st.st_mode)){
                printf("cp cannot copy directory without -r flag \n");
                exit_status=1;
            }else{
                // Checking if second argument exists.
                if(stat(argv[2], &st)==0){                
                    char *dest=malloc(strlen(argv[2])+1); 
                    strcpy(dest,argv[2]); 
                    // If destination path specified is a directory, final path of the file is made.
                    if(S_ISDIR(st.st_mode)){
                        strcat(dest,"/");
                        strcat(dest,argv[1]);
                        // dest is the destination path of the copied file.
                        exit_status=cp_swap_file(argv[1],dest);

                    }else{
                        // If destination is a path of the final file.
                        exit_status=cp_swap_file(argv[1],argv[2]);                        
                    }
                }else{
                    perror("cp() ");
                    exit_status=1;
                }
            }
        }else{
            printf("File to be copied does not exist or the file to be copied is named \"-r\"(use -r flag in this case) \n");
            exit_status=1;
        }
    }
    else{
        // If number of srguments is greater than 3, there is a possibility that user may have used -r flag.
        if(stat(argv[argc-1], &st)==0){
            if(strcmp(argv[1],"-r")==0){
                // mode offset ensures that the iteration will start from the correct place if -r if given as the flag.
                mode_offset=1;
            }            
            char *dest;
            dest=malloc(PATH_MAX+1);
            // Checking if destination is a directory. 
            if(S_ISDIR(st.st_mode)){    
                // Iterating over all the arguments of the user. 
                for(int i=1+mode_offset;i<argc-1;i++){
                    char *initial_filename=basename(argv[i]);
                    strcpy(dest,argv[argc-1]);                    
                    strcat(dest,"/");
                    strcat(dest,argv[i]);
                    stat(argv[i], &st);
                    // Checking if an argument is a directory.
                    if(S_ISDIR(st.st_mode)){
                        if(mode_offset==1){
                            // copying a directory only if -r flag is used by the user.
                            exit_status=cp_swap_directory(argv[i],argv[argc-1],initial_filename);
                        }else{
                            exit_status=1;
                            printf("cp cannot copy directory without -r flag \n");
                        }
                    }else{
                        // copying a file.
                        exit_status=cp_swap_file(argv[i],dest);
                    }
                }
                
            }else{    
                exit_status=1;            
                perror("cp() ");
                
            }
        }else{
            exit_status=1;
            perror("cp() ");            
        }
    }
    if(exit_status==0){
        printf("Copied!\n");
    }else{
        printf("Error in Copying a file(s)/directory(s) or sub-file(s)\n");
    }
    return exit_status;
}
