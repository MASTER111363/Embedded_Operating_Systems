#include <stdio.h>
#include <stdlib.h>

int people_region[9] = {0,0,0,0,0,0,0,0,0};
int ms_region[9][2] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};

int main(void) {
	while(1){
		system ("clear");
		for (int i=0; i<9; i++){
			people_region[i] = ms_region[i][0] + ms_region[i][1];
		}
		
		printf("1. Confirmed case\n");
		printf("2. Reporting system\n");
		printf("3. Exit\n");
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
				scanf("%c", &region);
				fgetc(stdin);

				if (region-48<=9 && region-48>=0){
					system ("clear");
					printf("Mild : %d\n", ms_region[region-48][0]);
					printf("Severe : %d\n", ms_region[region-48][1]);
					system("sh -c 'read i'");
				}else if(region =='q'){
					break;
				}else{
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
				else{printf("please enter 'm' or 's' in Mild or Severe\n");}

				scanf("%c", &back);
				fgetc(stdin);
				if (back == 'c'){continue;}
				else if (back == 'e'){break;}
				else{printf("please enter 'c' or 'e'\n");}
			}
		}else if(choice == 3){
			break;
		}else{
			printf("please enter 1-3\n");
			fgetc(stdin);
			// system("sh -c 'read i'");
		}
	}
    return 0;
}