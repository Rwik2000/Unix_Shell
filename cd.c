#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// To change the user's directory.
int cd_model(int argc,char *argv[])
{
	char *path;
	int exit_status=0;
	// chdir to change the path.
	// argv[1] is the path included by the user.
	if(argc<2){
		printf("Usage: cd [destinationPath]\n");
		exit_status=1;
	}else{
		path=argv[1];
		if(chdir(path)==0){
			// Success
			printf("Directory Changed !\n");
		}else{
			// Error
			perror("cd() ");
			exit_status=1;
		}
	}
    return exit_status;
	
}


