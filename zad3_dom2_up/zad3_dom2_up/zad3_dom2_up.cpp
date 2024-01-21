/**
*
* Solution to homework assignment 2
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Aleksandar Mandev
* @idnumber 5MI0600297
* @task 3
*
*/


#include <iostream>

bool isDigit(char ch) {
    return (ch >= '0' && ch <= '9');
}

void swap(char& a, char& b) {
    int temp = a;
    a = b;
    b = temp;
}

unsigned strLen(const char* str) {
    if (!str)
        return 0;

    unsigned length = 0;

    while (*str) {
        str++;
        length++;
    }
    return length;
}

int convertToNum(const char* str) {
    if (!str)
        return -1;

    int result = 0;

    while (*str) {
        if (!isDigit(*str))
            return -1;

        (result *= 10) += (*str - '0');
        str++;
    }

    return result;
}

void skipIntervals(char*& str) {
    if (!str)
        return;

    while (*str == ' ') {
        str++;
    }
}

void splitInput(char* input, int& n) {
    if (!input)
        return;

    while (*input != ' ') {
        input++;
    }

    *input = 0;
    input++;

    skipIntervals(input);

    n = convertToNum(input);
}

void concatLettersAtBack(char* str, unsigned firstIndex, char ch, unsigned count)
{
    for (int i = 0; i < count; i++)
        str[firstIndex + i] = ch;
}

char* sortStr(const char* str) {
    if (!str)
        return nullptr;

    constexpr int VALUES_OF_CHAR = 256;
    constexpr char FIRST_CHAR = -128;
    int count[VALUES_OF_CHAR]{ 0 };

    int length = strLen(str);
    char* result = new char[length + 1];

    while (*str) {
        count[*str - FIRST_CHAR]++;
        str++;
    }

    int resultIndex = 0;
    for (int i = 0; i < VALUES_OF_CHAR; i++)
    {
        concatLettersAtBack(result, resultIndex, FIRST_CHAR+i, count[i]);
        resultIndex += count[i];
    }
    result[length] = '\0';

    return result;
}

void printPermutationsOfSortedStr(char* str, int n) {
    if (!str || n < 1)
        return;

    int length = strLen(str);

    std::cout << str << std::endl;
    n--;

    bool isNewPerm = false;

    for (int i = 0; i < n; i++)
    {
        isNewPerm = false;
        for (int i = length-1; i > 0; i--)
        {
            if (str[i] > str[i - 1]) {
                swap(str[i], str[i - 1]);
                std::cout << str << std::endl;
                isNewPerm = true;
                break;
            }
        }
        if (!isNewPerm) {
            break;
        }
    }
}

int main()
{
    constexpr int MAX_STRING_LENGTH = 1024;
    char str[MAX_STRING_LENGTH];
    std::cin.getline(str, MAX_STRING_LENGTH);

    int n = 0;
    splitInput(str, n);
    
    char* sortedString = sortStr(str);

    printPermutationsOfSortedStr(sortedString, n);

    delete[] sortedString;
}