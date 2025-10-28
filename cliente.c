#include <stdio.h>
#include <string.h>         //strcmp() - Comparar dos cadenas | strcpy() para copiar dos cadenas
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

struct Credenciales {
    char usuario[50];
    char contrasena[50];
};

//Usuario: admin
//Contraseña: 1234

int main(){
    int sock = 0;
    struct sockaddr_in server;
    char buffer[100] = {0};

    /*-------------------------------------
    |Crear nuestro socket TCP del servidor|
    |AF_INET = IPv4, SOCK_STREAM = TCP    |
    ---------------------------------------
    */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    /*-------------------------------------------------------------
    |Configuramos la direccion del servidor                       |
    |IP y puerto deben coincidir con los del servidor             |    
    ---------------------------------------------------------------
    */
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0){
        perror("Error al asociar la ip con el socket, ip o puerto no valido");
        exit(EXIT_FAILURE);
    }

    /*-------------------------------------------------------------------
    |Conctamos al servidor mediante el metodo connect()                 |
    |connect() intenta establecer una conexion TCP con el servidor      |    
    ---------------------------------------------------------------------
    */
    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("Error al cconectarse al servidor");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Conectado al servidor de autenticacion. \n");

    /*---------------------------------------------------------------------------
    |Solicitar credenciales al usuario                                          |
    |scanf lee hasta 49 caracteres para evitar el desbordamiento de buffer      |    
    -----------------------------------------------------------------------------
    */   
    struct Credenciales cred;
    printf("Ingrese el usuario: ");
    scanf("%49s",cred.usuario);
    printf("Ingrese la contraseña: ");
    scanf("%49s",cred.contrasena);

    /*---------------------------------------------------------------------------
    |Enviamos la estructura completa de credenciales al servidor                |
    |send() transmitir los bytes de la estructura a traves del socket           |    
    -----------------------------------------------------------------------------
    */
    send(sock, &cred, sizeof(cred), 0);
    printf("Credenciales enviadas al servidor... \n");

    /*---------------------------------------------------------------------------
    |Esperamos la respuesta del servidor                                        |
    |read() bloquea hasta recibir datos o que la conexion se cierre             |    
    -----------------------------------------------------------------------------
    */
    int bytes = read(sock, buffer, sizeof(buffer) - 1);
    if(bytes > 0){
        buffer[bytes] = '\0';
        printf("Respuesta del servidor: %s\n",buffer);
    } else{
        perror("Error al leer la respuesta del servidor");
    }

    /*--------------------------------------------------------------------------
    |Cerramos la conexion y liberamos recursos                              |   
    ----------------------------------------------------------------------------
    */   
    close(sock);
    printf("Conexion cerrada, servidor finalizado...\n");

    //Comentario adicional para fines ilustrativos de GitHub

    printf("Linea de codigo para el branch third");

    return 0;
}