#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

#define CONTROL_PORT "21"
#define DATA_PORT "20"

const char* const ftpServer = "ftp.bom.gov.au";
const char* const ftpPath = "/anon/gen";
//error
const char* const socketError = "Unable to create socket.\n";
const char* const imageryError = "Latest radar imagery unavailable.\n";
const char* const controlError = "Control port unavailable\n";
const char* const dataError = "Data port unavailable\n";
const char* const addrError = "getaddrinfo resulted in an error - %s\n";
//exit codes
typedef enum {
	EXIT_SOCKET = 6,
	EXIT_IMAGERY = 51,
	EXIT_GETADDRINFO = 21
} ExitCodes;

int ftp(void)
{
	struct addrinfo hints;
	struct addrinfo* results;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	int status = getaddrinfo(ftpServer, CONTROL_PORT, &hints, &results);
	if (status != 0) {
		fprintf(stderr, addrError, gai_strerror(status));
		exit(EXIT_GETADDRINFO);
	}

	int controlSocket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	if (controlSocket < 0) {
		fprintf(stderr, "%s", socketError);
		exit(EXIT_SOCKET);
	}

	if (connect(controlSocket, results->ai_addr, results->ai_addrlen) < 0) {
		//connection error
		freeaddrinfo(results);
	}

	freeaddrinfo(results);
	return controlSocket;
}
