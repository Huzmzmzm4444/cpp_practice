// bot.h

#ifndef BOT_H
#define BOT_H

#include "backend.h"

// Предварительное объявление класса ChessBoard
class ChessBoard;

class BotPlayer {
public:
    BotPlayer(ChessGame* game, ChessBoard* board = nullptr);
    void makeMove();
    void performBotMove();

private:
    ChessGame* chessGame;
    ChessBoard* chessBoard;

    int maxDepth;

    struct BotMove {
        Move move;
        int score;
    };

    struct GameState {
        char board[SIZE][SIZE];
        std::vector<char> whiteCapturedPieces;
        std::vector<char> blackCapturedPieces;
    };

    static void botMoveCallback(void* data);

    BotMove minimax(const GameState& state, int depth, int alpha, int beta, bool isMaximizingPlayer);

    int evaluateBoard(const GameState& state);

    std::vector<Move> generateAllPossibleMoves(const GameState& state, char playerColor);

    std::vector<Move> getValidMovesForPiece(const GameState& state, int fromRow, int fromCol, char playerColor);

    void makeMoveOnBoard(GameState& state, const Move& move);

    bool isGameOver(const GameState& state);

    bool isInCheck(const GameState& state, char kingChar);

    void copyGameState(const ChessGame& game, GameState& state);

    bool isOpponentPiece(char piece, char targetPiece);

    int evaluateTactics(const GameState& state, char playerColor);
};

#endif // BOT_H