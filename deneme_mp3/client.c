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

void str_overwrite_stdout()
{
	printf("%s", "> ");
	fflush(stdout);
}
void str_trim_lf(char *arr, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{ // trim \n
		if (arr[i] == '\n')
		{
			arr[i] = '\0';
			break;
		}
	}
}

int main()
{

	char *ip = "127.0.0.1";
	int port = 8080;

	
   	 FILE *out;

	int sockfd,n;
	struct sockaddr_in server_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("\t[-] SOKET HATASI	\n");
		exit(1);
	}
	printf("\t[+] SOKET YARATILDI	\n ");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);

	printf(" HOSGELDINIZ \n");
	printf("\t ISLEM SECENEKLERİ\n");
	printf("\t 1.Servera Dosya Kaydet\n");
	printf("\t 2.Dosya Oynat\n");
	printf("\t 3.Cikis\n");
	printf("\t secim\n");

	char buffer[BUFFER_SIZE];
	int dosyaBoyutu;

	int c = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (c < 0)
	{
		perror("	[-] CONNECTION ERROR \n");
		exit(1);
	}
	printf("	[+] CONNECTED TO SERVER \n");

	char secim[SIZE];
	printf("\tIslemSecimi  :");
	fgets(secim, SIZE, stdin);
	printf("SECIMINIZ : '%s'\n", secim);

	int s = send(sockfd, secim, SIZE, 0);
	if (s < 0)
	{
		printf("\t [-] Secim gonderilemedi.....:\n");
		exit(1);
	}
	else
	{
		printf(" \t [+] SECIM GONDERİLDİ \n");
	}



	str_trim_lf(secim, SIZE);
	
	if (strcmp(secim,"1")==0)
	{

		printf("\t Dosya adini giriniz : \n");
 	  	 bzero(buffer, BUFFER_SIZE);
   	 	fgets(buffer, BUFFER_SIZE - 1, stdin);
    		buffer[strlen(buffer) - 1] = '\0';
    		
			printf(" \t [+] 1 GONDERİLDİ \n");
			printf(" \t [+] %s \n", buffer);
	
		n = write(sockfd, buffer, strlen(buffer));
 		   if (n < 0) 
  			 error("\tERROR sokete yazmada.\n");
  			 printf("\t sokete yazma basarii...\n");
  			 

   		out = fopen("out.wav", "wb");
   		while (1) {
  		  n = read(sockfd, buffer, BUFFER_SIZE);
  		  if (n < 0) 
    		    error("ERROR reading from socket");
  		  else if (n == 0) // Socket closed. Transfer is complete (or borked)
     		 	  break;

    			fwrite(buffer, 1, n, out);
	}


	return 0;
}
}
