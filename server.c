#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>


#define SIZE 1024
#define BUFFER_SIZE 32

void str_trim_lf (char* arr, int length) {
  int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') {
      arr[i] = '\0';
      break;
    }
  }
}


int main(){

	FILE *fp;

	char * ip="127.0.0.1";
	int port=8080;
	int n;
	
	int sockfd,new_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_size;
	char buffer[SIZE];
		int option = 1;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		perror("	[-] SOKET HATASI	\n");
		exit(1);
	}
	
	printf("	[+] SERVER SOKET YARATILDI	\n ");
	
	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	server_addr.sin_addr.s_addr=inet_addr(ip);
	
	socklen_t serverAddrSize=sizeof(server_addr);
	
	int b=bind(sockfd,(struct sockaddr*)&server_addr,serverAddrSize);
	if(b<0)
	{
		perror("	[-] BINDING HATASI \n");
		exit(1)	;
	}	
	printf("	[+] BINDING BASARILI	\n");
	

	int l=listen(sockfd,10);
	
	if(l==0){		printf("	[+] Dinleniyor......\n");	}
	else{	
		perror("	[-] Dinlenemiyor....!\n");
		exit(1);
	}
	
	

	char *recvDosyaAdi;
	int recvDosyaBoyutu;

	char secim[SIZE]={0};

	addr_size=sizeof(client_addr);
	while (1)
	{
		int cli=accept(sockfd,(struct sockaddr *)&client_addr,&addr_size);
		if(cli<0)
		{
			perror("\t[-] kabul edilmedi....!\n");
			exit(1);
		}
		else
		{
			printf("\t[+] kabul edildi...%s;%d \n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));	
			
		}


		bzero(buffer, BUFFER_SIZE);
    		n = read(cli, buffer, BUFFER_SIZE);
   		if (n < 0) error("ERROR reading from socket");

    		 printf("Received file name: %s\n", buffer);
		
		

		close(cli);
		bzero(buffer, SIZE);


	

   		 
      
			
	
	
	}

	
	
	
	
	
	
	
 }

