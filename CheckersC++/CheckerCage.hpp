#pragma once

#include <cstdint>
#include <utility>

enum Color {
    BLACK,
    WHITE,
    NONE
};

// --------------------------------------------------------------------------------------

struct BoardImplCage {
    int32_t row;
    char column;
};

// --------------------------------------------------------------------------------------

struct Point {
    int32_t x;
    int32_t y;

    bool operator==(const Point& other) {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) {
        return !(*this == other);
    }
};

// --------------------------------------------------------------------------------------

class CheckerCage {
public:
    void SetCheckerInfo(bool has_checker, int8_t checker_color);
    void SetCageInfo(int32_t x, int32_t y, int8_t cage_color);

    bool IsEmpty() const;
    bool CageIsWhite() const;
    uint8_t CheckerColor() const;
    bool CheckerIsLady() const;

    void DeleteChecker();
    void SetChecker(int8_t color);

    void UpdateCheckerPriority();

private:
    Point coords_{0, 0};
    bool has_checker_{false};
    bool is_lady_{false};
    int8_t checker_color_{WHITE};
    int8_t cage_color_{WHITE};
};

// --------------------------------------------------------------------------------------

BoardImplCage ToBoardImplCage(const Point& point);

Point ToPoint(const BoardImplCage& board_impl_cage);

// --------------------------------------------------------------------------------------