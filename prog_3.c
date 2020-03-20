#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* freads(char* string, FILE* f){
	int size = 0;
	char c;
	char* aux_string;
		while (((c = fgetc(f)) != EOF) && (c != '\n')){
			size += 1;

			aux_string = calloc( size , sizeof(char));
			strcpy(aux_string , string);
			string = aux_string;

			string[size - 1] = c;
		}
	if (c == '\n'){
		size += 1;

		aux_string = calloc( size , sizeof(char));
		strcpy(aux_string , string);
		string = aux_string;

		string[size - 1] = c;
	}
	if (size > 0)
		return string;
	else 
		return NULL;
}

main(int argc, char* argv[]){
	char* str;
	str = malloc(sizeof(char));

		FILE* file = fopen (argv[1] , "r");
		FILE* aux = tmpfile();

		while ((str = freads(str , file)) != NULL){
			if (str[0] != '#'){
				fputs(str , aux);
			}
			free(str);
			str = calloc( 1 , sizeof(char) );			
		}

		free(str);
		str = calloc( 1 , sizeof(char) );

		fclose (file);

		file = fopen(argv[1] , "w");

		fseek(aux  , 0 , SEEK_SET);

		while ((str = freads(str , aux)) != NULL){
			fputs(str , file);
			free(str);
			str = calloc( 1 , sizeof(char) );
		}
		fclose (file);
	free (str);
	return 0;
}
