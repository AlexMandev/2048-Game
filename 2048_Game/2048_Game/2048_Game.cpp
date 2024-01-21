/**
*
* Solution to course project # 4
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Aleksandar Mandev
* @idnumber 5MI0600297
* @compiler Visual Studio
*
* Code for 2048
*
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

constexpr int MIN_MATRIX_SIZE = 4;
constexpr int MAX_MATRIX_SIZE = 10;
constexpr int MAX_INPUT_STRING_LEN = 101;
constexpr int MAX_LEADERBOARD_PLAYERS = 5;

void clearConsole() {
    system("CLS");
}

unsigned strLen(char* str) {
    if (!str)
        return 0;

    unsigned length = 0;

    while (*str) {
        length++;
        str++;
    }
    return length;
}

void strCpy(const char* source, char* dest) {
    if (!source || !dest)
        return;

    while (*source)
    {
        *dest = *source;
        source++;
        dest++;
    }
    *dest = '\0';
}

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
void swap(char* a, char* b) {
    int lengthOfA = strLen(a);

    char* temp = new char[lengthOfA + 1];
    strCpy(a, temp);
    strCpy(b, a);
    strCpy(temp, b);

    delete[] temp;
}

// IO FUNCTIONS

char* readStr() {
    char* input = new char[MAX_INPUT_STRING_LEN];
    cin.getline(input, MAX_INPUT_STRING_LEN);
    return input;
}


bool isDigit(char ch) {
    return (ch >= '0' && ch <= '9');
}

int charToIndex(char ch) {
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    return 0;
}
char digitToChar(int n) {
    if (n >= 0 && n <= 9)
        return '0' + n;
    return '\0';
}

int convertToNum(const char* str) {
    if (!str)
        return 0;

    int number = 0;

    while (*str) {
        

        if (!isDigit(*str)) {
            return 0;
        }

        int index = charToIndex(*str);

        (number *= 10) += index;
        str++;
    }

    return number;
}

int readNum() {
    char* input = readStr();
    int num = convertToNum(input);
    delete[] input;
    return num;
}

unsigned countDigits(unsigned n) {
    if (n == 0)
        return 1;
    
    int count = 0;
    while (n != 0) {
        count++;
        n /= 10;
    }
    return count;
}

bool isValidSize(int n) {
    return (n >= MIN_MATRIX_SIZE && n <= MAX_MATRIX_SIZE);
}
bool isValidMenuOption(int n) {
    return (n >= 1 && n <= 3);
}
bool isValidMove(char* move) {
    if (!move || (strLen(move)!= 1))
        return false;
    
    return (*move == 'w' || *move == 'a' || *move == 's' || *move == 'd');
}

void printMenu() {
    cout << "1. Start Game\n";
    cout << "2. Leaderboard\n";
    cout << "3. Exit\n";
}

int readMenuChoice() {
    cout << "Choose your option(1-3): ";
    int choice = readNum();

    while (!(choice >= 1 && choice <= 3)) {
        clearConsole();
        printMenu();
        cout << "Invalid option!\n";
        cout << "Choose a new option(1-3): ";
        choice = readNum();
    }

    return choice;
}

int readMatrixSize() {
    cout << "Choose a field size (" << MIN_MATRIX_SIZE << "-" << MAX_MATRIX_SIZE << "): ";
    int size = readNum();

    while (!isValidSize(size)) {
        clearConsole();
        cout << "Invalid size! Size should be between " << MIN_MATRIX_SIZE << " and " << MAX_MATRIX_SIZE << "!\n";
        cout << "Enter a new size: ";
        size = readNum();
    }

    return size;
}

// LEADERBOARD FUNCTIONS

void sortLeaderboard(char nicknames[][MAX_INPUT_STRING_LEN], unsigned* scores, int indexAdded) {
    for (int i = indexAdded - 1; i >= 0; i--)
    {
        if (scores[i] < scores[indexAdded]) {
            swap(scores[i], scores[indexAdded]);
            swap(nicknames[i], nicknames[indexAdded]);
            indexAdded = i;
        }
    }
}

char* generateFileName(int mtxSize) {
    int fileLength = countDigits(mtxSize) + 5;

    char* name = new char[fileLength];

    int index = 0;

    if (mtxSize < 10) {
        name[index++] = digitToChar(mtxSize);
    }
    else {
        name[index++] = '1';
        name[index++] = '0';
    }

    name[index++] = '.';
    name[index++] = 't';
    name[index++] = 'x';
    name[index++] = 't';
    name[index++] = '\0';

    return name;
}

void updateHighscores(char* nickname, unsigned score, int size) {
    char nicknames[MAX_LEADERBOARD_PLAYERS + 1][MAX_INPUT_STRING_LEN];
    unsigned scores[MAX_LEADERBOARD_PLAYERS + 1];

    strCpy(nickname, nicknames[MAX_LEADERBOARD_PLAYERS]);
    scores[MAX_LEADERBOARD_PLAYERS] = score;

    unsigned playersInFile = 0;

    char* fileName = generateFileName(size);

    ifstream ifs(fileName);

    if (ifs.is_open())
    {

        for (int i = 0; i < MAX_LEADERBOARD_PLAYERS; i++) // read the leaderboard
        {
            ifs.getline(nicknames[i], MAX_INPUT_STRING_LEN, '\n');
            if (nicknames[i][0] != '\0') {
                ifs >> scores[i];
                ifs.ignore(MAX_INPUT_STRING_LEN, '\n');
                playersInFile++;
            }
            else {
                playersInFile = i;
                break;
            }
        }

        for (int i = playersInFile; i < MAX_LEADERBOARD_PLAYERS; i++)
            scores[i] = 0;

        bool added = false;
        int addedIndex = 0;

        for (int i = MAX_LEADERBOARD_PLAYERS - 1; i >= 0; i--) // add the new score, if needed
        {
            if (scores[i] < score) {
                strCpy(nickname, nicknames[i]);
                scores[i] = score;
                added = true;
                addedIndex = i;
                break;
            }
        }

        if (added) {
            sortLeaderboard(nicknames, scores, addedIndex); // sort if new score is added
            if (playersInFile != MAX_LEADERBOARD_PLAYERS)
                playersInFile++;
        }

        ifs.clear();
        ifs.close();

        ofstream ofs(fileName);
        for (int i = 0; i < playersInFile; i++)
        {
            ofs << nicknames[i] << '\n' << scores[i] << '\n';
        }

        ofs.clear();
        ofs.close();
    }
    else {
        ofstream ofs;
        ofs.open(fileName);
        
        ofs << nickname << '\n' << score << '\n';

        ofs.clear();
        ofs.close();
    }

    delete[] fileName;
}


// MATRIX FUNCTIONS

unsigned calculateScore(unsigned** mtx, int size) {
    if (!mtx)
        return 0;

    unsigned score = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            score += mtx[i][j];
        }
    }
    return score;
}

unsigned findMaxInMatrix(unsigned** mtx, int size) {
    if (!mtx)
        return 0;
    unsigned max = 0;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (mtx[i][j] > max)
                max = mtx[i][j];
        }
    }
    return max;
}

void printMatrix(unsigned** mtx, int size, unsigned& score) {
    clearConsole();

    unsigned max = findMaxInMatrix(mtx, size);
    int digitsOfMax = countDigits(max);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int whitespace = 0;
            if (j != 0)
                whitespace = 3;

            cout << setw(digitsOfMax + whitespace) << mtx[i][j]; 
        }
        cout << "\n\n";
    }

    score = calculateScore(mtx, size);
    cout << "Score: " << score << "\n\n";
}

unsigned** createMatrix(int size) {
    unsigned** mtx = new unsigned* [size];

    for (int i = 0; i < size; i++) {
        mtx[i] = new unsigned[size];

        for (int j = 0; j < size; j++)
        {
            mtx[i][j] = 0;
        }
    }
    
    return mtx;
}

void deleteMatrix(unsigned** mtx, int size) {
    for (int i = 0; i < size; i++)
        delete[] mtx[i];
    delete[] mtx;
}

bool areValidIndices(int x, int y, int size) {
    return (x >= 0 && x < size) && (y >= 0 && y < size);
}

bool containsEmptySpace(unsigned** mtx, int size) {
    if (!mtx)
        return false;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
        {
            if (!mtx[i][j])
                return true;
        }
    }
    return false;
}

void generateRandomInMatrix(unsigned** mtx, int size, unsigned score) {
    if (!mtx || !containsEmptySpace(mtx, size))
        return;

    constexpr unsigned LOWER_SCORE_GENERATOR = 1500;
    constexpr unsigned HIGHER_SCORE_GENERATOR = 3000;

    srand(time(0));

    int row = rand() % size;
    int col = rand() % size;

    while (mtx[row][col] != 0) {
        row = rand() % size;
        col = rand() % size;
    }

    unsigned values[12] = { 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 8 };

    int indexBorder = 0;
    if (score < LOWER_SCORE_GENERATOR) {
        indexBorder = 7;
    }
    else if (score < HIGHER_SCORE_GENERATOR) {
        indexBorder = 9;
    }
    else {
        indexBorder = 12;
    }

    int index = rand() % indexBorder;
    unsigned value = values[index];

    mtx[row][col] = value;
}

bool hasEqualAdjacentNumbers(unsigned** mtx, int size) {
    if (!mtx)
        return false;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if ((areValidIndices(i + 1, j, size) && mtx[i + 1][j] == mtx[i][j])
                || (areValidIndices(i, j + 1, size) && mtx[i][j + 1] == mtx[i][j])) {
                return true;
            }
        }
    }
    return false;
}

bool existsPossibleMove(unsigned** mtx, int size) {
    if (!mtx)
        return false;

    return (containsEmptySpace(mtx, size) || hasEqualAdjacentNumbers(mtx, size));
}

void moveLeft(unsigned** mtx, int size, bool& movedOrSummed) {
    if (!mtx)
        return;

    for (int i = 0; i < size; i++) {
        int pos = 0;
        for (int j = 0; j < size; j++) {
            if (mtx[i][j] != 0) {
                mtx[i][pos++] = mtx[i][j];

                if (pos - 1 != j) {
                    movedOrSummed = true;
                }
            }
        }

        for (int j = pos; j < size; j++) {
            mtx[i][j] = 0;
        }
    }
}

void moveRight(unsigned** mtx, int size, bool& movedOrSummed) {
    if (!mtx)
        return;

    for (int i = 0; i < size; i++)
    {
        int pos = size - 1;
        for (int j = size-1; j >= 0; j--)
        {
            if (mtx[i][j] != 0) {
                mtx[i][pos--] = mtx[i][j];

                if (pos + 1 != j)
                    movedOrSummed = true;
            }
        }

        for (int j = pos; j >= 0; j--) {
            mtx[i][j] = 0;
        }
    }
    
}
void moveUp(unsigned** mtx, int size, bool& movedOrSummed) {
    for (int j = 0; j < size; j++)
    {
        int pos = 0;
        for (int i = 0; i < size; i++)
        {
            if (mtx[i][j]) {
                mtx[pos++][j] = mtx[i][j];

                if (pos - 1 != i)
                    movedOrSummed = true;
            }
        }
        
        for (int i = pos; i < size; i++)
        {
            mtx[i][j] = 0;
        }
    }
}
void moveDown(unsigned** mtx, int size, bool& movedOrSummed) {
    for (int j = 0; j < size; j++)
    {
        int pos = size - 1;
        for (int i = size-1; i >= 0; i--)
        {
            if (mtx[i][j] != 0) {
                mtx[pos--][j] = mtx[i][j];
                
                if (pos + 1 != i)
                    movedOrSummed = true;
            }
        }

        for (int i = pos; i >= 0; i--)
        {
            mtx[i][j] = 0;
        }
    }
}

void sumElementsLeft(unsigned** mtx, int size, bool& movedOrSummed) {
    if (!mtx)
        return;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size - 1; j++)
        {
            if (mtx[i][j] == mtx[i][j + 1] && mtx[i][j] != 0) {
                mtx[i][j] *= 2;
                mtx[i][j + 1] = 0;
                movedOrSummed = true;
            }
        }
    }
}
void sumElementsRight(unsigned** mtx, int size, bool& movedOrSummed) {
    if (!mtx)
        return;

    for (int i = 0; i < size; i++)
    {
        for (int j = size-1; j > 0; j--)
        {
            if (mtx[i][j] == mtx[i][j - 1] && mtx[i][j] != 0) {
                mtx[i][j] *= 2;
                mtx[i][j - 1] = 0;
                movedOrSummed = true;
            }
        }
    }
}
void sumElementsUp(unsigned** mtx, int size, bool& movedOrSummed) {
    if (!mtx)
        return;

    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < size-1; i++)
        {
            if (mtx[i][j] == mtx[i+1][j] && mtx[i][j] != 0) {
                mtx[i][j] *= 2;
                mtx[i+1][j] = 0;
                movedOrSummed = true;
            }
        }
    }
}
void sumElementsDown(unsigned** mtx, int size, bool& movedOrSummed) {
    if (!mtx)
        return;
    
    for (int j = 0; j < size; j++) {
        for (int i = size - 1; i > 0; i--)
        {
            if (mtx[i][j] == mtx[i - 1][j] && mtx[i][j] != 0) {
                mtx[i][j] *= 2;
                mtx[i-1][j] = 0;
                movedOrSummed = true;
            }

        }
    }
}

void executeLeft(unsigned** mtx, int size, bool& moved) {
    moveLeft(mtx, size, moved);

    sumElementsLeft(mtx, size, moved);

    moveLeft(mtx, size, moved);
}
void executeRight(unsigned** mtx, int size, bool& moved) {
    moveRight(mtx, size, moved);

    sumElementsRight(mtx, size, moved);

    moveRight(mtx, size, moved);
}
void executeUp(unsigned** mtx, int size, bool& moved) {
    moveUp(mtx, size, moved);

    sumElementsUp(mtx, size, moved);

    moveUp(mtx, size, moved);
}
void executeDown(unsigned** mtx, int size, bool& moved) {
    moveDown(mtx, size, moved);

    sumElementsDown(mtx, size, moved);

    moveDown(mtx, size, moved);
}

void executeMove(unsigned** mtx, int size, char direction, bool& moved) {
    switch (direction) {
        case 'w':
            executeUp(mtx, size, moved);
            break;
        case 'a':
            executeLeft(mtx, size, moved);
            break;
        case 's':
            executeDown(mtx, size, moved);
            break;
        case 'd':
            executeRight(mtx, size, moved);
            break;
        default:
            return;
    }
}

void startGame() {
    clearConsole();
    cout << "Enter your name: ";
    char* playerName = readStr();

    int size = readMatrixSize();

    unsigned** mtx = createMatrix(size);

    generateRandomInMatrix(mtx, size, 0);
    unsigned score = 0;
    printMatrix(mtx, size, score);


    while (existsPossibleMove(mtx, size)) {
        cout << "Enter a move(w,a,s,d): ";
        char* move = readStr();

        while(!isValidMove(move)) {
            delete[] move;
            clearConsole();
            printMatrix(mtx, size, score);
            cout << "Invalid move!\nChoose a new move(w,a,s,d): ";
            move = readStr();
        }

        bool moved = false;

        executeMove(mtx, size, move[0], moved);

        clearConsole();

        if(moved)
            generateRandomInMatrix(mtx, size, score);

        printMatrix(mtx, size, score);

        delete[] move;
    }

    delete[] playerName;
    deleteMatrix(mtx, size);
}

void leaderboard() {
    clearConsole();

    cout << "Choose field size for the leaderboard(" << MIN_MATRIX_SIZE << '-' << MAX_MATRIX_SIZE << "): ";
    int size = readNum();

    while (size < MIN_MATRIX_SIZE || size > MAX_MATRIX_SIZE) {
        clearConsole();
        cout << "Invalid option!\n";
        cout << "Choose a new field size for the leaderboard(" << MIN_MATRIX_SIZE << '-' << MAX_MATRIX_SIZE << "): ";
        size = readNum();
    }

    clearConsole();

    char* fileName = generateFileName(size);
    
    ifstream ifs(fileName);

    if (!ifs.is_open()) {
        cout << "Leaderboard for " << size << "x" << size << " is still empty.\n\n";
        delete[] fileName;
        return;
    }

    char nickname[MAX_INPUT_STRING_LEN];
    int score;

    for (int i = 0; i < MAX_LEADERBOARD_PLAYERS; i++)
    {
        ifs.getline(nickname, MAX_INPUT_STRING_LEN, '\n');
        if (nickname[0] != '\0') {
            ifs >> score;
            ifs.ignore(MAX_INPUT_STRING_LEN, '\n');
            cout << i + 1 << ". " << nickname << " - " << score << "\n";
        }
    }
    cout << '\n';

    ifs.clear();
    ifs.close();
    delete[] fileName;
}

void runGame() {
    bool running = true;

    while (running) {
        printMenu();
        int menuChoice = readMenuChoice();

        switch (menuChoice) {
        case 1:
            startGame();
            break;
        case 2:
            leaderboard();
            break;
        case 3:
            running = false;
            break;
        default:
            break;
        }
    }
}

int main()
{
    runGame();
}
