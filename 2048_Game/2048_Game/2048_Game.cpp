#include <iostream>

bool isValidSize(int n, int minSize, int maxSize) {
    return (n >= minSize && n <= maxSize);
}
bool isValidMenuOption(int n, int menuSize) {
    return (n >= 1 && n <= menuSize);
}

void printMenu() {
    std::cout << "1. Start game" << std::endl;
    std::cout << "2. Leaderboard" << std::endl;
    std::cout << "3. Exit" << std::endl;
}

void errorCommands() {
    constexpr int CHARACTERS_TO_IGNORE = 128;

    std::cin.clear();
    std::cin.ignore(CHARACTERS_TO_IGNORE, '\n');
    std::cout << "Invalid option!" << std::endl;
}


void inputMenuChoiceMessage(int menuSize) {
    std::cout << "Choose an option (1-" << menuSize << "): ";
}

void inputMatrixSizeMessage(int minSize, int maxSize) {
    std::cout << "Choose a field size (" << minSize << "-" << maxSize << "): ";
}

char* getPlayerNickname() {
    constexpr unsigned MAX_LENGTH = 64;
    char nickname[MAX_LENGTH];

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter your nickname (max length " << MAX_LENGTH << " characters): ";
    std::cin.getline(nickname, MAX_LENGTH);

    return nickname;
}

int getMatrixSize() {
    constexpr int MIN_SIZE = 4;
    constexpr int MAX_SIZE = 10;

    inputMatrixSizeMessage(MIN_SIZE, MAX_SIZE);
    int size;
    std::cin >> size;

    while (!isValidSize(size, MIN_SIZE, MAX_SIZE)) {
        errorCommands();
        inputMatrixSizeMessage(MIN_SIZE, MAX_SIZE);
        std::cin >> size;
    }

    return size;
}

int getMenuChoice() {
    constexpr int MENU_SIZE = 3;

    inputMenuChoiceMessage(MENU_SIZE);
    int choice;
    std::cin >> choice;
    
    while (!isValidMenuOption(choice, MENU_SIZE)) {
        errorCommands();
        inputMenuChoiceMessage(MENU_SIZE);
        std::cin >> choice;
    }
    return choice;
}

int** createMatrix(int size) {
    int** mtx = new int* [size];

    for (int i = 0; i < size; i++)
        mtx[i] = new int[size] {0};

    return mtx;
}

void deleteMatrix(int** mtx, int size) {
    if (!mtx)
        return;

    for (int i = 0; i < size; i++)
        delete[] mtx[i];
    delete[] mtx;
}

void startGame() {
    char* playerNickname = getPlayerNickname();

    std::cout << playerNickname;

    int size = getMatrixSize();

    int** mtx = createMatrix(size);
    


    deleteMatrix(mtx, size);
}

void leaderboard() {

}

void exit() {

}


void runGame() {
    printMenu();
    
    int menuChoice = getMenuChoice();
    
    switch (menuChoice) {
        case 1:
            startGame();
            break;
        case 2:
            break;
        case 3:
            break;
    }
}

int main()
{
    runGame();
}
