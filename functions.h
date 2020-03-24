#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <stdint.h>

void window(uint8_t character_h,uint8_t character_v);
void menu(void);
void ini_life(void);
void ini_ship(void);
void ini_shot(void);
void ini_shot_monster(void);
void ini_monster(void);
void ini_blocs(void);
uint8_t life(void);
void score(void);
void sleep(unsigned long n);
void ship(void);
void shot(void);
void hit_box(void);
void all_monster(void);
void shot_one_monster(uint8_t num_monstre,uint8_t num_lignes);
void shot_all_monster(uint8_t num_monstre);
void hit_box_monster(void);
uint8_t end (void);
uint8_t end_2 (void);

#endif /* FUNCTIONS_H_ */
