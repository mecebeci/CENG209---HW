#ifndef MECHANICS_H
#define MECHANICS_H

#include "initialize.h"

void move(Player* player, char choice);
void look(Player* player);
void list(Player* player);
void stats(Player* player);
void pickup_item(Player* player);
void attack(Player* player, Enemy* enemy, bool attack_flag);
void attack_session(Player* player, Enemy* enemy);
void save_game(Game_state* game_state, const char* file_name);
void load_game(Game_state *game_state, const char *file_name);

#endif
