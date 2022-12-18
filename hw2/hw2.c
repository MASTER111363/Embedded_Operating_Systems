#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc , char *argv[])

{
    //socket的建立       
    int sockfd = 0,forClientSockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    //socket的連線
    struct sockaddr_in serverInfo,clientInfo;
    int addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_port = htons((unsigned short)atoi(argv[1]));
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd,5);

    while(1){
        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);

        char initial[5] = {};
        recv(forClientSockfd,initial,sizeof(initial),0);
        // printf("initial:%s\n",initial);

        char message[256] = {};
        char reporting_system[20] = {};
        char confirmed_case[20] = {};
        memset(message, '\0', sizeof(message));
        memset(reporting_system, '\0', sizeof(reporting_system));
        memset(confirmed_case, '\0', sizeof(confirmed_case));

        char copy[256] = {};
        memset(copy, '\0', sizeof(copy));
        char *order[20];
        // printf("order[0]:%s\n", order[0]);

        int people_region[9] = {0,0,0,0,0,0,0,0,0};
        int ms_region[9][2] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};

        
        if(strcmp(initial, "list") == 0){
            char initial_list[] = {"1. Confirmed case\n2. Reporting system\n3. Exit\n"};
            send(forClientSockfd,initial_list,sizeof(initial_list),0);

            while(1){

                recv(forClientSockfd,message,sizeof(message),0);
                memcpy(reporting_system,message,16);
                memcpy(confirmed_case,message,14);
                printf("%s\n",message);
                // printf("reporting_system:%s\n",reporting_system);
                // printf("confirmed_case:%s\n",confirmed_case);
                
                // char copy[100] = {};
                memcpy(copy,message,sizeof(message));
                
                int i = 0;
                char *token = strtok(copy, "|");
                order[0] = token;
                while(token!=NULL){
                    order[i] = token;
                    token = strtok(NULL, "|");
                    // printf("order[%d]:%s\n", i, order[i]);
                    i+=1;
                }
                // printf("order[0]:%s\n", order[0]);
                // printf("order len:%d\n",i);

                if(strcmp(confirmed_case, "Confirmed case") == 0){

                    if(strcmp(message, "Confirmed case") == 0){
                        char all_num[256];
                        memset(all_num, '\0', sizeof(all_num));
                        int j=0;
                        for (j=0; j<9; j++){
                            char nn[5];
                            sprintf(nn, "%d", j);
                            strcat(all_num, nn);
                            strcat(all_num, " : ");
                            sprintf(nn, "%d", people_region[j]);
                            strcat(all_num, nn);
                            strcat(all_num, "\n");
                        }
                        // printf("%s",all_num);
                        send(forClientSockfd,all_num,sizeof(all_num),0);

                    }else{
                        char area_ms_num[50] = {};
                        char Comfirmcase[] = {"Area "};
                        // order[1]
                        int Cnumnum;
                        char *p;
                        char Mild_num[5];
                        char Severe_num[5];
                        char *Cnumstr = strtok(order[1]+1, " ");

                        Cnumstr = strtok(NULL, " ");
                        Cnumnum = strtol(Cnumstr, &p, 10);
                        sprintf(Mild_num, "%d", ms_region[Cnumnum][0]);
                        sprintf(Severe_num, "%d", ms_region[Cnumnum][1]);
                        strcat(area_ms_num, Comfirmcase);
                        strcat(area_ms_num, Cnumstr);
                        strcat(area_ms_num, " - Mild : ");
                        strcat(area_ms_num, Mild_num);
                        strcat(area_ms_num, " | Severe : ");
                        strcat(area_ms_num, Severe_num);
                        strcat(area_ms_num, "\n");

                        send(forClientSockfd,area_ms_num,sizeof(area_ms_num),0);
                        // printf("wfwqefwf:%s\n",area_ms_num);
                    }


                }else if(strcmp(reporting_system, "Reporting system") == 0){

                    char wait[] = {"Please wait a few seconds...\n"};
                    send(forClientSockfd,wait,sizeof(wait),0);
                    
                    int waitsecond = 0;
                    char area_mild_n[256] = {};
                    memset(area_mild_n, '\0', sizeof(area_mild_n));
                    for (int k = 1; k<i; k++){
                        strcat(area_mild_n, order[k]+1);
                        // printf("Bsize%ld\n",strlen(order[k]));
                        int areanum, numnum, MorS;
                        char *ptr,*ptr2;
                        if(k%2==0){
                        	
                            if (k < i-1){
                            	int last = strlen(area_mild_n)-1;
		                    	memmove(&area_mild_n[last],&area_mild_n[last+1],strlen(area_mild_n)-last);
		                    }
                         
                            strcat(area_mild_n, "\n");
                            char *numstr = strtok(order[k]+1, " ");
                            if(strcmp(numstr, "Mild") == 0){
                                MorS = 0;
                            }else if(strcmp(numstr, "Severe") == 0){
                                MorS = 1;
                            }

                            numstr = strtok(NULL, " ");
                            numnum = strtol(numstr, &ptr, 10);
                            // printf("num:%d\n", numnum);
                            ms_region[areanum][MorS] += numnum;
                            people_region[areanum] += numnum;

                        }else{
                            strcat(area_mild_n, "| ");
                            char *areastr = strtok(order[k]+1, " ");
                            areastr = strtok(NULL, " ");
                            areanum = strtol(areastr, &ptr2, 10);
                            
                            if (areanum > waitsecond){
                                waitsecond = areanum;
                            }
                            // printf("num:%d\n", areanum);
                        }
                    }

                    sleep(waitsecond);
                    printf("%d second\n",waitsecond);
                    // printf("size%ld\n",sizeof(area_mild_n));

                    send(forClientSockfd,area_mild_n,sizeof(area_mild_n),0);

                }else if(strcmp(message, "Exit") == 0){
                    break;
                }
            }
        }
    }
    return 0;
}