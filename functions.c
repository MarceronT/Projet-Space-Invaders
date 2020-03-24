#include "functions.h"
#include "serial.h"
#include "vt100.h"
#define NB_MONSTRES (7)
#define COOR_LIFE (3)
#define BLOCS (6)
#define COOR (2)
#define LIFE (1)


typedef struct
{
	uint8_t ligne1[NB_MONSTRES][COOR_LIFE];
	uint8_t ligne2[NB_MONSTRES][COOR_LIFE];
	uint8_t ligne3[NB_MONSTRES][COOR_LIFE];
	uint8_t bloc1[BLOCS][COOR_LIFE];
	uint8_t bloc2[BLOCS][COOR_LIFE];
	uint8_t bloc3[BLOCS][COOR_LIFE];
	uint8_t bloc4[BLOCS][COOR_LIFE];
	uint8_t barre[COOR];
	uint8_t tir[COOR_LIFE];
	uint8_t tir_monstre[NB_MONSTRES][COOR_LIFE];
	uint8_t vie[LIFE];
	uint8_t score[1];
	uint8_t fin[1];
} monstres_t;

monstres_t monstre;
monstres_t *pointeur = &monstre;
static uint8_t val_sens = 0;
static uint8_t tab_tir[2];
static uint8_t val_touch = 0;
static uint8_t val_life = 0;
static uint8_t val_score = 0;

/* Affichage de la fenêtre de jeu */
void window(uint8_t character_h, uint8_t character_v)
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

/* Affichage du menu */
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

/* Initialisation et affichage de la vie */
void ini_life(void)
{
	pointeur->vie[0] = 3;
	pointeur->fin[0] = 0;
	vt100_move(72, 23);
	serial_puts("Life: 2");
}

/* Initialisation des coordonnées du vaisseau */
void ini_ship(void)
{
	pointeur->barre[0] = 38;
	pointeur->barre[1] = 22;
}

/* Initialisation du tir de mon vaisseau */
void ini_shot(void)
{
	/* Initialisation des coordonnées du tir et de l'état du tir */
	pointeur->tir[0] = pointeur->barre[0];
	pointeur->tir[1] = 21;
	pointeur->tir[2] = 0;
}

/* Initialisation des tirs de chaque monstres */
void ini_shot_monster(void)
{
	/* Initialisation des coordonnées des tirs et de l'état des tirs */
	for(uint8_t i=0; i<7;i++){
		pointeur->tir_monstre[i][0] = 0;
		pointeur->tir_monstre[i][1] = 0;
		pointeur->tir_monstre[i][2] = 0;
	}
}

/* Fonction d'initialisation de tous mes monstres */
void ini_monster(void)
{
	uint8_t val = 3;
	/* Initialise ma ligne 1,2 et 3 de monstres */
	for (uint8_t i = 0; i < 7; i++)
	{
		/* On met une abscisse de départ */
		pointeur->ligne1[i][0] = val;
		pointeur->ligne2[i][0] = val;
		pointeur->ligne3[i][0] = val;

		/* On met une ordonnée de départ */
		pointeur->ligne1[i][1] = 2;
		pointeur->ligne2[i][1] = 4;
		pointeur->ligne3[i][1] = 6;

		/* On initialise les monstres VIVANT */
		pointeur->ligne1[i][2] = 1;
		pointeur->ligne2[i][2] = 1;
		pointeur->ligne3[i][2] = 1;

		/* Affichage de mes monstres */
		vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
		serial_puts("(-o-)");
		vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
		serial_puts("/-o-\\");
		vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
		serial_puts("(-o-)");
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

uint8_t life(void)
{
	/* On utilise la variable val_life pour que notre programme passe qu'une fois dans les conditions */
	if (pointeur->vie[0] == 2 && val_life == 0){
		val_life = 1;
		vt100_move(72, 23);
		serial_puts("Life: 1");
		return pointeur->vie[0];
	}
	else if (pointeur->vie[0] == 1 && val_life == 1){
		val_life = 0;
		vt100_move(72, 23);
		serial_puts("Life: 0");
		return pointeur->vie[0];
	}
	else if (pointeur->vie[0] == 0){
		vt100_clear_screen();
		for (uint8_t index = 1; index < 80; index++)
		{
			vt100_move(index, 1);
			serial_putchar(94);
			vt100_move(index, 24);
			serial_putchar(94);
			vt100_move(1, index);
			serial_putchar(124);
			vt100_move(80, index);
			serial_putchar(124);
		}
		vt100_move(35, 6);
		serial_puts("YOU HAVE LOST ...");
		vt100_move(32, 14);
		serial_puts("Press any touch");
		vt100_move(36, 12);
		serial_puts("To RETRY");
		while (serial_get_last_char() == -1)
		{
		}
		return pointeur->vie[0];
	}
	return pointeur->vie[0];
}

void score(void)
{
	/* Dès qu'un monstre meurt on ajoute 10 au score */
	if (pointeur->fin[0] == 0 && val_score == 0){
		val_score = 1;
		vt100_move(2, 23);
		serial_puts("Score: 0");
	}
	else if (pointeur->fin[0] == 1 && val_score == 1){
		val_score = 2;
		vt100_move(2, 23);
		serial_puts("Score: 10");
	}
	else if (pointeur->fin[0] == 2 && val_score == 2){
		val_score = 3;
		vt100_move(2, 23);
		serial_puts("Score: 20");
	}
	else if (pointeur->fin[0] == 3 && val_score == 3){
		val_score = 4;
		vt100_move(2, 23);
		serial_puts("Score: 30");
	}
	else if (pointeur->fin[0] == 4 && val_score == 4){
		val_score = 5;
		vt100_move(2, 23);
		serial_puts("Score: 40");
	}
	else if (pointeur->fin[0] == 5 && val_score == 5){
		val_score = 6;
		vt100_move(2, 23);
		serial_puts("Score: 50");
	}
	else if (pointeur->fin[0] == 6 && val_score == 6){
		val_score = 7;
		vt100_move(2, 23);
		serial_puts("Score: 60");
	}
	else if (pointeur->fin[0] == 7 && val_score == 7){
		val_score = 8;
		vt100_move(2, 23);
		serial_puts("Score: 70");
	}
	else if (pointeur->fin[0] == 8 && val_score == 8){
		val_score = 9;
		vt100_move(2, 23);
		serial_puts("Score: 80");
	}
	else if (pointeur->fin[0] == 9 && val_score == 9){
		val_score = 10;
		vt100_move(2, 23);
		serial_puts("Score: 90");
	}
	else if (pointeur->fin[0] == 10 && val_score == 10){
		val_score = 11;
		vt100_move(2, 23);
		serial_puts("Score: 100");
	}
	else if (pointeur->fin[0] == 11 && val_score == 11){
		val_score = 12;
		vt100_move(2, 23);
		serial_puts("Score: 110");
	}
	else if (pointeur->fin[0] == 12 && val_score == 12){
		val_score = 13;
		vt100_move(2, 23);
		serial_puts("Score: 120");
	}
	else if (pointeur->fin[0] == 13 && val_score == 13){
		val_score = 14;
		vt100_move(2, 23);
		serial_puts("Score: 130");
	}
	else if (pointeur->fin[0] == 14 && val_score == 14){
		val_score = 15;
		vt100_move(2, 23);
		serial_puts("Score: 140");
	}
	else if (pointeur->fin[0] == 15 && val_score == 15){
		val_score = 16;
		vt100_move(2, 23);
		serial_puts("Score: 150");
	}
	else if (pointeur->fin[0] == 16 && val_score == 16){
		val_score = 17;
		vt100_move(2, 23);
		serial_puts("Score: 160");
	}
	else if (pointeur->fin[0] == 17 && val_score == 17){
		val_score = 18;
		vt100_move(2, 23);
		serial_puts("Score: 170");
	}
	else if (pointeur->fin[0] == 18 && val_score == 18){
		val_score = 19;
		vt100_move(2, 23);
		serial_puts("Score: 180");
	}
	else if (pointeur->fin[0] == 19 && val_score == 19){
		val_score = 20;
		vt100_move(2, 23);
		serial_puts("Score: 190");
	}
	else if (pointeur->fin[0] == 20 && val_score == 20){
		val_score = 0;
		vt100_move(2, 23);
		serial_puts("Score: 200");
	}
}

/* Fonction d'attente */
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

/* Fonction de déplacement de mon vaisseau */
void ship(void)
{
	uint8_t touche = serial_get_last_char();
	vt100_move(pointeur->barre[0], pointeur->barre[1]);
	serial_puts("/-|-\\");
	/* Bordure de l'écran droit */
	if (pointeur->barre[0] < 75)
	{
		/* Déplacement du vaisseau vers la droite */
		if (touche == 'd')
		{
			vt100_move(pointeur->barre[0], pointeur->barre[1]);
			serial_puts("     ");
			pointeur->barre[0] += 1;
			vt100_move(pointeur->barre[0], pointeur->barre[1]);
			serial_puts("/-|-\\");
		}
	}
	/* Bordure de l'écran gauche */
	if (pointeur->barre[0] > 2)
	{
		/* Déplacement du vaisseau vers la gauche */
		if (touche == 'q')
		{
			vt100_move(pointeur->barre[0], pointeur->barre[1]);
			serial_puts("     ");
			pointeur->barre[0] -= 1;
			vt100_move(pointeur->barre[0], pointeur->barre[1]);
			serial_puts("/-|-\\");
		}
	}
	/* On appel la fonction tir si on appui sur la touche m */
	if (touche == 'm')
	{
		/* On initialise le tir sur "En Cours" */
		pointeur->tir[2] = 1;
		/* On appel la fonction tir */
		shot();
	}
}

void shot(void)
{
	/* On récupére l'abscisse de la barre jusqu'à qu'un ennemi soit touché ou que le tir ait atteind ca limite */
	if (pointeur->tir[2] == 0)
	{
		pointeur->tir[0] = pointeur->barre[0];
	}
	/* Si le tir n'a pas atteind la bordure et si il a rien touché on le décale de 1 */
	if (pointeur->tir[1] > 2 && pointeur->tir[2] == 1)
	{
		pointeur->tir[2] = 1;
		vt100_move(pointeur->tir[0]+2,pointeur->tir[1]);
		serial_puts(" ");
		pointeur->tir[1] -= 1;
		vt100_move(pointeur->tir[0]+2,pointeur->tir[1]);
		serial_puts("î");
		sleep(1);
		tab_tir[0] = pointeur->tir[0]+2;
		tab_tir[1] = pointeur->tir[1];
		hit_box();
	}
	/* Vérification si le tir a touché la bordure, si oui on remet l'ordonnée et l'abscisse du vaisseau puis on déclare que le tir est fini  */
	else if (pointeur->tir[1] == 2)
	{
		pointeur->tir[2] = 0;
		vt100_move(pointeur->tir[0]+2,pointeur->tir[1]);
		serial_puts(" ");
		pointeur->tir[1] = 21;
	}
	/* Vérification si le tir a touché un ennemie, si oui on remet par défaut la var val_touch, on remet l'ordonnée du vaisseau puis on déclare que le tir est fini */
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
			pointeur->fin[0] += 1;
		}
		else if ((pointeur->ligne2[i][0] == tab_tir[0] || pointeur->ligne2[i][0]+1 == tab_tir[0] || pointeur->ligne2[i][0]+2 == tab_tir[0] || pointeur->ligne2[i][0]+3 == tab_tir[0] || pointeur->ligne2[i][0]+4 == tab_tir[0]) && (pointeur->ligne2[i][1] == tab_tir[1]) && (pointeur->ligne2[i][2] == 1))
		{
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("     ");
			pointeur->ligne2[i][2] = 0;
			val_touch = 1;
			pointeur->fin[0] += 1;
		}
		else if ((pointeur->ligne3[i][0] == tab_tir[0] || pointeur->ligne3[i][0]+1 == tab_tir[0] || pointeur->ligne3[i][0]+2 == tab_tir[0] || pointeur->ligne3[i][0]+3 == tab_tir[0] || pointeur->ligne3[i][0]+4 == tab_tir[0]) && (pointeur->ligne3[i][1] == tab_tir[1]) && (pointeur->ligne3[i][2] == 1))
		{
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("     ");
			pointeur->ligne3[i][2] = 0;
			val_touch = 1;
			pointeur->fin[0] += 1;
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
	/* On chosit quelles lignes de monstres on veut déplacer */
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
	/* On décale le monstre de 1 vers la droite si il est en vie */
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
	/* On chosit quelles lignes de monstres on veut déplacer */
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
	/* On décale le monstre de 1 vers la gauche si il est en vie */
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
	/* On chosit quelles lignes de monstres on veut déplacer */
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
	/* On décale le monstre de 1 vers le bas si il est en vie */
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
	/* On chosit quelles lignes de monstres on veut déplacer */
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
	/* On décale le monstre de 1 vers le bas si il est en vie */
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
		/* On les déplaces tous à droite si ils sont en vie */
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i,1);
			one_monster_right(i,2);
			one_monster_right(i,3);
		}
		if (pointeur->ligne1[6][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
				one_monster_down_right(i,3);
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
			one_monster_right(i,3);
		}
		if (pointeur->ligne1[5][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
				one_monster_down_right(i,3);
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
			one_monster_right(i,3);
		}
		if (pointeur->ligne1[4][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
				one_monster_down_right(i,3);
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
			one_monster_right(i,3);
		}
		if (pointeur->ligne1[3][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
				one_monster_down_right(i,3);
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
			one_monster_right(i,3);
		}
		if (pointeur->ligne1[2][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
				one_monster_down_right(i,3);
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
			one_monster_right(i,3);
		}
		if (pointeur->ligne1[1][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
				one_monster_down_right(i,3);
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
			one_monster_right(i,3);
		}
		if (pointeur->ligne1[0][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i,1);
				one_monster_down_right(i,2);
				one_monster_down_right(i,3);
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
			one_monster_left(i,3);
		}
		if (pointeur->ligne1[0][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
				one_monster_down_left(i,3);
			}
		}
	}
	/* Test est ce que monstre N°2 est vivant et n'a pas atteind la bordure gauche */
	else if (pointeur->ligne1[1][0] > 2 && val_sens == 1 && pointeur->ligne1[1][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
			one_monster_left(i,3);
		}
		if (pointeur->ligne1[1][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
				one_monster_down_left(i,3);
			}
		}
	}
	/* Test est ce que monstre N°3 est vivant et n'a pas atteind la bordure gauche */
	else if (pointeur->ligne1[2][0] > 2 && val_sens == 1 && pointeur->ligne1[2][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
			one_monster_left(i,3);
		}
		if (pointeur->ligne1[2][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
				one_monster_down_left(i,3);
			}
		}
	}
	/* Test est ce que monstre N°4 est vivant et n'a pas atteind la bordure gauche */
	else if (pointeur->ligne1[3][0] > 2 && val_sens == 1 && pointeur->ligne1[3][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
			one_monster_left(i,3);
		}
		if (pointeur->ligne1[3][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
				one_monster_down_left(i,3);
			}
		}
	}
	/* Test est ce que monstre N°5 est vivant et n'a pas atteind la bordure gauche */
	else if (pointeur->ligne1[4][0] > 2 && val_sens == 1 && pointeur->ligne1[4][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
			one_monster_left(i,3);
		}
		if (pointeur->ligne1[4][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
				one_monster_down_left(i,3);
			}
		}
	}
	/* Test est ce que monstre N°6 est vivant et n'a pas atteind la bordure gauche */
	else if (pointeur->ligne1[5][0] > 2 && val_sens == 1 && pointeur->ligne1[5][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
			one_monster_left(i,3);
		}
		if (pointeur->ligne1[5][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
				one_monster_down_left(i,3);
			}
		}
	}
	/* Test est ce que monstre N°7 est vivant et n'a pas atteind la bordure gauche */
	else if (pointeur->ligne1[6][0] > 2 && val_sens == 1 && pointeur->ligne1[6][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i,1);
			one_monster_left(i,2);
			one_monster_left(i,3);
		}
		if (pointeur->ligne1[6][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i,1);
				one_monster_down_left(i,2);
				one_monster_down_left(i,3);
			}
		}
	}
}

void shot_one_monster(uint8_t num_monstre,uint8_t num_lignes)
{
	/* On choisit quelle monstre on veut faire tirer */
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
	/* On test si aucun tir n'est en cours et on attribut les coordonnées du monstre au tir et on le déclare sur "en cours"*/
	if (pointeur->tir_monstre[num_monstre][2] == 0)
	{
		pointeur->tir_monstre[num_monstre][0] = tab[0];
		pointeur->tir_monstre[num_monstre][1] = tab[1]+1;
		pointeur->tir_monstre[num_monstre][2] = 1;
	}
	/* On prend les coordonnées du tir on ajoute 1 à l'ordonnée et on test la hit box*/
	else if (pointeur->tir_monstre[num_monstre][1] < 22)
	{
		vt100_move(pointeur->tir_monstre[num_monstre][0]+2,pointeur->tir_monstre[num_monstre][1]);
		serial_puts(" ");
		pointeur->tir_monstre[num_monstre][1] += 1;
		vt100_move(pointeur->tir_monstre[num_monstre][0]+2,pointeur->tir_monstre[num_monstre][1]);
		serial_puts("|");
		sleep(1);
		hit_box_monster();
	}
	/* On test si le tir à touché la bordure si oui on déclare qu'il n'est plus en cours */
	else if (pointeur->tir_monstre[num_monstre][1] == 22)
	{
		pointeur->tir_monstre[num_monstre][2] = 0;
		vt100_move(pointeur->tir_monstre[num_monstre][0]+2,pointeur->tir_monstre[num_monstre][1]);
		serial_puts(" ");
	}
}

void shot_all_monster(uint8_t num_monstre)
{
	/* On test si l'un des monstres de la ligne 3 est vivant et on continue le tir si un monstre à déjà tiré et est mort pendant son tir */
	if (pointeur->ligne3[num_monstre][2] == 1 || (pointeur->tir_monstre[num_monstre][2] == 1 && pointeur->ligne3[num_monstre][2] == 0))
	{
		shot_one_monster(num_monstre,3);
	}
	/* On test si l'un des monstres de la ligne 2 est vivant et on continue le tir si un monstre à déjà tiré et est mort pendant son tir */
	else if (pointeur->ligne2[num_monstre][2] == 1 || (pointeur->tir_monstre[num_monstre][2] == 1 && pointeur->ligne2[num_monstre][2] == 0))
	{
		shot_one_monster(num_monstre,2);
	}
	/* On test si l'un des monstres de la ligne 1 est vivant et on continue le tir si un monstre à déjà tiré et est mort pendant son tir */
	else if (pointeur->ligne1[num_monstre][2] == 1 || (pointeur->tir_monstre[num_monstre][2] == 1 && pointeur->ligne1[num_monstre][2] == 0))
	{
		shot_one_monster(num_monstre,1);
	}
}

void hit_box_monster(void)
{
	/* On test si l'un des tirs des monstres touchent mon vaisseau */
	for (uint8_t i = 0; i < 7; i++)
	{
		if ((pointeur->tir_monstre[i][0]+2 == pointeur->barre[0] || pointeur->tir_monstre[i][0]+2 == pointeur->barre[0]+1 || pointeur->tir_monstre[i][0]+2 == pointeur->barre[0]+2 || pointeur->tir_monstre[i][0]+2 == pointeur->barre[0]+3 || pointeur->tir_monstre[i][0]+2 == pointeur->barre[0]+4) && (pointeur->tir_monstre[i][1] == pointeur->barre[1]) && (pointeur->tir_monstre[i][2] == 1))
		{
			vt100_move(pointeur->tir_monstre[i][0]+2, pointeur->tir_monstre[i][1]);
			serial_puts(" ");
			vt100_move(pointeur->barre[0], pointeur->barre[1]);
			serial_puts("     ");
			pointeur->vie[0] -= 1;
			pointeur->tir_monstre[i][2] = 0;
		/* On test si les tirs des monstres touche le tir de mon vaisseau si oui on les effaces */
		}
		else if ((pointeur->tir_monstre[i][0]+2 == pointeur->tir[0]+2 && pointeur->tir_monstre[i][1] == pointeur->tir[1]) && pointeur->tir_monstre[i][2] == 1 && pointeur->tir[2] == 1)
		{
			vt100_move(pointeur->tir_monstre[i][0]+2, pointeur->tir_monstre[i][1]);
			serial_puts(" ");
			vt100_move(pointeur->tir[0]+2, pointeur->tir[1]);
			serial_puts(" ");
			pointeur->tir_monstre[i][2] = 0;
			pointeur->tir[2] = 0;
		}
	}
	/* On test si l'un des tirs des monstres touchent l'un des blocs */
	for (uint8_t t = 0; t < 7; t++)
	{
		for (uint8_t i = 0; i < 6; i++)
		{
			if (pointeur->bloc1[i][0] == pointeur->tir_monstre[t][0]+2 && pointeur->bloc1[i][1] == pointeur->tir_monstre[t][1] && pointeur->bloc1[i][2] == 1)
			{
				vt100_move(pointeur->bloc1[i][0], pointeur->bloc1[i][1]);
				serial_puts(" ");
				vt100_move(pointeur->tir_monstre[t][0]+2, pointeur->tir_monstre[t][1]);
				serial_puts(" ");
				pointeur->bloc1[i][2] = 0;
				pointeur->tir_monstre[t][2] = 0;
			}
			else if (pointeur->bloc2[i][0] == pointeur->tir_monstre[t][0]+2 && pointeur->bloc2[i][1] == pointeur->tir_monstre[t][1] && pointeur->bloc2[i][2] == 1)
			{
				vt100_move(pointeur->bloc2[i][0], pointeur->bloc2[i][1]);
				serial_puts(" ");
				vt100_move(pointeur->tir_monstre[t][0]+2, pointeur->tir_monstre[t][1]);
				serial_puts(" ");
				pointeur->bloc2[i][2] = 0;
				pointeur->tir_monstre[t][2] = 0;
			}
			else if (pointeur->bloc3[i][0] == pointeur->tir_monstre[t][0]+2 && pointeur->bloc3[i][1] == pointeur->tir_monstre[t][1] && pointeur->bloc3[i][2] == 1)
			{
				vt100_move(pointeur->bloc3[i][0], pointeur->bloc3[i][1]);
				serial_puts(" ");
				vt100_move(pointeur->tir_monstre[t][0]+2, pointeur->tir_monstre[t][1]);
				serial_puts(" ");
				pointeur->bloc3[i][2] = 0;
				pointeur->tir_monstre[t][2] = 0;
			}
			else if (pointeur->bloc4[i][0] == pointeur->tir_monstre[t][0]+2 && pointeur->bloc4[i][1] == pointeur->tir_monstre[t][1] && pointeur->bloc4[i][2] == 1)
			{
				vt100_move(pointeur->bloc4[i][0], pointeur->bloc4[i][1]);
				serial_puts(" ");
				vt100_move(pointeur->tir_monstre[t][0]+2, pointeur->tir_monstre[t][1]);
				serial_puts(" ");
				pointeur->bloc4[i][2] = 0;
				pointeur->tir_monstre[t][2] = 0;
			}
		}
	}
}

uint8_t end(void)
{
	/* On test si tous les monstres on été tués */
	if (pointeur->fin[0] == 21)
	{
		vt100_clear_screen();
		for (uint8_t index = 1; index < 80; index++)
		{
			vt100_move(index, 1);
			serial_putchar(94);
			vt100_move(index, 24);
			serial_putchar(94);
			vt100_move(1, index);
			serial_putchar(124);
			vt100_move(80, index);
			serial_putchar(124);
		}
		vt100_move(37, 6);
		serial_puts("YOU HAVE WIN !");
		vt100_move(34, 14);
		serial_puts("Press any touch");
		vt100_move(39, 12);
		serial_puts("To RETRY");
		while (serial_get_last_char() == -1)
		{
		}
		return pointeur->fin[0];
	}
}

uint8_t end_2 (void)
{
	/* On test si l'un des monstres de la ligne 3 à la même ordonnée que notre vaisseau, si oui c'est la fin du jeu */
	for (uint8_t i = 0; i < 7; i++){
		if (pointeur->ligne3[i][1] == pointeur->barre[1] && pointeur->ligne3[i][2] == 1)
		{
			vt100_clear_screen();
			for (uint8_t index = 1; index < 80; index++)
			{
				vt100_move(index, 1);
				serial_putchar(94);
				vt100_move(index, 24);
				serial_putchar(94);
				vt100_move(1, index);
				serial_putchar(124);
				vt100_move(80, index);
				serial_putchar(124);
			}
			vt100_move(37, 6);
			serial_puts("YOU HAVE LOST ...");
			vt100_move(34, 14);
			serial_puts("Press any touch");
			vt100_move(39, 12);
			serial_puts("To RETRY");
			while (serial_get_last_char() == -1)
			{
			}
			return 0;
		}
		/* On test si l'un des monstres de la ligne 2 à la même ordonnée que notre vaisseau, si oui c'est la fin du jeu */
		else if (pointeur->ligne2[i][1] == pointeur->barre[1] && pointeur->ligne2[i][2] == 1)
		{
			vt100_clear_screen();
			for (uint8_t index = 1; index < 80; index++)
			{
				vt100_move(index, 1);
				serial_putchar(94);
				vt100_move(index, 24);
				serial_putchar(94);
				vt100_move(1, index);
				serial_putchar(124);
				vt100_move(80, index);
				serial_putchar(124);
			}
			vt100_move(37, 6);
			serial_puts("YOU HAVE LOST ...");
			vt100_move(34, 14);
			serial_puts("Press any touch");
			vt100_move(39, 12);
			serial_puts("To RETRY");
			while (serial_get_last_char() == -1)
			{
			}
			return 0;
		}
		/* On test si l'un des monstres de la ligne 1 à la même ordonnée que notre vaisseau, si oui c'est la fin du jeu */
		else if (pointeur->ligne1[i][1] == pointeur->barre[1] && pointeur->ligne1[i][2] == 1)
		{
			vt100_clear_screen();
			for (uint8_t index = 1; index < 80; index++)
			{
				vt100_move(index, 1);
				serial_putchar(94);
				vt100_move(index, 24);
				serial_putchar(94);
				vt100_move(1, index);
				serial_putchar(124);
				vt100_move(80, index);
				serial_putchar(124);
			}
			vt100_move(37, 6);
			serial_puts("YOU HAVE LOST ...");
			vt100_move(34, 14);
			serial_puts("Press any touch");
			vt100_move(39, 12);
			serial_puts("To RETRY");
			while (serial_get_last_char() == -1)
			{
			}
			return 0;
		}
	}
}
