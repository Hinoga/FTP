/*
* Author: Pablo Camarillo Ramírez.
* Fecha/Date: 28 de Septiembre de 2010/September 28th 2010.
* Redes de Computadora / Computer networks.
* Description: 	This application contains the client functionality
* 		to send/receive files through C-like sockets by using
*		TCP/UDP packages.
*/
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 1024
#define	ERROR	-1
#define	PUERTO	1100

struct data_struct 
{ 
	char name[50];
	char type[5];
	long int numPacket;
	int size;
	char data[1024];
};
// ----------------------- Se definen las funciones a usar --------------------------
char * getName (char* name);
long int findSize(FILE* fp);
void sendData(int SocketFD, FILE* archivo);

// ------------------------ Main del algoritmo ---------------------------------------
int main(int argc, char *argv[]){
	struct sockaddr_in stSockAddr;
	int sockLen;
	int Res;
	int SocketFD;
	int recibido;
	char buffer[BUFFSIZE];
	char mensaje[80];
	int totalBytesRcvd;
	int bytesRcvd;
	FILE *archivo;
	
	//data_struct data;
	if(argc < 3){
		perror("USO:./clientFiles <archivo a enviar> <direccionIP del servidor>");
		exit(EXIT_FAILURE);
	}


	/*Se abre el archivo a enviar*/
	archivo = fopen(argv[1],"rb");

	/*Se crea el socket*/
	SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	/*Se verifica la integridad del archivo*/
	if(!archivo){
		perror("Error al abrir el archivo:");
		exit(EXIT_FAILURE);
	}
    
	/*Se verifica la integridad del socket*/
	if (SocketFD == ERROR){
		perror("cannot create socket");
		exit(EXIT_FAILURE);
	}

	/*Se configura la dirección del socket del cliente*/
	memset(&stSockAddr, 0, sizeof stSockAddr);
 
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(1100);
	Res = inet_pton(AF_INET, argv[2], &stSockAddr.sin_addr);
 
	sockLen = sizeof(stSockAddr);

	if (0 > Res){
		perror("error: El primer paránmetro no es una familia de direcciónes");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}else if (Res == 0){
		perror("char string (El segundo parmetro no contiene una dirección IP válida");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}

	if (connect(SocketFD, (struct sockaddr *)&stSockAddr, sizeof stSockAddr) == ERROR){
		perror("Error a la hora de conectarse con el cliente");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}

	printf("Se ha conectado con el servidor:%s\n",(char *)inet_ntoa(stSockAddr.sin_addr));

	
	sendData(SocketFD,archivo);

	
	

	return 0;
}//End main

long int findSize(FILE* fp) {   
    fseek(fp, 0L, SEEK_END); 
    long int res = ftell(fp); 
    return res; 
} 


char * getName (char* name){

   const char ch = '/';
   char *ret;
   ret = strrchr(name, ch);
   return ret;
}

void sendData(int SocketFD, FILE* archivo){
	char buffer[BUFFSIZE];
	long int fileSize1;
	long int prueba;
	int buffTmp;
	long int sizeTmp;
	int cont = 0;
	fileSize1 = findSize(archivo);
	rewind(archivo);
	printf("Tamano: %d\n",fileSize1);
	printf("Ubicacion: %d\n",ftell(archivo));
	sizeTmp = fileSize1;
	prueba = fileSize1;
	buffTmp= BUFFSIZE;

	while(!feof(archivo)){
		cont = cont +1;
		printf("Contador: %d\n",cont);
		fread(buffer,buffTmp,1,archivo);//(fileSize/BUFFSIZE),archivo);
		if(sizeTmp>BUFFSIZE){
			sizeTmp = sizeTmp-BUFFSIZE;
			buffTmp = BUFFSIZE;
			prueba = prueba - buffTmp;
			printf("Tamano 1: %d\n",prueba);
		} else {
			//sizeTmp = sizeTmp-BUFFSIZE;
			buffTmp = sizeTmp;
			prueba = prueba - buffTmp;
			printf("Tamano 2: %d\n",prueba);
		}
		if(send(SocketFD,buffer,buffTmp,0) == ERROR)
			perror("Error al enviar el archivo:");

		memset(buffer, 0, BUFFSIZE);
		/*if(packetPerFile>0)
			packetPerFile =packetPerFile+1;
		*/
		//data.numPacket=1;
		//data.size = BUFFSIZE;
		//data.data = buffer;
		//printf("%s",buffer);
	}


	printf("finalizo papus: %d\n",prueba);
	/*read(SocketFD,mensaje,sizeof(mensaje));
	printf("\nConfirmación recibida:\n%s\n",mensaje);
	
	read(SocketFD,mensaje,sizeof(mensaje));
	printf("\nMD5SUM:\n%s\n",mensaje);*/
	
	fclose(archivo);
	close(SocketFD);
	/*
	char mensaje[80] = "Paquete Recibido";
	int lenMensaje = strlen(mensaje);
	printf("\nConfirmación enviada\n");
	if(write(SocketFD,mensaje,sizeof(mensaje)) == ERROR)
			perror("Error al enviar la confirmación:");
	*/
	
}