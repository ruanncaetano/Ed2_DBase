#include<stdio.h>
#include<string.h>
#include<conio2.h>

//função para ser adicionada no trabalho
void tratandoComandos(char comandoDbase[50], char *comando, char *nome)
{
	int i=0, j=0;
	
//	tratando o comando "SET DEFAULT TO" para separar as coisas
	if(strcmp(comandoDbase,"SET DEFAULT TO C:") == 0 || strcmp(comandoDbase,"SET DEFAULT TO D:") == 0)
	{
		// Copiando "SET DEFAULT TO" para comando
	    for(i = 0; i < 15; i++)
	        comando[i] = comandoDbase[i];
	    comando[i] = '\0'; // Adiciona terminador nulo
	
	    // Copiando "C:" para nome
	    while(comandoDbase[i] != '\0')
	    {
	    	nome[j] = comandoDbase[i];
	    	j++;
	    	i++;
		}
	        
	    nome[j] = '\0'; // Adiciona terminador nulo
	}
	else //comandos com apenas uma palavra
	{
		while(comandoDbase[i] != ' ')
		{
			comando[i] = comandoDbase[i];
			i++;
		}
		comando[i] = ' '; //adiciona espaço ao final do comando
	    i++;
	    comando[i] = '\0'; //adiciona \0 para indicar o fim da string
	    
//	    retirando os espaços a mais 
	    while(comandoDbase[i] == ' ') //ignora espaços após o comando
        	i++;
        
        while(comandoDbase[i] != '\0') 
	    {
	        nome[j] = comandoDbase[i];
	        j++;
	        i++;
	    }
	    nome[j] = '\0'; //adiciona \0 para indicar o fim da string
	}
}

void tratandoStringGeral(char comandoDbase[50], char *comando, char *nome)
{
    int i=0, j=0, pos;

    pos = strlen(comandoDbase);

    printf("\nComandoDbase: %s",comandoDbase);
    printf("\nTamanho comandoDbase: %d\n",pos);

    while(comandoDbase[i] != ' ' && comandoDbase[i] != '\0') 
    {
        comando[i] = comandoDbase[i];
        i++;
    }
    comando[i] = ' '; //adiciona espaço ao final do comando
    i++;
    comando[i] = '\0'; //adiciona \0 para indicar o fim da string

    //copia o restante
//    while (comandoDbase[i] == ' ') //ignora espaços após o comando
//        i++;

    while(comandoDbase[i] != '\0') 
    {
        nome[j] = comandoDbase[i];
        j++;
        i++;
    }
    nome[j] = '\0'; //adiciona \0 para indicar o fim da string

    printf("\nComando: %s",comando);
    printf("\nNome do Arquivo: \"%s\"\n",nome);
}

void tratandoSetDefaultTo(char comandoDbase[50], char *comando, char *unid)
{
	int i, pos, j=0;
	
	pos = strlen(comandoDbase); //tamanho da string
	
	printf("\nComandoDbase: %s",comandoDbase);
	printf("\nTamanho comando comandoDbase: %d\n",pos);
	
	// Copiando "SET DEFAULT TO" para comando
    for(i = 0; i < 15; i++)
        comando[i] = comandoDbase[i];
    comando[i] = '\0'; // Adiciona terminador nulo

    // Copiando "C:" para unid
    while(comandoDbase[i] != '\0')
    {
    	unid[j] = comandoDbase[i];
    	j++;
    	i++;
	}
        
    unid[j] = '\0'; // Adiciona terminador nulo
		
	printf("\nComando: %s",comando);
	printf("\nTamanho comando comando: %d\n",pos);
	
	printf("\nUnidade: %s\n",unid);
	
	if(strcmp(comando,"SET DEFAULT TO ") == 0)
		printf("\nPrimeiro if ta ok!!!\n");
		
	if(strcmp(unid,"C:") == 0)
		printf("\nSegundo if ta ok!!!\n");
}


int main(void)
{
	char comandoDbase[50], comando[20], nome[40], unid[3];
	
	printf("\nComandoDbase: ");
	fflush(stdin);
	gets(comandoDbase);
	tratandoStringGeral(comandoDbase,comando,nome);
	printf("\nCOMANDO: %s",comando);
	printf("\nNOME: %s\n",nome);
	getch();
	
	printf("\nComandoDbase: ");
	fflush(stdin);
	gets(comandoDbase);
	tratandoSetDefaultTo(comandoDbase,comando,unid);
	printf("\nCOMANDO: %s",comando);
	printf("\nUNIDADE: %s",unid);
		
	getch();
}
