#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
// Listing the files and directories in a location
// specified by the user. If nothing is specified,
// current location is taken.
int ls_model(int argc,char* argv[])
{
    DIR *dir;
    struct dirent *ent;
    char *path;
    int exit_status=0;
    // Checking if user input is a path. If no path is specified,
    // check current directories.
    for(int i=0;i<argc;i++){
        if(argc==1){
            // No path specified.
            path=".";
        }else if(i==0){
            continue;
        }else{
            path=argv[i];
        }
        if ((dir = opendir(path)) != NULL) {
            printf("Directory: %s\n\n",path);
            // Iterating over files/directories.
            while ((ent = readdir(dir)) != NULL) {
                printf ("%s\n", ent->d_name);
        }
        // Closing the directory.
        closedir (dir);
        } 
        else {
            perror ("ls() ");
            exit_status=1;
        }
        printf("\n");
    }
    return exit_status;
}
