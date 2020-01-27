	serial_init(115200);
	fenetre (42,124);
	menu();
	while (serial_get_last_char() == -1)
	{
	}
	fenetre(94,124);
	monstres();
