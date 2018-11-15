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
void sendCommand(int SocketFD, char *command){
	int buffTmp = 20; //se define un buff temporal
	char buffer[BUFFSIZE]; //se define buffer con un BUFFSIZE definido globalmente
	printf("Comando enviado\n");
	if(send(SocketFD,command,buffTmp,0) == ERROR)// Se envía el comando al servidor
			perror("Error al enviar el comando\n");	
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
void readCommand(int SocketFD){
	char command[60];
	printf("Ingrese el comando: ");
    scanf("%s", command); // comando capturado      
    commandMenu(command, SocketFD); // se llama la funcion que decide que comando ejecutar
}

/*
Developer: Brayan Tabares, Daniela Ortiz, Andrés García.
Description: Esta función se encarga de desconectar el socket.
version: 0.3
Parameter: none
Return: none
Date: 15/11/2018
*/

void commandDisconnect(int IdSocket){ // esta es la función de desconectar, retorna un entero.
	close(IdSocket); //cierra el socket.
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

void commandExit(int IdSocket){// esta es la función de salidad, no retorna nada.
	close(IdSocket); //cierra el socket
	
	exit(EXIT_SUCCESS); //cierra la interfaz
}

/*
Developer: DanielToro05 - Hinoga
Description: Menú para ejecutar el comando propuesto
Parameter: command
Return: void
Date: 07/11/2018
*/

void commandMenu(char *command, int IdSocket) { //función que llama las funciones dependiendo del comando escrito por el cliente
	int band=0;
	int recibido = -1;
	char buffer[255];
	
	if ((strcmp ("-help", command)==0) || (strcmp ("-h", command)==0)){
		commandHelp();
		band=1;
	}
	else if (strcmp (command,"-ls")==0){
		sendCommand(IdSocket, command);
		readCommand(IdSocket);
		/*while(recibido < 0){
			recibido = recv(IdSocket, buffer, BUFFSIZE, 0);
			printf("%s\n", buffer);
			recibido =0;
		}
		recibido = -1;*/
		band=1;
	}
	else if (strcmp ("-disconnect", command)==0){
		commandDisconnect(IdSocket);
		readCommand(IdSocket);
		band=1;
	}
	else if (strcmp ("-exit", command)==0){
		commandExit(IdSocket);
		band=1;
	}
	else if (strcmp ("-connect", command)==0){
		printf("GG5");
		band=1;
	}
	else if (strcmp ("-user", command)==0){
		char buffTmp[15];
		sendCommand(IdSocket, command);
		while(recibido < 0){
			recibido = recv(IdSocket, buffer, BUFFSIZE, 0);
			printf("%s", buffer);
		}
		recibido = -1;
		char usuario[10]; // Se declara el arreglo para almacenar el usuario
		scanf("%s",& usuario);
		sendCommand(IdSocket, usuario);
		if(recibido < 0){
			recibido = recv(IdSocket, buffer, BUFFSIZE, 0);
			printf("%s\n", buffer);
		}
		readCommand(IdSocket);
		//commandUser();
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
		readCommand(IdSocket);
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
    readCommand(sockfd); // Pide al usuario un comando a ingresar

    /* Copiar la stdin al descriptor de socket*/
    //xfer_data(fileno(stdin), sockfd);

    exit(EXIT_SUCCESS);

}
