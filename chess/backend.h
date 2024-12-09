#pragma once

#include <vector>
#include <utility> // для std::pair
#include <algorithm> // для std::fill
#include <iostream> // при необходимости

const int SIZE = 8;

enum GameMode {
    AGAINST_COMPUTER,
    AGAINST_FRIEND
};

struct Move {
    char piece;       // Фигура, которая ходит
    int fromRow, fromCol; // Откуда
    int toRow, toCol;     // Куда
    char playerColor;     // 'W' или 'B'
};

class ChessGame {
public:
    ChessGame(GameMode mode);

    void initializeBoard();

    // Проверка легальности хода
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
                     char playerColor, bool ignoreCheck = false,
                     const char customBoard[SIZE][SIZE] = nullptr);

    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);

    bool isSquareAttacked(int row, int col, char opponentColor, const char boardState[SIZE][SIZE]);

    // Копирование доски
    void copyBoard(const char srcBoard[SIZE][SIZE], char destBoard[SIZE][SIZE]);

    // Проверка шаха, мата, пата
    bool isInCheck(char kingChar, const char customBoard[SIZE][SIZE] = nullptr);
    bool isInCheckmate(char kingChar);
    bool isStalemate(char kingChar);

    bool isKingPresent(char kingChar);

    // Поиск позиции короля
    void findKingPosition(char kingChar, int& kingRow, int& kingCol);

    // Вычисление возможных ходов для фигуры
    void calculatePossibleMoves(int fromRow, int fromCol, char playerColor, std::vector<std::pair<int,int>>& moves);

    // Проверки ходов для разных фигур
    bool isValidPawnMove(int fromRow, int fromCol, int toRow, int toCol, char playerColor, const char currentBoard[SIZE][SIZE]);
    bool isValidRookMove(int fromRow, int fromCol, int toRow, int toCol, const char currentBoard[SIZE][SIZE]);
    bool isValidKnightMove(int fromRow, int fromCol, int toRow, int toCol);
    bool isValidBishopMove(int fromRow, int fromCol, int toRow, int toCol, const char currentBoard[SIZE][SIZE]);
    bool isValidQueenMove(int fromRow, int fromCol, int toRow, int toCol, const char currentBoard[SIZE][SIZE]);
    bool isValidKingMove(int fromRow, int fromCol, int toRow, int toCol, const char currentBoard[SIZE][SIZE]);

    // Данные игры
    GameMode gameMode;

    char board[SIZE][SIZE]; // Шахматная доска

    std::vector<char> whiteCapturedPieces; // Захваченные белые фигуры (чёрными)
    std::vector<char> blackCapturedPieces; // Захваченные чёрные фигуры (белыми)
    std::vector<Move> moveHistory;         // История ходов

    bool whiteKingMoved;         // Двигался ли белый король
    bool blackKingMoved;         // Двигался ли чёрный король
    bool whiteRookMoved[2];      // Двигались ли белые ладьи (0 - слева, 1 - справа)
    bool blackRookMoved[2];      // Двигались ли чёрные ладьи (0 - слева, 1 - справа)

    int enPassantTargetRow;      // Координаты поля, которое может быть взято на проходе
    int enPassantTargetCol;

    char currentPlayer;          // Текущий игрок ('W' или 'B')
};