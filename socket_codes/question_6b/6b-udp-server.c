#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char**argv)
{
	int sockfd,n,on=1,s_port;
   	struct sockaddr_in servaddr,cliaddr;
   	socklen_t len;
   	char mesg[1000];

   	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd == -1)  // socket descriptor cannot be -1
		printf("Error opening the socket!!!");
	else
		printf("Socket descripter: %d...\n",sockfd);
	
	
	memset(&servaddr,0,sizeof(servaddr));
	printf("Memset complete...\n");
	//bzero(&servaddr, sizeof(servaddr));
   	servaddr.sin_family = AF_INET;
   	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	printf("Enter the port number of the server:\n");
	scanf("%d",&s_port);
   	servaddr.sin_port=htons(s_port);
	printf("Server port: %d\n",s_port);
   	
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(char *)&on, sizeof(on));
	printf("Setsock implemented...\n");

	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	if(bind<0)
		printf("Bind failed...\n");
	else
		printf("Bind completed...\n");

   	printf("UDP server started...\n");
   	for (;;)
   	{
		//printf("Entered for loop\n");
      		len = sizeof(cliaddr);
      		n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
		//printf("n:%d",n);
      		sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
      		printf("-------------------------------------------------------\n");
      		mesg[n] = 0;
      		printf("Received the following:\n");
      		printf("%s\n",mesg);
      		printf("-------------------------------------------------------\n");
   	}
}
