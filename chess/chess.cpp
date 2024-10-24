
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SIZE = 8;
char board[SIZE][SIZE];

void initBoard() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (i == 1)
                board[i][j] = 'P'; // Пешка белых
            else if (i == 6)
                board[i][j] = 'p'; // Пешка черных
            else
                board[i][j] = '.'; // Пустые клетки
        }
    }
    board[0][0] = board[0][7] = 'R'; // Ладьи белых
    board[0][1] = board[0][6] = 'N'; // Конь белых
    board[0][2] = board[0][5] = 'B'; // Слоны белых
    board[0][3] = 'Q'; // Ферзь белых
    board[0][4] = 'K'; // Король белых

    board[7][0] = board[7][7] = 'r'; // Ладьи черных
    board[7][1] = board[7][6] = 'n'; // Конь черных
    board[7][2] = board[7][5] = 'b'; // Слоны черных
    board[7][3] = 'q'; // Ферзь черных
    board[7][4] = 'k'; // Король черных
}

void printBoard() {
    cout << "  0 1 2 3 4 5 6 7\n"; // Заголовок для столбцов
    for (int i = 0; i < SIZE; ++i) {
        cout << i << ' '; // Заголовок для строк
        for (int j = 0; j < SIZE; ++j) {
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
}

bool isPathBlocked(int fromRow, int fromCol, int toRow, int toCol) {
    int rowStep = (toRow - fromRow) == 0 ? 0 : (toRow - fromRow > 0 ? 1 : -1);
    int colStep = (toCol - fromCol) == 0 ? 0 : (toCol - fromCol > 0 ? 1 : -1);

    int currentRow = fromRow + rowStep;
    int currentCol = fromCol + colStep;

    while (currentRow != toRow || currentCol != toCol) {
        if (board[currentRow][currentCol] != '.')
            return true; // Путь заблокирован
        currentRow += rowStep;
        currentCol += colStep;
    }
    return false; // Путь свободен
}

bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (toRow < 0 || toRow >= SIZE || toCol < 0 || toCol >= SIZE)
        return false; // Вне границ
    if (board[toRow][toCol] != '.' &&
        ((board[fromRow][fromCol] <= 'Z' && board[toRow][toCol] <= 'Z') ||
            (board[fromRow][fromCol] >= 'a' && board[toRow][toCol] >= 'a')))
        return false; // Захват своей фигуры

    char piece = board[fromRow][fromCol];

    // Проверка правил для каждой фигуры
    switch (piece) {
    case 'P': // Пешка белых
        if (fromCol == toCol && toRow == fromRow + 1 && board[toRow][toCol] == '.')
            return true; // Обычный ход
        if (fromCol == toCol && fromRow == 1 && toRow == fromRow + 2 && board[toRow][toCol] == '.')
            return true; // Ход с начальной позиции
        if (abs(fromCol - toCol) == 1 && toRow == fromRow + 1 &&
            board[toRow][toCol] >= 'a' && board[toRow][toCol] <= 'z')
            return true; // Захват
        break;

    case 'p': // Пешка черных
        if (fromCol == toCol && toRow == fromRow - 1 && board[toRow][toCol] == '.')
            return true; // Обычный ход
        if (fromCol == toCol && fromRow == 6 && toRow == fromRow - 2 && board[toRow][toCol] == '.')
            return true; // Ход с начальной позиции
        if (abs(fromCol - toCol) == 1 && toRow == fromRow - 1 &&
            board[toRow][toCol] >= 'A' && board[toRow][toCol] <= 'Z')
            return true; // Захват
        break;

    case 'N': // Конь белых
    case 'n': // Конь черных
        if ((abs(fromRow - toRow) == 2 && abs(fromCol - toCol) == 1) ||
            (abs(fromRow - toRow) == 1 && abs(fromCol - toCol) == 2))
            return true;
        break;

    case 'B': // Слон белых
    case 'b': // Слон черных
        if (abs(fromRow - toRow) == abs(fromCol - toCol) && !isPathBlocked(fromRow, fromCol, toRow, toCol))
            return true;
        break;

    case 'R': // Ладья белых
    case 'r': // Ладья черных
        if ((fromRow == toRow || fromCol == toCol) && !isPathBlocked(fromRow, fromCol, toRow, toCol))
            return true;
        break;

    case 'Q': // Ферзь белых
    case 'q': // Ферзь черных
        if ((abs(fromRow - toRow) == abs(fromCol - toCol) || fromRow == toRow || fromCol == toCol) &&
            !isPathBlocked(fromRow, fromCol, toRow, toCol))
            return true;
        break;

    case 'K': // Король белых
    case 'k': // Король черных
        if (abs(fromRow - toRow) <= 1 && abs(fromCol - toCol) <= 1)
            return true;
        break;
    }
    return false;
}

void botMove() {
    this_thread::sleep_for(chrono::seconds(rand() % 30 + 30)); // Задержка от 30 до 60 секунд
    int fromRow, fromCol, toRow, toCol;

    do {
        fromRow = rand() % SIZE;
        fromCol = rand() % SIZE;
        toRow = rand() % SIZE;
        toCol = rand() % SIZE;
    } while (!isValidMove(fromRow, fromCol, toRow, toCol) ||
        board[fromRow][fromCol] == '.' ||
        (board[fromRow][fromCol] < 'a' || board[fromRow][fromCol] > 'z')); // Не позволяем боту двигать чужие фигуры

    cout << "Bot moved from (" << fromRow << "," << fromCol << ") to (" << toRow << "," << toCol << ")" << endl;
    board[toRow][toCol] = board[fromRow][fromCol]; // Перемещение
    board[fromRow][fromCol] = '.'; // Удаление с начальной позиции
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    initBoard();
    printBoard();

    while (true) {
        int fromRow, fromCol, toRow, toCol;
        cout << "Enter your move in format: fromRow fromCol toRow toCol (0-7): ";
        cin >> fromRow >> fromCol >> toRow >> toCol;

        // Проверка на принадлежность фигуры игрока
        if (board[fromRow][fromCol] < 'A' || board[fromRow][fromCol] > 'Z') {
            cerr << "You can only move your pieces!" << endl;
            continue;
        }

        // Проверка на корректность хода
        if (!isValidMove(fromRow, fromCol, toRow, toCol) || board[fromRow][fromCol] == '.') {
            cerr << "Invalid move! Please try again." << endl;
            continue; // Повторяем ввод, если ход неправильный
        }

        // Выполнение хода игрока
        board[toRow][toCol] = board[fromRow][fromCol]; // Перемещение
        board[fromRow][fromCol] = '.'; // Удаление с начальной позиции
        printBoard();

        // Ход бота
        botMove();
        printBoard();
    }

    return 0;
}
