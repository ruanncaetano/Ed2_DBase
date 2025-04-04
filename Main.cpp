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

int buscaFuncao(char *comando)
{
    char comandos[][20] = {
        "set default to", "create", "dir", "quit", "use", "list", "list structure",
        "list for", "append", "clear", "locate for", "goto", "display",
        "edit", "delete", "delete all", "recall", "set deleted", "pack", "zap"
    };

    int tamanho = sizeof(comandos) / sizeof(comandos[0]);
    for (int i = 0; i < tamanho; i++)
    {
        if (stricmp(comando, comandos[i]) == 0)
            return i; // Retorna o índice do comando encontrado
    }
    return -1;
}

void LerPromt(char leitura[50], char *comando, char *resto)
{
    char comando_composto[100] = "";
    char *copia = (char *)malloc((strlen(leitura) + 1) * sizeof(char));
    
    if(copia == NULL)
    {
        printf("\nErro ao alocar memória.\n");
        return;
    }

    strcpy(copia, leitura);
    char *inicio = copia;

    // Ignora espaços no início
    while(*inicio == ' ')
        inicio++;

    int posComando = -1;
    char temp[100] = "";
    char *fim = inicio;

    int terminou = 0; // flag pra controlar quando deve parar

    while(*fim != '\0' && !terminou)
    {
        // Move fim até o final da palavra
        while(*fim != ' ' && *fim != '\0')
            fim++;

        char backup = *fim;
        *fim = '\0';

        char tempAnterior[100];
        strcpy(tempAnterior, temp); // salva o anterior pra controle

        if(strlen(temp) > 0)
            strcat(temp, " ");
        
        strcat(temp, inicio);

        if (buscaFuncao(temp) != -1)
        {
            strcpy(comando_composto, temp);
            posComando = 1;
        }
        else if (posComando != -1)
        {
            // Já tinha comando válido antes e agora não é mais, para aqui
            terminou = 1;

            // Volta temp ao valor anterior, pois a concatenação atual não foi válida
            strcpy(temp, tempAnterior);
        }

        *fim = backup;

        if(!terminou && *fim != '\0')
        {
            inicio = fim + 1;
            while (*inicio == ' ')
                inicio++;

            fim = inicio;
        }
    }

    if (posComando == -1)
    {
        strcpy(comando, "");
        strcpy(resto, leitura);
    }
    else
    {
        strcpy(comando, comando_composto);

        int len = strlen(comando_composto);
        char *pos = leitura + len;

        while (*pos == ' ')
            pos++;

        strcpy(resto, pos);
    }

    free(copia);
}

void inserirCampo(celula *auxCelula, char type, int col, int lin)
{
	switch(type)
	{
		case 'N':
			gotoxy(col,lin);
			scanf("%f",&auxCelula->dados.valorN);
			break;
	    case 'L':
	    	gotoxy(col,lin);
			scanf("%c",&auxCelula->dados.valorL);
			break;
	    case 'C':
	    	gotoxy(col,lin);
			fflush(stdin);
			gets(auxCelula->dados.valorC);
			break;
	    case 'D':
	    	gotoxy(col,lin);
			fflush(stdin);
			gets(auxCelula->dados.valorD);
			break;
	    case 'M':
	    	gotoxy(col,lin);
			fflush(stdin);
			gets(auxCelula->dados.valorM);
			break;
	}
}

void append(arquivo *aberto, status **posStatus)
{
	system("cls");
	
	campo *auxCampo = aberto->campos;
	
	status *novoStatus = (status*)malloc(sizeof(status));
	strcpy(novoStatus->status,"true"); // 1 =True
	novoStatus->prox = NULL;
	
	int col=1, lin=1;
	
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
		
		gotoxy(col,lin);
		printf("%s ",auxCampo->fieldName);
		col += 15;
		inserirCampo(novaCelula,auxCampo->type,col,lin);
		col=1;
		lin++;

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
	    }
	}
    
}

void list(arquivo *aberto, status *posStatus)
{
    campo *auxCampo = aberto->campos;
    system("cls");

    int pos = 1, col = 1, lin = 1;
    char type;

    gotoxy(col, lin); // 1 - 1
    printf(". LIST");
    lin++;
    gotoxy(col, lin); // 1 - 2
    printf("Record# ");
    lin++;

    // --- Primeira coluna: numeração dos registros ativos ---
    celula *auxCelula = aberto->campos->pDados;
    status *auxStatus = posStatus;

    while (auxCelula != NULL && auxStatus != NULL)
    {
        if (stricmp(auxStatus->status, "true") == 0)
        {
            gotoxy(col, lin); // 1 - 3 em diante
            printf("%d", pos);
            lin++;
            pos++;
        }
        auxCelula = auxCelula->prox;
        auxStatus = auxStatus->prox;
    }

    // --- Colunas com os campos e valores ativos ---
    auxCampo = aberto->campos;
    while (auxCampo != NULL)
    {
        celula *auxCelula = auxCampo->pDados;
        auxStatus = posStatus;  // Resetar status pro início de novo
        col += 20;
        lin = 2;
        gotoxy(col, lin);
        printf("%s", auxCampo->fieldName);

        while (auxCelula != NULL && auxStatus != NULL)
        {
            if (stricmp(auxStatus->status, "true") == 0)
            {
                lin++;
                type = auxCampo->type;
                printUnion(auxCelula, type, col, lin);
            }
            auxCelula = auxCelula->prox;
            auxStatus = auxStatus->prox;
        }

        auxCampo = auxCampo->prox;
    }
}


// FALTA TERMINAR
//void listFor(arquivo *auxAberto, status *posStatus, char resto[50])
//{
//	campo *auxCampo = auxAberto->campos;
//	status *auxStatus = posStatus;
//	
//	char campo[50], registro[50];
//    int i = 0, j = 0;
//
////	lendo os campos
//    while(resto[i] != ' ' && resto[i] != '\0')
//        campo[j++] = resto[i++];
//    
//    campo[j] = '\0';
//
////	pulando os espaços
//    while(resto[i] == ' ')
//        i++;
//
////	lendo o resgistro
//    j = 0;
//    while(resto[i] != '\0' && j < 49)
//        registro[j++] = resto[i++];
//
//    registro[j] = '\0';
//
////	procurando o campo
//    while(stricmp(auxCampo->fieldName,campo) != 0 && auxCampo != NULL)
//    	auxCampo = auxCampo->prox
//    	
//    if(stricmp(auxCampo->fieldName,campo) != 0) //achou
//    {
//    	celula *auxCelula = auxCampo->pDados;
//    	
//    	switch(auxCampo->type)
//    	{
//    		case 'N':
////    			se for um numero eu tenho que tranformar em int
//    			int num = atoi(registro);
//    			procurando o numero passado no resgitro, e se achar vai exibindo
//				while(auxCelula->dados.valorN != num && auxCelula != NULL)
//				{
//					if(auxCelula == registro)
//			        {
////			            aqui eu vou exibir os registros
//			        }
//			        auxCelula = auxCelula->prox;
//				}
//		        
//				break;
//	        case 'L':
//				
//				break;
//	        case 'C':
//				
//				break;
//	        case 'D': 
//				
//				break;
//	        case 'M': 
//				
//				break;
//		}
//    }
//    else
//    	printf("\nO campo não foi encontrado!");
//}

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

void display(arquivo *auxAberto, status *posStatus)
{
    campo *auxCampo = auxAberto->campos;
    celula *auxCelula;
    status *auxStatus = posStatus;

    system("cls");

    int pos = 1, col = 1, lin = 1;
    char type;

    gotoxy(col, lin);
    printf(". DISPLAY");
    lin++;
    gotoxy(col, lin);
    printf("Record# ");

// 	Exibir nomes dos campos
    campo *tempCampo = auxAberto->campos;
    while(tempCampo != NULL)
    {
        col += 20;
        gotoxy(col, lin);
        printf("%s", tempCampo->fieldName);
        tempCampo = tempCampo->prox;
    }
    lin++;

	if(auxStatus != NULL && stricmp(auxStatus->status,"true") == 0)
	{
		// obter a posição baseada em um dos campos
	    campo *baseCampo = auxAberto->campos;
	    celula *aux = baseCampo->pDados;
	    pos = 1;
	    while(aux != baseCampo->pAtual)
	    {
	        aux = aux->prox;
	        pos++;
	    }
	
//		mostrar número do registro
	    col = 1;
	    gotoxy(col, lin);
	    printf("%d", pos);
	
//		mostrar os dados de cada campo no pAtual
	    auxCampo = auxAberto->campos;
	    while(auxCampo != NULL)
	    {
	        col += 20;
	        auxCelula = auxCampo->pAtual;
	        type = auxCampo->type;
	
	        if(auxCelula != NULL)
	            printUnion(auxCelula, type, col, lin);
	
	        auxCampo = auxCampo->prox;
	    }
	}
}

void Goto(arquivo *auxAberto, status **posStatus, int pos)
{
    campo *auxCampo = auxAberto->campos;
    status *auxStatus = auxAberto->status;

    int cont = 0, totalRegistro = 0;

    //quantidade de registros ativos
    while(auxStatus != NULL)
    {
        if(stricmp(auxStatus->status, "true") == 0)
            totalRegistro++;
        auxStatus = auxStatus->prox;
    }

    if(pos > 0 && pos <= totalRegistro)
    {
        auxStatus = auxAberto->status; // Resetando para o início

        // Sempre voltar pAtual para o início
        while(auxCampo != NULL)
        {
            auxCampo->pAtual = auxCampo->pDados;
            auxCampo = auxCampo->prox;
        }

        auxCampo = auxAberto->campos;
        *posStatus = auxStatus;

        // avançando até encontrar o registro ativo da posição desejada
        cont = 0;
        while(auxStatus != NULL && cont < pos)
        {
            if(stricmp(auxStatus->status, "true") == 0)
                cont++;

            if(cont < pos)
            {
                auxStatus = auxStatus->prox;

                // Avançar todos os ponteiros pAtual dos campos
                auxCampo = auxAberto->campos;
                while(auxCampo != NULL)
                {
                    auxCampo->pAtual = auxCampo->pAtual->prox;
                    auxCampo = auxCampo->prox;
                }
            }
        }

        // Caso encontrou a posição correta
        *posStatus = auxStatus;

        if(*posStatus == NULL)
            printf("\nNenhum registro ativo encontrado!!!\n");
    }
    else
    {
        printf("\nQuantidade inválida!!!\n");
    }
}


void edit(arquivo *auxAberto)
{
	campo *auxCampo = auxAberto->campos;
	celula *auxCelula;
	
	int col=1, lin=1;
	char type, campo[50];
	
	system("cls");
	
	//exibindo os campos do resgistro em uso
	while(auxCampo != NULL)
	{
		auxCelula = auxCampo->pAtual;
		gotoxy(col,lin);
		printf("%s",auxCampo->fieldName);
		col += 15;
		type = auxCampo->type;
		printUnion(auxCelula,type,col,lin);
		col = 1;
		lin++;
		auxCampo = auxCampo->prox;
	}
	
	//perguntando qual registro ele quer alterar
	auxCampo = auxAberto->campos; //voltando do inicio
	col = 1;
	lin++;
	gotoxy(col,lin);
	printf("Qual campo deseja editar: ");
	fflush(stdin);
	gets(campo);
	lin++;
	
	//buscando o campo que o usuario quer editar
	while(stricmp(campo,auxCampo->fieldName) != 0 && auxCampo != NULL)
		auxCampo = auxCampo->prox;
		
	//verificando se achou
	if(stricmp(campo,auxCampo->fieldName) == 0) //achou
	{
		//exibindo apenas o campo a ser editado
		auxCelula = auxCampo->pAtual;
		gotoxy(col,lin);
		printf("%s",auxCampo->fieldName);
		col += 15;
		type = auxCampo->type;
		printUnion(auxCelula,type,col,lin);
		col = 1;
		lin++;
		
	//fazer uma função inserir campo que serve tanto para o edit quanto para o append
		lin++;
		gotoxy(col,lin);
		printf("%s",auxCampo->fieldName);
		col += 15;
		inserirCampo(auxCelula,type,col,lin);
		
		lin++;
		col=1;
		printf("Registro alterado com sucesso!");
	}
	else
	{
		gotoxy(col,lin);
		printf("Esse campo nao existe!");
	}
}

void Delete(arquivo *auxAberto, status **posStatus)
{
	int col=1, lin=1;
	
	system("cls");
	
    strcpy((*posStatus)->status, "false");

    //reiniciando ponteiros
    status *auxStatus = auxAberto->status;
    campo *auxCampo = auxAberto->campos;

    //resetando todos os pAtual para o início (pDados)
    while(auxCampo != NULL)
    {
        auxCampo->pAtual = auxCampo->pDados;
        auxCampo = auxCampo->prox;
    }

    auxStatus = auxAberto->status;

    while(auxStatus != NULL && stricmp(auxStatus->status, "true") != 0)
    {
        // Avançando todos os pAtual
        auxCampo = auxAberto->campos;
        while(auxCampo != NULL)
        {
            if(auxCampo->pAtual != NULL)
                auxCampo->pAtual = auxCampo->pAtual->prox;
            auxCampo = auxCampo->prox;
        }
        auxStatus = auxStatus->prox;
    }

    *posStatus = auxStatus;

    gotoxy(col,lin);
    printf("Registro deletado!!!");
}

void deleteAll(arquivo *auxAberto)
{
	status *auxStatus = auxAberto->status; //apontando para o cabeça de status
	
	system("cls");
	
	int col=1, lin=1;
	
	while(auxStatus != NULL)
	{
		strcpy(auxStatus->status,"false");
		auxStatus = auxStatus->prox;
	}
	
	gotoxy(col,lin);
	printf("Todos os registro foram deletados (logicamente)");
}

//void pack(arquivo *auxAberto, status **posStatus) {
//    int cont = 0;
//    campo *atualCampo = auxAberto->campos; // Ponteiro para a cabeça da lista de campos
//    celula *atualCelula, *auxCelula; // Ponteiros para manipular a lista de células
//    status *atualStatus = auxAberto->status; // Ponteiro para a cabeça da lista de status
//    status *auxStatus; // Ponteiro auxiliar para exclusão
//
//    while (atualStatus != NULL) {
//        if (stricmp(atualStatus->status, "false") == 0) {
//            if (cont == 0) { // Se for excluir o primeiro registro
//                auxStatus = atualStatus;
//                atualStatus = atualStatus->prox;
//                auxAberto->status = atualStatus;
//
//                atualCampo = auxAberto->campos;
//                while (atualCampo != NULL) {
//                    atualCelula = atualCampo->pDados;
//                    auxAberto->campos->pDados = atualCelula->prox;
//                    auxCelula = atualCelula;
//                    atualCelula = atualCelula->prox;
//                    free(auxCelula);
//                    atualCampo = atualCampo->prox;
//                }
//            } else { // Caso não seja o primeiro registro
//                auxStatus = atualStatus;
//                atualStatus = atualStatus->prox;
//
//                atualCampo = auxAberto->campos;
//                while (atualCampo != NULL) {
//                    auxCelula = atualCelula;
//                    atualCelula = atualCelula->prox;
//                    free(auxCelula);
//                    atualCampo = atualCampo->prox;
//                }
//            }
//            free(auxStatus);
//        }
//        
//        atualCampo = auxAberto->campos;
//        while (atualCampo != NULL) {
//            auxCelula = atualCelula;
//            atualCelula = atualCelula->prox;
//            atualCampo = atualCampo->prox;
//        }
//        atualCampo = auxAberto->campos;
//        atualStatus = atualStatus->prox;
//        cont++;
//    }
//
//    // Atualiza `posStatus` para apontar ao primeiro status "true"
//    atualStatus = auxAberto->status;
//    while (atualStatus != NULL && stricmp(atualStatus->status, "false") == 0) {
//        atualStatus = atualStatus->prox;
//        atualCampo = auxAberto->campos;
//        while (atualCampo != NULL) {
//            atualCampo->pAtual = atualCampo->pAtual->prox;
//            atualCampo = atualCampo->prox;
//        }
//    }
//
//    if (atualStatus == NULL) {
//        *posStatus = NULL;
//        atualCampo->pAtual = NULL;
//    } else {
//        *posStatus = atualStatus; // Atualizando o ponteiro do status
//    }
//}


int main(void)
{ 
	char comandoDbase[50], unid[3];
	char nomeDBF[50], data[11], hora[6];
	
	char comando[50],resto[50];
	
	int num=0;
	
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
		if(stricmp(comando, "list structure") == 0)
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
						list(aberto,aberto->status);
					else
						printf("\nVoce ainda nao colocou dados no arquivo!\n");
				}
				else
					printf("\nVoce ainda nao abriu um arquivo!\n");
			}
			else
				printf("\nVoce ainda nao escolheu uma unidade!\n");
		}
//		else
//		if(stricmp(comando,"list for") == 0)
//		{
//			if(auxListUnid != NULL)
//			{
//				if(aberto != NULL)
//				{
//					if(aberto->campos->pDados != NULL)
//						listFor(aberto,posStatus,resto);
//					else
//						printf("\nVoce ainda nao colocou dados no arquivo!\n");
//				}
//				else
//					printf("\nVoce ainda nao abriu um arquivo!\n");
//			}
//			else
//				printf("\nVoce ainda nao escolheu uma unidade!\n");
//		}
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
				// pega o resto de tudo, que é a informação que quer buscar
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
						display(aberto,posStatus);
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
						Goto(aberto,&posStatus,num);
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
		{
			if(auxListUnid != NULL)
			{
				if(aberto != NULL)
				{
					if(aberto->campos->pAtual != NULL)
					{
						Delete(aberto,&posStatus);
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
		if(stricmp(comando,"delete all") == 0)
		{
			if(auxListUnid != NULL)
			{
				if(aberto != NULL)
				{
					if(aberto->campos->pAtual != NULL)
					{
						deleteAll(aberto);
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
		if(stricmp(comando,"edit") == 0)
		{
			if(auxListUnid != NULL)
			{
				if(aberto != NULL)
				{
					if(aberto->campos->pAtual != NULL)
					{
						if(stricmp(aberto->status->status,"true") == 0)
							edit(aberto);
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
		if(stricmp(comando,"pack") == 0)
		{
			if(auxListUnid != NULL)
			{
				if(aberto != NULL)
				{
					if(aberto->campos->pAtual != NULL)
						pack(aberto,&posStatus);
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

//essa função le o prompt com vetor e não ponteiro
//void LerPromt(char leitura[50], char *comando, char *resto)
//{
//    char comando_composto[100] = "";
//    char copia[100];
//    char inicio[20] = "";
//    char pos[20] = "";
//    
//    strcpy(copia, leitura);
//
//    int i = 0, j = 0;
//
//    // Ignora espaços no início
//    while (copia[i] == ' ')
//        i++;
//
//    // Copia a parte útil para 'inicio'
//    while (copia[i] != '\0' && j < 19)
//    {
//        inicio[j++] = copia[i++];
//    }
//    inicio[j] = '\0';
//
//    int posComando = -1;
//    char temp[100] = "";
//    int lenInicio = strlen(inicio);
//    int indice = 0;
//    int terminou = 0;
//
//    while (indice < lenInicio && !terminou)
//    {
//        // Pega uma palavra
//        char palavra[50] = "";
//        int k = 0;
//
//        // Ignora espaços
//        while (inicio[indice] == ' ' && indice < lenInicio)
//            indice++;
//
//        // Copia a palavra até espaço ou fim
//        while (inicio[indice] != ' ' && inicio[indice] != '\0' && k < 49)
//        {
//            palavra[k++] = inicio[indice++];
//        }
//        palavra[k] = '\0';
//
//        // Guarda o estado anterior
//        char tempAnterior[100];
//        strcpy(tempAnterior, temp);
//
//        if (strlen(temp) > 0)
//            strcat(temp, " ");
//
//        strcat(temp, palavra);
//
//        if (buscaFuncao(temp) != -1)
//        {
//            strcpy(comando_composto, temp);
//            posComando = 1;
//        }
//        else if (posComando != -1)
//        {
//            strcpy(temp, tempAnterior); // volta ao último comando válido
//            terminou = 1; // encerra o loop sem usar break
//        }
//    }
//
//    if (posComando == -1)
//    {
//        strcpy(comando, "");
//        strcpy(resto, leitura);
//    }
//    else
//    {
//        strcpy(comando, comando_composto);
//        int len = strlen(comando_composto);
//        int r = 0, c = len;
//
//        // Pula espaços
//        while (leitura[c] == ' ')
//            c++;
//
//        while (leitura[c] != '\0' && r < 19)
//        {
//            pos[r++] = leitura[c++];
//        }
//        pos[r] = '\0';
//
//        strcpy(resto, pos);
//    }
//}


//Função que o Gui fez para buscar se um comando é valido
//int buscaFuncao(char *comando, char *resto)
//{
//	char comandos[][20] = {
//		"set default to", "create", "dir", "quit", "use", "list", "list structure",
//        "list for","append",  "clear", "locate for", "goto", "display", 
//        "edit", "delete", "delete all" "recall", "set deleted", "pack", "zap"
//	};
//	
//	int tamanho = sizeof(comandos) / sizeof(comandos[0]);
//	for(int i=0; i<tamanho; i++)
//	{
//		if(stricmp(comando,comandos[i]) == 0)
//			return 1;
//	}
//	return 0;
//}

// Função para ler e tokenizar a string
//void LerPromt(char leitura[50], char *comando, char *resto) RUAN
//{
//    // Variável para armazenar o comando composto
//	char comando_composto[100] = "";
//	
//	// Aloca memória para a cópia da string
//    char *copia = (char *)malloc((strlen(leitura) + 1) * sizeof(char)); // não sei se isso esta certo
//    if(copia == NULL)
//	{
//        printf("\nErro ao alocar memoria.\n");
//    }
//	else
//	{
//	    // Copia a string original para a cópia
//	    strcpy(copia, leitura);
//	
//	    // Ignorar espaços no começo
//	    char *inicio = copia;
//	    while(*inicio == ' ')
//		{
//	        inicio++;
//	    }
//		char *fim_palavra = inicio;
//	    // Pega a primeira palavra
//	    while(*fim_palavra != ' ' && *fim_palavra != '\0')
//		{
//	        fim_palavra++;
//	    }
//	
//	    // Separa a primeira palavra
//	    if(*fim_palavra == ' ')
//		{
//	        *fim_palavra = '\0'; // Termina a primeira palavra
//	        strcat(comando_composto, inicio);
//	        inicio = fim_palavra + 1;
//	
//	        // Remove espaços iniciais do restante
//	        while(*inicio == ' ')
//			{
//	            inicio++;
//	        }
//	    }
//		else
//		{
//	        strcat(comando_composto, inicio);
//	        inicio = fim_palavra; // Fim da string
//	    }
//	
//	    // Verifica se a primeira palavra é um comando válido
//	    int buscaSeE = buscaFuncao(comando_composto, inicio);
//	
//	    // Loop para concatenar mais palavras enquanto a busca retornar 0
//	    while(buscaSeE == 0 && *inicio != '\0')
//		{
//	        // Pega a próxima palavra
//	        fim_palavra = inicio;
//	        while(*fim_palavra != ' ' && *fim_palavra != '\0')
//			{
//	            fim_palavra++;
//	        }
//	
//	        // Concatena a próxima palavra ao comando composto
//	        if(*fim_palavra == ' ')
//			{
//	            *fim_palavra = '\0'; // Termina a próxima palavra
//	            strcat(comando_composto, " ");
//	            strcat(comando_composto, inicio);
//	            inicio = fim_palavra + 1;
//	
//	            // Remove espaços iniciais do restante
//	            while(*inicio == ' ')
//				{
//	                inicio++;
//	            }
//	        }
//			else
//			{
//	            strcat(comando_composto, " ");
//	            strcat(comando_composto, inicio);
//	            inicio = fim_palavra; // Fim da string
//	        }
//	
//	        // Verifica se o comando composto é válido
//	        buscaSeE = buscaFuncao(comando_composto, inicio);
//	    }
//		strcpy(comando,comando_composto);
//		strcpy(resto,inicio);
//	    // Libera a memória alocada para a cópia
//	    free(copia);
//	}
//}
