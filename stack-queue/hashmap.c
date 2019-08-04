#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//structura unei liste generice:
typedef struct cel
{
	struct cel *urm;
	void *info;
} TCelula, *TLista;


//structura stivei:
typedef struct stiva
{
	TLista varf;
} TStiva;

//structura cozii:
typedef struct coada
{
	TLista ic, sc;
} TCoada;

//structura informatiei celulei:
typedef struct info
{
	char tip[2];
	int id;
} TInfo;

//functie de alocare spatiu pentru celula:
TLista AlocCelula(void *ae)
{
	TLista aux = (TLista)malloc(sizeof(TCelula));

	if (!aux) {
		printf("Alloc failed!\n");
		return NULL;
	}

	aux->urm = NULL;
	aux->info = ae;

	return aux;
}

int Push(TStiva *s, void *ae)
{
	TLista aux = AlocCelula(ae); //aloc celula cu verificare
	if(!aux) {
		printf("alocare esuata\n");
		return 0;
	}

	if (s) { //inserez la inceputul listei
		aux->urm = s->varf;
		s->varf = aux;
	} else {
		printf("stiva neinitializata\n");
		return 0;
	}

	return 1;
}

int IntrQ(TCoada *c, void *ae)
{
	TLista aux = AlocCelula(ae); //aloc celula cu verificare

	if(!aux) {
		printf("alocare esuata\n");
		return 0;
	}

	if (c) {
		/* daca sfarsitul cozii e null, inseamna ca adaugam
		 * primul element care va fi si inceputul si sfarsitul cozii:
		 */
		if (!c->sc) {
			c->sc = aux;
			c->ic = aux;
		} else { //altfel, adaug elementul la sfarsit:
			c->sc->urm = aux;
			c->sc = aux; //reactualizez sfarsitul cozii
		}
	} else {
		printf("coada neinitializata\n");
		return 0;
	}

	return 1;
}

void *Pop(TStiva *s)
{
	void *aux;
	TLista p; //pointer auxiliar pentru eliberare de spatiu

	if (!s)
		return NULL;

	if (!s->varf)
		return NULL;
	else {
		aux = s->varf->info; //salvez informatia
		s->varf->info = NULL; //rup legatura
		p = s->varf;
		s->varf = s->varf->urm; //actualizez varful stivei
		p->urm = NULL;
		free(p); //eliberez celula din care am extras info
	}

	return aux; //returnez informatia
}

void *ExtrQ(TCoada *c)
{
	void *aux;
	TLista p; //pointer auxiliar pentru eliberare de spatiu

	if (!c)
		return NULL;

	if (!c->ic)
		return NULL;

	aux = c->ic->info; //salvez informatia
	c->ic->info = NULL; //rup legatura
	p = c->ic;
	if (c->ic == c->sc) { //daca este un singur element,
		c->ic = NULL; //inceputul si sfarsitul
		c->sc = NULL; //pointeaza catre null
	} else 
		c->ic = c->ic->urm; //altfel, actualizez inceputul cozii
	p->urm = NULL;
	free(p); //eliberez celula din care am extras info

	return aux; //returnez informatia
}

//functie eliberare stiva:
void DistrS(TStiva *s)
{
	while (s->varf)
		free(Pop(s));

}

//functie eliberare coada:
void DistrQ(TCoada *c)
{
	while (c->ic)
		free(ExtrQ(c));

}


//functie care "rastoarna" o stiva:
void Rastoarna(TStiva *s, TStiva *saux)
{
	void *aux;

	if (!s)
		return;

	while(s->varf) {
		aux = Pop(s); //extrag elementul
		Push(saux, aux); //il adaug intr-o stiva auxiliara
	}

}

void PrintS(TStiva *s, FILE *g)
{

	if (s) {if (!s->varf) return;}
	else return;

	TStiva *saux = calloc(sizeof(TStiva),1);
	void *aux;

	if(!saux) {
		printf("alocare esuata\n");
		return;
	}

	//"rastorn" stiva pentru afisarea elementelor de la baza acesteia
	Rastoarna(s, saux);

	while (saux->varf) {
		aux = Pop(saux); //extrag elementul
		fprintf(g, "%s", ((TInfo*)aux)->tip); //il afisez
		Push(s, aux); //il pun la loc in stiva
	}

	free(saux); //eliberez stiva auxiliara

}

void SortS(TStiva *s)
{
	TStiva *saux = calloc(sizeof(TStiva),1);
	void *aux;

//verificare alocari:
	if (!saux) {printf("alocare esuata"); return;}

	if ((!s)||(!s->varf)) {
		free(saux);
		return;
	}

	while(s->varf) {
		aux = Pop(s); //extrag un element din stiva
		while (saux->varf && ((TInfo*)(saux->varf->info))->id<((TInfo*)aux)->id) {
		//daca exista unul mai mic in stiva auxiliara il introduc in stiva originala
			Push(s, Pop(saux));
		}
		Push(saux, aux); //daca nu, se introduce elementul in stiva auxiliara si se continua cautarea
	}
 
//se muta elementele ramase din stiva auxiliara in stiva originala

	Rastoarna(saux,s);

	free(saux); //eliberez stiva auxiliara

}

void PrintQ(TCoada *c, FILE *g)
{
	TCoada *caux = calloc(sizeof(TCoada),1);
	void *aux;

//verificare alocari:
	if (!caux) {
		printf("alocare esuata\n");
		return;
	}

	if ((!c)||(!c->ic)) {
		free(caux);
		return;
	}

	while(c->ic) {
		aux = ExtrQ(c); //extrag element din coada
		fprintf(g, "%s", ((TInfo*)aux)->tip); //il afisez
		IntrQ(caux, aux); //il introduc in coada auxiliara
	}

	//reactualizez pointerii astfel incat coada originala sa fie cea auxiliara:
	c->ic = caux->ic;
	c->sc = caux->sc;

	//eliberez coada auxiliara:
	caux->ic = NULL;
	caux->sc = NULL;

	free(caux);
}

//sortez coada cu ajutorul functiei de sortare a stivei:
void SortQ(TCoada *c)
{
	TStiva *saux = calloc(sizeof(TStiva),1);
	TStiva *saux2 = calloc(sizeof(TStiva),1);
	void *aux;

//verificare alocari:
	if (!saux)
		return;

	if (!saux2) {
		free(saux);
		return;
	}

	if ((!c)||(!c->ic)) {
		free(saux);
		free(saux2);
		return;
	}

	//extrag elementele din coada si le pun intr-o stiva:
	while (c->ic) {
		aux = ExtrQ(c);
		Push(saux, aux);
	}

	//sortez stiva:
	SortS(saux);

	//rastorn stiva, pentru a le introduce in ordinea corecta cand le extrag rand pe rand
	Rastoarna(saux, saux2);

	//introduc elementele inapoi in coada:
	while(saux2->varf) {
		aux = Pop(saux2);
		IntrQ(c, aux);
	}

	free(saux);
	free(saux2);

}

int Match(void* x, void* y)
{
	if  ( 	(strcmp(((TInfo*)x)->tip,"(")==0) &&
		(strcmp(((TInfo*)y)->tip,")")==0) )
		return 1;

	if  ( 	(strcmp(((TInfo*)x)->tip,"[")==0) &&
		(strcmp(((TInfo*)y)->tip,"]")==0) )
		return 1;

	if  ( 	(strcmp(((TInfo*)x)->tip,"{")==0) &&
		(strcmp(((TInfo*)y)->tip,"}")==0) )
		return 1;

	return 0;
}

int CorrectS(TStiva *s)
{
//verifica daca e stiva vida:
	if((!s) || (!s->varf))
		return 0;

	int max = 0, ind2=-1;
	/* folosesc o stiva de indici, una auxiliara in care verific daca
	 * parantezele fac match, si una auxiliara in care salvez copii
	 * dupa elemente ca sa nu se piarda stiva originala:
	 */	
	TStiva *sindici = calloc(sizeof(TStiva),1), *saux = calloc(sizeof(TStiva),1);
	TStiva *saux2 = calloc(sizeof(TStiva),1);
	void *aux, *ind, *aux2;

	//verificare alocari:
	if (!sindici) {
		printf("alocare esuata\n");
		return -1;
	}

	if (!saux) {
		free(sindici);
		printf("alocare esuata\n");
		return -1;
	}

	if (!saux2) {
		free(sindici);
		free(saux);
		printf("alocare esuata\n");
		return -1;
	}

	ind = malloc(sizeof(int));
	memcpy(ind, (void*)(&ind2), sizeof(int));

	if (!Push(sindici, ind)) printf("fail push\n");

	while (s->varf) {
		ind2++;
		aux = Pop(s);
		aux2 = malloc(sizeof(TInfo));
		memcpy(aux2, aux, sizeof(TInfo));
		Push(saux2, aux2);
		if ((saux->varf) && (Match(aux, saux->varf->info))) {
			free(Pop(saux));
			free(Pop(sindici));
			free(aux);
		}
		else {
			ind = malloc(sizeof(int));
			memcpy(ind, (void*)(&ind2), sizeof(int));
			Push(sindici, (void*)ind);
			Push(saux, aux);
		}
	}

	ind2++;
	ind = malloc(sizeof(int));
	memcpy(ind, (void*)(&ind2), sizeof(int));
	Push(sindici, (void*)ind);

	while (sindici->varf) {
		aux = Pop(sindici);
		if ((sindici->varf)&&(*(int*)aux - *((int*)(sindici->varf->info))-1> max))
			max = *(int*)aux - *((int*)(sindici->varf->info))-1;
		free(aux);
	}

	Rastoarna(saux2,s);

	free(saux2);

	DistrS(sindici);
	free(sindici);
	DistrS(saux);
	free(saux);

	return max;
}

int CorrectQ(TCoada *c)
{
	int max;

	TStiva *saux = calloc(sizeof(TStiva),1);
	TStiva *saux2 = calloc(sizeof(TStiva),1);
	void *aux;

	if (!saux) {
		printf("alocare esuata\n");
		return -1;
	}

	if (!saux2) {
		free(saux);
		printf("alocare esuata\n");
		return -1;
	}

	if ((!c) || (!c->sc)) {
		free(saux);
		free(saux2);
		return -1;
	}

	//extrag elementele din coada si le pun intr-o stiva:
	while (c->sc) {
		aux = ExtrQ(c);
		Push(saux, aux);
	}

	//fac verificarea:
	max = CorrectS(saux);

	//rastorn stiva, pentru a le introduce in ordinea corecta cand le extrag rand pe rand
	Rastoarna(saux, saux2);

	//introduc elementele inapoi in coada:
	while(saux2->varf) {
		aux = Pop(saux2);
		IntrQ(c, aux);
	}

	free(saux);
	free(saux2);

	return max;
}

int main(int argc, char *argv[])
{

	FILE *f = fopen(argv[1], "r");
	FILE *g = fopen(argv[2], "w+");

	int nrinstr, nrstive, nrcozi, i, j, n, id, m;
	char instructiune[20], tip[2];

	fscanf(f, "%d %d %d", &nrinstr, &nrstive, &nrcozi);

	TStiva s[nrstive];
	TCoada c[nrcozi];
	TInfo *info;

	//initializez stivele si cozile:
	for (i = 0; i < nrstive; i++)
		s[i].varf = NULL;

	for (i = 0; i < nrcozi; i++) {
		c[i].ic = NULL;
		c[i].sc = NULL;
	}

	//se executa toate instructiunile din fisier cu ajutorul functiilor:

	for (i = 0; i<nrinstr; i++) {
		fscanf(f, "%s", instructiune);

		if (strcmp(instructiune, "push")==0) {
			fscanf(f, "%d %d %s", &n, &id, tip);

			info = (TInfo*)malloc(sizeof(TInfo));
			if (!info) {
				printf("alocare esuata\n");
				continue;
			}
			info->id = id;
			strcpy(info->tip,tip);

			if (!Push(s+n, (void*)info))
				printf("fail push\n");
		}	

		if (strcmp(instructiune, "prints")==0) {
			for (j = 0; j < nrstive; j++) {
				fprintf(g, "\"");
				PrintS(s+j, g);
				fprintf(g, "\"\n");
			}
		}

		if (strcmp(instructiune, "corrects")==0) {
			fscanf(f, "%d", &n);
			m = CorrectS(s+n);
			fprintf(g, "%d\n", m);
		}

		if (strcmp(instructiune, "intrq")==0) {
			fscanf(f, "%d %d %s", &n, &id, tip);
			info = (TInfo*)malloc(sizeof(TInfo));
			info->id = id;
			strcpy(info->tip,tip);
			if (!IntrQ(c+n, (void*)info))
				printf("fail intrq\n");
		}

		if (strcmp(instructiune, "printq")==0) {
			for (j = 0; j < nrcozi; j++) {
				fprintf(g, "\"");
				PrintQ(c+j, g);
				fprintf(g, "\"\n");
			}
		}

		if (strcmp(instructiune, "correctq")==0) {
			fscanf(f, "%d", &n);
			m = CorrectQ(c+n);
			fprintf(g, "%d\n", m);
		}

		if (strcmp(instructiune, "extrq")==0) {
			fscanf(f, "%d", &n);
			free(ExtrQ(c+n));
		}

		if (strcmp(instructiune, "pop")==0) {
			fscanf(f, "%d", &n);
			free(Pop(s+n));
		}

		if (strcmp(instructiune, "sorts")==0) {
			fscanf(f, "%d", &n);
			SortS(s+n);
		}

		if (strcmp(instructiune, "sortq")==0) {
			fscanf(f, "%d", &n);
			SortQ(c+n);
		}

	}	

	//se elibereaza tot:
	for (i = 0; i < nrstive; i++)
		DistrS(s+i);

	for (i = 0; i < nrcozi; i++)
		DistrQ(c+i);

	fclose(f);
	fclose(g);

	return 0;

}
