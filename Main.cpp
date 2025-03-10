#include<stdio.h>
#include<conio2.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

//Estrutura union do trabalho com os campos passado pelo professor
union Dados
{
	float valorN;
	char valorD[10];
	char valorL; //fica entre 1/0
	char valorC[50];
	char valorM[50];
};

//struct que faz com que a union seja tratada dinamicamente
struct Celula
{
	struct Celula *prox;
	union Dados dados; //não tenho total certeza que tem que usar * na declaração
};
typedef struct Celula celula;

//Struct que vai ter se o status do arquivo para lidar com a exclusão logica
struct Status
{
	//não sei se tem o tipo Boolean em C
	int status; //1-True | 0-False
	struct Status *prox;
};
typedef struct Status status;

//struct que tem os campos que existem dentro do arquivo
struct Campo
{
	char fieldName[50];
	char type;
	int width;
	int dec;
	struct Celula *pAtual, *pDados;
	struct Campo *prox;
};
typedef struct Campo campo;

//struct que contem os dados do arquivo
struct Arquivo
{
	char nomeDBF[50];
	char data[11];
	char hora[6];
	struct Status *status;
	struct Campo *campos;
	struct Arquivo *prox, *ant;
};
typedef struct Arquivo arquivo;

//struct que vai ter a unidade de disco do arquivo
struct Unidade
{
	char unid[3];
	struct Arquivo *arqs;
	struct Unidade *top, *bottom;
};
typedef struct Unidade unidade;

//função que cria as duas unidades de disco automatico
void criarUnidade(unidade **listUnid)
{
	//criando a unidade C:
	unidade *novaUnid1 = (unidade*)malloc(sizeof(unidade));
	strcpy(novaUnid1->unid,"C:");
	novaUnid1->arqs = NULL;
	novaUnid1->bottom = NULL;
	
	//criando a unidade D:
	unidade *novaUnid2 = (unidade*)malloc(sizeof(unidade));
	strcpy(novaUnid2->unid,"D:");
	novaUnid2->arqs = NULL;
	novaUnid2->top = NULL;
	
	//fazendo as ligações entre as unidades e o cabeça da lista
	*listUnid = novaUnid1;
	novaUnid1->top = novaUnid2;
	novaUnid2->bottom = novaUnid1;
}

//Função que realiza a operação de SetDefaultTo (APENAS TESTE)
void setDefaultTo(unidade **auxListUnid, unidade *listUnid, char unidade[3])
{
	if(strcmp(unidade,"C:") == 0) //se caso a unidade for a C:
	{
		*auxListUnid = listUnid;
		printf("\nUnidade mudada para: %s",(*auxListUnid)->unid);
	}
	else
	{
		*auxListUnid = listUnid->top; //apontando para unidade D:
		printf("\nUnidade mudada para: %s",(*auxListUnid)->unid);
	}
}

int main(void)
{
	char comandoDbase[20], unid[3];
	
	unidade *listUnid = NULL;
	unidade *auxListUnid; //ponteiro para manipular o disco C: e D:
	criarUnidade(&listUnid); //cria as duas unidades padrão - C: e D:
	
	do
	{
		printf("\nDigite o comando do DBase: ");
		fflush(stdin);
		gets(comandoDbase);
		
		if(strcmp(comandoDbase,"SET DEFAULT TO") == 0)
		{//apenas para definir a uidade de disco que vai salvar o arquivo
			printf("\nDigite a unidade de disco: ");
			fflush(stdin);
			gets(unid);
				
			if(strcmp(unid,"C:") == 0 || strcmp(unid,"D:") == 0)
			{
				setDefaultTo(&auxListUnid,listUnid,unid);
			}
			else
				printf("\nNao existe essa unidade!\n");
		}
		else
		if(strcmp(comandoDbase,"QUIT") == 0)
		{
			printf("\nAmbiente DBase encerrado!\n");
		}
		getch();	
	}while(strcmp(comandoDbase,"QUIT") != 0);
}
