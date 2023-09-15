#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//Alunos: Guilherme Matheus Alves e Rodrigo da Silva

int main() {
    // Declaração de variáveis
    struct sockaddr_in target;
    char palavra[10];

    // Inicialização do socket UDP
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Configuração do endereço do servidor
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr("127.0.0.1");
    target.sin_port = htons(9000);

    printf("\nDigite 1 (uma) palavra em minúsculo (sem espaços): ");
    do{
        // Leitura da entrada do usuário
        scanf("%s",palavra);
        
        // Envia a palavra para o servidor
        sendto(sock, palavra, strlen(palavra), 0, (struct sockaddr *)&target, sizeof(target));
        
        if (strcmp(palavra, "exit") != 0){
            // Recebimento da resposta do servidor
            ssize_t bytes_received = recvfrom(sock, palavra, sizeof(palavra), 0, NULL, NULL);
            if (bytes_received == -1) {
                perror("Erro ao receber resposta do servidor");
                break;
            }

            // Imprime a resposta do servidor
            printf("Servidor converteu para: %s", palavra);
            // Verifica se o usuário deseja sair
            printf("\nDigite 'exit' para sair: ");
        }
    }while(strcmp(palavra, "exit") != 0);

    // Fecha o socket
    close(sock);

    return 0;
}
