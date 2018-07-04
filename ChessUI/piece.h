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

#ifndef PIECE_H
#define PIECE_H

#include "color.h"

enum Piece{
    W = 0, B = 1,
    WP = 2, BP = 3,
    WN = 4, BN = 5,
    WB = 6, BB = 7,
    WR = 8, BR = 9,
    WQ = 10, BQ = 11,
    WK = 12, BK = 13,
    Empty = 14
};

inline Color piece_color(const Piece p){
    return Color(p%2);
}

inline void operator++ (Piece &x, int) {
    x = Piece(int(x) + 1);
}

inline void operator+= (Piece &p, int x) {
    p = Piece(int(p) + x);
}

Piece get_piece(const char c);

char piece_to_char(const Piece p);

#endif // PIECE_H
