/*
 * Creación del servidor TCP/IP
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#define BUFFSIZE 1024
#define ERROR		-1
#define LINE_SIZE 100

void err_quit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void xfer_data(int srcfd, int tgtfd, int sockfd)
{	
	char buf[80];
	char command1[80] = "-ls"; //se predefine el comando -ls
	char command2[80] = "-user"; //se predefine el comando -user
	char user2[15] = "Connected\n"; //se predefine el comando a enviar
	int cnt, len;
	int buffTmp = 255;
	

    // Leer de la entrada fd y escribir en la salida fd
    while((cnt = read(srcfd, buf, sizeof(buf))) > 0){
		if (strcmp(buf,command1)== 0){
			printf("el if funciona\n");	
			system("ls > temporal.txt");
				//if(send(srcfd,fileBuffer,LINE_SIZE,0) == ERROR)
					//perror("Error al enviar el comando\n");	
		}
		else if (strcmp(buf,command2)==0){ //Compara si el comando enviado es igual a-user
			char user [10] = "daniel"; // se predefine un usuario
			char sendcommand[15] = "Type user:"; //string que se envia al cliente
			if(send(srcfd,sendcommand,buffTmp,0) == ERROR) //envía la string al cliente mediante send
					perror("Error al enviar bienvenida usuario\n");	 //si hay algun error, informa
			while((cnt = read(srcfd, buf, sizeof(buf))) > 0){ //lee lo que manda el cliente
				if (strcmp(buf,user)== 0) { //compara la informacion del cliente a conectar con el usuario predefinido 
					if(send(srcfd,user2,buffTmp,0) == ERROR){ //Envia al cliente la informacion de que se conectó
						perror("Error al enviar bienvenida usuario\n");	//si hay algun error, informa
					}
				}
			}
		}
	}
}

recibirComando(int SocketFD, char *command){
	char buffer[BUFFSIZE];
	int buffTmp;
	int recibido = -1;
	printf("hey");
	while ((recibido = recv(SocketFD, buffer, BUFFSIZE, 0)) > 0){
		printf("%s",buffer);
	}
}
int main(void)
{
    int sockfd, infd; /* Descriptor del socket */
    struct sockaddr_in srv; /* Estructura del socket en el servidor */
    socklen_t socklen;
    int i = 1; /* Para setsockopt */
    char command[15];

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
	err_quit("Ligados");

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
    xfer_data(infd, fileno(stdout), sockfd);
    //recibirComando(sockfd, command);
    //exit(EXIT_SUCCESS);

}
