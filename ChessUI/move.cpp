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

#include "move.h"

Move::Move(int type, Square from, Square to){
    this->type = type;
    this->from = from;
    this->to = to;
}

std::string move_to_string(const Move m){
    Square to = m.getTo();
    int type = m.getType();
    if(type >= PromotionKnightMove && type <= PromotionQueenMove)
        return square_to_string(m.getFrom())
                + square_to_string(to) + piece_to_char(Piece(type+Black));
    return square_to_string(m.getFrom()) + square_to_string(to);
}
