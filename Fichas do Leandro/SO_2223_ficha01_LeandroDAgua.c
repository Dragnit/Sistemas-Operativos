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

int sinalRecebido = 0;

void listagemProcessos(){
    int opcao;
    do{

        printf("Listagem de Processo\n");
        printf("1-Listagem de Processos\n");
        printf("2-Listagem de Processos com parâmetros definidos pelo utilizador\n");
        printf("3-Listagem de Processos á escolha do utilizador\n");
        printf("4-Listagem de Processos em tempo real\n");
        printf("Opcao: ");scanf("%d",&opcao);

        switch(opcao){
            case 1: system("ps aux");break;

            case 2: char string[10];
                    printf("Introduza os parametros: ");scanf("%s",&string);
                    char stringToda[20] = "ps ";
                    strcat(stringToda,string);
                    system(stringToda);break;

            case 3: char string1[10];
                    printf("Introduza a string: ");scanf("%s",&string1);
                    char stringToda1[20] = "ps aux | grep ";
                    strcat(stringToda1,string1);
                    system(stringToda1);break;

            case 4: execlp("top",NULL);break;

            default: printf("Opcao invalida");break;
        }

    }while(!(opcao == 1 || opcao == 2 || opcao == 3 || opcao == 4));
}

void consultarPID(){
    int opcao;
    do{
        printf("Consultar PIDs\n");
        printf("1-PID do processo atual\n");
        printf("2-PID do processo pai\n");
        printf("Opcao: ");scanf("%d",&opcao);

        switch(opcao){
            case 1:printf("Processo atual: %d",getpid());break;
            case 2:printf("Processo pai: %d",getppid());break;
        }
    }while(!(opcao == 1 || opcao == 2));
}

void estatistica(){
    char nomeFicheiro[50];
    char *resultado;
    char buffer[1024];
    int linha = 1;
    FILE *file;

    char *token;
    int indiceColuna = -1;
    int i = 0;
    char nome[20];
    
    int valor = 0;
    int n = 0;
    int media = 0;
    int variancia = 0;

    printf("Indique o nome do ficheiro: ");scanf("%s",&nomeFicheiro);
    //strcpy(nomeFicheiro,"PL_22-23_Programacao_00_DB.csv");
    file = fopen(nomeFicheiro,"r");
    if(file == NULL){
        printf("O ficheiro %s não existe.",nomeFicheiro);
    }
    else{
        printf("Indique o nome da coluna: ");scanf("%s",&nome);
        //strcpy(nome,"Age");
        fgets(buffer,1024,file);//Le a primeira linha
        //printf("Colunas: \n%s",buffer);
        token = strtok(buffer, ",");
        while(token != NULL){
            if(strcmp(token,nome) == 0){
                indiceColuna = i;
                break;
            }
            i++;
            token = strtok(NULL,",");
        }
        if(indiceColuna == -1){
            printf("A coluna %s não existe",nome);
        }
        else{//ler cada linha
            //printf("%d",indiceColuna);
            while(fgets(buffer,100,file)){
                token = strtok(buffer,",");
                i = 0;
                while(token != NULL && i < indiceColuna){
                    token = strtok(NULL,",");
                    i++;
                }
                if(token != NULL){
                    valor = atoi(token);
                    n++;
                    media += valor;
                    variancia += valor * valor;
                }
            }
            
            if(n>0){
                media /= n;
                variancia = variancia / n - (media*media);
                printf("Media da coluna %s: %d\n",nome,media);
                printf("Variancia da coluna %s: %d\n",nome,variancia);
            }
            else{
                printf("Nao foi possivel cacular a media");
            }

            fclose(file);
            //printf("%d",valor);
        }
    }
}

void estatisticaAwk(){
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
    //strcpy(nomeFicheiro,"PL_22-23_Programacao_00_DB.csv");
    file = fopen(nomeFicheiro,"r");
    if(file == NULL){
        printf("O ficheiro %s não existe.",nomeFicheiro);
    }
    else{
        printf("Indique o nome da coluna: ");scanf("%s",&nome);
        //strcpy(nome,"Age");
        fgets(buffer,1024,file);//Le a primeira linha
        //printf("Colunas: \n%s",buffer);
        token = strtok(buffer, ",");
        while(token != NULL){
            if(strcmp(token,nome) == 0){
                indiceColuna = i;
                break;
            }
            i++;
            token = strtok(NULL,",");
        }
        if(indiceColuna == -1){
            printf("A coluna %s não existe",nome);
        }
        else{
            sprintf(comando, "awk -F',' '{ valor = strtonum($%d); print valor }' %s > temp.txt",indiceColuna+1,nomeFicheiro);
            system(comando);

            int n = 0;
            float soma = 0;
            float valor = 0;
            float var = 0;

            fileTemp = fopen("temp.txt","r");
            if(fileTemp == NULL){
                printf("Erro ao abrir ao ficheiro");
            }
            else{
                while (fscanf(fileTemp, "%f", &valor) == 1) {
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

void mediaVarianciaProcessos(){
    char nomeFicheiro[50];
    char nome[20];
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

    printf("Indique o nome do ficheiro: ");scanf("%s",&nomeFicheiro);
    //strcpy(nomeFicheiro,"PL_22-23_Programacao_00_DB.csv");
    file = fopen(nomeFicheiro,"r");
    if(file == NULL){
        printf("O ficheiro %s não existe.",nomeFicheiro);
    }
    else{
        printf("Indique o nome da coluna: ");scanf("%s",&nome);
        //strcpy(nome,"Education_Num");
        fgets(buffer,1024,file);//Le a primeira linha
        //printf("Colunas: \n%s",buffer);
        token = strtok(buffer, ",");
        while(token != NULL){
            if(strcmp(token,nome) == 0){
                indiceColuna = i;
                break;
            }
            i++;
            token = strtok(NULL,",");
        }
        if(indiceColuna == -1){
            printf("A coluna %s não existe",nome);
        }
        else{
            filho1 = fork();
            if(filho1 == 0){//processo do 1 filho
                FILE* file_dat = fopen("hist.dat","w");
                if(file_dat == NULL){
                    printf("o ficheiro nao existe");
                }
                else{
                    while(fgets(buffer,100,file)){
                        token = strtok(buffer,",");
                        i = 0;
                        while(token != NULL && i < indiceColuna){
                            token = strtok(NULL,",");
                            i++;
                        }
                        if(token != NULL){
                            valor = atoi(token);
                            fprintf(file_dat,"%d\n",valor);
                            n++;
                            media += valor;
                            variancia += valor * valor;
                        }
                    }
                    fclose(file_dat);
                
                    if(n>0){
                        media /= n;
                        //printf("\nMedia: %d\n",media);
                        variancia = variancia / n - (media*media);
                        //printf("\nVariancia: %d\n",variancia);
                        char comando[50] = "";
                        sprintf(comando,"echo media: %d, variancia: %d >> ficha02_media.dat",media,variancia);
                        //printf("%s",comando);
                        system(comando);

                        printf("Tarefa executada com sucesso pelo filho1: processo %d\n",getpid());
                        printf("Pressione qualquer tecla para prosseguir.....");
                        fflush(stdin);
                        getchar();

                        fclose(file);
                        /*system("gnuplot -e \"plot 'ficha02_media.dat' using 1:2 lines title 'TITULO',
                         \'ficha02_media.dat' using 1:2 with lines title 'BRO',
                         \'ficha02_media.dat' using 1:2 with lines title 'AAAAAAAA'; pause mouse;\"");
                        */
                    }
                    else{
                        printf("Nao foi possivel cacular a media");
                    }
                    kill(getpid(),SIGSEGV);
                }
            }
            else{//processo do pai
                wait(NULL);
                filho2 = fork();
                if(filho2 == 0){//processos do 2 filho
                    
                    time_t tempo;
                    struct tm * tempoinfo;
                    time(&tempo);
                    tempoinfo = localtime(&tempo);
                    
                    int day = tempoinfo->tm_mday;
                    int month = tempoinfo->tm_mon +1;
                    int year = tempoinfo->tm_year + 1960;
                    //printf("Data atual: %02d/%02d/%02d\n",day,month,year);

                    char comando[250] = "";
                    sprintf(comando,"echo %02d/%02d/%02d Operação realizada com sucesso! >> ficha02_sucesso.dat",day,month,year);
                    system(comando);

                    FILE * file_media = fopen("ficha02_media.dat","r");
                    FILE * file_sucesso = fopen("ficha02_sucesso.dat","r");

                    if(file_media == NULL && file_sucesso == NULL){
                        printf("Deu erro no ficheiro");
                    }
                    else{
                        char conteudo1[1000],conteudo2[1000];
                        printf("Conteudo do ficheiro ficha02_media.dat: \n");
                       
                        while(fgets(conteudo1,1000,file_media)){
                            printf("%s\n",conteudo1);
                        }
                        printf("Conteudo do ficheiro ficha02_sucesso.dat: \n");
                        while(fgets(conteudo2,1000,file_sucesso)){
                            printf("%s\n",conteudo2);
                        }

                        fclose(file_media);
                        fclose(file_sucesso);
                        printf("Tarefa executada com sucesso pelo filho2: processo %d\n",getpid());
                        printf("Pressione qualquer tecla para prosseguir.....");
                        fflush(stdin);
                        getchar();
                    }
                    //acabar o processo
                    kill(getpid(),SIGSEGV);
                }
                else{//processo do pai         
                    wait(NULL);          
                    printf("Tarefa executada com sucesso pelo pai: processo %d\n",getpid());
                }
            }
        }
    }
}

void apagarFicheirosResultados(){
    int filho1,filho2;

    filho1 = fork();

    if(filho1 == 0){//processo do filho 1
        if(remove("ficha02_media.dat") == 0){
            printf("Removido o ficheiro 'ficha02_media.dat'\n");
        }
        else
            printf("Nao foi possicvel remover o ficheiro 'ficha02_media.dat'\n");
        kill(getpid(),SIGSEGV);
    }
    else{
        wait(NULL);
        filho2 = fork();
        if(filho2 == 0){//processo do filho 2
            if(remove("ficha02_sucesso.dat") == 0){
                printf("Removido o ficheiro 'ficha02_sucesso.dat'\n");
            }
            else
                printf("Nao foi possivel remocer o ficheiro 'ficha02_sucesso.dat'\n");
            kill(getpid(),SIGSEGV);
        }
        else{//processo do pai
            wait(NULL);
            printf("Os processos filhos terminaram!\n");
            printf("Pressione qualquer tecla para prosseguir.....");
            fflush(stdin);
            getchar();
        }
    }
    //printf("%d",getpid());
}

void menu1(){
    int opcao = -1;

    do{
        printf("Menu da ficha1\n");
        printf("1-Listagem de processos\n");
        printf("2-Consultar PIDs\n");
        printf("3-Estatistica Descritiva (Mediana e variancia)\n");
        printf("4-Estatistica Descritiva (Mediana e variancia) com awk\n");
        printf("5-Sair\n");
        printf("Opção: ");
        scanf("%d",&opcao);
        switch(opcao){
            case 1: listagemProcessos();break;
            case 2: consultarPID();break;
            case 3: estatistica();break;
            case 4: estatisticaAwk();break;
            case 5: printf("A sair......\n");break;
            default: printf("Opcao invalida");break;
        }
    }while(opcao != 5);
}

void menu2(){
    int opcao = -1;

    do{
        printf("Menu da ficha 2\n");
        printf("1-Cálculo da media e variancia\n");
        printf("2-Apagar ficheiros de resultados\n");
        printf("3-Sair\n");
        printf("Opcao: ");
        scanf("%d",&opcao);
        switch(opcao){
            case 1: mediaVarianciaProcessos();break;
            case 2: apagarFicheirosResultados();break;
            case 3: printf("A sair......\n");break;
            default:printf("Opcao invalida\n");break;
        }
    }while(opcao != 3);
}

void sigusr1_handler_filho1(int signal){
    printf("Sinal SIGUSR1 recebido pelo processo com PID %d\n\n",getpid());

    //recebeu o sinal agora fazer o resto
    char nomeFicheiro[50];
    char nome[20];
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

    FILE *fp;

    printf("Indique o nome do ficheiro: ");scanf("%s",&nomeFicheiro);
    //strcpy(nomeFicheiro,"PL_22-23_Programacao_00_DB.csv");
    file = fopen(nomeFicheiro,"r");
    if(file == NULL){
        printf("O ficheiro %s não existe.",nomeFicheiro);
    }
    else{
        printf("Indique o nome da coluna: ");scanf("%s",&nome);
        //strcpy(nome,"Education_Num");
        fgets(buffer,1024,file);//Le a primeira linha
        //printf("Colunas: \n%s",buffer);
        token = strtok(buffer, ",");
        while(token != NULL){
            if(strcmp(token,nome) == 0){
                indiceColuna = i;
                break;
            }
            i++;
            token = strtok(NULL,",");
        }
        if(indiceColuna == -1){
            printf("A coluna %s não existe",nome);
        }
        else{
            FILE * apagarColunas;
            apagarColunas = fopen("ficha03_coluna.dat","w");
            fclose(apagarColunas);
            fp = fopen("ficha03_coluna.dat","w+");
            while(fgets(buffer,100,file)){

                token = strtok(buffer,",");
                i = 0;
                while(token != NULL && i < indiceColuna){
                    token = strtok(NULL,",");
                    i++;
                }
                if(token != NULL){
                    valor = atoi(token);
                    n++;
                    media += valor;
                    variancia += valor * valor;

                    fprintf(fp,"%d\n",valor);
                }
            }
            fclose(fp);
            
        
            if(n>0){
                media /= n;
                variancia = variancia / n - (media*media);
                char comando[200] = "";
                sprintf(comando,"echo media: %d, variancia: %d >> ficha03_media.dat");
                system(comando);

                time_t tempo;
                struct tm * tempoinfo;
                time(&tempo);
                tempoinfo = localtime(&tempo);
                
                int day = tempoinfo->tm_mday;
                int month = tempoinfo->tm_mon +1;
                int year = tempoinfo->tm_year + 1960;
                //printf("Data atual: %02d/%02d/%02d\n",day,month,year);

                char comando1[250] = "";
                sprintf(comando1,"echo %02d/%02d/%02d Operação realizada com sucesso! >> ficha03_sucesso.dat",day,month,year);
                system(comando1);

                printf("Operações realizadas pelo processo filho A (PID %d) com sucesso!\n\n",getpid());
                printf("Pression uma tecla para continuar\n");
                fflush(stdin);
                getchar();
            }
        }
    }

    printf("Sinal USR1 enviado pelo processo filho A (PID %d) ao processo pai (PID %d).\n\n",getpid(),getppid());

    kill(getppid(),SIGUSR1);

}
void sigusr2_handler_filho1(int signal){
    printf("Sinal SIGUSR2 recebido pelo processo A com PID %d\n\n",getpid());

    printf("Pression uma tecla para continuar\n");
    fflush(stdin);
    getchar();

    char comando [200] = "";
    sprintf(comando,"cat ficha03_media.dat");
    printf("Conteudo do ficheiro ficha03_media.dat: \n");
    system(comando);

    strcpy(comando,"");
    sprintf(comando,"cat ficha03_sucesso.dat");
    printf("\nConteudo do ficheiro ficha03_sucesso.dat: \n");
    system(comando);

    printf("Pression uma tecla para continuar\n");
    fflush(stdin);
    getchar();

    kill(getppid(),SIGUSR2);
    printf("Sinal SIGUSR2 enviado pelo processo filho A (PID %d) ao processo pai (PID %d)\n\n",getpid(),getppid());
}

void sigusr1_handler_pai(int signal){
    printf("Sinal SIGUSR1 recebido pelo processo pai (PID %d), vou enviar de seguida o sinal SIGUSR2 ao processo filho A (PID %d) para geração do histograma\n\n",getpid(),getppid());

    printf("Pression uma tecla para continuar\n");
    fflush(stdin);
    getchar();
}
void sigusr2_handler_pai(int signal){
    printf("Sinal SIGUSR2 recebido pelo processo pai (PID %d), proveniente do processo filho A (PID %d). Retornando para o submenu da ficha3.\n\n",getpid(),getppid());

    printf("Pression uma tecla para continuar\n");
    fflush(stdin);
    getchar();
}

void sigterm_handler_filho2(int signal){
    printf("Sinal SIGTERM recebido pelo processo B (PID %d). Irei de seguida apagar os ficheiros .dat\n\n",getpid());

    printf("Pression uma tecla para continuar\n");
    fflush(stdin);
    getchar();

    if(remove("ficha03_media.dat") == 0 && 
        remove("ficha03_sucesso.dat") == 0 &&
        remove("ficha03_coluna.dat") == 0){
            printf("Eliminação de ficheiros .dat com sucesso realizados pelo processo filho B (PID %d)!\n\n",getpid());
    }

    printf("Pression uma tecla para continuar\n");
    fflush(stdin);
    getchar();

    printf("Sinal SIGTERM enviado pelo processo filho B (PID %d) ao processo pai (PID %d).\n\n",getpid(),getppid());
    kill(SIGTERM,getppid());
}
void sigterm_handler_pai(int signal){
    printf("Sinal SIGTERM recebido pelo pai (PID %d), retornando para o submenu da ficha 3.\n\n");
    
    printf("Pression uma tecla para continuar\n");
    fflush(stdin);
    getchar();
}

void sigkill_filho1(int signal){
    kill(getpid(),SIGTERM);
}
void sigkill_filho2(int signal){
    kill(getpid(),SIGTERM);
}


void menu3(){
    int filho1,filho2,pai = getpid();
    int opcao = -1;

    filho1 = fork();
    if(filho1 == 0){
        //signal(SIGKILL,sigkill_filho1);

        signal(SIGUSR1,sigusr1_handler_filho1);

        pause();

        signal(SIGUSR2,sigusr2_handler_filho1);

        pause();
    }else{

        filho2 = fork();
        if(filho2 == 0){
            //signal(SIGKILL,sigkill_filho2);

            signal(SIGTERM,sigterm_handler_filho2);

            pause();

        }else{
            printf("Menu da ficha 3\n");
            printf("1-Calculo da media e variancia\n");
            printf("2-Apagar ficheiros de resultados\n");
            printf("3-Sair\n");

            printf("Sou o processo pai (PID %d), e criei os processos A (PID %d) e B (PID %d).\n",pai,filho1,filho2);

            printf("Opcao: ");
            scanf("%d",&opcao);

            if(opcao == 1){
                printf("Sinal SIGUSR1 enviado pelo processo pai (PID %d) ao processo filho A (PID %d).\n",pai,filho1);
                kill(filho1,SIGUSR1);

                signal(SIGUSR1,sigusr1_handler_pai);

                pause();

                printf("Sinal SIGUSR2 enviado pelo processo pai (PID %d) ao processo filho A (PID %d).\n",pai,filho1);
                kill(filho1,SIGUSR2);

                signal(SIGUSR2,sigusr2_handler_pai);

                pause();
            }else if(opcao == 2){
                printf("Sinal SIGTERM enviado pelo processo pai (PID %d) ao processo B (PID %d).\n\n",pai,filho2);
                kill(filho2,SIGTERM);
                
                signal(SIGTERM,sigterm_handler_pai);

                pause();                
            }else if(opcao == 3){
                printf("Sinal SIGKILL enviado com PIDs %d, e %d, retornando para o menu principal.\n\n",filho1,filho2);
                kill(filho1,SIGKILL);
                kill(filho2,SIGKILL);
                printf("Pression uma tecla para continuar\n");
                fflush(stdin);
                getchar();

            }else{
                printf("Opcao invalida\n");
            }
            
        }
    }
}

void* transformacaoColuna(){
    char nomeFicheiro[50];
    int numColuna;
    int numColunaAtual;
    int numRepeticoes;

    char buffer[1000];
    FILE* file;
    FILE* fileInv;

    int estado = 1;


    printf("Indique o nome do ficheiro: ");scanf("%s",&nomeFicheiro);
    //strcpy(nomeFicheiro,"PL_22-23_Programacao_00_DB.csv");
    file = fopen(nomeFicheiro,"r");
    if(file == NULL){
        printf("O ficheiro %s não existe.",nomeFicheiro);
    }
    else{
        printf("Indique o numero da coluna: ");scanf("%d",&numColuna);
        //numColuna = 1;

        printf("Indique o numero de repeticoes: ");scanf("%d",&numRepeticoes);
        //numRepeticoes = 4;

        char nomeFicheiroInv[50] = "a.csv";
        //printf(nomeFicheiroInv, "%s_inv", nomeFicheiro);

        fileInv = fopen(nomeFicheiroInv, "w");
        if (fileInv != NULL) {
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), file) != NULL){
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == numColuna){
                        for(int i=0;i<numRepeticoes;i++){
                            fprintf(fileInv,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }

            fclose(fileInv);
            printf("Transformação concluída. Ficheiro %s criado com sucesso.\n", nomeFicheiroInv);

            printf("Pressione qualquer tecla para continuar...\n");
            getchar();
            getchar();
        }else{
            printf("O ficheiro nao existe");
        }


        fclose(file);
    }

    pthread_exit(NULL);

}

typedef struct {
    int coluna;
    int repeticoes;
    char nomeFicheiro[50];
} ThreadEstrutura;

void* thread1Converter(void* arg){
    FILE* file;
    FILE* filePrin;
    file = fopen("PL_Programacao_04_Threads.csv","a");
    if(file != NULL){
        ThreadEstrutura* args = (ThreadEstrutura*)arg;
        int coluna = args->coluna;
        int repeticoes = args->repeticoes;
        char nomeFicheiro[50];
        strcpy(nomeFicheiro,args->nomeFicheiro);

        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), filePrin) != NULL){
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == coluna){
                        for(int i=0;i<repeticoes;i++){
                            fprintf(file,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }else{
            printf("Nao foi possivel abrir o ficheiro");
        }
        
        fclose(file);
        printf("Thread 1 do processo %d escreveu com sucesso no ficheiro PL_Programacao_04_Threads.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }

    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
    pthread_exit(NULL);
    return NULL;
}
void* thread2Converter(void* arg){
    FILE* file;
    FILE* filePrin;
    file = fopen("PL_Programacao_04_Threads.csv","a");
    if(file != NULL){
        ThreadEstrutura* args = (ThreadEstrutura*)arg;
        int coluna = args->coluna;
        int repeticoes = args->repeticoes;
        char nomeFicheiro[50];
        strcpy(nomeFicheiro,args->nomeFicheiro);

        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), filePrin) != NULL){
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == coluna){
                        for(int i=0;i<repeticoes;i++){
                            fprintf(file,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }else{
            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(file);
        printf("Thread 2 do processo %d escreveu com sucesso no ficheiro PL_Programacao_04_Threads.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();

    pthread_exit(NULL);
    return NULL;
}
void* thread3Converter(void* arg){
    FILE* file;
    FILE* filePrin;
    file = fopen("PL_Programacao_04_Threads.csv","a");
    if(file != NULL){
        ThreadEstrutura* args = (ThreadEstrutura*)arg;
        int coluna = args->coluna;
        int repeticoes = args->repeticoes;
        char nomeFicheiro[50];
        strcpy(nomeFicheiro,args->nomeFicheiro);

        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), filePrin) != NULL){
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == coluna){
                        for(int i=0;i<repeticoes;i++){
                            fprintf(file,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }else{
            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(file);
        printf("Thread 3 do processo %d escreveu com sucesso no ficheiro PL_Programacao_04_Threads.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();

    pthread_exit(NULL);
    return NULL;
}

void ExtracaoColuna(){
    FILE* file;
    FILE* filePrin;
    char nomeFicheiro[50];
    int col1,col2,col3;
    int numRepeticoes;

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    file = fopen("PL_Programacao_04_Threads.csv", "w");
    if(file != NULL){
        printf("Geração do ficheiro PL_Programacao_04_Threads.csv efetuada com sucesso\n");

        printf("Nome do ficheiro: ");scanf("%s",nomeFicheiro);
        //strcpy(nomeFicheiro,"PL_22-23_Programacao_00_DB.csv");
        
        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            printf("Indique o nome da primeira coluna: ");scanf("%d",&col1);
            printf("Indique o nome da segunda coluna: ");scanf("%d",&col2);
            printf("Indique o nome da terceira coluna: ");scanf("%d",&col3);

            //col1 = 1;
            //col2 = 4;
            //col3 = 6;

            printf("Indique o numero de repeticoes: ");scanf("%d",&numRepeticoes);
            //numRepeticoes = 2;
            

            ThreadEstrutura thread1Estrutura;
            thread1Estrutura.coluna = col1;
            thread1Estrutura.repeticoes = numRepeticoes;
            strcpy(thread1Estrutura.nomeFicheiro,nomeFicheiro);

            ThreadEstrutura thread2Estrutura;
            thread2Estrutura.coluna = col2;
            thread2Estrutura.repeticoes = numRepeticoes;
            strcpy(thread2Estrutura.nomeFicheiro,nomeFicheiro);

            ThreadEstrutura thread3Estrutura;
            thread3Estrutura.coluna = col3;
            thread3Estrutura.repeticoes = numRepeticoes;
            strcpy(thread3Estrutura.nomeFicheiro,nomeFicheiro);


            pthread_create(&thread1,NULL,thread1Converter,&thread1Estrutura);
            pthread_join(thread1, NULL);
            pthread_create(&thread2,NULL,thread2Converter,&thread2Estrutura);
            pthread_join(thread2, NULL);
            pthread_create(&thread3,NULL,thread3Converter,&thread3Estrutura);
            pthread_join(thread3, NULL);

            

        }else{
            printf("O ficheiro nao existe");
        }


        fclose(file);
    }else{
        printf("O ficheiro nao existe");
    }
    
}

void menu4(){
    int opcao = -1;

    pthread_t transformacao;

    do{
        printf("Menu ficha 4\n");
        printf("1-Transformação de coluna em linha com repetições\n");
        printf("2-Extração e gravação de colunas\n");
        printf("3-Sair\n");
        printf("Opcao: ");
        scanf("%d",&opcao);

        switch(opcao){
            case 1: pthread_create(&transformacao,NULL,transformacaoColuna,NULL);
            pthread_join(transformacao, NULL);
            break;
            case 2:ExtracaoColuna();break;
            case 3:printf("A sair.....\n");break;
            default:printf("Opção invalida\n");break;
        }
    }while(opcao!= 3);
}


pthread_mutex_t mutex;

void* thread1Extracao(void *arg){

    pthread_mutex_lock(&mutex);

    FILE* file;
    FILE* filePrin;
    file = fopen("PL_Programacao_05_Sinc.csv","a");
    if(file != NULL){
        ThreadEstrutura* args = (ThreadEstrutura*)arg;
        int coluna = args->coluna;
        int repeticoes = args->repeticoes;
        char nomeFicheiro[50];
        strcpy(nomeFicheiro,args->nomeFicheiro);

        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), filePrin) != NULL){
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == coluna){
                        for(int i=0;i<repeticoes;i++){
                            fprintf(file,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }else{
            printf("Nao foi possivel abrir o ficheiro");
        }
        
        fclose(file);
        printf("Thread 1 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }

    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
    return NULL;

}
void* thread2Extracao(void *arg){
    
    pthread_mutex_lock(&mutex);

    FILE* file;
    FILE* filePrin;
    file = fopen("PL_Programacao_05_Sinc.csv","a");
    if(file != NULL){
        ThreadEstrutura* args = (ThreadEstrutura*)arg;
        int coluna = args->coluna;
        int repeticoes = args->repeticoes;
        char nomeFicheiro[50];
        strcpy(nomeFicheiro,args->nomeFicheiro);

        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), filePrin) != NULL){
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == coluna){
                        for(int i=0;i<repeticoes;i++){
                            fprintf(file,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }else{
            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(file);
        printf("Thread 2 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
    return NULL;

}
void* thread3Extracao(void *arg){
    
    pthread_mutex_lock(&mutex);

    FILE* file;
    FILE* filePrin;
    file = fopen("PL_Programacao_05_Sinc.csv","a");
    if(file != NULL){
        ThreadEstrutura* args = (ThreadEstrutura*)arg;
        int coluna = args->coluna;
        int repeticoes = args->repeticoes;
        char nomeFicheiro[50];
        strcpy(nomeFicheiro,args->nomeFicheiro);

        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), filePrin) != NULL){
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == coluna){
                        for(int i=0;i<repeticoes;i++){
                            fprintf(file,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }else{
            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(file);
        printf("Thread 3 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
    return NULL;

}

void extracaoGravacaoMutex(){
    FILE* file;
    FILE* filePrin;
    char nomeFicheiro[50];
    int col1,col2,col3;
    int numRepeticoes;

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    file = fopen("PL_Programacao_05_Sinc.csv", "w");
    if(file != NULL){
        printf("Geração do ficheiro PL_Programacao_05_Sinc.csv efutuada com sucesso\n");

        printf("Nome do ficheiro: ");scanf("%s",nomeFicheiro);
        //strcpy(nomeFicheiro,"PL_22-23_Programacao_00_DB.csv");
        
        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            printf("Indique o nome da primeira coluna: ");scanf("%d",&col1);
            printf("Indique o nome da segunda coluna: ");scanf("%d",&col2);
            printf("Indique o nome da terceira coluna: ");scanf("%d",&col3);

            //col1 = 1;
            //col2 = 4;
            //col3 = 6;

            printf("Indique o numero de repeticoes: ");scanf("%d",&numRepeticoes);
            //numRepeticoes = 2;

            ThreadEstrutura thread1Estrutura;
            thread1Estrutura.coluna = col1;
            thread1Estrutura.repeticoes = numRepeticoes;
            strcpy(thread1Estrutura.nomeFicheiro,nomeFicheiro);

            ThreadEstrutura thread2Estrutura;
            thread2Estrutura.coluna = col2;
            thread2Estrutura.repeticoes = numRepeticoes;
            strcpy(thread2Estrutura.nomeFicheiro,nomeFicheiro);

            ThreadEstrutura thread3Estrutura;
            thread3Estrutura.coluna = col3;
            thread3Estrutura.repeticoes = numRepeticoes;
            strcpy(thread3Estrutura.nomeFicheiro,nomeFicheiro);

            pthread_mutex_init(&mutex, NULL);

            pthread_create(&thread1,NULL,thread1Extracao,&thread1Estrutura);
            pthread_join(thread1, NULL);
            pthread_create(&thread2,NULL,thread2Extracao,&thread2Estrutura);
            pthread_join(thread2, NULL);
            pthread_create(&thread3,NULL,thread3Extracao,&thread3Estrutura);
            pthread_join(thread3, NULL);
          
        }else{
            printf("O ficheiro nao existe");
        }
    }else{
        printf("O ficheiro nao existe");
    }

}


sem_t semaphore;

void* thread1ExtracaoSem(void *arg){
    FILE* file;
    FILE* filePrin;
    file = fopen("PL_Programacao_05_Sinc.csv","a");
    if(file != NULL){
        ThreadEstrutura* args = (ThreadEstrutura*)arg;
        int coluna = args->coluna;
        int repeticoes = args->repeticoes;
        char nomeFicheiro[50];
        strcpy(nomeFicheiro,args->nomeFicheiro);

        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), filePrin) != NULL){
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == coluna){
                        for(int i=0;i<repeticoes;i++){
                            fprintf(file,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }else{
            printf("Nao foi possivel abrir o ficheiro");
        }
        
        fclose(file);
        printf("Thread 1 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }

    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();

    sem_post(&semaphore);

    pthread_exit(NULL);
    return NULL;

}
void* thread2ExtracaoSem(void *arg){
    FILE* file;
    FILE* filePrin;
    file = fopen("PL_Programacao_05_Sinc.csv","a");
    if(file != NULL){
        ThreadEstrutura* args = (ThreadEstrutura*)arg;
        int coluna = args->coluna;
        int repeticoes = args->repeticoes;
        char nomeFicheiro[50];
        strcpy(nomeFicheiro,args->nomeFicheiro);

        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), filePrin) != NULL){
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == coluna){
                        for(int i=0;i<repeticoes;i++){
                            fprintf(file,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }else{
            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(file);
        printf("Thread 2 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();

    sem_post(&semaphore);

    pthread_exit(NULL);
    return NULL;

}
void* thread3ExtracaoSem(void *arg){
    FILE* file;
    FILE* filePrin;
    file = fopen("PL_Programacao_05_Sinc.csv","a");
    if(file != NULL){
        ThreadEstrutura* args = (ThreadEstrutura*)arg;
        int coluna = args->coluna;
        int repeticoes = args->repeticoes;
        char nomeFicheiro[50];
        strcpy(nomeFicheiro,args->nomeFicheiro);

        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            char linhas[1000];

            while(fgets(linhas,sizeof(linhas), filePrin) != NULL){
                char *token = strtok(linhas, ",");
                int count = 1;
                while(token != NULL){
                    if(count == coluna){
                        for(int i=0;i<repeticoes;i++){
                            fprintf(file,"%s\n",token);
                        }
                        break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            }
        }else{
            printf("Nao foi possivel abrir o ficheiro");
        }
        fclose(file);
        printf("Thread 3 do processo %d escreveu com sucesso no ficheiro PL_Programacao_05_Sinc.csv, a coluna %d repetida %d vezes.\n",getpid(),coluna,repeticoes);
    }
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();

    sem_post(&semaphore);

    pthread_exit(NULL);
    return NULL;

}

void extracaoGravacaoSem(){
    FILE* file;
    FILE* filePrin;
    char nomeFicheiro[50];
    int col1,col2,col3;
    int numRepeticoes;

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    file = fopen("PL_Programacao_05_Sinc.csv", "w");
    if(file != NULL){
        printf("Geração do ficheiro PL_Programacao_05_Sinc.csv efutuada com sucesso\n");

        printf("Nome do ficheiro: ");scanf("%s",nomeFicheiro);
        //strcpy(nomeFicheiro,"PL_22-23_Programacao_00_DB.csv");
        
        filePrin = fopen(nomeFicheiro,"r");
        if(filePrin != NULL){
            printf("Indique o nome da primeira coluna: ");scanf("%d",&col1);
            printf("Indique o nome da segunda coluna: ");scanf("%d",&col2);
            printf("Indique o nome da terceira coluna: ");scanf("%d",&col3);

            //col1 = 1;
            //col2 = 4;
            //col3 = 6;

            printf("Indique o numero de repeticoes: ");scanf("%d",&numRepeticoes);
            //numRepeticoes = 2;

            ThreadEstrutura thread1Estrutura;
            thread1Estrutura.coluna = col1;
            thread1Estrutura.repeticoes = numRepeticoes;
            strcpy(thread1Estrutura.nomeFicheiro,nomeFicheiro);

            ThreadEstrutura thread2Estrutura;
            thread2Estrutura.coluna = col2;
            thread2Estrutura.repeticoes = numRepeticoes;
            strcpy(thread2Estrutura.nomeFicheiro,nomeFicheiro);

            ThreadEstrutura thread3Estrutura;
            thread3Estrutura.coluna = col3;
            thread3Estrutura.repeticoes = numRepeticoes;
            strcpy(thread3Estrutura.nomeFicheiro,nomeFicheiro);

            sem_init(&semaphore, 0, 0);

            pthread_create(&thread1,NULL,thread1ExtracaoSem,&thread1Estrutura);
            sem_wait(&semaphore);
            pthread_create(&thread2,NULL,thread2ExtracaoSem,&thread2Estrutura);
            sem_wait(&semaphore);
            pthread_create(&thread3,NULL,thread3ExtracaoSem,&thread3Estrutura);
            sem_wait(&semaphore);
          
        }else{
            printf("O ficheiro nao existe");
        }
    }else{
        printf("O ficheiro nao existe");
    }
}

void menu5(){
    int opcao = -1;

    do{
        printf("Menu ficha 5\n");
        printf("1-Extracao e gravaçao de colunas (pthread_mutex)\n");
        printf("2-Extracao e gracacao de colunas (sem)\n");
        printf("3-Sair\n");
        printf("Opcao: ");
        scanf("%d",&opcao);

        switch(opcao){
            case 1:extracaoGravacaoMutex();break;
            case 2:extracaoGravacaoSem();break;
            case 3:printf("A sair....\n");break;
            default:printf("Opcao invalida\n");break;
        }
    }while(opcao != 3);
}

void menuTotal(){
    int opcao = -1;

    do{
        printf("Menu total\n");
        printf("1-Ficha 1\n");
        printf("2-Ficha 2\n");
        printf("3-Ficha 3\n");
        printf("4-Ficha 4\n");
        printf("5-Ficha 5\n");
        printf("6-Sair\n");
        printf("Opção: ");
        scanf("%d",&opcao);
        switch(opcao){
            case 1:menu1();break;
            case 2:menu2();break;
            case 3:menu3();break;
            case 4:menu4();break;
            case 5:menu5();break;
            case 6:printf("A sair.....\n");break;
            default:printf("Opção invalida\n");break;
        }
    }while(opcao != 6);
}

int main(){

    menuTotal();


    return 0;
}