#ifndef BOARD2D_H
#define BOARD2D_H

#include <QWidget>
#include <QDebug>
#include "settings.h"
#include "position.h"

class Board2D : public QWidget, Position
{
    Q_OBJECT

signals:
    void sendTextUI(const QString &text);

public:
    explicit Board2D(Settings* settings, QWidget *parent = 0);
    bool fliped;

private:
    Settings* settings;
    QColor wSqColor;
    QColor bSqColor;
    QFont font;
    QPixmap piece[14];
    QString wSqPath;
    QString bSqPath;
    QString piecePath;
    QList<Square> colored_squares;
    bool sWSqColor;
    bool sBSqColor;

    //QRect tmpRect;
    QList<Square> attacked; //list of squares that are attacked by selected piece
    Square king_attacked;
    bool color_king;
    Square from;
    int mX;
    int mY;
    Piece mP;
    Move move[256];
    bool imediate;
    bool nullMove;

    void load();
    Square getSquare(QPoint point);
    void updateSquare(const Square sq, bool imediate);
    void paintEvent(QPaintEvent* e);
    void changeEvent(QEvent * event);
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void drawMove(Square &from, Square to, Color &player);
};

#endif // BOARD2D_H
