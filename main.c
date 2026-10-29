///@file main.c
///@author Daniele De Lazzari 899082

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

///Funzione per stampare a schermo il labirinto
///
///Prende in ingresso il labirinto e le sue dimensioni, e codifica ogni int nel suo char corrispettivo stampandole a schermo
///@param lab : matrice del labirinto
///@return void
void stampa(int **lab, int n, int m){
	int i, j;
	for(i=0; i<n; i++){
		printf("\n\t");
		for(j=0; j<m; j++){
			char ch=248;
			switch(lab[i][j]){
				case 1:
					printf("#");
				break;
				case 0:
					printf(" ");
				break;
				case 8:
					printf("o");
				break;
				case 9:
					printf("_");
				break;
				case 2:
					printf("$");
				break;
				case 3:
					printf("!");
				break;	
				case 4:
					printf(".");
				break;
				case 5:
					printf("*");
				break;
				case 7:
					printf("T");
				break;
				case 6:
					printf("%c", ch);
				default:
					printf("");
				break;
			}
		}
	}
}

///Funzione di ricezione a schermo del labirinto
///
///Questa funzione serve per prendere da tastiera il labirinto e restituire la matrice con i char convertiti in int e altri parametri
///@param sx assieme a @param sy memorizza la posizione di partenza dello snake
///@param money memorizza il numero di monete nel labirinto
///@param fx assieme a @param fy memorizza le coordinate di destinazione (fine labirinto)
///@param walls tiene il numero di pareti nel labirinto
///@param tri tiene a mente il numero di trivelle
void inserisci_labirinto(int **lab, int n, int m, int *sx, int *sy, int *money, int *fx, int *fy, int *walls, int *tri){
	int i;
	int j;
	
	char caratteri[m];
	gets(caratteri);
	for(i=0; i<n; i++){
		gets(caratteri);
		for(j=0;j<m;j++){
			switch(caratteri[j]){
				case '#':
					lab[i][j]=1;
					*walls++;
				break;
				case ' ':
					lab[i][j]=0;
				break;
				case 'o':
					lab[i][j]=8;
					*sx=i;
					*sy=j;
				break;
				case 'T':
					lab[i][j]=7;
					*tri++;
				break;
				case '_':
					lab[i][j]=9;
					*fx=i;
					*fy=j;
				break;
				case '$':
					lab[i][j]=2;
					*money++;
				break;
				case '!':
					lab[i][j]=3;
				break;
				default:
					lab[i][j]=88;
				break;
			}
		}
	}
}

///Funzione di copia del labirinto
///
///Questa funzione prende un labirinto e lo copia su un'altro
///@param lab è il labirinto da copiare
///@param lab2 è il labirinto di destinazione
void copia_lab(int **lab, int **lab2, int n, int m){
	int i, j;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			lab2[i][j]=lab[i][j];
		}
	}
}


///Funzione per la direzione iniziale
///
///Funzione che calcola la direzione verso cui lo snake deve guardare inizialmente
///@param rot è  la direzione verso cui sta guardando lo snake
///@param v è il vettore con i dati di posizione e punteggio
void inizio(int *v, int *rot, int n, int m){
	if(v[0]==0){
		rot[1]=1;
		rot[0]=0;
		rot[3]=rot[5]=0;
		rot[2]=rot[4]=1;
	}else{
		if(v[0]==n){
			rot[1]=-1;
			rot[0]=0;
			rot[3]=rot[5]=0;
			rot[2]=rot[4]=-1;	
		}else{
			rot[1]=0;
			if(v[1]==0){
				rot[0]=1;
				rot[3]=rot[5]=1;
				rot[2]=rot[4]=0;
			}else{
				rot[0]=-1;
				rot[3]=rot[5]=-1;
				rot[2]=rot[4]=0;
			}
		}
	}
}

///Funzione per la posizione iniziale
///
///Questa funzione, collegata alla funzione inizio, memorizza le coordinate di inizio e calcola la direzione verso cui deve guardare lo snake inizialmente
///@param lab è il labirinto
///@see inizio()
void coord_simple(int **lab, int *rot, int n, int m, int *v){
	int i;
	int j;
	int tr, tc;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			if(lab[i][j]==8){
				v[0]=i;
				v[1]=j;
			}
			if(lab[i][j]==9){	
				tr=i;
				tc=j;
			}
		}
	}
	inizio(v, rot, n, m);
}

///Funzione di cambio posizione (modalità interattiva)
///
///Funzione per spostare le coordinate di posizione dello snake
///@param a e @param b sono i valori per capire verso dove deve traslare (possono assumero valori -1, 0, 1)
void sposta(int **lab, int *v, int a, int b){
	int i=v[0];
	int j=v[1];
	lab[i+a][j+b]=8;
	lab[i][j]=0;
	
	v[0]=i+a;
	v[1]=j+b;
	v[2]--;
}

///Funzione per possibilià snake (modalità interattiva)
///
///Funzione che presa in ingresso la direzione verso cui si deve andare analizza le casistiche
///@param mosse è l'array di valori char che conserva le mosse fatte dal giocatore
///@see sposta()
void casistica(int **lab, int *v, int a, int b, char *mosse){
	int i=v[0];
	int j=v[1];
	int point=v[2];
	int x=lab[i+a][j+b];
	switch(x){
		case 1:
			v[3]=0;
			printf("\nNon puoi passare attraverso le pareti!\n");
			if(v[5]==0){
				mosse[v[4]]='\0';
				v[4]--;
			}else{
				sposta(lab, v, a, b);
				v[5]--;
			}
			
		break;
		case 0:
			v[3]=0;
			sposta(lab, v, a, b);
		break;
		case 2:
			v[2]=point+10;
			v[3]=0;
			sposta(lab, v, a, b);
			v[6]++;
		break;
		case 3:
			v[2]=point/2;
			v[3]=0;
			sposta(lab, v, a, b);
			v[6]==v[6]/2;
			if(v[6]==0){
				v[6]++;
			}
		break;
		case 9:
			v[2]--;
			v[3]=1;
			sposta(lab, v, a, b);
		break;
		case 7:
			v[3]=0;
			sposta(lab, v, a, b);
			v[5]=v[5]+3;
		break;
		default:
			v[3]=3;
		break;
	}
}

///Funzione ricezione mossa (modalità interattiva)
///
///Questa funzione prende in ingresso la mossa che l'utente vuole fare e fa analizzare le possibilità dalla funzione casistica()
///@see casistica()
void movimento(int **lab, int *v, char *mosse){
	int a=0, b=0;
	printf("\nChe mossa vuoi fare\?: ");
	char l;
	scanf(" %c", &l);
	mosse[v[4]]=l;
	lab[0][0]=1;
	switch(mosse[v[4]]){
		case 'n': case 'N':
			a=-1;
			b=0;
			casistica(lab, v, a, b, mosse);
		break;
		case 'w': case 'W':
			a=0;
			b=-1;
			casistica(lab, v, a, b, mosse);
		break;
		case 's': case 'S':
			a=1;
			b=0;
			casistica(lab, v, a, b, mosse);
		break;
		case 'e': case 'E':
			a=0;
			b=1;
			casistica(lab, v, a, b, mosse);
		break;
		default:
			printf("\nL\'input non e\' valido\nSe desideri continuare digita 1, altrimenti verrai fatto uscire dalla modalita\' interattiva:\n-");
			int esc;
			scanf("%d", &esc);
			if(esc==1){
				v[3]=0;
				mosse[v[4]]='\0';
				v[4]--;
				printf("\n");
			}else{
				v[3]=5;
				mosse[v[4]]='\0';
				v[4]--;
			}
		break;
	}
	v[4]++;
}

///Funzione di calcolo rotazione
///
///Funzione che calcola la rotazione di novanta gradi della direzione dello snake
void rotazione_default(int *rot){
	rot[0]=rot[2];
	rot[1]=rot[3];
	rot[2]=(rot[4])%4-1;
	rot[3]=(rot[5])%4-1;
	rot[2]=rot[2]%2;
	rot[3]=rot[3]%2;
	rot[4]=(rot[4]+1); 
	rot[5]=(rot[5]+1);
	rot[2]=-rot[2];
	rot[3]=-rot[3];
}

///Funzione numero di rotazioni (AI)
///
///Funzione prende in ingresso il numero di rotazioni da far fare alla funzione rotazione_default()
///@param num è il numero di rotazioni
///@see rotazione_default
void rotazione(int **lab, int *v, int *rot, int num){
	int i;
	for(i=0;i<num;i++){
		rotazione_default(rot);
	}
}

///Funzione memorizzazione mossa
///
///Funzione che prende in ingresso la direzione verso cui si deve spostare lo snake (nell'array rot) e memorizza in @param mosse, la mossa corrispettiva
void wasd(int *v, char *mosse, int *rot){
	if(rot[0]==1){
		mosse[v[4]]='S';
	}else{
		if(rot[0]==0){
			if(rot[1]==1){
				mosse[v[4]]='E';
			}else{
				mosse[v[4]]='W';
			}
		}else{
			mosse[v[4]]='N';
		}
	}
}

///Funzione di casistica (AI)
///
///Funzione che modifica i punteggi dello snake a seconda del blocco in cui sta andando
void p_algoritmo(int *v, int *rot, int **lab){
	switch(lab[v[0]+rot[0]][v[1]+rot[1]]){
		case 0:
			v[3]=0;
		break;
		case 9:
			v[3]=1;
		break;
		case 2:
			v[3]=0;
			v[2]=v[2]+10;
		break;
		case 3:
			v[3]=0;
			v[2]=v[2]/2;
		break;
		case 4:
			v[3]=0;
		break;
		case 7:
			v[6]=v[6]+3;
			v[3]=0;
		break;
		case 1:
			v[3]=0;
		break;
		default:
			v[3]=3;
		break;
	}
}


///Funzione che trasla lo snake (AI)
///
///Funzione che trasla di 1 in una delle 4 direzioni la testa dello snake
void sposta_algoritmo(int **lab, int *v, int *rot){
	lab[v[0]+rot[0]][v[1]+rot[1]]=8;
	lab[v[0]][v[1]]=4;
	
	v[0]=v[0]+rot[0];
	v[1]=v[1]+rot[1];
	
	
	v[4]++;
}

///Funzione di funzionamento del primo algoritmo
///
///Questa funzione è il metodo di funzionamento dell'algoritmo della parete destra
///@see rotazione()
///@see wasd()
///@see p_algoritmo()
///@see sposta_algoritmo()
void algoritmo1(int **lab, int *v, int n, int m, char *mosse, int *rot){
	int num;
	int mom=0;
	int var=0;
	int i;
	
	do{
		if(lab[v[0]+rot[0]][v[1]+rot[1]]!=1&&lab[v[0]+rot[0]][v[1]+rot[1]]!=5&&(lab[v[0]+rot[0]][v[1]+rot[1]]!=4||rot[6]==1)){
			num=1;
			rotazione(lab, v, rot, num);
			if(lab[v[0]+rot[0]][v[1]+rot[1]]==1||lab[v[0]+rot[0]][v[1]+rot[1]]==5||(lab[v[0]+rot[0]][v[1]+rot[1]]==4||rot[6]==1)){
				num=3;
				rotazione(lab, v, rot, num);
				wasd(v, mosse, rot);
				p_algoritmo(v, rot, lab);
				sposta_algoritmo(lab, v, rot);
				mom=1;
			}
		}else{
			if(lab[v[0]+rot[0]][v[1]+rot[1]]==1||lab[v[0]+rot[0]][v[1]+rot[1]]==5){
				num=1;
				rotazione(lab, v, rot, num);
				if(lab[v[0]+rot[0]][v[1]+rot[1]]==1||lab[v[0]+rot[0]][v[1]+rot[1]]==5){
					num=2;
					rotazione(lab, v, rot, num);
				}
			}else{
				if(lab[v[0]+rot[0]*2][v[1]+rot[1]*2]==1||lab[v[0]+rot[0]*2][v[1]+rot[1]*2]==5){
					num=2;
					rotazione(lab, v, rot, num);
					rot[6]=1;
				}else{
					wasd(v, mosse, rot);
					p_algoritmo(v, rot, lab);
					sposta_algoritmo(lab, v, rot);
					mom=1;
					rot[6]=1;
				}
			}
		}
	}while(mom==0);
	
	
	if(rot[6]==1){
		lab[v[0]-rot[0]][v[1]-rot[1]]=5;
		v[4]--;
		mosse[v[4]]='\0';
		v[4]--;
	}
	
	if(lab[v[0]+rot[0]][v[1]+rot[1]]!=4){
		rot[6]=0;
	}

	
	
	if(rot[6]==1){
		var=1;
	}
	num=1;
	rotazione(lab, v, rot, num);
	if(lab[v[0]+rot[0]][v[1]+rot[1]]!=1&&lab[v[0]+rot[0]][v[1]+rot[1]]!=5&&lab[v[0]+rot[0]][v[1]+rot[1]]!=4){
		var++;
	}
	num=2;
	rotazione(lab, v, rot, num);
	if(lab[v[0]+rot[0]][v[1]+rot[1]]!=1&&lab[v[0]+rot[0]][v[1]+rot[1]]!=5&&lab[v[0]+rot[0]][v[1]+rot[1]]!=4){
		var++;
	}
	num=1;
	rotazione(lab, v, rot, num);
	
	if(var==3){
		num=1;
		rotazione(lab, v, rot, num);
		
		rot[6]=0;
	}
	
	for(i=0;i<4;i++){
		if(lab[v[0]+rot[0]][v[1]+rot[1]]==9){
			wasd(v, mosse, rot);
			p_algoritmo(v, rot, lab);
			sposta_algoritmo(lab, v, rot);
			i=4;
		}
		num=1;
		rotazione(lab, v, rot, num);
	}
	
	
	if(v[4]-1==0){
		lab[v[0]-rot[0]][v[1]-rot[1]]=5;
	}
}

///Funzione start primo algoritmo
///
///Funzione che prepara e fa partire il primo algoritmo di risoluzione
///@see algoritmo1()
///@see coord_simple()
///@see stampa()
void algoritmo1_ext(int **lab, int n, int m){
	int *v;
	v=malloc(sizeof(int)*7);

	char *mosse;
	mosse=malloc(sizeof(char));
	
	int *rot;
	rot=malloc(sizeof(int)*8);
	
	
	int p=0;
	int nm=0;
	int F;
	int r;
	int c;
	int k;
	
	
	v[2]=1000;
	v[3]=0;
	v[4]=0;
	v[6]=0;
	rot[6]=0;
	rot[7]=0;
	k=3;
	
	coord_simple(lab, rot, n, m, v);
	
	
	int i,j;
	for(i=0;i<n;i++){
		for(j=0; j<m;j++){
			if(lab[i][j]==7){
				lab[i][j]=0;
			}
		}
	}
	for(i=0;i<4;i++){
		mosse[i]='\0';
	}
	
	do{
		algoritmo1(lab, v, n, m, mosse, rot);
		if(v[4]>=k-1){
			k=k+10;
			mosse=realloc(mosse, k*sizeof(char));
		}
	}while(v[3]==0);
	
	mosse[v[4]]='\0';
	switch(v[3]){
		case 1:
			stampa(lab, n, m);
			printf("\nIl computer ha trovato il percorso con queste mosse:\n\t");
			for(i=0;i<v[4];i++){
				switch(mosse[i]){
					case 's':
						printf("S");
					break;
					case 'w':
						printf("N");
					break;
					case 'a':
						printf("W");
					break;
					case 'd':
						printf("E");
					break;
					default:
						printf("-");
					break;
				}
			}
		break;
		default:
			printf("\n\tERROR\n");
		break;
	}
}

///Funzione di funzionamento del secondo algoritmo
///
///Questa funzione è il metodo di funzionamento dell'algoritmo che procede fino ad incontrare una parete
///@see rotazione()
///@see wasd()
///@see p_algoritmo()
///@see sposta_algoritmo()
void algoritmo2(int **lab, int *v, int n, int m, char *mosse, int *rot){
	int num;
	int mom=0;
	int pv;
	int var=0;
	int px;
	int py;
	do{
		if((lab[v[0]+rot[0]][v[1]+rot[1]]!=1&&lab[v[0]+rot[0]][v[1]+rot[1]]!=5&&(lab[v[0]+rot[0]][v[1]+rot[1]]!=4||rot[6]==1)&&rot[7]==0)||(rot[7]==1&&lab[v[0]+rot[0]][v[1]+rot[1]]==4)){
			wasd(v, mosse, rot);
			p_algoritmo(v, rot, lab);
			sposta_algoritmo(lab, v, rot);
			mom=1;
		}else{
			if(lab[v[0]+rot[0]][v[1]+rot[1]]==1||lab[v[0]+rot[0]][v[1]+rot[1]]==5){
				num=1;
				rotazione(lab, v, rot, num);
				if(lab[v[0]+rot[0]][v[1]+rot[1]]==1||lab[v[0]+rot[0]][v[1]+rot[1]]==5){
					num=2;
					rotazione(lab, v, rot, num);
				}
			}else{
				if(lab[v[0]+rot[0]*2][v[1]+rot[1]*2]==1||lab[v[0]+rot[0]*2][v[1]+rot[1]*2]==5){
					num=2;
					rotazione(lab, v, rot, num);
					rot[6]=1;
				}else{
					if(lab[v[0]+rot[0]*2][v[1]+rot[1]*2]==4){
						wasd(v, mosse, rot);
						p_algoritmo(v, rot, lab);
						sposta_algoritmo(lab, v, rot);
						mom=1;
						rot[6]=1;
					}else{
						px=v[0]+rot[0];
						py=v[1]+rot[1];
						num=2;
						rotazione(lab, v, rot, num);
						rot[6]=1;
						rot[7]=1;
					}
					
				}
			}
		}
	}while(mom==0);
	
	
	if(rot[6]==1){
		lab[v[0]-rot[0]][v[1]-rot[1]]=5;
		v[4]--;
		mosse[v[4]]='\0';
		v[4]--;
	}
	
	if(px==v[0]&&py==v[1]&&rot[7]==1){
		num=1;
		rotazione(lab, v, rot, num);
		if(lab[v[0]+rot[0]][v[1]+rot[1]]==5){
			num=2;
			rotazione(lab, v, rot, num);
		}
		rot[7]=0;
		rot[6]=0;
		px=0;
		py=0;
	}
	
	
	
	
	
	if(lab[v[0]+rot[0]][v[1]+rot[1]]!=4&&rot[7]==0&&rot[7]==0){
		rot[6]=0;
	}
	
	int i;
	
	
	
	if(rot[7]==0){
		if(rot[6]==1){
			var=1;
		}
		num=1;
		rotazione(lab, v, rot, num);
		if(lab[v[0]+rot[0]][v[1]+rot[1]]!=1&&lab[v[0]+rot[0]][v[1]+rot[1]]!=5&&lab[v[0]+rot[0]][v[1]+rot[1]]!=4){
			var++;
		}
		num=2;
		rotazione(lab, v, rot, num);
		if(lab[v[0]+rot[0]][v[1]+rot[1]]!=1&&lab[v[0]+rot[0]][v[1]+rot[1]]!=5&&lab[v[0]+rot[0]][v[1]+rot[1]]!=4){
			var++;
		}
		num=1;
		rotazione(lab, v, rot, num);
		
		if(var==3){
			num=1;
			rotazione(lab, v, rot, num);
			
			rot[6]=0;
		}
	}
	
	
	for(i=0;i<4;i++){
		if(lab[v[0]+rot[0]][v[1]+rot[1]]==9){
			wasd(v, mosse, rot);
			p_algoritmo(v, rot, lab);
			sposta_algoritmo(lab, v, rot);
			i=4;
		}
		num=1;
		rotazione(lab, v, rot, num);
	}
	
	
	if(v[4]-1==0){
		lab[v[0]-rot[0]][v[1]-rot[1]]=5;
	}
}

///Funzione start secondo algoritmo
///
///Funzione che prepara e fa partire il secondo algoritmo di risoluzione
///@see algoritmo2()
///@see coord_simple()
///@see stampa()
void algoritmo2_ext(int **lab, int n, int m){
	
	int *v;
	v=malloc(sizeof(int)*5);

	char *mosse;
	mosse=malloc(sizeof(char));
	
	int *rot;
	rot=malloc(sizeof(int)*8);
	
	int p;
	int F;
	int r;
	int c;
	int k;
	v[0]=r=1;
	v[1]=c=0;
	v[2]=p=0;
	v[3]=F=0;
	v[4]=0;
	
	int t=rot[0]=1;
	int h=rot[1]=0;
	int x=rot[2]=0;
	int y=rot[3]=1;
	int a=rot[4]=0;
	int b=rot[5]=1;
	int indietro=rot[6]=0;
	rot[7]=0;
	
	k=1;
	coord_simple(lab, rot, n, m, v);
	
	do{
		algoritmo2(lab, v, n, m, mosse, rot);
		if(v[4]>=k-1){
			k=k+10;
			mosse=realloc(mosse, k*sizeof(char));
		}

	}while(v[3]==0);
	
	
	mosse[v[4]]='\0';
	switch(v[3]){
		case 1:
			stampa(lab, n, m);
			printf("\nIl computer ha trovato il percorso con queste mosse:\n\t");
			int i;
			for(i=0;i<v[4];i++){
				switch(mosse[i]){
					case 's':
						printf("S");
					break;
					case 'w':
						printf("N");
					break;
					case 'a':
						printf("W");
					break;
					case 'd':
						printf("E");
					break;
					default:
						printf("-");
					break;
				}
			}
		break;
		default:
			printf("\n\tERROR\n");
		break;
	}
}

///Funzione di funzionamento del terzo algoritmo
///
///Questa funzione è il metodo di funzionamento dell'algoritmo ricorsivo
///@param x e @param y sono la posizione attuale
///@param point è il punteggio
///@param xpast e @param ypast sono il percorso passato dello snake
///@param dim_past è la dimensionde del vattore delle posizioni passate
///@param max_t e @param start_t sono i valori che tengono il tempo di esecuzione della funzione
///@return punteggio
int alg_ric(int **lab, int x, int y, int t, int n, int m, int point, int *xpast, int *ypast, int *dim_past, int num, int max_t, time_t start_t){
	
	time_t end_t;
	time(&end_t);
	if(end_t-start_t>max_t){
		return -100000;
	}
	
	int i;
	num++;
	point--;
	int var=0;
	for(i=0;i<num;i++){
		if(x==xpast[i]&&y==ypast[i]){
			var=1;
		}
	}
	if(lab[x][y]==9){
		xpast[num-1]=x;
		ypast[num-1]=y;
		return point;
	}else{
		if((lab[x][y]==1&&t==0)||var==1||point==0){
			return 0;
		}else{
			xpast[num-1]=x;
			ypast[num-1]=y;
			switch(lab[x][y]){
				case 3:
					point=point/2;
				break;
				case 2:
					point=point+10;
				break;
				case 7:
					t=t+3;
				break;
				case 1:
					if(t>0){
						t--;
					}
				break;
				default:
				break;
			}
			int punti=point;
			int xp[*dim_past];
			int yp[*dim_past];
			for(i=0;i<*dim_past;i++){
				xp[i]=xpast[i];
				yp[i]=ypast[i];
			}
			
			int xtmp[*dim_past][4];
			int ytmp[*dim_past][4];
			int tmp[4];
			if(x!=0){
				tmp[0]=alg_ric(lab, x-1, y, t, n, m, point, xpast, ypast, dim_past, num, max_t, start_t);
			}else{
				tmp[0]=0;
			}
			for(i=0;i<*dim_past;i++){
				xtmp[i][0]=xpast[i];
				ytmp[i][0]=ypast[i];
			}
			for(i=0;i<*dim_past;i++){
				xpast[i]=xp[i];
				ypast[i]=yp[i];
			}
			
			point=punti;
			if(y!=0){
				tmp[1]=alg_ric(lab, x, y-1, t, n, m, point, xpast, ypast, dim_past, num, max_t, start_t);
			}else{
				tmp[1]=0;
			}
			for(i=0;i<*dim_past;i++){
				xtmp[i][1]=xpast[i];
				ytmp[i][1]=ypast[i];
			}
			for(i=0;i<*dim_past;i++){
				xpast[i]=xp[i];
				ypast[i]=yp[i];
			}
			
			point=punti;
			if(x!=n-1){
				tmp[2]=alg_ric(lab, x+1, y, t, n, m, point, xpast, ypast, dim_past, num, max_t, start_t);
			}else{
				tmp[2]=0;
			}
			for(i=0;i<*dim_past;i++){
				xtmp[i][2]=xpast[i];
				ytmp[i][2]=ypast[i];
			}
			for(i=0;i<*dim_past;i++){
				xpast[i]=xp[i];
				ypast[i]=yp[i];
			}
			
			point=punti;
			if(y!=m-1){
				tmp[3]=alg_ric(lab, x, y+1, t, n, m, point, xpast, ypast, dim_past, num, max_t, start_t);
			}else{
				tmp[3]=0;
			}
			for(i=0;i<*dim_past;i++){
				xtmp[i][3]=xpast[i];
				ytmp[i][3]=ypast[i];
			}
			for(i=0;i<*dim_past;i++){
				xpast[i]=xp[i];
				ypast[i]=yp[i];
			}
			
			int posizione=0;
			for(i=0;i<4;i++){
				if(tmp[i]>tmp[posizione]){
					posizione=i;
				}
			}
			int max=tmp[posizione];
			for(i=0;i<*dim_past;i++){
				xpast[i]=xtmp[i][posizione];
				ypast[i]=ytmp[i][posizione];
			}
			return max;
		}
	}
}

///Funzione start terzo algoritmo
///
///Funzione che prepara e fa partire il terzo algoritmo di risoluzione
///@see alg_ric()
///@see stampa()
void alg_ric_ext(int **lab, int n, int m, int sx, int sy, int money, int fx, int fy, int walls, int tri){
	
	int t=0;
	int point=1000;
	
	int dim_past=n*m-walls+tri*3+3;
	int num=0;
	int *xpast;
	xpast=malloc(dim_past*sizeof(int));
	int *ypast;
	ypast=malloc(dim_past*sizeof(int));
	
	
	printf("\nQuanto tempo vuoi dare in secondi all\'algoritmo per risolvere il labirinto?\t");
	int max_t=0;
	scanf("%d", &max_t);
	printf("\n");
	time_t start_t;
	time(&start_t);
	
	int b=alg_ric(lab, sx, sy, t, n, m, point, xpast, ypast, &dim_past, num, max_t, start_t);
	int i=0;
	do{
		i++;
	}while(lab[xpast[i-1]][ypast[i-1]]!=9);
	int dimensione=i;
	int xfinali[dimensione];
	int yfinali[dimensione];
	for(i=0;i<dimensione;i++){
		xfinali[i]=xpast[i];
		yfinali[i]=ypast[i];
	}
	
	
	char mosse[dimensione];
	for(i=1;i<dimensione;i++){
		if(xpast[i]-xpast[i-1]==1){
			mosse[i-1]='S';
		}else{
			if(xpast[i]-xpast[i-1]==-1){
				mosse[i-1]='N';
			}else{
				if(ypast[i]-ypast[i-1]==1){
					mosse[i-1]='E';
				}else{
					if(ypast[i]-ypast[i-1]==-1){
						mosse[i-1]='W';
					}
				}
			}
		}
	}
	for(i=0;i<dimensione;i++){
		lab[xpast[i]][ypast[i]]=4;
		if(i==dimensione-1){
			lab[xpast[i]][ypast[i]]=8;
		}
	}
	lab[sx][sy]=5;
	
	stampa(lab, n, m);
	if(b<-1000){
		printf("\nIl computer non ha trovato un\'uscita nel tempo datogli a disposizione :C\n");
	}else{
		printf("\nIl computer ha trovato il percorso con queste mosse:\n\t");
		printf("%s\n\n", mosse);
	}
}

///Funzione di aggiunta cosa (modalità interattiva)
///
///Funzione che a seconda delle monete raccolte inserisce la coda dello snake nel labirinto
void aggiungi_coda(int **lab, int *v, int n, int m, int x, int y, char *mosse){
	int i,j;
	for(i=v[4]-1;i>v[4]-v[6];i--){
		switch(mosse[i]){
			case 'w': case 'W':
				x=x+1;
				y=y;
				if(y==v[0]&&x==v[1]){
					v[6]=v[4]-i;
					i=-4;
				}else{
					lab[y][x]=6;
				}
			break;
			case 'e': case 'E':
				x=x-1;
				y=y;
				if(y==v[0]&&x==v[1]){
					v[6]=v[4]-i;
					i=-4;
				}else{
					lab[y][x]=6;
				}
			break;
			case 'n': case 'N':
				x=x;
				y=y+1;
				if(y==v[0]&&x==v[1]){
					v[6]=v[4]-i;
					i=-4;
				}else{
					lab[y][x]=6;
				}
			break;
			case 's': case 'S':
				x=x;
				y=y-1;
				if(y==v[0]&&x==v[1]){
					v[6]=v[4]-i;
					i=-4;
				}else{
					lab[y][x]=6;
				}
			break;
			default:
				lab[y][x]=1;
			break;
		}
	}
}

///Funzione per la modalità interattiva
///
///Questa funzione gestisce l'interità della modalità interattiva
///@see coord_simple()
///@see movimento()
///@see copia_lab()
///@aggiungi_coda
///@see stampa()
void manuale(int **lab, int n, int m){
	int i;
	int *v;
	v=malloc(sizeof(int)*7);

	char *mosse;
	mosse=malloc(sizeof(char)*2);
	
	int *rot;
	rot=malloc(sizeof(int)*8);
	
	int **lab2;
	lab2=(int **)malloc(sizeof(int *)*n);
	for(i=0; i<n; i++){
    	lab2[i]=(int *)malloc(sizeof(int)*m);
	}
	
	v[0]=1;
	v[1]=0;
	v[2]=1000;
	v[3]=0;
	v[4]=0;
	v[5]=0;
	v[6]=1;
	
	int k=2;
	coord_simple(lab, rot, n, m, v);
	
	do{
		movimento(lab, v, mosse);
		printf("Per ora hai fatto %d punti\n", v[2]);
		copia_lab(lab, lab2, n, m);
		if(v[6]>0){
			int y=v[0];
			int x=v[1];
			aggiungi_coda(lab2, v, n, m, x, y, mosse);
			stampa(lab2, n, m);
		}else{
			stampa(lab, n, m);
		}
		
		
		printf("\n\n");
		if(v[4]>=k){
			k=k+10;
			mosse=realloc(mosse, k*sizeof(char));
		}
	}while((v[3]==0||v[3]==2));
	
	switch(v[3]){
		case 1:
			printf("\n\nHAI VINTO!!\n\nCon un totale di: %d$ punti!\nEd una lunghezza di: %d!!!!", v[2], v[6]);
			printf("\nCon %d mosse: ", v[4]+1);
			mosse[v[4]]='\0';
			int i;
			for(i=0;i<v[4];i++){
				switch(mosse[i]){
					case 's': case 'S':
						printf("S");
					break;
					case 'n': case 'N':
						printf("N");
					break;
					case 'e': case 'E':
						printf("E");
					break;
					case 'w': case 'W':
						printf("W");
					break;
					default:
						printf(".");
					break;
				}
			}
			printf("\n\n");
		break;
		case 3:
			printf("\n\nSi e\' presentato un errore :(");
		break;
		default:
			printf("\n\nExit\n\n");
		break;
	}
}

///Funzione Main
///
///Funzione principale che gestisce le interazioni iniziali con l'utente e collega tutti metodi di risoluzione del labirinto
///@see inserisci_labirinto()
///@see stampa()
///@see copia_lab()
///@see manuale
///@see algoritmo1_ext()
///@see algoritmo2_ext()
///@see alg_ric_ext()
int main(){
	int n, m;
	int i,j;
	scanf("%d", &m);
	scanf("%d", &n);
	
	int **lab;
	lab=(int **)malloc(sizeof(int *)*n);
    for(i=0; i<n; i++){
    	lab[i]=(int *)malloc(sizeof(int)*m);
	}
	int **lab2;
	lab2=(int **)malloc(sizeof(int *)*n);
	for(i=0; i<n; i++){
    	lab2[i]=(int *)malloc(sizeof(int)*m);
	}
	int sx, sy, fx, fy, walls=0, tri=0;
	int money=0;
	int point=1000;
	inserisci_labirinto(lab, n, m, &sx, &sy, &money, &fx, &fy, &walls, &tri);
	int out=0;
	int g;
	for(g=0;g<2;g++){
		
		printf("\nCome vuoi procedere?\n\t1.Voglio risolvere lo snake-maze manualmente\n\t2.Voglio farlo compilare dal computer\n\t3.Exit\n-");
		scanf("%d", &out);
		
		if(out==1){
			copia_lab(lab, lab2, n, m);
			manuale(lab2, n, m);
		}
		
		if(out==2){
			int metodo=0;
			int f;
			for(f=0;f<2;f++){
				printf("\nChe metodo vorresti fare usare al computer?\n\t1: segue la parete destra\n\t2: prosegue finchÃ¨ non trova una parete e poi gira a destra\n\t3: per ricorsione prova tutti i percorsi possibili\n\t4:Exit\n-");
				scanf("%d", &metodo);
				if(metodo==1){
					copia_lab(lab, lab2, n, m);
					algoritmo1_ext(lab2, n, m);
				}
				if(metodo==2){
					copia_lab(lab, lab2, n, m);
					algoritmo2_ext(lab2, n, m);
				}
				if(metodo==3){
					copia_lab(lab, lab2, n, m);
					alg_ric_ext(lab2, n, m, sx, sy, money, fx, fy, walls, tri);
				}
				if(metodo!=1&&metodo!=2&&metodo!=3&&metodo!=4){
					f=0;
					printf("\n\tERROR\n");
				}else{
					f=2;
				}
			}
		}
		
		if(out!=1&&out!=2&&out!=3){
			printf("\n\tERROR\n");
		}
		
		if((out==3)){
			g=2;
		}else{
			g=0;
		}
	}
	
	
	return 0;
}
