// frontend.cpp

#include "frontend.h"
#include "bot.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Button.H>     // Added this include for Fl_Button
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include <iostream>
#include <cstdlib>
#include <ctime>

ChessBoard::ChessBoard(int X, int Y, int W, int H, Fl_Box* msgBox, GameMode mode)
        : Fl_Widget(X, Y, W, H), isPlayerTurn(true), gameOver(false), messageBox(msgBox), gameMode(mode), currentPlayer('W'), checkWindow(nullptr) {
    loadPieceImages();
    updateMessage();
    chessGame = new ChessGame(mode);
}

ChessBoard::~ChessBoard() {
    // Освобождаем ресурсы
    for (auto& pair : pieceImages) {
        delete pair.second;
    }
    for (auto& pair : smallPieceImages) {
        delete pair.second;
    }
    if (checkImage) {
        delete checkImage;
    }
    if (checkWindow) {
        checkWindow->hide();
        delete checkWindow;
    }
    delete chessGame;
}

void ChessBoard::draw() {
    const int cell_size = 50;
    const int x_offset = 20; // Сдвиг доски вправо на 10 пикселей

    // Очищаем всю область виджета
    fl_color(FL_GRAY);
    fl_rectf(x(), y(), w(), h());

    // Рисуем захваченные фигуры чёрных (сверху, над доской)
    for (size_t i = 0; i < chessGame->blackCapturedPieces.size(); ++i) {
        char pieceChar = chessGame->blackCapturedPieces[i];
        if (smallPieceImages.count(pieceChar)) {
            Fl_Image* img = smallPieceImages[pieceChar];
            if (img) {
                int offsetX = 5 + i * 25; // Интервал в 25 пикселей
                int offsetY = y() + 455; // Верхняя область
                img->draw(x() + offsetX, offsetY + 10); // Центрируем по вертикали
            }
        }
    }

    // Рисуем шахматную доску и фигуры
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            fl_color((i + j) % 2 == 0 ? FL_WHITE : FL_BLUE);
            fl_rectf(x() + x_offset + j * cell_size, y() + 50 + i * cell_size, cell_size, cell_size);

            // Выделение возможных ходов
            if (isPossibleMove(i, j)) {
                fl_color(FL_GREEN);
                fl_rect(x() + x_offset + j * cell_size, y() + 50 + i * cell_size, cell_size, cell_size);
            }

            // Выделение выбранной фигуры
            if (i == selectedFromRow && j == selectedFromCol) {
                fl_color(FL_YELLOW);
                fl_rect(x() + x_offset + j * cell_size, y() + 50 + i * cell_size, cell_size, cell_size);
            }

            // Рисуем фигуры
            if (chessGame->board[i][j] != '.') {
                char pieceChar = chessGame->board[i][j];
                if (pieceImages.count(pieceChar)) {
                    Fl_Image* img = pieceImages[pieceChar];
                    if (img) {
                        int imgWidth = img->w();
                        int imgHeight = img->h();
                        int offsetX_img = (cell_size - imgWidth) / 2;
                        int offsetY_img = (cell_size - imgHeight) / 2;
                        img->draw(x() + x_offset + j * cell_size + offsetX_img, y() + 50 + i * cell_size + offsetY_img);
                    }
                }
            }
        }
    }

    // Рисуем буквы столбцов снизу (A-H)
    fl_font(FL_HELVETICA, 14);
    for (int j = 0; j < SIZE; ++j) {
        char label[2];
        sprintf(label, "%c", 'A' + j);
        fl_color(FL_BLACK);
        int text_x = x() + x_offset + j * cell_size + cell_size / 2 - fl_width(label) / 2;
        int text_y = y() + 50 + SIZE * cell_size + 15;
        fl_draw(label, text_x, text_y);
    }

    // Рисуем цифры строк слева (8-1)
    for (int i = 0; i < SIZE; ++i) {
        char label[2];
        sprintf(label, "%d", 8 - i);
        fl_color(FL_BLACK);
        int text_x = x() + x_offset - 15;
        int text_y = y() + 50 + i * cell_size + cell_size / 2 + fl_height() / 2;
        fl_draw(label, text_x, text_y);
    }

    // Рисуем захваченные фигуры белых (снизу, под доской)
    for (size_t i = 0; i < chessGame->whiteCapturedPieces.size(); ++i) {
        char pieceChar = chessGame->whiteCapturedPieces[i];
        if (smallPieceImages.count(pieceChar)) {
            Fl_Image* img = smallPieceImages[pieceChar];
            if (img) {
                int offsetX = 5 + i * 25; // Интервал в 25 пикселей
                int offsetY = y() - 5; // Нижняя область
                img->draw(x() + offsetX, offsetY + 10); // Центрируем по вертикали
            }
        }
    }

    // Если король под шахом, выделяем его красной рамкой
    char kingChar = (currentPlayer == 'W') ? 'K' : 'k';
    if (chessGame->isInCheck(kingChar)) {
        int kingRow, kingCol;
        chessGame->findKingPosition(kingChar, kingRow, kingCol);
        if (kingRow != -1 && kingCol != -1) {
            fl_color(FL_RED);
            fl_rect(x() + x_offset + kingCol * cell_size, y() + 50 + kingRow * cell_size, cell_size, cell_size);
        }
    }

    // Рисуем историю ходов
    drawMoveHistory();
}

int ChessBoard::handle(int event) {
    if (gameOver) {
        // Если игра окончена, игнорируем ввод
        return 0;
    }

    switch (event) {
        case FL_PUSH: {
            if (!isPlayerTurn) {
                // Если сейчас не ход игрока, игнорируем ввод
                return 0;
            }

            int mx = Fl::event_x();
            int my = Fl::event_y();
            const int cell_size = 50;
            const int x_offset = 20; // Сдвиг доски вправо на 10 пикселей
            int col = (mx - x_offset) / cell_size;
            int row = (my - y() - 50) / cell_size;

            if (col < 0 || col >= SIZE || row < 0 || row >= SIZE) return 0;

            if (selectedFromRow != -1 && selectedFromCol != -1) {
                // Попытка переместить выбранную фигуру

                if (chessGame->isValidMove(selectedFromRow, selectedFromCol, row, col, currentPlayer)) {
                    // Ход корректен, выполняем его
                    if (!chessGame->movePiece(selectedFromRow, selectedFromCol, row, col)) {
                        // Если по каким-то причинам ход не смог выполниться (что маловероятно, раз isValidMove=true)
                        selectedFromRow = -1;
                        selectedFromCol = -1;
                        possibleMoves.clear();
                        redraw();
                        return 1;
                    }

                    // Сбрасываем выделение
                    selectedFromRow = -1;
                    selectedFromCol = -1;
                    possibleMoves.clear();
                    redraw();
                    Fl::flush();

                    // Определяем противника
                    char opponentKing = (currentPlayer == 'W') ? 'k' : 'K';

                    // Проверяем мат
                    if (chessGame->isInCheckmate(opponentKing)) {
                        gameOver = true;
                        fl_message("%s победили! Мат %s королю.",
                                   currentPlayer == 'W' ? "Белые" : "Чёрные",
                                   currentPlayer == 'W' ? "Чёрному" : "Белому");
                        updateMessage();
                        showCheckWindow(false);
                        return 1;
                    }

                    // Проверяем шах
                    if (chessGame->isInCheck(opponentKing)) {
                        showCheckWindow(true);
                    } else {
                        showCheckWindow(false);
                    }

                    // Смена хода
                    if (gameMode == AGAINST_FRIEND) {
                        currentPlayer = (currentPlayer == 'W') ? 'B' : 'W';
                        isPlayerTurn = true;
                        updateMessage();
                    } else {
                        // Против компьютера
                        currentPlayer = 'B';
                        isPlayerTurn = false;
                        updateMessage();

                        // Ход бота
                        BotPlayer* bot = new BotPlayer(chessGame, this);
                        bot->makeMove();
                    }

                } else {
                    // Недопустимый ход, сбрасываем выбор
                    selectedFromRow = -1;
                    selectedFromCol = -1;
                    possibleMoves.clear();
                }
            } else {
                // Выбор фигуры
                char piece = chessGame->board[row][col];
                if ((currentPlayer == 'W' && piece >= 'A' && piece <= 'Z') ||
                    (currentPlayer == 'B' && piece >= 'a' && piece <= 'z')) {
                    selectedFromRow = row;
                    selectedFromCol = col;
                    chessGame->calculatePossibleMoves(selectedFromRow, selectedFromCol, currentPlayer, possibleMoves);
                }
            }
            redraw();
            return 1;
        }
        default:
            return 0;
    }
}

void ChessBoard::updateMessage() {
    if (gameOver) {
        messageBox->label("Игра окончена");
    } else if (isPlayerTurn) {
        if (gameMode == AGAINST_FRIEND) {
            messageBox->label(currentPlayer == 'W' ? "Ход белых" : "Ход чёрных");
        } else {
            messageBox->label("Ход белых");
        }
    } else {
        if (gameMode == AGAINST_FRIEND) {
            messageBox->label(currentPlayer == 'W' ? "Ход белых" : "Ход чёрных");
        } else {
            messageBox->label("Ход чёрных");
        }
    }
    messageBox->redraw();
}

void ChessBoard::showCheckWindow(bool show) {
    if (show) {
        if (!checkWindow) {
            checkWindow = new Fl_Window(300, 200, "Шах!");
            Fl_Box* imageBox = new Fl_Box(0, 0, 300, 150);
            imageBox->image(checkImage->copy(300, 150)); // Устанавливаем изображение

            Fl_Box* textBox = new Fl_Box(0, 150, 300, 50, "Ваш король под шахом!");
            textBox->labelsize(20);
            textBox->labelfont(FL_BOLD);
            textBox->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);

            checkWindow->end();
            checkWindow->set_modal(); // Делает окно модальным
        }
        checkWindow->show();
    } else {
        if (checkWindow) {
            checkWindow->hide();
        }
    }
}

void ChessBoard::loadPieceImages() {
    // Устанавливаем базовый путь к изображениям
    std::string basePath = "images/";

    Fl_PNG_Image* img;

    img = new Fl_PNG_Image((basePath + "wP.png").c_str()); // White pawn
    pieceImages['P'] = img->copy(50, 50);
    smallPieceImages['P'] = img->copy(30, 30);
    delete img;

    img = new Fl_PNG_Image((basePath + "bP.png").c_str()); // Black pawn
    pieceImages['p'] = img->copy(50, 50);
    smallPieceImages['p'] = img->copy(30, 30);
    delete img;

    img = new Fl_PNG_Image((basePath + "wR.png").c_str()); // White rook
    pieceImages['R'] = img->copy(50, 50);
    smallPieceImages['R'] = img->copy(30, 30);
    delete img;

    img = new Fl_PNG_Image((basePath + "bR.png").c_str()); // Black rook
    pieceImages['r'] = img->copy(50, 50);
    smallPieceImages['r'] = img->copy(30, 30);
    delete img;

    img = new Fl_PNG_Image((basePath + "wN.png").c_str()); // White knight
    pieceImages['N'] = img->copy(50, 50);
    smallPieceImages['N'] = img->copy(30, 30);
    delete img;

    img = new Fl_PNG_Image((basePath + "bN.png").c_str()); // Black knight
    pieceImages['n'] = img->copy(50, 50);
    smallPieceImages['n'] = img->copy(30, 30);
    delete img;

    img = new Fl_PNG_Image((basePath + "wB.png").c_str()); // White bishop
    pieceImages['B'] = img->copy(50, 50);
    smallPieceImages['B'] = img->copy(30, 30);
    delete img;

    img = new Fl_PNG_Image((basePath + "bB.png").c_str()); // Black bishop
    pieceImages['b'] = img->copy(50, 50);
    smallPieceImages['b'] = img->copy(30, 30);
    delete img;

    img = new Fl_PNG_Image((basePath + "wQ.png").c_str()); // White queen
    pieceImages['Q'] = img->copy(50, 50);
    smallPieceImages['Q'] = img->copy(30, 30);
    delete img;

    img = new Fl_PNG_Image((basePath + "bQ.png").c_str()); // Black queen
    pieceImages['q'] = img->copy(50, 50);
    smallPieceImages['q'] = img->copy(30, 30);
    delete img;

    img = new Fl_PNG_Image((basePath + "wK.png").c_str()); // White king
    pieceImages['K'] = img->copy(50, 50);
    smallPieceImages['K'] = img->copy(30, 30);
    delete img;

    img = new Fl_PNG_Image((basePath + "bK.png").c_str()); // Black king
    pieceImages['k'] = img->copy(50, 50);
    smallPieceImages['k'] = img->copy(30, 30);
    delete img;

    // Загружаем изображение шаха
    checkImage = new Fl_PNG_Image((basePath + "cat.png").c_str()); // Замените "cat.png" на ваше изображение
}

bool ChessBoard::isPossibleMove(int row, int col) {
    for (const auto& move : possibleMoves) {
        if (move.first == row && move.second == col) {
            return true;
        }
    }
    return false;
}

void ChessBoard::drawMoveHistory() {
    int startX = x() + 420 + 20; // Начало по X (после доски)
    int startY = y() + 50;       // Начало по Y
    int lineHeight = 25;         // Высота строки для каждого хода
    int columnWidth = 150;       // Ширина каждого столбца

    fl_font(FL_HELVETICA, 14);
    fl_color(FL_BLACK);
    fl_draw("История ходов:", startX, startY - 20);

    for (size_t i = 0; i < chessGame->moveHistory.size(); ++i) {
        int column = i % 2; // 0 для левого столбца, 1 для правого
        int row = i / 2;

        int posX = startX + column * columnWidth;
        int posY = startY + row * lineHeight;

        if (posY + lineHeight > h()) {
            // Если выходим за пределы виджета, прекращаем рисование
            break;
        }

        Move& move = chessGame->moveHistory[i];

        // Рисуем маленькое изображение фигуры
        if (smallPieceImages.count(move.piece)) {
            Fl_Image* img = smallPieceImages[move.piece];
            if (img) {
                img->draw(posX, posY);
            }
        }

        // Формируем текст хода
        std::string fromCoord = getCoordinate(move.fromRow, move.fromCol);
        std::string toCoord = getCoordinate(move.toRow, move.toCol);
        std::string moveText = fromCoord + " → " + toCoord;

        // Рисуем текст хода
        fl_color(FL_BLACK);
        fl_draw(moveText.c_str(), posX + 35, posY + 20);
    }
}

std::string ChessBoard::getCoordinate(int row, int col) {
    char file = 'A' + col;
    char rank = '8' - row;
    return std::string(1, file) + std::string(1, rank);
}