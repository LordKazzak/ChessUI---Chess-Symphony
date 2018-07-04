#ifndef CHESSUI_H
#define CHESSUI_H

#include <QMainWindow>
#include "engine.h"
#include "settings.h"
#include "about.h"
#include <stdio.h>
#include "QPlainTextEdit"
#include "QWidget"
#include <QTableWidget>
#include <QLabel>
namespace Ui {

class ChessUI;
}

class ChessUI : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ChessUI(QWidget *parent = 0);
    ~ChessUI();
    
private:
    QWidget * widget;
    Board2D * board;
    Ui::ChessUI *ui;
    Settings * settings;
    void drawBoard();
    QStringList dataList;
    /*QPlainTextEdit *right;
    QTableWidget *bottom;
    QLabel * white;
    QLabel * black;*/
    void fillTitleBar(const QString &text);
    void fillTableRow(const QString &text);

private slots:
    //void showText(QPlainTextEdit *edit, const QString &message);
    void showRightText(const QString &message);
    void showBottomText(const QString &message);
    void changeLabel(const bool &label, const QString &text);
    void showSettings();
    void showAbout();
    void startEngine();
    void againstHuman();
};

#endif // CHESSUI_H
