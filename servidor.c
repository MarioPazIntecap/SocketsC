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
int main() {
    int server_fd, client_fd;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);
    struct Credenciales cred;
    char respuesta[128];

    /*-------------------------------------
    |Crear nuestro socket TCP del servidor|
    |AF_INET = IPv4, SOCK_STREAM = TCP    |
    ---------------------------------------
    */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1){
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    /*--------------------------------------------------------------------
    |Asignamos direccion e IP al socket del servidor (bind)              |
    |INADDR_ANY: Acepta conexiones desde cualquier interfaz de red local |    
    ----------------------------------------------------------------------
    */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    if(bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("Error al asociar la ip con el socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    /*---------------------------------------------------------------------------
    |Ponemos el servidior en modo escucha                                       |
    |listen(socket,backlog) - Define cuantas conexiones pendientes puede aceptar|    
    -----------------------------------------------------------------------------
    */
    if(listen(server_fd ,3) < 0){
        perror("Error al intentar colocar el socket en modo escucha");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor de autenticacion esperando en el puerto 8080...\n");

    /*---------------------------------------------------------------------------------
    |Aceptamos una conexion entrante                                                  |
    |accept() Retornar un nuevo descriptor de socket para comunicarnos con ese cliente|    
    -----------------------------------------------------------------------------------
    */
    client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);
    if(client_fd < 0){
        perror("Error al aceptar una conexion");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    /*-----------------------------------------------------------------------
    |Recibimos las credenciales enviadas por el cliente                     |
    |recv() bloqueara hasta recibir los bytes esperados o cerrar la conexion|    
    -------------------------------------------------------------------------
    */
    recv(client_fd, &cred, sizeof(cred),0);
    printf("Usuario recibido: %s\n",cred.usuario);
    printf("Contraseña recibido: %s\n",cred.contrasena);

    /*--------------------------------------------------------------------------
    |Validamos las credenciales.                                               |
    |Tenemos que usar datos fijos o un archivo de texto/binario para simular BD|    
    ----------------------------------------------------------------------------
    */
    if(strcmp(cred.usuario, "admin") == 0 && strcmp(cred.contrasena, "1234") == 0){
        strcpy(respuesta, "Acceso concecido\n");
        printf("Credenciales correctas\n");
    } else{
        strcpy(respuesta, "Acceso denegado\n");
        printf("Credenciales incorrectas\n");
    }

    /*--------------------------------------------------------------------------
    |Enviamos respuesta al cliente                                             |
    |Se envia una cadena simple terminando en "\0"                             |    
    ----------------------------------------------------------------------------
    */
    send(client_fd, respuesta, strlen(respuesta) + 1, 0);
    printf("Respuesta enviada al cliente...\n");

    /*--------------------------------------------------------------------------
    |Cerramos las conexiones y liberamos recursos                              |   
    ----------------------------------------------------------------------------
    */   
    close(client_fd);
    close(server_fd);
    printf("Conexion cerrada, servidor finalizado...\n");

    return 0;
}