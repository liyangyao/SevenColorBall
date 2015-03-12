#include "mainform.h"
#include <QApplication>
#include <QTextCodec>
#include <time.h>
int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    QTextCodec* codec=QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
#endif
    QApplication a(argc, argv);
    qsrand(time(0));
    MainForm w;
    w.show();

    return a.exec();
}
