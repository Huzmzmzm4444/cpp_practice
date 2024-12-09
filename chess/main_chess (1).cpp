// main.cpp

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include "frontend.h"
#include "backend.h"
#include <locale>

// Функция-обработчик для кнопок меню
void startGame(Fl_Widget* widget, void* data) {
    GameMode mode = *(GameMode*)data;

    // Создаём окно игры
    Fl_Window* window = new Fl_Window(900, 600, "Chess Game");

    // Создаём Fl_Box для отображения сообщения
    Fl_Box* messageBox = new Fl_Box(0, 0, 900, 50, "Ход белых");
    messageBox->labelsize(24);
    messageBox->labelfont(FL_BOLD);
    messageBox->labeltype(FL_NORMAL_LABEL);
    messageBox->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);

    // Создаём шахматную доску
    ChessBoard* chessBoard = new ChessBoard(0, 50, 900, 550, messageBox, mode);

    window->end();
    window->show();

    // Закрываем окно меню
    Fl_Window* menuWindow = widget->window();
    menuWindow->hide();
}

// Главная функция
int main() {
    // Инициализация генератора случайных чисел (если необходимо)
    srand(static_cast<unsigned>(time(0)));

    // Устанавливаем глобальную локаль для поддержки кириллицы
    std::locale::global(std::locale(""));

    // Создаём окно меню
    Fl_Window* menuWindow = new Fl_Window(300, 200, "Выбор режима игры");

    // Создаём кнопку "Против компьютера"
    Fl_Button* btnAgainstComputer = new Fl_Button(50, 50, 200, 40, "Против компьютера");
    btnAgainstComputer->labelsize(16);
    btnAgainstComputer->labelfont(FL_BOLD);

    // Создаём кнопку "Против друга"
    Fl_Button* btnAgainstFriend = new Fl_Button(50, 110, 200, 40, "Против друга");
    btnAgainstFriend->labelsize(16);
    btnAgainstFriend->labelfont(FL_BOLD);

    // Устанавливаем колбэки для кнопок
    GameMode modeAgainstComputer = AGAINST_COMPUTER;
    btnAgainstComputer->callback(startGame, &modeAgainstComputer);

    GameMode modeAgainstFriend = AGAINST_FRIEND;
    btnAgainstFriend->callback(startGame, &modeAgainstFriend);

    menuWindow->end();
    menuWindow->show();

    // Запуск основного цикла FLTK
    return Fl::run();
}