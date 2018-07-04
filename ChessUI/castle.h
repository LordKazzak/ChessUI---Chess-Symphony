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

#ifndef CASTLE_H
#define CASTLE_H

enum Castle{
    N0_CASTLE = 0,
    WK_CASTLE = 1, WQ_CASTLE = 2, W_CASTLE = 3,
    BK_CASTLE = 4, BQ_CASTLE = 8, B_CASTLE = 12
};

inline void operator&= (Castle& c, int x) {
        c = Castle(int(c) & x);
}

inline void operator|= (Castle& c, int x) {
        c = Castle(int(c) | x);
}

#endif // CASTLE_H
