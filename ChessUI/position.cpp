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

#include <sstream>
#include <iostream>

#include "position.h"
#include "global.h"

bool Position::init = false;

Position::AttackFunction Position::attack[14] = {
    0, 0, 0, 0,
    &Position::knight_attack, &Position::knight_attack,
    &Position::bishop_attack, &Position::bishop_attack,
    &Position::rook_attack, &Position::rook_attack,
    &Position::queen_attack, &Position::queen_attack,
    &Position::king_attack, &Position::king_attack
};

Position::Position(){
    enPassant = false;
    if(!init) bb_init();
    initial();
}

Position::Position(const std::string fen){
    int index = 0, line = 7, intc;
    Square sq = A8;
    empty();
    while(fen[index] == ' ' || fen[index] == '\t') index ++;
    while(fen[index] != ' '){
        intc = (int)fen[index];
        if(intc >= 48 && intc <=59){ sq += intc - 48;}
        else if(fen[index] != '/'){ add(get_piece(fen[index]),sq++); }
        else {
            if(sq % 8 != 0){
                // Error
            }
            sq = Square(--line * 8);
        }
        index ++;
    }
    if(sq % 8 != 0){
        // Error
    }
    while(fen[index] == ' ' || fen[index] == '\t') index ++;
    if(fen[index] == 'b') stm = Black;
    else if(fen[index] != 'w') {
        // Error
    }
    index ++;
    while(fen[index] == ' ' || fen[index] == '\t') index ++;
    while(fen[index] != ' '){
        switch (fen[index]){
            case 'K':{
                castle |= WK_CASTLE;
                break;
            }
            case 'Q':{
                castle |= WQ_CASTLE;
                break;
            }
            case 'k':{
                castle |= BK_CASTLE;
                break;
            }
            case 'q':{
                castle |= BQ_CASTLE;
                break;
            }
            case '-':{ break; }
            default: {
                // Erro
            }
        }
        index++;
    }
    index ++;
    while(fen[index] == ' ' || fen[index] == '\t') index ++;
    if(fen[index] != '-'){
        Square sq = get_square(fen.substr(index,2));
        ep = sq;
    }
    checkers = get_checkers();
}

inline Bitboard Position::knight_attack(const Square sq) const{
    return BB_KnightAttack[sq];
}

inline Bitboard Position::bishop_attack(const Square sq) const{
    return bb_bishop_attack(sq,occupied);
}

inline Bitboard Position::rook_attack(const Square sq) const{
    return bb_rook_attack(sq,occupied);
}

inline Bitboard Position::queen_attack(const Square sq) const{
    return rook_attack(sq) | bishop_attack(sq);
}

inline Bitboard Position::king_attack(const Square sq) const{
    return BB_KingAttack[sq];
}

QList<Square> Position::getAttacks(const Square sq){
    QList<Square> list;
    Move moves[256];
    getMoves(moves);
    list.append(sq);
    int i=0;
    while(moves[i].getType() != NullMove)
    {
        if(moves[i].getFrom() == sq)
        {
            //qDebug() << "Possible moves: " << QString::fromStdString(square_to_string(moves[i].getTo()));
            list.append(moves[i].getTo());
        }
        i++;
    }

    return list;
}

Square Position::is_king_attacked(QList<Square> squares){ //return king's square, if it's attacked
    int i=0;
    while(i<squares.count())
    {
        Square king = first_bit(bitboard[WK+stm]);
        if(is_pinned(king,squares[i])){
            return king;
        }
        i++;
    }
    return NO_SQ;
}

inline bool Position::sq_attacked(const Square sq, const Color c) const{ //for castling
    const Color opp = opp_color(c);
    return (rook_attack(sq)&(bitboard[WR+opp]|bitboard[WQ+opp]))
        || (bishop_attack(sq)&(bitboard[WB+opp]|bitboard[WQ+opp]))
        || (knight_attack(sq)&bitboard[WN+opp])
        || (bb_pawn_attack(c,sq)&bitboard[WP+opp]);
}

inline Bitboard Position::get_checkers() const{
    int opp = opp_color(stm);
    const Square sq = first_bit(bitboard[WK+stm]);
    return (rook_attack(sq)
            & (bitboard[WR+opp]|bitboard[WQ+opp]))
            | (bishop_attack(sq)&(bitboard[WB+opp]|bitboard[WQ+opp]))
            | (knight_attack(sq)&bitboard[WN+opp])
            | (bb_pawn_attack(stm,sq)&bitboard[WP+opp]);
}

inline bool Position::is_attacked(const Square from, const Square to)const { //castling, king moves
    const int opp = opp_color(stm);
    Bitboard occ = occupied ^ (Bitboard(1)<<from);
    return (bb_rook_attack(to,occ)&(bitboard[WR+opp]|bitboard[WQ+opp]))
            || (bb_bishop_attack(to,occ)&(bitboard[WB+opp]|bitboard[WQ+opp]))
            || (knight_attack(to)&bitboard[WN+opp])
            || (bb_pawn_attack(stm,to)&bitboard[WP+opp]);
}

inline bool Position::is_pinned(const Square from, const Square to) const{ //true if king under attack
    //qDebug() << "ispinned function" << QString::fromStdString(square_to_string(from)) << QString::fromStdString(square_to_string(to));
    const int opp = opp_color(stm);
    const Square k_sq = first_bit(bitboard[WK+stm]); //king square
    Bitboard bb_to = Bitboard(1)<<to;
    if(bb_to & occupied){ //shrug
        Bitboard occ = occupied ^ (Bitboard(1)<<from);
        Bitboard bb_opp = ~bb_to;
        //qDebug() << "attacker" << QString::fromStdString(square_to_string(attacking_figure)) << QString::fromStdString(square_to_string(to));
        if(to == attacking_figure)
            return false;

        return (bb_rook_attack(k_sq,occ) & (bitboard[WR+opp]|bitboard[WQ+opp]) & bb_opp)
                || (bb_bishop_attack(k_sq,occ) & (bitboard[WB+opp]|bitboard[WQ+opp]) & bb_opp)
                || (knight_attack(k_sq) & (bitboard)[WN+opp])
                || (bb_pawn_attack(stm,k_sq) & (bitboard)[WP+opp]);
    }
    else{ //in case king attacked
        Bitboard occ = occupied ^ (Bitboard(1)<<from | Bitboard(1)<<to);
        /*return (bb_rook_attack(k_sq,occ) & (bitboard[WR+opp]|bitboard[WQ+opp]))
                || (bb_bishop_attack(k_sq,occ) & (bitboard[WB+opp]|bitboard[WQ+opp]))
                || (knight_attack(k_sq) & (bitboard)[WN+opp])
                || (bb_pawn_attack(stm,k_sq) & (bitboard)[WP+opp]);*/


        //if((knight_attack(k_sq) & (bitboard)[WN+opp]) || (bb_pawn_attack(stm,k_sq) & (bitboard)[WP+opp])){
            //qDebug() << "Special attack";
            //exception_attacking_king = true;
            //exception_square = k_sq;

            /*else
                return true;*/
        //}
        //else{
            //exception_attacking_king = false;
            //exception_square = NO_SQ;
        return (bb_rook_attack(k_sq,occ) & (bitboard[WR+opp]|bitboard[WQ+opp]))
                || (bb_bishop_attack(k_sq,occ) & (bitboard[WB+opp]|bitboard[WQ+opp]))
                || (knight_attack(k_sq) & (bitboard)[WN+opp])
                || (bb_pawn_attack(stm,k_sq) & (bitboard)[WP+opp]);
    }
}

inline bool Position::is_pinned(const Square from, const Square to,
                                const Square c_sq) const{
    const int opp = opp_color(stm);
    const Square k_sq = first_bit(bitboard[WK+stm]);
    Bitboard occ = occupied ^
                   (Bitboard(1)<<from | Bitboard(1)<<to | Bitboard(1) << c_sq);
    return (bb_rook_attack(k_sq,occ)&(bitboard[WR+opp]|bitboard[WQ+opp])
            || (bb_bishop_attack(k_sq,occ)&(bitboard[WB+opp]|bitboard[WQ+opp]))
            || (bb_pawn_attack(stm,k_sq) & (bitboard)[WP+opp])
            || (knight_attack(k_sq) & (bitboard)[WN+opp]));
}

void Position::add(const Piece p, const Square sq){
    bb_add_piece(bitboard[piece_color(p)],sq);
    bb_add_piece(bitboard[p],sq);
    bb_add_piece(occupied,sq);
    piece[sq] = p;
}

void Position::remove(const Square sq){
    const Piece p = piece[sq];
    if(p == Empty) return;
    bb_remove_piece(bitboard[piece_color(p)],sq);
    bb_remove_piece(bitboard[p],sq);
    bb_remove_piece(occupied,sq);
    piece[sq] = Empty;
}

void Position::empty(){
    stm = White;
    for(Piece p=W; p<=BK; p++) bitboard[p] = 0;
    for(Square sq=A1; sq<=H8; sq++) piece[sq] = Empty;
    checkers = occupied = 0;
    castle = N0_CASTLE;
    ep = NO_SQ;
}

void Position::initial(){
    empty();
    for(Square sq = A2; sq <= H2; sq++) add(WP, sq);
    for(Square sq = A7; sq <= H7; sq++) add(BP, sq);
    add(WR,A1);
    add(WN,B1);
    add(WB,C1);
    add(WQ,D1);
    add(WK,E1);
    add(WB,F1);
    add(WN,G1);
    add(WR,H1);
    add(BR,A8);
    add(BN,B8);
    add(BB,C8);
    add(BQ,D8);
    add(BK,E8);
    add(BB,F8);
    add(BN,G8);
    add(BR,H8);
    stm = White;
    castle = Castle(W_CASTLE | B_CASTLE);
}

void Position::moveDo(Move & m){
    m.set_ep(ep);
    m.set_castle(castle);
    m.set_captured(Empty);
    switch(m.getType()){
        case QuietMove:     { quiet_do(m); break; }
        case CaptureMove:   { capture_do(m); break; }
        case CastleMove:    { castle_do(m); break; }
        case EnPassanMove:  { ep_do(m); break;}
        case PromotionKnightMove:
        case PromotionBishopMove:
        case PromotionRookMove:
        case PromotionQueenMove:
                            { promotion_do(m); break;}
        case NullMove:      { break; }
        default:            { }
    };
    stm = opp_color(stm);
    checkers = get_checkers();
    attacking_figure = m.getTo();
    //qDebug() << "attacker" << QString::fromStdString(square_to_string(m.getTo()));
}

void Position::move_undo(const Move m){
    stm = opp_color(stm);
    switch(m.getType()){
        case QuietMove:     { quiet_undo(m); break; }
        case CaptureMove:   { capture_undo(m); break; }
        case CastleMove:    { castle_undo(m); break; }
        case EnPassanMove:  { ep_undo(m); break; }
        case PromotionKnightMove:
        case PromotionBishopMove:
        case PromotionRookMove:
        case PromotionQueenMove:
                            { promotion_undo(m); break;}
        case NullMove:      { break; }
        default:            { }
    };
    castle = m.get_castle();
    ep = m.get_ep();
}

void Position::quiet_do(const Move m){
    const Square from = m.getFrom();
    const Square to = m.getTo();
    const Piece p = piece[from];
    bb_move_piece(bitboard[stm],from,to);
    bb_move_piece(bitboard[p],from,to);
    bb_move_piece(occupied,from,to);
    piece[from] = Empty;
    piece[to] = p;
    if(stm == White){
        if(p == WP){
            if(to - from == 16 && (bitboard[BP] & bb_pawn_attack(White,to-8)))
                ep = to - 8;
        }
        if(castle & W_CASTLE){
            if(castle & WK_CASTLE){
                if(from == E1 || from == H1) castle &= ~ WK_CASTLE;
            }
            if(castle & WQ_CASTLE){
                if(from == E1 || from == A1) castle &= ~WQ_CASTLE;
            }
        }
    }
    else{
        if(p == BP){
            if(from - to == 16 && (bitboard[WP] & bb_pawn_attack(Black,to+8)))
                ep = to + 8;
        }
        if(castle & B_CASTLE){
            if(castle & BK_CASTLE){
                if(from == E8 || from == H8) castle &= ~BK_CASTLE;
            }
            if(castle & BQ_CASTLE){
                if(from == E8 || from == A8) castle &= ~BQ_CASTLE;
            }
        }
    }
}

void Position::quiet_undo(const Move m){
    const Square from = m.getFrom();
    const Square to = m.getTo();
    const Piece p = piece[to];
    bb_move_piece(bitboard[stm],to,from);
    bb_move_piece(bitboard[p],to,from);
    bb_move_piece(occupied,to,from);
    piece[from] = p;
    piece[to] = Empty;
}

void Position::capture_do(Move& m){
    const Square from = m.getFrom();
    const Square to = m.getTo();
    const Piece p = piece[from];
    const Piece c = piece[to];
    m.set_captured(c);
    bb_remove_piece(occupied,from);
    bb_move_piece(bitboard[stm],from,to);
    bb_move_piece(bitboard[p],from,to);
    bb_remove_piece(bitboard[opp_color(stm)],to);
    bb_remove_piece(bitboard[c],to);
    piece[from] = Empty;
    piece[to] = p;
    if(stm == White){
        if(castle & W_CASTLE){
            if(castle & WK_CASTLE){
                if(from == E1 || from == H1) castle &= ~WK_CASTLE;
            }
            if(castle & WQ_CASTLE){
                if(from == E1 || from == A1) castle &= ~WQ_CASTLE;
            }
        }
        if(castle & B_CASTLE){
            if(castle & BK_CASTLE){
                if(to == H8) castle &= ~BK_CASTLE;
            }
            if(castle & BQ_CASTLE){
                if(to == A8) castle &= ~BQ_CASTLE;
            }
        }
    }
    else{
        if(castle & B_CASTLE){
            if(castle & BK_CASTLE){
                if(from == E8 || from == H8) castle &= ~BK_CASTLE;
            }
            if(castle & BQ_CASTLE){
                if(from == E8 || from == A8) castle &= ~BQ_CASTLE;
            }
        }
        if(castle & W_CASTLE){
            if(castle & WK_CASTLE){
                if(to == H1) castle &= ~WK_CASTLE;
            }
            if(castle & WQ_CASTLE){
                if(to == A1) castle &= ~WQ_CASTLE;
            }
        }
    }
}

void Position::capture_undo(const Move m){
    const Square from = m.getFrom();
    const Square to = m.getTo();
    const Piece p = piece[to];
    const Piece c = m.get_captured();

    bb_add_piece(occupied,from);
    bb_move_piece(bitboard[stm],to,from);
    bb_move_piece(bitboard[p],to,from);
    bb_add_piece(bitboard[opp_color(stm)],to);
    bb_add_piece(bitboard[c],to);
    piece[from] = p;
    piece[to] = c;
}

void Position::ep_do(Move & m){
    const Square from = m.getFrom();
    const Square to = m.getTo();
    const Piece p = Piece(WP + stm);
    const Piece c = Piece(WP + opp_color(stm));
    Square c_sq;
    if(stm == Black) c_sq = to + 8;
    else c_sq = to - 8;
    m.set_captured(c);
    bb_move_piece(bitboard[stm],from,to);
    bb_move_piece(bitboard[p],from,to);
    bb_remove_piece(bitboard[opp_color(stm)],c_sq);
    bb_remove_piece(bitboard[c],c_sq);
    bb_move_piece(occupied,from,to);
    bb_remove_piece(occupied,c_sq);
    piece[from] = Empty;
    piece[c_sq] = Empty;
    piece[to] = p;
}

void Position::ep_undo(const Move m){
    Square c_sq;
    const Square from = m.getFrom();
    const Square to = m.getTo();
    const Piece p = Piece(WP + stm);
    const Piece c = Piece(WP + opp_color(stm));
    if(stm == White) c_sq = to - 8;
    else c_sq = to + 8;
    bb_move_piece(bitboard[stm],to,from);
    bb_move_piece(bitboard[p],to,from);
    bb_add_piece(bitboard[opp_color(stm)],c_sq);
    bb_add_piece(bitboard[c],c_sq);
    bb_move_piece(occupied,to,from);
    bb_add_piece(occupied,c_sq);
    piece[from] = p;
    piece[to] = Empty;
    piece[c_sq] = Piece(WP + opp_color(stm));
}

void Position::castle_do(Move &m){
    Square r_from, r_to;
    const Square to = m.getTo();
    const Square from = m.getFrom();
    const Piece p = Piece(WK + stm);
    const Piece r = Piece(WR + stm);
    switch(to){
        case G1: {
            r_from = H1;
            r_to = F1;
            m.set_castle(WK_CASTLE);
            break;
        }
        case C1: {
            r_from = A1;
            r_to = D1;
            m.set_castle(WQ_CASTLE);
            break;
        }
        case G8: {
            r_from = H8;
            r_to = F8;
            m.set_castle(BK_CASTLE);
            break;
        }
        case C8: {
            r_from = A8;
            r_to = D8;
            m.set_castle(BQ_CASTLE);
            break;
        }
        default: { }
    }
    bb_move_piece(bitboard[stm],from,to);
    bb_move_piece(bitboard[stm],r_from,r_to);
    bb_move_piece(occupied,from,to);
    bb_move_piece(occupied,r_from,r_to);
    bb_move_piece(bitboard[p],from,to);
    bb_move_piece(bitboard[r],r_from,r_to);
    piece[from] = piece[r_from] = Empty;
    piece[to] = p;
    piece[r_to] = r;
}

void Position::castle_undo(const Move m){
    Piece p, r;
    Square r_from, r_to;
    const Square from = m.getFrom();
    const Square to = m.getTo();
    switch(to){
        case G1: {
            r_from = H1;
            r_to = F1;
            p = WK;
            r = WR;
            break;
        }
        case C1: {
            r_from = A1;
            r_to = D1;
            p = WK;
            r = WR;
            break;
        }
        case G8: {
            r_from = H8;
            r_to = F8;
            p = BK;
            r = BR;
            break;
        }
        case C8: {
            r_from = A8;
            r_to = D8;
            p = BK;
            r = BR;
            break;
        }
        default: { }
    }
    bb_move_piece(bitboard[stm],to,from);
    bb_move_piece(bitboard[stm],r_to,r_from);
    bb_move_piece(occupied,to,from);
    bb_move_piece(occupied,r_to,r_from);
    bb_move_piece(bitboard[p],to,from);
    bb_move_piece(bitboard[r],r_to,r_from);
    piece[from] = p;
    piece[r_from] = r;
    piece[r_to] = piece[to] = Empty;
}

void Position::promotion_do(Move & m){
    const Square from = m.getFrom();
    const Square to = m.getTo();
    const Piece p = Piece(WP + stm);
    const Piece pr = Piece(m.getType() + stm);
    const Piece c =piece[to];
    m.set_captured(c);
    bb_move_piece(bitboard[stm],from,to);
    bb_move_piece(occupied,from,to);
    bb_remove_piece(bitboard[p],from);
    bb_add_piece(bitboard[pr],to);
    piece[from] = Empty;
    piece[to] = pr;
    if(c != Empty){
        if(stm == White){
            if(castle & B_CASTLE){
                if(castle & BK_CASTLE){
                    if(to == H8){
                        castle &= ~BK_CASTLE;
                    }
                }
                if(castle & BQ_CASTLE){
                    if(to == A8){
                        castle &= ~BQ_CASTLE;
                    }
                }
            }
        }
        else{
            if(castle & W_CASTLE){
                if(castle & WK_CASTLE){
                    if(to == H1){
                        castle &= ~WK_CASTLE;
                    }
                }
                if(castle & WQ_CASTLE){
                    if(to == A1){
                        castle &= ~WQ_CASTLE;
                    }
                }
            }
        }
        bb_remove_piece(bitboard[opp_color(stm)],to);
        bb_remove_piece(bitboard[c],to);
        bb_add_piece(occupied,to);
    }
}

void Position::promotion_undo(const Move m){
    const Square from = m.getFrom();
    const Square to = m.getTo();
    const Piece p = Piece(WP + stm);
    const Piece pr = Piece(m.getType() + stm);
    const Piece c = m.get_captured();
    bb_move_piece(bitboard[stm],to,from);
    bb_move_piece(occupied,to,from);
    bb_add_piece(bitboard[p],from);
    bb_remove_piece(bitboard[pr],to);
    piece[from] = p;
    piece[to] = Empty;
    if(c != Empty){
        bb_add_piece(bitboard[opp_color(stm)],to);
        bb_add_piece(bitboard[c],to);
        bb_add_piece(occupied,to);
        piece[to] = c;
    }
}

void Position::getMoves(Move m[256]){
    int index = 0;
    Bitboard bb, bb_dest, bb_attack;
    const Bitboard bb_empty = ~occupied, bb_opponent = bitboard[opp_color(stm)];
    Square from, to, c_sq;
    for(Piece p = Piece(WN+stm); p <= BQ; p += 2){
        bb = bitboard[p];
        while(bb){
            from = first_bit_clear(bb);
            bb_attack = (this->*attack[p])(from);
            bb_dest = bb_attack & bb_empty;
            while(bb_dest){
                to = first_bit_clear(bb_dest);
                if(!is_pinned(from,to)) m[index++] = Move(QuietMove,from,to);
            }
            bb_dest = bb_attack & bb_opponent;
            while(bb_dest){
                to = first_bit_clear(bb_dest);
                //if(!is_pinned(from,to)) m[index++] = Move(CaptureMove,from,to);

                if(!is_pinned(from,to)/* || exception_attacking_king && from == exception_square*/)
                    //qDebug() << stm << "pinned: " << is_pinned(from,to) << "attack: " << exception_attacking_king << QString::fromStdString(square_to_string(from)) << " == " << QString::fromStdString(square_to_string(exception_square));
                    m[index++] = Move(CaptureMove,from,to);
                }
            }
        }

    bb = bitboard[WP + stm];
    if(stm == White){
        bb_dest = bb_attack = bb_shift_up(bb) & bb_empty & ~BB_RANK_8;
        while(bb_dest){ //normal move
            to = first_bit_clear(bb_dest);
            from = to - 8;
            if(!is_pinned(from,to)) m[index++] = Move(QuietMove,from,to);
        }
        bb_dest = bb_shift_up(bb_attack) & bb_empty & BB_RANK_4;
        while(bb_dest){ //double move
            to = first_bit_clear(bb_dest);
            from = to - 16;
            if(!is_pinned(from,to)) m[index++] = Move(QuietMove,from,to);
        }
        bb_dest = bb_shift_up(bb) & bb_empty & BB_RANK_8;
        while(bb_dest){ //center
            to = first_bit_clear(bb_dest);
            from = to - 8;
            if(!is_pinned(from,to)){
                for(int i= PromotionQueenMove; i>= PromotionKnightMove; i--)
                    m[index++] = Move(i,from,to);
            }
        }
        bb_dest = bb_shift_up_right(bb) & bb_opponent;
        while(bb_dest){ //left
            to = first_bit_clear(bb_dest);
            from = to - 9;
            if(!is_pinned(from,to)){
                if(to >= A8){
                    for(int i= PromotionQueenMove; i>= PromotionKnightMove; i--)
                        m[index++] = Move(i,from,to);
                }
                else
                    m[index++] = Move(CaptureMove,from,to);
            }
        }
        bb_dest = bb_shift_up_left(bb) & bb_opponent;
        while(bb_dest){ //right
            to = first_bit_clear(bb_dest);
            from = to - 7;
            if(!is_pinned(from,to)){
                if(to >= A8){
                    for(int i= PromotionQueenMove; i>= PromotionKnightMove; i--)
                        m[index++] = Move(i,from,to);
                }
                else m[index++] = Move(CaptureMove,to-7,to);
            }
        }
        if(!checkers && castle & W_CASTLE){
            if(castle & WK_CASTLE
                && (((BB_F1|BB_G1) & bb_empty) == (BB_F1|BB_G1))
                && !sq_attacked(F1,White)
                && ((BB_F2|BB_G2|BB_H2) & bitboard[BK]) == 0){
                    if(!is_attacked(E1,G1)) m[index++] = Move(CastleMove,E1,G1);
                }
            if(castle & WQ_CASTLE
                && (((BB_B1|BB_C1|BB_D1)& bb_empty) == (BB_B1|BB_C1|BB_D1))
                && !sq_attacked(D1,White)
                && ((BB_B2|BB_C2|BB_D2) & bitboard[BK]) == 0){
                    if(!is_attacked(E1,C1)) m[index++] = Move(CastleMove,E1,C1);
                }
        }
    }
    else{
        bb_dest = bb_attack = bb_shift_down(bb) & bb_empty & ~BB_RANK_1;
        while(bb_dest){
            to = first_bit_clear(bb_dest);
            from = to + 8;
            if(!is_pinned(from,to)) m[index++] = Move(QuietMove,from,to);
        }
        bb_dest = bb_shift_down(bb_attack) & bb_empty & BB_RANK_5;
        while(bb_dest){
            to = first_bit_clear(bb_dest);
            from = to + 16;
            if(!is_pinned(from,to)) m[index++] = Move(QuietMove,from,to);
        }
        bb_dest = bb_shift_down(bb) & bb_empty & BB_RANK_1;
        while(bb_dest){
            to = first_bit_clear(bb_dest);
            from = to + 8;
            if(!is_pinned(from,to)){
                for(int i= PromotionQueenMove; i>= PromotionKnightMove; i--)
                    m[index++] = Move(i,from,to);
            }
        }
        bb_dest = bb_shift_down_right(bb) & bb_opponent;
        while(bb_dest){
            to = first_bit_clear(bb_dest);
            from = to + 7;
            if(!is_pinned(from,to)){
                if(to <= H1){
                    for(int i= PromotionQueenMove; i>= PromotionKnightMove; i--)
                        m[index++] = Move(i,from,to);
                }
                else m[index++] = Move(CaptureMove,from,to);
             }
        }
        bb_dest = bb_shift_down_left(bb) & bb_opponent;
        while(bb_dest){
            to = first_bit_clear(bb_dest);
            from = to + 9;
            if(!is_pinned(from,to)){
                if(to <= H1){
                    for(int i= PromotionQueenMove; i>= PromotionKnightMove; i--)
                        m[index++] = Move(i,from,to);
                }
                else m[index++] = Move(CaptureMove,from,to);
            }
        }
        if(!checkers && castle & B_CASTLE){
            if(castle & BK_CASTLE
                && (((BB_F8|BB_G8) & bb_empty) == (BB_F8|BB_G8))
                && !sq_attacked(F8,Black)
                && ((BB_F7|BB_G7|BB_H7) & bitboard[WK]) == 0){
                    if(!is_attacked(E8,G8)) m[index++] = Move(CastleMove,E8,G8);
                }
            if(castle & BQ_CASTLE
                && (((BB_B8|BB_C8|BB_D8)& bb_empty) == (BB_B8|BB_C8|BB_D8))
                && !sq_attacked(D8,Black)
                && ((BB_B7|BB_C7|BB_D7) & bitboard[WK]) == 0){
                    if(!is_attacked(E8,C8)) m[index++] = Move(CastleMove,E8,C8);
                }
        }
    }
    if(ep != NO_SQ){
        bb_dest = bb & bb_pawn_attack(opp_color(stm),ep);
        while(bb_dest){
            from = first_bit_clear(bb_dest);
            if(stm == Black) c_sq = to+8;
            else c_sq = to-8;
            if(!is_pinned(from,ep,c_sq)){
                m[index++] = Move(EnPassanMove,from,ep);
                enPassant = true;
            }
        }
    }
    from = first_bit(bitboard[WK+stm]);
    bb_attack = king_attack(from)
                & ~king_attack(first_bit(bitboard[WK+opp_color(stm)]));
    bb_dest = bb_attack & bitboard[opp_color(stm)];
    while(bb_dest){
            to = first_bit_clear(bb_dest);
            if(!is_attacked(from,to)) m[index++] = Move(CaptureMove,from,to); //king capture move
    }
    bb_dest = bb_attack & bb_empty;
    while(bb_dest){
        to = first_bit_clear(bb_dest);
        if(!is_attacked(from,to)) m[index++] = Move(QuietMove,from,to); //king quiet move
    }
    m[index] = Move(NullMove,A1,A1);
}

std::string Position::to_string() const{
    Piece p;
    std::string board;
    std::stringstream stream;
    for(int i=0; i<8; i++) board += "# # # # \n";
    for(Square sq = A1; sq<=H8; sq++){
        p = piece[sq];
        if(p != Empty)
            board[70-rank(sq)*9-(7-file(sq))] = piece_to_char(p);
    }
    stream<<board<<std::endl;
    if(stm == White) stream << "stm: White"<<std::endl;
    else stream<<"stm: Black"<<std::endl;
    stream<<"ep: "<<square_to_string(ep)<<std::endl;
    stream<<"Castle: w( ";
    if(castle & WK_CASTLE) stream<<"00 ";
    if(castle & WQ_CASTLE) stream<<"000 ";
    stream<<") b( ";
    if(castle & BK_CASTLE) stream<<"00 ";
    if(castle & BQ_CASTLE) stream<<"000 ";
    stream<<")"<<std::endl;
    return stream.str();
}
