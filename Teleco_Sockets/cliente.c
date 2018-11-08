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
/*
Developer: Hinoga
Description: Funcion que arroja al usuario las posibles opciones
Parameter: none
Return: void
Date: 07/11/2018
*/
void commandHelp(){
	printf("-help, -h, #Display local help information \n");
    printf("-ls, #Lists the content of the directory \n");
    printf("-user, -Required [ Username ] [ Password ],#Send new user information, #Required Your username, Your password \n");
    printf("-get, #Receive file, #Required File that want to get \n");
    printf("-connect, #Connect to remote FTP\n");
    printf("-disconnect #Terminate FTP session \n");
    printf("-exit,#Terminate FTP sessions and exit  \n");	
    readCommand();
}

/*
Developer: DanielToro05 - Hinoga
Description: Funcion para pedir el comando al usuario
Parameter: none
Return: void
Date: 07/11/2018
*/
void readCommand(){
	char command[60];
	printf("Ingrese el comando: "); //Display Hello in red   
    scanf("%s", command); // comando capturado      
    commandMenu(command); // se llama la funcion que decide que comando ejecutar
}

/*
Developer: DanielToro05 - Hinoga
Description: Menu para ejecutar el comando propuesto
Parameter: command
Return: void
Date: 07/11/2018
*/
void commandMenu(char *command) {
	int band=0;
	
	if ((strcmp ("-help", command)==0) || (strcmp ("-h", command)==0)){
		commandHelp();
		band=1;
	}
	else if (strcmp (command,"-ls")==0){
		printf("GG2");
		band=1;
	}
	else if (strcmp ("-disconnect", command)==0){
		printf("GG3");
		band=1;
	}
	else if (strcmp ("-exit", command)==0){
		printf("GG4");
		band=1;
	}
	else if (strcmp ("-connect", command)==0){
		printf("GG5");
		band=1;
	}
	else if (strcmp ("-user", command)==0){
		printf("GG6");
		band=1;
	}
	else if (strcmp ("-get", command)==0){
		printf("GG7");
		band=1;
	}
	else if (strcmp ("-send", command)==0){
		printf("GG8");
		band=1;
	}
	else if(band == 0){
		printf("Command not found\n");
		readCommand();
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


    /*LECTURA DE COMANDOS*/
    readCommand(); // Pide al usuario un comando a ingresar
    



    /* Copiar la stdin al descriptor de socket*/
    //xfer_data(fileno(stdin), sockfd);

    //exit(EXIT_SUCCESS);

}
