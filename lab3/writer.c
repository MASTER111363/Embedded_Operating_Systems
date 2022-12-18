#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv[]){
	for (int i=0; i<9; i++){
		FILE *fp;
		fp = fopen("/dev/etx_device", "w+");
		fprintf(fp, "%c\n", argv[1][i]);
		fclose(fp);
		printf("%c\n",argv[1][i]);
		sleep(1);
	}

	return 0;
}

