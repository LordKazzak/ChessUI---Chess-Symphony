#include "engine.h"
#include "color.h"
#include <stdio.h>
#include <QDebug>
#include <QTimerEvent>

Engine *Engine::engine = NULL;

Engine::Engine(){}

Engine::~Engine(){
    //qDebug() << "Engine destructor.";
    if(isOpen()){
        write("quit\n");
        waitForBytesWritten();
        qDebug() << "Close command sent to engine.";
        close();
    }
}

bool Engine::run(QString enginePath, int depth, int time){
    start(enginePath,QIODevice::ReadWrite);
    waitForReadyRead(1000);
    QString tmp = "";
    if(canReadLine())
        readLine();

    QList<QStringList> initdata;// = new QList<QStringList>(); //vsebuje id, nastavitve in "uciok"

    write("uci\n");
    waitForBytesWritten();

    waitForReadyRead();

    while(canReadLine()){
        tmp = readLine().trimmed();
        initdata.append(parseCommand(tmp));
        //updateRightText(tmp);
    }

    if(initdata.count()-1 < 0 || "uciok" != initdata.at(initdata.count()-1).at(0).trimmed()){
        updateRightText(tr("Loading failed!\nEngine did not respond with 'uciok'.\nCheck engine path in settings!"));
        write("quit\n");
        waitForBytesWritten();
        qDebug() << "Close command sent to engine.";
        close();
        return false;
    }

    updateRightText(initdata.at(0).at(2));

    sendCommand("setoption name Hash value 32\n", "Initialization successful!", "Hash value can not be set.");

    sendCommand("ucinewgame\n", "Newgame successful!", "Newgame failed!");

    if(!isReady())
        updateRightText(tr("Initialisation failure! Engine probably does not support 'ucinewgame' command."));
    else
        updateRightText(tr("Engine started"));

    sendCommand("position startpos\n");

    return true;
}

QStringList Engine::parseCommand(QString text){
    return text.split(" ");
}

bool Engine::isReady(){
    int i=0;
    while(i++<30){
        write("isready\n");
        waitForReadyRead(1000);
        if("readyok" == readLine().trimmed())
            return true;
    }
    updateRightText(tr("No response to command."));
    return false;
}

void Engine::sendCommand(const char * string, QString success, QString failure){
    write(string);
    waitForBytesWritten();

    waitForReadyRead(100);
    if(!canReadLine()){
        qDebug() << success;
    }
    else{
        //updateRightText(failure);
        qDebug() << failure;
        //qDebug() << readLine();
    }
}

QList<Square> Engine::move(int depth, int time)
{
    bool failure = false;
    std::string command = std::string("position startpos moves") + position + "\n";
    write(command.c_str());
    waitForBytesWritten();
/**/
    //timed search
    command = std::string("go movetime ") + QString::number(time).toStdString() + "\n";
    write(command.c_str());
    waitForBytesWritten();
/**/
/**/
    //depth search
    command = std::string("go depth ") + QString::number(depth).toStdString() + "\n"; //so it doesn't take too long (maxdepth 16)
    write(command.c_str());
    waitForBytesWritten();
/**/
    waitForReadyRead();

    QList<QString> list;
    QString listElem;
    //QStringList tmpList;

    while(canReadLine()){
        listElem = QString(readLine().trimmed());

        if("info" == listElem.split(" ").at(0)){
            qDebug() << listElem;
            updateBottomText(listElem);
        }
        list.append(listElem);
        if(!canReadLine() && parseCommand(listElem).at(0) != "bestmove")
            waitForReadyRead(); //če ne izpiše vsega naenkrat
    }

    QList<QString> mv = parseCommand(list.at(list.count()-1)); //zadnji ukaz je oblike "bestmove a1a2 ponder h1h2"

    qDebug() << "Move string: " << mv;
    if(mv.at(0) != "bestmove"){
        qDebug() << "Error! Move string not recognised!:\n " << mv.at(1);
        failure = true;
    }

    QString from_str, to_str;
    if(4 == mv.at(1).length()){
        from_str = mv.at(1).left(2);
        to_str = mv.at(1).right(2);
    }
    else if(5 == mv.at(1).length()){ //promotion
        from_str = mv.at(1).left(2);
        to_str = mv.at(1).mid(2,2);
    }
    else{
        qDebug() << mv.length();
        qDebug() << "Error: current datas " << list;
        failure = true;
    }

    QList<Square> squares;
    if(!failure){
        squares.append(get_square(from_str.toStdString()));
        squares.append(get_square(to_str.toStdString()));
    }
    else
        squares.append(NO_SQ);

    //qDebug() << "Original... From: " << from_str << "\tTo: " << to_str;
    //qDebug() << "Squares...  From: " << QString::fromStdString(square_to_string(squares[0])) << "\tTo: " << QString::fromStdString(square_to_string(squares[1]));
    return squares;
}
