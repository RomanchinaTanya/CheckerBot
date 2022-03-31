#include "CheckerCage.hpp"

// --------------------------------------------------------------------------------------

void CheckerCage::SetCheckerInfo(bool has_checker, int8_t checker_color) {
    has_checker_ = has_checker;
    checker_color_ = checker_color;
}

void CheckerCage::SetCageInfo(int32_t x, int32_t y, int8_t cage_color) {
    coords_ = {x, y};
    cage_color_ = cage_color;
}

// --------------------------------------------------------------------------------------

bool CheckerCage::IsEmpty() const {
    return !has_checker_;
}

bool CheckerCage::CageIsWhite() const {
    return cage_color_ == WHITE;
}

uint8_t CheckerCage::CheckerColor() const {
    return checker_color_;
}

bool CheckerCage::CheckerIsLady() const {
    return is_lady_;
}

void CheckerCage::DeleteChecker() {
    has_checker_ = false;
    is_lady_ = false;
    checker_color_ = NONE;
}

void CheckerCage::SetChecker(int8_t color) {
    has_checker_ = true;
    checker_color_ = color;
}

void CheckerCage::UpdateCheckerPriority() {
    if ((checker_color_ == WHITE && coords_.x == 7) || (checker_color_ == BLACK && coords_.x == 0)) {
        is_lady_ = true;
    }
}

// --------------------------------------------------------------------------------------

BoardImplCage ToBoardImplCage(const Point& point) {
    return BoardImplCage{8 - point.x, static_cast<char>(point.y + 'a')};
}

Point ToPoint(const BoardImplCage& board_impl_cage) {
    return Point{8 - board_impl_cage.row, board_impl_cage.column - 'a'};
}

// --------------------------------------------------------------------------------------