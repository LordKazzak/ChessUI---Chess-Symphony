#include "chessui.h"
#include "ui_chessui.h"
#include "board2d.h"
#include "engine.h"
#include "QLabel"
#include "QMessageBox"


ChessUI::ChessUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChessUI)
{
    settings = new Settings(this);
    widget = NULL;
    ui->setupUi(this);
    connect(ui->actionSettings,SIGNAL(triggered()),this,SLOT(showSettings()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(showAbout()));
    connect(ui->actionNew_game, SIGNAL(triggered()), this, SLOT(againstHuman()));
    connect(ui->actionNew_game_against_computer, SIGNAL(triggered()), this, SLOT(startEngine()));
    settings->setIsHuman(White, true);
    settings->setIsHuman(Black, false);
    againstHuman();
}

ChessUI::~ChessUI()
{
    Engine::deleteEngine();
    delete ui;
}

void ChessUI::showSettings()
{
    settings->show();
}

void ChessUI::showAbout()
{
    About *about = new About(this);
    about->show();
}

/*void ChessUI::showText(QPlainTextEdit *edit, const QString &message)
{
    if(edit)
        edit->appendPlainText(message);
    else
        qDebug() << "Error: QPlaintext 'edit' not initialized!";
}*/

void ChessUI::showRightText(const QString &message){
    if(ui->right)
        ui->right->appendPlainText(message);
    else
        qDebug() << "Error: QPlaintext 'right' not initialized!";
}

void ChessUI::changeLabel(const bool &label, const QString &text){
    if(White == label)
        ui->labelWhite->setText(text);
    else
        ui->labelBlack->setText(text);
}

void ChessUI::fillTitleBar(const QString &text){
    QRegExp namesReg("((\\s\\D{2}\\w*)+)");
    int pos = 0;
    while ((pos = namesReg.indexIn(text, pos)) != -1){
        dataList  << namesReg.cap(1).trimmed();
        pos += namesReg.matchedLength();
    }
    ui->bottom->setColumnCount(dataList.count());
    //qDebug() << "reg " << dataList;

    for(int i=0; i<ui->bottom->columnCount(); i++){
        ui->bottom->setHorizontalHeaderItem(i, new QTableWidgetItem(dataList.at(i)));
        if(ui->bottom->columnCount() - 1 != i)
            ui->bottom->setColumnWidth(i, 60);
        else
            ui->bottom->setColumnWidth(i, ui->bottom->width() - 60*(dataList.count()));
    }
}

void ChessUI::fillTableRow(const QString &text){
    QRegExp namesReg("((\\s\\D{2}\\w*)+)");
    QRegExp numsReg("((\\s-?\\d+)|(\\s\\w\\d\\w\\d\\w?)+)");
    QStringList namesList;
    QStringList numsList;
    QString tmp = "";
    int pos = 0;

    ui->bottom->insertRow(ui->bottom->rowCount());
    while ((pos = namesReg.indexIn(text, pos)) != -1){
        namesList << namesReg.cap(1).trimmed();
        pos += namesReg.matchedLength();
    }
    pos=0;
    while ((pos = numsReg.indexIn(text, pos)) != -1){
        numsList << numsReg.cap(1).trimmed();
        pos += numsReg.matchedLength();
    }

    /*qDebug() << "data" << dataList;
    qDebug() << "names" << namesList;
    qDebug() << "nums" << numsList;*/
    for(int i=0; i<dataList.count(); i++)
        for(int j=0; j<namesList.count(); j++)
            if(dataList.at(i) == namesList.at(j)){
                    tmp = numsList.at(j);
                    ui->bottom->setItem(ui->bottom->rowCount()-1, i, new QTableWidgetItem(tmp));
                    break;
                }
}

void ChessUI::showBottomText(const QString &message){
    if(ui->bottom){
        if(dataList.empty()){
            fillTitleBar(message);
        }
        else if(dataList.count() < 2){ //samo depth je notri
            dataList.clear();
            fillTitleBar(message);
        }
        fillTableRow(message);
    }
    else
        qDebug() << "Error: QPlaintext 'bottom' not initialized!";
}

void ChessUI::drawBoard()
{
    if(widget){
        qDebug() << "Deleting widget";
        ui->gridLayout->removeWidget(board);
        ui->centralWidget->repaint();
        delete board;
        delete widget;
        widget = NULL;
    }

    Engine::deleteEngine();

    widget = new QWidget();
    board = new Board2D(settings,widget);

    if(!settings->getIsHuman(White))
        board->fliped = true;

    board->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->gridLayout->addWidget(board, 0, 0, 2, 1);

    /*white = new QLabel("WHITE");
    white->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->gridLayout->addWidget(white, 0, 1, 1 ,1);

    black = new QLabel("BLACK");
    black->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->gridLayout->addWidget(black, 0, 2, 1 ,1);

    right = new QPlainTextEdit();
    right->setReadOnly(true);
    right->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //right->setFixedWidth(300);

    ui->gridLayout->addWidget(right, 1, 1, 1, 2, Qt::AlignRight);

    //connect(board, SIGNAL(sendTextUI(const QString&)), this, SLOT(showRightText(const QString&)));

    bottom = new QTableWidget();
    bottom->setFixedHeight(60);

    ui->gridLayout->addWidget(bottom, 2, 0, 1, 3);*/

    connect(board, SIGNAL(sendTextUI(const QString&)), this, SLOT(showRightText(const QString&)));
}

void ChessUI::againstHuman()
{
    ui->bottom->setRowCount(0);
    ui->bottom->setColumnCount(0);
    ui->right->clear();
    settings->setIsHuman(White, true);
    settings->setIsHuman(Black, true);
    drawBoard();
}

void ChessUI::startEngine()
{
    ui->bottom->setRowCount(0);
    ui->bottom->setColumnCount(0);
    ui->right->clear();
    ui->labelWhite->setText(tr("Player 1"));
    ui->labelBlack->setText(tr("Player 2"));

    if(settings->getIsHuman(White) == settings->getIsHuman(Black)){
        settings->setIsHuman(White, true);
        settings->setIsHuman(Black,false);
    }
    dataList.clear();

    drawBoard();
    Engine *engine;
    engine = Engine::instance();

    connect(engine, SIGNAL(updateRightText(const QString&)), this, SLOT(showRightText(const QString&)));
    connect(engine, SIGNAL(updateBottomText(const QString&)), this, SLOT(showBottomText(const QString&)));

    if(!QFile::exists(settings->getEnginePath())){

        QMessageBox Error;
        Error.setText(tr("No engine to play against found! Please check path in settings. Starting a two player game."));
        Error.exec();
        againstHuman();
        return;
    }

    if(!engine->run(settings->getEnginePath(), settings->getDepth(), settings->getTime())){
        QMessageBox Error;
        Error.setText(tr("Engine did not respond to initialization command. Starting a two player game."));
        Error.exec();
        againstHuman();
        return;
    }

    if(engine->isOpen()){
        //QString engineName="Engine";
        QString engineName;
        //if(ui->right->toPlainText() != "")
            engineName = ui->right->toPlainText().split("\n").at(0);

        if(!settings->getIsHuman(Black))
            changeLabel(Black, engineName);
        else
            changeLabel(White, engineName);
    }
    else{
        qDebug() << "Engine not running.";
        settings->setIsHuman(White,true);
        settings->setIsHuman(Black,true);
        delete engine;
        engine = NULL;
    }
}
