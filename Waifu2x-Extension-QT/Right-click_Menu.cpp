#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::Init_ActionsMenu_checkBox_DelOriginal()
{
    QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->setText("Move to Recycle Bin.");
    QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->setCheckable(1);
    //===
    ui->checkBox_DelOriginal->addAction(QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal);
}

void MainWindow::Init_ActionsMenu_checkBox_ReplaceOriginalFile()
{
    QAction_checkBox_MoveToRecycleBin_checkBox_ReplaceOriginalFile->setText("Move to Recycle Bin.");
    QAction_checkBox_MoveToRecycleBin_checkBox_ReplaceOriginalFile->setCheckable(1);
    //===
    ui->checkBox_ReplaceOriginalFile->addAction(QAction_checkBox_MoveToRecycleBin_checkBox_ReplaceOriginalFile);
}

void MainWindow::Init_ActionsMenu_pushButton_RemoveItem()
{
    RemoveALL_image->setText("Remove all image files.");
    RemoveALL_gif->setText("Remove all gif files.");
    RemoveALL_video->setText("Remove all video files.");
    //==============
    RemoveALL_image->setIcon(QIcon(":/icon/icon/picture_Rmenu.svg"));
    RemoveALL_gif->setIcon(QIcon(":/icon/icon/gif_Rmenu.svg"));
    RemoveALL_video->setIcon(QIcon(":/icon/icon/video_Rmenu.svg"));
    //==============
    connect(RemoveALL_image, SIGNAL(triggered()), this, SLOT(RemoveALL_image_slot()));
    connect(RemoveALL_gif, SIGNAL(triggered()), this, SLOT(RemoveALL_gif_slot()));
    connect(RemoveALL_video, SIGNAL(triggered()), this, SLOT(RemoveALL_video_slot()));
    //==============
    ui->pushButton_RemoveItem->addAction(RemoveALL_image);
    ui->pushButton_RemoveItem->addAction(RemoveALL_gif);
    ui->pushButton_RemoveItem->addAction(RemoveALL_video);
}

void MainWindow::RemoveALL_image_slot()
{
    ui->pushButton_RemoveItem->setEnabled(0);
    int rowCount_image = Table_model_image->rowCount();
    if (rowCount_image > 0)
    {
        for (int i = 0; i < rowCount_image; i++)
        {
            CustRes_remove(Table_model_image->item(i, 2)->text());
        }
        Table_model_image->clear();
        Init_Table();
        curRow_image = -1;
        ui->tableView_image->clearSelection();
        ui->tableView_image->setVisible(0);
    }
    Table_FileCount_reload();
    if (Table_model_gif->rowCount() == 0 && Table_model_image->rowCount() == 0 && Table_model_video->rowCount() == 0)
    {
        on_pushButton_ClearList_clicked();
    }
}

void MainWindow::RemoveALL_gif_slot()
{
    ui->pushButton_RemoveItem->setEnabled(0);
    int rowCount_gif = Table_model_gif->rowCount();
    if (rowCount_gif > 0)
    {
        for (int i = 0; i < rowCount_gif; i++)
        {
            CustRes_remove(Table_model_gif->item(i, 2)->text());
        }
        Table_model_gif->clear();
        Init_Table();
        curRow_gif = -1;
        ui->tableView_gif->clearSelection();
        ui->tableView_gif->setVisible(0);
    }
    Table_FileCount_reload();
    if (Table_model_gif->rowCount() == 0 && Table_model_image->rowCount() == 0 && Table_model_video->rowCount() == 0)
    {
        on_pushButton_ClearList_clicked();
    }
}

void MainWindow::RemoveALL_video_slot()
{
    ui->pushButton_RemoveItem->setEnabled(0);
    int rowCount_video = Table_model_video->rowCount();
    if (rowCount_video > 0)
    {
        for (int i = 0; i < rowCount_video; i++)
        {
            CustRes_remove(Table_model_video->item(i, 2)->text());
        }
        Table_model_video->clear();
        Init_Table();
        curRow_video = -1;
        ui->tableView_video->clearSelection();
        ui->tableView_video->setVisible(0);
    }
    Table_FileCount_reload();
    if (Table_model_gif->rowCount() == 0 && Table_model_image->rowCount() == 0 && Table_model_video->rowCount() == 0)
    {
        on_pushButton_ClearList_clicked();
    }
}

void MainWindow::Init_ActionsMenu_lineEdit_outputPath()
{
    OpenFolder_lineEdit_outputPath->setText("Open output path");
    OpenFolder_lineEdit_outputPath->setIcon(QIcon(":/icon/icon/opne_folder.svg"));
    connect(OpenFolder_lineEdit_outputPath, SIGNAL(triggered()), this, SLOT(OpenOutputFolder()));
    ui->lineEdit_outputPath->addAction(OpenFolder_lineEdit_outputPath);
}

void MainWindow::OpenOutputFolder()
{
    QString OutPutPath = ui->lineEdit_outputPath->text();
    if (file_OpenFolder(OutPutPath) == false)
    {
        QMessageBox::warning(this,"Error","Output path does not exists!");
    }
}

void MainWindow::Init_ActionsMenu_FilesList()
{
    OpenFile_QAction_FileList->setText("Open file");
    OpenFile_QAction_FileList->setIcon(QIcon(":/icon/icon/view_file.svg"));
    connect(OpenFile_QAction_FileList, SIGNAL(triggered()), this, SLOT(OpenSelectedFile_FilesList()));
    ui->tableView_image->addAction(OpenFile_QAction_FileList);
    ui->tableView_gif->addAction(OpenFile_QAction_FileList);
    ui->tableView_video->addAction(OpenFile_QAction_FileList);
    //===
    OpenFilesFolder_QAction_FileList->setText("Open folder");
    OpenFilesFolder_QAction_FileList->setIcon(QIcon(":/icon/icon/opne_folder.svg"));
    connect(OpenFilesFolder_QAction_FileList, SIGNAL(triggered()), this, SLOT(OpenSelectedFilesFolder_FilesList()));
    ui->tableView_image->addAction(OpenFilesFolder_QAction_FileList);
    ui->tableView_gif->addAction(OpenFilesFolder_QAction_FileList);
    ui->tableView_video->addAction(OpenFilesFolder_QAction_FileList);
    //===
    RemoveFile_FilesList_QAction_FileList->setText("Remove from list [Delete]");
    RemoveFile_FilesList_QAction_FileList->setIcon(QIcon(":/icon/icon/RemoveFile.svg"));
    connect(RemoveFile_FilesList_QAction_FileList, SIGNAL(triggered()), this, SLOT(on_pushButton_RemoveItem_clicked()));
    ui->tableView_image->addAction(RemoveFile_FilesList_QAction_FileList);
    ui->tableView_gif->addAction(RemoveFile_FilesList_QAction_FileList);
    ui->tableView_video->addAction(RemoveFile_FilesList_QAction_FileList);
    //===
    Apply_CustRes_QAction_FileList->setText("Apply custom resolution [Ctrl+A]");
    Apply_CustRes_QAction_FileList->setIcon(QIcon(":/icon/icon/Apply.svg"));
    connect(Apply_CustRes_QAction_FileList, SIGNAL(triggered()), this, SLOT(Apply_CustRes_QAction_FileList_slot()));
    ui->tableView_image->addAction(Apply_CustRes_QAction_FileList);
    ui->tableView_gif->addAction(Apply_CustRes_QAction_FileList);
    ui->tableView_video->addAction(Apply_CustRes_QAction_FileList);
    //===
    Cancel_CustRes_QAction_FileList->setText("Cancel custom resolution [Ctrl+C]");
    Cancel_CustRes_QAction_FileList->setIcon(QIcon(":/icon/icon/cancel.svg"));
    connect(Cancel_CustRes_QAction_FileList, SIGNAL(triggered()), this, SLOT(Cancel_CustRes_QAction_FileList_slot()));
    ui->tableView_image->addAction(Cancel_CustRes_QAction_FileList);
    ui->tableView_gif->addAction(Cancel_CustRes_QAction_FileList);
    ui->tableView_video->addAction(Cancel_CustRes_QAction_FileList);
}
void MainWindow::Apply_CustRes_QAction_FileList_slot()
{
    EnableApply2All_CustRes = false;
    on_pushButton_CustRes_apply_clicked();
    EnableApply2All_CustRes = true;
}
void MainWindow::Cancel_CustRes_QAction_FileList_slot()
{
    EnableApply2All_CustRes = false;
    on_pushButton_CustRes_cancel_clicked();
    EnableApply2All_CustRes = true;
}
void MainWindow::OpenSelectedFile_FilesList()
{
    if (curRow_image == -1 && curRow_video == -1 && curRow_gif == -1)
    {
        ui->tableView_image->clearSelection();
        ui->tableView_gif->clearSelection();
        ui->tableView_video->clearSelection();
        //=====
        QMessageBox::warning(this,"Warning","No items are currently selected.");
        //====
        return;
    }
    //==========================
    if (curRow_image >= 0)
    {
        if (file_OpenFile(Table_model_image->item(curRow_image, 2)->text()) == false)
        {
            OpenSelectedFile_FailedWarning_FilesList();
        }
        return;
    }
    //============================================================
    if (curRow_video >= 0)
    {
        if (file_OpenFile(Table_model_video->item(curRow_video, 2)->text()) == false)
        {
            OpenSelectedFile_FailedWarning_FilesList();
        }
        return;
    }
    //============================================================
    if (curRow_gif >= 0)
    {
        if (file_OpenFile(Table_model_gif->item(curRow_gif, 2)->text()) == false)
        {
            OpenSelectedFile_FailedWarning_FilesList();
        }
        return;
    }
}

void MainWindow::OpenSelectedFilesFolder_FilesList()
{
    if (curRow_image == -1 && curRow_video == -1 && curRow_gif == -1)
    {
        ui->tableView_image->clearSelection();
        ui->tableView_gif->clearSelection();
        ui->tableView_video->clearSelection();
        //=====
        QMessageBox::warning(this,"Warning","No items are currently selected.");
        //====
        return;
    }
    //==========================
    if (curRow_image >= 0)
    {
        if (file_OpenFilesFolder(Table_model_image->item(curRow_image, 2)->text()) == false)
        {
            OpenSelectedFile_FailedWarning_FilesList();
        }
        return;
    }
    //============================================================
    if (curRow_video >= 0)
    {
        if (file_OpenFilesFolder(Table_model_video->item(curRow_video, 2)->text()) == false)
        {
            OpenSelectedFile_FailedWarning_FilesList();
        }
        return;
    }
    //============================================================
    if (curRow_gif >= 0)
    {
        if (file_OpenFilesFolder(Table_model_gif->item(curRow_gif, 2)->text()) == false)
        {
            OpenSelectedFile_FailedWarning_FilesList();
        }
        return;
    }
}

void MainWindow::OpenSelectedFile_FailedWarning_FilesList()
{
    QMessageBox::warning(this,"Error","Target file(folder) does not exists!");
}
