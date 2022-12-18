#include <stdio.h>
#include <unistd.h>
#include <string.h> 

int main (int argc, char *argv[]){
	sleep(1);
	for (int i=0; i<strlen(argv[1]); i++){
		FILE *fp;
		fp = fopen("/dev/mydev", "w+");
		fprintf(fp, "%c\n", argv[1][i]);
		fclose(fp);
		printf("%c\n",argv[1][i]);
		sleep(1);
	}
	return 0;
}
