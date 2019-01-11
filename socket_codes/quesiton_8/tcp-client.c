#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct   sockaddr_in cad;
     
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1000];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);

 if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    
    //binding the local ip and port to the client - using port 2222
    memset(&cad, 0, sizeof(struct sockaddr_in));
    cad.sin_family = AF_INET;
    cad.sin_port = htons(2466);
    cad.sin_addr.s_addr = inet_addr("192.168.0.12");
    bind(sockfd, (struct sockaddr *)&cad, sizeof(struct sockaddr));
    

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    bzero(buffer,1000);
    
    /* Creating the required APDU */

    //Preamble
    char preamble[10]="NCSU"; 

    // Student ID and payload
    char sid[14] = {50,48,48,50,53,52,57,54,51,'\0'};
    char payload[100] = "\nANKIT NANAVATY\n200254963\n";
    unsigned long sidl = 200254963; // my id
    unsigned char bytes[4];
    bytes[0] = (sidl >> 24) & 0xFF;
    bytes[1] = (sidl >> 16) & 0xFF;
    bytes[2] = (sidl >> 8) & 0xFF;
    bytes[3] = sidl & 0xFF;

    // IP and ports
    char ip[15] = "192.168.0.12\n";
    char port[8];
    sprintf(port,"%d",ntohs(cad.sin_port));
    strcat(port,"\n");
    strcat(buffer,preamble);
    strcat(buffer,sid);
    strcat(buffer,bytes);
    strcat(buffer,payload);
    strcat(buffer,ip);
    strcat(buffer,port);

    int i;
    for(i=0;i<sizeof(buffer);i++){
		if(buffer[i]==NULL)
			buffer[i]=' ';
	}

    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,1000);
    n = read(sockfd,buffer,1000);
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    return 0;
}
