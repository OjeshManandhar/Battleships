#include <iostream>
#include <cstring>
#include <conio.h>
#include <windows.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

#define FIELD_LENGTH 10
#define FIELD_BREADTH 10

#define SCR_LENGTH 30
#define SCR_BREADTH 120

#define LENGTH (2 * FIELD_LENGTH + 1)
#define BREADTH (4 * FIELD_BREADTH + 1)

#define ROW_POS(length) (SCR_LENGTH - length)/2
#define COL_POS(breadth) (SCR_BREADTH - breadth)/2

#define TO_ROW(i) (2*i + 1)
#define TO_COL(j) (4*j + 2)
#define TO_I(row) ((row - 1)/2)
#define TO_J(col) ((col - 2)/4)

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define SPACE 32
#define ENTER 13
#define ESC 27

using namespace std;

enum ship_status
{
    floating, shunk
};

enum turn_detail
{
    users, comps
};

//Structures and Classes Declaration
struct position_detail;
class Ships;
class BattleField;

//Function Declaration
//Backend Functions
int getkey();
void gotoxy(const short int, const short int);
void print_box(const short int, const short int);
void print_detail(const char *);
int display_menu(char **, const int, unsigned int);

//Main Functions
void play();
void battle(BattleField &);
uint8_t user_attacks(BattleField &, BattleField &, uint8_t &);
uint8_t comp_attakcs(BattleField &, BattleField &, uint8_t &);

//Structures and Classes Definition
struct position_detail
{
    unsigned short int row, col;
}top, bot;

class Ships
{
private:
    uint8_t hits, s;
    ship_status status;
public:
    Ships()
    {
        hits = 0;
        status = floating;
    }

    void set_size(uint8_t n)
    {
        s = n;
    }

    void hit()
    {
        hits++;
        if (hits == s)
            status = shunk;
    }

    uint8_t return_size() const
    {
        return s;
    }

    ship_status return_status() const
    {
        return status;
    }

    void display_ship(position_detail pos, char align) const
    {
        uint8_t i;

        if (align == 'H')
            for (i = pos.col; i < (pos.col + s); i++)
            {
                gotoxy(top.row + TO_ROW(pos.row), top.col + TO_COL(i));
                cout << 'S';
            }
        else
            for (i = pos.row; i < (pos.row + s); i++)
            {
                gotoxy(top.row + TO_ROW(i), top.col + TO_COL(pos.col));
                cout << 'S';
            }
    }
};

class BattleField
{
private:
    char field[FIELD_LENGTH][FIELD_BREADTH];
    Ships ship[5];
public:
    BattleField()
    {
        uint8_t i, row, col;

        for (row = 0; row < FIELD_LENGTH; row++)
            for (col = 0; col < FIELD_BREADTH; col++)
                field[row][col] = '0';

        for (i = 0; i < 5; i++)
            if (i == 0)
                ship[i].set_size(2);
            else
                ship[i].set_size(i + 1);
    }

    char find_random_pos(position_detail &pos)
    {
        srand(time(0));

        pos.row = rand()%10;
        pos.col = rand()%10;

        if (rand()%2 == 1)
            return 'H';
        else
            return 'V';
    }

    void set_data(position_detail pos, char ch)
    {
        position_detail temp;
        char c = field[pos.row][pos.col];
        char x;

        if (c == '0')
            field[pos.row][pos.col] = '.';
        else
        {
            ship[c - '0' - 1].hit();
            field[pos.row][pos.col] += 16;
            if (ship[c - '0' - 1].return_status() == shunk)
            {
                x = field[pos.row][pos.col];
                for (temp.row = 0; temp.row < FIELD_LENGTH; temp.row++)
                    for (temp.col = 0; temp.col < FIELD_BREADTH; temp.col++)
                        if (field[temp.row][temp.col] == x)
                            field[temp.row][temp.col] = '#';
            }
        }
    }

    char return_data(position_detail pos) const
    {
        return field[pos.row][pos.col];
    }

    bool check_for_ship(position_detail pos, char align, uint8_t s) const
    {
        uint8_t row, col;

        if (align == 'H')
        {
            if ((pos.col + s - 1) >= FIELD_BREADTH)
                return false;
            if (pos.row >= FIELD_LENGTH)
                return false;

            for (col = pos.col; col < (pos.col + s); col++)
                if ((field[pos.row][col] >= '1') && (field[pos.row][col] <= '5'))
                    return false;

            return true;
        }
        else
        {
            if ((pos.row + s - 1) >= FIELD_LENGTH)
                return false;
            if (pos.col >= FIELD_BREADTH)
                return false;

            for (row = pos.row; row < (pos.row + s); row++)
                if ((field[row][pos.col] >= '1') && (field[row][pos.col] <= '5'))
                    return false;

            return true;
        }
    }

    char find_first_pos(position_detail &pos, uint8_t s)
    {
        uint8_t flag = 0;
        uint8_t row, col;
        char align;

        for (row = 0; row < FIELD_LENGTH; row++)
        {
            for (col = 0; col < FIELD_BREADTH; col++)
            {
                pos.row = row;
                pos.col = col;

                if (check_for_ship(pos, 'H', s) == true)
                {
                    flag = 1;
                    align = 'H';
                    break;
                }
                else if (check_for_ship(pos, 'V', s) == true)
                {
                    flag = 1;
                    align = 'V';
                    break;
                }
            }
            if (flag == 1)
                break;
        }

        return align;
    }

    void show_field(uint8_t type = 0) const
    {
        unsigned short int row, col;
        unsigned char ch;

        top.row = ROW_POS(LENGTH);
        top.col = COL_POS(BREADTH);
        bot.row = top.row + LENGTH - 1;
        bot.col = top.col + BREADTH - 1;

        system("cls");

        for (row = 0; row < LENGTH; row++)
            for (col = 0; col < BREADTH; col++)
            {
                if (row == 0)
                {
                    if (col == 0)
                        ch = 201;
                    else if (col == BREADTH - 1)
                        ch = 187;
                    else if (col%4 == 0)
                        ch = 209;
                    else
                        ch = 205;
                }
                else if (row == LENGTH - 1)
                {
                    if (col == 0)
                        ch = 200;
                    else if (col == BREADTH - 1)
                        ch = 188;
                    else if (col%4 == 0)
                        ch = 207;
                    else
                        ch = 205;
                }
                else if (row%2 == 0)
                {
                    if (col == 0)
                        ch = 199;
                    else if (col == BREADTH - 1)
                        ch = 182;
                    else if (col%4 == 0)
                        ch = 197;
                    else
                        ch = 196;
                }
                else
                {
                    if ((col == 0) || (col == BREADTH - 1))
                        ch = 186;
                    else if (col%4 == 0)
                        ch = 179;
                    else if (col%2 == 0)
                        ch = field[TO_I(row)][TO_J(col)];
                    else
                        ch = ' ';
                }

                gotoxy(top.row + row, top.col + col);
                if (type == 1)
                {
                    if (ch != '0')
                    {
                        if ((ch >= '1') && (ch <= '5'))
                            cout << 'S';
                        else if ((ch >= 'A') && (ch <= 'E'))
                            cout << '*';
                        else
                            cout << ch;
                    }
                }
                else if (type == 0)
                {
                    if ((ch >= 'A') && (ch <= 'E'))
                            cout << '*';
                    else if ((ch < '0') || (ch > '5'))
                        cout << ch;
                }
            }
    }

    void set_user_ships()
    {
        position_detail pos, temp, old_pos;
        uint8_t i, j, flag;
        int key, choice;
        char align, old_align;
        char no[2] = "", msg[21] = "";
        char message[] = "Set ship of size ";

        uint8_t len = strlen("Please wait while the ships are being placed") + 2 + 2;
        char **menu_items = NULL;
        char random[] = "> All ships placed randomly <";

        for (i = 0; i < 5; i++)
        {
            strcpy(msg, message);
            no[0] = ship[i].return_size() + '0';
            strcat(msg, no);
            print_detail(msg);

            align = find_first_pos(pos, ship[i].return_size());

            show_field(1);
            if ((SCR_LENGTH - 1) > (top.row + LENGTH - 1 + 2))
            {
                temp.row = top.row + LENGTH - 1 + 2;
                gotoxy(temp.row, 0);
                cout << "Enter: Set ship";
                gotoxy(temp.row, COL_POS(strlen("R: Randomly place all ships")));
                cout << "R: Randomly place all ships";
                gotoxy(temp.row, SCR_BREADTH - strlen("Space: Change alignment"));
                cout << "Space: Change alignment";
            }

            do
            {
                ship[i].display_ship(pos, align);

                temp = pos;
                old_pos = pos;
                flag = 0;

                key = getkey();
                switch (key)
                {
                case UP:
                    while (temp.row > 0)
                    {
                        temp.row--;
                        if (check_for_ship(temp, align, ship[i].return_size()) == true)
                        {
                            flag = 1;
                            pos = temp;
                            break;
                        }
                    }
                    break;
                case DOWN:
                    if (align == 'V')
                        while ((temp.row + ship[i].return_size() - 1) < FIELD_LENGTH)
                        {
                            temp.row++;
                            if (check_for_ship(temp, align, ship[i].return_size()) == true)
                            {
                                flag = 1;
                                pos = temp;
                                break;
                            }
                        }
                    else if (align == 'H')
                        while (temp.row < FIELD_LENGTH)
                        {
                            temp.row++;
                            if (check_for_ship(temp, align, ship[i].return_size()) == true)
                            {
                                flag = 1;
                                pos = temp;
                                break;
                            }
                        }
                    break;
                case LEFT:
                    while (temp.col > 0)
                    {
                        temp.col--;
                        if (check_for_ship(temp, align, ship[i].return_size()) == true)
                        {
                            flag = 1;
                            pos = temp;
                            break;
                        }
                    }
                    break;
                case RIGHT:
                    if (align == 'H')
                        while ((temp.col + ship[i].return_size() - 1) < FIELD_BREADTH)
                        {
                            temp.col++;
                            if (check_for_ship(temp, align, ship[i].return_size()) == true)
                            {
                                flag = 1;
                                pos = temp;
                                break;
                            }
                        }
                    else if (align == 'V')
                        while (temp.col < FIELD_BREADTH)
                        {
                            temp.col++;
                            if (check_for_ship(temp, align, ship[i].return_size()) == true)
                            {
                                flag = 1;
                                pos = temp;
                                break;
                            }
                        }
                    break;
                case 'R':
                    flag = 0;

                    choice = 2;
                    menu_items = new char*[3];
                    for (j = 0; j < 3; j++)
                        menu_items[j] = new char[15];

                    strcpy(menu_items[0], "Place Randomly");
                    strcpy(menu_items[1], "Yes");
                    strcpy(menu_items[2], "No");

                    choice = display_menu(menu_items, 3, choice);

                    if (choice == 1)
                    {
                        clear_ships();

                        system("cls");
                        top.row = ROW_POS(3);
                        top.col = COL_POS(len);
                        bot.row = top.row + 3 - 1;
                        bot.col = top.col + len - 1;
                        print_box(3, 0);
                        gotoxy(top.row + 1, top.col + 2);
                        cout << "Please wait while the ships are being placed";
                        gotoxy(SCR_LENGTH - 1, SCR_BREADTH - 1);
                        set_comp_ships();
                        display_message(random);
                    }
                    else if (choice == 2)
                    {
                        key = 0;
                        show_field(1);
                        if ((SCR_LENGTH - 1) > (top.row + LENGTH - 1 + 2))
                        {
                            temp.row = top.row + LENGTH - 1 + 2;
                            gotoxy(temp.row, 0);
                            cout << "Enter: Set ship";
                            gotoxy(temp.row, COL_POS(strlen("R: Randomly place all ships")));
                            cout << "R: Randomly place all ships";
                            gotoxy(temp.row, SCR_BREADTH - strlen("Space: Change alignment"));
                            cout << "Space: Change alignment";
                        }
                    }

                    for (j = 0; j < 3; j++)
                        delete[] menu_items[j];
                    delete[] menu_items;

                    break;
                case SPACE:
                    if (align == 'H')
                    {
                        if (check_for_ship(temp, 'V', ship[i].return_size()) == true)
                        {
                            flag = 1;
                            old_align = 'H';
                            align = 'V';
                        }
                    }
                    else if (align == 'V')
                    {
                        if (check_for_ship(temp, 'H', ship[i].return_size()) == true)
                        {
                            flag = 1;
                            old_align = 'V';
                            align = 'H';
                        }
                    }
                    break;
                case ENTER:
                    if (align == 'H')
                        for (j = pos.col; j < pos.col + ship[i].return_size(); j++)
                            field[pos.row][j] = (i + 1) + '0';
                    else if (align == 'V')
                        for (j = pos.row; j < pos.row + ship[i].return_size(); j++)
                            field[j][pos.col] = (i + 1) + '0';
                    break;
                }

                if (flag == 1)
                {
                    if ((key == UP) || (key == DOWN) || (key == LEFT) || (key == RIGHT))
                    {
                        if (align == 'H')
                            for (j = old_pos.col; j < old_pos.col + ship[i].return_size(); j++)
                            {
                                gotoxy(top.row + TO_ROW(old_pos.row), top.col + TO_COL(j));
                                cout << ' ';
                            }
                        else if (align == 'V')
                            for (j = old_pos.row; j < old_pos.row + ship[i].return_size(); j++)
                            {
                                gotoxy(top.row + TO_ROW(j), top.col + TO_COL(old_pos.col));
                                cout << ' ';
                            }
                    }
                    else if (key == SPACE)
                    {
                        if (old_align == 'H')
                            for (j = old_pos.col; j < old_pos.col + ship[i].return_size(); j++)
                            {
                                gotoxy(top.row + TO_ROW(old_pos.row), top.col + TO_COL(j));
                                cout << ' ';
                            }
                        else if (old_align == 'V')
                            for (j = old_pos.row; j < old_pos.row + ship[i].return_size(); j++)
                            {
                                gotoxy(top.row + TO_ROW(j), top.col + TO_COL(old_pos.col));
                                cout << ' ';
                            }
                    }
                }
            }while ((key != ENTER) && (key != 'R'));

            if (key == 'R')
                break;
        }
    }

    void clear_ships()
    {
        uint8_t row, col;

        for (row = 0; row < FIELD_LENGTH; row++)
            for (col = 0; col < FIELD_BREADTH; col++)
                field[row][col] = '0';
    }

    void set_comp_ships()
    {
        position_detail pos;
        uint8_t i, j;
        char align;

        for (i = 0; i < 5; i++)
            while (1)
            {
                align = find_random_pos(pos);

                if (check_for_ship(pos, align, ship[i].return_size()) == true)
                {
                    if (align == 'H')
                        for (j = pos.col; j < pos.col + ship[i].return_size(); j++)
                            field[pos.row][j] = (i + 1) + '0';
                    else if (align == 'V')
                        for (j = pos.row; j < pos.row + ship[i].return_size(); j++)
                            field[j][pos.col] = (i + 1) + '0';

                    break;
                }
            }
    }

    bool check_ship_status(uint8_t n)
    {
        if (ship[n].return_status() == shunk)
            return true;
        else
            return false;
    }

    bool ships_shunk()
    {
        uint8_t i;

        for (i = 0; i < 5; i++)
            if (ship[i].return_status() == floating)
                return false;

        return true;
    }

    char find_random_direction(position_detail pos)
    {
        uint8_t n;
        char dir;

        while (1)
        {
            srand(time(0));
            n = rand()%4;

            if (n == 0)
                dir = 'R';
            else if (n == 1)
                dir = 'L';
            else if (n == 2)
                dir = 'U';
            else if (n == 3)
                dir = 'D';

            if (dir == 'R')
            {
                if (pos.col < (FIELD_BREADTH - 1))
                    break;
            }
            else if (dir == 'L')
            {
                if (pos.col > 0)
                    break;
            }
            else if (dir == 'U')
            {
                if (pos.row > 0)
                    break;
            }
            else if (dir == 'D')
            {
                if (pos.row < (FIELD_LENGTH - 1))
                    break;
            }
        }

        return dir;
    }

    void display_message(char *msg)
    {
        uint8_t row;

        show_field(1);

        if ((SCR_LENGTH - 1) > (top.row + LENGTH - 1 + 2))
        {
            row = top.row + LENGTH - 1 + 2;
            gotoxy(row, COL_POS(strlen(msg)));
            cout << msg;
        }

        (void)getkey();
    }
};

//Functions Definition
int main()
{
    unsigned int i, choice = 1;
    char **menu_items = NULL;

    if ((SCR_LENGTH < LENGTH) || (SCR_BREADTH < BREADTH))
    {
        print_detail("Screen size of not enough");
        return 0;
    }

    menu_items = new char*[3];
    for (i = 0; i < 3; i++)
        menu_items[i] = new char[15];

    strcpy(menu_items[0], "Battleships");
    strcpy(menu_items[1], "Play");
    strcpy(menu_items[2], "Exit");

    do
    {
        choice = display_menu(menu_items, 3, choice);

        switch (choice)
        {
        case 1:
            play();
            break;
        case 2:
            print_detail("Thanks for playing. Press any key...");

            for (i = 0; i < 3; i++)
                delete[] menu_items[i];
            delete[] menu_items;
            break;
        }
    }while (choice != 2);

    return 0;
}

int getkey()
{
    int ch;

    gotoxy(SCR_LENGTH - 1, SCR_BREADTH - 1);
    ch = getch();
    if (ch == 224)
    {
        ch = getch();
        return ch;
    }

    return ch;
}

void gotoxy(const short int row, const short int col)        //"row" ra "col" exchange gareko xa yo linema sajiloko lagi|
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {col,row};
    SetConsoleCursorPosition(h, position);
}

void print_box(const short int box_type, const short int line_no)
{
    struct box_type_ASCII_codes
    {
        unsigned char tl, tr, bl, br, ver, hor;
    }box;

    short unsigned int i, j;

    switch (box_type)
    {
    case 1:     //single horizontal and single vertical
        box.tl = 218;
        box.tr = 191;
        box.bl = 192;
        box.br = 217;
        box.ver = 179;
        box.hor = 196;
        break;
    case 2:     //double horizontal and double vertical
        box.tl = 201;
        box.tr = 187;
        box.bl = 200;
        box.br = 188;
        box.ver = 186;
        box.hor = 205;
        break;
    case 3:     //double horizontal and single vertical
        box.tl = 213;
        box.tr = 184;
        box.bl = 212;
        box.br = 190;
        box.ver = 179;
        box.hor = 205;
        break;
    case 4:     //single horizontal and double vertical
        box.tl = 214;
        box.tr = 183;
        box.bl = 211;
        box.br = 189;
        box.ver = 186;
        box.hor = 196;
        break;
    default:
        box.tl = (unsigned char)box_type;
        box.tr = (unsigned char)box_type;
        box.bl = (unsigned char)box_type;
        box.br = (unsigned char)box_type;
        box.ver = (unsigned char)box_type;
        box.hor = (unsigned char)box_type;
    }

    for (i = top.col ; i <= bot.col; i++)
    {
        gotoxy(top.row, i);
        cout << box.hor;
        gotoxy(bot.row, i);
        cout << box.hor;
    }

    for (i = top.row; i <= bot.row; i++)
    {
        gotoxy(i, top.col);
        cout << box.ver;
        gotoxy(i, bot.col);
        cout << box.ver;

        if (i == (line_no + top.row - 1))
        {
            for (j = top.col ; j <= bot.col; j++)
            {
                gotoxy(i, j);
                cout << static_cast<unsigned char>(205);
            }
            gotoxy(i, top.col);
            cout << static_cast<unsigned char>(204);
            gotoxy(i, bot.col);
            cout << static_cast<unsigned char>(185);
        }
    }

    gotoxy(top.row, top.col);
    cout << box.tl;
    gotoxy(top.row, bot.col);
    cout << box.tr;
    gotoxy(bot.row, top.col);
    cout << box.bl;
    gotoxy(bot.row, bot.col);
    cout << box.br;
}

void print_detail(const char *data)
{
    short unsigned int length, breadth, len = strlen(data);

    length = 3;
    breadth = len + 2 + 2;

    system("cls");

    top.row = ROW_POS(length);
    top.col = COL_POS(breadth);
    bot.row = top.row + length - 1;
    bot.col = top.col + breadth - 1;
    print_box(3, 0);

    gotoxy(top.row + 1, top.col + 2);
    cout << data;

    (void)getkey();
}

int display_menu(char **menu_items, const int n, unsigned int choice)
{
    short unsigned int i, len, length, breadth, key;

    length = n + 2 + 2 + 1;
    breadth = 30;

    while (1)
    {
        system("cls");

        top.row = ROW_POS(length);
        top.col = COL_POS(breadth);
        bot.row = top.row + length - 1;
        bot.col = top.col + breadth - 1;
        print_box(2, 3);

        len = strlen(menu_items[0]);
        gotoxy(top.row + 1, ((SCR_BREADTH - 1 - len)/2));
        cout << menu_items[0];

        top.row += 3;
        top.col += 1;
        bot.row = top.row + 2;
        bot.col -= 1;

        for (i = 1; i < n; i++)
        {
            if (choice == i)
            {
                print_box(3, 0);

                len = strlen(menu_items[i]);
                gotoxy(top.row + 1, ((SCR_BREADTH - 1 - len)/2));
                cout << menu_items[i];

                top.row += 3;
            }
            else
            {
                gotoxy(top.row, top.col + 2);
                cout << menu_items[i];

                top.row += 1;
            }
            bot.row = top.row + 2;
        }

        key = getkey();
        switch (key)
        {
        case UP:
            if (choice > 1)
                choice--;
            else
                choice = n - 1;
            break;
        case DOWN:
            if (choice < static_cast<unsigned int>(n - 1))
                choice++;
            else
                choice = 1;
            break;
        case ESC:
            choice = n - 1;

            if (strcmp(menu_items[choice], "Exit") != 0)
                return choice;

            break;
        case ENTER:
            return choice;
        }
    }
}

void play()
{
    BattleField user;
    uint8_t i, flag = 0;
    unsigned int choice = 1, replace_choice = 1;
    char **menu_items = NULL;
    char **replace_menu_items = NULL;

    menu_items = new char*[5];
    for (i = 0; i < 5; i++)
        menu_items[i] = new char[15];

    strcpy(menu_items[0], "Options");
    strcpy(menu_items[1], "Place Ships");
    strcpy(menu_items[2], "View Ships");
    strcpy(menu_items[3], "Battle");
    strcpy(menu_items[4], "Back");

    do
    {
        choice = display_menu(menu_items, 5, choice);

        switch (choice)
        {
        case 1:
            if (flag == 1)
            {
                replace_menu_items = new char*[3];
                for (i = 0; i < 3; i++)
                    replace_menu_items[i] = new char[15];

                strcpy(replace_menu_items[0], "Options");
                strcpy(replace_menu_items[1], "Replace Ships");
                strcpy(replace_menu_items[2], "Back");

                replace_choice = 1;

                replace_choice = display_menu(replace_menu_items, 3, replace_choice);

                if (replace_choice == 1)
                {
                    user.clear_ships();
                    user.set_user_ships();
                    print_detail("All ships placed");
                }

                for (i = 0; i < 3; i++)
                    delete[] replace_menu_items[i];
                delete[] replace_menu_items;
            }
            else
            {
                user.set_user_ships();
                print_detail("All ships placed");

                flag = 1;
            }
            break;
        case 2:
            if (flag == 0)
            {
                print_detail("You haven't place your ships yet");
                break;
            }
            user.show_field(1);
            (void)getkey();
            break;
        case 3:
            if (flag == 0)
            {
                print_detail("You haven't place your ships yet");
                break;
            }
            battle(user);
            break;
        case 4:
            for (i = 0; i < 4; i++)
                delete[] menu_items[i];
            delete[] menu_items;

            break;
        }
    }while (choice != 4);
}

void battle(BattleField &user)
{
    turn_detail turn;
    BattleField comp;
    uint8_t finish, turn_switch;
    uint8_t len = strlen("Please wait while the computer places its ships") + 2 + 2;

    system("cls");
    top.row = ROW_POS(3);
    top.col = COL_POS(len);
    bot.row = top.row + 3 - 1;
    bot.col = top.col + len - 1;
    print_box(3, 0);
    gotoxy(top.row + 1, top.col + 2);
    cout << "Please wait while the computer places its ships";
    gotoxy(SCR_LENGTH - 1, SCR_BREADTH - 1);
    comp.set_comp_ships();
    print_detail("Computer has placed its ship. Battle Starts...");

    srand(time(0));
    if (rand()%2 == 0)
        turn = users;
    else
    {
        turn = comps;
        print_detail("Computers turn");
    }

    finish = 0;
    turn_switch = 1;

    while (1)
    {
        if (turn == users)
        {
            finish = user_attacks(user, comp, turn_switch);
            if (finish == 1)
                break;
            if (turn_switch == 1)
            {
                print_detail("You've missed");
                turn = comps;
            }
        }
        else if (turn == comps)
        {
            finish = comp_attakcs(user, comp, turn_switch);
            if (finish == 1)
                break;
            if (turn_switch == 1)
                turn = users;
        }
    }

    if (turn == users)
        print_detail("You win!!!");
    else if (turn == comps)
        print_detail("Computer wins!!!");
}

uint8_t user_attacks(BattleField &user, BattleField &comp, uint8_t &turn_switch)
{
    static position_detail pos = {0, 0};

    position_detail old_pos, temp;
    uint8_t flag, flag_enter, finish = 0;
    int key;
    char ch;

    if (turn_switch == 1)
    {
        print_detail("Your turn");
        turn_switch = 0;
    }

    comp.show_field();         //Pass '1' as argument to show the position of computers ships
    if ((SCR_LENGTH - 1) > (top.row + LENGTH - 1 + 2))
    {
        temp.row = top.row + LENGTH - 1 + 2;
        gotoxy(temp.row, 0);
        cout << "\'.\': Miss";
        gotoxy(temp.row, COL_POS(strlen("\'*\': Hit")));
        cout << "\'*\': Hit";
        gotoxy(temp.row, SCR_BREADTH - strlen("\'#\': Shunk"));
        cout << "\'#\': Shunk";
    }

    do
    {
        gotoxy(top.row + TO_ROW(pos.row), top.col + TO_COL(pos.col));
        cout << 'X';

        temp = pos;
        old_pos = pos;
        flag = 0;

        key = getkey();
        switch (key)
        {
        case UP:
            if (temp.row == 0)
                temp.row = FIELD_LENGTH - 1;
            else if (temp.row > 0)
                temp.row--;

            flag = 1;
            pos = temp;
            break;
        case DOWN:
            if (temp.row == (FIELD_LENGTH - 1))
                temp.row = 0;
            else if (temp.row < (FIELD_LENGTH - 1))
                temp.row++;

            flag = 1;
            pos = temp;
            break;
        case LEFT:
            if (temp.col == 0)
                temp.col = FIELD_BREADTH - 1;
            else if (temp.col > 0)
                temp.col--;

            flag = 1;
            pos = temp;
            break;
        case RIGHT:
            if (temp.col == (FIELD_BREADTH - 1))
                temp.col = 0;
            else if (temp.col < (FIELD_BREADTH - 1))
                temp.col++;

            flag = 1;
            pos = temp;
            break;
        case ENTER:
            ch = comp.return_data(pos);

            if ((ch < '0') || (ch > '5'))
                break;

            if (ch == '0')
            {
                comp.set_data(pos, '.');
                turn_switch = 1;
            }
            else
            {
                comp.set_data(pos, '*');
                if (comp.ships_shunk() == true)
                {
                    finish = 1;

                    comp.show_field();
                    if ((SCR_LENGTH - 1) > (top.row + LENGTH - 1 + 2))
                    {
                        temp.row = top.row + LENGTH - 1 + 2;
                        gotoxy(temp.row, 0);
                        cout << "\'.\': Miss   \'*\': Hit   \'#\': Shunk";
                    }

                    (void)getkey();
                    break;
                }
            }

            flag_enter = 0;
            while (1)
            {
                for (; pos.row < FIELD_LENGTH; pos.row++, pos.col = 0)
                {
                    for (; pos.col < FIELD_BREADTH; pos.col++)
                        if ((comp.return_data(pos) >= '0') && (comp.return_data(pos) <= '5'))
                        {
                            flag_enter = 1;
                            break;
                        }
                    if (flag_enter == 1)
                        break;
                }
                if (flag_enter == 1)
                    break;
                else
                    pos.row = pos.col = 0;
            }
            break;
        }

        if (flag == 1)
        {
            gotoxy(top.row + TO_ROW(old_pos.row), top.col + TO_COL(old_pos.col));

            ch = comp.return_data(old_pos);
            if ((ch >= '0') && (ch <= '5'))
                cout << ' ';
            else if (ch == '.')
                cout << '.';
            else if ((ch >= 'A') && (ch <= 'E'))
                cout << '*';
            else if (ch == '#')
                cout << '#';
        }
    }while (key != ENTER);

    return finish;
}

uint8_t comp_attakcs(BattleField &user, BattleField &comp, uint8_t &turn_switch)
{
    enum attack_status
    {
        miss, hits, shunk, cant_attack
    }status;

    static bool hit = false;
    static bool second_dir = false;
    static char ship_id = '0';
    static char direction = 'N';
    static position_detail pos, hit_pos = {0, 0};

    bool shunk_status;
    uint8_t finish = 0;
    uint8_t len = strlen("Please wait while computer attacks") + 2 + 2;
    char ch;

    char missed[] = "> Computer missed its attack <";
    char shinked[] = "> Computer shunk one of your ships <";
    char attacked[] = "> Computer hits one of your ships <";
    char finished[] = "> All your ships have been shunk <";

    if (turn_switch == 1)
    {
        system("cls");
        top.row = ROW_POS(3);
        top.col = COL_POS(len);
        bot.row = top.row + 3 - 1;
        bot.col = top.col + len - 1;
        print_box(3, 0);
        gotoxy(top.row + 1, top.col + 2);
        cout << "Please wait while computer attacks";
        gotoxy(SCR_LENGTH - 1, SCR_BREADTH - 1);
        turn_switch = 0;
    }

    if (hit == false)
    {
        while (1)
        {
            (void)user.find_random_pos(pos);
            ch = user.return_data(pos);

            if ((ch >= '0') && (ch <= '5'))
                break;
        }

        if (ch == '0')
        {
            user.set_data(pos, '.');
            turn_switch = 1;

            user.display_message(missed);

            return finish;
        }
        else
        {
            user.set_data(pos, '*');
            hit = true;
            direction = 'N';
            ship_id = ch;
            hit_pos = pos;

            user.display_message(attacked);
        }
    }
    if (hit == true)
    {
        if (direction == 'N')
        {
            while (1)
            {
                direction = user.find_random_direction(hit_pos);
                pos = hit_pos;

                if (direction == 'R')
                    pos.col++;
                else if (direction == 'L')
                    pos.col--;
                else if (direction == 'U')
                    pos.row--;
                else if (direction == 'D')
                    pos.row++;

                ch = user.return_data(pos);
                if ((ch >= '0') && (ch <= '5'))
                    break;
            }

            if (ch == '0')
            {
                direction = 'N';
                user.set_data(pos, '.');
                turn_switch = 1;

                user.display_message(missed);

                return finish;
            }
            else
            {
                status = hits;
                user.set_data(pos, '*');
                shunk_status = user.check_ship_status(ship_id - '0' - 1);
                if (shunk_status == true)
                {
                    status = shunk;
                    hit = false;
                    direction = 'N';
                    if (user.ships_shunk() == true)
                    {
                        finish = 1;

                        user.display_message(finished);

                        return finish;
                    }

                    user.display_message(shinked);
                    return finish;
                }

                user.display_message(attacked);
            }
        }
        while (1)
        {
            if (direction == 'R')
            {
                if (pos.col == (FIELD_BREADTH - 1))
                    status = cant_attack;
                else
                {
                    pos.col++;
                    ch = user.return_data(pos);
                    if ((ch < '0') || (ch > '5'))
                        status = cant_attack;
                    else if (ch == '0')
                        status = miss;
                    else
                        status = hits;
                }

                if ((status == cant_attack) || (status == miss))
                {
                    if (second_dir == true)
                    {
                        srand(time(0));
                        if (rand()%2 == 0)
                            direction = 'U';
                        else
                            direction = 'D';

                        second_dir = false;
                    }
                    else
                    {
                        direction = 'L';
                        second_dir = true;
                    }
                }

                if (status == cant_attack)
                    pos = hit_pos;
                else if (status == miss)
                    break;
            }
            if (direction == 'L')
            {
                if (pos.col == 0)
                    status = cant_attack;
                else
                {
                    pos.col--;
                    ch = user.return_data(pos);
                    if ((ch < '0') || (ch > '5'))
                        status = cant_attack;
                    else if (ch == '0')
                        status = miss;
                    else
                        status = hits;
                }

                if ((status == cant_attack) || (status == miss))
                {
                    if (second_dir == true)
                    {
                        srand(time(0));
                        if (rand()%2 == 0)
                            direction = 'U';
                        else
                            direction = 'D';

                        second_dir = false;
                    }
                    else
                    {
                        direction = 'R';
                        second_dir = true;
                    }
                }

                if (status == cant_attack)
                    pos = hit_pos;
                else if (status == miss)
                    break;
            }
            if (direction == 'U')
            {
                if (pos.row == 0)
                    status = cant_attack;
                else
                {
                    pos.row--;
                    ch = user.return_data(pos);
                    if ((ch < '0') || (ch > '5'))
                        status = cant_attack;
                    else if (ch == '0')
                        status = miss;
                    else
                        status = hits;
                }

                if ((status == cant_attack) || (status == miss))
                {
                    if (second_dir == true)
                    {
                        srand(time(0));
                        if (rand()%2 == 0)
                            direction = 'L';
                        else
                            direction = 'R';

                        second_dir = false;
                    }
                    else
                    {
                        direction = 'D';
                        second_dir = true;
                    }
                }

                if (status == cant_attack)
                    pos = hit_pos;
                else if (status == miss)
                    break;
            }
            if (direction == 'D')
            {
                if (pos.row == (FIELD_LENGTH - 1))
                    status = cant_attack;
                else
                {
                    pos.row++;
                    ch = user.return_data(pos);
                    if ((ch < '0') || (ch > '5'))
                        status = cant_attack;
                    else if (ch == '0')
                        status = miss;
                    else
                        status = hits;
                }

                if ((status == cant_attack) || (status == miss))
                {
                    if (second_dir == true)
                    {
                        srand(time(0));
                        if (rand()%2 == 0)
                            direction = 'L';
                        else
                            direction = 'R';

                        second_dir = false;
                    }
                    else
                    {
                        direction = 'U';
                        second_dir = true;
                    }
                }

                if (status == cant_attack)
                    pos = hit_pos;
                else if (status == miss)
                    break;
            }

            if (status == hits)
            {
                user.set_data(pos, '*');
                shunk_status = user.check_ship_status(ship_id - '0' - 1);
                if (shunk_status == true)
                {
                    status = shunk;
                    hit = false;
                    direction = 'N';
                    if (user.ships_shunk() == true)
                    {
                        finish = 1;
                        user.display_message(finished);

                        return finish;
                    }

                    user.display_message(shinked);
                    break;
                }

                user.display_message(attacked);
            }
        }

        if (status == miss)
        {
            user.set_data(pos, '.');
            pos = hit_pos;
            turn_switch = 1;

            user.display_message(missed);
        }
    }

    return finish;
}
