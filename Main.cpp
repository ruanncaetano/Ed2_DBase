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
void criarUnidade(unidade **listUnid) //OK
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
void setDefaultTo(unidade **auxListUnid, unidade *listUnid, char unidade[3]) //OK
{
	if(strcmp(unidade,"C:") == 0) //se caso a unidade for a C:
	{
		*auxListUnid = listUnid;
	}
	else
	{
		*auxListUnid = listUnid->top; //apontando para unidade D:
	}
	printf("Unidade mudada para: %s\n",(*auxListUnid)->unid);
}

//função que cria um novo arquivo dentro de algum disco
void create(unidade **auxListUnid, char nomeDBF[50], char data[11], char hora[6]) //OK
{
	arquivo *novoArq, *atualArq;
	novoArq = (arquivo*)malloc(sizeof(arquivo));
	
	strcpy(novoArq->nomeDBF,nomeDBF);
	strcpy(novoArq->data,data);
	strcpy(novoArq->hora,hora);
	novoArq->status = NULL;
	novoArq->campos = NULL;
	novoArq->prox = NULL;
	
	if((*auxListUnid)->arqs == NULL) //se caso ainda não tiver nenhum arquivo
	{
		(*auxListUnid)->arqs = novoArq;
		novoArq->ant = NULL;
	}
	else
	{
		atualArq = (*auxListUnid)->arqs;
		while(atualArq->prox != NULL)
			atualArq = atualArq->prox;
			
		novoArq->ant = atualArq;
		atualArq->prox = novoArq;
	}
	
	printf("\nArquivo inserido com sucesso!!!\n");
}

//função que exibe os arquivos da unidade
void dir(unidade *auxListUnid) //OK
{
	arquivo *arqAux = auxListUnid->arqs;
	printf("\nUNIDADE ATUAL: %s",auxListUnid->unid);
	if(arqAux != NULL)
	{
		while(arqAux != NULL)
		{
			printf("\n*** EXIBINDO DADOS DOS ARQUIVOS ***");
			printf("\nNome DBF: %s",arqAux->nomeDBF);
			printf("\nData: %s",arqAux->data);
			printf("\nHora: %s\n",arqAux->hora);
			arqAux = arqAux->prox;
		}
	}
	else
		printf("\nUnidade sem nenhum arquivo!\n");
	
}

//função que busca o arquivo e faz o ponteiro aberto apontar para ele
void use(arquivo **aberto, unidade *auxListUnid, char nomeDBF[50]) //OK
{

	arquivo *auxArq = auxListUnid->arqs;
	
	while(auxArq != NULL && strcmp(auxArq->nomeDBF,nomeDBF) != 0)
		auxArq = auxArq->prox;
			
	if(auxArq != NULL) //achou
	{
		*aberto = auxArq;
		printf("\nArquivo %s aberto com sucesso!",(*aberto)->nomeDBF);
//		para testar se está sendo feito o apontamento correto
//		printf("\n### ARQUIVO QUE O PONTEIRO ABERTO ESTA APONTANDO ###");
//		printf("\nNome DBF: %s",(*aberto)->nomeDBF);
//		printf("\nData: %s",(*aberto)->data);
//		printf("\nHora: %s",(*aberto)->hora);
	}	
	else //não achou o arquivo
		printf("\nArquivo nao encontrado!\n");
	
}

//função que vai montar a estrutura com os campos presente no arquivo
//void listStructure(arquivo *aberto)
//{
//	//criando a caixa "CODIGO"
//	campo *auxCod = (campo*)malloc(sizeof(campo));
//	strcpy(auxCod->fieldName,"CODIGO");
//	auxCod->type = 'N';
//	auxCod->width = 8;
//	auxCod->dec = 0;
//	auxCod->pAtual = auxCod->pDados = NULL;
//	
//	//criando a caixa "NOME"
//	campo *auxNome = (campo*)malloc(sizeof(campo));
//	strcpy(auxNome->fieldName,"NOME");
//	auxNome->type = 'C';
//	auxNome->width = 20;
//	auxNome->dec = 0;
//	auxNome->pAtual = auxNome->pDados = NULL;
//	
//	//criando a caixa "FONE"
//	campo *auxFone = (campo*)malloc(sizeof(campo));
//	strcpy(auxFone->fieldName,"FONE");
//	auxFone->type = 'C';
//	auxFone->width = 10;
//	auxFone->dec = 0;
//	auxFone->pAtual = auxFone->pDados = NULL;
//	
//	
//	//aqui no final é ligar uma caixa na outra
//	auxCod->prox = auxNome;
//	auxNome->prox = auxFone;
//	auxFone->prox = NULL;
//	
//	aberto->campos = auxCod;
//}

void exibeStructure(arquivo *aberto, unidade *auxListUnid, int field, int total)
{
	int cont = 1;
	arquivo *auxArq = aberto;
	campo *auxCampo = aberto->campos;
	printf("\n. LIST STRUCTURE");
	printf("\nStructure for database\t: %s%s",auxListUnid->unid,auxArq->nomeDBF);
	printf("\nNumber of data records\t: 0");
	printf("\nDate of last update   \t: %s",auxArq->data);
	
	while(cont != field)
	{
		printf("\nField: %d",cont);
		printf("\nField name: %s",auxCampo->fieldName);
		
		//aqui eu acho que tem que colocar o restante dos tipos de dados da tabela
		if(auxCampo->type == 'C' || auxCampo->type == 'c')
			printf("\nType: Character");
		else
		if(auxCampo->type == 'N' || auxCampo->type == 'n')
			printf("\nType: Numeric");
		
		printf("\nWidth: %d",auxCampo->width);
		printf("\nDec: %d\n",auxCampo->dec);
		
		auxCampo = auxCampo->prox;
		cont++;
	}
	
	printf("\n** Total **: %d",total);
}

void listStructure(arquivo *aberto, unidade *auxListUnid)
{
	//para manipular a struct campo
	char fieldName[50], type, op;
	int width, field=1, total = 1;
	
	printf("\n[ESC] - Sair");
	op = getch();
	while(op != 27)
	{
		//pegandos os dados que o usuario digitou
		printf("\nFieldName: ");
		fflush(stdin);
		gets(fieldName);
		printf("\nType: ");
		scanf("%c",&type);
		printf("\nWidth: ");
		scanf("%d",&width);
		
		total = total + width;
		
		//colocando os dados em seus devidos campos na caixinha
		campo *novoCampo = (campo*)malloc(sizeof(campo));
		strcpy(novoCampo->fieldName,fieldName);
		novoCampo->type = type;
		novoCampo->width = width;
		novoCampo->dec = 0;
		novoCampo->pAtual = novoCampo->pDados = NULL;
		novoCampo->prox = NULL;
		
		if(aberto->campos == NULL)
			aberto->campos = novoCampo;
		else
		{
			campo *auxCampo = aberto->campos;
			while(auxCampo->prox != NULL)
				auxCampo = auxCampo->prox;
				
			auxCampo->prox = novoCampo;
		}
		
		field++;
		printf("\n[ESC] - Sair");
		op = getch();
	}
	
	exibeStructure(aberto,auxListUnid,field,total);
}

int main(void)
{
	char comandoDbase[20], unid[3];
	char nomeDBF[50], data[11], hora[6];
	
	unidade *listUnid = NULL;
	unidade *auxListUnid = NULL; //ponteiro para manipular o disco C: e D:
	criarUnidade(&listUnid); //cria as duas unidades padrão - C: e D:
	
	//esse ponteiro vamos usar apenas para manipular o arquivo que o usuario deseja
	arquivo *aberto = NULL;
	
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
				aberto = NULL;
//				coloquei o aberto para NULL aqui porque toda vez a pessoa mudar o disco
//				ela tem que ser obrigada a dizer qual arquivo ela quer abrir
			}
			else
				printf("\nNao existe essa unidade!\n");
		}
		else
		if(strcmp(comandoDbase,"CREATE") == 0)
		{
			if(auxListUnid != NULL)
			{
				printf("\nNome do arquivo .DBF: ");
				fflush(stdin);
				gets(nomeDBF);
				printf("\nData do arquivo .DBF (dd/MM/yyyy): ");
				fflush(stdin);
				gets(data);
				printf("\nHora do arquivo .DBF (hh:mm): ");
				fflush(stdin);
				gets(hora);
				
				create(&auxListUnid,nomeDBF,data,hora);
			}
			else
				printf("\nVoce ainda nao definiu a unidade!\n");
			
		}
		else
		if(strcmp(comandoDbase,"QUIT") == 0)
		{
			printf("\nAmbiente DBase encerrado!\n");
		}
		else
		if(strcmp(comandoDbase,"DIR") == 0)
		{
			if(auxListUnid != NULL)
				dir(auxListUnid);
			else
				printf("\nVoce ainda nao definiu a unidade!\n");
		}
		else
		if(strcmp(comandoDbase,"USE") == 0)
		{
			if(auxListUnid != NULL)
			{
				if(auxListUnid->arqs != NULL) //contem arquivos dentro da unidade
				{
					printf("\nNome arquivo: ");
					fflush(stdin);
					gets(nomeDBF);
					
					use(&aberto,auxListUnid,nomeDBF);
				}
				else //não contem arquivos dentro da unidade
					printf("\nNao contem arquivos dentro da unidade!\n");
			}
			else
				printf("\nVoce ainda nao definiu a unidade!\n");
			
		}
		else
		if(strcmp(comandoDbase,"LIST STRUCTURE") == 0)
		{
			if(aberto != NULL) //contem um arquivo
			{
				listStructure(aberto,auxListUnid);
			}
			else
				printf("\nNao contem arquivos dentro da unidade!\n");
		}
		else
			printf("\nNao existe esse comando!\n");
		getch();	
	}while(strcmp(comandoDbase,"QUIT") != 0);
}
