#include "mainwindow.h"
#include "math.h"


int main(int argc, char *argv[])
{
    QString title = "wecho";
    QString text = "Hello World!";
    if (argc >= 2){
        title = QString(argv[1]);
    }
    if (argc >= 3){
        QString text_build;
        for (int i = 2;i<argc;i++){
            text_build.append(argv[i]);
            text_build.append(" ");
        }
        text = text_build;
    }
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // high dpi
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeDialogs);
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    QApplication app(argc, argv);
    QFile file(":/light/stylesheet.qss");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    app.setStyleSheet(stream.readAll());
    int fontId = QFontDatabase::addApplicationFont(":/CascadiaMono.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontName, 8);
    QApplication::setFont(font);
    double l = text.length();
    double guidance = floor(sqrt(l / 1.6)) + 2.0;
    double width;
    double height;
    if (guidance < 10.0) {
            width = 300.0;
            height = 200.0;
    } else if (guidance > 30) {
            width = 1000.0;
            height = 600.0;
        } else {
        width = guidance * 1.6 * 15.0 +10.0;
        height = guidance * 20.0 + 10.0;
    }
    MainWindow *w = new MainWindow(text);
    w ->setFixedSize(QSize(width,height));
    w->setWindowIcon(QIcon(":/icon_main.png"));
    w->setWindowTitle(title);
    w->show();
    int return_v = app.exec();
    delete w;
    return return_v;
}
