#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "digest.hpp"
#define SERV_TCP_PORT 1111
#define SERV_ADDR "127.0.0.1"

int main()
{
	int s1, s2, x;
	struct sockaddr_in serv_addr, cli_addr;
	char buf[256];
	size_t xx;
	printf("Hi, I am the server\n");
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(SERV_ADDR);
	serv_addr.sin_port = htons(SERV_TCP_PORT);
	if ((s1 = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket creation error\n");
		exit(1);
	}
	printf("socket created successfully. socket num is %d\n", s1);
	x = bind(s1, (struct sockaddr*)&serv_addr, sizeof(serv_addr));


	if (x < 0)
	{
		printf("binding failed\n");
		exit(1);
	}
	printf("binding passed\n");
	listen(s1, 5);
	xx = sizeof(cli_addr);
	s2 = accept(s1, (struct sockaddr*)&cli_addr, (socklen_t*)&xx);
	printf("we passed accept. new socket num is %d\n", s2);
	read(s2, buf, 10);
	buf[256] = 0;
	printf("we got request for %s from cli\n", buf);

	printf("[+] I have to send this message, hmac to cli\n");

	char msg[] = "hi there";

	std::string key = "secret";
	std::string data = msg;
	digest::HMAC<digest::SHA256> hmac(key);
	std::string hmacvalue = hmac.add(data).hexdigest();

	
	printf("[+] Send my message, hmac to cli !\n");
	write(s2, msg, 9);
	write(s2, hmacvalue.c_str(), strlen(hmacvalue.c_str()));
	exit(0);

	return 0;
}