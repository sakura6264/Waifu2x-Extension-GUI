#include "mainwindow.h"

MainWindow::MainWindow(QString text,QWidget *parent)
    : QMainWindow(parent)
{
    this->textBrowser = new QTextBrowser(this);
    this->textBrowser->setText(text);
    this->textBrowser->setToolTip("[ESC]:Exit\n[CTRL+SHIFT+C]:Copy All");
    this->setCentralWidget(textBrowser);
    this->copy = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C),this);
    this->esc = new QShortcut(QKeySequence(Qt::Key_Escape),this);
    connect(this->esc,&QShortcut::activated,this,&QWidget::close);
    connect(this->copy,&QShortcut::activated,this,&MainWindow::copyText);
}

MainWindow::~MainWindow()
{
    disconnect(this->esc,&QShortcut::activated,this,&QWidget::close);
    disconnect(this->copy,&QShortcut::activated,this,&MainWindow::copyText);
    delete textBrowser;
    delete esc;
    delete copy;
}

void MainWindow::copyText()
{
    QApplication::clipboard()->setText(this->textBrowser->toPlainText());
}

