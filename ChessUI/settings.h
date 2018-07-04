#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QMainWindow>

class Board2D;

namespace Ui {
    class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QMainWindow * mainWindow);
    ~Settings();
    inline QColor getWhiteSquareColor(){
        return wSqColor;
    }
    inline QColor getBlackSquareColor(){
        return bSqColor;
    }

    inline QFont getBoardFont(){
        return boardFont;
    }

    inline QString getPiecePath(){
        return piecePath;
    }

    inline QString getWSqPath(){
        return wSqPath;
    }

    inline QString getBSqPath(){
        return bSqPath;
    }

    inline bool getSWSqColor(){
        return sWSqColor;
    }

    inline bool getSBSqColor(){
        return sBSqColor;
    }
    void setIsHuman(int color, bool is_human){
        isHuman[color] = is_human;
    }
    inline bool getIsHuman(int color){
        return isHuman[color];
    }
    inline QString getEnginePath(){
        return enginePath;
    }
    inline int getDepth(){
        return depth;
    }
    inline int getTime(){
        return time * 1000;
    }

    inline QString getLanguage(){
        return language;
    }

private:
    Ui::Settings *ui;
    QMainWindow* mainWindow;
    QColor wSqColor;
    QColor tmpWSqColor;
    QColor bSqColor;
    QColor tmpBSqColor;
    QFont boardFont;
    QFont tmpBoardFont;
    QString piecePath;
    QString tmpPiecePath;
    QString wSqPath;
    QString tmpWSqPath;
    QString bSqPath;
    QString tmpBSqPath;
    QString enginePath;
    bool sWSqColor;
    bool tmpSWSqColor;
    bool sBSqColor;
    bool tmpSBSqColor;
    bool isHuman[2];
    int depth;
    int time;
    QString language;

    void save();
    void load();

private slots:
    void on_bSqPathButton_clicked();
    void on_wSqPathButton_clicked();
    void on_pieceButton_clicked();
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void on_fontButton_clicked();
    void on_wSqCButton_clicked();
    void on_bSqCButton_clicked();
    void on_pushButtonEnginePath_clicked();
};

#endif // SETTINGS_H
