# Dungeon Exploration Game

This is a **Dungeon Exploration Game** developed in **C**. The player explores a rooms, encounters enemies, collects treasures, and uses talismans to increase stats. The final challenge is to defeat the **boss enemy, Erreth-Akbe**, and escape the dungeon.

## Gameplay Overview

The player explores rooms by moving **forward** or **backward**. Except for the final boss room, each enemy room is followed by a treasure room and a talisman room. In the enemy room, the player defeats the enemy to obtain the code for the treasure room. In the treasure room, the player can pick up items, and in the talisman room, they can use the acquired talisman. This allows the player to increase their stats, gradually strengthening their character to face increasingly powerful enemies.

## Key Features

- **Effect Type**: The player benefits from items and talismans based on their **effect type**.
- **Save/Load Mechanism**: The game allows saving and loading the game state within a session.
- **Battle System**: The player engages in turn-based combat with enemies.

## Architecture Overview

The program's structure was redesigned **three times** during development. Initially, I tried using a **doubly linked list**, but managing additional nodes proved difficult. I then moved to a **two-dimensional array**, but faced many **segmentation faults**. Finally, I settled on a **one-dimensional array** structure, which resolved most issues.

## Modules/Components

The project contains the following modules:

- **mechanics.c**: Handles the game mechanics, including combat and item usage.
- **initialize.c**: Initializes the game with starting values and functions to retrieve data from CSV files.
- **main.c**: Manages the overall program flow and integrates the other modules.

## Implementation

The game was developed in **C** as specified in the project requirements. The biggest challenge I faced was managing **segmentation faults**, which became increasingly difficult to resolve as the project grew. A notable issue that I couldn't fix was with the **save/load** mechanism, where it works correctly during a session but causes a **segmentation fault** after restarting the game.

Despite this, I managed to resolve most other bugs and improve the game's functionality.
