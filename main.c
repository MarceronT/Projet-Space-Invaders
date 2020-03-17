int main(void)
{
	uint8_t boucle = 0;
	uint8_t boucle_2 = 0;
	/**
	 *  IMPORTANT NOTE!
	 *  The symbol VECT_TAB_SRAM needs to be defined when building the project
	 *  if code has been located to RAM and interrupts are used.
	 *  Otherwise the interrupt table located in flash will be used.
	 *  See also the <system_*.c> file and how the SystemInit() function updates
	 *  SCB->VTOR register.
	 *  E.g.  SCB->VTOR = 0x20000000;
	 */

	/* TODO - Add your application code here */
	debut:
	serial_init(115200);
	fenetre(42, 124);
	menu();
	while (serial_get_last_char() == -1)
	{
	}
	fenetre(94, 124);
	ini_monstres();
	ini_blocs();
	ini_barre();
	ini_tir();
	ini_life();

	/* Initialize LEDs */
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);

	/* Turn on LEDs */
	STM_EVAL_LEDOn(LED3);
	STM_EVAL_LEDOn(LED4);
	STM_EVAL_LEDOn(LED5);
	STM_EVAL_LEDOn(LED6);

	/* Infinite loop */
	while (1)
	{
		if (boucle == 150){
			all_monster();
		}
		if (boucle_2 == 200){
			tir_all_monster(3);
			tir_all_monster(2);
			tir_all_monster(6);
			tir_all_monster(5);
			tir_all_monster(4);
			tir_all_monster(1);
		}
		barre();
		boucle = (boucle+1)%151;
		boucle_2 = (boucle_2+1)%201;
		if (life() == 0 || end() == 21){
			goto debut;
		}
	}
}
