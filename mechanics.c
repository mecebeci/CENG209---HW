#include "mechanics.h"

extern Enemy* enemy_array;
extern Talisman* talisman_array;
extern Item* item_array;
extern Room* room_array;
bool final_flag;
bool load_error_flag = false;
bool lose_flag = false;
extern char* trim(char* str);

/*
 * Function: move
 * -------------------------------------------------------------
 * Moves the player to a new location based on their choice.
 */
void move(Player* player, char choice){
     // Check if the player chooses to move forward ('f')
    if(choice == 'f'){
        // Ensure the player is not already at the last room
        if(player->current_location < ROOM_NUMBER - 1){
            player->current_location++;
        }
    }
    // Check if the player chooses to move backward ('b')
    else if(choice == 'b'){
        // Ensure the player is not already at the first room
        if(player->current_location > 0){
            player->current_location--;
        }
    }
    else{
        printf("Invalid move\n");
    }
}

/*
 * Function: look
 * -------------------------------------------------------------
 * Displays room description
 */
void look(Player* player){
    printf("%s\n", room_array[player->current_location].room_description);
}

/*
 * Function: list
 * -------------------------------------------------------------
 * Displays the player's inventory.
 */
void list(Player* player){
    if(player->inventory.size == 0){
        printf("Player doesn't have anything...\n");
        return;
    }

    Item temp_item;
    for(int i = 0; i < player->inventory.size; i++){
        temp_item = player->inventory.items[i];
        printf("=============================\n");
        printf("%d. item: %s\n", (i + 1), temp_item.item_name);

        if(temp_item.effect_type == POWER){
            printf("Item effect type: POWER\n");
        }else{
            printf("Item effect type: STAMINA\n");
        }

        printf("Item value: %d\n", temp_item.value);
    }
}


/*
 * Function: stats
 * -------------------------------------------------------------
 * Displays the player's stats
 */
void stats(Player* player){
    printf("Player name: %s\n", player->player_name);
    printf("Player health: %d\n", player->health);
    printf("Player power: %d\n", player->power);
    printf("Player stamina: %d\n", player->stamina);
    printf("Player critic value: %d\n", player->crit_value);

}

/*
 * Function: pickup_item
 * -------------------------------------------------------------
 * Adds an item from the current location to the player's inventory.
 */
void pickup_item(Player* player){
    // Check if the current room contains an item.
    if(room_array[player->current_location].item == NULL){
        printf("There's nothing you can get here...\n");
        return;
    }
    // Check if player's inventory capacity is full
    if(player->inventory.capacity == player->inventory.size){
        printf("Capacity is full!\n");
        return;
    }

    // Retrieve the item from the current room and remove it from the room.
    Item *new_item = room_array[player->current_location].item;
    room_array[player->current_location].item = NULL;

    // Add the item to the player's inventory.
    player->inventory.items[player->inventory.size++] =*new_item;
    printf("Item has just pickuped! %s\n", new_item->item_name);
    printf("Inventory size is: %d\n", player->inventory.size);

    // Apply item effects to the player.
    if(new_item->effect_type == POWER){
        int impact = new_item->value;
        printf("Player old power stat: %d\n", player->power);
        player->power += impact;
        printf("Player new power stat: %d\n", player->power);
    }
    else if(new_item->effect_type == STAMINA){
        int impact = new_item->value;
        printf("Player old stamina stat: %d\n", player->stamina);
        player->stamina += impact;
        printf("Player new stamina stat: %d\n", player->stamina);
    }
}
/*
 * Function: attack
 * -------------------------------------------------------------
 * Simulates an attack by the player on an enemy or vice versa.
 * The function calculates the attack's impact on the enemy based on the player's stats and updates the enemy's health and vice versa.
 */
void attack(Player* player, Enemy* enemy, bool attack_flag){
    // Initialize variables to store damage and total damage
    int damage = 0;
    int total_damage = 0;
    // if attack_flag is true, then player attacks to enemy
    if(attack_flag){
        // Seed the random number generator
        srand(time(NULL));
        // Generate a random value to determine critical hit
        int random_value = rand() % 100;
        // Base damage is the player's power
        damage = player->power;

        // Check if the random value is less than the player's critical hit chance
        if(random_value < player->crit_value){
            // Double the damage for a critical hit
            damage *= 2;
            // Subtract enemy's stamina from damage
            total_damage = damage - enemy->stamina;
            printf(total_damage > 0
                ? "Critical hit! Damage: %d\n"
                : "Miss!\n",
                (total_damage));

        }else{
            // Calculate normal damage
            total_damage = damage - enemy->stamina;
            printf(total_damage > 0
                ? "Damage: %d\n"
                : "Miss!\n",
                (total_damage));
        }
        // Apply damage to the enemy's health, ensuring it doesn't go below 0
        enemy->health -= (total_damage > 0 ? total_damage : 0);
    }
         // If attack_flag is false, the enemy attacks the player
    else{
        // Base damage is the enemy's power
        damage = enemy->power;
        // Subtract player's stamina from damage
        total_damage = damage - player->stamina;
        printf(total_damage > 0
            ? "Damage: %d\n"
            : "Miss!\n",
            (total_damage));
        // Apply damage to the player's health, ensuring it doesn't go below 0
        player->health -= (total_damage > 0 ? total_damage : 0);
    }
}

 /*
 * Function: attack_session
 * -------------------------------------------------------------
 * Simulates a turn-based battle session between the player and an enemy.
 * During each turn, both the player and the enemy will attack each other.
 */
void attack_session(Player* player, Enemy* enemy){
        printf("==============================\n");
        printf("Enemy health: %d\n", enemy->health);
        printf("Enemy power: %d\n", enemy->power);
        printf("Enemy stamina: %d\n", enemy->stamina);

    // Battle continues while both player and enemy have health greater than zero
    while(player->health > 0 && enemy->health > 0){
        printf("==============================\n");
        sleep(1);
        printf("Your turn!\n");
        // wait 0.75 second
        usleep(750000);
        // Player's turn to attack
        attack(player, enemy, 1);
        printf("Enemy health: %d\n", enemy->health);
        // If the enemy's health reaches zero or less, the enemy is defeated
        if(enemy->health <= 0){
            enemy-> health = 0;
            printf("%s is defated!\n", enemy->enemy_name);
            // Special case: If the defeated enemy is "Erreth-Akbe", end the game or trigger final event
            if(strcmp(trim(enemy->enemy_name), "Erreth-Akbe") == 0){
                room_array[player->current_location].enemy = NULL;
                final_flag = true;
                return;
            }
            // General case: Update room description to reflect enemy defeat
            strcpy(room_array[player->current_location].room_description, "You defeated the enemy. You can move forward or backward...\n");
            room_array[player->current_location].enemy = NULL;
            // Check if there is another room after the current one
            if(&room_array[player->current_location + 1] != NULL){
                // If there is another room, print its password
                printf("%s's password is: %s\n", room_array[player->current_location + 1].room_name, room_array[player->current_location + 1].password);
            }
            return;
        }
        usleep(750000);
        printf("==============================\n");
        printf("Enemy turn!\n");
        usleep(750000);
        // Enemy attacks with attack_flag set to 0 (enemy's attack)
        attack(player, enemy, 0);
        printf("Your health: %d\n", player->health);
        // If the player's health reaches zero or less, the player is defeated
        if(player->health <= 0){
            player->health = 0;
            printf("You are defated...\n");
            lose_flag = true;
            return;

        }
        usleep(750000);
    }
    return;
}

 /*
 * Function: save_game
 * -------------------------------------------------------------
 * Saves the current game state to a specified file.
 */
void save_game(Game_state *game_state, const char *file_name) {
    FILE *file = fopen(file_name, "wb");
    if (file == NULL) {
        printf("Cannot open file for saving: %s\n", file_name);
        return;
    }

    // Save base structure
    fwrite(game_state, sizeof(Game_state), 1, file);

    // Save player data
    if (game_state->player != NULL) {
        fwrite(game_state->player, sizeof(Player), 1, file);
    }

    // Save room array
    if (game_state->room_array != NULL) {
        fwrite(game_state->room_array, sizeof(Room), ROOM_NUMBER, file);
    }

    fclose(file);
    printf("Game saved: %s\n", file_name);
}


 /*
 * Function: load_game
 * -------------------------------------------------------------
 * Loads the game state from a specified file and restores the game to its previous state.
 */
void load_game(Game_state *game_state, const char *file_name) {
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        printf("Cannot open file for loading: %s\n", file_name);
        load_error_flag = true;
        return;
    }

    // Load base structure
    fread(game_state, sizeof(Game_state), 1, file);

    // Allocate and load player data
    if (game_state->player == NULL) {
        game_state->player = malloc(sizeof(Player));
        if (game_state->player == NULL) {
            printf("Failed to allocate memory for player.\n");
            fclose(file);
            load_error_flag = true;
            return;
        }
    }
    fread(game_state->player, sizeof(Player), 1, file);

    // Allocate and load room array
    if (game_state->room_array == NULL) {
        game_state->room_array = malloc(sizeof(Room) * ROOM_NUMBER);
        if (game_state->room_array == NULL) {
            printf("Failed to allocate memory for room array.\n");
            fclose(file);
            load_error_flag = true;
            return;
        }
    }
    fread(game_state->room_array, sizeof(Room), ROOM_NUMBER, file);

    fclose(file);
    printf("Game loaded: %s\n", file_name);
    load_error_flag = false;
}



