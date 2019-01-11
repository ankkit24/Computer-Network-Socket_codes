#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define _OPEN_SYS_ITOA_EXT
#include <stdlib.h>

main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cad;
   //char ip[20];
   char recvline[1000];

   if (argc != 2)
   {
      printf("usage:  udp-client <IP address>\n");
      exit(1);
   }

   // Creating the socket
   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   // Binding the client IP and port 
   memset(&cad, 0, sizeof(struct sockaddr_in));
   cad.sin_family = AF_INET;
   cad.sin_port = htons(2466);
   cad.sin_addr.s_addr = inet_addr("192.168.0.123");
   bind(sockfd, (struct sockaddr *)&cad, sizeof(struct sockaddr));

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   servaddr.sin_port=htons(8222);

   /* Creating the APDU in the desired format */
   //Preamble
   char sendline[1000] = "NCSU";

   //Student id;
   char sid[14] = {50,48,48,50,53,52,57,54,51,'\0'};
   strcat(sendline,sid);

   // Student id in 4 Byte format
   unsigned long sidl = 200254963; // my id
   unsigned char bytes[4];
   bytes[0] = (sidl >> 24) & 0xFF;
   bytes[1] = (sidl >> 16) & 0xFF;
   bytes[2] = (sidl >> 8) & 0xFF;
   bytes[3] = sidl & 0xFF;
   strcat(sendline,bytes);
    
   // IP and port
   char ip[15] = "192.168.0.123\n";
   char port[8]; 
   sprintf(port,"%d",ntohs(cad.sin_port));
   strcat(port,"\n");
   //int term = 0;
   //sprintf(term,"%d",0);

   //Payload
   char payload[100] = "\nANKIT NANAVATY\n200254963\n";
   strcat(sendline,payload);
   strcat(sendline,ip);
   strcat(sendline,port);
   //strcat(sendline,term);
   int i=0;
   for(i=0;i<1000;i++){
	if(sendline[i] == NULL){
		sendline[i] = 0;
		break;
	}
   } 

   // Send the PDU to the server
   sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
	
   // Receiving the response   
   n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
   recvline[n]=0;
   fputs(recvline,stdout);

}
