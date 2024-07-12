
#include "mainwindow.h"
#include <QFontDatabase>
#include <QApplication>
#include <QSplashScreen>
#include "QtGlobal"

int main(int argc, char *argv[])
{
    qputenv("DISABLE_LAYER_AMD_SWITCHABLE_GRAPHICS_1","1");
    // this putenv is to fix vulkan find device error
    // has no effect on this program
    // only for subprocess
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeDialogs);
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    QApplication app(argc, argv);
    QSplashScreen *splash = new QSplashScreen;
    QPixmap splash_img = QPixmap(":/SplashScreen.png");
    splash->setPixmap(splash_img);
    splash->show();
    QFile file(":/light/stylesheet.qss");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    app.setStyleSheet(stream.readAll());
    int fontId = QFontDatabase::addApplicationFont(":/CascadiaMono.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontName, 8);
    QApplication::setFont(font);
    app.setQuitOnLastWindowClosed(false); // 隐藏无窗口时保持运行
    MainWindow *w = new MainWindow;
    w->show();
    splash->finish(w);
    delete splash;
    int return_v = app.exec();
    delete w;
    return return_v;
}
