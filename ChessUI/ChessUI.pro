#-------------------------------------------------
#
# Project created by QtCreator 2014-03-01T16:43:36
#
# Author: Martin Konečnik
# Contact: martin.konecnik@gmail.com
# All rights reserved
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChessUI
TEMPLATE = app
RESOURCES = media/images.qrc
DESTDIR = $$PWD
CONFIG += debug_and_release

SOURCES += main.cpp\
        chessui.cpp \
    square.cpp \
    position.cpp \
    piece.cpp \
    move.cpp \
    color.cpp \
    castle.cpp \
    board2d.cpp \
    bitboard.cpp \
    settings.cpp \
    engine.cpp \
    global.cpp \
    about.cpp

HEADERS  += chessui.h \
    square.h \
    position.h \
    piece.h \
    move.h \
    color.h \
    castle.h \
    board2d.h \
    bitboard.h \
    settings.h \
    engine.h \
    global.h \
    about.h

FORMS    += chessui.ui \
    settings.ui \
    about.ui

TRANSLATIONS = chessui_sl.ts
