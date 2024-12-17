#include "initialize.h"
#include "mechanics.h"

extern Enemy* enemy_array;
extern Talisman* talisman_array;
extern Item* item_array;
extern Room* room_array;
extern bool final_flag; // if Erreth-Akbe is defeated, this will be true
extern bool load_error_flag; // if there is no file, it will be false otherwise true.
extern bool lose_flag; //if player is defeated, this will be true
bool talisman_flag = false;
bool load_flag = false;
Game_state game;




/*
 * Function: end
 * -------------------------------------------------------------
 * This function displays the contents of an ASCII art file named "ascii_end.txt" when the game ends.
 */
void end(){
    FILE *file = fopen("ascii_end.txt", "r");
    if (file == NULL) {
        printf("File couldn't be open...\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

/*
 * Function: trim
 * -------------------------------------------------------------
 * This function removes any leading and trailing whitespace characters
 */
char* trim(char* str) {
    char* end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // All spaces?
    if (*str == 0)
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

int main() {
    load_enemies_from_csv("enemies.csv");
    load_talismans_from_csv("talisman.csv");
    load_items_from_csv("items.csv");
    load_rooms_from_csv("room.csv");

    FILE *file = fopen("ascii_start.txt", "r");
    if (file == NULL) {
        printf("File couldn't be open...\n");
        return 0;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);

    char outer_choice;
    Player *player;
    do{
        // if load_flag is false, then we need to start new game
        if(load_flag == false){
            printf("==============================\n");
            printf("\n1. New game\n");
            printf("\n2. Load the game\n");
            printf("\nq. Exit\n");
            printf("\nPlease make a choice: ");
            scanf(" %c", &outer_choice);
        }

        if (outer_choice == '1') {
            // if load flag is false then we need to initalize our character -> Player
            if(load_flag == false){
                printf("Please enter a character name: ");
                char* name = (char*)malloc(20 * sizeof(char));
                if (name == NULL) {
                    printf("Memory allocation for name failed!\n");
                    return 1;
                }
                scanf("%19s", name);

                player = (Player*)malloc(sizeof(Player));

                player->player_name = (char*)malloc(20 * sizeof(char));
                if (player->player_name == NULL) {
                    printf("Memory allocation for player name failed!\n");
                    free(name);  // Free the allocated memory for name
                    return 1;
                }
                strcpy(player->player_name, name);

                player->health = 1500;
                player->power = 120;
                player->stamina = 40;
                player->crit_value = 0;

                player->inventory.capacity = 2;
                player->inventory.size = 0;
                player->inventory.items = (Item*)malloc(player->inventory.capacity * sizeof(Item));
                if (player->inventory.items == NULL) {
                    printf("Memory allocation for inventory items failed!\n");
                    return 1;
                }
                // Initialize if necessary or set to NULL
                player->inventory.talisman;
                player->current_location = 0;
                printf("Character is generated...\n");
            }

            char inner_choice = '\0';
            while ((inner_choice = getchar()) != '\n' && inner_choice != EOF);

            do{
                sleep(1);
                printf("=================================================\n");
                printf("\nPlease make a choice:\n l. Look\n i. Look the inventory\n k. Stats\n u. Use talisman\n p. Pickup item\n a. Attack\n m. Move\n s. Save\n t. Main Menu\n");

                scanf(" %c", &inner_choice);

                switch(inner_choice){

                    case 'l':
                        look(player);
                        break;

                    case 'i':
                        list(player);
                        break;

                    case 'k':
                        stats(player);
                        break;

                    case 'a':
                        if(room_array[player->current_location].enemy == NULL){
                            printf("There is no enemy...\n");
                        }
                        else{
                            attack_session(player, room_array[player->current_location].enemy);
                            if(final_flag == true){
                                end();
                                sleep(5);
                                return 0;
                            }
                            if(lose_flag == true){
                                return 0;
                            }
                        }
                        break;

                    case 'm':
                        char direction;
                        printf("Please enter a direction: 'f' for forward and 'b' for backward: ");
                        scanf(" %c", &direction);
                        if(direction == 'f'){
                            if(player->current_location < ROOM_NUMBER - 1 && room_array[player->current_location].enemy == NULL){
                                move(player, direction);
                                // if room contains a password, player should enter this
                                if(strcmp(trim(room_array[player->current_location].password), "NULL") != 0){
                                    printf("Please enter password: ");
                                    char password_trial[20];
                                    scanf("%19s", password_trial);
                                    if(strcmp(trim(room_array[player->current_location].password), password_trial) == 0){
                                        printf("You're in the next room...\n");
                                    }
                                    else{
                                        printf("You entered the password incorrectly. You will be redirected to the previous room...\n");
                                        move(player, 'b');
                                    }
                                }
                            }
                            // if there is a enemy, player couldn't move another room.
                            else if(room_array[player->current_location].enemy != NULL){
                                printf("You couldn't move another room until you defeat enemy!\n");
                            }
                            printf("Current location: %s\n", room_array[player->current_location].room_name);
                        }
                        else if(direction == 'b'){
                            if(player->current_location > 0){
                                move(player, direction);
                                printf("You have moved back to the previous room...\n");
                            }
                            else{
                                printf("You couldn't move back. You are already in starting point...\n");
                            }
                            printf("Current location: %s\n", room_array[player->current_location].room_name);
                        }
                        break;


                    case 'p':
                        if(room_array[player->current_location].item == NULL){
                            printf("There is nothing to pick up here...\n");
                        }
                        else{
                            pickup_item(player);
                            player->inventory.talisman = *room_array[player->current_location].talisman;
                            printf("You also take %s\n", player->inventory.talisman.talisman_name);
                            talisman_flag = true;
                        }
                        break;


                    case 'u':
                        // Player can use talisman only if he/she is in the talisman room.
                        if(talisman_flag && strncmp(trim(room_array[player->current_location].room_name), "Talis", 5) == 0){
                            char talisman_choice;
                            printf("You are about to use the %s. Are you sure? (y/n): ", player->inventory.talisman.talisman_name);
                            scanf(" %c", &talisman_choice);
                            if(talisman_choice == 'y'){
                                sleep(1);
                                printf("The operation was completed successfully!\n");
                                if(player->inventory.talisman.effect_type == POWER){
                                    player->power += player->inventory.talisman.value;
                                    printf("Your power has increased by %d.\n", player->inventory.talisman.value);
                                    Talisman empty_talisman = {0};
                                    player->inventory.talisman = empty_talisman;
                                }
                                else if(player->inventory.talisman.effect_type == CRIT_VALUE){
                                    player->crit_value += player->inventory.talisman.value;
                                    printf("Your critic value has increased by %d.\n", player->inventory.talisman.value);
                                    Talisman empty_talisman = {0};
                                    player->inventory.talisman = empty_talisman;
                                }
                                talisman_flag = false;
                            }
                            else{
                                printf("You are being returned to the menu...\n");
                            }
                        }
                        else{
                            printf(talisman_flag
                            ? "You are currently not in the Talisman room...\n"
                            : "You don't have talisman...\n"
                            );

                        }

                        break;


                    case 's':
                        game.player = player;
                        game.room_array = room_array;
                        save_game(&game, "save.dat");
                        break;

                    case 't':
                        printf("You are back in main menu...\n");
                        continue;


                    default:
                        printf("Invalid input!\n");
                        break;

                }

            }while(inner_choice != 't');

        }
        else if (outer_choice == '2') {
                load_game(&game, "save.dat");
                if(load_error_flag == false){
                    player = game.player;
                    room_array = game.room_array;
                    // we initialize load_flag true so that, when we start a game, initialization process is skipped.
                    load_flag = true;
                    outer_choice = '1';
                    continue;
                }
                else{
                    printf("There is no such file. You are bring back to the menu...\n");
                    continue;
                }
        }
        else{
            if(outer_choice != 'q'){
                printf("Invalid input!\n");
            }
        }
    }while(outer_choice != 'q');

    free(enemy_array);
    free(talisman_array);
    free(item_array);
    free(room_array);
    return 0;
}
