#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextBrowser>
#include <QMainWindow>
#include <QFontDatabase>
#include <QTextStream>
#include <QFile>
#include <QIcon>
#include <QWidget>
#include <QShortcut>
#include <QClipboard>
#include <QApplication>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString text,QWidget *parent = nullptr);
    ~MainWindow();
    QTextBrowser * textBrowser;
    QShortcut * esc;
    QShortcut * copy;
public slots:
    void copyText();
};
#endif // MAINWINDOW_H
