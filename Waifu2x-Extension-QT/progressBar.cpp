
#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
设定进度条最小值与最大值
*/
void MainWindow::progressbar_setRange_min_max(int min, int max)
{
    ui->progressBar->setRange(min, max);
    TaskNumTotal = max;
    ui->label_progressBar_filenum->setText("0/" + QString::number(max, 10));
}
/*
将进度条直接充满
*/
void MainWindow::progressbar_SetToMax(int maxval)
{
    ui->progressBar->setValue(maxval);
    ui->label_progressBar_filenum->setText(QString::number(maxval, 10) + "/" + QString::number(maxval, 10));
}
/*
进度条 进度+1
*/
void MainWindow::progressbar_Add()
{
    Progressbar_CurrentVal++;
    TaskNumFinished++;
    NewTaskFinished = true;
    ui->progressBar->setValue(Progressbar_CurrentVal);
    ui->label_progressBar_filenum->setText(QString::number(Progressbar_CurrentVal, 10) + "/" + QString::number(Progressbar_MaxVal, 10));
}
/*
清空进度条
*/
void MainWindow::progressbar_clear()
{
    ui->progressBar->setRange(0, 1);
    ui->progressBar->setValue(0);
    ui->label_progressBar_filenum->setText("0/0");
}
