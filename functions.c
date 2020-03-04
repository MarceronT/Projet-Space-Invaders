#include "functions.h"
#include "serial.h"
#include "vt100.h"
#define NB_MONSTRES (7)
#define COOR_LIFE (3)
#define BLOCS (6)
#define COOR (2)


typedef struct
{
	uint8_t ligne1[NB_MONSTRES][COOR_LIFE];
	uint8_t ligne2[NB_MONSTRES][COOR_LIFE];
	uint8_t ligne3[NB_MONSTRES][COOR_LIFE];
	uint8_t bloc1[BLOCS][COOR];
	uint8_t bloc2[BLOCS][COOR];
	uint8_t bloc3[BLOCS][COOR];
	uint8_t bloc4[BLOCS][COOR];

} monstres_t;

monstres_t monstre;
monstres_t *pointeur = &monstre;
static uint8_t t = 38;
static uint8_t val_sens = 0;
static uint8_t tab_tir[2];
uint8_t val_touch;

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
	if (touche == 'm')
	{
		tir();
	}
}
static uint8_t m;
void tir(void)
{
	val_touch = 0;
	for (m = 21; m >= 2; m--)
	{
		vt100_move(t + 2, m);
		serial_puts("î");
		sleep(1);
		vt100_move(t + 2, m);
		serial_puts(" ");
		tab_tir[0] = t + 2;
		tab_tir[1] = m;
		if (hit_box() == 1){
			break;
		}
	}
}

uint8_t hit_box(void)
{
	for (uint8_t i = 0; i < 7; i++)
	{
		if ((pointeur->ligne1[i][0] == tab_tir[0] || pointeur->ligne1[i][0]+1 == tab_tir[0] || pointeur->ligne1[i][0]+2 == tab_tir[0] || pointeur->ligne1[i][0]+3 == tab_tir[0] || pointeur->ligne1[i][0]+4 == tab_tir[0]) && (pointeur->ligne1[i][1] == tab_tir[1] || pointeur->ligne1[i][1]+1 == tab_tir[1] || pointeur->ligne1[i][1]+2 == tab_tir[1] || pointeur->ligne1[i][1]+3 == tab_tir[1] || pointeur->ligne1[i][1]+4 == tab_tir[1]))
		{
			vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
			serial_puts("     ");
			pointeur->ligne1[i][2] = 0;
			val_touch = 1;
			return val_touch;
		}
		else if (pointeur->ligne2[i][0] == tab_tir[0] && pointeur->ligne2[i][1] == tab_tir[1])
		{
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("     ");
			pointeur->ligne2[i][2] = 0;
		}
		else if (pointeur->ligne3[i][0] == tab_tir[0] && pointeur->ligne3[i][1] == tab_tir[1])
		{
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("     ");
			pointeur->ligne3[i][2] = 0;
		}
	}
}

static void one_monster_right(uint8_t num_monstre)
{
	if (pointeur->ligne1[num_monstre][2] == 1)
	{
		vt100_move(pointeur->ligne1[num_monstre][0],
				pointeur->ligne1[num_monstre][1]);
		serial_puts("     ");
 		pointeur->ligne1[num_monstre][0] += 1;
		vt100_move(pointeur->ligne1[num_monstre][0],
				pointeur->ligne1[num_monstre][1]);
		serial_puts("(-o-)");
	}
}
static void one_monster_left(uint8_t num_monstre)
{
	if (pointeur->ligne1[num_monstre][2] == 1)
	{
		vt100_move(pointeur->ligne1[num_monstre][0],pointeur->ligne1[num_monstre][1]);
		serial_puts("     ");
		pointeur->ligne1[num_monstre][0] -= 1;
		vt100_move(pointeur->ligne1[num_monstre][0],
				pointeur->ligne1[num_monstre][1]);
		serial_puts("(-o-)");
	}
}

static void one_monster_down_right(uint8_t num_monstre)
{
	if (pointeur->ligne1[num_monstre][2] == 1)
	{
		val_sens = 1;
		vt100_move(pointeur->ligne1[num_monstre][0],pointeur->ligne1[num_monstre][1]);
		serial_puts("     ");
		pointeur->ligne1[num_monstre][1] += 1;
		vt100_move(pointeur->ligne1[num_monstre][0],pointeur->ligne1[num_monstre][1]);
		serial_puts("(-o-)");
	}
}

static void one_monster_down_left(uint8_t num_monstre)
{
	if (pointeur->ligne1[num_monstre][2] == 1)
	{
		val_sens = 0;
		vt100_move(pointeur->ligne1[num_monstre][0],pointeur->ligne1[num_monstre][1]);
		serial_puts("     ");
		pointeur->ligne1[num_monstre][1] += 1;
		vt100_move(pointeur->ligne1[num_monstre][0],pointeur->ligne1[num_monstre][1]);
		serial_puts("(-o-)");
	}
}


void all_monster(void)
{
	/* Test est ce que monstre N°7 est vivant et n'a pas atteind la bordure droite */
	if (pointeur->ligne1[6][0] < 75 && val_sens == 0 && pointeur->ligne1[6][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne1[6][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°6 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[5][0] < 75 && val_sens == 0
			&& pointeur->ligne1[5][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne1[5][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°5 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[4][0] < 75 && val_sens == 0
			&& pointeur->ligne1[4][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne1[4][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°4 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[3][0] < 75 && val_sens == 0
			&& pointeur->ligne1[3][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne1[3][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°3 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[2][0] < 75 && val_sens == 0
			&& pointeur->ligne1[2][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne1[2][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°2 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[1][0] < 75 && val_sens == 0
			&& pointeur->ligne1[1][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne1[1][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°1 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne1[0][0] < 75 && val_sens == 0
			&& pointeur->ligne1[0][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne1[0][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°1 est vivant et n'a pas atteind la bordure gauche */
	if (pointeur->ligne1[0][0] > 2 && val_sens == 1 && pointeur->ligne1[0][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne1[0][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne1[1][0] > 2 && val_sens == 1 && pointeur->ligne1[1][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne1[1][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne1[2][0] > 2 && val_sens == 1 && pointeur->ligne1[2][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne1[2][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne1[3][0] > 2 && val_sens == 1 && pointeur->ligne1[3][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne1[3][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne1[4][0] > 2 && val_sens == 1 && pointeur->ligne1[4][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne1[4][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne1[5][0] > 2 && val_sens == 1 && pointeur->ligne1[5][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne1[5][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne1[6][0] > 2 && val_sens == 1 && pointeur->ligne1[6][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne1[6][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
}

void all_monster_2()
{
	/* Test est ce que monstre N°7 est vivant et n'a pas atteind la bordure droite */
	if (pointeur->ligne2[6][0] < 75 && val_sens == 0 && pointeur->ligne2[6][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne2[6][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°6 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne2[5][0] < 75 && val_sens == 0
			&& pointeur->ligne2[5][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne2[5][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°5 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne2[4][0] < 75 && val_sens == 0
			&& pointeur->ligne2[4][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne2[4][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°4 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne2[3][0] < 75 && val_sens == 0
			&& pointeur->ligne2[3][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne2[3][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°3 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne2[2][0] < 75 && val_sens == 0
			&& pointeur->ligne2[2][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne2[2][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°2 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne2[1][0] < 75 && val_sens == 0
			&& pointeur->ligne2[1][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne2[1][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°1 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne2[0][0] < 75 && val_sens == 0
			&& pointeur->ligne2[0][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne2[0][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°1 est vivant et n'a pas atteind la bordure gauche */
	if (pointeur->ligne2[0][0] > 2 && val_sens == 1 && pointeur->ligne2[0][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne2[0][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne2[1][0] > 2 && val_sens == 1 && pointeur->ligne2[1][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne2[1][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne2[2][0] > 2 && val_sens == 1 && pointeur->ligne2[2][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne2[2][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne2[3][0] > 2 && val_sens == 1 && pointeur->ligne2[3][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne2[3][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne2[4][0] > 2 && val_sens == 1 && pointeur->ligne2[4][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne2[4][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne2[5][0] > 2 && val_sens == 1 && pointeur->ligne2[5][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne2[5][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne2[6][0] > 2 && val_sens == 1 && pointeur->ligne2[6][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne2[6][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
}

void all_monster_3()
{
	/* Test est ce que monstre N°7 est vivant et n'a pas atteind la bordure droite */
	if (pointeur->ligne3[6][0] < 75 && val_sens == 0 && pointeur->ligne3[6][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne3[6][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°6 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne3[5][0] < 75 && val_sens == 0
			&& pointeur->ligne3[5][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne3[5][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°5 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne3[4][0] < 75 && val_sens == 0
			&& pointeur->ligne3[4][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne3[4][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°4 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne3[3][0] < 75 && val_sens == 0
			&& pointeur->ligne3[3][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne3[3][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°3 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne3[2][0] < 75 && val_sens == 0
			&& pointeur->ligne3[2][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne3[2][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°2 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne3[1][0] < 75 && val_sens == 0
			&& pointeur->ligne3[1][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne3[1][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°1 est vivant et n'a pas atteind la bordure droite */
	else if (pointeur->ligne3[0][0] < 75 && val_sens == 0
			&& pointeur->ligne3[0][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_right(i);
		}
		if (pointeur->ligne3[0][0] == 75)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_right(i);
			}
		}
	}
	/* Test est ce que monstre N°1 est vivant et n'a pas atteind la bordure gauche */
	if (pointeur->ligne3[0][0] > 2 && val_sens == 1 && pointeur->ligne3[0][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne3[0][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne3[1][0] > 2 && val_sens == 1 && pointeur->ligne3[1][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne3[1][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne3[2][0] > 2 && val_sens == 1 && pointeur->ligne3[2][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne3[2][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne3[3][0] > 2 && val_sens == 1 && pointeur->ligne3[3][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne3[3][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne3[4][0] > 2 && val_sens == 1 && pointeur->ligne3[4][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne3[4][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne3[5][0] > 2 && val_sens == 1 && pointeur->ligne3[5][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne3[5][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
	else if (pointeur->ligne3[6][0] > 2 && val_sens == 1 && pointeur->ligne3[6][2] == 1)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			one_monster_left(i);
		}
		if (pointeur->ligne3[6][0] == 2)
		{
			for (uint8_t i = 0; i < 7; i++)
			{
				one_monster_down_left(i);
			}
		}
	}
}

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
void ini_blocs(void)
{
	pointeur->bloc1[0][0] = 4;
	pointeur->bloc1[0][1] = 20;
	pointeur->bloc1[1][0] = 8;
	pointeur->bloc1[1][1] = 20;
	pointeur->bloc1[2][0] = 5;
	pointeur->bloc1[2][1] = 19;
	pointeur->bloc1[3][0] = 6;
	pointeur->bloc1[3][1] = 19;
	pointeur->bloc1[4][0] = 7;
	pointeur->bloc1[4][1] = 19;
	pointeur->bloc1[5][0] = 6;
	pointeur->bloc1[5][1] = 18;
	for (uint8_t i = 0; i < 6; i++)
	{
		pointeur->bloc2[i][0] = pointeur->bloc1[i][0] + 10;
		pointeur->bloc2[i][1] = pointeur->bloc1[i][1];

		pointeur->bloc3[i][0] = pointeur->bloc2[i][0] + 10;
		pointeur->bloc3[i][1] = pointeur->bloc2[i][1];

		vt100_move(pointeur->bloc1[i][0], pointeur->bloc1[i][1]);
		serial_puts("□");
		vt100_move(pointeur->bloc2[i][0], pointeur->bloc2[i][1]);
		serial_puts("□");
		vt100_move(pointeur->bloc3[i][0], pointeur->bloc3[i][1]);
		serial_puts("□");
	}
}
