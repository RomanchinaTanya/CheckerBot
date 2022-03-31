#include "CheckerBoard.hpp"

#include <iostream>
#include <string>
#include <cstdint>
#include <utility>

void PrintIntroduction();

void PrintContinueOrNot();

void PrintAcceptingInfo();

void PrintReturnInfo();

void PrintNewTurnInfo();

bool GameContinues(const CheckerBoard& checker_board);

int main() {

    PrintIntroduction();

    CheckerBoard checker_board;
    checker_board.PrintInitialState();

    // -----------------------------------------------------------------------------------
    char column_from = '0';
    char column_to = '0';
    int32_t row_from = 0;
    int32_t row_to = 0;
    int32_t number_of_moves = 0;
    std::string query;
    // -----------------------------------------------------------------------------------

    while (true) {
        PrintContinueOrNot();
        std::cin >> query;
        if (query == "exit") {
            break;
        } else if (query == "ok") {
            PrintNewTurnInfo();
        }

        std::cin >> number_of_moves;
        for (int32_t i = 0; i < number_of_moves; ++i) {
            std::cin >> column_from >> row_from >> column_to >> row_to;

            BoardImplCage from{row_from, column_from};
            BoardImplCage to{row_to, column_to};

            // All player moves are supposed to be valid!
            checker_board.ReceiveMove(ToPoint(from), ToPoint(to));
        }
        PrintAcceptingInfo();
        checker_board.PrintState();

        if (!GameContinues(checker_board)) {
            break;
        }

        checker_board.ReternMove();

        PrintReturnInfo();
        checker_board.PrintState();

        if (!GameContinues(checker_board)) {
            break;
        }

        PrintNewTurnInfo();
    }
}

void PrintIntroduction() {
    std::cout << "--------------------------------------------------------------------------------------\n";
    std::cout << "Welcome, checkers lover! You are playing white!\n";
    std::cout << "Enter your checker move in format \"how many moves\" \"from\" \"to\", for example : 1\n";
    std::cout << "                                                                              a 6 b 5\n";
    std::cout << "If number of moves is more than one (you have the opportunity to eat more than one opponent checker)\n";
    std::cout << "then each following \"from\" coordinate must be the same as previous \"to\" coordinate.\n";
    std::cout << "--------------------------------------------------------------------------------------\n\n";
}

void PrintContinueOrNot() {
    std::cout << "--------------------------------------------------------------------------------------\n";
    std::cout << "We continue playing?\n";
    std::cout << "--------------------------------------------------------------------------------------\n\n";
}

void PrintAcceptingInfo() {
    std::cout << "--------------------------------------------------------------------------------------\n";
    std::cout << "Here is the result of your turn:\n";
    std::cout << "--------------------------------------------------------------------------------------\n\n";
}

void PrintReturnInfo() {
    std::cout << "--------------------------------------------------------------------------------------\n";
    std::cout << "Here is the result of my turn:\n";
    std::cout << "--------------------------------------------------------------------------------------\n\n";
}

void PrintNewTurnInfo() {
    std::cout << "--------------------------------------------------------------------------------------\n";
    std::cout << "Enter your next turn:\n";
    std::cout << "--------------------------------------------------------------------------------------\n\n";
}

bool GameContinues(const CheckerBoard& checker_board) {
    int32_t game_state = checker_board.GameState();
    if (game_state == WHITE_WON) {
        return false;
    } else if (game_state == BLACK_WON) {
        return false;
    } else {
        return true;
    }
}