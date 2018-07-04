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

#ifndef POSITION_H
#define POSITION_H

#include "square.h"
#include "piece.h"
#include "move.h"
#include "castle.h"
#include "color.h"
#include "piece.h"
#include "bitboard.h"
#include "chessui.h"
#include <inttypes.h>
#include <QDebug>

class Position{
public:
    Position();
    Position(const Position & pos);
    Position(const std::string fen);
    void add(const Piece p, const Square sq);
    void remove(const Square sq);
    void empty();
    void initial();
    void moveDo(Move & m);
    void move_undo(const Move m);
    void set_ep(const Square sq);
    Square get_ep(const Square sq) const;
    void set_castle (const Castle c);
    void set_stm(const Color c);
    Color getStm() const;
    Castle get_castle() const;
    Piece getPiece(const Square sq) const;
    QList<Square> getAttacks(const Square sq);
    Square is_king_attacked(QList<Square> squares);
    std::string get_fen() const;
    void getMoves(Move m[256]);
    std::string to_string() const;
    Color stm;
    Castle castle;
    bool enPassant;
private:
    Bitboard bitboard[14];
    Bitboard occupied;
    Bitboard checkers;

    Square ep;
    Piece piece[64];
    static bool init;

    void quiet_do(const Move m);
    void capture_do(Move & m);
    void castle_do(Move &m);
    void ep_do(Move & m);
    void promotion_do(Move & m);

    void quiet_undo(const Move m);
    void capture_undo(const Move m);
    void castle_undo(const Move m);
    void ep_undo(const Move m);
    void promotion_undo(const Move m);
    void null_undo();

    Bitboard knight_attack(const Square sq) const;
    Bitboard bishop_attack(const Square sq) const;
    Bitboard rook_attack(const Square sq) const;
    Bitboard queen_attack(const Square sq) const;
    Bitboard king_attack(const Square sq) const;
    Bitboard get_checkers() const;

    bool sq_attacked(const Square sq, const Color c) const;
    bool is_attacked(const Square from, const Square to) const;
    bool is_pinned(const Square from, const Square to) const;
    bool is_pinned(const Square from, const Square to, const Square c_sq) const;

    typedef Bitboard (Position::*AttackFunction)(const Square sq) const;
    static AttackFunction attack[14];
};

inline Piece Position::getPiece(const Square sq) const{
    return piece[sq];
}

inline Color Position::getStm() const{
    return stm;
}

#endif // POSITION_H
