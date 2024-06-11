﻿
#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
开始处理当前文件
*/
void MainWindow::CurrentFileProgress_Start(QString FileName, int FrameNum)
{
    ui->groupBox_CurrentFile->setTitle("Current File: " + FileName);
    //=================================
    ui->progressBar_CurrentFile->setRange(0, FrameNum);
    ui->progressBar_CurrentFile->setValue(0);
    //=================================
    ui->label_FrameProgress_CurrentFile->setText("0/" + QString::number(FrameNum, 10));
    ui->label_TimeRemain_CurrentFile->setText("Time remaining:NULL");
    ui->label_TimeCost_CurrentFile->setText("Time taken:NULL");
    ui->label_ETA_CurrentFile->setText("ETA:NULL");
    //=================================
    TimeCost_CurrentFile = 0;
    TaskNumTotal_CurrentFile = FrameNum;
    TaskNumFinished_CurrentFile = 0;
    NewTaskFinished_CurrentFile = false;
    ETA_CurrentFile = 0;
    //=================================
    ui->groupBox_CurrentFile->setVisible(1);
    //=================================
    isStart_CurrentFile = true;
    return;
}
/*
停止处理当前文件
*/
void MainWindow::CurrentFileProgress_Stop()
{
    ui->groupBox_CurrentFile->setVisible(0);
    isStart_CurrentFile = false;
    return;
}
/*
进度条 进度+1
*/
void MainWindow::CurrentFileProgress_progressbar_Add()
{
    TaskNumFinished_CurrentFile++;
    NewTaskFinished_CurrentFile = true;
    ui->progressBar_CurrentFile->setValue(TaskNumFinished_CurrentFile);
    ui->label_FrameProgress_CurrentFile->setText(QString::number(TaskNumFinished_CurrentFile, 10) + "/" + QString::number(TaskNumTotal_CurrentFile, 10));
}
/*
进度条 进度+片段時長
*/
void MainWindow::CurrentFileProgress_progressbar_Add_SegmentDuration(int SegmentDuration)
{
    TaskNumFinished_CurrentFile += SegmentDuration;
    NewTaskFinished_CurrentFile = true;
    ui->progressBar_CurrentFile->setValue(TaskNumFinished_CurrentFile);
    ui->label_FrameProgress_CurrentFile->setText(QString::number(TaskNumFinished_CurrentFile, 10) + "/" + QString::number(TaskNumTotal_CurrentFile, 10));
}
