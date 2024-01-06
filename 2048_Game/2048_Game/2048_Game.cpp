#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

constexpr int MIN_MATRIX_SIZE = 4;
constexpr int MAX_MATRIX_SIZE = 10;
constexpr int MAX_INPUT_STRING_LEN = 100;

void clearConsole() {
    system("CLS");
}

// STR/INPUT/HELPER FUNCTIONS

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

char* readStr() {
    char* input = new char[MAX_INPUT_STRING_LEN];
    std::cin.getline(input, MAX_INPUT_STRING_LEN);
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
    std::cout << "1. Start Game\n";
    std::cout << "2. Leaderboard\n";
    std::cout << "3. Exit\n";
}

int readMenuChoice() {
    std::cout << "Choose your option(1-3): ";
    int choice = readNum();

    while (!(choice >= 1 && choice <= 3)) {
        clearConsole();
        printMenu();
        std::cout << "Invalid option!\n";
        std::cout << "Choose a new option(1-3): ";
        choice = readNum();
    }

    return choice;
}

int readMatrixSize() {
    std::cout << "Choose a field size (" << MIN_MATRIX_SIZE << "-" << MAX_MATRIX_SIZE << "): ";
    int size = readNum();

    while (!isValidSize(size)) {
        clearConsole();
        std::cout << "Invalid size! Size should be between " << MIN_MATRIX_SIZE << " and " << MAX_MATRIX_SIZE << "!\n";
        std::cout << "Enter a new size: ";
        size = readNum();
    }

    return size;
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

            std::cout << std::setw(digitsOfMax + whitespace) << mtx[i][j]; 
        }
        std::cout << "\n\n";
    }

    score = calculateScore(mtx, size);
    std::cout << "Score: " << score << "\n\n";
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
    if (!mtx)
        return;

    for (int i = 0; i < size; i++)
        delete[] mtx[i];
    delete[] mtx;
}

bool areValidIndices(int x, int y, int size) {
    return (x >= 0 && x < size) && (y >= 0 && y < size);
}

bool containsEmptySpace(unsigned** mtx, int size);

void generateRandomInMatrix(unsigned** mtx, int size, unsigned score) {
    if (!mtx || !containsEmptySpace(mtx, size))
        return;

    constexpr unsigned LOWER_SCORE_GENERATOR = 2048;
    constexpr unsigned HIGHER_SCORE_GENERATOR = 8192;

    srand(time(0));

    int row = rand() % size;
    int col = rand() % size;

    while (mtx[row][col] != 0) {
        row = rand() % size;
        col = rand() % size;
    }

    unsigned values[12] = { 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 8, 8 };

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
        for (int j = 0; j < size; ++j) {
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
    std::cout << "Enter your name: ";
    char* playerName = readStr();

    int size = readMatrixSize();

    unsigned** mtx = createMatrix(size);

    generateRandomInMatrix(mtx, size, 0);
    unsigned score = 0;
    printMatrix(mtx, size, score);

    while (existsPossibleMove(mtx, size)) {
        std::cout << "Enter a move(w,a,s,d): ";
        char* move = readStr();

        while(!isValidMove(move)) {
            delete[] move;
            clearConsole();
            printMatrix(mtx, size, score);
            std::cout << "Invalid move!\nChoose a new move(w,a,s,d): ";
            move = readStr();
        }

        bool moved = false;

        executeMove(mtx, size, move[0], moved);

        clearConsole();

        if (moved)
            generateRandomInMatrix(mtx, size, score);

        printMatrix(mtx, size, score);

        delete[] move;
    }

    delete[] playerName;
    deleteMatrix(mtx, size);
    // TO DO
}

void leaderboard() {
    clearConsole();
    // TO DO
}

void exit() {
    // TO DO
}


void runGame() {
    printMenu();
    
    int menuChoice = readMenuChoice();
    
    switch (menuChoice) {
        case 1:
            startGame();
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
}

int main()
{
    runGame();
    return 0;
}
