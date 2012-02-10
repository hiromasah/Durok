#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTranslator>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QTranslator translator;
    translator.load("Durok");
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
