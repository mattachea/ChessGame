#include "game.h"
#include "chess.h"
#include "board.cpp"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;


void TESTPawnValidMove();
void TESTKnightValidMove();
void TESTRookValidMove();
void TESTBishopValidMove();
void TESTQueenValidMove();
void TESTKingValidMove();
void TESTmakeMove();
void TESTisChecked();
void TESTcheckOrStale();
void TESTcastle();

int main() {
    TESTPawnValidMove();
    TESTKnightValidMove();
    TESTRookValidMove();
    TESTBishopValidMove();
    TESTQueenValidMove();
    TESTKingValidMove();
    TESTmakeMove();
    TESTisChecked();
    TESTcheckOrStale();
    TESTcastle();
}


void TESTcastle() {
  ChessGame chess1;
  ChessGame chess2;
  ChessGame chess3;
  chess1.ChessGame::load("zcastle1.txt");
  chess2.ChessGame::load("zcastle2.txt");
  chess3.ChessGame::load("zcastle3.txt");
  Position start1;
  start1.x = 4;
  start1.y = 0;
  Position end1;
  end1.x = 6;
  end1.y = 0;
  Position start2;
  start2.x = 4;
  start2.y = 0;
  Position end2;
  end2.x = 2;
  end2.y = 0;
  Position start3;
  start3.x = 4;
  start3.y = 0;
  Position end3;
  end3.x = 2;
  end3.y = 0;

  int  r1 = chess1.castle(start1, end1);
  int  r2 = chess2.castle(start2, end2);
  int  r3 = chess3.castle(start3, end3);
  
  if(r1 == SUCCESS  &&
     r2 == SUCCESS   &&            
     r3 == MOVE_ERROR_CANT_CASTLE){
        cout << "castle()  -  passed" << endl;
    }
    else cout << "castle()  -  failed" << endl;
}






void TESTmakeMove() {
    ChessGame chess;
    chess.ChessGame::load("zmove1.txt");
    //white's turn
    Position start1;
    start1.x = 0;
    start1.y = 2;
    Position end1;
    end1.x = 0;
    end1.y = 0;
    Position start2;
    start2.x = 3;
    start2.y = 0;
    Position end2;
    end2.x = 7;
    end2.y = 4;
    Position start3;
    start3.x = 4;
    start3.y = 2;
    Position end3;
    end3.x = 4;
    end3.y = 4;
    Position start4;
    start4.x = 5;
    start4.y = 2;
    Position end4;
    end4.x = 6;
    end4.y = 4;
    //successful move
    int r1 = chess.makeMove(start1, end1);
    //blocked move
    int r2 = chess.makeMove(start2, end2);
    //illegal move
    int r3 = chess.makeMove(start3, end3);
    //capture
    int r4 = chess.makeMove(start4, end4);

    if(r1 == SUCCESS            &&
            r2 == MOVE_ERROR_BLOCKED &&
            r3 == MOVE_ERROR_ILLEGAL &&
            r4 == MOVE_CAPTURE       ) {
        cout << "makeMove()     - passed" << endl;
    }
    else cout << "makeMove()     - failed" << endl;
}


void TESTcheckOrStale() {
    //checkmate
    ChessGame chess1;
    ChessGame chess2;
    ChessGame chess3;
    ChessGame chess4;
    chess1.ChessGame::load("zmate1.txt");
    chess2.ChessGame::load("zmate2.txt");
    chess3.ChessGame::load("zmate2.txt");
    chess4.ChessGame::load("zstalemate1.txt");

    Position start1;
    start1.x = 7;
    start1.y = 6;
    Position end1;
    end1.x = 7;
    end1.y = 7;
    Position start2;
    start2.x = 6;
    start2.y = 0;
    Position end2;
    end2.x = 6;
    end2.y = 6;
    Position start3;
    start3.x = 6;
    start3.y = 0;
    Position end3;
    end3.x = 6;
    end3.y = 5;
    Position start4;
    start4.x = 2;
    start4.y = 4;
    Position end4;
    end4.x = 2;
    end4.y = 1;

    chess1.makeMove(start1, end1);
    chess2.makeMove(start2, end2);
    chess3.makeMove(start3, end3);
    chess4.makeMove(start4, end4);

    int r1 = chess1.checkOrStale();
    int r2 = chess2.checkOrStale();
    int r3 = chess3.checkOrStale();
    int r4 = chess4.checkOrStale();

    if (r1 == MOVE_CHECKMATE &&
            r2 == MOVE_CHECKMATE &&
            r3 == MOVE_STALEMATE &&
            r4 == MOVE_STALEMATE) {
        cout << "checkOrStale() - passed" << endl;
    }
    else cout << "checkOrStale() - failed" << endl;

    //cout << r1 << endl;
    //cout << r2 << endl;

}

void TESTisChecked() {
    ChessGame chess1;
    chess1.ChessGame::load("check1.txt");
    int result1 = chess1.isChecked();

    ChessGame chess2;
    chess2.ChessGame::load("check2.txt");
    int result2 = chess2.isChecked();

    ChessGame chess3;
    chess3.ChessGame::load("check3.txt");
    int result3 = chess3.isChecked();

    if (result1 == -2 && result2 == 2 && result3 == 1) {
        cout << "isChecked()    - passed" << endl;
    }

    else {
        cout << "isChecked()    - failed" << endl;
    }
}

void TESTPawnValidMove() {
    ChessGame chess;
    chess.ChessGame::load("zpawn1.txt");

    Position start1;
    Position end1;
    start1.x = 4;
    start1.y = 4;
    end1.x = 4;
    end1.y = 3;
    Piece *temp1 = chess.getPiece(start1);

    Position start2;
    Position end2;
    start2.x = 1;
    start2.y = 4;
    end2.x = 1;
    end2.y = 5;
    Piece *temp2 = chess.getPiece(start2);

    Position start3;
    Position end3;
    start3.x = 1;
    start3.y = 4;
    end3.x = 2;
    end3.y = 4;
    Piece *temp3 = chess.getPiece(start3);

    Position start4;
    Position end4;
    start4.x = 1;
    start4.y = 4;
    end4.x = 2;
    end4.y = 3;
    Piece *temp4 = chess.getPiece(start4);

    Position start5;
    Position end5;
    start5.x = 1;
    start5.y = 4;
    end5.x = 1;
    end5.y = 3;
    Piece *temp5 = chess.getPiece(start5);

    Position start6;
    Position end6;
    start6.x = 0;
    start6.y = 6;
    end6.x = 0;
    end6.y = 3;
    Piece *temp6 = chess.getPiece(start6);

    Position start7;
    Position end7;
    start7.x = 0;
    start7.y = 6;
    end7.x = 0;
    end7.y = 4;
    Piece *temp7 = chess.getPiece(start7);

    Position start8;
    Position end8;
    start8.x = 6;
    start8.y = 4;
    end8.x = 7;
    end8.y = 3;
    Piece *temp8 = chess.getPiece(start8);

    //blocked by another piece
    int r1 = temp1->validMove(start1, end1, chess);
    //ERROR moving backward
    int r2 = temp2->validMove(start2, end2, chess);
    //ERROR moving right
    int r3 = temp3->validMove(start3, end3, chess);
    //ERROR moving forward diagonal, without capturing
    int r4 = temp4->validMove(start4, end4, chess);
    //forward
    int r5 = temp5->validMove(start5, end5, chess);
    //ERROR for triple move from starting position
    int r6 = temp6->validMove(start6, end6, chess);
    //double move from starting position
    int r7 = temp7->validMove(start7, end7, chess);
    //capturing forward diagonally
    int r8 = temp8->validMove(start8, end8, chess);

    if(r1 == MOVE_ERROR_BLOCKED  &&
            r2 == MOVE_ERROR_ILLEGAL  &&
            r3 == MOVE_ERROR_ILLEGAL  &&
            r4 == MOVE_ERROR_ILLEGAL  &&
            r5 == SUCCESS             &&
            r6 == MOVE_ERROR_ILLEGAL  &&
            r7 == SUCCESS             &&
            r8 == MOVE_CAPTURE        ) {
        cout << "Pawn  -  validMove() passed" << endl;
    }
    else cout << "Pawn  -  validMove() failed" << endl;
}

void TESTKnightValidMove() {
    ChessGame chess;
    chess.ChessGame::load("zknight1.txt");
    Position start1;
    Position end1;
    start1.x = 3;
    start1.y = 3;
    end1.x = 4;
    end1.y = 5;
    Piece *temp1 = chess.getPiece(start1);

    Position start2;
    Position end2;
    start2.x = 3;
    start2.y = 3;
    end2.x = 1;
    end2.y = 2;
    Piece *temp2 = chess.getPiece(start2);

    Position start3;
    Position end3;
    start3.x = 3;
    start3.y = 3;
    end3.x = 4;
    end3.y = 4;
    Piece *temp3 = chess.getPiece(start3);

    Position start4;
    Position end4;
    start4.x = 5;
    start4.y = 5;
    end4.x = 7;
    end4.y = 4;
    Piece *temp4 = chess.getPiece(start4);

    //blocked by another piece
    int r1 = temp1->validMove(start1, end1, chess);
    //valid move, an L
    int r2 = temp2->validMove(start2, end2, chess);
    //invalid move, not an L movement
    int r3 = temp3->validMove(start3, end3, chess);
    //capture enemy
    int r4 = temp4->validMove(start4, end4, chess);

    if(r1 == MOVE_ERROR_BLOCKED  &&
            r2 == SUCCESS &&
            r3 == MOVE_ERROR_ILLEGAL &&
            r4 == MOVE_CAPTURE) {
        cout << "Knight-  validMove() passed" << endl;
    }
    else cout << "Knight-  validMove() failed" << endl;
}

void TESTRookValidMove() {
    ChessGame chess;
    chess.ChessGame::load("zrook1.txt");
    Position start1;
    Position end1;
    start1.x = 3;
    start1.y = 4;
    end1.x = 3;
    end1.y = 6;
    Piece *temp1 = chess.getPiece(start1);

    Position start2;
    Position end2;
    start2.x = 3;
    start2.y = 4;
    end2.x = 7;
    end2.y = 4;
    Piece *temp2 = chess.getPiece(start2);

    Position start3;
    Position end3;
    start3.x = 3;
    start3.y = 4;
    end3.x = 4;
    end3.y = 3;
    Piece *temp3 = chess.getPiece(start3);

    Position start4;
    Position end4;
    start4.x = 3;
    start4.y = 4;
    end4.x = 1;
    end4.y = 4;
    Piece *temp4 = chess.getPiece(start4);

    //blocked by another piece
    int r1 = temp1->validMove(start1, end1, chess);
    //valid move- right
    int r2 = temp2->validMove(start2, end2, chess);
    //invalid move, diagonal
    int r3 = temp3->validMove(start3, end3, chess);
    //capture enemy
    int r4 = temp4->validMove(start4, end4, chess);

    if(r1 == MOVE_ERROR_BLOCKED  &&
            r2 == SUCCESS &&
            r3 == MOVE_ERROR_ILLEGAL &&
            r4 == MOVE_CAPTURE) {
        cout << "Rook  -  validMove() passed" << endl;
    }
    else cout << "Rook  -  validMove() failed" << endl;
}

void TESTBishopValidMove() {
    ChessGame chess;
    chess.ChessGame::load("zbishop1.txt");
    Position start1;
    Position end1;
    start1.x = 4;
    start1.y = 4;
    end1.x = 6;
    end1.y = 6;
    Piece *temp1 = chess.getPiece(start1);

    Position start2;
    Position end2;
    start2.x = 4;
    start2.y = 4;
    end2.x = 6;
    end2.y = 2;
    Piece *temp2 = chess.getPiece(start2);

    Position start3;
    Position end3;
    start3.x = 4;
    start3.y = 4;
    end3.x = 7;
    end3.y = 4;
    Piece *temp3 = chess.getPiece(start3);

    Position start4;
    Position end4;
    start4.x = 2;
    start4.y = 3;
    end4.x = 5;
    end4.y = 0;
    Piece *temp4 = chess.getPiece(start4);

    //blocked by another piece
    int r1 = temp1->validMove(start1, end1, chess);
    //valid move- diagonal
    int r2 = temp2->validMove(start2, end2, chess);
    //invalid move, horizontal
    int r3 = temp3->validMove(start3, end3, chess);
    //capture enemy
    int r4 = temp4->validMove(start4, end4, chess);

    if(r1 == MOVE_ERROR_BLOCKED  &&
            r2 == SUCCESS &&
            r3 == MOVE_ERROR_ILLEGAL &&
            r4 == MOVE_CAPTURE) {
        cout << "Bishop-  validMove() passed" << endl;
    }
    else cout << "Bishop-  validMove() failed" << endl;
}

void TESTQueenValidMove() {
    ChessGame chess;
    chess.ChessGame::load("zqueen1.txt");
    Position start;
    Position end1;
    start.x = 6;
    start.y = 3;
    end1.x = 3;
    end1.y = 6;
    Piece *temp1 = chess.getPiece(start);

    Position end2;
    end2.x = 0;
    end2.y = 3;
    Piece *temp2 = chess.getPiece(start);

    Position end3;
    end3.x = 3;
    end3.y = 0;
    Piece *temp3 = chess.getPiece(start);

    Position end4;
    end4.x = 3;
    end4.y = 3;
    Piece *temp4 = chess.getPiece(start);

    Position end5;
    end5.x = 4;
    end5.y = 2;
    Piece *temp5 = chess.getPiece(start);

    Position end6;
    end6.x = 4;
    end6.y = 5;
    Piece *temp6 = chess.getPiece(start);

    //blocked diagonally by another piece
    int r1 = temp1->validMove(start, end1, chess);
    //blocked horizontally by another piece
    int r2 = temp2->validMove(start, end2, chess);
    //valid move- diagonal
    int r3 = temp3->validMove(start, end3, chess);
    //valid move, horizontal
    int r4 = temp4->validMove(start, end4, chess);
    //invalid move
    int r5 = temp5->validMove(start, end5, chess);
    //capture enemy
    int r6 = temp6->validMove(start, end6, chess);

    if(r1 == MOVE_ERROR_BLOCKED  &&
            r2 == MOVE_ERROR_BLOCKED  &&
            r3 == SUCCESS             &&
            r4 == SUCCESS             &&
            r5 == MOVE_ERROR_ILLEGAL  &&
            r6 == MOVE_CAPTURE        ) {
        cout << "Queen -  validMove() passed" << endl;
    }
    else cout << "Queen -  validMove() failed" << endl;
}

void TESTKingValidMove() {
    ChessGame chess;
    chess.ChessGame::load("zking1.txt");
    Position start;
    Position end1;
    start.x = 5;
    start.y = 3;
    end1.x = 4;
    end1.y = 2;
    Piece *temp1 = chess.getPiece(start);

    Position end2;
    end2.x = 6;
    end2.y = 3;
    Piece *temp2 = chess.getPiece(start);

    Position end3;
    end3.x = 6;
    end3.y = 2;
    Piece *temp3 = chess.getPiece(start);

    Position end4;
    end4.x = 4;
    end4.y = 3;
    Piece *temp4 = chess.getPiece(start);

    Position end5;
    end5.x = 6;
    end5.y = 4;
    Piece *temp5 = chess.getPiece(start);

    //blocked diagonally by another piece
    int r1 = temp1->validMove(start, end1, chess);
    //blocked horizontally by another piece
    int r2 = temp2->validMove(start, end2, chess);
    //valid move- diagonal
    int r3 = temp3->validMove(start, end3, chess);
    //valid move, horizontal
    int r4 = temp4->validMove(start, end4, chess);
    //capture
    int r5 = temp5->validMove(start, end5, chess);

    if(r1 == MOVE_ERROR_BLOCKED  &&
            r2 == MOVE_ERROR_BLOCKED  &&
            r3 == SUCCESS             &&
            r4 == SUCCESS             &&
            r5 == MOVE_CAPTURE        ) {
        cout << "King  -  validMove() passed" << endl;
    }
    else cout << "King  -  validMove() failed" << endl;
}
