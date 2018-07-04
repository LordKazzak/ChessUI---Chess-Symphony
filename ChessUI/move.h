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

#ifndef MOVE_H
#define MOVE_H

#include "square.h"
#include "castle.h"
#include "piece.h"

enum {
    NullMove = 0,
    QuietMove = 1,
    CaptureMove = 2,
    CastleMove = 3,
    PromotionKnightMove = 4,
    PromotionBishopMove = 6,
    PromotionRookMove = 8,
    PromotionQueenMove = 10,
    EnPassanMove = 11
};


class Move{
public:
    Move(){}
    Move(int type, Square from, Square to);
    inline int getType() const;
    inline Square getFrom() const;
    inline Square getTo() const;
    inline Square get_ep() const;
    inline Castle get_castle() const;
    inline Piece get_captured() const;
    inline void set_ep(const Square ep);
    inline void set_castle(const Castle c);
    inline void set_captured(const Piece p);
private:
    int type;
    Square from;
    Square to;
    Square ep;
    Castle castle;
    Piece captured;
};

inline int Move::getType() const{
    return type;
}

inline Square Move::getFrom() const{
    return from;
}

inline Square Move::getTo() const{
    return to;
}

inline Castle Move::get_castle() const{
    return castle;
}

inline Square Move::get_ep() const{
    return ep;
}

inline Piece Move::get_captured() const{
    return captured;
}

inline void Move::set_ep(const Square ep){
    this->ep = ep;
}

inline void Move::set_castle(const Castle c){
    castle = c;
}

inline void Move::set_captured(const Piece p){
    captured = p;
}

std::string move_to_string(const Move m);

#endif // MOVE_H
