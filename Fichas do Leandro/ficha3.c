#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

int valorFilhoC, valorFilhoA, valorFilhoB, valorPai;

//sinal SIGUSR1 para o processo pai, vindo do A 
void sigusr1_handler_pai(int signum) 
{
    printf("sigusr1_handler_pai\n\n");
    printf("PROCESSO PAI (PID %d): Sinal SIGUSR1 recebido.\n", getpid());
    //printf("Sinal SIGUSR2 será enviado para o processo filho B (PID %d) para apresentação do conteúdo dos ficheiros gerados.\n", getppid);
    printf("Pressione uma tecla para continuar...\n");
    getchar();
    fflush(stdin);
    getchar(); // Aguarda a entrada do usuário antes de continuar
}

// Manipulador de sinal SIGUSR2 para o processo pai
void sigusr2_handler_pai(int signum)
{   
    system("clear");
    printf("\nsigusr2_handler_pai\n");
    printf("PROCESSO PAI (PID %d): Sinal SIGUSR2 recebido.\n", valorPai);
    printf("Sinal vindo do processo filho B (PID %d).\n", valorFilhoB);
    printf("Sinal SIGTERM será de seguida enviado ao processo filho C (PID %d) para geração do histograma.\n", valorFilhoC);
    printf("PROCESSO PAI (PID %d): Sinal SIGTERM enviado ao processo filho C (PID %d)", valorPai, valorFilhoC);
    printf("Pressione Enter para continuar...\n");

    kill(getppid(), SIGTERM);

    getchar();
    fflush(stdin);
    getchar();
    system("clear");
}

// Manipulador de sinal SIGTERM para o processo pai
void sigterm_handler_pai(int signum)
{
    printf("sigterm_handler_pai\n\n");
    printf("PROCESSO PAI (PID %d): Sinal SIGTERM recebido do processo filho C (PID %d).\n", getpid(), getppid);
    printf("Retornando para o submenu da ficha 3.\n");
    printf("Pressione uma tecla para continuar...\n");
    getchar();
    fflush(stdin);
    getchar(); // Aguarda a entrada do usuário antes de continuar
}

void sigusr1_handler_filho1(int signum) // A --> PAI
{
    printf("PROCESSO FILHO A (PID %d): Sinal SIGUSR1 recebido.\n", getpid());

    char nomeFicheiro[100];
    char linhaCabecalho;
    char nomeColuna[20];
    char buffer[1024];
    FILE *fp;
    char *token;
    int numeroColuna = -1;

    int i = 0;
    int n = 0;
    double valor = 0;

    double soma = 0;
    double somaQuadrados = 0;

    printf("Digite o nome do ficheiro a abrir: ");
    scanf("%s", nomeFicheiro);

    printf("Existe uma linha cabeçalho no ficheiro? (s/n): ");
    scanf(" %c", &linhaCabecalho);

    printf("Indique o nome da coluna: ");
    scanf("%s", nomeColuna);

    fp = fopen(nomeFicheiro, "r");

    if(fp == NULL)
    {
        printf("O ficheiro %s não existe.\n", nomeFicheiro);
        return;
    }

    fgets(buffer, 1024, fp); // Lê a primeira linha (cabeçalho ou não)

    token = strtok(buffer, ",");
    while(token != NULL)
    {
        if(strcmp(token, nomeColuna) == 0)
        {
            numeroColuna = i;
            break;
        }
        i++;
        token = strtok(NULL, ",");
    }

    if (numeroColuna == -1)
    {
        printf("A coluna %s não existe\n", nomeColuna);
        fclose(fp);
        return;
    }

    if(linhaCabecalho == 's' || linhaCabecalho == 'S')
    {
        fgets(buffer, 1024, fp); // Descarta a linha do cabeçalho
    }

    while(fgets(buffer, 1024, fp))
    {
        token = strtok(buffer, ",");
        i = 0;
        while(token != NULL)
        {
            if(i == numeroColuna)
            {
                valor = atof(token); // Converte a string para double
                n++;
                soma += valor;
                somaQuadrados += valor * valor;
                break;
            }
            token = strtok(NULL, ",");
            i++;
        }
    }

    fclose(fp);

    if(n > 0)
    {
        double media = soma / n;
        double variancia = (somaQuadrados / n) - (media * media);
        double desvioPadrao = sqrt(variancia);

        printf("Media: %.2f\n", media);
        printf("Desvio Padrão: %.2f\n", desvioPadrao);

        // Abre o arquivo para escrita em modo de acrescentamento
        FILE *mediana_file = fopen("ficha03_mediana.dat", "a");
        if (mediana_file == NULL)
        {
            printf("Erro ao abrir o arquivo ficha03_mediana.dat\n");
        }
        else
        {
            fprintf(mediana_file, "%.2f %.2f\n", media, desvioPadrao);
            fclose(mediana_file);
            printf("ficheiro ficha03_mediana.dat atualizado com sucesso!\n");
        }

        // Abre o arquivo para escrita em modo de substituição
        FILE *coluna_file = fopen("ficha03_coluna.dat", "w");
        if (coluna_file == NULL)
        {
            printf("Erro ao abrir o arquivo ficha03_coluna.dat\n");
        }
        else
        {
            fprintf(coluna_file, "%.2f\n", media);
            fclose(coluna_file);
            printf("ficheiro ficha03_coluna.dat atualizado com sucesso!\n");
        }

        printf("Operações realizadas com sucesso!\n");
    }
    else
    {
        printf("Não foi possível calcular a média e o desvio padrão.\n");
    }

    printf("Sinal SIGUSR1 enviado pelo processo filho A (PID %d) ao processo pai (PID %d).\n\n", getpid(), getppid());

    kill(getppid(), SIGUSR1);
}

// Manipulador de sinal SIGUSR2 para o processo filho B
void sigusr2_handler_filho2(int signum)
{
    system("clear");

    printf("sigusr2_handler_filho2\n\n");
    printf("PROCESSO FILHO B (PID %d): Sinal SIGUSR2 enviado ao processo pai (PID %d)\n", getpid(), getppid());
    printf("Pressione Enter para continuar...\n");
    fflush(stdin);
    getchar(); // Aguarda a entrada do usuário antes de continuar

    kill(getppid(), SIGUSR2); // Envia sinal SIGUSR2 ao processo pai

}

// Manipulador de sinal SIGTERM para o processo filho C
void sigterm_handler_filho3(int signum)
{
    printf("sigterm_handler_filho3\n\n");
    printf("PROCESSO FILHO C (PID %d): Sinal SIGTERM recebido.\n", getpid());
    printf("Pressione Enter para continuar...\n");
    getchar();
    fflush(stdin);
    getchar(); // Aguarda a entrada do usuário antes de continuar
    system("clear");
    kill(getppid(), SIGTERM); // Envia sinal SIGTERM ao processo pai

}

void sigkill_filho1(int signal)
{
    printf("morri com sucesso\n");
}
void sigkill_filho2(int signal)
{
    printf("morri com sucesso\n");
}
void sigkill_filho3(int signal)
{
    printf("morri com sucesso\n");
}

void Ficha3()
{
    int filho1, filho2, filho3, pai = getpid();
    int opcao;

    filho1 = valorFilhoA = fork();
    filho2 = valorFilhoB = fork();
    filho3 = valorFilhoC = fork();
    valorPai = pai;  

    while (1)
    {
    
        if (filho1 == 0)
        {
            signal(SIGUSR1, sigusr1_handler_filho1);
            pause();
        }
        else
        {
            if (filho2 == 0)
            {
                signal(SIGUSR2, sigusr2_handler_filho2);
                pause();
            }
            else
            {
                if (filho3 == 0)
                {
                    signal(SIGTERM, sigterm_handler_filho3);
                    pause();
                }
                else
                {
                    printf("Menu da ficha 3\n");
                    printf("1 - Cálculo da média e variância\n");
                    printf("2 - Apagar ficheiros\n");
                    printf("0 - Sair\n");

                    printf("PROCESSO PAI (PID %d): criei os processos A (PID %d), B (PID %d) e C (PID %d)\n", pai, filho1, filho2, filho3);
                    printf("Por favor introduza a opção pretendida: ");
                    scanf("%d", &opcao);

                    if (opcao == 1)
                    {
                        system("clear");
                        printf("\n\nSinal SIGUSR1 enviado pelo processo pai (PID %d) ao processo filho A (PID %d) para iniciar o cálculo.\n\n", pai, filho1);
                        kill(filho1, SIGUSR1); //enviar o sinal, ou seja, chamamos o filho 1
                        signal(SIGUSR1, sigusr1_handler_pai); //receber o sinal do filho A
                        pause();

                        // ... (Aguarda sinal SIGUSR1 do processo filho A)
                        system("clear");
                        printf("voltas-te ao menu");
                        printf("\n\nPROCESSO PAI (PID %d): Sinal SIGUSR1 recebido do processo filho A (PID %d).\n\n", pai, filho1);
                        printf("Sinal SIGUSR2 será enviado ao processo filho B (PID %d) para apresentação do conteúdo dos ficheiros gerados.\n", filho2);
                        printf("Pressione Enter para continuar...\n");
                        fflush(stdin);
                        getchar();
                        kill(filho2, SIGUSR2);
                        signal(SIGUSR2, sigusr2_handler_pai);
                        pause();

                        system("clear");
                        kill(filho3, SIGTERM);
                        signal(SIGTERM, sigterm_handler_pai);
                        pause();
                        

                    }
                    else if (opcao == 2)
                    {
                        remove("ficha03_media.dat");
                        remove("ficha03_sucesso.dat");
                        printf("\nPROCESSO PAI (PID %d): Eliminação com sucesso dos ficheiros .dat.\n", valorPai);
                        printf("Pressione uma tecla para continuar...");
                        getchar();
                        fflush(stdin);
                        getchar();
                    }
                    else if (opcao == 0)
                    {
                        kill(valorFilhoA, SIGKILL);
                        kill(valorFilhoB, SIGKILL);
                        kill(valorFilhoC, SIGKILL);

                        printf("PROCESSO PAI (PID %d): Sinal SIGKILL enviado aos filhos com PIDs %d, %d, e %d.",valorPai, valorFilhoA, valorFilhoB, valorFilhoC);
                        getchar();
                        fflush(stdin);
                        getchar();
                        break;
                    }
                }
            }
        }
    }
}

void main()
{
    Ficha3();
}