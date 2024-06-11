
#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
在文本框内输出一条新消息
*/
void MainWindow::TextBrowser_NewMessage(QString message)
{
    QDateTime time = QDateTime::currentDateTime();
    QString Current_Time = time.toString("[yyyy-MM-dd hh:mm:ss]");
    QString OutPutStr = Current_Time + " " + message;
    ui->textBrowser->append(OutPutStr);
    ui->textBrowser->moveCursor(QTextCursor::End);
}
/*
输出文本框的欢迎消息
启动时显示
*/
void MainWindow::TextBrowser_StartMes()
{
    TextBrowser_NewMessage("Start Complete.");
}
