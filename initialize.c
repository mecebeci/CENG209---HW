#include "initialize.h"

Enemy* enemy_array;
Talisman* talisman_array;
Item* item_array;
Room* room_array;


/*
 * Function: load_enemies_from_csv
 * -------------------------------------------------------------
 * Reads enemy data from a CSV file and loads it into the program.
 */
void load_enemies_from_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file\n");
        return;
    }

    char line[256];

    // Allocate memory for the global enemy array
    enemy_array = (Enemy*)malloc(ENEMY_NUMBER * sizeof(Enemy));
    if (!enemy_array) {
        printf("Memory allocation failed\n");
        fclose(file);
        return;
    }
    // Skip header line
    fgets(line, sizeof(line), file);

    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        // Parse enemy name
        enemy_array[i].enemy_name = strdup(token);
        // Parse health
        token = strtok(NULL, ",");
        enemy_array[i].health = atoi(token);
        // Parse power
        token = strtok(NULL, ",");
        enemy_array[i].power = atoi(token);
        // Parse stamina
        token = strtok(NULL, ",");
        enemy_array[i].stamina = atoi(token);

        i++;
    }

    fclose(file);
}


/*
 * Function: find_enemy_by_name
 * -------------------------------------------------------------
 * Searches for an enemy by name in the array of loaded enemies.
 * Returns a pointer to the enemy if found, or NULL if no match is found.
 */
Enemy* find_enemy_by_name(const char* name){
    if(!name) return NULL;
    for(int i = 0;i < ENEMY_NUMBER; i++){
        if(strcmp(enemy_array[i].enemy_name, name) == 0){
            return &enemy_array[i];
        }
    }
    return NULL;
}


/*
 * Function: load_talismans_from_csv
 * -------------------------------------------------------------
 * Reads talisman data from a CSV file and loads it into the program.
 */
void load_talismans_from_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file\n");
        return;
    }

    char line[256];

    fgets(line, sizeof(line), file);

    // Allocate memory for the global talisman array
    talisman_array = (Talisman*)malloc(TALISMAN_NUMBER * sizeof(Talisman));
    if (!talisman_array) {
        printf("Memory allocation failed\n");
        fclose(file);
        return;
    }

    int i = 0;
    while (fgets(line, sizeof(line), file) && i < TALISMAN_NUMBER) {
        // Parse name
        char* token = strtok(line, ",");
        talisman_array[i].talisman_name = strdup(token);

        // Parse effect type
        token = strtok(NULL, ",");
        if (strcmp(token, "STAMINA") == 0) {
            talisman_array[i].effect_type = STAMINA;
        } else if (strcmp(token, "POWER") == 0) {
            talisman_array[i].effect_type = POWER;
        } else if (strcmp(token, "CRIT_VALUE") == 0) {
            talisman_array[i].effect_type = CRIT_VALUE;
        }
        // Parse value
        token = strtok(NULL, ",");
        talisman_array[i].value = atoi(token);

        i++;
    }

    fclose(file);
}

/*
 * Function: find_talisman_by_name
 * -------------------------------------------------------------
 * Searches for an talisman by name in the array of loaded talismans.
 * Returns a pointer to the talisman if found, or NULL if no match is found.
 */
Talisman* find_talisman_by_name(const char* name){
    if(!name) return NULL;
    for(int i = 0;i < TALISMAN_NUMBER; i++){
        if(strcmp(talisman_array[i].talisman_name, name) == 0){
            return &talisman_array[i];
        }
    }
    return NULL;
}

/*
 * Function: load_items_from_csv
 * -------------------------------------------------------------
 * Reads item data from a CSV file and loads it into the program.
 */
void load_items_from_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file\n");
        return;
    }

    char line[256];

    // Allocate memory for the global item array
    item_array = (Item*)malloc(ITEM_NUMBER * sizeof(Item));
    if (!item_array) {
        printf("Memory allocation failed\n");
        fclose(file);
        return;
    }
    // Skip header line
    fgets(line, sizeof(line), file);

    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        // Parse name
        char* token = strtok(line, ",");
        item_array[i].item_name = strdup(token);

        // Parse effect type
        token = strtok(NULL, ",");
        if (strcmp(token, "STAMINA") == 0) {
            item_array[i].effect_type = STAMINA;
        } else if (strcmp(token, "POWER") == 0) {
            item_array[i].effect_type = POWER;
        } else if (strcmp(token, "CRIT_VALUE") == 0) {
            item_array[i].effect_type = CRIT_VALUE;
        }
        // Parse power
        token = strtok(NULL, ",");
        item_array[i].value = atoi(token);
        // Parse stamina
        token = strtok(NULL, ",");
        item_array[i].room_name = strdup(token);

        i++;
    }

    fclose(file);
}

/*
 * Function: find_item_by_name
 * -------------------------------------------------------------
 * Searches for an item by name in the array of loaded items.
 * Returns a pointer to the item if found, or NULL if no match is found.
 */
Item* find_item_by_name(const char* name){
    if(!name) return NULL;
    for(int i = 0;i < ITEM_NUMBER; i++){
        if(strcmp(item_array[i].item_name, name) == 0){
            return &item_array[i];
        }
    }
    return NULL;
}

/*
 * Function: fill_the_room_array
 * -------------------------------------------------------------
 * Populates an array of Room structures with specified data.
 * Each room is assigned a name, description, associated enemy, talisman, item, password, and location within the game world.
 */
void fill_the_room_array(Room* room_array, char* name, char* description, Enemy* enemy,
                         Talisman* talisman, Item* item, char* password, int location)
{
    room_array[location].room_name = strdup(name);
    room_array[location].room_description = strdup(description);
    room_array[location].enemy = enemy;
    room_array[location].talisman = talisman;
    room_array[location].item = item;
    room_array[location].password = strdup(password);

}

/*
 * Function: load_rooms_from_csv
 * -------------------------------------------------------------
 * Reads room data from a CSV file and loads it into the program.
 */
void load_rooms_from_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[1024];

    room_array = (Room*)malloc(ROOM_NUMBER * sizeof(Room));
    if (!room_array) {
        fprintf(stderr, "Memory allocation failed for room_array\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (!room_array) {
        printf("Memory allocation failed\n");
        fclose(file);
        return;
    }
    // Skip the header line
    fgets(line, sizeof(line), file);
    int i = 0;
    while (fgets(line, sizeof(line), file) && i < ROOM_NUMBER) {
        char room_name[100] = "";
        char room_description[256] = "";
        char enemy_name[100] = "";
        char talisman_name[100] = "";
        char item_name[100] = "";
        char password[100] = "";

        // Parse name
        char* token = strtok(line, ",");
        if (token != NULL) {
            strcpy(room_name, token);
        }
        // Parse description
        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(room_description, token);
        }
        // Parse enemy name
        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(enemy_name, token);
        }
        // Parse talisman name
        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(talisman_name, token);
        }
        // Parse item name
        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(item_name, token);
        }
        // Parse password
        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(password, token);
        }
        // Fill the room array. We use find_*** functions for this because we only have enemy name, talisman name, item name at this point.
        fill_the_room_array(room_array, room_name, room_description, find_enemy_by_name(enemy_name),
                            find_talisman_by_name(talisman_name), find_item_by_name(item_name), password, i);
        i++;
    }
    fclose(file);
}
