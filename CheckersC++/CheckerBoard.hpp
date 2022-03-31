#pragma once

#include "CheckerCage.hpp"

#include <iostream>
#include <vector>
#include <cstdint>

enum State {
    CONTINUES,
    WHITE_WON,
    BLACK_WON,
    NO_ONE
};

// --------------------------------------------------------------------------------------

class CheckerBoard {
public:
    CheckerBoard();
    CheckerBoard(int8_t color);
    ~CheckerBoard() = default;

    void SetPlayerColor(int8_t color);

    void PrintInitialState();
    void PrintState();

    void ReceiveMove(const Point& from, const Point& to);
    void ReternMove();

    int32_t GameState() const;

private:
    void PrintEmptyBoardLine(int32_t i);
    void PrintFilledBoardLine(int32_t i);

    void MakeMove(const Point& from, const Point& to);
    void MakeChange(int8_t our_color, const Point& our_from_location,
                    const Point& our_to_location, const Point& opponent_location);
    void DecreaseCheckerCount(int8_t color);

    std::pair<Point, Point> CheckDiagonal(const Point& start, const Point& finish);
    std::pair<Point, Point> CheckIfTripple(const Point& point1, const Point& point2, const Point& point3);
    std::pair<Point, Point> CheckNeighbours(const Point& point, bool check_danger);

private:
    std::vector<std::vector<CheckerCage>> checker_board_;
    int32_t player_color_{WHITE};
    int32_t white_checkers_count_{12};
    int32_t black_checkers_count_{12};
    bool blocked_{false};
};

// --------------------------------------------------------------------------------------