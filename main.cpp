//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux  
#include <iostream>
#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  

#define TRUE   1  
#define FALSE  0  
#define PORT 8888  
unsigned long fileLen;

char* readFile(char*fileName) {
	system("ls");
	system("pwd");
    char buf;
    FILE *pr;
    
    pr=fopen(fileName,"rb");

    
    fseek(pr, 0, SEEK_END);
    int fileLen = ftell(pr);
    fseek(pr, 0, SEEK_SET);
    std::cout<<"File open and size found";
    char *buffer=(char*)malloc(sizeof(char)*fileLen);
    
    for(int t = 0; t<fileLen; t++)  buffer[t]=0;
    
    int o = 0;
    
    while(!feof(pr)){
        fread(&buf,1,1,pr);
        buffer[o++]=buf;
    }
    
    //for (int i = 0; i < fileLen; i++)   fwrite(&buffer[i],1,1,pw);
        

    fclose(pr);
    return buffer;
}

void sending(int new_socket, char* fileName = "2.htm") {

	//system("ls"); system("pwd");
	/*
	FILE *f = fopen(fileName, "rb");
	int i = 0;

	char messageBuffer[80000];

	for (int i = 0; i < 80000; i++)
		messageBuffer[i] = NULL;*/
	
	/*char c = getc(f);
	//while (c != '\377') {
	while(i<9950){
		messageBuffer[i++] = c;
		c = getc(f);
	}*/
	/*const char bs[150] = "HTTP/1.1 200 OK\n\n";//Server: 192.168.0.103:8888\nContent-Type: image/jpeg\nContent-Length: 80000\n\n";

	char message[80150];
	for (int i = 0; i < 80150; i++)
		message[i] = NULL;
	strcat(message, bs);
	strcat(message, messageBuffer);*/
	//std::cout << readFile(fileName);
	//Content-Type: image/jpeg
	//send(new_socket, "HTTP/1.1 200 OK\nContent-Type: image/jpeg\nContent-Length: 30300\n\n", strlen("HTTP/1.1 200 OK\nContent-Type: image/jpeg\nContent-Length: 30300\n\n"),0);
	
	
	const char *w = "HTTP/1.1 200 OK\r\n\n";
	//ETag: \"56d-9989200-1132c580\"\r\nContent-Type: text/plain; charset=UTF-8\r\nConnection:keep-alive\r\n\r\n";
	
	FILE *pw;
	char* file_to_send = readFile(fileName);

	pw=fopen("sent.jpg","w");
	for (int i = 0; i < fileLen; i++)   fwrite(&file_to_send[i],1,1,pw);
	fclose(pw);

	
	//send(new_socket, w, sizeof(w), 0);
	
	
	std::cout<<"\n\nHTML OK sent\ntime for file "<<fileName<<std::endl<<std::endl;
	
	
	if (send(new_socket, file_to_send, fileLen, 0) != fileLen)
	{
		perror("send");
	}
	//close(new_socket);
	//fclose(f);
}

void receiveAndReply(int new_socket, char* buffer) {
	read(new_socket, buffer, 1024);
	std::cout << "Received message: ";// << buffer << std::endl;

	for (int i = 0; i < 1024 && buffer[i] != '\n'; i++)
		std::cout << buffer[i];
	//std::cout << buffer;


	if (strstr(buffer,"GET /"))
	{
		//buffer[5];
		int i = 5;

		if (buffer[i]!=' '){
		while (buffer[i] != ' ')
			i++;
		char *fileName = (char*)malloc(i - 5);
		
		for (int n = 0; n < i - 5; n++)
			fileName[n] = buffer[n+5];
		
		sending(new_socket, fileName);//fileName);
		free(fileName);
		}
		else
		sending(new_socket);
	}
	
	




}

void rf() {
	FILE* file = fopen("1327-r.jpg", "r");
	//FILE* copy = fopen("3.tif", "w");

	fseek(file, 0, SEEK_END);
	unsigned long fileLen = ftell(file);
	char* file_data;
	rewind(file);
	file_data = (char*)malloc((fileLen) * sizeof(char));
	if (file_data == NULL) {
		printf("Memory error"); exit(2);
	}
	int num_read = 0;
	char s;
	/*while ((num_read = fread(&s, 1, 1, file))) {
		strncat(file_data, &s, 1);
	}*/

	fread(file_data, fileLen, 1, file);
	//fread(file_data, 1, fileLen, file);
	//read(file,);

	int i = 0;
	/*while ((s = fgetc(file)) != EOF)
		fputc(s, copy);*/
	
		//file_data[i++] = s;
		//fputc(s, file_data);

	//printf("file contents: %s", file_data);
	std::cout << fileLen << " == " << strlen(file_data)<<" == "<<sizeof(file_data);
	fclose(file);
	FILE*file2 = fopen("dog.txt", "wb");
	fwrite(file_data, fileLen, 1, file2);
}


int main(int argc, char *argv[])
{
	//readFile("cat.jpg");
	int opt = TRUE;
	int master_socket, addrlen, new_socket, client_socket[30],
		max_clients = 30, activity, i, valread, sd;
	int max_sd;
	struct sockaddr_in address;

	char buffer[1025];  //data buffer of 1K  

	//set of socket descriptors  
	fd_set readfds;

	//a message  
	//sending();


	//initialise all client_socket[] to 0 so not checked  
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}

	//create a master socket  
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//set master socket to allow multiple connections ,  
	//this is just a good habit, it will work without this  
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	//type of socket created  
	address.sin_family = AF_INET; //address family IPV4
	address.sin_addr.s_addr = INADDR_ANY; //when we don't need to bind socket to a specific IP
	address.sin_port = htons(PORT);

	//bind the socket to localhost port 8888  
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", PORT);

	//try to specify maximum of 3 pending connections for the master socket  
	if (listen(master_socket, 3) < 0) 
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//accept the incoming connection  
	addrlen = sizeof(address);
	puts("Waiting for connections ...");

	while (TRUE)
	{
		//clear the socket set  
		FD_ZERO(&readfds);

		//add master socket to set  
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		//add child sockets to set  
		for (i = 0; i < max_clients; i++)
		{
			//socket descriptor  
			sd = client_socket[i];

			//if valid socket descriptor then add to read list  
			if (sd > 0)
				FD_SET(sd, &readfds);

			//highest file descriptor number, need it for the select function  
			if (sd > max_sd)
				max_sd = sd;
		}

		//wait for an activity on one of the sockets , timeout is NULL ,  
		//so wait indefinitely  
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR))
		{
			printf("select error");
		}

		//If something happened on the master socket ,  
		//then its an incoming connection  
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket,
				(struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}

			receiveAndReply(new_socket,buffer);

			printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs
			(address.sin_port));

			//send new connection greeting message  
			/*if (send(new_socket, message, (int)strlen(message), 0) != strlen(message))
			{
				perror("send");
			}

			puts("Welcome message sent successfully");
			//puts(message);*/
			//add new socket to array of sockets  

			for (i = 0; i < max_clients; i++)
			{
				//if position is empty  
				if (client_socket[i] == 0)
				{
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n", i);

					break;
				}
			}
		}

		//else its some IO operation on some other socket 
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];

			if (FD_ISSET(sd, &readfds))
			{
				//Check if it was for closing , and also read the  
				//incoming message  
				if ((valread = read(sd, buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print  
					getpeername(sd, (struct sockaddr*)&address, \
						(socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n",
						inet_ntoa(address.sin_addr), ntohs(address.sin_port));

					//Close the socket and mark as 0 in list for reuse  
					close(sd);
					client_socket[i] = 0;
				}

				//Echo back the message that came in  
				else
				{
					//set the string terminating NULL byte on the end  
					//of the data read  
					buffer[valread] = '\0';
					send(sd, buffer, strlen(buffer), 0);
				}
			}
		}
	}

	return 0;
}
