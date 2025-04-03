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
	union Dados dados; //n�o tenho total certeza que tem que usar * na declara��o
};
typedef struct Celula celula;

//Struct que vai ter se o status do arquivo para lidar com a exclus�o logica
struct Status
{
	//n�o sei se tem o tipo Boolean em C
	char status[6]; //True | False
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

//fun��o que cria as duas unidades de disco automatico
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
	
	//fazendo as liga��es entre as unidades e o cabe�a da lista
	*listUnid = novaUnid1;
	novaUnid1->top = novaUnid2;
	novaUnid2->bottom = novaUnid1;
}

//Fun��o que realiza a opera��o de SetDefaultTo (APENAS TESTE)
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

//fun��o que cria um novo arquivo dentro de algum disco
void create(unidade **auxListUnid, char nomeDBF[50]) //OK
{
	char data[11], hora[6];
	
	struct tm *dataAtual;  
	arquivo *novoArq, *atualArq;
	novoArq = (arquivo*)malloc(sizeof(arquivo));
	strcpy(novoArq->nomeDBF,nomeDBF);
	
	//RUAN - COLOCA A FORMATA��O
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
	if((*auxListUnid)->arqs == NULL) //se caso ainda n�o tiver nenhum arquivo
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
		
//		na fun��o valida type eu j� coloco o type dentro da caixinha
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

//fun��o que exibe os arquivos da unidade
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

//fun��o que busca o arquivo e faz o ponteiro aberto apontar para ele
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
	else //n�o achou o arquivo
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

//Fun��o que o Gui fez para buscar se um comando � valido
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

// Fun��o para ler e tokenizar a string
void LerPromt(char leitura[50], char *comando, char *resto)
{
    // Vari�vel para armazenar o comando composto
	char comando_composto[100] = "";
	
	// Aloca mem�ria para a c�pia da string
    char *copia = (char *)malloc((strlen(leitura) + 1) * sizeof(char)); // n�o sei se isso esta certo
    if(copia == NULL)
	{
        printf("\nErro ao alocar memoria.\n");
    }
	else
	{
	    // Copia a string original para a c�pia
	    strcpy(copia, leitura);
	
	    // Ignorar espa�os no come�o
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
	
	        // Remove espa�os iniciais do restante
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
	
	    // Verifica se a primeira palavra � um comando v�lido
	    int buscaSeE = buscaFuncao(comando_composto, inicio);
	
	    // Loop para concatenar mais palavras enquanto a busca retornar 0
	    while(buscaSeE == 0 && *inicio != '\0')
		{
	        // Pega a pr�xima palavra
	        fim_palavra = inicio;
	        while(*fim_palavra != ' ' && *fim_palavra != '\0')
			{
	            fim_palavra++;
	        }
	
	        // Concatena a pr�xima palavra ao comando composto
	        if(*fim_palavra == ' ')
			{
	            *fim_palavra = '\0'; // Termina a pr�xima palavra
	            strcat(comando_composto, " ");
	            strcat(comando_composto, inicio);
	            inicio = fim_palavra + 1;
	
	            // Remove espa�os iniciais do restante
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
	
	        // Verifica se o comando composto � v�lido
	        buscaSeE = buscaFuncao(comando_composto, inicio);
	    }
		strcpy(comando,comando_composto);
		strcpy(resto,inicio);
	    // Libera a mem�ria alocada para a c�pia
	    free(copia);
	}
}

void append(arquivo *aberto, status **posStatus)
{
	campo *auxCampo = aberto->campos;
	
	status *novoStatus = (status*)malloc(sizeof(status));
	strcpy(novoStatus->status,"true"); // 1 =True
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

void printUnion(celula *auxCelula,char type, int col, int lin)
{
    if(auxCelula != NULL)
	{
		gotoxy(col, lin);

	    switch(type) 
	    {
	        case 'N': 
				printf("%.0f", auxCelula->dados.valorN); 
				break;
	        case 'L':
				printf("%c", auxCelula->dados.valorL);
				break;
	        case 'C':
				printf("%s", auxCelula->dados.valorC);
				break;
	        case 'D': 
				printf("%s", auxCelula->dados.valorD);
				break;
	        case 'M': 
				printf("%s", auxCelula->dados.valorM);
				break;
	        default: 
				printf("[ERRO]");
				break;
	    }
	}
    
}

void list(arquivo *aberto, status *posStatus)
{
    campo *auxCampo = aberto->campos;
    status *auxStatus = posStatus;
    celula *auxCelula = aberto->campos->pDados;
    
    system("cls");
    
    int pos = 1, col = 1, lin = 1;
    char type;
    
    gotoxy(col,lin); //1 - 1
    printf(". LIST");
    lin++;
    gotoxy(col,lin); //1 - 2
    printf("Record# ");
    lin++;
    
    //exbindo o record apenas mudando de linha
    while(auxCelula != NULL)
    {
    	if(stricmp(auxStatus->status,"true") == 0)
    	{
	        gotoxy(col,lin); //1 - 3 em diante
	        printf("%d", pos);
	        pos++;
	        lin++;
		}
		auxCelula = auxCelula->prox;
	}
    
    while(auxCampo != NULL)
    {
    	celula *auxCelula = auxCampo->pDados;
    	col += 20; 
    	lin = 2;
        gotoxy(col,lin); //21 - 2
        printf("%s", auxCampo->fieldName);
            
        while(auxCelula != NULL)
        {
            if(stricmp(auxStatus->status,"true") == 0)
            {
            	lin++;
            	type = auxCampo->type;
            	printUnion(auxCelula, type, col, lin);
			}
			auxCelula = auxCelula->prox;
		}
		auxCampo = auxCampo->prox;
	}
}

void localeFor(arquivo *aberto, char *campo, void *busca) 
{
	int record=1;
    if (aberto == NULL) {
        printf("\nNenhum arquivo aberto!\n");
        return;
    }
	printf("Campo: |%s|\n", campo);
	printf("Valor: |%s|\n", busca);

    Campo *auxCampo = aberto-> campos;
    while (auxCampo != NULL)
	 {
        if (strcmp(auxCampo->fieldName, campo) == 0)
		 {
            // Campo encontrado, agora verificar o valor
            celula *auxCelula = auxCampo->pDados;
            while (auxCelula != NULL) 
			{
                switch (auxCampo->type)
				 {
                    case 'C': // Character
                        if (strcmp(auxCelula->dados.valorC, (char*)busca) == 0) 
						{
                            printf("\nRecord = %d\n",record);
                        }
                        break;
                    case 'N': // Numeric
                        if (auxCelula->dados.valorN == *(int*)busca) 
						{
                              printf("\nRecord = %d\n",record);
                        }
                        break;
                    case 'D': // Date
                        if (strcmp(auxCelula->dados.valorD, (char*)busca) == 0) 
						{
                            printf("\nRecord = %d\n",record);
                        }
                        break;
                    case 'L': // Logical
                        if (auxCelula->dados.valorL == *(char*)busca)
						 {
                              printf("\nRecord = %d\n",record);
                        }
                        break;
                    case 'M': // Memo
                        if (strcmp(auxCelula->dados.valorM, (char*)busca) == 0) 
						{
                            printf("\nRecord = %d\n",record);
                        }
                        break;
                }
                record++;
                auxCelula = auxCelula->prox;
            }
            printf("\nValor nao encontrado no campo %s.\n", campo);
            return;
        }
        auxCampo = auxCampo->prox;
    }
  
}

arquivo *buscaStatusAtivo(arquivo *auxArq, int &pos)
{
	while(stricmp(auxArq->status->status,"true") != 0 && auxArq->status != NULL)
	{
		auxArq->status = auxArq->status->prox;
		auxArq->campos = auxArq->campos->prox;
		pos++;
	}
	
	if(stricmp(auxArq->status->status,"true") == 0)
		return auxArq;
	else
		return NULL;
}

void display(arquivo *auxAberto)
{
	campo *auxCampo = auxAberto->campos;  //ok
	campo *auxAtualCampo = auxAberto->campos; //ok
	celula *auxCelula = auxAberto->campos->pAtual; //ok
	
	arquivo *auxArq = auxAberto;
	
	system("cls");
    
    int pos = 1, col = 1, lin = 1;
    char type;
    
    gotoxy(col,lin);
    printf(". DISPLAY");
    lin++;
    gotoxy(col,lin);
    printf("Record# ");

    //exibir nome dos campos
    while(auxCampo != NULL)
    {
    	col += 20;
        gotoxy(col,lin);
        printf("%s", auxCampo->fieldName);
        auxCampo = auxCampo->prox;
    }
    lin++;
    
    //resetando o auxCampo
    auxCampo = auxAberto->campos;
		
	col = 1;
		
	//procurar a posi��o que o atual est� para saber o valor
	while(auxAtualCampo->pDados != auxCampo->pAtual)
	{
		auxAtualCampo->pDados = auxAtualCampo->pDados->prox;
		pos++;
	}
		
    gotoxy(col, lin);
    printf("%d", pos);

    auxCampo = auxArq->campos;
    while(auxCampo != NULL) 
    {
        col += 20;
		auxCelula = auxCampo->pAtual;
        type = auxCampo->type;

        if(auxCelula != NULL)
            printUnion(auxCelula, type, col, lin);
        auxCampo = auxCampo->prox;
    }
    //voltandos os ponteiros para origem
    auxCampo = auxArq->campos;
}

void Goto(arquivo *auxAberto, int pos)
{
    campo *auxCampo = auxAberto->campos;
    status *auxStatus = auxAberto->status;

    // Sempre voltar pAtual para o in�cio (pDados sempre aponta para a cabe�a)
    while (auxCampo != NULL)
    {
        auxCampo->pAtual = auxCampo->pDados;
        auxCampo = auxCampo->prox;
    }
    auxCampo = auxAberto->campos; // Resetando auxCampo
    auxStatus = auxAberto->status; // Voltando status para o in�cio

    int cont = 1;

    // Avan�ar at� a posi��o desejada
    while (cont < pos && auxStatus != NULL)
    {
        auxStatus = auxStatus->prox;
        auxCampo = auxAberto->campos;

        while (auxCampo != NULL)
        {
            auxCampo->pAtual = auxCampo->pAtual->prox;
            auxCampo = auxCampo->prox;
        }
        cont++;
    }

    // Se o status for "false", continuar avan�ando at� encontrar um "true"
    while (stricmp(auxStatus->status, "true") != 0)
    {
        auxStatus = auxStatus->prox;
        auxCampo = auxAberto->campos;

        while (auxCampo != NULL)
        {
            auxCampo->pAtual = auxCampo->pAtual->prox;
            auxCampo = auxCampo->prox;
        }
    }
}


void Delete(arquivo *auxAberto)
{
//	//deixando o registro inativo com a exclusao logica
//	strcpy(auxAberto->status->status,"false");
//	
////	PRA MIM N�O TEM A NECESSIDADE DE MUDAR OS PONTEIROS
////	PORQUE MESMO QUE A GENTE EXCLUIU N�O PODE PERDER O ENDERE�O DESSE REGISTRO
////	PORQUE TALVEZ A PESSOA PEDE PARA REATIVAR ELE AI TEM QUE ESTAR EM ORDEM AINDA
////	VAMOS PULAR ESSE RESGISTRO APENAS NA HORA DE EXIBIR
//
////	PARA EU FAZER O DELETE EU PRECISO DO GOTO2 PARA COLOCAR O PATUAL EM UM
////	DETERMINADO REGISTRO E ATRAVES DELE EU DELETO ESSE REGISTRO QUE PATUAL APONTA
////	ENT�O TENHO QUE FAZER ESSE GOTO2 ANTES
}

int main(void)
{ 
	char comandoDbase[50], unid[3];
	char nomeDBF[50], data[11], hora[6];
	
	char comando[50],resto[50];
	
	int num=0;
	
	unidade *listUnid = NULL;
	unidade *auxListUnid = NULL; //ponteiro para manipular o disco C: e D:
	criarUnidade(&listUnid); //cria as duas unidades padr�o - C: e D:
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
		if(stricmp(comando, "Set Default To") == 0)
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
		if(stricmp(comando,"locate for") == 0)
		{
			if(auxListUnid != NULL)
			{
				int i=0,j=0,cont=strlen(resto);
				char aux[50]={0}, copia[50]={0};
				strcpy(aux,resto);
				// limapando a string de = e ""
				while(cont >0)
				{
					if((aux[i] >= 'A' && aux[i]<='Z') || (aux[i] >= 'a' && aux[i]<='z') || (aux[i] >= '0' && aux[i]<='9') || aux[i] == ' ')
					{
						copia[j] = aux[i];
						j++;
					}
					
					i++;
					cont--;
				}
				copia[j]='\0';
				
				
				//////////
				
				char campo2[50]={0},nome[50]={0};
				int i2=0,j2=0;
				
				
				// serapra o campo de busca
				while(copia[i2] != ' ' && copia[i2] != '\0')
				{
					campo2[i2]=copia[i2];
					i2++;
				}
				campo2[i2] = '\0'; 
				// pega o resto de tudo, que � a informa��o que quer buscar
				while(copia[i2] != '\0')
				{
					if(copia[i2] != ' ')
					{
						nome[j2]=copia[i2];
						j2++;
					}
				
					i2++;
				}
				nome[j2] = '\0';
				printf("Campo: |%s|\n", campo2);
				printf("Valor: |%s|\n", nome);
	
				localeFor(aberto,campo2,nome);
			}
			else
				printf("\nVoce ainda nao escolheu uma unidade!\n");
		}
		else
		if(stricmp(comando,"display") == 0)
		{
			if(auxListUnid != NULL)
			{
				if(aberto != NULL)
				{
					if(aberto->campos->pAtual != NULL)
					{
						display(aberto);
					}
					else
						printf("\nVoce ainda nao inseriu um registro!\n");
				}
				else
					printf("\nVoce ainda nao abriu um arquivo!\n");
			}
			else
				printf("\nVoce ainda nao escolheu uma unidade!\n");
		}
		else
		if(stricmp(comando,"goto") == 0)
		{
			if(auxListUnid != NULL)
			{
				if(aberto != NULL)
				{
					if(aberto->campos->pAtual != NULL)
					{
						num = atoi(resto);
						Goto(aberto,num);
					}
					else
						printf("\nVoce ainda nao inseriu um registro!\n");
				}
				else
					printf("\nVoce ainda nao abriu um arquivo!\n");
			}
			else
				printf("\nVoce ainda nao escolheu uma unidade!\n");
		}
		else
		if(stricmp(comando,"delete") == 0) 
		{//para dar um delete eu tenho que ter usado o goto
			if(auxListUnid != NULL)
			{
				if(aberto != NULL)
				{
					if(aberto->campos->pAtual != NULL)
					{
						if(stricmp(aberto->status->status,"true") == 0)
							Delete(aberto);
						else
							printf("\nAtual ja excluido!\n");
					}
					else
						printf("\nVoce ainda nao inseriu um registro!\n");
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
