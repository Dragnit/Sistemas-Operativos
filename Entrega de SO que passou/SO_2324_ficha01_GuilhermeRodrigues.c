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


void listarProcessosAux() 
{
    system("ps aux");
}

void listarProcessosComParametros() 
{
    char comando[100];
    char comandoCmpleto[100] = "ps ";

    printf("Digite o comando 'ps' com os parâmetros desejados: ");
    scanf("%s", comando);

    strcat(comandoCmpleto, comando);
    system(comandoCmpleto);

}

void listarProcessoEspecifico() 
{
    char processo[50];
    char processoCmpleto[100] = "ps aux | grep ";

    printf("Digite o nome do processo a ser pesquisado: ");
    scanf("%s", processo);

    strcat(processoCmpleto, processo);
    system(processoCmpleto);

}

void monitorarProcessosEmTempoReal() 
{
    system("top");
}

void submenuPIDs() 
{

    int opcaoPID;

    do 
    {
        printf("\n\nSubmenu PIDs\n\n");
        printf("1. PID do processo atual (getpid())\n");
        printf("2. PID do processo pai (getppid())\n");
        printf("0. Voltar ao menu principal\n");

        printf("\nDigite sua opção: ");
        scanf("%d", &opcaoPID);

        switch (opcaoPID) {
            case 1:
                printf("Processo atual: %d",getpid());
                break;
            case 2:
                printf("Processo pai: %d",getppid());
                break;
            case 0:
                printf("\nVoltando ao menu principal...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
    } while (opcaoPID != 0);
}

void estatistica()
{
    char nomeFicheiro[100];
    char linhaCabecalho;
    int coluna;
    char linha[1024];
    double  valores[1000];
    int indice = 0;

    printf("Digite o nome do ficheiro CSV: ");
    scanf("%s", nomeFicheiro);
    printf("Existe uma linha de cabecalho no ficheiro? (s/n): ");
    scanf(" %c", &linhaCabecalho);
    printf("Qual coluna deseja analisar (começando de 1): ");
    scanf("%d", &coluna);

    FILE *ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) 
    {
        printf("Erro ao abrir o ficheiro.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), ficheiro)) 
    {
        if (linhaCabecalho == 's') 
        {
            // Ignorar a primeira linha (cabeçalho)
            if (indice == 0) 
            {
                indice++;
                continue;
            }
        }
        char *token = strtok(linha, ",");
        int col = 0;
        while (token != NULL) 
        {
            col++;
            if (col == coluna) 
            {
                // Converter o valor da coluna para double
                double valor = atof(token);
                valores[indice++] = valor;
                break;
            }
            token = strtok(NULL, ",");
        }
    }
    fclose(ficheiro);

    // Calcular a média e a variância
    double soma = 0.0;
    for (int i = 0; i < indice; i++) {
        soma += valores[i];
    }
    double media = soma / indice;

    soma = 0.0;
    for (int i = 0; i < indice; i++) {
        soma += pow(valores[i] - media, 2);
    }
    double variancia = soma / indice;

    printf("Média da coluna %d: %.2f\n", coluna, media);
    printf("Variância da coluna %d: %.2f\n", coluna, variancia);
}

void estatisticaAWK()
{
    char nomeFicheiro[50];
    char nome[20];
    char buffer[1024];
    FILE *file;
    FILE *fileTemp;
    int indiceColuna = -1;
    char *token;
    int i =0;
    char comando [200];


    printf("Indique o nome do ficheiro: ");scanf("%s",&nomeFicheiro);
 
    file = fopen(nomeFicheiro,"r");

    if(file == NULL)
    {
        printf("O ficheiro %s não existe.",nomeFicheiro);
    }
    else
    {
        printf("Indique o nome da coluna: ");scanf("%s",&nome);
   
        fgets(buffer,1024,file);//Le a primeira linha
   
        token = strtok(buffer, ",");

        while(token != NULL)
        {
            if(strcmp(token,nome) == 0)
            {
                indiceColuna = i;
                break;
            }
            i++;
            token = strtok(NULL,",");
        }

        if(indiceColuna == -1)
        {
            printf("A coluna %s não existe",nome);
        }
        else
        {
            sprintf(comando, "awk -F',' '{ valor = strtonum($%d); print valor }' %s > temp.txt",indiceColuna+1,nomeFicheiro);
            system(comando);

            int n = 0;
            float soma = 0;
            float valor = 0;
            float var = 0;

            fileTemp = fopen("temp.txt","r");

            if(fileTemp == NULL)
            {
                printf("Erro ao abrir ao ficheiro");
            }
            else
            {
                while (fscanf(fileTemp, "%f", &valor) == 1) 
                {
                    soma += valor;
                    var += valor * valor;
                    n++;
                }

                fclose(fileTemp);

                float media = soma / n;
                float variancia = (var / n) - (media * media);

                printf("Media: %f\n",media);
                printf("Variancia: %f\n",variancia);
            }
        }
    }
}

int processos() 
{
    int opcao;

    do 
    {
        printf("\n\nMenu de Listagem de Processos\n\n");
        printf("1. Listar todos os processos (ps aux)\n");
        printf("2. Listar processos com parâmetros (ps)\n");
        printf("3. Listar processo específico (ps aux | grep)\n");
        printf("4. Monitorar processos em tempo real (top)\n");
        printf("0. Sair\n");

        printf("\nDigite sua opção: ");
        scanf("%d", &opcao);

        switch (opcao) 
        {
            case 1:
                listarProcessosAux();
                break;
            case 2:
                listarProcessosComParametros();
                break;
            case 3:
                listarProcessoEspecifico();
                break;
            case 4:
                monitorarProcessosEmTempoReal();
                break;
            case 0:
                printf("\nSaindo do menu...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

void Ficha1()
{
   int opcao;

    do {
        printf("1. Listagem de Processos)\n");
        printf("2. Consultar PIDs\n");
        printf("3. Estatística Descritiva (Média e Variância)\n");
        printf("4. Estatística Descritiva (Média e Variância) com awk\n");
        printf("0. Sair\n");

        printf("\nDigite sua opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                processos();
                break;
            case 2:
                submenuPIDs();
                break;
            case 3:
                estatistica();
                break;
            case 4:
                estatisticaAWK();
                break;   
            case 0:
                printf("\nSaindo do menu...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);
 
}

void mediana() 
{
    char nomeFicheiro[50];
    char nomeColuna[20];
    char buffer[1024];
    FILE *file;
    int indiceColuna = -1;
    char *token;
    int i =0;

    int filho1;
    int filho2;

    int valor = 0;
    int n = 0;
    int media = 0;
    int variancia = 0;

    int status;

    printf("Indique o nome do ficheiro: ");
    scanf("%s",&nomeFicheiro);
    
    file = fopen(nomeFicheiro,"r");
    if(file == NULL)
    {
        printf("O ficheiro %s não existe.",nomeFicheiro);
    }
    else
    {
        printf("Indique o nome da coluna: ");
        scanf("%s",&nomeColuna);
        
        fgets(buffer,1024,file);//Le a primeira linha
        
        token = strtok(buffer, ",");
        while(token != NULL)
        {
            if(strcmp(token,nomeColuna) == 0)
            {
                indiceColuna = i;
                break;
            }
            i++;
            token = strtok(NULL,",");
        }
        if(indiceColuna == -1)
        {
            printf("A coluna %s não existe",nomeColuna);
        }
        else
        {
            filho1 = fork();
            if(filho1 == 0) //processo do 1 filho
            {
                FILE* file_dat = fopen("hist.dat", "w");

                if (file_dat == NULL || file == NULL) 
                {
                    printf("o ficheiro nao existe\n");
                    if (file_dat)
                    {
                        fclose(file_dat);
                        return;
                    }
                }

                int valores[1000]; // Supondo que o máximo de valores lidos seja 1000
                int n = 0;
                char buffer[100];
                char* token;
                int valor;

                while (fgets(buffer, 100, file)) 
                {
                    token = strtok(buffer, ",");
                    int i = 0;

                    while (token != NULL && i < indiceColuna) 
                    {
                        token = strtok(NULL, ",");
                        i++;
                    }

                    if (token != NULL) 
                    {
                        valor = atoi(token);
                        fprintf(file_dat, "%d\n", valor);
                        valores[n++] = valor;
                    }

                }
                    
                fclose(file_dat);

                if (n > 0) 
                {
                    // Ordenar os valores
                    for (int i = 0; i < n - 1; i++) 
                    {
                        for (int j = i + 1; j < n; j++) 
                        {
                            if (valores[i] > valores[j]) 
                            {
                                int temp = valores[i];
                                valores[i] = valores[j];
                                valores[j] = temp;
                            }
                        }
                    }

                    // Calcular a mediana
                    double mediana;
                    if (n % 2 == 0) 
                    {
                        mediana = (valores[n / 2 - 1] + valores[n / 2]) / 2.0;
                    } 
                    else 
                    {
                        mediana = valores[n / 2];
                    }

                    // Calcular a média
                    double soma = 0;
                    for (int i = 0; i < n; i++) 
                    {
                        soma += valores[i];
                    }
                    
                    double media = soma / n;

                    // Calcular a soma dos quadrados das diferenças em relação à média
                    double somaQuadrados = 0;
                    
                    for (int i = 0; i < n; i++) 
                    {
                        somaQuadrados += (valores[i] - media) * (valores[i] - media);
                    }

                    // Calcular a variância e o desvio padrão
                    double variancia = somaQuadrados / n;
                    double desvioPadrao = sqrt(variancia);

                    // Gravar a mediana e o desvio padrão no arquivo
                    FILE* output = fopen("ficha02_mediana.dat", "a");
                    if (output != NULL) 
                    {
                        printf("mediana: %.2f, desvio padrao: %.2f\n", mediana, desvioPadrao);
                        fprintf(output, "mediana: %.2f, desvio padrao: %.2f\n", mediana, desvioPadrao);
                        printf("pressione qualquer tecla para prosseguir.....");
                        getchar();
                        fflush(stdin); 
                        getchar();
                        fclose(output);
                    } 
                    else
                    {
                        printf("Nao foi possivel abrir o ficheiro de saída\n");
                    }

                    printf("Tarefa executada com sucesso pelo filho1: processo %d\n", getpid());
                    printf("Pressione qualquer tecla para prosseguir.....");
                    fflush(stdin);
                    getchar();
                } 
                else
                {
                    printf("Nao foi possivel cacular a media");
                }
                kill(getpid(),SIGSEGV);
            }
            else
            {//processo do pai
                wait(NULL);
                filho2 = fork();
                if(filho2 == 0)
                {//processos do 2 filho
                    
                    const int MAX_VALORES = 1000;
                    int valores[MAX_VALORES];
                    FILE * file_mediana = fopen("ficha02_mediana.dat","r");
                    FILE * file_sucesso = fopen("hist.dat","r");
                    FILE * ordena = fopen("ficha02_ordenacao.dat","w");

                    if(file_mediana == NULL && file_sucesso == NULL)
                    {
                        printf("Deu erro no ficheiro");
                    }
                    else
                    {
                        while (fscanf(file_sucesso, "%d", &valor) != EOF) 
                        {
                            if (n < MAX_VALORES) 
                            {
                                valores[n++] = valor;
                            } 
                            else 
                            {
                                printf("Número máximo de valores excedido.\n");
                                fclose(file_sucesso);
                                exit(1);
                            }
                        }

                        fclose(file_sucesso);

                        // Ordenar os valores
                        for (i = 0; i < n - 1; i++) 
                        {
                            for (int j = i + 1; j < n; j++) 
                            {
                                if (valores[i] > valores[j]) 
                                {
                                    int temp = valores[i];
                                    valores[i] = valores[j];
                                    valores[j] = temp;
                                }
                            }
                        }

                        FILE * ordena = fopen("ficha02_ordenacao.dat", "w");

                        if (ordena == NULL) 
                        {
                            printf("Erro ao abrir o ficheiro %s para escrita.\n", nomeFicheiro);
                            exit(1);
                        }

                        for (i = 0; i < n; i++) 
                        {
                            fprintf(ordena, "%d\n", valores[i]);
                        }

                        fclose(ordena);

                        printf("Ficheiro ficha02_ordenacao.dat organizado com sucesso!\n");


                        fclose(file_mediana);
                        fclose(file_sucesso);
                        printf("Tarefa executada com sucesso pelo filho2: processo %d\n",getpid());
                        printf("Pressione qualquer tecla para prosseguir.....");
                        fflush(stdin);
                        getchar();
                    }
                    //acabar o processo
                    kill(getpid(),SIGSEGV);
                }
                else
                {//processo do pai         
                    wait(NULL);          
                    printf("Tarefa executada com sucesso pelo pai: processo %d\n",getpid());
                }
            }
        }
    }
}

void apagar()
{

    int filho1,filho2;

    filho1 = fork();

    if (filho1 == 0)
    {
        if(remove("ficha02_mediana.dat") == 0)
        {
            printf("Removido o ficheiro 'ficha02_mediana.dat'\n");
        }
        else
        {
            printf("Não foi possivel remover o ficheiro 'ficha02_mediana.dat'\n");
        }
        kill(getpid(),SIGSEGV);
    }
    else
    {
        wait(NULL);
        filho2 = fork();
        if (filho2 == 0)
        {
            if(remove("ficha02_ordenacao.dat") == 0)
            {
                printf("Removido o ficheiro 'ficha02_mediana.dat'\n");
            }
            else
            {
                printf("Não foi possivel remover o ficheiro 'ficha02_mediana.dat'\n");
            }

            kill(getpid(),SIGSEGV);
        }
        else 
        {   
            wait(NULL);
            printf("Ficheiros apagados pelos filhos");
        }
    }
    
}

void Ficha2()
{
    int opcao;

    do {
        printf("1. Cálculo da mediana e desvio padrão\n");
        printf("2. Apagar ficheiros de resultados\n");
        printf("0. Sair\n");

        printf("\nInsira sua opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                mediana();
                break;
            case 2:
                apagar();
                break;  
            case 0:
                printf("\nA Sair...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);
}

int valorFilhoC, valorFilhoA, valorFilhoB, valorPai;

//sinal SIGUSR1 para o processo pai, vindo do A 
void sigusr1_handler_pai(int signum) 
{
    system("clear");
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
    printf("\nPROCESSO PAI (PID %d): Sinal SIGUSR2 recebido.\n", valorPai);
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
    system("clear");
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
        FILE *ficheiro2 = fopen("ficha03_mediana.dat", "a");

        if (ficheiro2 == NULL) 
        {
            printf("O ficheiro nao existe");
            exit(1);
        } 
        else 
        {
            FILE *fp = fopen(nomeFicheiro, "r");
            if (fp == NULL) 
            {
                printf("Erro ao abrir o ficheiro %s.\n", nomeFicheiro);
                fclose(ficheiro2);
                exit(1);
            }

            char buffer[1024];
            char *token;
            int i, n = 0, valor;
            int valores[1000]; // Array para armazenar os valores

            fgets(buffer, 1024, fp); // Descartar a linha do cabeçalho

            double soma = 0.0;
            double somaQuadrados = 0.0;

            while (fgets(buffer, 1024, fp)) 
            {
                token = strtok(buffer, ",");
                i = 0;
                while (token != NULL && i < numeroColuna) 
                {
                    token = strtok(NULL, ",");
                    i++;
                }
                if (token != NULL) 
                {
                    valor = atoi(token); // atoi transforma a string em inteiro
                    valores[n++] = valor; // Armazena o valor no array
                    fprintf(ficheiro2, "%d\n", valor);
                    soma += valor;
                    somaQuadrados += valor * valor;
                }
            }

            fclose(fp);
            fclose(ficheiro2);

            if (n > 0) 
            {
                // Ordenar os valores para calcular a mediana
                for (i = 0; i < n - 1; i++) {
                    for (int j = i + 1; j < n; j++) 
                    {
                        if (valores[i] > valores[j]) 
                        {
                            int temp = valores[i];
                            valores[i] = valores[j];
                            valores[j] = temp;
                        }
                    }
                }

                // Calcular a mediana
                double mediana;
                if (n % 2 == 0) 
                {
                    mediana = (valores[n / 2 - 1] + valores[n / 2]) / 2.0;
                } 
                else 
                {
                    mediana = valores[n / 2];
                }

                // Calcular o desvio padrão
                double media = soma / n;
                double variancia = (somaQuadrados / n) - (media * media);
                double desvioPadrao = sqrt(variancia);

                printf("Mediana: %.2f\n", mediana);
                printf("Desvio Padrão: %.2f\n", desvioPadrao);

                // Abre o arquivo para escrita em modo de acrescentamento
                FILE *mediana_file = fopen("ficha03_mediana.dat", "a");
                if (mediana_file == NULL) {
                    printf("Erro ao abrir o arquivo ficha03_mediana.dat\n");
                } else {
                    fprintf(mediana_file, "%.2f %.2f\n", mediana, desvioPadrao);
                    fclose(mediana_file);
                    printf("ficheiro ficha03_mediana.dat atualizado com sucesso!\n");
                }

                // Abre o arquivo para escrita em modo de substituição
                FILE *coluna_file = fopen("ficha03_coluna.dat", "w");
                if (coluna_file == NULL) {
                    printf("Erro ao abrir o arquivo ficha03_coluna.dat\n");
                } else {
                    for (int k = 0; k < n; k++) {
                        fprintf(coluna_file, "%d\n", valores[k]);
                    }
                    fclose(coluna_file);
                    printf("ficheiro ficha03_coluna.dat atualizado com sucesso!\n");
                }

                printf("Operações realizadas com sucesso!\n");
            } else {
                printf("Não foi possível calcular a mediana e o desvio padrão.\n");
            }
            printf("Sinal SIGUSR1 enviado pelo processo filho A (PID %d) ao processo pai (PID %d).\n\n", getpid(), getppid());
        }
        kill(getppid(), SIGUSR1);
    }
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
    const char *comando = "gnuplot -e \"set boxwidth 0.8; set style fill solid; plot 'ficha03_coluna.dat' using 1 smooth frequency with boxes; pause mouse;\"";
    printf("sigterm_handler_filho3\n\n");
    printf("PROCESSO FILHO C (PID %d): Sinal SIGTERM recebido.\n", getpid());
    system(comando);
    printf("Pressione Enter para continuar...\n");
    getchar();
    fflush(stdin);
    getchar(); // Aguarda a entrada do usuário antes de continuar
    system("clear");
    kill(getppid(), SIGTERM); // Envia sinal SIGTERM ao processo pai

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

                        system("clear");
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

void* transformaColuna()
{
    char nomeFicheiro[25];
    char nomeFicheiro2[25];
    char linhas[1000];
    int numColuna;
    int numColunaAtual;
    int numRepeticoes;

    char buffer[1000];
    FILE* fp;
    FILE* invertido;

    int n = 1;

    printf("Introduza o nome do ficheiro sem a extenção do mesmo: ");
    scanf("%s", nomeFicheiro);
    printf("Introduza o número da coluna: ");
    scanf("%d", &numColuna);        
    printf("Introduza o número de repetições: ");
    scanf("%d", &numRepeticoes);

    strcat(nomeFicheiro, ".csv");
    printf("%s\n", nomeFicheiro);

    fp = fopen(nomeFicheiro, "r");
    

    if(fp == NULL)
    {
        printf("Ficheiro inexistente!\n");
    }
    else
    {
        strcpy(nomeFicheiro2, nomeFicheiro);
        strcat(nomeFicheiro2, "_Inv.csv");

        invertido = fopen(nomeFicheiro2, "w");
        if(invertido != NULL)
        {

            while(fgets(linhas,sizeof(linhas), fp) != NULL)
            {
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL)
                {
                    if(count == numColuna)
                    {
                        for(int i=0;i<numRepeticoes;i++)
                        {
                            fprintf(invertido,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }

            printf("Geração  do  ficheiro  com  troca  de  coluna  por  linha  com  repetições efetuada com sucesso");
            fclose(invertido);

            printf("\nPressione Enter para continuar...\n");
            getchar();
            fflush(stdin);
            getchar();
            
        }
        else
        {
            printf("O ficheiro nao existe");
        }

        fclose(fp);
    }
    pthread_exit(NULL);
}

typedef struct 
{
    int coluna;
    int repeticoes;
    char nomeFicheiro[50];
} Thread;

void* threadA(void* argumento)
{
    FILE* fp;
    FILE* fp2;

    fp = fopen("PL_Programacao_04_Threads.csv","a");

    if(fp != NULL)
    {
        Thread* argumentos = (Thread*)argumento;

        int coluna = argumentos->coluna;
        int repeticoes = argumentos->repeticoes;
        char nomeFicheiro[50];

        strcpy(nomeFicheiro,argumentos->nomeFicheiro);

        fp2 = fopen(nomeFicheiro,"r");

        if(fp2 != NULL)
        {
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), fp2) != NULL)
            {
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL)
                {
                    if(count == coluna)
                    {
                        for(int i=0;i<repeticoes;i++)
                        {
                            fprintf(fp,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }
        else
        {
            printf("Nao foi possivel abrir o ficheiro");
        }
        
        fclose(fp);
        printf("Thread 1 do processo %d escreveu com sucesso no ficheiro PL_Programacao_04_Threads.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }

    printf("Pressione Enter para continuar...\n");
    getchar();
    fflush(stdin);
    getchar();
    pthread_exit(NULL);
    return NULL;
}

void* threadB(void* argumento)
{
    FILE* file;
    FILE* filePrin;

    file = fopen("PL_Programacao_04_Threads.csv","a");

    if(file != NULL)
    {
        Thread* argumentos = (Thread*)argumento;

        int coluna = argumentos->coluna;
        int repeticoes = argumentos->repeticoes;
        char nomeFicheiro[50];

        strcpy(nomeFicheiro,argumentos->nomeFicheiro);

        filePrin = fopen(nomeFicheiro,"r");

        if(filePrin != NULL)
        {
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), filePrin) != NULL)
            {
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL)
                {
                    if(count == coluna)
                    {
                        for(int i=0;i<repeticoes;i++)
                        {
                            fprintf(file,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }
        else
        {
            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(file);
        printf("Thread 2 do processo %d escreveu com sucesso no ficheiro PL_Programacao_04_Threads.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    fflush(stdin);
    getchar();

    pthread_exit(NULL);
    return NULL;
}

void* threadC(void* argumento)
{
    FILE* file;
    FILE* filePrin;

    file = fopen("PL_Programacao_04_Threads.csv","a");

    if(file != NULL){
        Thread* argumentos = (Thread*)argumento;

        int coluna = argumentos->coluna;
        int repeticoes = argumentos->repeticoes;
        char nomeFicheiro[50];

        strcpy(nomeFicheiro,argumentos->nomeFicheiro);

        filePrin = fopen(nomeFicheiro,"r");
        
        if(filePrin != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), filePrin) != NULL)
            {
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == coluna){
                        for(int i=0;i<repeticoes;i++)
                        {
                            fprintf(file,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }
        else
        {
            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(file);
        printf("Thread 3 do processo %d escreveu com sucesso no ficheiro PL_Programacao_04_Threads.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    fflush(stdin);
    getchar();

    pthread_exit(NULL);
    return NULL;
}

void ExtracaoColuna()
{
    FILE* fp;
    FILE* fp2;

    char nomeFicheiro[50];
    int coluna1,coluna2,coluna3;
    int nRepeticoes;

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    fp = fopen("PL_Programacao_04_Threads.csv", "w");

    if(fp != NULL)
    {
        printf("Geração do ficheiro PL_Programacao_04_Threads.csv efetuada com sucesso\n");

        printf("Nome do ficheiro: ");
            scanf("%s",nomeFicheiro);
        
        fp2 = fopen(nomeFicheiro,"r");

        if(fp2 != NULL)
        {
            printf("Indique o numero das 3 colunas: ");
            scanf("%d %d %d",&coluna1,&coluna2,&coluna3);

            printf("Indique o numero de repeticoes: ");
            scanf("%d",&nRepeticoes);          

            Thread thread1Estrutura;
            thread1Estrutura.coluna = coluna1;
            thread1Estrutura.repeticoes = nRepeticoes;
            strcpy(thread1Estrutura.nomeFicheiro,nomeFicheiro);

            Thread thread2Estrutura;
            thread2Estrutura.coluna = coluna2;
            thread2Estrutura.repeticoes = nRepeticoes;
            strcpy(thread2Estrutura.nomeFicheiro,nomeFicheiro);

            Thread thread3Estrutura;
            thread3Estrutura.coluna = coluna3;
            thread3Estrutura.repeticoes = nRepeticoes;
            strcpy(thread3Estrutura.nomeFicheiro,nomeFicheiro);


            pthread_create(&thread1,NULL,threadA,&thread1Estrutura);
            pthread_create(&thread2,NULL,threadB,&thread2Estrutura);
            pthread_create(&thread3,NULL,threadC,&thread3Estrutura);
            
            pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);
            pthread_join(thread3, NULL);

        }
        else
        {
            printf("O ficheiro nao existe");
        }


        fclose(fp);
    }else{
        printf("O ficheiro nao existe");
    }
    
}

void Ficha4()
{
    int opcao = 0;

    pthread_t transforma;

    do
    {
        printf("Menu ficha 4\n");
        printf("1-Transformação de coluna em linha com repetições\n");
        printf("2-Extração e gravação de colunas\n");
        printf("3-Sair\n");
        printf("Opcao: ");
        scanf("%d",&opcao);

        switch(opcao)
        {
            case 1: 
                pthread_create(&transforma,NULL,transformaColuna,NULL);
                pthread_join(transforma, NULL);
                break;
            case 2:
                ExtracaoColuna();
                break;
            case 3: 
                printf("A sair.....\n");
                break;
            default:printf("Opção invalida\n");break;
        }
    }while(opcao!= 3);
}

pthread_mutex_t mutex;

void* thread1ExtracaoMutex(void *argumento)
{

    pthread_mutex_lock(&mutex);

    FILE* fp;
    FILE* fp2;

    fp = fopen("PL_Programacao_05_Sinc.csv","a");

    if(fp != NULL)
    {
        Thread* argumentos = (Thread*)argumento;

        int coluna = argumentos->coluna;
        int nRepeticoes = argumentos->repeticoes;

        char nomeFicheiro[50];

        strcpy(nomeFicheiro,argumentos->nomeFicheiro);

        fp2 = fopen(nomeFicheiro,"r");

        if(fp2 != NULL)
        {
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), fp2) != NULL)
            {
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL)
                {
                    if(count == coluna)
                    {
                        for(int i=0;i<nRepeticoes;i++)
                        {
                            fprintf(fp,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }
        else
        {
            printf("Nao foi possivel abrir o ficheiro");
        }
        
        fclose(fp);
        printf("Thread 1 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(), coluna, nRepeticoes);
    }

    printf("Pressione Enter para continuar...\n");
    getchar();
    fflush(stdin);
    getchar();

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
    return NULL;

}

void* thread2ExtracaoMutex(void *argumento)
{
    
    pthread_mutex_lock(&mutex);

    FILE* fp;
    FILE* fp2;
    fp = fopen("PL_Programacao_05_Sinc.csv","a");
    if(fp != NULL){
        Thread* argumentos = (Thread*)argumento;

        int coluna = argumentos->coluna;
        int nRepeticoes = argumentos->repeticoes;

        char nomeFicheiro[50];

        strcpy(nomeFicheiro,argumentos->nomeFicheiro);

        fp2 = fopen(nomeFicheiro,"r");

        if(fp2 != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), fp2) != NULL)
            {
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == coluna){
                        for(int i=0;i<nRepeticoes;i++)
                        {
                            fprintf(fp,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }
        else{

            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(fp);
        printf("Thread 2 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(), coluna, nRepeticoes);
    }
    printf("Pressione Enter para continuar...\n");
    getchar();
    fflush(stdin);
    getchar();

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
    return NULL;

}

void* thread3ExtracaoMutex(void *argumento)
{
    
    pthread_mutex_lock(&mutex);

    FILE* fp;
    FILE* fp2;

    fp = fopen("PL_Programacao_05_Sinc.csv","a");

    if(fp != NULL){
        Thread* argumentos = (Thread*)argumento;

        int coluna = argumentos->coluna;
        int repeticoes = argumentos->repeticoes;

        char nomeFicheiro[50];

        strcpy(nomeFicheiro,argumentos->nomeFicheiro);

        fp2 = fopen(nomeFicheiro,"r");
        if(fp2 != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), fp2) != NULL)
            {
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL)
                {
                    if(count == coluna)
                    {
                        for(int i=0;i<repeticoes;i++)
                        {
                            fprintf(fp,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }
        else
        {
            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(fp);
        printf("Thread 3 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }
    printf("Pressione Enter para continuar...\n");
    getchar();
    fflush(stdin);
    getchar();

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
    return NULL;

}

void extracaoGravacaoMutex()
{
    FILE* fp;
    FILE* fp2;

    char nomeFicheiro[50];
    int coluna1, coluna2, coluna3;
    int nRepeticoes;

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    fp = fopen("PL_Programacao_05_Sinc.csv", "w");

    if(fp != NULL)
    {
        printf("Geração do ficheiro PL_Programacao_05_Sinc.csv efutuada com sucesso\n");

        printf("Insira o nome do ficheiro: ");
        scanf("%s",nomeFicheiro);
        
        fp2 = fopen(nomeFicheiro,"r");  
        if(fp2 != NULL)
        {
            printf("Indique o numero das 3 colunas: ");
            scanf("%d %d %d",&coluna1,&coluna2,&coluna3);
            
            printf("Indique o numero de repeticoes: ");
            scanf("%d",&nRepeticoes);

            Thread thread1Estrutura;
            thread1Estrutura.coluna = coluna1;
            thread1Estrutura.repeticoes = nRepeticoes;
            strcpy(thread1Estrutura.nomeFicheiro,nomeFicheiro);

            Thread thread2Estrutura;
            thread2Estrutura.coluna = coluna2;
            thread2Estrutura.repeticoes = nRepeticoes;
            strcpy(thread2Estrutura.nomeFicheiro,nomeFicheiro);

            Thread thread3Estrutura;
            thread3Estrutura.coluna = coluna3;
            thread3Estrutura.repeticoes = nRepeticoes;
            strcpy(thread3Estrutura.nomeFicheiro,nomeFicheiro);

            pthread_mutex_init(&mutex, NULL);

            pthread_create(&thread1,NULL,thread1ExtracaoMutex,&thread1Estrutura);
            pthread_create(&thread2,NULL,thread2ExtracaoMutex,&thread2Estrutura);
            pthread_create(&thread3,NULL,thread3ExtracaoMutex,&thread3Estrutura);
            
            pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);
            pthread_join(thread3, NULL);
          
        }
        else
        {
            printf("O ficheiro nao existe");
        }
    }else{
        printf("O ficheiro nao existe");
    }

}

sem_t semaforo;

void* thread1ExtracaoSemaforo(void *argumento)
{
    FILE* fp;
    FILE* fp2;

    fp = fopen("PL_Programacao_05_Sinc.csv","a");

    if(fp != NULL)
    {
        Thread* argumentos = (Thread*)argumento;

        int coluna = argumentos->coluna;
        int nRepeticoes = argumentos->repeticoes;

        char nomeFicheiro[50];
        strcpy(nomeFicheiro,argumentos->nomeFicheiro);

        fp2 = fopen(nomeFicheiro,"r");
        if(fp2 != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), fp2) != NULL)
            {
                char *token = strtok(linhas, ",");
                int count = 1;

                while(token != NULL)
                {
                    if(count == coluna){
                        for(int i=0;i<nRepeticoes;i++)
                        {
                            fprintf(fp,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }
        else
        {
            printf("Nao foi possivel abrir o ficheiro");
        }
        
        fclose(fp);
        printf("Thread 1 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(), coluna, nRepeticoes);
    }

    printf("Pressione Enter para continuar...\n");
    getchar();
    fflush(stdin);
    getchar();

    sem_post(&semaforo);

    pthread_exit(NULL);
    return NULL;

}

void* thread2ExtracaoSemaforo(void *argumento){
    FILE* fp;
    FILE* fp2;

    fp = fopen("PL_Programacao_05_Sinc.csv","a");

    if(fp != NULL)
    {
        Thread* argumentos = (Thread*)argumento;

        int coluna = argumentos->coluna;
        int nRepeticoes = argumentos->repeticoes;

        char nomeFicheiro[50];

        strcpy(nomeFicheiro,argumentos->nomeFicheiro);

        fp2 = fopen(nomeFicheiro,"r");
        if(fp2 != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), fp2) != NULL)
            {
                char *token = strtok(linhas, ",");
                int count = 1;

                while(token != NULL)
                {
                    if(count == coluna){
                        for(int i=0;i<nRepeticoes;i++)
                        {

                            fprintf(fp,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }
        else
        {
            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(fp);
        printf("Thread 2 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(), coluna, nRepeticoes);
    }
    printf("Pressione Enter para continuar...\n");
    getchar();
    fflush(stdin);
    getchar();

    sem_post(&semaforo);

    pthread_exit(NULL);
    return NULL;

}

void* thread3ExtracaoSemaforo(void *argumento)
{
    FILE* fp;
    FILE* fp2;

    fp = fopen("PL_Programacao_05_Sinc.csv","a");

    if(fp != NULL)
    {
        Thread* argumentos = (Thread*)argumento;

        int coluna = argumentos->coluna;
        int nRepeticoes = argumentos->repeticoes;

        char nomeFicheiro[50];

        strcpy(nomeFicheiro,argumentos->nomeFicheiro);

        fp2 = fopen(nomeFicheiro,"r");

        if(fp2 != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), fp2) != NULL)
            {
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL)
                {
                    if(count == coluna)
                    {
                        for(int i=0;i<nRepeticoes;i++)
                        {
                            fprintf(fp,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }
        else
        {
            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(fp);
        printf("Thread 3 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(), coluna, nRepeticoes);
    }
    printf("Pressione Enter para continuar...\n");
    getchar();
    fflush(stdin);
    getchar();

    sem_post(&semaforo);

    pthread_exit(NULL);
    return NULL;

}

void extracaoGravacaoSemaforo(){
    FILE* fp;
    FILE* fp2;

    char nomeFicheiro[50];
    int coluna1,coluna2,coluna3;
    int nRepeticoes;

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    fp = fopen("PL_Programacao_05_Sinc.csv", "w");

    if(fp != NULL)
    {
        printf("Geração do ficheiro PL_Programacao_05_Sinc.csv efutuada com sucesso\n");

        printf("Nome do ficheiro: ");scanf("%s",nomeFicheiro);
        
        fp2 = fopen(nomeFicheiro,"r");

        if(fp2 != NULL)
        {
            printf("Indique o numero das 3 colunas: ");
            scanf("%d %d %d",&coluna1, &coluna2, &coluna3);

            printf("Indique o numero de repeticoes: ");
            scanf("%d",&nRepeticoes);

            Thread thread1Estrutura;
            thread1Estrutura.coluna = coluna1;
            thread1Estrutura.repeticoes = nRepeticoes;
            strcpy(thread1Estrutura.nomeFicheiro,nomeFicheiro);

            Thread thread2Estrutura;
            thread2Estrutura.coluna = coluna2;
            thread2Estrutura.repeticoes = nRepeticoes;
            strcpy(thread2Estrutura.nomeFicheiro,nomeFicheiro);

            Thread thread3Estrutura;
            thread3Estrutura.coluna = coluna3;
            thread3Estrutura.repeticoes = nRepeticoes;
            strcpy(thread3Estrutura.nomeFicheiro,nomeFicheiro);

            sem_init(&semaforo, 0, 0);

            pthread_create(&thread1,NULL,thread1ExtracaoSemaforo,&thread1Estrutura);
            pthread_create(&thread2,NULL,thread2ExtracaoSemaforo,&thread2Estrutura);
            pthread_create(&thread3,NULL,thread3ExtracaoSemaforo,&thread3Estrutura);
            
            sem_wait(&semaforo);
            sem_wait(&semaforo);
            sem_wait(&semaforo);
          
        }
        else
        {
            printf("O ficheiro nao existe");
        }
    }
    else
    {
        printf("O ficheiro nao existe");
    }
}

void Ficha5()
{
    int opcao = 0;

    do
    {
        printf("Menu ficha 5\n");
        printf("1-Extracao e gravaçao de colunas (pthread_mutex)\n");
        printf("2-Extracao e gracacao de colunas (sem)\n");
        printf("3-Sair\n");
        printf("Opcao: ");
        scanf("%d",&opcao);

        switch(opcao)
        {
            case 1:
                extracaoGravacaoMutex();
                break;
            case 2:
                extracaoGravacaoSemaforo();
                break;
            case 3: 
                printf("A sair....\n");
                    break;
            default:    
                printf("Opcao invalida\n");
                break;
        }
    }while(opcao != 3);
}

void main()
{
    int opcao;
    do 
    {
        printf("Menu\n");
        printf("1.Ficha1\n");
        printf("2.Ficha2\n");
        printf("3.Ficha3\n");
        printf("4.Ficha4\n");
        printf("5.Ficha5\n");
        printf("0. Sair\n");

        printf("\nDigite sua opção: ");
        scanf("%d", &opcao);

        switch (opcao) 
        {
            case 1:
                Ficha1();
                break;
            case 2:
                Ficha2();
                break;
            case 3:
                Ficha3();
                break;
            case 4:
                Ficha4();
                break;
            case 5:
                Ficha5();
                break;
            case 0:
                printf("\nSaindo do menu...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);
}