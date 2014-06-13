/*
This file is part of Echo-Server.

Copyright (c) 2014 Paul Ferguson

Echo-Server is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

Echo-Server is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
Echo-Server.  If not, see <http://www.gnu.org/licenses/>.
*/

/*Echo Server By Paul Ferguson
* Simultaneously runs three echo servers:
* -Single line TCP via Telnet 127.0.0.1 7777
* -Single line UDP via ncat 127.0.0.1 7777 -u
* -Null terminated TCP via Telnet 127.0.0.1 7778 (NOT WORKING)
* Uses Fork() to separate instances.
*/


#include <strings.h>	/*Handles bzero*/
#include <errno.h>	/*Handles errno output*/
#include <stdio.h>	/*Handles Input/Output*/
#include <unistd.h>	/*Symbolic constants*/
#include <stdlib.h>	/*Standard Library*/
#include <netdb.h>	/*Defines Network Information*/
#include <sys/types.h> 	/*Handles data types*/
#include <sys/socket.h>	/*Handles Socket use*/
#include <netinet/in.h>	/*Defines Internet Protocols*/
#include <arpa/inet.h>	/*Defines IP Addresses*/

#define MY_PORT		7777
#define MAXBUF		65536

/************************Socket Creation************************/
/*---------Creates sockets based on protocol given---------*/

int sockIn(int Sock){
if(Sock == 1){ /*Protocol TCP = 2*/
 if ( (Sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{	perror("Socket TCP Error");
		exit(errno);
	}
}else if (Sock == 2){ /*Protocol UDP = 2*/
 if ( (Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 )
	{	perror("Socket UDP Error");
		exit(errno);
	}

}else{ 
perror("unrecognized socket protocol");
		exit(errno);
}
return Sock;}


/************************TCP Functions************************/

/*---------Binds TCP---------*/
int binderT(struct sockaddr_in echoSock, int sockBind, int upPort){
	
	int newPort = MY_PORT + upPort;/*Used for 7778 on */
	bzero(&echoSock, sizeof(echoSock));
	echoSock.sin_family = AF_INET;
	echoSock.sin_port = htons(newPort);
	echoSock.sin_addr.s_addr = INADDR_ANY;

	if ( bind(sockBind, (struct sockaddr*)&echoSock, sizeof(echoSock)) != 0 ){ return -1;
	}
return 0;
}



/*---------listens for TCP connections---------*/
int listener(int sockBind){
	if ( listen(sockBind, 20) != 0 ){return -1; 
	}
return 0;
}
/*---------Performs the Server for TCP---------*/
int TCPServ(int sockintT){
/*---------TCP Client Declarations---------*/
	char bufferT[MAXBUF];
	int clientSockTCP;
	struct sockaddr_in TCP_addr;
	socklen_t TCPAddyLen=sizeof(TCP_addr);

/*---------TCP Accept---------*/
	clientSockTCP = accept(sockintT, (struct sockaddr *) &TCP_addr, &TCPAddyLen);
	printf("TCP: %s:%d connection found\n", inet_ntoa(TCP_addr.sin_addr), ntohs(TCP_addr.sin_port));
		
/*---------TCP Catch and Release---------*/
	send(clientSockTCP, bufferT, recv(clientSockTCP, bufferT, MAXBUF, 0), 0);
/*---------TCP Close Connection---------*/
	close(clientSockTCP);
return 0;
}

/*---------Performs the Server for Null Terminated TCP---------*/
int NTCPServ(int sockintNT){
/*---------Null TCP Client Declarations---------*/
	char bufferNT[MAXBUF];
	int clientSockNTCP;
	struct sockaddr_in NTCP_addr;
	socklen_t NTCPAddyLen=sizeof(NTCP_addr);

/*---------Null TCP Accept---------*/
	clientSockNTCP = accept(sockintNT, (struct sockaddr *) &NTCP_addr, &NTCPAddyLen);
	printf("Null TCP: %s:%d connection found\n", inet_ntoa(NTCP_addr.sin_addr), ntohs(NTCP_addr.sin_port));
		
/*---------Null TCP Catch and Release---------*/
	send(clientSockNTCP, bufferNT, recv(clientSockNTCP, bufferNT, MAXBUF, 0), 0);
/*---------Null TCP Close Connection---------*/
	close(clientSockNTCP);
return 0;
}

/********************UDP Functions************************/
/*---------Binds UDP---------*/
int binderU(struct sockaddr_in echoSock, int sockBind, int upPort){
	
	int newPort = MY_PORT + upPort;/*Used for 7778 on */
	bzero(&echoSock, sizeof(echoSock));
	echoSock.sin_family = AF_INET;
	echoSock.sin_port = htons(newPort);
	echoSock.sin_addr.s_addr = htonl(INADDR_ANY);

	if ( bind(sockBind, (struct sockaddr*)&echoSock, sizeof(echoSock)) != 0 ){ return -1;
	}
return 0;
}

/*---------Performs the Server for UDP---------*/
int UDPServ(int sockintU){
/*---------UDP Client Declarations---------*/
	char bufferU[MAXBUF];
	int MSG;
	int clientSockUDP = sockintU;
	struct sockaddr_in UDP_addr;
	socklen_t UDPAddyLen=sizeof(UDP_addr);
/*---------UDP Recieve---------*/
	MSG = recvfrom(clientSockUDP, bufferU, MAXBUF, 0,(struct sockaddr *) &UDP_addr, &UDPAddyLen);
		
/*---------UDP Report---------*/
	if ( MSG > 0 ){
printf("Packet Found: %s:%d\n", inet_ntoa(UDP_addr.sin_addr), ntohs(UDP_addr.sin_port));
		
/*---------UDP Send---------*/
	sendto(clientSockUDP, bufferU, MSG, 0,(struct sockaddr *) &UDP_addr, UDPAddyLen);
}		
return 0;
}

/************************Begin Main Program************************/
int main()
{   	
	
	/*Shared Declarations*/
	int portUp = 1; /*Increases port number by 1*/
	/* TCP Declarations */
	int sockintTCP = sockIn(1);
	struct sockaddr_in echoSockTCP;
	binderT(echoSockTCP, sockintTCP, 0);
	listener(sockintTCP);

	/* NULL Terminated TCP Declarations (NTCP)*/
	int sockintNTCP = sockIn(1);
	struct sockaddr_in echoSockNTCP;
	binderT(echoSockNTCP, sockintNTCP, portUp);
	listener(sockintNTCP);

	/* UDP Declarations */
	int sockintUDP = sockIn(2);
	struct sockaddr_in echoSockUDP;
	binderU(echoSockUDP, sockintUDP, 0);


	int child = fork();
	int grandChild = 0;
	/* This fork continuously calls the TCP Server*/
	if (child == 0){	
		while (1)
		{	
			TCPServ(sockintTCP);
		}
	} else 
		grandChild = fork();
	/* This fork continuously calls the Null Terminated TCP Server*/
		if (grandChild == 0) {
		while (1)
		{	
			NTCPServ(sockintNTCP);
		}
	} else {
	/* This fork continuously calls the UDP Server*/
		while (1)
		{	
			UDPServ(sockintUDP);
		}
	}
	return 0; 
}
