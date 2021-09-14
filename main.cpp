#include <iostream>
#include <vector>
#include <iomanip>

enum cell
{
    blank,
    X,
    O
};

class State
{

private:
    static std::vector<std::pair<State, int>> library;
    std::vector<cell>
        data;
    int turn = 1;

    static bool sortbysecasc(std::pair<int, int> &a, std::pair<int, int> &b)
    {
        return a.second < b.second;
    }

    bool scanrow(int x, cell c)
    {
        int result = 0;
        for (int i = x; i < x + 3; i++)
            result += data[i] == c;
        if (result == 3)
            return 1;
        else
            return 0;
    }
    bool scancollum(int x, cell c)
    {
        int result = 0;
        for (int i = x; i < x + 7; i += 3)
            result += data[i] == c;
        if (result == 3)
            return 1;
        else
            return 0;
    }
    bool scandiags(cell c)
    {
        int result = 0, ret = 0;
        for (int i = 0; i < 9; i += 4)
            result += data[i] == c;
        if (result == 3)
            ret++;
        result = 0;
        for (int i = 2; i < 7; i += 2)
            result += data[i] == c;
        if (result == 3)
            ret++;
        if (ret == 0)
            return 0;
        else
            return 1;
    }

    static int minimax(State s, bool b)
    {
        if (s.winning(X))
            return 10;
        if (s.winning(O))
            return -10;
        if (s.tie())
            return 0;
        int max = -20, min = 20;
        for (int x = 0; x < 9; x++)
        {
            if (s.data[x] != blank)
                continue;
            State tempstate = s;
            tempstate.move(x);
            int score;
            bool done = 0;
            if (library.size() > 0)
            {
                for (auto x : library)
                {
                    if (x.first == tempstate)
                    {
                        score = x.second;
                        done = 1;
                        break;
                    }
                }
            }
            if (!done)
            {
                score = minimax(tempstate, !b);
                library.push_back(std::make_pair(tempstate, score));
            }
            if (score > max)
                max = score;
            if (score < min)
                min = score;
        }
        if (b)
            return max;
        else
            return min;
    }

    void move(int i)
    {
        cell c;
        if (turn % 2 == 1)
            c = X;
        else
            c = O;
        data[i] = c;
        turn++;
    }
    void next()
    {
        if (turn == 1)
        {
            move(0);
            return;
        }
        bool b = turn % 2 == 1;
        int i, max = -20, min = 20, mini, maxi;
        for (int x = 0; x < 9; x++)
        {
            if (data[x] != blank)
                continue;
            State tempstate = *this;
            tempstate.move(x);
            i = minimax(tempstate, !b);
            if (i > max)
            {
                max = i;
                maxi = x;
            }
            if (i < min)
            {
                min = i;
                mini = x;
            }
        }

        if (b)
            move(maxi);
        else
            move(mini);
    }

public:
    State()
    {
        data.resize(9, blank);
    }
    bool winning(cell c)
    {
        if (turn < 5)
            return 0;
        for (int i = 0; i < 7; i += 3)
            if (scanrow(i, c) == 1)
                return 1;
        for (int i = 0; i < 3; i++)
            if (scancollum(i, c) == 1)
                return 1;
        return scandiags(c);
    }
    bool tie()
    {
        return turn >= 10;
    }

    void play(cell c)
    {
        int playersmove = 0;
        if (c == O)
        {
            std::cout << *this;
            std::cout << "Your turn, choose an available spot\n";
            std::cin >> playersmove;
            while (playersmove <= 0 || playersmove > 9 || data[playersmove - 1] != blank)
            {
                std::cout << "Type a valid move please\n";
                std::cin >> playersmove;
            }
            move(playersmove - 1);
        }

        std::cout << "Let me think for a bit...\n";
        while (!winning(X) && !winning(O) && !tie())
        {
            next();
            std::cout << *this;
            if (!winning(X) && !winning(O) && !tie())
            {
                std::cout << "Your turn, choose an available spot\n";
                std::cin >> playersmove;
                while (playersmove <= 0 || playersmove > 9 || data[playersmove - 1] != blank)
                {
                    std::cout << "Type a valid move please\n";
                    std::cin >> playersmove;
                }
                move(playersmove - 1);
            }
        }
        std::cout << "\nGame Over\n";
        std::cout << *this;
        if (winning(X))
            if (c == X)
                std::cout << "You lose X wins!";
            else
                std::cout << "You win X wins!";
        else if (winning(O))
            if (c == X)
                std::cout << "You win O wins!";
            else
                std::cout << "You lose O wins!";
        else
            std::cout << "Its a tie!";
    }

    friend bool operator==(const State &a, const State &b)
    {
        if (a.turn != b.turn)
            return 0;
        for (int i = 0; i < 9; i++)
            if (a.data[i] != b.data[i])
                return 0;

        return 1;
    }

    friend bool operator!=(const State &a, const State &b)
    {
        return !(a == b);
    }

    friend std::ostream &operator<<(std::ostream &out, const State &s)
    {
        for (int i = 0; i < s.data.size(); i++)
        {
            switch (s.data[i])
            {
            case blank:
            {
                out << std::setw(3) << i + 1;
                if (!((i + 1) % 3 == 0))
                    out << std::setw(3) << '|';
                break;
            }
            case X:
            {
                out << std::setw(3) << 'X';
                if (!((i + 1) % 3 == 0))
                    out << std::setw(3) << '|';

                break;
            }
            case O:
            {
                out << std::setw(3) << 'O';
                if (!((i + 1) % 3 == 0))
                    out << std::setw(3) << '|';

                break;
            }
            }

            if ((i + 1) % 3 == 0)
            {
                out << '\n';
                if (i != 8)
                    out << "-----|-----|-----\n";
            }
        }
        return out;
    }
};

std::vector<std::pair<State, int>> State::library;

int main()
{
    State s;
    std ::cout << "Hello there!!\n"
               << "lets play tic tac toe\n"
               << "do you want to play as X or O\n"
               << "remember X goes first\n";
    char a = 'x';
    cell c = X;
    std::cin >> a;
    while (a != 'x' && a != 'o')
    {
        std::cout << "Type either x or o please\n";
        std::cin >> a;
    }
    if (a == 'o')
    {
        c = X;
        std::cout << "You are O\n";
    }
    else
    {
        c = O;
        std::cout << "You are X\n";
    }
    s.play(c);
    return 0;
}
