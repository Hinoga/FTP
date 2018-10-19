/*
 * Cliente tipo TCP/IP empleando sockets
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

void err_quit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void xfer_data(int srcfd, int tgtfd)
{
    char buf[1024];
    int cnt, len;

    /* Leemos desde la entrada fd y escribimos a la salida fd*/
    while((cnt = read(srcfd, buf, sizeof(buf))) > 0)
    {
	if(len < 0)
	    err_quit("xfer_data:read");
	if((len=write(tgtfd, buf, cnt)) != cnt)
	    err_quit("xfer_data:write");
    }
}



int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in cli;
    socklen_t socklen;

    /* Esperar la dirección IP en la linea de comando */
    if(argc != 2)
    {
	puts("ERROR: cliente <direccion IP>");
	exit(EXIT_FAILURE);
    }
    
    /* Creacion del socket */    
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	err_quit("socket");

    /* Configuracion del cliente */
    memset(&cli, 0, sizeof(cli));
    cli.sin_family = AF_INET;
    cli.sin_port = htons(50000);
    /* Destruir socket cuando la IP es invalida*/
    if(!(inet_aton(argv[1], &cli.sin_addr)))
	err_quit("inet_aton");

    /*Conectarse al socket*/
    socklen = sizeof(cli);
    if(connect(sockfd, (struct sockaddr *)&cli, socklen))
        err_quit("coexion");
    puts("conectado al servidor");
    printf("\t Puerto: %d\n",ntohs(cli.sin_port));
    printf("\tDirección: %s\n",inet_ntoa(cli.sin_addr));

    /* Copiar la stdin al descriptor de socket*/
    xfer_data(fileno(stdin), sockfd);

    exit(EXIT_SUCCESS);

}
