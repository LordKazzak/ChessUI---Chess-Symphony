#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include "chessui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("chessui_" + QLocale::system().name());
    a.installTranslator(&myappTranslator);

    ChessUI w;
    w.show();

    return a.exec();
}
