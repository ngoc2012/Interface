// Function printf
#include <stdio.h>
// Declare NULL
#include <stddef.h>
// Function write, sleep
#include <unistd.h>
// Function malloc, atoi
#include <stdlib.h>

#include "core.h"

void dessiner_cadres(int n, int n2, int n3, int nl){

	printf("dessiner_cadres avec n=%d\tn2=%d\tn3=%d\tnl=%d\n", n, n2, n3, nl);

	// OUVERTURE
	// Dessiner cadre externe
	write(1,"| o ",4);
	int i;
	for (i=0; i<nl; i++){
		write(1,"o ",2);
	}

	for (i=0; i<n2/2; i++){
		write(1,"| o o | o ",10);
	}

	for (i=0; i<n3/2; i++){
		// if n and n3 are even, at the last n3
		if (n3*0.5 == ((int) n3/2) & n*0.5 == ((int) n/2) & i == (n3/2-1)) {
			write(1,"| o o o |",9);
		} else {
			write(1,"| o o o | o ",12);
		}
		
	}

	// if n2 is odd
	if (n2*0.5 != ((int) n2/2)) {
		write(1,"| o o |",7);
	}
	
	// if n3 is odd
	if (n3*0.5 != ((int) n3/2)) {
		write(1,"| o o o |",9);
	}

	// FERMETURE
	for (i=0; i<n3/2; i++){
		// if n and n3 are even, at the last n3
		if (i==0 & n3*0.5 == ((int) n3/2) & n*0.5 == ((int) n/2)) {
			write(1,"| o o o |",9);
		}else{
			write(1," o | o o o |",12);
		}
		
	}

	for (i=0; i<n2/2; i++){
		if (i==0 & n2*0.5 == ((int) n2/2) & n3==0) {
			write(1,"| o o |",7);
		}else{
			write(1," o | o o |",10);
		}
	}
	
	for (i=0; i<nl; i++){
		write(1," o",2);
	}
	write(1," o |\n",5);
}

void display_cadre(int n, int n2, int n3, int nl, int *n2_min, int *n3_min, int *nl_min){
	dessiner_cadres(n, n2, n3, nl);
	if (((n2+n3) < (*n2_min+*n3_min)) | (((n2+n3) == (*n2_min+*n3_min)) & n3>*n3_min))
	{
		*n2_min = n2;
		*n3_min = n3;
		*nl_min = nl;
	}
}

int dispo_cadres(int n, int *n2_min, int *n3_min, int n2_ini, int n3_ini, int *nl_min, int nr){
	
	printf("n=%d\tn2_min=%d\tn3_min=%d\tn2_ini=%d\tn3_ini=%d\tnl_min=%d\tnr=%d\n", n, *n2_min, *n3_min, n2_ini, n3_ini, *nl_min, nr);

	if (nr<-1) {
		return 0;
	}
	// n2, n3 Nombre de petits et grands cadres sur la moitié du cage
	int n2 = 0, n3 = 0, nl = 1;

	switch (nr) {
		case -1:
			nl=0;
			n2=0; n3=0;
			display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			break;
		case 0:
			nl=0;
			n2=0; n3=0;
			display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			break;
		case 1:
			nl=1;
			n2=0; n3=0;
			display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			break;
		case 2:
			nl=0;
			n2=1; n3=0;
			display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			write_str("(Nota: Bar entre n2 et n3 à supprimer)");
			break;
		case 3:
			nl=0;
			n2=1; n3=0;
			display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			break;
		case 4:
			nl=1;
			n2=1; n3=0;
			display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			break;
		case 5:
			nl=1;
			n2=0; n3=1;
			display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			break;
		case 6:
			nl=0;
			n2=2; n3=0;
			display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			break;
		case 7:
			nl=1;
			n2=2; n3=0;
			display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			break;
		case 8:
			nl=0;
			n2=0; n3=2;
			display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			break;
		case 9:
			nl=1;
			n2=0; n3=2;
			display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			break;
	}
	
	if (nr>9) {
		// Equation : nr = n2*x + n3*y
		// Equation : nr-1 = n2*x + n3*y
		while (n3 <= nr/4){

			printf("n=%d\tn2=%d\tn3=%d\n", (nr-2), n2, n3);

			nl=0;
			if ((nr-nl)/n2*1.0 == ((int) (nr-nl)/n2)){
				display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			}

			nl=1;
			if ((nr-nl)/n2*1.0 == ((int) (nr-nl)/n2)){
				display_cadre(n, n2*2+n2_ini, n3*2+n3_ini, nl, n2_min, n3_min, nl_min);
			}

			n3++;
		}
	}

	return 1;
}

int main(int argc, char *argv[])
{	
	/*
	Contrainte:
	- Les cadres doivent être symétriques,
	- Au moins un grand cadre pour bétonnage,

	Optimistion:
	- Nombre de brins minimal.

					_n3_ini_ (ou n2_ini)
		____nr_____	
	
	    nl
		^				| <- axe
		|				|
	-----------------------------------
	| o o | o o | o | o o o |
	|     |     |   |       |
	|     |     |   |       |
	|     |     |   |       |
	| o o | o o | o | o o o |
	-----------------------------------
						|
						|
	*/
	/*
	dessiner_cadres(1, 2, 3, 1);
	dessiner_cadres(1, 3, 2, 1);
	dessiner_cadres(17, 2, 2, 1);
	dessiner_cadres(16, 2, 2, 1);
	dessiner_cadres(1, 2, 1, 0);
	dessiner_cadres(1, 1, 0, 0);
	dessiner_cadres(1, 0, 1, 0);
	dessiner_cadres(1, 2, 0, 0);
	dessiner_cadres(1, 0, 2, 0);
	print_int(12345678);
	write(1,"\n",2);
	*/
	
	//printf("%d\n", argc);
	// Neu user khong nhap n thi out khoi chuong trinh luon
	if (argc<2) 
		return 0;

	int n = atoi(argv[1]);
	int nr;	// Nomnre de bars résidu chaque côté
	int n2_min=10000, n3_min=10000; // Nombre de petits et grands cadres minimal de tous les cas
	int n2_ini, n3_ini; // Nombre de petits et grands cadres initial (minimal requis pour ce cas)
	int i;
	int nl_min=0; // Nombre de bars à l'extremite minimal

	if (n*0.5 == ((int) n/2)) {

		printf("Nombre bars pair : %d\n",n);
		write_str("Cas 1.1: nombre de cadres impair");
		write_str("Un cadre n2 au milieu et un cadre n3 minimal chaque côté");
		nr = (n/2-1)-2-(3+1);
		n2_ini = 1; n3_ini = 2; nl_min = 1;
		i = dispo_cadres(n, &n2_min, &n3_min, n2_ini, n3_ini, &nl_min, nr);

		write_str("Cas 1.2: nombre de cadres pair");
		write_str("Un cadre n3 minimal chaque côté");
		nr = (n/2-1)-3;
		n2_ini = 0; n3_ini = 2; nl_min = 1;
		i = dispo_cadres(n, &n2_min, &n3_min, n2_ini, n3_ini, &nl_min, nr);

	} else {

		printf("Nombre bars impair : %d\n",n);
		write_str("Cas 2.1: nombre de cadres impair");
		write_str("Un cadre n3 au milieu");
		nr = ((n+1)/2+1-1)-3;
		n2_ini = 0; n3_ini = 1; nl_min = 1;
		i = dispo_cadres(n, &n2_min, &n3_min, n2_ini, n3_ini, &nl_min, nr);

		write_str("Cas 2.2: nombre de cadres pair");
		write_str("Un cadre n3 minimal chaque côté");
		nr = (n-1)/2-1-3;
		n2_ini = 0; n3_ini = 2; nl_min = 1;
		i = dispo_cadres(n, &n2_min, &n3_min, n2_ini, n3_ini, &nl_min, nr);
	}

	write_str("=================================================================");
	write_str("CONCLUSION");
	if (n2_min<10000 & n3_min<10000){
		write_str("Cadres optimisé : ");
		dessiner_cadres(n, n2_min, n3_min, nl_min);
	} else {
		write_str("Pas de disposition possible");
	}
	return (0) ;
}