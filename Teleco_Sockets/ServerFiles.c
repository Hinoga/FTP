/*
* Author: Pablo Camarillo Ramírez.
* Fecha/Date: 28 de Septiembre de 2010/September 28th 2010.
* Redes de Computadora / Computer networks.
* Description: 	This application contains the server functionality
* 		to send/receive files through C-like sockets by using
*		TCP/UDP packages.
*/
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // hace llamadas al sistema y proporciona funciones de sockets
#include <stdio.h> // entrada y salida
#include <stdlib.h>
#include <string.h> // libreria que permite uso de cadenas 
#include <unistd.h>

/*Definición de constantes*/
#define BUFFSIZE 1024
#define PUERTO		1100
#define ERROR		-1

struct data_struct 
{ 
	char name[50];
	char type[5];
	long int numPacket;
	int size;
	char data[1024];
};

/*Prototipos de función*/
void recibirArchivo(int SocketFD, FILE *file);
void enviarConfirmacion(int SocketFD);
void enviarMD5SUM(int SocketFD);
void getIP(int tipo, char * IP);
/*Recibe la clave de la interfaz que va a manejar:
 * lo : 0
 * wlan: 1
 * eth0: 2
*/
int main(int argc, char *argv[]){
	struct sockaddr_in stSockAddr;
    	struct sockaddr_in clSockAddr;
	FILE *archivo;
	char *direccIP;
	int SocketServerFD;
	int SocketClientFD;
	int clientLen;
	int serverLen;


	


	direccIP = malloc(20);

	/*Verifica que el número de parametros sea el correcto*/
	if(argc < 2){
		perror("Uso ./ServerFiles <clave de interfaz 0:lo 1:wlan 2:eth0>");
		exit(EXIT_FAILURE);	
	}

	/*Se obtiene la IP de la interfaz solicitada*/
	getIP(atoi(argv[1]),direccIP);

	/*Se crea el socket*/
	//PF_INET - Dominio, para aplicaciones que estan en diferentes localidades del sistema
	// Sock_stream usa un flujo de datos que se utiliza en la red
	//IPPROTO_TCP protocolo tcp ip
	if((SocketServerFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == ERROR){
		perror("No se puede crear el socket");
		exit(EXIT_FAILURE);
	}//End if-SocketFD
	
		/*Se configura la dirección del socket*/
	memset(&stSockAddr, 0, sizeof stSockAddr);
	 
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(PUERTO);
	stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
	if(bind(SocketServerFD,(struct sockaddr *)&stSockAddr, sizeof stSockAddr) == ERROR){
		perror("Error en bind");
		close(SocketServerFD);
		exit(EXIT_FAILURE);
	}//End if-bind
	inet_pton(AF_INET, direccIP, &stSockAddr.sin_addr);
	printf("Socket atado a la dirección %s\n",(char *)inet_ntoa(stSockAddr.sin_addr));	

	// se define la cola de clientes en este caso 10
	// liste es una funcion del sistema
	if(listen(SocketServerFD, 10) == ERROR){
		perror("Error listen");
		close(SocketServerFD);
		exit(EXIT_FAILURE);
	}//End if-listen

	// aca deberian de ir los hilos con el codigo que hay de aca para abajo
	while (1){
		clientLen = sizeof(clSockAddr);

		//Espera por la conexión de un cliente//
		if ((SocketClientFD = accept(SocketServerFD, 
						    (struct sockaddr *) &clSockAddr,
						    &clientLen)) < 0){
			perror("Fallo para acpetar la conexión del cliente");
		}//End if-accept

		/*Se configura la dirección del cliente*/
		clSockAddr.sin_family = AF_INET;
		clSockAddr.sin_port = htons(PUERTO);
		printf("Cliente conectado: %s\n",inet_ntoa(clSockAddr.sin_addr));

		/*Se recibe el archivo del cliente*/
		recibirArchivo(SocketClientFD, archivo);
		

	}//End infinity while

 	close(SocketClientFD);
	close(SocketServerFD);
	return 0;
}//End main program



void recibirArchivo(int SocketFD, FILE *file){
	char buffer[BUFFSIZE];
	int buffTmp;
	long int sizeTmp;
	int recibido = -1;

	/*Se abre el archivo para escritura*/
	file = fopen("archivoRecibido","wb");
	enviarConfirmacion(SocketFD);
	enviarMD5SUM(SocketFD);
	sizeTmp = 153031579;
	buffTmp = BUFFSIZE;

	int cont = 0;
	while((recibido = recv(SocketFD, buffer, BUFFSIZE, 0)) > 0){
		cont = cont + 1;
		printf("contador packs : %d\n",cont);
		if(sizeTmp>BUFFSIZE){
			sizeTmp = sizeTmp-BUFFSIZE;
			buffTmp = BUFFSIZE;
			
		} else{
			// sizeTmp = sizeTmp-BUFFSIZE;
			buffTmp = sizeTmp;
		}
		//printf("%s",buffer);
		fwrite(buffer,buffTmp,1,file);
		memset(buffer, 0, BUFFSIZE);
	}//Termina la recepción del archivo

	fclose(file);
	

}//End recibirArchivo procedure

void enviarConfirmacion(int SocketFD){
	char mensaje[80] = "Paquete Recibido";
	int lenMensaje = strlen(mensaje);
	printf("\nConfirmación enviada\n");
	if(write(SocketFD,mensaje,sizeof(mensaje)) == ERROR)
			perror("Error al enviar la confirmación:");

	
}//End enviarConfirmacion

void enviarMD5SUM(int SocketFD){
	FILE *tmp;//Apuntador al archivo temporal que guarda el MD5SUM del archivo.
	char *fileName = "verificacion";
	char md5sum[80];
	system("md5sum archivoRecibido >> verificacion");
	
	tmp = fopen(fileName,"r");
	fscanf(tmp,"%s",md5sum);	
	printf("\nMD5SUM:%s\n",md5sum);	
	write(SocketFD,md5sum,sizeof(md5sum));
	fclose(tmp);

}//End enviarMD5DUM

void getIP(int tipo, char * IP){
	FILE *tmpIP;
	char dIP[20];
	char dIP2[20];
	int i,j;
	switch(tipo){
		case 0:
			system("ifconfig lo | grep inet > tmp");
			break;
		case 1:
			system("ifconfig wlan | grep inet > tmp");
			break;
		case 2:
			system("ifconfig eth | grep inet > tmp");
			break;

	
	}//End switch
	j = 0;
	tmpIP = fopen("tmp","r");
	fscanf(tmpIP,"%s %s",dIP,dIP);
	for(i = 5;i<20;i++){
		IP[j] = dIP[i];	
		j++;		
	}
}//End getIP

