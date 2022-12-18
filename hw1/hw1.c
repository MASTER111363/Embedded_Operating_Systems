#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int people_region[9] = {0,0,0,0,0,0,0,0,0};
int ms_region[9][2] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
char gpio[10] = {'0','0','0','0','0','0','0','0','0','0'};
char dark[10] = {':','0','0','0','0','0','0','0','0','0'};
void showgpio(char led[]);

int main(void) {
	while(1){
		int total = 0;
		system ("clear");
		showgpio(gpio);
		for (int i=0; i<9; i++){
			people_region[i] = ms_region[i][0] + ms_region[i][1];
			total += people_region[i];
			if (people_region[i]>0){
				gpio[i+1] = '1';
			}
		}

		printf("1. Confirmed case\n");
		printf("2. Reporting system\n");
		printf("3. Exit\n");

		// char int_str[10];
		// sprintf(int_str, "%d", total);
		// for (int j=0; j<strlen(int_str); j++){
		// 	gpio[0] = int_str[j];
		// 	showgpio(gpio);
		// 	usleep(500000);
		// }

		int choice;
		scanf("%d", &choice);
		fgetc(stdin);

		if (choice == 1){
			while(1){

				char region;
				system ("clear");
				for (int j=0; j<9; j++){
					printf("%d : %d\n", j, people_region[j]);
				}

				char int_str[10];
				sprintf(int_str, "%d", total);
				for (int j=0; j<strlen(int_str); j++){
					gpio[0] = int_str[j];
					showgpio(gpio);
					usleep(500000);
				}

				scanf("%c", &region);
				fgetc(stdin);

				if (region-48<=9 && region-48>=0){
					system ("clear");
					printf("Mild : %d\n", ms_region[region-48][0]);
					printf("Severe : %d\n", ms_region[region-48][1]);

					char int_str1[5];
					sprintf(int_str1, "%d", people_region[region-48]);
					for (int k=0; k<strlen(int_str1); k++){
						gpio[0] = int_str1[k];
						showgpio(gpio);
						usleep(500000);
					}

					char temp = gpio[region-48+1];
					gpio[region-48+1] = '1';
					showgpio(gpio);
					usleep(500000);
					gpio[region-48+1] = '0';
					showgpio(gpio);
					usleep(500000);
					gpio[region-48+1] = '1';
					showgpio(gpio);
					usleep(500000);
					gpio[region-48+1] = '0';
					showgpio(gpio);
					usleep(500000);
					gpio[region-48+1] = '1';
					showgpio(gpio);
					usleep(500000);
					gpio[region-48+1] = '0';
					showgpio(gpio);
					usleep(500000);
					gpio[region-48+1] = '1';
					showgpio(gpio);
					gpio[region-48+1] = temp;

					system("sh -c 'read i'");

				}else if(region =='q'){
					showgpio(gpio);
					break;
				}else{
					showgpio(gpio);
					continue;
				}
			}
		}else if(choice == 2){
			while(1){
				int area, num;
				char MS, back;
				system ("clear");
				printf("Area (0~8) : ");
				scanf("%d", &area);
				fgetc(stdin);

				printf("Mild or Severe ('m' or 's') : ");
				scanf("%c", &MS);
				fgetc(stdin);
				// if (MS!='m'&&MS!='s'){printf("please enter 'm' or 's' at confirmed case\n");}

				printf("The number of confirmed case : ");
				scanf("%d", &num);
				fgetc(stdin);

				if (MS == 'm'){ms_region[area][0] += num;}
				else if (MS == 's'){ms_region[area][1] += num;}
				// else{printf("please enter 'm' or 's' in Mild or Severe\n");}

				int total1 = 0;
				for (int i=0; i<9; i++){
					people_region[i] = ms_region[i][0] + ms_region[i][1];
					total1 += people_region[i];
					if (people_region[i]>0){
						gpio[i+1] = '1';
					}
				}

				char int_str[10];
				sprintf(int_str, "%d", total1);
				for (int j=0; j<strlen(int_str); j++){
					gpio[0] = int_str[j];
					showgpio(gpio);
					usleep(500000);
				}

				scanf("%c", &back);
				fgetc(stdin);
				if (back == 'c'){continue;}
				else if (back == 'e'){break;}
				else{printf("please enter 'c' or 'e'\n");}
			}
		}else if(choice == 3){
			showgpio(dark);
			break;
		}else{
			printf("please enter 1-3\n");
			fgetc(stdin);
			// system("sh -c 'read i'");
		}
	}
    return 0;
}

void showgpio(char led[]){	
	FILE *fp;
	fp = fopen("/dev/etx_device", "w+");
	fprintf(fp, "%s\n", led);
	fclose(fp);
}

