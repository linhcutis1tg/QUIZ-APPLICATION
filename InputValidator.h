#pragma once
#include <iostream>
#include <string>

class InputValidator {
public:
    InputValidator() = delete; // Class tiện ích, không cho khởi tạo đối tượng
    
    static char getValidatedAnswer(int optionCount = 0);
    static bool validateQuestionIndex(int choiceIndex, int totalQuestions);
};

char InputValidator::getValidatedAnswer(int optionCount) 
{
    std::string input;
    while (true) {
        if (optionCount > 0) 
        {
            std::cout << "Your answer (or 'S' to skip): ";
        }
        
        if (!(std::cin >> input)) 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input error! Please try again.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (input.length() == 1) 
        {
            char ch = static_cast<char>(std::toupper(input[0]));
            if (ch == 'S') return ch;

            if (optionCount > 0) {
                if (ch >= 'A' && ch < 'A' + optionCount) return ch;
            } else {
                if (ch >= 'A' && ch <= 'Z') return ch;
            }
        }
        std::cout << "Invalid choice! Please enter a valid character.\n";
    }
}

bool InputValidator::validateQuestionIndex(int choiceIndex, int totalQuestions) 
{
    return choiceIndex >= 1 && choiceIndex <= totalQuestions;
}