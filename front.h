#define tf 100

void limparExibicao()
{
	textcolor(15);
    textbackground(0);

  	for (int i = 1; i <= 22; i++) 
	{
        gotoxy(1, i);
        for (int j = 1; j <= 80; j++) 
		{ 
            printf(" ");
        }
    }
    
    gotoxy(1,1);
}

void imprimirComando(char *comando,int y)
{
	textcolor(0);
    textbackground(15);
	gotoxy(1, y+5);
	printf("%s", comando);
	textbackground(0);
	textcolor(15);	
}


void imprimirDisco(char *disco,int posy)
{
	char *pos = strchr(disco, '\n');
    if (pos) {
        *pos = '\0';
    }
    
	textcolor(0);
	textbackground(15);
	gotoxy(27, posy+5);
	printf("||<%s>||", disco);
	textbackground(0);
	textcolor(15);
}

void imprimirDBF(char *DBF)
{
	textcolor(0);
	textbackground(15);
	gotoxy(35, 23);
	printf("%s", DBF);
	textbackground(0);
	textcolor(15);
}

void imprimirREC(char *REC, char *TOTALREC, int y)
{
	textcolor(0);
	textbackground(15);
	gotoxy(65, y);
	if (strcmp(TOTALREC,"")){
		printf("||Rec: %s/%s", REC, TOTALREC);
	}else {
		printf("||Rec: %s", REC);
	}
	textbackground(0);
	textcolor(15);
}

void desenharLinha(int posy)
{

	textcolor(0);
    textbackground(15);
    for (int j = 1; j <= 80; j++) 
    { 
        gotoxy(j, posy+5); 
        printf(" ");
    }
    textbackground(0);
    textcolor(15);
}

void desenharMenu(int posy,char *comando, char *disco){
	desenharLinha(posy);
	imprimirComando(comando, posy);
	char REC[100] = "NONE";
	char TOTALREC[100] = "";
	imprimirREC(REC,TOTALREC,posy+5);
	imprimirDisco(disco,posy);
	textbackground(0);
	textcolor(15);
    gotoxy(1,2);
}
