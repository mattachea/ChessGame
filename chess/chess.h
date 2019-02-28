#ifndef CHESS_H
#define CHESS_H

#include "game.h"

// Game status codes
// -----------------
// These enumerations are optional. You can choose to use them,
// or you can decide they're not needed. They *could* be used, for
// example, to return values from validMove() and makeMove(), and
// any other methods you want. But so long as you follow the conventions
// of those methods (>0 is success, <0 is failure), you're free to
// do things your own way.
enum status {
    LOAD_FAILURE = -10,
    SAVE_FAILURE,
    PARSE_ERROR,
    MOVE_ERROR_OUT_OF_BOUNDS,
    MOVE_ERROR_NO_PIECE,
    MOVE_ERROR_BLOCKED,
    MOVE_ERROR_CANT_CASTLE,
    MOVE_ERROR_MUST_HANDLE_CHECK,
    MOVE_ERROR_CANT_EXPOSE_CHECK,
    MOVE_ERROR_ILLEGAL,
    SUCCESS = 1,
    MOVE_CHECK,
    MOVE_CAPTURE,
    MOVE_CHECKMATE,
    MOVE_STALEMATE,
    GAME_WIN,
    GAME_OVER
};

// Possible pieces
enum PieceEnum {
    PAWN_ENUM = 0,
    ROOK_ENUM,
    KNIGHT_ENUM,
    BISHOP_ENUM,
    QUEEN_ENUM,
    KING_ENUM
};


class Pawn : public Piece {
protected:
    friend PieceFactory<Pawn>;
    Pawn(Player owner, int id) : Piece(owner, id) {}
public:
    int validMove(Position start, Position end,
        const Board& board) const override;
};


class Rook : public Piece {
protected:
    friend PieceFactory<Rook>;
    Rook(Player owner, int id) : Piece(owner, id) {}
public:
    int validMove(Position start, Position end,
        const Board& board) const override;
};


class Knight : public Piece {
protected:
    friend PieceFactory<Knight>;
    Knight(Player owner, int id) : Piece(owner, id) {}
public:
    int validMove(Position start, Position end,
        const Board& board) const override;
};


class Bishop : public Piece {
protected:
    friend PieceFactory<Bishop>;
    Bishop(Player owner, int id) : Piece(owner, id) {}
public:
    int validMove(Position start, Position end,
        const Board& board) const override;
};


class Queen : public Piece {
protected:
    friend PieceFactory<Queen>;
    Queen(Player owner, int id) : Piece(owner , id) {}
public:
    int validMove(Position start, Position end,
        const Board& board) const override;
};


class King : public Piece {
protected:
    friend PieceFactory<King>;
 King(Player owner, int id) : Piece(owner, id) {}
public:
    int validMove(Position start, Position end, const Board& board) const override;
};


class ChessGame : public Board {
public:
    ChessGame() : Board(8, 8) {
        // Add all factories needed to create Piece subclasses
        addFactory(new PieceFactory<Pawn>(PAWN_ENUM));
        addFactory(new PieceFactory<Rook>(ROOK_ENUM));
        addFactory(new PieceFactory<Knight>(KNIGHT_ENUM));
        addFactory(new PieceFactory<Bishop>(BISHOP_ENUM));
        addFactory(new PieceFactory<Queen>(QUEEN_ENUM));
        addFactory(new PieceFactory<King>(KING_ENUM));
    }

    // Setup the chess board with its initial pieces
    virtual void setupBoard();

    // Whether the chess game is over
    virtual bool gameOver() const override {return false;}

    // Perform a move from the start Position to the end Position
    // The method returns an integer with the status
    // >= 0 is SUCCESS, < 0 is failure
    virtual int makeMove(Position start, Position end) override;

    // The actual top-level game implementation
    virtual int run();

    //not from skeleton code
    //display the chess board to screen
    void printBoard();

    //save game
    int save(std::string file);

    //load game
    int load(std::string file);
    
    // if moving a piece to a position will put the player in check 
    int isChecked();

    // for enum statuses
    void printMsg(int code);

    //if the player got checkmated
    int checkOrStale();

    // is castling possible
    int castle(Position start, Position end);
};

#endif
