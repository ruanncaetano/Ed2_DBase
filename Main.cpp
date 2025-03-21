#include<stdio.h>
#include<conio2.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>

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
	char date[11];
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
void setDefaultTo(unidade **auxListUnid, unidade *listUnid, char unid[50]) //OK
{
	if(stricmp(unid,"c:") == 0) //se caso a unidade for a C:
	{
		*auxListUnid = listUnid;
	}
	else
	{
		*auxListUnid = listUnid->top; //apontando para unidade D:
	}
	printf("Unidade mudada para: %s\n",(*auxListUnid)->unid);
}

int validaType(campo *novoCampo, char type[20])
{
	int flag = 1;
	if(stricmp(type,"character") == 0)
		novoCampo->type = 'C';
	else
	if(stricmp(type,"numeric") == 0)
		novoCampo->type = 'N';
	else
	if(stricmp(type,"date") == 0)
		novoCampo->type = 'D';
	else
	if(stricmp(type,"logical") == 0)
		novoCampo->type = 'L';
	else
	if(stricmp(type,"memo") == 0)
		novoCampo->type = 'M';
	else
		flag = 0;
		
	return flag;
}

//função que cria um novo arquivo dentro de algum disco
void create(unidade **auxListUnid, char nomeDBF[50]) //OK
{
	char data[11], hora[6];
	
	struct tm *dataAtual;  
	arquivo *novoArq, *atualArq;
	novoArq = (arquivo*)malloc(sizeof(arquivo));
	strcpy(novoArq->nomeDBF,nomeDBF);
	
	//RUAN - COLOCA A FORMATAÇÃO
	printf("\nData (dd/MM/yyyy): ");
	fflush(stdin);
	gets(novoArq->date);
	printf("\nHora (hh:mm): ");
	fflush(stdin);
	gets(novoArq->hora);
	
	novoArq->status = NULL;
	novoArq->campos = NULL;
	novoArq->prox = NULL;

	//prints teste
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
	
//	PARA CRIAR OS CAMPOS DE DENTRO DO ARQUIVO CRIADO PELO USUARIO (AINDA FALTA ARRUMAR)
//	para manipular a struct campo
	char fieldName[50], type[20], op;
	int width, flag;
	
	printf("\n[ESC] - Sair");
	op = getch();
	while(op != 27)
	{
		campo *novoCampo = (campo*)malloc(sizeof(campo));
		//pegandos os dados que o usuario digitou
		printf("\nFieldName: ");
		fflush(stdin);
		gets(fieldName);
		
		printf("\nType: ");
		fflush(stdin);
		gets(type);
		flag = validaType(novoCampo,type);
		while(flag == 0) //obriga ele a digitar um tipe correto
		{
			printf("\nType invalido! Digite novamente...\n");
			printf("\nType: ");
			fflush(stdin);
			gets(type);
			flag = validaType(novoCampo,type);
		}	
		printf("\nWidth: ");
		scanf("%d",&width);
		
//		na função valida type eu já coloco o type dentro da caixinha
		strcpy(novoCampo->fieldName,fieldName);
		novoCampo->width = width;
		novoCampo->dec = 0;
		novoCampo->pAtual = novoCampo->pDados = NULL;
		novoCampo->prox = NULL;
			
		if(novoArq->campos == NULL)
			novoArq->campos = novoCampo;
		else
		{
			campo *auxCampo = novoArq->campos;
			while(auxCampo->prox != NULL)
				auxCampo = auxCampo->prox;
				
			auxCampo->prox = novoCampo;
		}
		printf("\n[ESC] - Sair");
		op = getch();
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
			printf("\nData: %s",arqAux->date);
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
	
	while(auxArq != NULL && stricmp(auxArq->nomeDBF,nomeDBF) != 0)
		auxArq = auxArq->prox;
			
	if(auxArq != NULL) //achou
	{
		*aberto = auxArq;
		printf("\nArquivo %s aberto com sucesso!",(*aberto)->nomeDBF);
	}	
	else //não achou o arquivo
		printf("\nArquivo nao encontrado!\n");
	
}

void listStructure(arquivo *aberto, unidade *auxListUnid)
{
	int cont=1, total=1;
	arquivo *auxArq = aberto;
	campo *auxCampo = aberto->campos;
	printf("\n. LIST STRUCTURE");
	printf("\nStructure for database\t: %s%s",auxListUnid->unid,auxArq->nomeDBF);
	printf("\nNumber of data records\t: 0");
	printf("\nDate of last update   \t: %s",auxArq->date);
	
	if(aberto->campos != NULL)
	{
		while(auxCampo != NULL)
		{
			printf("\nField: %d",cont);
			printf("\nField name: %s",auxCampo->fieldName);
			
			if(auxCampo->type == 'C')
				printf("\nType: Character");
			else
			if(auxCampo->type == 'N')
				printf("\nType: Numeric");
			else
			if(auxCampo->type == 'D')
				printf("\nType: Date");
			else
			if(auxCampo->type == 'L')
				printf("\nType: Logical");
			else
			if(auxCampo->type == 'M')
				printf("\nType: Memo");
			
			printf("\nWidth: %d",auxCampo->width);
			printf("\nDec: %d\n",auxCampo->dec);
			
			total = total + auxCampo->width;
			
			auxCampo = auxCampo->prox;
			cont++;
		}
		printf("\n** Total **: %d",total);
	}
	else
		printf("\nNao contem campos no arquivo!\n");
		
}

//Função que o Gui fez para buscar se um comando é valido
int buscaFuncao(char *comando, char *resto)
{
	char comandos[][20] = {
		"set default to", "create", "dir", "quit", "use", "liste structure",
        "append", "list", "list for", "clear", "locate for", "goto",
        "display", "edit", "delete", "recall", "set deleted", "pack", "zap"
	};
	
	int tamanho = sizeof(comandos) / sizeof(comandos[0]);
	for(int i=0; i<tamanho; i++)
	{
		if(stricmp(comando,comandos[i]) == 0)
			return 1;
	}
	return 0;
}

// Função para ler e tokenizar a string
void LerPromt(char leitura[50], char *comando, char *resto)
{
    // Variável para armazenar o comando composto
	char comando_composto[100] = "";
	
	// Aloca memória para a cópia da string
    char *copia = (char *)malloc((strlen(leitura) + 1) * sizeof(char)); // não sei se isso esta certo
    if(copia == NULL)
	{
        printf("\nErro ao alocar memoria.\n");
    }
	else
	{
	    // Copia a string original para a cópia
	    strcpy(copia, leitura);
	
	    // Ignorar espaços no começo
	    char *inicio = copia;
	    while(*inicio == ' ')
		{
	        inicio++;
	    }
		char *fim_palavra = inicio;
	    // Pega a primeira palavra
	    while(*fim_palavra != ' ' && *fim_palavra != '\0')
		{
	        fim_palavra++;
	    }
	
	    // Separa a primeira palavra
	    if(*fim_palavra == ' ')
		{
	        *fim_palavra = '\0'; // Termina a primeira palavra
	        strcat(comando_composto, inicio);
	        inicio = fim_palavra + 1;
	
	        // Remove espaços iniciais do restante
	        while(*inicio == ' ')
			{
	            inicio++;
	        }
	    }
		else
		{
	        strcat(comando_composto, inicio);
	        inicio = fim_palavra; // Fim da string
	    }
	
	    // Verifica se a primeira palavra é um comando válido
	    int buscaSeE = buscaFuncao(comando_composto, inicio);
	
	    // Loop para concatenar mais palavras enquanto a busca retornar 0
	    while(buscaSeE == 0 && *inicio != '\0')
		{
	        // Pega a próxima palavra
	        fim_palavra = inicio;
	        while(*fim_palavra != ' ' && *fim_palavra != '\0')
			{
	            fim_palavra++;
	        }
	
	        // Concatena a próxima palavra ao comando composto
	        if(*fim_palavra == ' ')
			{
	            *fim_palavra = '\0'; // Termina a próxima palavra
	            strcat(comando_composto, " ");
	            strcat(comando_composto, inicio);
	            inicio = fim_palavra + 1;
	
	            // Remove espaços iniciais do restante
	            while(*inicio == ' ')
				{
	                inicio++;
	            }
	        }
			else
			{
	            strcat(comando_composto, " ");
	            strcat(comando_composto, inicio);
	            inicio = fim_palavra; // Fim da string
	        }
	
	        // Verifica se o comando composto é válido
	        buscaSeE = buscaFuncao(comando_composto, inicio);
	    }
		strcpy(comando,comando_composto);
		strcpy(resto,inicio);
	    // Libera a memória alocada para a cópia
	    free(copia);
	}
}

void append(arquivo *aberto, status **posStatus)
{
	campo *auxCampo = aberto->campos;
	
	status *novoStatus = (status*)malloc(sizeof(status));
	novoStatus->status = 1; // 1 =True
	novoStatus->prox = NULL;
	
	if(*posStatus == NULL)
	{
		(*posStatus) = novoStatus;
		aberto->status = novoStatus;
	}
	else
	{
		status *auxStatus = *posStatus;
		while(auxStatus->prox != NULL)
			auxStatus = auxStatus->prox;
			
		auxStatus->prox = novoStatus;
	}
	
	while(auxCampo != NULL)
	{
		//criando a caixinha para inserir os dados da union
		celula *novaCelula = (celula*)malloc(sizeof(celula));
		novaCelula->prox = NULL;
		
		printf("\n%s ",auxCampo->fieldName);
		if(auxCampo->type == 'N')
		{
			scanf("%f",&novaCelula->dados.valorN);
		}
		else
		if(auxCampo->type == 'D')
		{
			fflush(stdin);
			gets(novaCelula->dados.valorD);
		}
		else
		if(auxCampo->type == 'L')
		{
			scanf("%c",&novaCelula->dados.valorL);
		}
		else
		if(auxCampo->type == 'C')
		{
			fflush(stdin);
			gets(novaCelula->dados.valorC);
		}
		else
		if(auxCampo->type == 'M')
		{
			fflush(stdin);
			gets(novaCelula->dados.valorM);
		}

		if(auxCampo->pDados == NULL) //insere no inicio se for vazia
		{
		    auxCampo->pDados = novaCelula;
		    auxCampo->pAtual = novaCelula;
		}
		else
		{
			celula *auxCelula = auxCampo->pDados;
			while(auxCelula->prox != NULL) //insere no final
				auxCelula = auxCelula->prox;
					
			auxCelula->prox = novaCelula;
		}
		auxCampo = auxCampo->prox;		
	}
}

//void printUnion(campo *auxCampo, int col, int lin)
//{
//	if(auxCampo->type == 'N')
//	{
//		gotoxy(col,lin);
//		printf("%.0f",auxCampo->pAtual->dados.valorN);
//	}
//	else
//	if(auxCampo->type == 'L')
//	{
//		gotoxy(col,lin);
//		printf("%c",auxCampo->pAtual->dados.valorL);
//	}
//	else
//	if(auxCampo->type == 'C')
//	{
//		gotoxy(col,lin);
//		printf("s",auxCampo->pAtual->dados.valorC);
//	}
//	else
//	if(auxCampo->type == 'D')
//	{
//		gotoxy(col,lin);
//		printf("%s",auxCampo->pAtual->dados.valorD);
//	}
//	else
//	if(auxCampo->type == 'M')
//	{
//		gotoxy(col,lin);
//		printf("%s",auxCampo->pAtual->dados.valorM);
//	}
//}

void printUnion(campo *auxCampo, int col, int lin)
{
    if (auxCampo->pAtual != NULL)
	{
		gotoxy(col, lin);

	    switch (auxCampo->type) 
	    {
	        case 'N': 
				printf("%.0f", auxCampo->pAtual->dados.valorN); 
				break;
	        case 'L':
				printf("%c", auxCampo->pAtual->dados.valorL);
				break;
	        case 'C':
				printf("%s", auxCampo->pAtual->dados.valorC);
				break;
	        case 'D': 
				printf("%s", auxCampo->pAtual->dados.valorD);
				break;
	        case 'M': 
				printf("%s", auxCampo->pAtual->dados.valorM);
				break;
	        default: 
				printf("[ERRO]");
				break;
	    }
	}
    
}

void list(arquivo *aberto, status *posStatus)
{
    campo *auxCampo;
    status *auxStatus = posStatus;
    celula *auxCelula;
    
    system("cls");
    
    int pos = 1, col = 15, lin = 7;
    
    gotoxy(5,5);
    printf(". LIST");
    gotoxy(5,6);
    printf("Record# ");

    // Exibir nomes dos campos
    auxCampo = aberto->campos;
    while (auxCampo != NULL) 
    {
        gotoxy(col,6);
        printf("%s", auxCampo->fieldName);
        col += 15;
        auxCampo = auxCampo->prox;
    }

    // Exibir dados armazenados
    auxStatus = posStatus;  // Reinicia ponteiro de status
    while (auxStatus != NULL) 
    {
        if (auxStatus->status == 1) 
        {
            col = 5;
            gotoxy(col, lin);
            printf("%d", pos);

            auxCampo = aberto->campos;
            while (auxCampo != NULL) 
            {
                //auxCelula = auxCampo->pAtual; // Usa ponteiro auxiliar sem alterar `pAtual`
                col += 15;

                if(auxCampo != NULL) 
                    printUnion(auxCampo, col, lin);
				
				auxCampo->pAtual = auxCampo->pAtual->prox;
                auxCampo = auxCampo->prox;
            }

            lin++;
            pos++;
        }
        auxStatus = auxStatus->prox;
    }

    //voltando o pAtual para o inicio da lista assim como pDados
    auxCampo = aberto->campos;
    while (auxCampo != NULL) 
    {
        auxCampo->pAtual = auxCampo->pDados; 
        auxCampo = auxCampo->prox;
    }
}


int main(void)
{ 
	char comandoDbase[50], unid[3];
	char nomeDBF[50], data[11], hora[6];
	
	char comando[50],resto[50];
	
	unidade *listUnid = NULL;
	unidade *auxListUnid = NULL; //ponteiro para manipular o disco C: e D:
	criarUnidade(&listUnid); //cria as duas unidades padrão - C: e D:
	//esse ponteiro vamos usar apenas para manipular o arquivo que o usuario deseja
	arquivo *aberto=NULL;
	status *posStatus = NULL; //criando a lista de status para os campos
	
	do
	{
		printf("\nComandoDBase: ");
		fflush(stdin);
		gets(comandoDbase);
		LerPromt(comandoDbase,comando,resto);
		//cada if pode tratar o *resto* como precisar
		if(stricmp(comando, "Set Default To") == 0) //OK - TESTADO!!!
		{
			if(stricmp(resto,"c:") == 0 || stricmp(resto,"d:") == 0)
	       		setDefaultTo(&auxListUnid, listUnid, resto);
	       	else
	       		printf("\nUnidade nao existe!\n");
	    }
	    else
		if(stricmp(comando, "create") == 0)
		{
			if(auxListUnid != NULL)
			{
				if(strlen(resto) > 4) //estou comparando > 4 = ".dbf"
					create(&auxListUnid,resto);
				else
					printf("\nVoce nao passou um nome!\n");
			}	
	        else
	        	printf("\nVoce ainda nao escolheu uma unidade!\n");
	    }
	    else
	    if(stricmp(comando, "dir") == 0)
		{
			if(auxListUnid != NULL)
	    		dir(auxListUnid);
	    	else
	    		printf("\nVoce ainda nao escolheu uma unidade!\n");
	    }
	    else
		if(stricmp(comando, "use") == 0)
		{
			if(auxListUnid != NULL)
				use(&aberto,auxListUnid,resto);
			else
				printf("\nVoce ainda nao escolheu uma unidade!\n");
	    }
	    else
		if(stricmp(comando, "liste structure") == 0)
		{
			if(auxListUnid != NULL)
			{
				if(aberto != NULL)
					listStructure(aberto,auxListUnid);
				else
					printf("\nVoce ainda nao abriu um arquivo!\n");
			}
			else
				printf("\nVoce ainda nao escolheu uma unidade!\n");
		}
		else
		if(stricmp(comando, "quit") == 0)
		{
			printf("\nAmbiente DBase encerrado!\n");
		}
		else
		if(stricmp(comando,"clear") == 0)
		{
			system("cls");
		}
		else
		if(stricmp(comando,"append") == 0)
		{
			if(auxListUnid != NULL)
			{
				if(aberto != NULL)
				{
					printf("\nTESTE APPEND 3");
					if(aberto->campos != NULL) //ele tem que ter criado os campos
						append(aberto,&posStatus);
					else
						printf("\nO arquivo nao tem campos criados!\n");
				}
				else
					printf("\nVoce ainda nao abriu um arquivo!\n");
			}
			else
				printf("\nVoce ainda nao escolheu uma unidade!\n");
		}
		else
		if(stricmp(comando,"list") == 0)
		{
			if(auxListUnid != NULL)
			{
				if(aberto != NULL)
				{
					if(aberto->campos->pDados != NULL)
						list(aberto,posStatus);
					else
						printf("\nVoce ainda nao colocou dados no arquivo!\n");
				}
				else
					printf("\nVoce ainda nao abriu um arquivo!\n");
			}
			else
				printf("\nVoce ainda nao escolheu uma unidade!\n");
		}
		else
			printf("\nNao existe esse comando!\n");
			
		getch();
	}while(stricmp(comando,"quit") != 0);
}
