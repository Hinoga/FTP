/*
 * Cliente tipo TCP/IP empleando sockets
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

#define ERROR		-1
#define BUFFSIZE 255

int bandera = 0;   //Variable que se le asigna al connect para saber si está conectado o no.
int autenticacion = 0; //Variable que se le asigna si el usuario ya inicio sesion.
	
void err_quit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
/*
Developer: Danieltoro05
Description: Función que envía el comando al servidor
Parameter: SocketFD, command 
Return: void
Date: 15/11/2018
*/
void sendCommand(int sockfd, char *command){
	int buffTmp = 20; //se define un buff temporal
	char buffer[BUFFSIZE]; //se define buffer con un BUFFSIZE definido globalmente
	
	if(send(sockfd,command,buffTmp,0) == ERROR)// Se envía el comando al servidor
			perror("Error al enviar el comando\n");
			
	printf("Comando enviado\n");
}

/*
Developer: Hinoga - LuisBenitez
Description: Funcion que arroja al usuario las posibles opciones
Parameter: none
Return: void
Date: 07/11/2018
*/
void commandHelp(int sockfd){
	printf("-help, -h, #Display local help information \n");
    printf("-ls, #Lists the content of the directory \n");
    printf("-user, -Required [ Username ] [ Password ],#Send new user information, #Required Your username, Your password \n");
    printf("-get, #Receive file, #Required File that want to get \n");
    printf("-connect, #Connect to remote FTP\n");
    printf("-disconnect #Terminate FTP session \n");
    printf("-exit,#Terminate FTP sessions and exit  \n");	
    readCommand(sockfd);
}

/*
Developer: DanielToro05 - Hinoga - LuisBenitez
Description: Funcion para pedir el comando al usuario
Parameter: none
Return: void
Date: 07/11/2018
*/
void readCommand(int sockfd){
	char command[60];
	printf("Ingrese el comando: ");
    scanf("%s", command); // comando capturado      
    commandMenu(command, sockfd); // se llama la funcion que decide que comando ejecutar
}

/*
Developer: Brayan Tabares, Daniela Ortiz, Andrés García.
Description: Esta función se encarga de desconectar el socket.
version: 0.3
Parameter: none
Return: none
Date: 15/11/2018
*/

void commandDisconnect(int sockfd){ // esta es la función de desconectar, retorna un entero.
	close(sockfd); //cierra el socket.
	printf("User disconnected\n");
    //int val=1; // se crea una variable entera y se le asigna el número 1.
    //return val; // retorna el valor que tenga val.
    
       
    //nota: se puede emplear shutdown, para cerrar el socket.
 }

/*
Developer: Brayan Tabares, Daniela Ortiz, Andrés García.
Description: Esta función se encarga de cerrar el terminar del ftp.
version: 0.3
Parameter: none
Return: none
Date: 15/11/2018
*/

void commandExit(int sockfd){// esta es la función de salidad, no retorna nada.
	close(sockfd); //cierra el socket
	
	exit(EXIT_SUCCESS); //cierra la interfaz
}

/*
Developer: DanielToro05 - Hinoga - LuisBenitez
Description: Menú para ejecutar el comando propuesto
Parameter: command
Return: void
Date: 07/11/2018
*/

void commandMenu(char *command, int sockfd, char *argv[]) { //función que llama las funciones dependiendo del comando escrito por el cliente
	int band=0;
	int recibido = -1;
	char buffer[255];
	
	if ((strcmp ("-help", command)==0) || (strcmp ("-h", command)==0)){
		commandHelp(sockfd);
		band=1;
	}
	else if (strcmp (command,"-ls")==0){
		if(bandera != 0 && autenticacion !=0)
		{
			sendCommand(sockfd, command);
			readCommand(sockfd);
		}else
		{
			printf("User has not connected or logged \n");
			readCommand(sockfd);
		}
		
		/*while(recibido < 0){
			recibido = recv(IdSocket, buffer, BUFFSIZE, 0);
			printf("%s\n", buffer);
			recibido =0;
		}
		recibido = -1;*/
		band=1;
	}
	else if (strcmp ("-disconnect", command)==0){
		if(bandera != 0)
		{
			bandera = 0;
			commandDisconnect(sockfd);
			readCommand(sockfd);
		}else
		{
			printf("User has not connected\n");
			readCommand(sockfd);
		}
		band=1;
	}
	else if (strcmp ("-exit", command)==0){
		commandExit(sockfd);
		band=1;
	}
	
/*
Developer: Erika , Julian, Junior
Description: Connect
Parameter: command
Return: void
Date: 15/11/2018
*/

	else if (strcmp ("-connect", command)==0){
		
		struct sockaddr_in cli;
		socklen_t socklen;
		char argc[20];
		printf("Direccion IP:");
		scanf("%s",argc);
		
		/* Configuracion del cliente */
		memset(&cli, 0, sizeof(cli));
		cli.sin_family = AF_INET;
		cli.sin_port = htons(50000);
	
		/* Esperar la dirección IP en la linea de comando */
		if(argc == "")
		{
		puts("ERROR: cliente <direccion IP>");
		exit(EXIT_FAILURE);
		}

		
		/* Destruir socket cuando la IP es invalida*/
		if(!(inet_aton(argc, &cli.sin_addr)))
		err_quit("inet_aton");

		/*Conectarse al socket*/
		socklen = sizeof(cli);
		if(connect(sockfd, (struct sockaddr *)&cli, socklen))
			err_quit("coexion");
		puts("conectado al servidor");
		printf("\t Puerto: %d\n",ntohs(cli.sin_port));
		printf("\tDirección: %s\n",inet_ntoa(cli.sin_addr));
		bandera = 1;
		
		readCommand(sockfd);
		
		band=1;
	}
	else if (strcmp ("-user", command)==0){
		if(bandera != 0)
		{
			char buffTmp[15];
			
			sendCommand(sockfd, command);
			while(recibido < 0){
				recibido = recv(sockfd, buffer, BUFFSIZE, 0);
				printf("%s", buffer);
			}
			recibido = -1;
			char usuario[10]; // Se declara el arreglo para almacenar el usuario
			scanf("%s",& usuario);
			sendCommand(sockfd, usuario);
			if(recibido < 0){
				recibido = recv(sockfd, buffer, BUFFSIZE, 0);
				printf("%s\n", buffer);
			}
			autenticacion = 1;
			readCommand(sockfd);
		}else
		{
			printf("User has not connected\n");
			readCommand(sockfd);
		}
		
		band=1;
	}
	else if (strcmp ("-get", command)==0){
		
		if(bandera != 0 && autenticacion !=0)
		{
			
		}else
		{
			printf("User has not connected or logged \n");
			readCommand(sockfd);
		}
		band=1;
	}
	else if (strcmp ("-send", command)==0){
		
		if(bandera != 0 && autenticacion !=0)
		{
			
		}else
		{
			printf("User has not connected or logged \n");
			readCommand(sockfd);
		}
		band=1;
	}
	else if(band == 0){
		printf("Command not found\n");
		readCommand(sockfd);
	}
}

int main()
{
	int sockfd;
	/* Creacion del socket */
	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	err_quit("socket");

	
	
    /*LECTURA DE COMANDOS*/
    readCommand(sockfd); // Pide al usuario un comando a ingresar
    
    /* Copiar la stdin al descriptor de socket*/
    //xfer_data(fileno(stdin), sockfd);

    exit(EXIT_SUCCESS);
}
