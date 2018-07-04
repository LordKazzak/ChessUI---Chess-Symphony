/***************************************************************************
 *   Copyright (C) 2011 by Borko Boskovic                                  *
 *   borko.boskovic@gmail.com                                              *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef SQUARE_H
#define SQUARE_H

#include <string>

enum Square {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NO_SQ
};

enum File {
    FILE_A = 0, FILE_B = 1, FILE_C = 2, FILE_D = 3,
    FILE_E = 4, FILE_F = 5, FILE_G = 6, FILE_H = 7, FILE_NONE = 8
};

enum Rank {
    RANK_1 = 0, RANK_2 = 1, RANK_3 = 2, RANK_4 = 3,
    RANK_5 = 4, RANK_6 = 5, RANK_7 = 6, RANK_8 = 7, RANK_NONE = 8
};

inline Rank rank(const Square sq){
        return Rank(sq>>3);
}

inline File file(const Square sq){
        return File(sq&7);
}

inline Square operator+ (Square sq, int i) {
    return Square(int(sq) + i);
}

inline Square operator- (Square sq, int i) {
    return Square(int(sq) - i);
}

inline Square operator++ (Square &x, int y) {
    y = int(x);
    x = Square(int(x) + 1);
    return Square(y);
}

inline Square operator-- (Square &x, int y) {
    y = int(x);
    x = Square(int(x) - 1);
    return Square(y);
}

inline void operator+= (Square &x, int y) {
    x = Square(int(x) + y);
}

inline Square operator++ (Square &x) {
    x = Square(int(x) + 1);
    return x;
}

inline Square operator-- (Square &x) {
    x = Square(int(x) - 1);
    return x;
}

inline void operator-= (Square &sq1, int sq2) {
    sq1 = Square(int(sq1) - sq2);
}

Square get_square(const std::string s);

std::string square_to_string(const Square sq);

#endif // SQUARE_H
