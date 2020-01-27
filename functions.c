#include "functions.h"
#include "serial.h"
#include "vt100.h"

void fenetre (uint8_t character_h, uint8_t character_v){
	vt100_clear_screen();
	unsigned char index;
	for (index=1; index<80; index++)
	{
		vt100_move(index,1);
		serial_putchar(character_h);
		vt100_move(index,24);
		serial_putchar(character_h);
		vt100_move(1,index);
		serial_putchar(character_v);
		vt100_move(80,index);
		serial_putchar(character_v);
	}
}

void menu(){
	/* Encadrage */
	unsigned char index;
	for (index=33; index<49; index++){
		vt100_move(index,5);
		serial_putchar(42);
		vt100_move(index,7);
		serial_putchar(42);
	}
	vt100_move(33,6);
	serial_putchar(124);
	vt100_move(48,6);
	serial_putchar(124);

	/* Ecriture du menu */
	vt100_move(34,6);
	serial_puts("SPACE INVADERS");
	vt100_move(34,14);
	serial_puts("Press any touch");
	vt100_move(39,12);
	serial_puts("PLAY");
	vt100_move(36,20);
	serial_puts("By Thomas");
}
void monstres(){

}
/*
void monstres(){
	unsigned char i;
	for (i=2; i<78; i+=10){
		vt100_move(i,2);
		serial_puts("{@@}");
		vt100_move(i,3);
		serial_puts("/\"\"\\");
	}
	for (i=5; i<80; i+=10){
		vt100_move(i,5);
		serial_puts("dOOb");
		vt100_move(i,6);
		serial_puts("^/\\^");
	}
} */
