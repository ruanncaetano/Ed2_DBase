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
	struct celula *pAtual, *pDados;
	struct Campo *prox;
};
typedef struct Campo campo;

//struct que contem os dados do arquivo
struct Arquivo
{
	char nomeDBF[50];
	char data[11];
	char hora[6];
	struct status *status;
	struct campo *campos;
	struct Arquivo *prox, *ant;
};
typedef struct Arquivo arquivo;

//struct que vai ter a unidade de disco do arquivo
struct Unidade
{
	char unid[3];
	struct arquivo *arqs;
	struct Unidade *top, *bottom;
};
typedef struct Unidade unidade;

//Função que realiza a operação de SetDefaultTo (APENAS TESTE)
void setDefaultTo(unidade **listaUnid, char unidade)
{
	if(strcmp(unidade,"C:") == 0 && strcmp(unidade,"D:") == 0)
	{
		//criando a nova caixinha de unidade
		unidade *novaUnid = (unidade*)malloc(sizeof(unidade));
		strcpy(novaUnid->unid,unidade);
		novaUnid->arqs = NULL;
		novaUnid->bottom = novaUnid->top = NULL;
	
		if(*listUnid == NULL) //se caso a lista de unidade ainda estiver vazia
		{
			*listUnid = novaUnid;
		}
		else //se caso já tiver um ou mais elemento na lista de unidade
		{
			novaUnid->top = *listUnid;
			(*listUnid)->bottom = novaUnid;
			*listUnid = novaUnid;
		}
	}
	else
		printf("\nNao existe essa unidade!\n");
		
}

int main(void)
{
	gotoxy(10,20);
	printf("\nTeste!!!");
}
