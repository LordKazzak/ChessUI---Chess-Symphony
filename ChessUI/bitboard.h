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

#ifndef BITBOARD_H
#define BITBOARD_H

#include <inttypes.h>

#include "color.h"
#include "square.h"

typedef uint64_t Bitboard;

extern Bitboard BB_PawnAttack[2][64];
extern Bitboard BB_KnightAttack[64];
extern Bitboard BB_KingAttack[64];
extern Bitboard BB_BishopAttack[64];
extern Bitboard BB_RookAttack[64];
extern Bitboard BB_QueenAttack[64];

const Bitboard BB_A1 = Bitboard(1);
const Bitboard BB_B1 = BB_A1<<1;
const Bitboard BB_C1 = BB_A1<<2;
const Bitboard BB_D1 = BB_A1<<3;
const Bitboard BB_E1 = BB_A1<<4;
const Bitboard BB_F1 = BB_A1<<5;
const Bitboard BB_G1 = BB_A1<<6;
const Bitboard BB_H1 = BB_A1<<7;
const Bitboard BB_A2 = BB_A1<<8;
const Bitboard BB_B2 = BB_A1<<9;
const Bitboard BB_C2 = BB_A1<<10;
const Bitboard BB_D2 = BB_A1<<11;
const Bitboard BB_E2 = BB_A1<<12;
const Bitboard BB_F2 = BB_A1<<13;
const Bitboard BB_G2 = BB_A1<<14;
const Bitboard BB_H2 = BB_A1<<15;
const Bitboard BB_A3 = BB_A1<<16;
const Bitboard BB_B3 = BB_A1<<17;
const Bitboard BB_C3 = BB_A1<<18;
const Bitboard BB_D3 = BB_A1<<19;
const Bitboard BB_E3 = BB_A1<<20;
const Bitboard BB_F3 = BB_A1<<21;
const Bitboard BB_G3 = BB_A1<<22;
const Bitboard BB_H3 = BB_A1<<23;
const Bitboard BB_A4 = BB_A1<<24;
const Bitboard BB_B4 = BB_A1<<25;
const Bitboard BB_C4 = BB_A1<<26;
const Bitboard BB_D4 = BB_A1<<27;
const Bitboard BB_E4 = BB_A1<<28;
const Bitboard BB_F4 = BB_A1<<29;
const Bitboard BB_G4 = BB_A1<<30;
const Bitboard BB_H4 = BB_A1<<31;
const Bitboard BB_A5 = BB_A1<<32;
const Bitboard BB_B5 = BB_A1<<33;
const Bitboard BB_C5 = BB_A1<<34;
const Bitboard BB_D5 = BB_A1<<35;
const Bitboard BB_E5 = BB_A1<<36;
const Bitboard BB_F5 = BB_A1<<37;
const Bitboard BB_G5 = BB_A1<<38;
const Bitboard BB_H5 = BB_A1<<39;
const Bitboard BB_A6 = BB_A1<<40;
const Bitboard BB_B6 = BB_A1<<41;
const Bitboard BB_C6 = BB_A1<<42;
const Bitboard BB_D6 = BB_A1<<43;
const Bitboard BB_E6 = BB_A1<<44;
const Bitboard BB_F6 = BB_A1<<45;
const Bitboard BB_G6 = BB_A1<<46;
const Bitboard BB_H6 = BB_A1<<47;
const Bitboard BB_A7 = BB_A1<<48;
const Bitboard BB_B7 = BB_A1<<49;
const Bitboard BB_C7 = BB_A1<<50;
const Bitboard BB_D7 = BB_A1<<51;
const Bitboard BB_E7 = BB_A1<<52;
const Bitboard BB_F7 = BB_A1<<53;
const Bitboard BB_G7 = BB_A1<<54;
const Bitboard BB_H7 = BB_A1<<55;
const Bitboard BB_A8 = BB_A1<<56;
const Bitboard BB_B8 = BB_A1<<57;
const Bitboard BB_C8 = BB_A1<<58;
const Bitboard BB_D8 = BB_A1<<59;
const Bitboard BB_E8 = BB_A1<<60;
const Bitboard BB_F8 = BB_A1<<61;
const Bitboard BB_G8 = BB_A1<<62;
const Bitboard BB_H8 = BB_A1<<63;

const Bitboard BB_FILE_A = BB_A1 | BB_A2 | BB_A3 | BB_A4
                           | BB_A5 | BB_A6 | BB_A7 | BB_A8;
const Bitboard BB_FILE_B = BB_FILE_A<<1;
const Bitboard BB_FILE_C = BB_FILE_A<<2;
const Bitboard BB_FILE_D = BB_FILE_A<<3;
const Bitboard BB_FILE_E = BB_FILE_A<<4;
const Bitboard BB_FILE_F = BB_FILE_A<<5;
const Bitboard BB_FILE_G = BB_FILE_A<<6;
const Bitboard BB_FILE_H = BB_FILE_A<<7;

const Bitboard BB_FILE[8] = {
    BB_FILE_A, BB_FILE_B, BB_FILE_C, BB_FILE_D,
    BB_FILE_E, BB_FILE_F, BB_FILE_G, BB_FILE_H
};

const Bitboard BB_NOT_FILE_A = ~ BB_FILE_A;
const Bitboard BB_NOT_FILE_H = ~ BB_FILE_H;
const Bitboard BB_NOT_FILES_AB = ~(BB_FILE_A | BB_FILE_B);
const Bitboard BB_NOT_FILES_GH = ~(BB_FILE_G | BB_FILE_H);

const Bitboard BB_RANK_1 = BB_A1 | BB_B1 | BB_C1
                           | BB_D1 | BB_E1 | BB_F1 | BB_G1 | BB_H1;
const Bitboard BB_RANK_2 = BB_RANK_1<<8;
const Bitboard BB_RANK_3 = BB_RANK_2<<8;
const Bitboard BB_RANK_4 = BB_RANK_3<<8;
const Bitboard BB_RANK_5 = BB_RANK_4<<8;
const Bitboard BB_RANK_6 = BB_RANK_5<<8;
const Bitboard BB_RANK_7 = BB_RANK_6<<8;
const Bitboard BB_RANK_8 = BB_RANK_7<<8;

const Bitboard BB_RANK[8] = {
    BB_RANK_1, BB_RANK_2, BB_RANK_3, BB_RANK_4,
    BB_RANK_5, BB_RANK_6, BB_RANK_7, BB_RANK_8
};

extern const int magic_bb_r_shift[64];
extern const Bitboard magic_bb_r_magics[64];
extern const Bitboard magic_bb_r_mask[64];
extern const Bitboard * magic_bb_r_indices[64];
extern Bitboard magic_bb_r_db[102400];
extern const int magic_bb_b_shift[64];
extern const Bitboard magic_bb_b_magics[64];
extern const Bitboard magic_bb_b_mask[64];
extern const Bitboard * magic_bb_b_indices[64];
extern Bitboard magic_bb_b_db[5248];

void bb_init();

inline void bb_add_piece(Bitboard & bb, const int sq){
    bb |= (Bitboard(1) << sq);
}

inline void bb_remove_piece(Bitboard & bb, const int sq){
    bb &= ~(Bitboard(1)<<sq);
}

inline void bb_move_piece(Bitboard & bb, const int from, const int to){
    bb ^= (Bitboard(1) << from) | (Bitboard(1) << to);
}

inline Bitboard bb_shift_down(const Bitboard b){
    return b>>8;
}

inline Bitboard bb_shift_2_down(const Bitboard b){
    return b>>16;
}

inline Bitboard bb_shift_up(const Bitboard b){
    return b<<8;
}

inline Bitboard bb_shift_2_up(const Bitboard b){
    return (b<<16);
}

inline Bitboard bb_shift_right(const Bitboard b){
    return (b<<1) & BB_NOT_FILE_A;
}

inline Bitboard bb_shift_2_right(const Bitboard b){
    return (b<<2) & BB_NOT_FILES_AB;
}

inline Bitboard bb_shift_left(const Bitboard b){
    return (b>>1) & BB_NOT_FILE_H;
}

inline Bitboard bb_shift_2_left(const Bitboard b){
    return (b>>2) & BB_NOT_FILES_GH;
}

inline Bitboard bb_shift_up_left(const Bitboard b){
    return (b<<7) & BB_NOT_FILE_H;
}

inline Bitboard bb_shift_up_right(const Bitboard b){
    return (b<<9) & BB_NOT_FILE_A;
}

inline Bitboard bb_shift_down_left(const Bitboard b){
    return (b>>9) & BB_NOT_FILE_H;
}

inline Bitboard bb_shift_down_right(const Bitboard b){
    return (b>>7) & BB_NOT_FILE_A;
}

inline Bitboard bb_shift_2_up_right(const Bitboard b){
    return (b<<17)& BB_NOT_FILE_A;
}

inline Bitboard bb_shift_2_up_left(const Bitboard b){
    return (b<<15)& BB_NOT_FILE_H;
}

inline Bitboard bb_shift_2_down_left(const Bitboard b){
    return (b>>17)& BB_NOT_FILE_H;
}

inline Bitboard bb_shift_2_down_right(const Bitboard b){
    return (b>>15)& BB_NOT_FILE_A;
}

inline Bitboard bb_pawn_attack(const Color c, const Square sq){
    return BB_PawnAttack[c][sq];
}

inline Bitboard bb_bishop_attack(const Square sq, const Bitboard bb){
    return *(magic_bb_b_indices[sq]+
            (((bb&magic_bb_b_mask[sq])*magic_bb_b_magics[sq])
            >>magic_bb_b_shift[sq]));
}

inline Bitboard bb_rook_attack(const Square sq, const Bitboard bb){
    return *(magic_bb_r_indices[sq]+
            (((bb&magic_bb_r_mask[sq])*magic_bb_r_magics[sq])
             >>magic_bb_r_shift[sq]));
}

inline Bitboard bb_queen_attack(const Square sq, const Bitboard bb){
    return bb_rook_attack(sq,bb) | bb_bishop_attack(sq,bb);
}

#ifdef __x86_64__
inline Square __attribute__((always_inline)) first_bit(const Bitboard bb) {
        Square sq;
        __asm__("bsf %1, %%rcx\n\t": "=c"(sq): "r"(bb));
        return sq;
}

inline Square __attribute__((always_inline)) last_bit(const Bitboard bb){
        Square sq;
        __asm__("bsr %1,%%rcx\n\t":"=c" (sq): "r"(bb));
        return sq;
}

inline Square __attribute__((always_inline)) first_bit_clear(Bitboard& bb) {
        const Square s = first_bit(bb);
        bb &= ~(Bitboard(1)<<s);
        return s;
}
#elif __i386__
inline Square __attribute__((always_inline)) first_bit(Bitboard bb) {
        Square sq;
        __asm__(
                "bsf %1,%%ecx\n\t"
                "jnz 1f\n\t"
                "bsf %2,%%ecx\n\t"
                "add $32,%%ecx\n"
                "1:\n"
                :"=c" (sq)
                :"r" (bb), "r"(bb>>32)
        );
        return sq;
}

inline Square __attribute__((always_inline)) last_bit(Bitboard bb) {
        Square sq;
        __asm__(
                "bsr %2,%%ecx\n\t"
                "jnz 1f\n\t"
                "bsr %1,%%ecx\n\t"
                "jmp 2f\n\t"
                "1:\n"
                "add $32,%%ecx\n"
                "2:\n"
                :"=c" (sq)
                :"r" (bb), "r"(bb>>32)
        );
        return sq;
}

inline Square __attribute__((always_inline)) first_bit_clear(Bitboard& bb) {
        const Square s = first_bit(bb);
        bb &= ~(Bitboard(1)<<s);
        return s;
}

#else
extern const Square BitTable[64];
inline Square first_bit(Bitboard b) {
        return BitTable[((b & -b) * 0x218a392cd3d5dbfULL) >> 58];
}

inline Square last_bit(Bitboard b){
        Square sq = (Square)63;
        if (!(b & 0xffffffff00000000ull)){b <<= 32; sq -= 32;}
        if (!(b & 0xffff000000000000ull)){b <<= 16; sq -= 16;}
        if (!(b & 0xff00000000000000ull)){b <<= 8; sq -= 8;}
        if (!(b & 0xf000000000000000ull)){b <<= 4; sq -= 4;}
        if (!(b & 0xc000000000000000ull)){b <<= 2; sq -= 2;}
        if (!(b & 0x8000000000000000ull)){b <<= 1; sq -= 1;}
        return sq;
}

inline Square first_bit_clear(Bitboard& b) {
        Bitboard bb = b;
        b &= (b - 1);
        return BitTable[((bb & -bb) * 0x218a392cd3d5dbfULL) >> 58];
}
#endif

#endif // BITBOARD_H
