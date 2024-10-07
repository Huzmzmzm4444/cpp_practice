// A little guessing game called
//    (for some obscure reason) <<Bulls and Cows>>.

#include <std_lib_facilities.h>
#include <limits>

const int SIZE = 4;

int count(const vector<char>& digits, char d)
{
    int k{};

    for (int i = 0; i < digits.size(); ++i)
        if (digits[i] == d)
            ++k;

    return k;
}

int validate(const vector<char>& number)
{
    for (int i = 0; i < number.size(); ++i)
    {
        if (number[i] < '0' || '9' < number[i])
        {
            cout << "the number contains not a digit,try again" << endl;
            return 1;
        }

        if (count(number, number[i]) != 1)
        {
            cout << "digits of the number are not unique, try again" << endl;
            return 1;
        }
    }
    return -1;
}

vector<char> user_guess()
{
    vector<char> number(SIZE);

    int a = 1;

    while (a == 1)
    {
        cout << "guess the number: ";
        for (int i = 0; i < number.size(); ++i)
            cin >> number[i];

        string command;
        getline(cin, command);

        if (!cin)
        {
            cout << "invalid input, try again" << endl;
            cin.clear();
            for (int i = 0; i < number.size(); ++i)
                cin >> number[i];
        }
        
        else
        {
            a = validate(number);
        }
    }

    return number;
}


vector<char> computer_num()
{
    srand(time(NULL));


    vector<char> number(SIZE);
    for (char& i : number) 
    {
        cout << i << ' ';
    }
    for (int i = 0; i < 4; ++i)
    {
        char alfavit[] = "123456789";

        for (int i = 0; i < number.size(); ++i)
        {
            number[i] = alfavit[rand() % 9];
            while (count(number, number[i]) != 1)
                number[i] = alfavit[rand() % 9];
        }

    }
    return number;
}


int main()
{
    try
    {
        cout << "<<Bulls and Cows>>\n"
            << "Computer sets a number of 4 unique digits.\n"
            << "Try to guess it.\n"
            << "<Bull> means right digit in the right position.\n"
            << "<Cow> means right digit in the wrong position.\n"
            << "\n"
            << "game is on" << endl;



        int bulls{};


        do
        {
            bulls = 0;
            int cows{};
            vector<char> number = computer_num();
            vector<char> uguess = user_guess();


            for (int i = 0; i < SIZE; ++i)
            {
                cout << number[i] << " ";
            }
            cout << endl;


            for (int i = 0; i < uguess.size(); ++i)
            {
                if (uguess[i] == number[i])
                    ++bulls;
                else if (count(number, uguess[i]) == 1)
                    ++cows;
            }
         

            cout << bulls << " bull(s) and " << cows << " cow(s)" << endl;
        } while (bulls != SIZE);

        cout << "game is over" << endl;
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    catch (...)
    {
        cerr << "Oops, something went wrong..." << endl;
        return 1;
    }
}