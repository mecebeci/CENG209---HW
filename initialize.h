#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

#define ROOM_NUMBER 8
#define ENEMY_NUMBER 3
#define TALISMAN_NUMBER 2
#define ITEM_NUMBER 2

typedef enum{
    STAMINA, POWER, CRIT_VALUE
} Effect_type;

typedef struct{
    char* talisman_name;
    Effect_type effect_type;
    int value;
} Talisman;

typedef struct {
    char* item_name;
    Effect_type effect_type;
    int value;
    char* room_name;
} Item;

typedef struct {
    Item* items;
    Talisman talisman;
    int size;
    int capacity;
} Inventory;

typedef struct {
    char* player_name;
    int health;
    int power;
    int stamina;
    int crit_value;
    Inventory inventory;
    int current_location;
} Player;

typedef struct {
    char* enemy_name;
    int health;
    int power;
    int stamina;
} Enemy;

typedef struct{
    char* room_name;
    char* room_description;
    Enemy* enemy;
    Talisman* talisman;
    Item* item;
    char* password;
    int location;
} Room;

typedef struct{
    Player *player;
    Room* room_array;
}Game_state;

void load_enemies_from_csv(const char* filename);
Enemy* find_enemy_by_name(const char* name);
void load_talismans_from_csv(const char* filename);
Talisman* find_talisman_by_name(const char* name);
void load_items_from_csv(const char* filename);
Item* find_item_by_name(const char* name);
void fill_the_room_array(Room* room_array, char* name, char* description, Enemy* enemy,
                         Talisman* talisman, Item* item, char* password, int location);
void load_rooms_from_csv(const char* filename);

#endif // INITIALIZE_H
