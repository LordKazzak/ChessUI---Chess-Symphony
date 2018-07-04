#include <QColorDialog>
#include <QFontDialog>
#include <QFileDialog>
#include <QEvent>
#include <QList>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

#include "settings.h"
#include "ui_settings.h"
#include "board2d.h"
#include "ChessUI.h"

Settings::Settings(QMainWindow *mainWindow) :
    ui(new Ui::Settings)
{
    setFixedSize(314, 288);
    setWindowModality(Qt::ApplicationModal);
    setParent(mainWindow,Qt::Dialog);
    load();
    ui->setupUi(this);
    QPixmap pm(QSize(100,100));
    pm.fill(wSqColor);
    ui->wSqCButton->setIcon(QIcon(pm));
    pm.fill((bSqColor));
    ui->bSqCButton->setIcon(QIcon(pm));
    ui->fontButton->setFont(boardFont);
    ui->fontButton->setText(QString("A1"));
    ui->lineEdit->setText(enginePath);
    ui->spinBoxDepth->setValue(depth);
    ui->spinBoxTime->setValue(time);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_wSqCButton_clicked()
{
    QColor tmpWSqColor = QColorDialog::getColor(this->tmpWSqColor, this);
    if(tmpWSqColor.isValid()){
        tmpSWSqColor = true;
        this->tmpWSqColor = tmpWSqColor;
        QPixmap pm(QSize(100,100));
        pm.fill(tmpWSqColor);
        ui->wSqCButton->setIcon(QIcon(pm));
    }
}

void Settings::on_bSqCButton_clicked()
{
    QColor tmpBSqColor = QColorDialog::getColor(this->tmpBSqColor, this);
    if(tmpBSqColor.isValid()){
        tmpSBSqColor = true;
        this->tmpBSqColor = tmpBSqColor;
        QPixmap pm(QSize(100,100));
        pm.fill(tmpBSqColor);
        ui->bSqCButton->setIcon(QIcon(pm));
    }
}

void Settings::on_fontButton_clicked()
{
    bool ok;
    QFont tmpBoardFont = QFontDialog::getFont(&ok,this->tmpBoardFont,this);
    if(ok){
        this->tmpBoardFont = tmpBoardFont;
        ui->fontButton->setFont(tmpBoardFont);
    }
}


void Settings::on_pieceButton_clicked()
{
    QString tmpPiecePath = QFileDialog::getExistingDirectory(this,"Pieces path",
                                                            this->tmpPiecePath);
    if(tmpPiecePath != "") this->tmpPiecePath = tmpPiecePath;
}

void Settings::on_wSqPathButton_clicked()
{
    QString tmpWSqPath = QFileDialog::getOpenFileName(this,"White Square File",
                                                      this->tmpWSqPath);
    if(tmpWSqPath != ""){
        tmpSWSqColor = false;
        this->tmpWSqPath = tmpWSqPath;
    }
}

void Settings::on_bSqPathButton_clicked()
{
    QString tmpBSqPath = QFileDialog::getOpenFileName(this,"Black Square File",
                                                      this->tmpBSqPath);
    if(tmpBSqPath != ""){
        tmpSBSqColor = false;
        this->tmpBSqPath = tmpBSqPath;
    }
}

void Settings::on_pushButtonEnginePath_clicked()
{
    QString tmpEnginePath = QFileDialog::getOpenFileName(this,"Engine",this->enginePath);
    //if(tmpWSqPath != ""){
      //  tmpSWSqColor = false;
    if(tmpEnginePath != "")
        this->enginePath = tmpEnginePath;
    qDebug() << enginePath;
    ui->lineEdit->setText(enginePath);
}


void Settings::on_buttonBox_accepted()
{
    wSqColor = tmpWSqColor;
    bSqColor = tmpBSqColor;
    boardFont = tmpBoardFont;
    piecePath = tmpPiecePath;
    wSqPath = tmpWSqPath;
    bSqPath = tmpBSqPath;
    sWSqColor = tmpSWSqColor;
    sBSqColor = tmpSBSqColor;
    isHuman[White] = ui->radioButtonW->isChecked();
    isHuman[Black] = ui->radioButtonB->isChecked();
    depth = ui->spinBoxDepth->value();
    time = ui->spinBoxTime->value();

    /*if(ui->comboBoxLanguage->itemText(ui->comboBoxLanguage->currentIndex()) != "English")
        language = "Slovene";
    else
        language = "English";*/


    QEvent event(QEvent::PaletteChange);
    QList<Board2D*> list = parent()->findChildren<Board2D *>("Board2D");

    for(int i=0; i<list.count(); i++)
        QApplication::sendEvent(list.at(i), &event);

    save();
    hide();
}

void Settings::on_buttonBox_rejected()
{
    hide();
}

void Settings::save(){
    QFile file("media/settings.xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        printf("Opening settings file for save failed.");
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Board2D");
    xml.writeTextElement("wSqColor", wSqColor.name());
    xml.writeTextElement("bSqColor", bSqColor.name());
    xml.writeTextElement("font", boardFont.toString());
    xml.writeTextElement("piecePath",piecePath);
    xml.writeTextElement("wSqPath",wSqPath);
    xml.writeTextElement("bSqPath",bSqPath);
    if(sWSqColor)
        xml.writeTextElement("sWSqColor","1");
    else
        xml.writeTextElement("sWSqColor","0");
    if(sBSqColor)
        xml.writeTextElement("sBSqColor","1");
    else
        xml.writeTextElement("sBSqColor","0");
    xml.writeTextElement("enginePath", enginePath);
    xml.writeTextElement("depth", QString::number(depth));
    xml.writeTextElement("time", QString::number(time));
    xml.writeTextElement("language", language);
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}

void Settings::load(){
    QFile file("media/settings.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        printf("Reading settings file failed.");
        return;
    }
    QXmlStreamReader xml(&file);
    while (!xml.atEnd()) {
          xml.readNextStartElement();
          if(xml.name() == "Board2D"){
              while(xml.readNextStartElement()){
                  if(xml.name() == "wSqColor")
                      tmpWSqColor = wSqColor = QColor(xml.readElementText());
                  else if(xml.name() == "bSqColor")
                      tmpBSqColor = bSqColor = QColor(xml.readElementText());
                  else if(xml.name() == "font"){
                      boardFont.fromString(xml.readElementText());
                      tmpBoardFont = boardFont;
                  }
                  else if(xml.name() == "piecePath")
                      piecePath = tmpPiecePath = xml.readElementText();
                  else if(xml.name() == "wSqPath")
                      wSqPath = tmpWSqPath =  xml.readElementText();
                  else if(xml.name() == "bSqPath")
                      bSqPath = tmpBSqPath =  xml.readElementText();
                  else if(xml.name() == "sWSqColor"){
                      if(xml.readElementText() == "1")
                          sWSqColor = tmpSWSqColor = true;
                      else sWSqColor = tmpSWSqColor = false;
                  }
                  else if(xml.name() == "sBSqColor"){
                      if(xml.readElementText() == "1")
                          sBSqColor = tmpSBSqColor = true;
                      else sBSqColor = tmpSBSqColor = false;
                  }
                  else if(xml.name() == "enginePath"){
                      enginePath = xml.readElementText();
                  }
                  else if(xml.name() == "depth"){
                      depth = xml.readElementText().toInt();
                  }
                  else if(xml.name() == "time"){
                      time = xml.readElementText().toInt();
                  }
                  else if(xml.name() == "language"){
                      language = xml.readElementText();
                  }
              }
          }
    }
    file.close();
}
