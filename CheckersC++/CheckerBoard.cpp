#include "CheckerBoard.hpp"

// --------------------------------------------------------------------------------------

CheckerBoard::CheckerBoard() : checker_board_(8, std::vector<CheckerCage>(8)) {
    for (int32_t i = 0; i < 8; ++i) {
        for (int32_t j = 0; j < 8; ++j) {
            if ((i + j) % 2) {
                checker_board_[i][j].SetCageInfo(i, j, WHITE);
            } else {
                checker_board_[i][j].SetCageInfo(i, j, BLACK);
            }
            if (i >= 0 && i < 3 && !((i + j) % 2)) {
                checker_board_[i][j].SetCheckerInfo(true, WHITE);
            } else if (i >= 5 && i < 8 && !((i + j) % 2)) {
                checker_board_[i][j].SetCheckerInfo(true, BLACK);
            } else {
                checker_board_[i][j].SetCheckerInfo(false, NONE);
            }
        }
    }
}

CheckerBoard::CheckerBoard(int8_t color) : CheckerBoard() {
    SetPlayerColor(color);
}

void CheckerBoard::SetPlayerColor(int8_t color) {
    player_color_ = color;
}

// --------------------------------------------------------------------------------------

void CheckerBoard::DecreaseCheckerCount(int8_t color) {
    if (color == WHITE) {
        --white_checkers_count_;
    } else if (color == BLACK) {
        --black_checkers_count_;
    }
}

void CheckerBoard::ReceiveMove(const Point& from, const Point& to) {
    MakeMove(from, to);
}

void CheckerBoard::ReternMove() {
    std::vector<std::pair<Point, Point>> diagonals = {{Point{4, 0}, Point{7, 3}},
                                                      {Point{2, 0}, Point{7, 5}},
                                                      {Point{0, 0}, Point{7, 7}},
                                                      {Point{0, 2}, Point{5, 7}},
                                                      {Point{0, 4}, Point{3, 7}},
                                                      {Point{0, 2}, Point{2, 0}},
                                                      {Point{0, 4}, Point{4, 0}},
                                                      {Point{0, 6}, Point{6, 0}},
                                                      {Point{1, 7}, Point{7, 1}},
                                                      {Point{3, 7}, Point{7, 3}},
                                                      {Point{5, 7}, Point{7, 5}}};
    bool tripples_still_exist = true;
    int32_t tripple_moves_count = 0;
    int32_t pair_moves_count = 0;

    while (tripples_still_exist) {
//        std::cout << "trying to find tripples\n";
        tripples_still_exist = false;
        for (size_t i = 0; i < diagonals.size(); ++i) {
            auto[black_checker_from, black_checker_to] = CheckDiagonal(diagonals[i].first, diagonals[i].second);
            if (black_checker_from != Point{-1, -1}) {
//                std::cout << "found tripple\n";
                std::cout << diagonals[i].first.x << " " << diagonals[i].first.y << "\n";
                ++tripple_moves_count;
                tripples_still_exist = true;
                MakeMove(black_checker_from, black_checker_to);
            }
        }
    }


    if (tripple_moves_count == 0) {
//        std::cout << "trying to find pair\n";
        for (int32_t i = 0; i < 8; ++i) {
            for (int32_t j = 0; j < 8; ++j) {
                if (checker_board_[i][j].CheckerColor() != BLACK) {
                    continue;
                }
                auto[black_checker_from, black_checker_to] = CheckNeighbours({i, j}, true);
                if (black_checker_from != Point{-1, -1}) {
                    ++pair_moves_count;
                    MakeMove(black_checker_from, black_checker_to);
                    return;
                }
            }
        }
        for (int32_t i = 0; i < 8; ++i) {
            for (int32_t j = 0; j < 8; ++j) {
                if (checker_board_[i][j].CheckerColor() != BLACK) {
                    continue;
                }
                auto[black_checker_from, black_checker_to] = CheckNeighbours({i, j}, false);
                if (black_checker_from != Point{-1, -1}) {
                    ++pair_moves_count;
                    MakeMove(black_checker_from, black_checker_to);
                    return;
                }
            }
        }

    }

    blocked_ = true;
}

std::pair<Point, Point> CheckerBoard::CheckDiagonal(const Point& start, const Point& finish) {
    Point vector{(finish.x - start.x) / std::abs(finish.x - start.x),
                 (finish.y - start.y) / std::abs(finish.y - start.y)};
    for (int32_t i = 0; i < std::abs(finish.x - start.x) - 2; ++i) {
        auto[black_checker_from, black_checker_to] = CheckIfTripple(
                {start.x + i * vector.x, start.y + i * vector.y},
                       {start.x + (i + 1) * vector.x, start.y + (i + 1) * vector.y},
                       {start.x + (i + 2) * vector.x, start.y + (i + 2) * vector.y});
        if (black_checker_from != Point{-1, -1}) {
            return std::make_pair(black_checker_from, black_checker_to);
        }
    }
    return {{-1, -1}, {-1, -1}};
}

std::pair<Point, Point> CheckerBoard::CheckIfTripple(const Point& point1, const Point& point2, const Point& point3) {
    if (checker_board_[point1.x][point1.y].CheckerColor() == BLACK &&
        checker_board_[point2.x][point2.y].CheckerColor() == WHITE &&
        checker_board_[point3.x][point3.y].CheckerColor() == NONE) {
        return std::make_pair(point1, point3);
    } else if (checker_board_[point1.x][point1.y].CheckerColor() == NONE &&
               checker_board_[point2.x][point2.y].CheckerColor() == WHITE &&
               checker_board_[point3.x][point3.y].CheckerColor() == BLACK) {
        return std::make_pair(point3, point1);
    }
    return {{-1, -1}, {-1, -1}};
}

std::pair<Point, Point> CheckerBoard::CheckNeighbours(const Point& point, bool check_danger) {
    std::vector<Point> offsets = {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
    for (size_t i = 0; i < offsets.size(); ++i) {
        Point new_point = Point{point.x + offsets[i].x, point.y + offsets[i].y};
        Point danger_point = Point{point.x + 1 + offsets[i].x, point.y + 1 + offsets[i].y};
        if (new_point.x >= 0 && new_point.x < 8 && new_point.y >= 0 && new_point.y < 8) {
            if (checker_board_[new_point.x][new_point.y].CheckerColor() == NONE) {
                if (check_danger) {
                    if (danger_point.x >= 0 && danger_point.x < 8 && danger_point.y >= 0 && danger_point.y < 8) {
                        if (checker_board_[danger_point.x][danger_point.y].CheckerColor() == WHITE) {
                            continue;
                        }
                    }
                }
                return std::make_pair(point, new_point);
            }
        }
    }
    return {{-1, -1}, {-1, -1}};
}

void CheckerBoard::MakeMove(const Point& from, const Point& to) {
    if (checker_board_[from.x][from.y].CheckerIsLady()) {
        Point opponent_location;
        Point vector{(to.x - from.x) / std::abs(to.x - from.x), (to.y - from.y) / std::abs(to.y - from.y)};
        int8_t our_color = checker_board_[from.x][from.y].CheckerColor();
        for (int32_t i = 1; i <= std::abs(to.x - from.x); ++i) {
            if (!checker_board_[from.x + i * vector.x][from.y + i * vector.y].IsEmpty()) {
                opponent_location = {from.x + i * vector.x, from.y + i * vector.y};
                break;
            }
        }
        MakeChange(our_color, from, to, opponent_location);
    } else {
        if (std::abs(from.x - to.x) == 1) {
            int8_t our_color = checker_board_[from.x][from.y].CheckerColor();
            checker_board_[from.x][from.y].DeleteChecker();
            checker_board_[to.x][to.y].SetChecker(our_color);
        } else if (std::abs(from.x - to.x) == 2) {
            Point opponent_location{(from.x + to.x) / 2,(from.y + to.y) /2};
            int8_t our_color = checker_board_[from.x][from.y].CheckerColor();
            MakeChange(our_color, from, to, opponent_location);
        }
    }
    checker_board_[to.x][to.y].UpdateCheckerPriority();
}

void CheckerBoard::MakeChange(int8_t color, const Point& our_from_location,
                const Point& our_to_location, const Point& opponent_location) {
    checker_board_[our_from_location.x][our_from_location.y].DeleteChecker();
    checker_board_[opponent_location.x][opponent_location.y].DeleteChecker();
    DecreaseCheckerCount(!color);
    checker_board_[our_to_location.x][our_to_location.y].SetChecker(color);
}

// --------------------------------------------------------------------------------------

int32_t CheckerBoard::GameState() const {
    if (blocked_) {
        return NO_ONE;
    } else if (white_checkers_count_ == 0) {
        return BLACK_WON;
    } else if (black_checkers_count_ == 0) {
        return WHITE_WON;
    } else {
        return CONTINUES;
    }
}

// --------------------------------------------------------------------------------------

void CheckerBoard::PrintState() {
    std::cout << "  |    a    |    b    |    c    |    d    |    e    |    f    |    g    |    h    |\n";
    for (int32_t i = 7; i >= 0; --i) {
        std::cout << "--|---------|---------|---------|---------|---------|---------|---------|---------|\n";
        PrintEmptyBoardLine(i);
        PrintFilledBoardLine(i);
        PrintEmptyBoardLine(i);
    }
    std::cout << "--|---------|---------|---------|---------|---------|---------|---------|---------|\n";
}

void CheckerBoard::PrintInitialState() {
    PrintState();
}

void CheckerBoard::PrintEmptyBoardLine(int32_t i) {
    std::cout << "  ";
    for (int32_t j = 0; j < 8; ++j) {
        if (checker_board_[i][j].CageIsWhite()) {
            std::cout << "|---------";
        } else {
            std::cout << "|         ";
        }
    }
    std::cout << "|\n";
}

void CheckerBoard::PrintFilledBoardLine(int32_t i) {
    std::cout << 8 - i << " ";
    for (int32_t j = 0; j < 8; ++j) {
        if (checker_board_[i][j].CheckerColor() == WHITE) {
            if (checker_board_[i][j].CheckerIsLady()) {
                std::cout << "|    W*   ";
            } else {
                std::cout << "|    W    ";
            }
        } else if (checker_board_[i][j].CheckerColor() == BLACK) {
            if (checker_board_[i][j].CheckerIsLady()) {
                std::cout << "|    B*   ";
            } else {
                std::cout << "|    B    ";
            }
        } else {
            if (checker_board_[i][j].CageIsWhite()) {
                std::cout << "|---------";
            } else {
                std::cout << "|         ";
            }
        }
    }
    std::cout << "|\n";
}

// --------------------------------------------------------------------------------------