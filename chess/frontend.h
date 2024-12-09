// frontend.h

#ifndef FRONTEND_H
#define FRONTEND_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <unordered_map>
#include <vector>

#include "backend.h"

// Предварительное объявление класса BotPlayer
class BotPlayer;

class ChessBoard : public Fl_Widget {
public:
    ChessBoard(int X, int Y, int W, int H, Fl_Box* msgBox, GameMode mode);
    ~ChessBoard();

    void draw() override;
    int handle(int event) override;

    void updateMessage();
    void showCheckWindow(bool show);

    bool isPlayerTurn;
    bool gameOver;
    char currentPlayer;

    friend class BotPlayer; // Даем доступ классу BotPlayer к приватным членам ChessBoard

private:
    ChessGame* chessGame;

    std::unordered_map<char, Fl_Image*> pieceImages;
    std::unordered_map<char, Fl_Image*> smallPieceImages; // Для маленьких изображений

    Fl_Box* messageBox;
    GameMode gameMode;
    Fl_Window* checkWindow;
    Fl_Image* checkImage;

    std::vector<std::pair<int, int>> possibleMoves;
    int selectedFromRow = -1, selectedFromCol = -1;

    void loadPieceImages();
    bool isPossibleMove(int row, int col);
    void drawMoveHistory();
    std::string getCoordinate(int row, int col);

    // Убираем методы botMove и botMoveCallback из ChessBoard
};

#endif // FRONTEND_H