#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void segundaF()
{
	int a = 0;
	while (a != 3) // Ciclo para que volte sempre ao menu cada vez que se inserir uma opcao diferente
	{

		//system("clear"); // Limpa o ecra

		puts("-------------------------------------------------------");
		puts("                         MENU                          ");
		puts("                                                       ");
		puts("     O que pretende realizar?                          ");
		puts("                                                       ");
		puts("1 -- Variancia                                         ");
		puts("2 -- Apagar Fichieros                                  ");
		puts("3 -- Voltar                                            ");
		puts("-------------------------------------------------------");

		printf("Insira a opcao: ");
		scanf("%d", &a);
		fflush(stdin);

		//system("clear");

		switch (a)
		{
			case 1: break;
			case 2: break;
			case 3: break;
			default: printf("Opcao invalida\n");
		}
	}
}

void pids()
{
	int a = 0;
	int b = getpid();
	int c = getppid();
	while (a != 3) // Ciclo para que volte sempre ao menu cada vez que se inserir uma opcao diferente
	{

		//system("clear"); // Limpa o ecra

		puts("-------------------------------------------------------");
		puts("                         MENU                          ");
		puts("                                                       ");
		puts("     O que pretende realizar?                          ");
		puts("                                                       ");
		puts("1 -- PID processo atual                                ");
		puts("2 -- PID processo pai                                  ");
		puts("3 -- Voltar                                            ");
		puts("-------------------------------------------------------");

		printf("Insira a opcao: ");
		scanf("%d", &a);
		fflush(stdin);

		//system("clear");

		switch (a)
		{
			case 1: printf("%d\n", b); break;
			case 2: printf("%d\n", c); break;
			case 3: break;
			default: printf("Opcao invalida\n");
		}
	}

	fflush(stdin);
	printf("\nOperacao concluida\7");
	system("pause > nul"); // Espera para que se carregue numa tecla para continuar
}

void processos()
{
	int a = 0;
	char b[50];
	char c[50];
	while (a != 5) // Ciclo para que volte sempre ao menu cada vez que se inserir uma opcao diferente
	{

		//system("clear"); // Limpa o ecra

		puts("-------------------------------------------------------");
		puts("                         MENU                          ");
		puts("                                                       ");
		puts("     O que pretende realizar?                          ");
		puts("                                                       ");
		puts("1 -- Listagem de processos                             ");
		puts("2 -- Processos com parametros inseridos pelo user      ");
		puts("3 -- Processos a escolha do user                       ");
		puts("4 -- Processos em tempo real                           ");
		puts("5 -- Voltar                                            ");
		puts("-------------------------------------------------------");

		printf("Insira a opcao: ");
		scanf("%d", &a);
		fflush(stdin);

		//system("clear");

		switch (a)
		{
			case 1: system("ps aux"); break;
			case 2: puts("Insira os parametros que pretende: ");
							scanf("%s", b);
							sprintf(c,"ps %s", b);
							system(c);
							break;
			case 3: puts("Insira a palavra a pesquisar: ");
							scanf("%s", b);
							sprintf(c,"ps aux | grep %s", b);
							system(c);
							break;
			case 4: system("top");
							exit(0);
							break;
			case 5: break;
			default: printf("Opcao invalida\n");
		}
	}

	fflush(stdin);
	printf("\nOperacao concluida\7");
	system("pause > nul"); // Espera para que se carregue numa tecla para continuar
}

void ficheiro()
{
	char ficheiro[50];
	FILE * fp;
	char coluna[50];

	puts("Insira o nome do ficheiro que pretende abrir: ");
	scanf("%s", ficheiro);
	puts("Insira a coluna a pesquisar: ");
	scanf("%s", coluna);

	fp = fopen(ficheiro, "r");

	if (fp == NULL)
		printf("Nao foi possivel abrir o %s. \n", ficheiro);
	else
	{
		printf("%s aberto com sucesso. \n", ficheiro);
		fclose(fp);
	}

	fclose(fp);
}

void primeiraF()
{
	int a = 0;
	while (a != 5) // Ciclo para que volte sempre ao menu cada vez que se inserir uma opcao diferente
	{

		//system("clear"); // Limpa o ecra

		puts("-------------------------------------------------------");
		puts("                         MENU                          ");
		puts("                                                       ");
		puts("     O que pretende realizar?                          ");
		puts("                                                       ");
		puts("1 -- Listagem de processos                             ");
		puts("2 -- Consultar PIDs                                    ");
		puts("3 -- Estatistica Descritiva (mediana e desvio padrao)  ");
		puts("4 -- Estatistica Descritiva (mediana e desvio padrao) com awk  ");
		puts("5 -- Voltar                                            ");
		puts("-------------------------------------------------------");

		printf("Insira a opcao: ");
		scanf("%d", &a);
		fflush(stdin);

		//system("clear");

		switch (a)
		{
			case 1: processos(); break;
			case 2: pids(); break;
			case 3: ficheiro(); break;
			case 4: ficheiro(); break;
			case 5: break;
			default: printf("Opcao invalida\n");
		}
	}

	fflush(stdin);
	printf("\nOperacao concluida\7");
	system("pause > nul"); // Espera para que se carregue numa tecla para continuar

}


void main()
{
	int a = 0;
	int b = getpid();
	int c = getppid();
	while (a != 3) // Ciclo para que volte sempre ao menu cada vez que se inserir uma opcao diferente
	{

		system("clear"); // Limpa o ecra

		puts("-------------------------------------------------------");
		puts("                         MENU                          ");
		puts("                                                       ");
		puts("     O que pretende realizar?                          ");
		puts("                                                       ");
		puts("1 -- Ficha 1                                           ");
		puts("2 -- Ficha 2                                           ");
		puts("3 -- Sair                                              ");
		puts("-------------------------------------------------------");

		printf("Insira a opcao: ");
		scanf("%d", &a);
		fflush(stdin);

		system("clear");

		switch (a)
		{
			case 1: primeiraF(); break;
			case 2: segundaF(); break;
			case 3: break;
			default: printf("Opcao invalida\n");
		}
	}
}
