#ifndef Engine_H
#define Engine_H

#include <QProcess>
#include <QStringList>
#include "square.h"
#include "settings.h"

class Engine : public QProcess
{
    Q_OBJECT

signals:
    void updateRightText(const QString& tmp);
    void updateBottomText(const QString& tmp);

public:
    Engine();
    ~Engine();
    bool run(QString enginePath, int depth, int time);
    QList<Square> move(int depth, int time);
    static Engine * engine;
    static Engine * instance(){
        if(!engine)
            engine = new Engine();
        return engine;
    }
    static void deleteEngine(){
        if(engine){
            delete engine;
            engine = NULL;
        }
    }
    void setBoardPosition(std::string move){position.append(move);}

private:
    std::string position; //string trenutnega položaja na šahovnici (moves)
    QStringList parseCommand(QString string);
    bool isReady();
    void sendCommand(const char *string, QString success="y", QString failure="n");
    //void setOptions(int difficulty);
};

#endif // Engine_H
