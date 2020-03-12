#include "functions.h"
#include "serial.h"
#include "vt100.h"
#define NB_MONSTRES (7)
#define COOR_LIFE (3)
#define BLOCS (6)


typedef struct
{
	uint8_t ligne1[NB_MONSTRES][COOR_LIFE];
	uint8_t ligne2[NB_MONSTRES][COOR_LIFE];
	uint8_t ligne3[NB_MONSTRES][COOR_LIFE];
	uint8_t bloc1[BLOCS][COOR_LIFE];
	uint8_t bloc2[BLOCS][COOR_LIFE];
	uint8_t bloc3[BLOCS][COOR_LIFE];
	uint8_t bloc4[BLOCS][COOR_LIFE];
	uint8_t barre[2];
	uint8_t tir[COOR_LIFE];
	uint8_t tir_monstre[COOR_LIFE];
} monstres_t;

monstres_t monstre;
monstres_t *pointeur = &monstre;
static uint8_t t = 38;
static uint8_t val_sens = 0;
static uint8_t tab_tir[2];
uint8_t val_touch=0;
uint8_t boucle=0;

void fenetre(uint8_t character_h, uint8_t character_v)
{
	vt100_clear_screen();
	unsigned char index;
	for (index = 1; index < 80; index++)
	{
		vt100_move(index, 1);
		serial_putchar(character_h);
		vt100_move(index, 24);
		serial_putchar(character_h);
		vt100_move(1, index);
		serial_putchar(character_v);
		vt100_move(80, index);
		serial_putchar(character_v);
	}
}

void menu(void)
{
	/* Encadrage */
	unsigned char index;
	for (index = 33; index < 49; index++)
	{
		vt100_move(index, 5);
		serial_putchar(42);
		vt100_move(index, 7);
		serial_putchar(42);
	}
	vt100_move(33, 6);
	serial_putchar(124);
	vt100_move(48, 6);
	serial_putchar(124);

	/* Ecriture du menu */
	vt100_move(34, 6);
	serial_puts("SPACE INVADERS");
	vt100_move(34, 14);
	serial_puts("Press any touch");
	vt100_move(39, 12);
	serial_puts("PLAY");
	vt100_move(36, 20);
	serial_puts("By Thomas");
}

void sleep(unsigned long n) {
        /* boucle vide parcourue (n * 100000) fois*/
        int i = 0;
        unsigned long int max = n * 100000;
        do {
                /* Faire qqch de stupide qui prend du temps */
                i++;
        }
        while(i <= max);
}

void ini_barre(void)
{
	pointeur->barre[0] = 38;
	pointeur->barre[1] = 22;
	vt100_move(pointeur->barre[0], pointeur->barre[1]);
	serial_puts("/-|-\\");
}

void ini_tir_monstre(void)
{
	pointeur->tir_monstre[0] = pointeur->ligne3[4][0];
	pointeur->tir_monstre[1] = pointeur->ligne3[4][1];
	pointeur->tir_monstre[2] = 0;
}

void ini_tir(void)
{
	/* Initialisation des coordonnées du tir et de l'état du tir */
	pointeur->tir[0] = pointeur->barre[0];
	pointeur->tir[1] = 21;
	pointeur->tir[2] = 0;
}

void barre(void)
{
	uint8_t touche = serial_get_last_char();
	vt100_move(t, 22);
	serial_puts("/-|-\\");
	/* Bordure de l'écran droit */
	if (t < 75)
	{
		/* Déplacement du vaisseau vers la droite */
		if (touche == 'd')
		{
			vt100_move(t, 22);
			serial_puts("     ");
			t += 1;
			vt100_move(t, 22);
			serial_puts("/-|-\\");
		}
	}
	/* Bordure de l'écran gauche */
	if (t > 2)
	{
		/* Déplacement du vaisseau vers la gauche */
		if (touche == 'q')
		{
			vt100_move(t, 22);
			serial_puts("     ");
			t -= 1;
			vt100_move(t, 22);
			serial_puts("/-|-\\");
		}
	}
	/* On appel la fonction tir si on appui sur la touche m ou si un tir est entrain d'être effectué */
	if (touche == 'm' || pointeur->tir[2] == 1)
	{
		tir();
	}
}

void tir_monstre()
{
	if (pointeur->tir_monstre[2] == 0)
	{
		pointeur->tir_monstre[0] = pointeur->ligne2[4][0];
		pointeur->tir_monstre[1] = pointeur->ligne2[4][1];
	}
	if (pointeur->tir_monstre[1] < 22)
	{
		pointeur->tir_monstre[2] = 1;
		vt100_move(pointeur->tir_monstre[0]+2,pointeur->tir_monstre[1]);
		serial_puts(" ");
		pointeur->tir_monstre[1] += 1;
		vt100_move(pointeur->tir_monstre[0]+2,pointeur->tir_monstre[1]);
		serial_puts("î");
		sleep(1);
	}
	else if (pointeur->tir_monstre[1] == 22)
	{
		pointeur->tir_monstre[2] = 0;
		vt100_move(pointeur->tir_monstre[0]+2,pointeur->tir_monstre[1]);
		serial_puts(" ");
	}
}

void tir(void)
{
	/* On récupére l'abscisse de la barre jusqu'à qu'un ennemi soit touché ou que le tir ait atteind ca limite */
	if (pointeur->tir[2] == 0)
	{
		pointeur->tir[0] = t;
	}
	/* Si le tir n'a pas atteind la bordure et si il a rien touché on le décale de 1 */
	if (pointeur->tir[1] > 2 && val_touch == 0)
	{
		pointeur->tir[2] = 1;
		vt100_move(pointeur->tir[0]+2,pointeur->tir[1]);
		serial_puts(" ");
		pointeur->tir[1] -= 1;
		vt100_move(pointeur->tir[0]+2,pointeur->tir[1]);
		serial_puts("î");
		tab_tir[0] = pointeur->tir[0]+2;
		tab_tir[1] = pointeur->tir[1];
		hit_box();
	}
	/* Vérification si le tir a touché la bordure, si oui on remet l'ordonnée par défaut 21, l'abscisse redevient celle de la barre puis on déclare que le tir est fini  */
	else if (pointeur->tir[1] == 2)
	{
		pointeur->tir[2] = 0;
		vt100_move(pointeur->tir[0]+2,pointeur->tir[1]);
		serial_puts(" ");
		pointeur->tir[1] = 21;
	}
	/* Vérification si le tir a touché un ennemie, si oui on remet par défaut la var val_touch, on remet l'ordonnée par défaut 21 puis on déclare que le tir est fini */
	if (val_touch == 1){
		val_touch = 0;
		pointeur->tir[2] = 0;
		pointeur->tir[1] = 21;
	}
}


void hit_box(void)
{
	/* Vérification si l'un des monstres à les mêmes coordonnées que le tir, si oui on l'efface, on l'indique "mort" et on déclare le tir sur "touché" */
	for (uint8_t i = 0; i < 7; i++)
	{
		if ((pointeur->ligne1[i][0] == tab_tir[0] || pointeur->ligne1[i][0]+1 == tab_tir[0] || pointeur->ligne1[i][0]+2 == tab_tir[0] || pointeur->ligne1[i][0]+3 == tab_tir[0] || pointeur->ligne1[i][0]+4 == tab_tir[0]) && (pointeur->ligne1[i][1] == tab_tir[1]) && (pointeur->ligne1[i][2] == 1))
		{
			vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
			serial_puts("     ");
			pointeur->ligne1[i][2] = 0;
			val_touch = 1;
		}
		else if ((pointeur->ligne2[i][0] == tab_tir[0] || pointeur->ligne2[i][0]+1 == tab_tir[0] || pointeur->ligne2[i][0]+2 == tab_tir[0] || pointeur->ligne2[i][0]+3 == tab_tir[0] || pointeur->ligne2[i][0]+4 == tab_tir[0]) && (pointeur->ligne2[i][1] == tab_tir[1]) && (pointeur->ligne2[i][2] == 1))
		{
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("     ");
			pointeur->ligne2[i][2] = 0;
			val_touch = 1;
		}
		else if ((pointeur->ligne3[i][0] == tab_tir[0] || pointeur->ligne3[i][0]+1 == tab_tir[0] || pointeur->ligne3[i][0]+2 == tab_tir[0] || pointeur->ligne3[i][0]+3 == tab_tir[0] || pointeur->ligne3[i][0]+4 == tab_tir[0]) && (pointeur->ligne3[i][1] == tab_tir[1]) && (pointeur->ligne3[i][2] == 1))
		{
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("     ");
			pointeur->ligne3[i][2] = 0;
			val_touch = 1;
		}
	}
	/* Vérification si l'un des blocs à les mêmes coordonnées que le tir, si oui on l'efface, on l'indique "mort" et on déclare le tir sur "touché" */
	for (uint8_t i = 0; i < 6; i++)
	{
		if (pointeur->bloc1[i][0] == tab_tir[0] && pointeur->bloc1[i][1] == tab_tir[1] && pointeur->bloc1[i][2] == 1)
		{
			vt100_move(pointeur->bloc1[i][0], pointeur->bloc1[i][1]);
			serial_puts(" ");
			pointeur->bloc1[i][2] = 0;
			val_touch = 1;
		}
		else if (pointeur->bloc2[i][0] == tab_tir[0] && pointeur->bloc2[i][1] == tab_tir[1] && pointeur->bloc2[i][2] == 1)
		{
			vt100_move(pointeur->bloc2[i][0], pointeur->bloc2[i][1]);
			serial_puts(" ");
			pointeur->bloc2[i][2] = 0;
			val_touch = 1;
		}
		else if (pointeur->bloc3[i][0] == tab_tir[0] && pointeur->bloc3[i][1] == tab_tir[1] && pointeur->bloc3[i][2] == 1)
		{
			vt100_move(pointeur->bloc3[i][0], pointeur->bloc3[i][1]);
			serial_puts(" ");
			pointeur->bloc3[i][2] = 0;
			val_touch = 1;
		}
		else if (pointeur->bloc4[i][0] == tab_tir[0] && pointeur->bloc4[i][1] == tab_tir[1] && pointeur->bloc4[i][2] == 1)
		{
			vt100_move(pointeur->bloc4[i][0], pointeur->bloc4[i][1]);
			serial_puts(" ");
			pointeur->bloc4[i][2] = 0;
			val_touch = 1;
		}
	}
}
/* Fonction de déplacement vers la droite d'un monstre */
static void one_monster_right(uint8_t num_monstre,uint8_t num_lignes)
{
	uint8_t *tab;
	if (num_lignes == 1){
		tab = pointeur->ligne1[num_monstre];
	}
	else if (num_lignes == 2){
		tab = pointeur->ligne2[num_monstre];
	}
	else if (num_lignes == 3){
		tab = pointeur->ligne3[num_monstre];
	}
	if (tab[2] == 1)
	{
		vt100_move(tab[0],
				tab[1]);
		serial_puts("     ");
 		tab[0] += 1;
		vt100_move(tab[0],
				tab[1]);
		serial_puts("(-o-)");
	}
}
/* Fonction de déplacement vers la gauche d'un monstre */
static void one_monster_left(uint8_t num_monstre,uint8_t num_lignes)
{
	uint8_t *tab;
	if (num_lignes == 1){
		tab = pointeur->ligne1[num_monstre];
	}
	else if (num_lignes == 2){
		tab = pointeur->ligne2[num_monstre];
	}
	else if (num_lignes == 3){
		tab = pointeur->ligne3[num_monstre];
	}
	if (tab[2] == 1)
	{
		vt100_move(tab[0],tab[1]);
		serial_puts("     ");
		tab[0] -= 1;
		vt100_move(tab[0],
				tab[1]);
		serial_puts("(-o-)");
	}
}
/* Fonction de déplacement vers le bas lorsqu'un monstre a atteind la bordure droite */
static void one_monster_down_right(uint8_t num_monstre, uint8_t num_lignes)
{
	uint8_t *tab;
	if (num_lignes == 1){
		tab = pointeur->ligne1[num_monstre];
	}
	else if (num_lignes == 2){
		tab = pointeur->ligne2[num_monstre];
	}
	else if (num_lignes == 3){
		tab = pointeur->ligne3[num_monstre];
	}
	if (tab[2] == 1)
	{
		val_sens = 1;
		vt100_move(tab[0],tab[1]);
		serial_puts("     ");
		tab[1] += 1;
		vt100_move(tab[0],tab[1]);
		serial_puts("(-o-)");
	}
}
/* Fonction de déplacement vers le bas lorsqu'un monstre a atteind la bordure gauche */
static void one_monster_down_left(uint8_t num_monstre,uint8_t num_lignes)
{
	uint8_t *tab;
	if (num_lignes == 1){
		tab = pointeur->ligne1[num_monstre];
	}
	else if (num_lignes == 2){
		tab = pointeur->ligne2[num_monstre];
	}
	else if (num_lignes == 3){
		tab = pointeur->ligne3[num_monstre];
	}
	if (tab[2] == 1)
	{
		val_sens = 0;
		vt100_move(tab[0],tab[1]);
		serial_puts("     ");
		tab[1] += 1;
		vt100_move(tab[0],tab[1]);
		serial_puts("(-o-)");
	}
}

void all_monster(void)
{
	/* Test est ce que monstre N°7 est vivant et n'a pas atteind la bordure droite */
	if ((pointeur->ligne1[6][0] < 75 && val_sens == 0 && pointeur->ligne1[6][2] == 1))
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i,1);
			one_monster_right(i,2);
		}
		if (pointeur->ligne1[6][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
			}
		}
	}
	/* Test est ce que monstre N°6 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[5][0] < 75 && val_sens == 0
			&& pointeur->ligne1[5][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i,1);
			one_monster_right(i,2);
		}
		if (pointeur->ligne1[5][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
			}
		}
	}
	/* Test est ce que monstre N°5 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[4][0] < 75 && val_sens == 0
			&& pointeur->ligne1[4][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i,1);
			one_monster_right(i,2);
		}
		if (pointeur->ligne1[4][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
			}
		}
	}
	/* Test est ce que monstre N°4 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[3][0] < 75 && val_sens == 0
			&& pointeur->ligne1[3][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i,1);
			one_monster_right(i,2);
		}
		if (pointeur->ligne1[3][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
			}
		}
	}
	/* Test est ce que monstre N°3 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[2][0] < 75 && val_sens == 0
			&& pointeur->ligne1[2][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i,1);
			one_monster_right(i,2);
		}
		if (pointeur->ligne1[2][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
			}
		}
	}
	/* Test est ce que monstre N°2 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[1][0] < 75 && val_sens == 0
			&& pointeur->ligne1[1][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i,1);
			one_monster_right(i,2);
		}
		if (pointeur->ligne1[1][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
			}
		}
	}
	/* Test est ce que monstre N°1 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[0][0] < 75 && val_sens == 0
			&& pointeur->ligne1[0][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i,1);
			one_monster_right(i,2);
		}
		if (pointeur->ligne1[0][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
			}
		}
	}
	/* Test est ce que monstre N°1 est vivant et n'a pas atteind la bordure gauche */
	if (pointeur->ligne1[0][0] > 2 && val_sens == 1 && pointeur->ligne1[0][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
		}
		if (pointeur->ligne1[0][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
			}
		}
	}
	else if (pointeur->ligne1[1][0] > 2 && val_sens == 1 && pointeur->ligne1[1][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
		}
		if (pointeur->ligne1[1][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
			}
		}
	}
	else if (pointeur->ligne1[2][0] > 2 && val_sens == 1 && pointeur->ligne1[2][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
		}
		if (pointeur->ligne1[2][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
			}
		}
	}
	else if (pointeur->ligne1[3][0] > 2 && val_sens == 1 && pointeur->ligne1[3][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
		}
		if (pointeur->ligne1[3][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
			}
		}
	}
	else if (pointeur->ligne1[4][0] > 2 && val_sens == 1 && pointeur->ligne1[4][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
		}
		if (pointeur->ligne1[4][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
			}
		}
	}
	else if (pointeur->ligne1[5][0] > 2 && val_sens == 1 && pointeur->ligne1[5][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
		}
		if (pointeur->ligne1[5][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
			}
		}
	}
	else if (pointeur->ligne1[6][0] > 2 && val_sens == 1 && pointeur->ligne1[6][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
		}
		if (pointeur->ligne1[6][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
			}
		}
	}
}

/* Fonction d'initialisation de tous mes monstres */
void ini_monstres(void)
{
	uint8_t val = 3;
	/* Initialise ma ligne 1,2 et 3 de monstres */
	for (uint8_t i = 0; i < 7; i++)
	{
		/* On met une abscisse de départ */
		pointeur->ligne1[i][0] = val;
		pointeur->ligne2[i][0] = val;
//		pointeur->ligne3[i][0] = val;

		/* On met une ordonnée de départ */
		pointeur->ligne1[i][1] = 2;
		pointeur->ligne2[i][1] = 4;
//		pointeur->ligne3[i][1] = 6;

		/* On initialise les monstres VIVANT */
		pointeur->ligne1[i][2] = 1;
		pointeur->ligne2[i][2] = 1;
//		pointeur->ligne3[i][2] = 1;

		vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
		serial_puts("(-o-)");
		vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
		serial_puts("/-o-\\");
//		vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
//		serial_puts("(-o-)");
		val += 10;
	}
}
/* Fonction d'initialisation de tous mes blocs */
void ini_blocs(void)
{
/* Initialisation du bloc 1 afin d'initialiser les autres */
	pointeur->bloc1[0][0] = 9;
	pointeur->bloc1[0][1] = 20;
	pointeur->bloc1[1][0] = 13;
	pointeur->bloc1[1][1] = 20;
	pointeur->bloc1[2][0] = 10;
	pointeur->bloc1[2][1] = 19;
	pointeur->bloc1[3][0] = 11;
	pointeur->bloc1[3][1] = 19;
	pointeur->bloc1[4][0] = 12;
	pointeur->bloc1[4][1] = 19;
	pointeur->bloc1[5][0] = 11;
	pointeur->bloc1[5][1] = 18;
	for (uint8_t i = 0; i < 6; i++)
	{
/* Initialisation des coordonnées des blocs */
		pointeur->bloc2[i][0] = pointeur->bloc1[i][0] + 20;
		pointeur->bloc2[i][1] = pointeur->bloc1[i][1];
		pointeur->bloc3[i][0] = pointeur->bloc2[i][0] + 20;
		pointeur->bloc3[i][1] = pointeur->bloc2[i][1];
		pointeur->bloc4[i][0] = pointeur->bloc3[i][0] + 20;
		pointeur->bloc4[i][1] = pointeur->bloc3[i][1];
/* Initialisation des blocs sur VIVANT */
		pointeur->bloc1[i][2] = 1;
		pointeur->bloc2[i][2] = 1;
		pointeur->bloc3[i][2] = 1;
		pointeur->bloc4[i][2] = 1;
/* Affichage des blocs */
		vt100_move(pointeur->bloc1[i][0], pointeur->bloc1[i][1]);
		serial_puts("□");
		vt100_move(pointeur->bloc2[i][0], pointeur->bloc2[i][1]);
		serial_puts("□");
		vt100_move(pointeur->bloc3[i][0], pointeur->bloc3[i][1]);
		serial_puts("□");
		vt100_move(pointeur->bloc4[i][0], pointeur->bloc4[i][1]);
		serial_puts("□");
	}
}
