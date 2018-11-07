/*
 * Creación del servidor TCP/IP
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void err_quit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void xfer_data(int srcfd, int tgtfd)
{
	fflush;
    char *command;
    command = malloc(11*sizeof(char));
    int cnt, len;
	char mn[] = "hoa";
    /* Leer de la entrada fd y escribir en la salida fd*/
    while((cnt = read(srcfd, command, sizeof(command))) > 0)
    {
	if (strcmp(command,mn)== 0){
		printf("el if funciona");
	}
	printf("%s \n",command); 
	/*
	if(len < 0)
	    err_quit("xfer_data:read");
	if((len = write(tgtfd, command, cnt)) != cnt){
	    err_quit("xfer_data:write");
	    printf("hola111: %d",len);
	    }
	 if ((strcmp(buf, "hola") == 0)){
		printf("respuesta");
		printf("%s",buf);
		}*/
    }
}


int main(void)
{
    int sockfd, infd; /* Descriptor del socket */
    struct sockaddr_in srv; /* Estructura del socket en el servidor */
    socklen_t socklen;
    int i = 1; /* Para setsockopt */

    /*Creacion del socket*/
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	err_quit("socket");
    /*Esperar fallo en la asignacion de la direccion local */
    setsockopt(sockfd, SOL_SOCKET, 0, &i, sizeof(i));

    /* Inicializacion y configuracion de la estructura en el servidor*/
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(50000); 

    /* Ligar el socket a la dirección*/
    socklen = sizeof(srv);
    if((bind(sockfd, (struct sockaddr *)&srv, socklen)) < 0)
	err_quit("Ligado");

    /* Esperar conexiones */
    if((listen(sockfd, 5)) < 0)
	err_quit("Escucha de peticiones");
    puts("Conexiones TCP/IP disponible");
    printf("Puerto: %d\n",ntohs(srv.sin_port));
    printf("Direccion Servidor: %s\n",inet_ntoa(srv.sin_addr));

    /*ciclo infinito de aceptacion de conexiones */
    if((infd = accept(sockfd, (struct sockaddr *)&srv, &socklen)) >= 0)
	puts("Nueva conexion aceptada");

    /*Leer desde el socket y escribe en stdout*/
    xfer_data(infd, fileno(stdout));
    
    exit(EXIT_SUCCESS);
}
