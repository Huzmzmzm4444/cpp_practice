// bot.cpp

#include "bot.h"
#include "backend.h"   // Для доступа к ChessGame и связанным функциям
#include "frontend.h"  // Для доступа к классу ChessBoard
#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <algorithm>   // Для std::max и std::min
#include <cstdlib>     // Для rand()
#include <iostream>    // Для отладочных выводов

BotPlayer::BotPlayer(ChessGame* game, ChessBoard* board)
        : chessGame(game), chessBoard(board) {
    // Устанавливаем максимальную глубину для алгоритма minimax
    maxDepth = 3; // Можно изменить для настройки производительности
}

void BotPlayer::makeMove() {
    std::cout << "BotPlayer::makeMove() called." << std::endl;

    if (chessBoard) {
        std::cout << "chessBoard is not nullptr. GUI mode." << std::endl;
        // Блокируем ход игрока
        chessBoard->isPlayerTurn = false;
        chessBoard->updateMessage();

        // Генерируем случайную задержку от 1 до 3 секунд
        double delay = 1.0 + static_cast<double>(rand()) / RAND_MAX * 2.0;
        std::cout << "Delay for bot move: " << delay << " seconds." << std::endl;

        // Планируем ход бота
        Fl::add_timeout(delay, botMoveCallback, this);
    } else {
        std::cout << "chessBoard is nullptr. Test mode." << std::endl;
        // В режиме тестирования выполняем ход сразу
        performBotMove();
    }
}

void BotPlayer::performBotMove() {
    std::cout << "BotPlayer::performBotMove() called." << std::endl;

    // Создаём копию текущего состояния игры
    GameState currentState;
    copyGameState(*chessGame, currentState);
    std::cout << "GameState copied." << std::endl;

    // Используем алгоритм minimax для поиска лучшего хода
    BotMove bestMove = minimax(currentState, maxDepth, -1000000, 1000000, true);
    std::cout << "minimax completed." << std::endl;

    if (bestMove.move.fromRow == -1) {
        std::cout << "No valid moves available." << std::endl;
        // Нет доступных ходов, игра окончена
        return;
    }

    std::cout << "Best move found: from (" << bestMove.move.fromRow << ", " << bestMove.move.fromCol
              << ") to (" << bestMove.move.toRow << ", " << bestMove.move.toCol << ")." << std::endl;

    // Выполняем лучший ход на реальной доске
    chessGame->movePiece(bestMove.move.fromRow, bestMove.move.fromCol, bestMove.move.toRow, bestMove.move.toCol);
    std::cout << "Move executed on the game board." << std::endl;

    // Добавляем ход в историю
    chessGame->moveHistory.push_back({chessGame->board[bestMove.move.toRow][bestMove.move.toCol],
                                      bestMove.move.fromRow, bestMove.move.fromCol,
                                      bestMove.move.toRow, bestMove.move.toCol, 'B'});
    std::cout << "Move added to moveHistory." << std::endl;
}

void BotPlayer::botMoveCallback(void* data) {
    BotPlayer* bot = static_cast<BotPlayer*>(data);

    // Проверяем, существует ли chessBoard
    if (bot->chessBoard && bot->chessBoard->gameOver) {
        delete bot;
        return;
    }

    // Выполняем ход бота
    bot->performBotMove();

    // Если chessBoard существует, обновляем GUI
    if (bot->chessBoard) {
        // Обновляем доску после хода бота
        Fl::redraw();

        // Проверяем, находится ли белый король под шахом
        if (bot->chessGame->isInCheck('K')) {
            if (bot->chessGame->isInCheckmate('K')) {
                bot->chessBoard->gameOver = true;
                fl_message("Чёрные победили! Мат белому королю.");
                bot->chessBoard->updateMessage();
                bot->chessBoard->showCheckWindow(false); // Скрываем окно шаха
                delete bot;
                return;
            } else {
                bot->chessBoard->showCheckWindow(true); // Показываем окно шаха
            }
        } else {
            bot->chessBoard->showCheckWindow(false); // Скрываем окно шаха
        }

        // Разблокируем ход игрока
        bot->chessBoard->isPlayerTurn = true;
        bot->chessBoard->currentPlayer = 'W';
        bot->chessBoard->updateMessage();

        // Удаляем объект бота
        delete bot;
    }
}

// Рекурсивная функция minimax с альфа-бета отсечением
BotPlayer::BotMove BotPlayer::minimax(const GameState& state, int depth, int alpha, int beta, bool isMaximizingPlayer) {
    if (depth == 0 || isGameOver(state)) {
        int score = evaluateBoard(state);
        return {{-1, -1, -1, -1, ' '}, score};
    }

    char playerColor = isMaximizingPlayer ? 'B' : 'W';
    std::vector<Move> possibleMoves = generateAllPossibleMoves(state, playerColor);

    if (possibleMoves.empty()) {
        // Нет доступных ходов
        int score = evaluateBoard(state);
        return {{-1, -1, -1, -1, ' '}, score};
    }

    BotMove bestMove;
    if (isMaximizingPlayer) {
        bestMove.score = -1000000;
        for (const auto& move : possibleMoves) {
            // Создаём копию состояния игры для следующего хода
            GameState newState = state;

            // Выполняем ход на новой копии состояния
            makeMoveOnBoard(newState, move);

            // Проверяем, не оставили ли мы своего короля под шахом
            if (isInCheck(newState, 'k')) {
                // Недопустимый ход, пропускаем его
                continue;
            }

            // Рекурсивный вызов
            BotMove currentMove = minimax(newState, depth - 1, alpha, beta, false);

            if (currentMove.score > bestMove.score) {
                bestMove.move = move;
                bestMove.score = currentMove.score;
            }
            alpha = std::max(alpha, bestMove.score);
            if (beta <= alpha) {
                break; // Beta отсечение
            }
        }
    } else {
        bestMove.score = 1000000;
        for (const auto& move : possibleMoves) {
            // Создаём копию состояния игры для следующего хода
            GameState newState = state;

            // Выполняем ход на новой копии состояния
            makeMoveOnBoard(newState, move);

            // Проверяем, не оставили ли мы своего короля под шахом
            if (isInCheck(newState, 'K')) {
                // Недопустимый ход, пропускаем его
                continue;
            }

            // Рекурсивный вызов
            BotMove currentMove = minimax(newState, depth - 1, alpha, beta, true);

            if (currentMove.score < bestMove.score) {
                bestMove.move = move;
                bestMove.score = currentMove.score;
            }
            beta = std::min(beta, bestMove.score);
            if (beta <= alpha) {
                break; // Alpha отсечение
            }
        }
    }
    return bestMove;
}

// Функция для выполнения хода на виртуальной доске
void BotPlayer::makeMoveOnBoard(GameState& state, const Move& move) {
    char piece = state.board[move.fromRow][move.fromCol];
    char targetPiece = state.board[move.toRow][move.toCol];

    // Если на целевой клетке есть фигура противника, добавляем её в список захваченных
    if (targetPiece != '.') {
        if (targetPiece >= 'A' && targetPiece <= 'Z') {
            state.whiteCapturedPieces.push_back(targetPiece);
        } else {
            state.blackCapturedPieces.push_back(targetPiece);
        }
    }

    // Перемещаем фигуру
    state.board[move.toRow][move.toCol] = piece;
    state.board[move.fromRow][move.fromCol] = '.';

    // Обработка превращения пешки
    if (tolower(piece) == 'p') {
        if (piece == 'P' && move.toRow == 0) {
            state.board[move.toRow][move.toCol] = 'Q'; // Белая пешка превращается в ферзя
        } else if (piece == 'p' && move.toRow == 7) {
            state.board[move.toRow][move.toCol] = 'q'; // Чёрная пешка превращается в ферзя
        }
    }
}

// Функция оценки состояния доски
int BotPlayer::evaluateBoard(const GameState& state) {
    int score = 0;

    int pieceValues[256] = {0};
    pieceValues['P'] = -100;
    pieceValues['N'] = -320;
    pieceValues['B'] = -330;
    pieceValues['R'] = -500;
    pieceValues['Q'] = -900;
    pieceValues['K'] = -20000;
    pieceValues['p'] = 100;
    pieceValues['n'] = 320;
    pieceValues['b'] = 330;
    pieceValues['r'] = 500;
    pieceValues['q'] = 900;
    pieceValues['k'] = 20000;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            char piece = state.board[i][j];
            if (piece != '.') {
                score += pieceValues[static_cast<unsigned char>(piece)];
            }
        }
    }

    // Добавляем оценку за тактические мотивы
    score += evaluateTactics(state, 'B'); // Положительный счёт для тактик бота
    score -= evaluateTactics(state, 'W'); // Отрицательный счёт для тактик соперника

    return score;
}

// Функция оценки тактических мотивов
int BotPlayer::evaluateTactics(const GameState& state, char playerColor) {
    int score = 0;

    // Цвет соперника
    char opponentColor = (playerColor == 'W') ? 'B' : 'W';

    // Пример тактической оценки: контроль центра
    for (int i = 2; i <= 5; ++i) {
        for (int j = 2; j <= 5; ++j) {
            char piece = state.board[i][j];
            if (piece != '.') {
                if ((playerColor == 'W' && piece >= 'A' && piece <= 'Z') ||
                    (playerColor == 'B' && piece >= 'a' && piece <= 'z')) {
                    score += 10; // Увеличиваем счёт за контроль центра
                }
            }
        }
    }

    return score;
}

// Функция генерации всех возможных ходов для игрока
std::vector<Move> BotPlayer::generateAllPossibleMoves(const GameState& state, char playerColor) {
    std::vector<Move> possibleMoves;

    for (int fromRow = 0; fromRow < SIZE; ++fromRow) {
        for (int fromCol = 0; fromCol < SIZE; ++fromCol) {
            char piece = state.board[fromRow][fromCol];
            if (piece == '.') continue;

            // Проверяем, принадлежит ли фигура текущему игроку
            if ((playerColor == 'W' && piece >= 'A' && piece <= 'Z') ||
                (playerColor == 'B' && piece >= 'a' && piece <= 'z')) {

                // Получаем все возможные ходы для данной фигуры
                std::vector<Move> pieceMoves = getValidMovesForPiece(state, fromRow, fromCol, playerColor);
                possibleMoves.insert(possibleMoves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }

    return possibleMoves;
}

// Функция получения всех допустимых ходов для конкретной фигуры
std::vector<Move> BotPlayer::getValidMovesForPiece(const GameState& state, int fromRow, int fromCol, char playerColor) {
    std::vector<Move> validMoves;

    char piece = state.board[fromRow][fromCol];

    if (tolower(piece) == 'p') {
        // Логика для пешки
        int direction = (piece == 'P') ? -1 : 1;
        int startRow = (piece == 'P') ? 6 : 1;

        // Ход вперёд
        int toRow = fromRow + direction;
        if (toRow >= 0 && toRow < SIZE) {
            if (state.board[toRow][fromCol] == '.') {
                validMoves.push_back({piece, fromRow, fromCol, toRow, fromCol, playerColor});
                // Первый ход пешки на два поля
                if (fromRow == startRow) {
                    int toRow2 = toRow + direction;
                    if (toRow2 >= 0 && toRow2 < SIZE && state.board[toRow2][fromCol] == '.') {
                        validMoves.push_back({piece, fromRow, fromCol, toRow2, fromCol, playerColor});
                    }
                }
            }
            // Взятие фигур
            for (int dc = -1; dc <= 1; dc += 2) {
                int toCol = fromCol + dc;
                if (toCol >= 0 && toCol < SIZE) {
                    char targetPiece = state.board[toRow][toCol];
                    if (targetPiece != '.' && isOpponentPiece(piece, targetPiece)) {
                        validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                    }
                }
            }
        }
    }
    else if (tolower(piece) == 'n') {
        // Логика для коня
        int knightMoves[8][2] = {
                {-2, -1}, {-2, 1},
                {-1, -2}, {-1, 2},
                {1, -2},  {1, 2},
                {2, -1},  {2, 1}
        };
        for (auto& moveOffset : knightMoves) {
            int toRow = fromRow + moveOffset[0];
            int toCol = fromCol + moveOffset[1];
            if (toRow >= 0 && toRow < SIZE && toCol >= 0 && toCol < SIZE) {
                char targetPiece = state.board[toRow][toCol];
                if (targetPiece == '.' || isOpponentPiece(piece, targetPiece)) {
                    validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                }
            }
        }
    }
    else if (tolower(piece) == 'b') {
        // Логика для слона
        int directions[4][2] = {
                {-1, -1}, {-1, 1},
                {1, -1},  {1, 1}
        };
        for (auto& dir : directions) {
            int toRow = fromRow;
            int toCol = fromCol;
            while (true) {
                toRow += dir[0];
                toCol += dir[1];
                if (toRow < 0 || toRow >= SIZE || toCol < 0 || toCol >= SIZE) break;
                char targetPiece = state.board[toRow][toCol];
                if (targetPiece == '.') {
                    validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                } else {
                    if (isOpponentPiece(piece, targetPiece)) {
                        validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                    }
                    break;
                }
            }
        }
    }
    else if (tolower(piece) == 'r') {
        // Логика для ладьи
        int directions[4][2] = {
                {-1, 0}, {1, 0},
                {0, -1}, {0, 1}
        };
        for (auto& dir : directions) {
            int toRow = fromRow;
            int toCol = fromCol;
            while (true) {
                toRow += dir[0];
                toCol += dir[1];
                if (toRow < 0 || toRow >= SIZE || toCol < 0 || toCol >= SIZE) break;
                char targetPiece = state.board[toRow][toCol];
                if (targetPiece == '.') {
                    validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                } else {
                    if (isOpponentPiece(piece, targetPiece)) {
                        validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                    }
                    break;
                }
            }
        }
    }
    else if (tolower(piece) == 'q') {
        // Логика для ферзя (комбинация ладьи и слона)
        int directions[8][2] = {
                {-1, -1}, {-1, 0}, {-1, 1},
                {0, -1},          {0, 1},
                {1, -1},  {1, 0},  {1, 1}
        };
        for (auto& dir : directions) {
            int toRow = fromRow;
            int toCol = fromCol;
            while (true) {
                toRow += dir[0];
                toCol += dir[1];
                if (toRow < 0 || toRow >= SIZE || toCol < 0 || toCol >= SIZE) break;
                char targetPiece = state.board[toRow][toCol];
                if (targetPiece == '.') {
                    validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                } else {
                    if (isOpponentPiece(piece, targetPiece)) {
                        validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                    }
                    break;
                }
            }
        }
    }
    else if (tolower(piece) == 'k') {
        // Логика для короля
        int kingMoves[8][2] = {
                {-1, -1}, {-1, 0}, {-1, 1},
                {0, -1},          {0, 1},
                {1, -1},  {1, 0},  {1, 1}
        };
        for (auto& moveOffset : kingMoves) {
            int toRow = fromRow + moveOffset[0];
            int toCol = fromCol + moveOffset[1];
            if (toRow >= 0 && toRow < SIZE && toCol >= 0 && toCol < SIZE) {
                char targetPiece = state.board[toRow][toCol];
                if (targetPiece == '.' || isOpponentPiece(piece, targetPiece)) {
                    validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                }
            }
        }
        // Логика для рокировки можно добавить здесь
    }
    // Добавьте дополнительные условия для других фигур, если необходимо

    return validMoves;
}

// Функция проверки, является ли фигура противника
bool BotPlayer::isOpponentPiece(char piece, char targetPiece) {
    if ((piece >= 'A' && piece <= 'Z') && (targetPiece >= 'a' && targetPiece <= 'z')) {
        return true;
    }
    if ((piece >= 'a' && piece <= 'z') && (targetPiece >= 'A' && targetPiece <= 'Z')) {
        return true;
    }
    return false;
}

// Функция проверки, находится ли король под шахом
bool BotPlayer::isInCheck(const GameState& state, char kingChar) {
    int kingRow = -1, kingCol = -1;

    // Находим позицию короля
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (state.board[i][j] == kingChar) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1) break;
    }

    if (kingRow == -1) return true; // Король отсутствует, считается под шахом

    char opponentColor = (kingChar == 'K') ? 'B' : 'W';

    // Проверяем, атакуют ли короля фигуры противника
    std::vector<Move> opponentMoves = generateAllPossibleMoves(state, opponentColor);

    for (const auto& move : opponentMoves) {
        if (move.toRow == kingRow && move.toCol == kingCol) {
            return true; // Король под шахом
        }
    }

    return false; // Король не под шахом
}

// Функция проверки, закончилась ли игра
bool BotPlayer::isGameOver(const GameState& state) {
    // Проверяем, есть ли оба короля на доске
    bool whiteKingPresent = false;
    bool blackKingPresent = false;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (state.board[i][j] == 'K') whiteKingPresent = true;
            if (state.board[i][j] == 'k') blackKingPresent = true;
        }
    }
    return !(whiteKingPresent && blackKingPresent);
}

// Функция копирования состояния игры
void BotPlayer::copyGameState(const ChessGame& game, GameState& state) {
    // Копируем доску
    for (int i = 0; i < SIZE; ++i) {
        std::copy(game.board[i], game.board[i] + SIZE, state.board[i]);
    }
    // Копируем списки захваченных фигур
    state.whiteCapturedPieces = game.whiteCapturedPieces;
    state.blackCapturedPieces = game.blackCapturedPieces;
    // Дополнительно можно скопировать другие необходимые данные
    std::cout << "copyGameState: State copied successfully." << std::endl;
}