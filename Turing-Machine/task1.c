#include<stdio.h>
#include<string.h>

//pentru organizare, am creat o structura "Tranzitie" avand campurile necesare rularii masinii turing:

typedef struct {

	char stare_curenta[10];
	char charb1;
	char charb2;
	char stare_urm[10];
	char charb1urm;
	char deplasareb1;
	char charb2urm;
	char deplasareb2;

} Tranzitie;



int main(void)
{
	
	int i, nrstari, nrstarifinale, nrtranzitii, cap1, cap2, terminat = 0, tranzitie = 0;
	char starecurenta[10], banda1[10000], banda2[10000];
	FILE *f = fopen("task1.in", "r");


	if (f == NULL) printf("failed to open task1.in\n");
	
//se citeste numarul de stari din fisier:
	fscanf(f, "%d", &nrstari);

	char stari[nrstari][10];

//se citesc starile:
	for (i = 0 ; i < nrstari ; i++)
		fscanf(f, "%s", stari[i]);	

//se citeste nr de stari finale:
	fscanf(f, "%d", &nrstarifinale);

	char starifinale[nrstarifinale][10];

//se citesc starile finale:
	for (i = 0 ; i < nrstarifinale ; i++)
		fscanf(f, "%s", starifinale[i]);

//se citeste starea initiala, care este si starea curenta in prima tranzitie a masinii:
	fscanf(f, "%s", starecurenta);

//se citeste nr de tranzitii:
	fscanf(f, "%d", &nrtranzitii);

	Tranzitie vec_tranzitii[nrtranzitii];
	
//se citesc tranzitiile:
	for (i = 0 ; i < nrtranzitii ; i++) {
		fscanf(f, "%s", vec_tranzitii[i].stare_curenta);
		fscanf(f, " %c ", &(vec_tranzitii[i].charb1));
		fscanf(f, " %c ", &(vec_tranzitii[i].charb2));
		fscanf(f, "%s", vec_tranzitii[i].stare_urm);
		fscanf(f, " %c ", &(vec_tranzitii[i].charb1urm));
		fscanf(f, " %c ", &(vec_tranzitii[i].deplasareb1));
		fscanf(f, " %c ", &(vec_tranzitii[i].charb2urm));
		fscanf(f, " %c ", &(vec_tranzitii[i].deplasareb2));
	}


//se citesc cele 2 benzi din fisier:
	fscanf(f, "%s", banda1);
	fscanf(f, "%s", banda2);

//initializam capetele de citire ale benzilor:
	cap1 = 1;
	cap2 = 1;

	FILE *g = fopen("task1.out", "w+");

	if (g == NULL) printf("failed to open task1.out\n");


//avem variabilele tranzitie si terminat care retin daca 
//masina a efectuat o tranzitie sau daca s-a terminat


	do {

		tranzitie = 0;

//intai se cauta in vectorul de tranzitii (vec_tranzitii) starea 
//curenta si cele 2 caractere de pe cele 2 benzi

		for (i = 0 ; i < nrtranzitii ; i++) {

			if (	(strcmp(vec_tranzitii[i].stare_curenta, starecurenta) == 0) &&
				(vec_tranzitii[i].charb1 == banda1[cap1]) &&
				(vec_tranzitii[i].charb2 == banda2[cap2]) ) {

	//cand s-a gasit o potrivire, actualizam starea curenta si suprascriem
	//caracterele de pe banda:

				strcpy(starecurenta, vec_tranzitii[i].stare_urm);
				banda1[cap1] = vec_tranzitii[i].charb1urm;
				banda2[cap2] = vec_tranzitii[i].charb2urm;

	//deplasam capetele dupa cum e necesar:

				if (vec_tranzitii[i].deplasareb1 == 'R')
					cap1++;

				if (vec_tranzitii[i].deplasareb1 == 'L')
					cap1--;

				if (vec_tranzitii[i].deplasareb2 == 'R')
					cap2++;

				if (vec_tranzitii[i].deplasareb2 == 'L')
					cap2--;

	//cand se ajunge in capatul unei benzi, se initializeaza urmatorul
	//caracter cu '#'pentru a putea fi suprascris daca este cazul

				if (strlen(banda1) == cap1)
					banda1[cap1] = '#';

				if (strlen(banda2) == cap2)
					banda2[cap2] = '#';

		//s-a realizat o tranzitie

				tranzitie = 1;

				break;

			}

		}

//se verifica daca starea curenta este o stare finala:
		
		for (i = 0 ; i < nrstarifinale ; i++) {
			if(strcmp(starecurenta, starifinale[i])==0)
				terminat = 1;
		}

//daca masina nici nu a ajuns in stare finala si nici nu a realizat o tranzitie, inseamna ca s-a blocat:

		if ((tranzitie == 0) && (terminat == 0)) {
			fprintf(g, "The machine has blocked!\n");
			fclose(f);
			fclose(g);
			return 0;
		}

//se repeta procedeul pana cand masina ajunge in stare finala

	} while (terminat == 0);


//dupa executia masinii, printam cele 2 benzi:

	for (i = 0 ; i < strlen(banda1) ; i++)
		if (banda1[i] != '#')
			fprintf(g, "%c", banda1[i]);

	fprintf(g, "\n");


	for (i = 0 ; i < strlen(banda2) ; i++)
		if (banda2[i] != '#')
			fprintf(g, "%c", banda2[i]);

	fprintf(g, "\n");


	fclose(f);
	fclose(g);

	return 0;

}
