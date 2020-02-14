#include "functions.h"
#include "serial.h"
#include "vt100.h"

uint8_t t = 38;
uint8_t val_useless = 0;

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

void menu()
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

void barre()
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
		if (touche == 'm')
		{
			tir();
		}
	}
}
uint8_t m;
void tir()
{
	for (m = 21; m >= 2; m--)
	{
		vt100_move(t + 2, m);
		serial_puts("î");
		sleep(1);
		vt100_move(t + 2, m);
		serial_puts(" ");
	}
}
void sleep(uint32_t n)
{
	/* boucle vide parcourue (n * 100000) fois*/
	int i = 0;
	uint32_t max = n * 100000;
	do
	{
		/* Faire qqch de stupide qui prend du temps */
		i++;
	} while (i <= max);
}

void monstres()
{
	typedef struct
	{
		uint8_t ligne1[7][2];
		uint8_t ligne2[7][2];
		uint8_t ligne3[7][2];
	} monstres_t;
	monstres_t monstre;
	monstres_t *pointeur;
	pointeur = &monstre;
	uint8_t val = 3;
	if (val_useless == 0)
	{
		/* Initialise ma ligne 1,2 et 3 de monstres */
		for (uint8_t i = 0; i < 7; i++)
		{
			pointeur->ligne1[i][0] = val;
			pointeur->ligne3[i][0] = val;
			pointeur->ligne2[i][0] = val;
			pointeur->ligne1[i][1] = 2;
			pointeur->ligne2[i][1] = 4;
			pointeur->ligne3[i][1] = 6;
			vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
			serial_puts("(-o-)");
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("/-o-\\");
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("(-o-)");
			val += 10;
		}
	}
	val_useless = 1;

	/* Déplacements des monstres vers la droite */
	while (pointeur->ligne1[6][0] != 75)
	{
		barre();
		for (uint8_t i = 0; i < 7; i++)
		{
			vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
			serial_puts("     ");
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("     ");
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("     ");
			pointeur->ligne1[i][0] += 1;
			pointeur->ligne2[i][0] += 1;
			pointeur->ligne3[i][0] += 1;
			sleep(4);
			vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
			serial_puts("(-o-)");
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("/-o-\\");
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("(-o-)");
		}
	}

	/* Descente des monstres*/
	if (pointeur->ligne1[6][0] == 75)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
			serial_puts("     ");
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("     ");
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("     ");
			pointeur->ligne1[i][1] += 1;
			pointeur->ligne2[i][1] += 1;
			pointeur->ligne3[i][1] += 1;
			sleep(4);
			vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
			serial_puts("(-o-)");
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("/-o-\\");
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("(-o-)");
		}
	}
	/* Déplacements des monstres vers la gauche */
	while (pointeur->ligne1[0][0] != 2)
	{
		barre();
		for (uint8_t i = 0; i < 7; i++)
		{
			vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
			serial_puts("     ");
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("     ");
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("     ");
			pointeur->ligne1[i][0] -= 1;
			pointeur->ligne2[i][0] -= 1;
			pointeur->ligne3[i][0] -= 1;
			sleep(4);
			vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
			serial_puts("(-o-)");
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("/-o-\\");
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("(-o-)");
		}
	}
	/* Descente des monstres*/
	if (pointeur->ligne1[0][0] == 2)
	{
		for (uint8_t i = 0; i < 7; i++)
		{
			vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
			serial_puts("     ");
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("     ");
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("     ");
			pointeur->ligne1[i][1] += 1;
			pointeur->ligne2[i][1] += 1;
			pointeur->ligne3[i][1] += 1;
			sleep(4);
			vt100_move(pointeur->ligne1[i][0], pointeur->ligne1[i][1]);
			serial_puts("(-o-)");
			vt100_move(pointeur->ligne2[i][0], pointeur->ligne2[i][1]);
			serial_puts("/-o-\\");
			vt100_move(pointeur->ligne3[i][0], pointeur->ligne3[i][1]);
			serial_puts("(-o-)");
		}
	}
}
void end_game(){
	vt100_clear_screen();
}
