#include "backend.h"
#include <cmath>
#include <algorithm>
#include <vector>

// Помощная функция для проверки, атаковано ли поле фигурами противника.
// Можно использовать логику из isInCheck, но подставить нашу цель.
bool ChessGame::isSquareAttacked(int row, int col, char opponentColor, const char boardState[SIZE][SIZE]) {
    // Проходим по всем фигурам противника и проверяем, могут ли они пойти на (row, col)
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            char p = boardState[r][c];
            if (p == '.') continue;

            if ((opponentColor == 'W' && p >= 'A' && p <= 'Z') ||
                (opponentColor == 'B' && p >= 'a' && p <= 'z')) {
                // Проверяем, легален ли ход этой фигуры на (row, col)
                // ignoreCheck=true, чтобы не зациклиться
                char pcColor = (p >= 'A' && p <= 'Z') ? 'W' : 'B';
                if (isValidMove(r, c, row, col, pcColor, true, boardState)) {
                    return true;
                }
            }
        }
    }
    return false;
}

ChessGame::ChessGame(GameMode mode) : gameMode(mode) {
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteRookMoved[0] = whiteRookMoved[1] = false;
    blackRookMoved[0] = blackRookMoved[1] = false;
    enPassantTargetRow = -1;
    enPassantTargetCol = -1;
    currentPlayer = 'W';
    initializeBoard();
}

void ChessGame::initializeBoard() {
    for (int i = 0; i < SIZE; ++i) {
        std::fill(board[i], board[i] + SIZE, '.');
    }

    // Белые фигуры
    board[7][0] = 'R'; board[7][1] = 'N'; board[7][2] = 'B'; board[7][3] = 'Q'; board[7][4] = 'K'; board[7][5] = 'B'; board[7][6] = 'N'; board[7][7] = 'R';
    for (int i = 0; i < SIZE; ++i) board[6][i] = 'P';

    // Чёрные фигуры
    board[0][0] = 'r'; board[0][1] = 'n'; board[0][2] = 'b'; board[0][3] = 'q'; board[0][4] = 'k'; board[0][5] = 'b'; board[0][6] = 'n'; board[0][7] = 'r';
    for (int i = 0; i < SIZE; ++i) board[1][i] = 'p';

    whiteCapturedPieces.clear();
    blackCapturedPieces.clear();
    moveHistory.clear();
}

void ChessGame::copyBoard(const char srcBoard[SIZE][SIZE], char destBoard[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        std::copy(srcBoard[i], srcBoard[i] + SIZE, destBoard[i]);
    }
}

bool ChessGame::isInCheck(char kingChar, const char customBoard[SIZE][SIZE]) {
    const char (*currentBoard)[SIZE] = (customBoard != nullptr) ? customBoard : board;
    int kingRow = -1, kingCol = -1;

    for (int i = 0; i < SIZE && kingRow == -1; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (currentBoard[i][j] == kingChar) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
    }

    if (kingRow == -1) {
        // Король отсутствует
        return true;
    }

    char opponentColor = (kingChar == 'K') ? 'B' : 'W';
    return isSquareAttacked(kingRow, kingCol, opponentColor, currentBoard);
}

bool ChessGame::isKingPresent(char kingChar) {
    for (int i = 0; i < SIZE; ++i) {
        if (std::find(board[i], board[i] + SIZE, kingChar) != board[i] + SIZE) {
            return true;
        }
    }
    return false;
}

bool ChessGame::isInCheckmate(char kingChar) {
    if (!isInCheck(kingChar)) {
        return false;
    }

    char playerColor = (kingChar == 'K') ? 'W' : 'B';
    // Ищем ход, спасающий от мата
    for (int fromRow = 0; fromRow < SIZE; ++fromRow) {
        for (int fromCol = 0; fromCol < SIZE; ++fromCol) {
            char piece = board[fromRow][fromCol];
            if (piece == '.') continue;

            if ((playerColor == 'W' && piece >= 'A' && piece <= 'Z') ||
                (playerColor == 'B' && piece >= 'a' && piece <= 'z')) {
                for (int toRow = 0; toRow < SIZE; ++toRow) {
                    for (int toCol = 0; toCol < SIZE; ++toCol) {
                        if (isValidMove(fromRow, fromCol, toRow, toCol, playerColor)) {
                            // Есть ход - нет мата
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool ChessGame::isStalemate(char kingChar) {
    // Можно реализовать аналогично isInCheckmate, но проверяя отсутствие шаха
    // Опустим детально, сейчас оставим false
    return false;
}

bool ChessGame::isValidMove(int fromRow, int fromCol, int toRow, int toCol, char playerColor, bool ignoreCheck, const char customBoard[SIZE][SIZE]) {
    const char (*currentBoard)[SIZE] = (customBoard != nullptr) ? customBoard : this->board;

    if (fromRow < 0 || fromRow >= SIZE || fromCol < 0 || fromCol >= SIZE ||
        toRow < 0 || toRow >= SIZE || toCol < 0 || toCol >= SIZE) return false;

    char piece = currentBoard[fromRow][fromCol];
    if (piece == '.') return false;
    if (playerColor=='W' && !(piece>='A' && piece<='Z')) return false;
    if (playerColor=='B' && !(piece>='a' && piece<='z')) return false;

    char targetPiece = currentBoard[toRow][toCol];
    if (targetPiece != '.') {
        if (playerColor=='W' && (targetPiece>='A' && targetPiece<='Z')) return false;
        if (playerColor=='B' && (targetPiece>='a' && targetPiece<='z')) return false;
    }

    // Проверка на особый ход: рокировка
    if ((piece=='K' && playerColor=='W' && fromRow==7 && fromCol==4 && toRow==7 && (toCol==6||toCol==2))) {
        // Белая рокировка
        if (whiteKingMoved) return false;
        if (toCol==6) {
            // Короткая
            if (whiteRookMoved[1]) return false;
            // Проверяем пустые клетки и атаки
            if (currentBoard[7][5]!='.' || currentBoard[7][6]!='.') return false;

            // Проверяем, не атакованы ли клетки [7][4],[7][5],[7][6]
            // [7][4] - старт короля, если он под шахом, уже invalidMove
            // [7][5] и [7][6] нельзя чтобы были атакованы
            char oppColor='B';
            char temp[SIZE][SIZE];
            copyBoard(currentBoard,temp);
            // Проверяем атаки на промежуточные клетки
            if (isSquareAttacked(7,4,oppColor,temp)) return false; // Король не должен начинать под шахом
            if (isSquareAttacked(7,5,oppColor,temp)) return false;
            if (isSquareAttacked(7,6,oppColor,temp)) return false;

            // Допустима рокировка
        } else {
            // Длинная рокировка
            if (whiteRookMoved[0]) return false;
            // Проверяем пустые
            if (currentBoard[7][1]!='.' || currentBoard[7][2]!='.'||currentBoard[7][3]!='.') return false;

            char oppColor='B';
            char temp[SIZE][SIZE];
            copyBoard(currentBoard,temp);
            if (isSquareAttacked(7,4,oppColor,temp)) return false;
            if (isSquareAttacked(7,3,oppColor,temp)) return false;
            if (isSquareAttacked(7,2,oppColor,temp)) return false;

            // Допустимо
        }
        // Если дошли сюда, рокировка допустима
        if (!ignoreCheck) {
            // Проверим, что после хода не остается шаха (уже проверили выше)
        }
        return true;
    }

    // Чёрная рокировка
    if ((piece=='k' && playerColor=='B' && fromRow==0 && fromCol==4 && toRow==0 && (toCol==6||toCol==2))) {
        if (blackKingMoved) return false;
        char oppColor='W';
        char temp[SIZE][SIZE];
        copyBoard(currentBoard,temp);

        if (toCol==6) {
            // Короткая чёрная
            if (blackRookMoved[1]) return false;
            if (currentBoard[0][5]!='.'||currentBoard[0][6]!='.') return false;
            if (isSquareAttacked(0,4,oppColor,temp)) return false;
            if (isSquareAttacked(0,5,oppColor,temp)) return false;
            if (isSquareAttacked(0,6,oppColor,temp)) return false;
        } else {
            // Длинная чёрная
            if (blackRookMoved[0]) return false;
            if (currentBoard[0][1]!='.'||currentBoard[0][2]!='.'||currentBoard[0][3]!='.') return false;
            if (isSquareAttacked(0,4,oppColor,temp)) return false;
            if (isSquareAttacked(0,3,oppColor,temp)) return false;
            if (isSquareAttacked(0,2,oppColor,temp)) return false;
        }
        return true;
    }

    // Проверяем обычный ход фигуры
    switch (std::tolower(piece)) {
        case 'p':
            if (!isValidPawnMove(fromRow, fromCol, toRow, toCol, playerColor, currentBoard)) return false;
            break;
        case 'r':
            if (!isValidRookMove(fromRow, fromCol, toRow, toCol, currentBoard)) return false;
            break;
        case 'n':
            if (!isValidKnightMove(fromRow, fromCol, toRow, toCol)) return false;
            break;
        case 'b':
            if (!isValidBishopMove(fromRow, fromCol, toRow, toCol, currentBoard)) return false;
            break;
        case 'q':
            if (!isValidQueenMove(fromRow, fromCol, toRow, toCol, currentBoard)) return false;
            break;
        case 'k':
            // Обычный ход короля (не рокировка)
            if (!isValidKingMove(fromRow, fromCol, toRow, toCol, currentBoard)) return false;
            break;
        default:
            return false;
    }

    // Проверка на шах, если ignoreCheck=false
    if (!ignoreCheck) {
        char tempBoard[SIZE][SIZE];
        copyBoard(currentBoard,tempBoard);
        tempBoard[toRow][toCol]=piece;
        tempBoard[fromRow][fromCol]='.';
        char kingChar=(playerColor=='W')?'K':'k';
        if (isInCheck(kingChar,tempBoard)) return false;
    }

    return true;
}

bool ChessGame::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    char piece = board[fromRow][fromCol];
    if (piece == '.') return false;
    char playerColor = (piece >= 'A' && piece <= 'Z') ? 'W' : 'B';

    // Если это рокировка, после isValidMove уже всё проверено.
    if (piece == 'K' && playerColor=='W' && fromRow==7 && fromCol==4 && toRow==7 && (toCol==6||toCol==2)) {
        bool shortCastling = (toCol==6);
        board[7][4]='.';
        board[7][toCol]='K';
        if (shortCastling) {
            board[7][7]='.';
            board[7][5]='R';
            whiteRookMoved[1]=true;
        } else {
            board[7][0]='.';
            board[7][3]='R';
            whiteRookMoved[0]=true;
        }
        whiteKingMoved=true;
        moveHistory.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
        currentPlayer=(currentPlayer=='W')?'B':'W';
        return true;
    }

    if (piece=='k' && playerColor=='B' && fromRow==0 && fromCol==4 && toRow==0 && (toCol==6||toCol==2)) {
        bool shortCastling=(toCol==6);
        board[0][4]='.';
        board[0][toCol]='k';
        if (shortCastling) {
            board[0][7]='.';
            board[0][5]='r';
            blackRookMoved[1]=true;
        } else {
            board[0][0]='.';
            board[0][3]='r';
            blackRookMoved[0]=true;
        }
        blackKingMoved=true;
        moveHistory.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
        currentPlayer=(currentPlayer=='W')?'B':'W';
        return true;
    }

    // Взятие на проходе: если ход валиден, isValidMove учтёт взятие на проходе.
    // Но нам нужно выполнить само взятие.
    if ((piece=='P' || piece=='p') && abs(fromCol - toCol)==1 && board[toRow][toCol]=='.') {
        // Проверим, совпадает ли это с en passant
        if (playerColor=='W' && fromRow==3 && toRow==2 && toRow==enPassantTargetRow && toCol==enPassantTargetCol) {
            // Белое взятие на проходе
            board[toRow][toCol]=piece;
            board[fromRow][fromCol]='.';
            board[toRow+1][toCol]='.';
            moveHistory.push_back({piece,fromRow,fromCol,toRow,toCol,playerColor});
            currentPlayer=(currentPlayer=='W')?'B':'W';
            enPassantTargetRow=-1;enPassantTargetCol=-1;
            return true;
        }
        if (playerColor=='B' && fromRow==4 && toRow==5 && toRow==enPassantTargetRow && toCol==enPassantTargetCol) {
            // Чёрное взятие на проходе
            board[toRow][toCol]=piece;
            board[fromRow][fromCol]='.';
            board[toRow-1][toCol]='.';
            moveHistory.push_back({piece,fromRow,fromCol,toRow,toCol,playerColor});
            currentPlayer=(currentPlayer=='W')?'B':'W';
            enPassantTargetRow=-1;enPassantTargetCol=-1;
            return true;
        }
    }

    // Обычный ход
    char captured=board[toRow][toCol];
    if (captured!='.') {
        if (playerColor=='W') blackCapturedPieces.push_back(captured);
        else whiteCapturedPieces.push_back(captured);
    }

    board[toRow][toCol]=piece;
    board[fromRow][fromCol]='.';

    // Обновляем флаги
    if (piece=='K') whiteKingMoved=true;
    if (piece=='k') blackKingMoved=true;
    if (piece=='R' && fromRow==7 && fromCol==0) whiteRookMoved[0]=true;
    if (piece=='R' && fromRow==7 && fromCol==7) whiteRookMoved[1]=true;
    if (piece=='r' && fromRow==0 && fromCol==0) blackRookMoved[0]=true;
    if (piece=='r' && fromRow==0 && fromCol==7) blackRookMoved[1]=true;

    // Обновляем enPassant
    if (piece=='P' && fromRow==6 && toRow==4 && fromCol==toCol && captured=='.') {
        enPassantTargetRow=5;enPassantTargetCol=toCol;
    } else if (piece=='p' && fromRow==1 && toRow==3 && fromCol==toCol && captured=='.') {
        enPassantTargetRow=2;enPassantTargetCol=toCol;
    } else {
        enPassantTargetRow=-1;enPassantTargetCol=-1;
    }

    // Превращение пешки
    if (piece=='P' && toRow==0) {
        board[toRow][toCol]='Q';
    }
    if (piece=='p' && toRow==7) {
        board[toRow][toCol]='q';
    }

    moveHistory.push_back({piece,fromRow,fromCol,toRow,toCol,playerColor});
    currentPlayer=(currentPlayer=='W')?'B':'W';

    return true;
}

bool ChessGame::isValidPawnMove(int fromRow,int fromCol,int toRow,int toCol,char playerColor,const char currentBoard[SIZE][SIZE]) {
    int direction = (playerColor=='W')?-1:1;
    char oppStart=(playerColor=='W')?'a':'A';
    char oppEnd=(playerColor=='W')?'z':'Z';

    char target = currentBoard[toRow][toCol];

    // Пешка идёт по вертикали вперёд
    if (fromCol==toCol) {
        // Одна клетка вперёд
        if ((toRow-fromRow)==direction && target=='.') return true;
        // Две клетки с начальной позиции
        if ((playerColor=='W'&&fromRow==6)||(playerColor=='B'&&fromRow==1)) {
            if ((toRow - fromRow)==2*direction && target=='.' && currentBoard[fromRow+direction][fromCol]=='.') {
                return true;
            }
        }
    } else if (abs(toCol - fromCol)==1 && (toRow - fromRow)==direction) {
        // Диагональный ход
        if (target!='.' && ((playerColor=='W'&&target>=oppStart&&target<=oppEnd)||(playerColor=='B'&&target>='A'&&target<='Z'))) {
            return true; // Обычное взятие
        }
        // Проверяем взятие на проходе
        if (target=='.') {
            // Взятие на проходе возможно, если (toRow,toCol) соответствует enPassantTarget
            if (toRow==enPassantTargetRow && toCol==enPassantTargetCol) {
                return true;
            }
        }
    }

    return false;
}

bool ChessGame::isValidRookMove(int fromRow,int fromCol,int toRow,int toCol,const char currentBoard[SIZE][SIZE]) {
    if (fromRow!=toRow && fromCol!=toCol) return false;

    int rStep=(toRow>fromRow)?1:((toRow<fromRow)?-1:0);
    int cStep=(toCol>fromCol)?1:((toCol<fromCol)?-1:0);

    int r=fromRow+rStep,c=fromCol+cStep;
    while (r!=toRow || c!=toCol) {
        if (currentBoard[r][c]!='.') return false;
        r+=rStep; c+=cStep;
    }
    return true;
}

bool ChessGame::isValidKnightMove(int fromRow,int fromCol,int toRow,int toCol) {
    int dr=abs(toRow-fromRow), dc=abs(toCol-fromCol);
    return (dr==2&&dc==1)||(dr==1&&dc==2);
}

bool ChessGame::isValidBishopMove(int fromRow,int fromCol,int toRow,int toCol,const char currentBoard[SIZE][SIZE]) {
    if (abs(toRow-fromRow)!=abs(toCol-fromCol)) return false;
    int rStep=(toRow>fromRow)?1:-1;
    int cStep=(toCol>fromCol)?1:-1;
    int r=fromRow+rStep,c=fromCol+cStep;
    while (r!=toRow && c!=toCol) {
        if (currentBoard[r][c]!='.') return false;
        r+=rStep;c+=cStep;
    }
    return true;
}

bool ChessGame::isValidQueenMove(int fromRow,int fromCol,int toRow,int toCol,const char currentBoard[SIZE][SIZE]) {
    return isValidRookMove(fromRow,fromCol,toRow,toCol,currentBoard)||
           isValidBishopMove(fromRow,fromCol,toRow,toCol,currentBoard);
}

bool ChessGame::isValidKingMove(int fromRow,int fromCol,int toRow,int toCol,const char currentBoard[SIZE][SIZE]) {
    int dRow=abs(toRow-fromRow), dCol=abs(toCol-fromCol);
    // Обычный ход короля: 1 клетка в любую сторону
    if (dRow<=1 && dCol<=1) return true;
    // Если король двигается на 2 клетки по горизонтали - это либо ошибка, либо рокировка,
    // но рокировку мы проверяем отдельно в isValidMove() до этого блока.
    return false;
}

void ChessGame::calculatePossibleMoves(int fromRow,int fromCol,char playerColor,std::vector<std::pair<int,int>>& moves) {
    moves.clear();
    for (int tr=0; tr<SIZE; tr++) {
        for (int tc=0; tc<SIZE; tc++) {
            if (isValidMove(fromRow, fromCol, tr, tc, playerColor)) {
                moves.push_back({tr,tc});
            }
        }
    }
}

void ChessGame::findKingPosition(char kingChar,int &kingRow,int &kingCol) {
    kingRow=-1;kingCol=-1;
    for (int i=0;i<SIZE && kingRow==-1;i++) {
        for (int j=0;j<SIZE;j++) {
            if (board[i][j]==kingChar) {kingRow=i;kingCol=j;break;}
        }
    }
}