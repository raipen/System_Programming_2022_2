/* timeclnt.c - a client for timeserv.c
*               usage: timeclnt hostname portnumber
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define oops(msg) {perror(msg); exit(1);}

int main(int ac, char *av[])
{
    struct sockaddr_in servadd; // the number to call
    struct hostent *hp; // used to get number
    int sock_id, sock_fd; // the socket and fd
    char message[BUFSIZ]; // to receive message
    int messlen; // store its length

    // step 1: get a socket
    sock_id = socket(AF_INET, SOCK_STREAM, 0); // get a line
    if (sock_id == -1)
        oops("socket");

    // step 2: connect to server
    bzero(&servadd, sizeof(servadd)); // zero the address
    hp = gethostbyname(av[1]); // lookup host's ip #
    if (hp == NULL)
        oops(av[1]); // if not found
    bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
    servadd.sin_port = htons(atoi(av[2])); // fill in port number
    servadd.sin_family = AF_INET; // fill in socket type
    if (connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
        oops("connect");

    // step 3: transfer data from server, then hangup
    messlen = read(sock_id, message, BUFSIZ); // read from socket
    if (messlen == -1)
        oops("read");
    if (write(1, message, messlen) != messlen) // write to stdout
        oops("write");
    close(sock_id);

    return 0;
}