#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
char *str;
int strlength;
char c;
int i;
	FILE* f = fopen (argv[1], "r+");
	str = malloc(81 * sizeof(char));
	if (f == NULL){
		printf("There is no such file in directory\n");
		return 1;
	}
	else{
		while(fgetc(f) != EOF){
			fseek(f, -1 ,  SEEK_CUR);
			fgets(str, 81 , f);
			strlength = strlen(str);
			for (i = 0 ; i < (strlength-1) / 2 ; i++){
				c = str[i];
				str[i] = str[strlength - 2 - i];
				str[strlength - 2 - i] = c;
			}
			fseek(f, -strlength,  SEEK_CUR);		
			fputs(str, f);
		}
	}
	free(str);
	fclose(f);
return 0;
}
