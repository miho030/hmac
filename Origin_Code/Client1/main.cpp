#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "digest.hpp"
#define SERV_TCP_PORT 1111
#define SERV_ADDR "127.0.0.1"


int main()
{
	int x;
	struct sockaddr_in serv_addr;
	char buf[256];
	printf("Hi, I am the client\n");
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = PF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(SERV_ADDR);
	serv_addr.sin_port = htons(SERV_TCP_PORT);


	if ((x = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket creation error\n");
		exit(1);
	}
	printf(" socket opened successfully. socket num is %d\n", x);
	if (connect(x, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("can't connect to the server\n");
		exit(1);
	}
	printf("now i am connected to the erver. enter a string to send\n");
	scanf("%s", buf);


	write(x, buf, strlen(buf));
	printf("now let's read from the server\n\n");



	char msgbuf[9];
	char hmacbuf[256];

	read(x, msgbuf, 9);
	read(x, hmacbuf, 256);

	std::string key = "secret";
	std::string data = msgbuf;
	digest::HMAC<digest::SHA256> hmac(key);
	std::string verifymsg = hmac.add(data).hexdigest();

	if (verifymsg == hmacbuf)
	{
		printf("[!] Hey, this message has been Verification!\n");
		printf("[*] Message is : %s\n", msgbuf);
	}
	else
	{
		printf("[-] Unsafe message!\n");
		exit(0);
	}

	return 0;
}