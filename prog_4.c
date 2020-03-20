#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int counter = 0;

void SigHndlr(int s){
	counter--;
	signal(SIGCHLD , SigHndlr);
	
}
int main(int argc , char* argv[]){

	FILE* file = fopen(argv[1] , "r");

	int N = atoi(argv[2]);

	int status;
	
	char* path_str = malloc(256 * sizeof(char));
	
	signal(SIGCHLD , SigHndlr);

	while (fgets(path_str , 256 , file) !=  NULL){
		
		path_str[strlen(path_str)-1] = '\0';

		while (counter > N){
			wait(&status);
		}
		
		counter++;

		if (fork() == 0){

			execlp("./prog_3" , "./prog_3" , path_str , NULL);

		}
	}
	
	free(path_str);
	fclose(file);

	return 0;
}
