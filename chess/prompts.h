#ifndef PROMPTS_H
#define PROMPTS_H

#include <iostream>
#include <string>

#include "game.h"

class Prompts {
public:
    static const char *getWhite(Player pl) {
        if (pl == WHITE)
            return "White";
        else
            return "Black";
    }

    static void menu() {
        std::cout
            << "1. Start a new game\n"
            << "2. Load a saved game\n";
    }

    static void loadGame() {
        std::cout << "Enter a file to load from:\n";
    }

    static void saveGame() {
        std::cout << "Enter a file to save to:\n";
    }

    static void loadFailure() {
        std::cout << "Failed to load the game\n";
    }

    static void saveFailure() {
        std::cout << "Failed to save the game\n";
    }

    static void playerPrompt(Player pl, int turn) {
        std::cout << getWhite(pl) << " turn " << turn << ':' << std::endl;
    }

    static void check(Player pl) {
        std::cout << getWhite(pl) << " checks!" << std::endl;
    }

    static void capture(Player pl) {
        std::cout << getWhite(pl) << " captures a piece." << std::endl;
    }

    static void checkMate(Player pl) {
        std::cout << getWhite(pl) << " checkmates!!!\n";
    }

    static void staleMate() {
        std::cout << "Stalemate!\n";
    }

    static void parseError() {
        std::cout << "Error: couldn't parse your move.\n";
    }

    static void outOfBounds() {
        std::cout << "Error: out of bounds.\n";
    }

    static void noPiece() {
        std::cout << "Error: you do not have a piece there.\n";
    }

    static void blocked() {
        std::cout << "Error: blocked by another piece.\n";
    }

    static void cantCastle() {
        std::cout << "Error: castling cannot be performed.\n";
    }

    static void mustHandleCheck() {
        std::cout << "Error: your king is checked.\n";
    }

    static void cantExposeCheck() {
        std::cout << "Error: you cannot expose your king to a check.\n";
    }


    static void illegalMove() {
        std::cout << "Error: illegal move.\n";
    }

    static void win(Player pl, int turn) {
        std::cout << getWhite(pl) << " wins on turn " << turn << '\n';
    }

    static void gameOver() {
        std::cout << "Game over. Goodbye!\n";
    }

};

#endif
