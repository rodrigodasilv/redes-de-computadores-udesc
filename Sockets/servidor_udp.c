#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

//Alunos: Guilherme Matheus Alves e Rodrigo da Silva

int main() {
    // Definição das estruturas do cliente e do servidor
    struct sockaddr_in me, from;
    
    // Criação do socket UDP
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Erro ao criar o socket");
        exit(1);
    }

    // Declaração das variáveis
    int tam, tam2 = 0;
    socklen_t addr_len = sizeof(from);
    char palavra[10];

    // Inicialização da estrutura do servidor
    //memset(&me, 0, sizeof(me)); Verificar necessidade
    me.sin_family = AF_INET;
    me.sin_addr.s_addr = htonl(INADDR_ANY);
    me.sin_port = htons(9000);

    // Associação do socket à porta
    if (bind(sock, (struct sockaddr *)&me, sizeof(me)) == -1) {
        perror("Porta Ocupada!");
        close(sock);
        exit(1);
    }

    do{
        // Recebimento de dados do cliente
        ssize_t bytes_received = recvfrom(sock, palavra, sizeof(palavra), 0, (struct sockaddr *)&from, &addr_len);
        if (bytes_received == -1) {
            perror("Erro ao receber dados do cliente");
            continue;
        }

        palavra[bytes_received] = '\0';

        if (strcmp(palavra, "exit") != 0) {
            tam = strlen(palavra);
            for (int i = 0; i < tam; i++) {
                if ((palavra[i] >= 'a') && (palavra[i] <= 'z')) {
                    palavra[i] = toupper(palavra[i]);
                }
            }
            tam2 = strlen(palavra);

            // Envio de dados para o cliente
            sendto(sock, palavra, tam2, 0, (struct sockaddr *)&from, sizeof(from));
            
            // Obtenção do IP e do número de porta do cliente
            char *from_ip = inet_ntoa(from.sin_addr);
            int from_port = ntohs(from.sin_port);
            printf("Cliente IP: %s, Porta: %d\n", from_ip, from_port);

            printf("Conversão Realizada\n");
        } else {
            printf("Fechando socket\n");
            close(sock);
            break;
        }
    }while(strcmp(palavra, "exit") != 0);

    return 0;
}
