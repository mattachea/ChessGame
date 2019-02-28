#undef FOR_RELEASE
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <ctype.h>
#include "prompts.h"
#include "game.h"
#include "chess.h"
#include "terminal.h"
using std::cin;
using std::string;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::vector;

// run handles all user input and organizes over game structure
int ChessGame::run() {
    Player p1 = WHITE;
    Player p2 = BLACK;
    string input1 = " "; // user input
    string input2 = " "; // user input
    Position start;
    Position end;
    string sFile = ""; // name of file to be saved
    char choice = ' '; // user input
    char in10 = ' ';
    char in11 = ' ';
    char in20 = ' ';
    char in21 = ' ';
    int boardStat = 0; // toggle on or off
    string lFile = " "; // save of file to load
    int code = 0; // ENUM from makeMove
    int cOrS = 0; // checkmate, stalemate, or neither

    // print out menu until a valid choice is made
    do {
        Prompts::menu();
        cin >> choice;
    }
    while (choice != '1' && choice != '2');

    // new game
    if (choice == '1') {
        this->setupBoard();
    }

    // load game - error if loading fails
    else {
        Prompts::loadGame();
        cin >> lFile;
        if (load(lFile) < 0) {
            printMsg(-10); // load error, then quit
            return -1;
        }
    }

    // playing the game
    do {
        // by default, board is off
        if(boardStat % 2) {
            ChessGame::printBoard();
        }

        // odd # = white turn, even # = black turn
        if (this->m_turn % 2) {
            Prompts::playerPrompt(p1, this->m_turn);
        }

        else {
            Prompts::playerPrompt(p2, this->m_turn);
        }

        // read in user input
        cin >> input1;

        in10 =  tolower(input1.at(0));

        // no quit
        if(in10 != 'q') {
            // if there is a second character
            if (input1.length() > 1) {
                in11 = tolower(input1.at(1));
            }

            // save, case insensitive
            if (input1.length() == 4 && in10 == 's' && in11 == 'a' && tolower(input1.at(2)) == 'v' && tolower(input1.at(3)) == 'e') {
                Prompts::saveGame();
                cin >> sFile;
                if (save(sFile) < 0 ) {
                    printMsg(-9); // save error
                }
            }

            // toggle board
            else if (input1.length() == 5 && in10 == 'b' && in11 == 'o' && tolower(input1.at(2)) == 'a' && tolower(input1.at(3)) == 'r' && tolower(input1.at(4)) == 'd') {
                boardStat++;
            }

            // forfeit
            else if (input1.length() == 7 && in10 == 'f' && in11 == 'o' && tolower(input1.at(2)) == 'r' && tolower(input1.at(3)) == 'f' && tolower(input1.at(4)) == 'e'
                     && tolower(input1.at(5)) == 'i' && tolower(input1.at(6)) == 't') {
                printMsg(6);
                printMsg(7);
                return 1;
            }

            // incorrect input
            else if (!isalpha(in10) || !isdigit(in11)) {
                printMsg(-8);
            }

            // valid first input - check second
            else {
                cin >> input2;
                in20 = tolower(input2.at(0));
                in21 = tolower(input2.at(1));

                // if input is in correct format
                if (isalpha(in20) && isdigit(in21)) {
                    start.x = in10 - 97;
                    start.y = in11 - '0' - 1;
                    end.x = in20 - 97;
                    end.y = in21 - '0' - 1;

                    code = makeMove(start, end); // try to make move, move if possible

                    cOrS = checkOrStale(); // did the move result in a checkmate or stalemate?

                    //no, game continues
                    if(cOrS != MOVE_STALEMATE && cOrS != MOVE_CHECKMATE) {
                        printMsg(code);
                        // only increment turn if move is successfully made
                        if (code > 0) {
                            this->m_turn++;
                        }
                    }

                    else if (cOrS == MOVE_STALEMATE) {
                        Prompts::staleMate();
                    }

                    else if (cOrS == MOVE_CHECKMATE) {
                        Prompts::checkMate(playerTurn());
                        Prompts::win(playerTurn(), m_turn);
                    }
                }

                // second input is in incorrect form
                else {
                    printMsg(-8);
                }
            }
        }

        // is input1 = "q"
        else {
            input1.at(0) = tolower(input1.at(0));
        }
    }
    while (input1 != "q" && cOrS != MOVE_STALEMATE && cOrS != MOVE_CHECKMATE);

    Prompts::gameOver();
    return 1;
}

// Make a move on the board. Return an int, with < 0 being failure
int ChessGame::makeMove(Position start, Position end) {
    int result = 0;
    Piece *temp = getPiece(start);
    int checkNow = 1; // is the player currently in check? > 0 if no
    int checkLater = 1; // is the player going to be in check? > 0 if no
    Piece *wasThere = getPiece(end); // what was at the end of the board
    Position rk; // where the rook is
    static int rkHasMoved; // keeps track of whether the rook has moved at all during the game
    static int kngHasMoved; // keeps track of whether the king has moved at all during the game
    unsigned int indexStart = index(start);
    unsigned int indexEnd = index(end);

    // checks out of bounds
    if (this->validPosition(start) && this->validPosition(end)) {
        // checks if piece exists
        if (temp == NULL) {
            result = MOVE_ERROR_NO_PIECE;
        }

        else {
            checkNow = this->isChecked(); // get current check status
            result = temp->validMove(start,end, *this);

            // make the move
            if(result >= 0) {
                indexStart = index(start);
                indexEnd = index(end);

                m_pieces[indexEnd] = m_pieces[indexStart];
                m_pieces[indexStart] = NULL;

                checkLater = this->isChecked(); // did the player just put itself in check?

                // player in check
                if (checkLater < 0) {
                    m_pieces[indexStart] = temp;
                    m_pieces[indexEnd] = wasThere;

                    // was not in check
                    if (checkNow > 0) {
                        result = MOVE_ERROR_CANT_EXPOSE_CHECK;
                    }

                    // was in check
                    else {
                        result = MOVE_ERROR_MUST_HANDLE_CHECK;
                    }
                }

                // player not in check
                else {
                    // if there was a capture, free the memory of the captured piece
                    if (wasThere != NULL) {
                        delete wasThere;
                    }

                    // player checls
                    if (checkLater == MOVE_CHECK) {
                        result = MOVE_CHECK;
                    }

                    // pawn reaches the end of the board and becoems a  queen
                    if (temp->id() == 0 && ((end.y == 7 && temp->owner() == WHITE) ||
                                            (end.y == 0 && temp->owner() == BLACK ))) {
                        delete temp;
                        m_pieces[indexEnd] = NULL;
                        Board::initPiece(4, playerTurn(), end);
                        result = SUCCESS;
                    }

                    // king or rook has  moved (castling)
                    if (temp->id() == 1) {
                        rkHasMoved++;
                    }

                    else if (temp->id() == 5) {
                        kngHasMoved++;
                    }
                }
            }

            else if (temp->id() == 5 && kngHasMoved == 0 && rkHasMoved ==0 && end.y == start.y && abs((int) (end.x - start.x)) == 2) {
                result = castle(start, end);
            }

            else if (temp->id() == 5 && end.y == start.y && abs((int) (end.x - start.x)) == 2) {
                result = MOVE_ERROR_CANT_CASTLE;
            }

        }
    }

    return result;
}

// if the player is in check, return MOVE_ERROR_CANT_EXPOSE_CHECK
// if the player is not in check, return 1
// if the player puts the other player in check, return MOVE_CHECK
int ChessGame::isChecked() {
    int result = 1;
    Player who = playerTurn();
    Piece* temp;
    Position here;
    Position meKing; // player's king
    Position youKing; // opponent's king

    // go through vector of pieces to find the player's and the opponent's king
    for(int i = 0; i < 64; i++) {
        here.x = (i % 8);
        here.y = (i-(int) here.x) / 8;
        temp = getPiece(here);

        if (temp != NULL) {
            // me king?
            if (temp->owner() == who && temp->id() == 5) {
                meKing = here;
            }

            // you king?
            else if (temp->owner() != who && temp->id() == 5) {
                youKing = here;
            }
        }
    }
    // go through all the pieces on the board again, and see if either the player is in check
    // or if the player put the opponent in check
    for(int i = 0; i < 64; i++) {
        here.x = (i % 8);
        here.y = (i-(int) here.x) / 8;
        temp = getPiece(here);

        if (temp != NULL) {
            this->m_turn++; // temporarily increment turn - we want to see if the opponent can capture the king

            // if the opponent can capture the king
            if (temp->validMove(here, meKing, *this) == 3) {
                result = MOVE_ERROR_CANT_EXPOSE_CHECK;
                this->m_turn--; // set turn back to its originnal value
                return result;
            }

            else {
                this->m_turn--; // set turn back to its original value to see if player can capture opponent king

                if (temp->id() != 5 && temp->validMove(here, youKing, *this) == 3) {
                    result = MOVE_CHECK;
                    return result;
                }
            }
        }
    }
    return result;
}

// returns MOVE_CHECKMATE if checkmate
// returns MOVE_STALEMATE if stalemate
// returns -1 if neither
int ChessGame::checkOrStale() {
    int realTurn = this->m_turn; // original turn
    this->m_turn++; // temporarily increment turn to see if opponent can make a move without being in check
    Player who = playerTurn(); // opponent
    Piece *isHere;
    Position here;
    Position there;
    Piece *wasThere;
    Position meKing;
    Player putBackOwner;
    int putBackID = 0;
    int moveResult = 0;
    int checkStatus = isChecked();

    //loops through all the pieces on the board
    for(int i = 0; i<64; i++) {
        here.x = i % 8;
        here.y = (i- (int) here.x) / 8;
        isHere = getPiece(here);

        // opponents piece
        if(isHere != NULL && isHere->owner() == who) {
            //loops to see if another piece can move anywhere on the board to block the check
            for(int i = 0; i<64; i++) {
                there.x = i % 8;
                there.y = (i- (int) here.x) / 8;
                wasThere = getPiece(there);

                if (wasThere != NULL) {
                    putBackOwner = wasThere->owner();
                    putBackID = wasThere->id();
                }

                moveResult = makeMove(here, there);
                // if any move can be made that does not leave the king in check, it is not checkmate or stalemate
                if (moveResult >= 0) {
                    //moves the piece back and puts piece back
                    m_pieces[index(here)] = isHere;
                    m_pieces[index(there)] = NULL;

                    // if checkmate/stalemate was avoided via capturing a piece
                    if(wasThere != NULL) {
                        Board::initPiece(putBackID, putBackOwner, there);
                    }

                    this->m_turn = realTurn; // set turn back to original
                    return -1;
                }
            }
        }
    }

    this->m_turn = realTurn; // set turn back to original

    // no moves that do not result in check, but currently not in check
    if (checkStatus > 0) {
        return MOVE_STALEMATE;
    }

    return MOVE_CHECKMATE;
}

// castle, returns 1 if king can castle with rook
// returns MOVE_ERROR_CANNOT_CASTLE if cannot
int ChessGame::castle(Position start, Position end) {
    Position left; // left rook;
    Position right; // right rook
    Position temp;
    Piece *rightRk;
    Piece *leftRk;
    Piece *kng;
    int result;
    Position finalRkPos;
    Position holder;
    Piece *wasThere = NULL;

    // define where the left and right rook are
    left.x = 0;
    left.y = start.y;
    right.x = 7;
    right.y = start.y;
    holder.x = start.x;
    holder.y = start.y;

    // define pieces
    rightRk = getPiece(right);
    leftRk = getPiece(left);
    kng = getPiece(start);

    // trying to castle to the right and rightRk and kng have not moved
    if ((int)(end.x - start.x) > 0 && rightRk != NULL && rightRk->id() == 1) {
        // check that moving to each position to end does not result in check and the path is empty
        for (int i = 1; i < 3; i++ ) {
            temp.x = start.x + i;
            temp.y = start.y;

            if (getPiece(temp) == NULL) {

                m_pieces[index(holder)] = NULL;
                m_pieces[index(temp)] = kng;

                holder.x = temp.x;

                if (isChecked() > 0) {
                    result = 1;
                }
                else {
                    result = MOVE_ERROR_CANT_CASTLE;
                    break;
                }
            }

            else {
                wasThere = getPiece(temp);
                result = MOVE_ERROR_CANT_CASTLE;
                break;
            }
        }


        if (result == 1) {
            finalRkPos.x = right.x -2;
            finalRkPos.y = right.y;
            m_pieces[index(start)] = NULL;
            m_pieces[index(end)] = kng;
            m_pieces[index(finalRkPos)] =  m_pieces[index(right)];
            m_pieces[index(right)] = NULL;
        }

        else {

            m_pieces[index(start)] = kng;
            m_pieces[index(temp)] = wasThere;
        }


    }

    // trying to castle to the left and leftRk and kng have not moved
    else if ((int)(end.x - start.x) < 0 && leftRk != NULL && leftRk->id() == 1) {
        // check that moving to each position to end does not result in check and the path is empty
        for (int i = 1; i < 3; i++ ) {
            temp.x = start.x - i;
            temp.y = start.y;

            if (getPiece(temp) == NULL) {
                m_pieces[index(holder)] = NULL;
                m_pieces[index(temp)] = kng;

                holder.x = temp.x;
                holder.y = temp.y;


                if (isChecked() > 0) {
                    result = 1;
                }
                else {
                    result = MOVE_ERROR_CANT_CASTLE;
                    break;
                }
            }

            else {
                wasThere = getPiece(temp);
                result = MOVE_ERROR_CANT_CASTLE;
                break;
            }

        }

        if (result == 1) {
            finalRkPos.x = left.x +3;
            finalRkPos.y = left.y;
            m_pieces[index(start)] = NULL;
            m_pieces[index(end)] = kng;
            m_pieces[index(finalRkPos)] =  m_pieces[index(left)];
            m_pieces[index(left)] = NULL;
        }
        else {

            m_pieces[index(start)] = kng;
            m_pieces[index(temp)] = wasThere;
        }

    }
    return result;
}

int Knight::validMove(Position start, Position end, const Board& board) const {
    Player p = board.playerTurn();
    Piece* atPos;
    int result = 0;
    //cout << "Player is: " <<p << endl;

    if(board.getPiece(start)->owner() != board.playerTurn()) {
        result = MOVE_ERROR_ILLEGAL;
    }
    else if ((end.x == (start.x - 2) && end.y == (start.y - 1)) ||
             (end.x == (start.x - 2) && end.y == (start.y + 1)) ||
             (end.x == (start.x + 2) && end.y == (start.y - 1)) ||
             (end.x == (start.x + 2) && end.y == (start.y + 1)) ||
             (end.x == (start.x - 1) && end.y == (start.y - 2)) ||
             (end.x == (start.x - 1) && end.y == (start.y + 2)) ||
             (end.x == (start.x + 1) && end.y == (start.y - 2)) ||
             (end.x == (start.x + 1) && end.y == (start.y + 2))) {

        atPos = board.getPiece(end);

        // clear path
        if (atPos == NULL) {
            result = 1;
        }

        // capture
        else if (atPos->owner() != p) {
            result = MOVE_CAPTURE;
        }

        // piece is blocked
        else {
            result = MOVE_ERROR_BLOCKED;
        }
    }

    else {
        result = MOVE_ERROR_ILLEGAL;
    }

    return result;
}

int Pawn::validMove(Position start, Position end, const Board& board) const {
    int result = 0;
    Player p = board.playerTurn();
    Position temp;
    Piece* atEnd = board.getPiece(end);

    if(board.getPiece(start)->owner() != board.playerTurn()) {
        result = MOVE_ERROR_ILLEGAL;
    }

    //killing another piece diagonally
    else if (((p == WHITE && end.y == (start.y + 1)) || (p == BLACK && end.y == (start.y - 1)))  && (end.x == (start.x + 1) || end.x == (start.x -1))) {
        if (atEnd != NULL && atEnd->owner() != p)   {
            result = MOVE_CAPTURE;
        }

        else if (atEnd != NULL) {
            if(atEnd->owner() == p) {
                result = MOVE_ERROR_BLOCKED;
            }
        }
        else {
            result = MOVE_ERROR_ILLEGAL;
        }
    }

    //not killing, normal forward movement
    else if (end.x != start.x) {
        result = MOVE_ERROR_ILLEGAL;
    }

    // end position is empty
    else if (atEnd == NULL) {
        // double jump from first position
        if ((p == WHITE && start.y == 1) || (p == BLACK && start.y == 6))  {
            if ((p == WHITE && end.y == 3) || (p == BLACK && end.y == 4)) {
                temp.x = end.x;

                if (p == WHITE) {
                    temp.y = end.y - 1;
                }

                else {
                    temp.y = end.y + 1;
                }

                //checking if path is clear
                if (board.getPiece(temp) == NULL) {
                    result = 1;
                }

                else {
                    result= MOVE_ERROR_BLOCKED;
                }

            }
            else if ((p == WHITE && (end.y > 3 || end.y < 1)) || (p == BLACK && (end.y < 4 || end.y > 6))) {
                result = MOVE_ERROR_ILLEGAL;
            }
            else {
                result = 1;
            }

        }

        // if pawn is not in first position can only move one space
        else if (((p==WHITE) && ((end.y - start.y) != 1)) || ((p==BLACK) && ((start.y - end.y) != 1))) {
            result = MOVE_ERROR_ILLEGAL;
        }

        else {
            result = 1;
        }
    }

    // end position is taken
    else {
        result = MOVE_ERROR_BLOCKED;
    }

    return result;
}

int Rook::validMove(Position start, Position end, const Board& board) const {
    int result = 0;
    Position temp;
    int horizontal = abs((int) end.x - (int) start.x); // horizonal distance to move
    int vertical = abs((int) end.y - (int) start.y); // vertical distance to move
    Piece* atPos;
    Player p = board.playerTurn();

    if(board.getPiece(start)->owner() != board.playerTurn()) {
        //changed this to return
        return  MOVE_ERROR_ILLEGAL;
    }
    else if (horizontal != 0 && vertical != 0) {
        return MOVE_ERROR_ILLEGAL;
    }

    for(int i = 1; i <= horizontal; i++) {
        // left
        if (start.x > end.x) {
            temp.x = start.x - i;
            temp.y = start.y;
        }
        // right
        else {
            temp.x = start.x + i;
            temp.y = start.y;
        }

        atPos = board.getPiece(temp);

        // clear path
        if (atPos == NULL) {
            result = 1;
        }
        // capture
        else if (temp.x == end.x && temp.y == end.y && atPos->owner() != p) {
            result = MOVE_CAPTURE;
        }
        // piece is blocked
        else {
            //i changed this to return -Matt
            return MOVE_ERROR_BLOCKED;
        }
    }

    for(int i = 1; i <= vertical; i++) {
        // down
        if (start.y > end.y) {
            temp.y = start.y - i;
            temp.x = start.x;
        }
        // up
        else {
            temp.y = start.y + i;
            temp.x = start.x;
        }

        atPos = board.getPiece(temp);

        // clear path
        if(board.getPiece(temp) == NULL) {
            result = 1;
        }
        // capture
        else if (temp.x == end.x && temp.y == end.y && atPos->owner() != p) {
            result = MOVE_CAPTURE;
        }
        // piece is blocked
        else {
            //i changed this to return too -Matt
            return MOVE_ERROR_BLOCKED;
        }
    }
    return result;
}

int Bishop::validMove(Position start, Position end, const Board& board) const {
    int result = 0;
    Position temp;
    Piece* atPos;
    Player p = board.playerTurn();
    int diagonal = abs((int)start.y - (int)end.y);

    if(board.getPiece(start)->owner() != board.playerTurn()) {
        result = MOVE_ERROR_ILLEGAL;
    }
    //checking if valid end position
    else if(((end.x == start.x + diagonal) && (end.y == start.y + diagonal)) || //up and right
            ((end.x == start.x - diagonal) && (end.y == start.y + diagonal)) || //up and left
            ((end.x == start.x + diagonal) && (end.y == start.y - diagonal)) || //down and right
            ((end.x == start.x - diagonal) && (end.y == start.y - diagonal))) { //down and left

        //checking if path is clear
        for (int i = 1; i <= diagonal; i++) {

            //moving up to the right
            if (end.x > start.x && end.y > start.y) {
                temp.x = start.x + i;
                temp.y = start.y + i;
            }

            //moving up to the left
            if(end.x < start.x && end.y > start.y) {
                temp.x = start.x - i;
                temp.y = start.y + i;
            }

            //moving down to the right
            if(end.x > start.x && end.y < start.y) {
                temp.x = start.x + i;
                temp.y = start.y - i;
            }

            //moving down to the right
            if(end.x < start.x && end.y < start.y) {
                temp.x = start.x - i;
                temp.y = start.y - i;
            }

            atPos = board.getPiece(temp);

            // clear path
            if (atPos == NULL) {
                result = 1;
            }

            // capture
            else if (temp.x == end.x && temp.y == end.y && atPos->owner() != p) {
                result = MOVE_CAPTURE;
            }

            // piece is blocked
            else {
                //changed this to return
                return  MOVE_ERROR_BLOCKED;
            }
        }
    }
    else {
        result = MOVE_ERROR_ILLEGAL;
    }
    return result;
}

int Queen::validMove(Position start, Position end, const Board& board) const {
    int result = 0;
    Position temp;
    Piece* atPos;
    Player p = board.playerTurn();
    int diagonal = abs((int)start.y - (int)end.y);
    int horizontal = abs((int) end.x - (int) start.x); // horizonal distance to move
    int vertical = abs((int) end.y - (int) start.y); // vertical distance to move

    if(board.getPiece(start)->owner() != board.playerTurn()) {
        //cout << "move error queen validMove" << endl;
        result = MOVE_ERROR_ILLEGAL;
    }
    //diagonal movement    //checking if valid end position
    else if(((end.x == start.x + diagonal) && (end.y == start.y + diagonal)) || //up and right
            ((end.x == start.x - diagonal) && (end.y == start.y + diagonal)) || //up and left
            ((end.x == start.x + diagonal) && (end.y == start.y - diagonal)) || //down and right
            ((end.x == start.x - diagonal) && (end.y == start.y - diagonal))) { //down and left

        //checking if path is clear
        for (int i = 1; i <= diagonal; i++) {

            //moving up to the right
            if (end.x > start.x && end.y > start.y) {
                temp.x = start.x + i;
                temp.y = start.y + i;
            }

            //moving up to the left
            if(end.x < start.x && end.y > start.y) {
                temp.x = start.x - i;
                temp.y = start.y + i;
            }

            //moving down to the right
            if(end.x > start.x && end.y < start.y) {
                temp.x = start.x + i;
                temp.y = start.y - i;
            }

            //moving down to the right
            if(end.x < start.x && end.y < start.y) {
                temp.x = start.x - i;
                temp.y = start.y - i;
            }

            atPos = board.getPiece(temp);

            // clear path
            if (atPos == NULL) {
                result = 1;
            }

            // capture
            else if (temp.x == end.x && temp.y == end.y && atPos->owner() != p) {
                result = MOVE_CAPTURE;
            }

            // piece is blocked
            else {
                return MOVE_ERROR_BLOCKED;
            }
        }
    }

    //horizontal or vertical movement
    else if (horizontal == 0 ||  vertical == 0) {

        for(int i = 1; i <= horizontal; i++) {
            // left
            if (start.x > end.x) {
                temp.x = start.x - i;
                temp.y = start.y;
            }

            // right
            else {
                temp.x = start.x + i;
                temp.y = start.y;
            }

            atPos = board.getPiece(temp);

            // clear path
            if (atPos == NULL) {
                result = 1;
            }

            // capture
            else if (temp.x == end.x && temp.y == end.y && atPos->owner() != p) {
                result = MOVE_CAPTURE;
            }

            // piece is blocked
            else {
                return MOVE_ERROR_BLOCKED;
            }
        }

        for(int i = 1; i <= vertical; i++) {
            // down
            if (start.y > end.y) {
                temp.y = start.y - i;
                temp.x = start.x;
            }

            // up
            else {
                temp.y = start.y + i;
                temp.x = start.x;
            }

            atPos = board.getPiece(temp);

            // clear path
            if(board.getPiece(temp) == NULL) {
                result = 1;
            }

            // capture
            else if (temp.x == end.x && temp.y == end.y && atPos->owner() != p) {
                result = MOVE_CAPTURE;
            }

            // piece is blocked
            else {
                return MOVE_ERROR_BLOCKED;
            }
        }
    }
    else {
        result = MOVE_ERROR_ILLEGAL;
    }
    return result;
}

int King::validMove(Position start, Position end, const Board& board) const {
    int result = 0;
    Position temp;
    Piece* atPos;
    Player p = board.playerTurn();
    int changeX = abs((int)start.x - (int)end.x);
    int changeY = abs((int)start.y - (int)end.y);

    if(board.getPiece(start)->owner() != board.playerTurn()) {
        result = MOVE_ERROR_ILLEGAL;
    }
    //one square only: horizontal, vertical, diagonal
    else if((changeX == 1 && changeY == 0) ||
            (changeX == 0 && changeY == 1) ||
            (changeX == 1 && changeY == 1)) {
        temp.x = end.x;
        temp.y = end.y;
        atPos = board.getPiece(temp);

        // clear path
        if (atPos == NULL) {
            result = 1;
        }

        // capture
        else if (atPos->owner() != p) {
            result = MOVE_CAPTURE;
        }

        // piece is blocked
        else {
            return MOVE_ERROR_BLOCKED;
        }
    }

    else {
        result = MOVE_ERROR_ILLEGAL;
    }
    return result;
}

void ChessGame::printMsg(int code) {
    switch (code) {
    case -10:
        Prompts::loadFailure();
        break;
    case -9:
        Prompts::saveFailure();
        break;
    case -8:
        Prompts::parseError();
        break;
    case -7:
        Prompts::outOfBounds();
        break;
    case -6:
        Prompts::noPiece();
        break;
    case -5:
        Prompts::blocked();
        break;
    case -4:
        Prompts::cantCastle();
        break;
    case -3:
        Prompts::mustHandleCheck();
        break;
    case -2:
        Prompts::cantExposeCheck();
        break;
    case -1:
        Prompts::illegalMove();
        break;
    case 2:
        Prompts::check(playerTurn());
        break;
    case 3:
        Prompts::capture(playerTurn());
        break;
    case 4:
        Prompts::checkMate(playerTurn());
        break;
    case 5:
        Prompts::staleMate();
        break;
    case 6:
        Prompts::win(playerTurn(), turn() - 1);
        break;
    case 7:
        Prompts::gameOver();
        break;
    default:
        break;
    }
}

// Setup the chess board with its initial pieces
void ChessGame::setupBoard() {
    std::vector<int> pieces {
        ROOK_ENUM, KNIGHT_ENUM, BISHOP_ENUM, QUEEN_ENUM,
        KING_ENUM, BISHOP_ENUM, KNIGHT_ENUM, ROOK_ENUM
    };
    for (size_t i = 0; i < pieces.size(); ++i) {
        initPiece(PAWN_ENUM, WHITE, Position(i, 1));
        initPiece(pieces[i], WHITE, Position(i, 0));
        initPiece(pieces[i], BLACK, Position(i, 7));
        initPiece(PAWN_ENUM, BLACK, Position(i, 6));
    }
}
//prints board to screen
//goes through m_pieces vector and prints out id of piece
void ChessGame::printBoard() {
    Terminal::set_default();
    int color = 0;
    int row = 8;
    Terminal::colorFg(false, Terminal::WHITE);
    cout << "\n   a b c d e f g h\n" << endl;

    for(int i = 63; i > -1 ; i = i-8) {

        Terminal::colorFg(false, Terminal::WHITE);
        cout << row-- << "  ";

        for(int j = 7; j > -1; j--) {
            Piece *pee = m_pieces[i-j];

            if(color % 2 == 0) {
                Terminal::colorBg(Terminal::GREEN);
            }
            else {
                Terminal::colorBg(Terminal::WHITE);
            }
            color++;

            if(pee != NULL) {
                int id = pee->id();
                if (pee->owner() == WHITE) {

                    Terminal::colorFg(false, Terminal::RED);
                    //cout << endl;
                    if(id == 0) {
                        cout << "p" << " ";
                    }
                    if(id == 1) {
                        cout << "r" << " ";
                    }
                    if(id == 2) {
                        cout << "n" << " ";
                    }
                    if(id == 3) {
                        cout << "b" << " ";
                    }
                    if(id == 4) {
                        cout << "q" << " ";
                    }
                    if(id == 5) {
                        cout << "k" << " ";
                    }
                }
                else {

                    Terminal::colorFg(false, Terminal::BLACK);
                    //cout << endl;
                    if(id == 0) {
                        cout << "P" << " ";
                    }
                    if(id == 1) {
                        cout <<  "R" << " ";
                    }
                    if(id == 2) {
                        cout << "N" << " ";
                    }
                    if(id == 3) {
                        cout << "B" << " ";
                    }
                    if(id == 4) {
                        cout << "Q" << " ";
                    }
                    if(id == 5) {
                        cout << "K" << " ";
                    }
                    //cout << endl;
                }
            }
            else {
                //cout << endl;
                cout << "  ";
                //cout << endl;
            }
        }
        Terminal::colorBg(Terminal::DEFAULT_COLOR);
        color++;
        cout << endl;
        //cout << endl;
    }
    cout << endl;
    Terminal::set_default();
    //cout << CSI << "Om";
}

//save game
int ChessGame::save(string file) {
    int result = 1;
    Piece* p;
    Position pos;
    char one;
    int turn = (this->m_turn) - 1;

    // open file
    ofstream output;
    output.open(file);

    // write name and turn to file
    output << "chess\n" << turn << '\n';

    // go through board and write remaining pieces to file
    for (int w = 0; w < (int) this->m_width; w++) {
        for (int h = 0; h < (int) this->m_height; h++) {
            pos.x = w;
            pos.y = h;
            p = getPiece(pos);
            one = pos.x + 'a';

            if (p != NULL ) {
                output << p->owner() << " " << one << (pos.y+1) << " " << p->id() << '\n';

            }
        }
    }

    // if save has failed
    size_t size = output.tellp();

    if (!output || size == 0) {
        result = -1;
    }

    output.close();
    return result;
}

int ChessGame::load(string file) {
    vector<string> info;
    int owner;
    string pos;
    int id;
    string junk;
    int turn;
    Position position;
    Player player = WHITE;

    //open saved file
    ifstream input;

    input.open(file);

    if (!input) {
        return -1;
    }

    // read chess and turn number
    input >> junk >> turn;

    this->m_turn = turn + 1;
    // until the end is reached
    while (input >> owner >> pos >> id) {
        position.x = pos.at(0) - 'a';
        position.y = pos.at(1) - '0' - 1;
        if(owner == 1) {
            player = BLACK;
        }

        else {
            player = WHITE;
        }
        // initialize piece on board
        Board::initPiece(id, player, position);
    }

    input.close();
    return 1;
}

