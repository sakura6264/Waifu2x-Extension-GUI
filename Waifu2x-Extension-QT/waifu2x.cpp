﻿
#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_pushButton_Start_clicked()
{
    /*
     * 判断是否启用自定义输出路径
     * 判断输出路径是否合法
     */
    if (ui->checkBox_OutPath_isEnabled->isChecked())
    {
        QString tmp = ui->lineEdit_outputPath->text();
        tmp = tmp.trimmed();
        if (tmp == "")
        {
            //=====
            QMessageBox::warning(this,"Error","Output path is empty.");
            //=====
            emit Send_TextBrowser_NewMessage("Output path is empty.");
            return;
        }
        tmp = tmp.replace("\\", "/");
        tmp = tmp.replace("\\\\", "/");
        tmp = tmp.replace("//", "/");
        if (tmp.right(1) == "/")
        {
            tmp = tmp.left(tmp.length() - 1);
        }
        QFileInfo fileinfo_tmp(tmp);
        if (file_isDirExist(tmp) && fileinfo_tmp.isDir() && file_isDirWritable(tmp))
        {
            OutPutFolder_main = tmp;
        }
        else
        {
            //=====
            QMessageBox::warning(this,"Error","Invalid output path.");
            //=====
            emit Send_TextBrowser_NewMessage("Invalid output path.");
            return;
        }
    }
    //=============================
    //      判断文件列表是否为空
    //=============================
    if (Table_model_image->rowCount() == 0 && Table_model_gif->rowCount() == 0 && Table_model_video->rowCount() == 0)
    {
        QMessageBox::warning(this,"Error","Unable to start processing files: The file list is empty.");
        return;
    }
    //======================================
    //      判断文件列表是否全部处理完成
    //======================================
    int rowCount_image = Table_model_image->rowCount();
    int rowCount_image_cache = rowCount_image;
    int rowCount_gif = Table_model_gif->rowCount();
    int rowCount_gif_cache = rowCount_gif;
    int rowCount_video = Table_model_video->rowCount();
    int rowCount_video_cache = rowCount_video;
    // 检查图片文件列表
    for (int RowNum = 0; RowNum < rowCount_image; RowNum++)
    {
        if (ui->checkBox_ReProcFinFiles->isChecked() == false)
        {
            if (Table_model_image->item(RowNum, 1)->text().toLower().contains("finished"))
            {
                rowCount_image_cache--;
                continue;
            }
        }
        if (Table_model_image->item(RowNum, 1)->text().toLower().contains("deleted"))
        {
            rowCount_image_cache--;
            continue;
        }
    }
    // 检查GIF文件列表
    for (int RowNum = 0; RowNum < rowCount_gif; RowNum++)
    {
        if (ui->checkBox_ReProcFinFiles->isChecked() == false)
        {
            if (Table_model_gif->item(RowNum, 1)->text().toLower().contains("finished"))
            {
                rowCount_gif_cache--;
                continue;
            }
        }
        if (Table_model_gif->item(RowNum, 1)->text().toLower().contains("deleted"))
        {
            rowCount_gif_cache--;
            continue;
        }
    }
    // 检查视频文件列表
    for (int RowNum = 0; RowNum < rowCount_video; RowNum++)
    {
        if (ui->checkBox_ReProcFinFiles->isChecked() == false)
        {
            if (Table_model_video->item(RowNum, 1)->text().toLower().contains("finished"))
            {
                rowCount_video_cache--;
                continue;
            }
        }
        if (Table_model_video->item(RowNum, 1)->text().toLower().contains("deleted"))
        {
            rowCount_video_cache--;
            continue;
        }
    }
    // 判断是否剩余可处理的文件
    if (rowCount_image_cache + rowCount_gif_cache + rowCount_video_cache == 0)
    {
        QMessageBox::warning(this,"Error","Unable to start processing files: All files in Files list are Finished or Deleted.");
        return;
    }
    // 正式开始处理列表内的文件
    Table_ChangeAllStatusToWaiting(); // 将table内没开始处理或者被打断的条目状态变为等待中
    //============== 判断是否需要隐藏ETA和剩余时间 ================
    if (Table_model_gif->rowCount() != 0 || Table_model_video->rowCount() != 0)
    {
        ui->label_TimeRemain->setVisible(0);
        ui->label_ETA->setVisible(0);
    }
    else
    {
        ui->label_TimeRemain->setVisible(1);
        ui->label_ETA->setVisible(1);
    }
    //============== 数值初始化 ==================
    waifu2x_STOP = false;
    waifu2x_STOP_confirm = false;
    ThreadNumMax = 0;
    ThreadNumRunning = 0;
    Progressbar_MaxVal = 0;
    Progressbar_CurrentVal = 0;
    TaskNumTotal = 0;
    TaskNumFinished = 0;
    NewTaskFinished = false;
    ETA = 0;
    isForceRetryClicked = false;
    ForceRetryCount = 1;
    //============== 界面初始化 ======================
    ui->pushButton_BrowserFile->setEnabled(0);
    ui->comboBox_ImageSaveFormat->setEnabled(0);
    ui->spinBox_ImageQualityLevel->setEnabled(0);
    emit Send_Table_EnableSorting(0); // 启用文件列表的排序功能
    RemoveFile_FilesList_QAction_FileList->setEnabled(0);
    Apply_CustRes_QAction_FileList->setEnabled(0);
    Cancel_CustRes_QAction_FileList->setEnabled(0);
    this->setAcceptDrops(0);             // 禁止drop file
    ui->pushButton_Stop->setVisible(1);  // 启用stop button
    ui->pushButton_Start->setVisible(0); // 禁用start button
    OutputSettingsArea_setEnabled(false);
    ui->pushButton_ClearList->setEnabled(0);
    ui->pushButton_RemoveItem->setEnabled(0);
    ui->groupBox_Engine->setEnabled(0);
    ui->groupBox_ScaleRaton_DenoiseLevel->setEnabled(0);
    ui->checkBox_OptGIF->setEnabled(0);
    ui->checkBox_DelOriginal->setEnabled(0);
    ui->checkBox_ReProcFinFiles->setEnabled(0);
    ui->pushButton_compatibilityTest->setEnabled(0);
    ui->pushButton_ReadFileList->setEnabled(0);
    progressbar_clear();
    ui->label_TimeCost->setText("Time taken:NULL");
    ui->label_ETA->setText("ETA:NULL");
    ui->label_TimeRemain->setText("Time remaining:NULL");
    ui->groupBox_video_settings->setEnabled(0);
    QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->setEnabled(0);
    ui->pushButton_ForceRetry->setVisible(1);
    ui->checkBox_AutoDetectAlphaChannel->setEnabled(0);
    ui->groupBox_AudioDenoise->setEnabled(0);
    ui->checkBox_ProcessVideoBySegment->setEnabled(0);
    ui->spinBox_SegmentDuration->setEnabled(0);
    ui->comboBox_ImageStyle->setEnabled(0);
    ui->checkBox_PreProcessImage->setEnabled(0);
    ui->checkBox_AlwaysPreProcessAlphaPNG->setEnabled(0);
    on_lineEdit_GPUs_Anime4k_editingFinished();
    on_lineEdit_MultiGPUInfo_Waifu2xCaffe_editingFinished();
    ui->checkBox_ReplaceOriginalFile->setEnabled(0);
    ui->groupBox_CustRes->setEnabled(0);
    //==========
    TimeCost = 0;
    TimeCostTimer->start(1000);
    emit Send_TextBrowser_NewMessage("Start processing files.");
    //==========
    PreLoad_Engines_Settings(); // 预读取引擎设定
    ui->comboBox_ImageStyle_Waifu2xCaffe->setEnabled(0);
    //==========
    Waifu2xMain = QtConcurrent::run(this, &MainWindow::Waifu2xMainThread); // 启动waifu2x 主线程
}

int MainWindow::Waifu2xMainThread()
{
    // 在table中的状态修改完成前一直block,防止偶发的多线程错误
    QMutex_Table_ChangeAllStatusToWaiting.lock();
    NULL;
    QMutex_Table_ChangeAllStatusToWaiting.unlock();
    //=======================
    int rowCount_image = Table_model_image->rowCount();
    int rowCount_gif = Table_model_gif->rowCount();
    int rowCount_video = Table_model_video->rowCount();
    //=======================
    Progressbar_MaxVal = rowCount_image + rowCount_gif + rowCount_video;
    Progressbar_CurrentVal = 0;
    TaskNumFinished = 0;
    emit Send_PrograssBar_Range_min_max(0, Progressbar_MaxVal);
    if (rowCount_image > 0)
    {
        int ImageEngine = ui->comboBox_Engine_Image->currentIndex();
        for (int currentRowNumber = 0; currentRowNumber < rowCount_image; currentRowNumber++)
        {
            if (waifu2x_STOP)
            {
                while (ThreadNumRunning > 0)
                {
                    Delay_msec_sleep(750);
                }
                waifu2x_STOP_confirm = true;
                return 0; // 如果启用stop位,则直接return
            }
            //=============== 判断状态 ================
            if (ui->checkBox_ReProcFinFiles->isChecked() == false)
            {
                if (Table_model_image->item(currentRowNumber, 1)->text().toLower().contains("finished"))
                {
                    emit Send_progressbar_Add();
                    continue;
                }
            }
            if (Table_model_image->item(currentRowNumber, 1)->text().toLower().contains("deleted"))
            {
                emit Send_progressbar_Add();
                continue;
            }
            //============== 判断权限 =====================
            if (file_isFilesFolderWritable_row_image(currentRowNumber) == false)
            {
                emit Send_progressbar_Add();
                continue;
            }
            //============ 自动跳过高分辨率文件 ===================
            if (Image_Gif_AutoSkip_CustRes(currentRowNumber, false))
            {
                emit Send_Table_image_ChangeStatus_rowNumInt_statusQString(currentRowNumber, "Skipped");
                continue;
            }
            //=========
            ThreadNumMax = ui->spinBox_ThreadNum_image->value(); // 获取image线程数量最大值
            //====================================================================================
            switch (ImageEngine)
            {
            case 0:
            {
                mutex_ThreadNumRunning.lock();
                ThreadNumRunning++; // 线程数量统计+1
                mutex_ThreadNumRunning.unlock();
                QtConcurrent::run(this, &MainWindow::Waifu2x_NCNN_Vulkan_Image, currentRowNumber, false);
                while (ThreadNumRunning >= ThreadNumMax)
                {
                    Delay_msec_sleep(750);
                }
                break;
            }
            case 1:
            {
                mutex_ThreadNumRunning.lock();
                ThreadNumRunning++; // 线程数量统计+1
                mutex_ThreadNumRunning.unlock();
                QtConcurrent::run(this, &MainWindow::Waifu2x_Converter_Image, currentRowNumber, false);
                while (ThreadNumRunning >= ThreadNumMax)
                {
                    Delay_msec_sleep(750);
                }
                break;
            }
            case 2:
            {
                mutex_ThreadNumRunning.lock();
                ThreadNumRunning++; // 线程数量统计+1
                mutex_ThreadNumRunning.unlock();
                QtConcurrent::run(this, &MainWindow::SRMD_NCNN_Vulkan_Image, currentRowNumber, false);
                while (ThreadNumRunning >= ThreadNumMax)
                {
                    Delay_msec_sleep(750);
                }
                break;
            }
            case 3:
            {
                mutex_ThreadNumRunning.lock();
                ThreadNumRunning++; // 线程数量统计+1
                mutex_ThreadNumRunning.unlock();
                QtConcurrent::run(this, &MainWindow::Anime4k_Image, currentRowNumber, false);
                while (ThreadNumRunning >= ThreadNumMax)
                {
                    Delay_msec_sleep(750);
                }
                break;
            }
            case 4:
            {
                mutex_ThreadNumRunning.lock();
                ThreadNumRunning++; // 线程数量统计+1
                mutex_ThreadNumRunning.unlock();
                QtConcurrent::run(this, &MainWindow::Waifu2x_Caffe_Image, currentRowNumber, false);
                while (ThreadNumRunning >= ThreadNumMax)
                {
                    Delay_msec_sleep(750);
                }
                break;
            }
            case 5:
            {
                mutex_ThreadNumRunning.lock();
                ThreadNumRunning++; // 线程数量统计+1
                mutex_ThreadNumRunning.unlock();
                QtConcurrent::run(this, &MainWindow::Realsr_NCNN_Vulkan_Image, currentRowNumber, false);
                while (ThreadNumRunning >= ThreadNumMax)
                {
                    Delay_msec_sleep(750);
                }
                break;
            }
            case 6:
            {
                mutex_ThreadNumRunning.lock();
                ThreadNumRunning++; // 线程数量统计+1
                mutex_ThreadNumRunning.unlock();
                QtConcurrent::run(this, &MainWindow::RealESRGAN_NCNN_Vulkan_Image, currentRowNumber, false);
                while (ThreadNumRunning >= ThreadNumMax)
                {
                    Delay_msec_sleep(750);
                }
                break;
            }
            case 7:
            {
                mutex_ThreadNumRunning.lock();
                ThreadNumRunning++; // 线程数量统计+1
                mutex_ThreadNumRunning.unlock();
                QtConcurrent::run(this, &MainWindow::RealCUGAN_NCNN_Vulkan_Image, currentRowNumber, false);
                while (ThreadNumRunning >= ThreadNumMax)
                {
                    Delay_msec_sleep(750);
                }
                break;
            }
            }
        }
    }
    while (ThreadNumRunning > 0)
    {
        Delay_msec_sleep(750);
    }
    mutex_ThreadNumRunning.lock();
    ThreadNumRunning = 0; // 线程数量统计+1
    mutex_ThreadNumRunning.unlock();
    //=========================================================
    //                   GIF 线程调度
    //===========================================================
    if (rowCount_gif > 0)
    {
        int GIFEngine = ui->comboBox_Engine_GIF->currentIndex();
        for (int currentRowNumber = 0; currentRowNumber < rowCount_gif; currentRowNumber++)
        {
            if (waifu2x_STOP)
            {
                waifu2x_STOP_confirm = true;
                return 0; // 如果启用stop位,则直接return
            }
            //=============== 判断状态 ================
            if (ui->checkBox_ReProcFinFiles->isChecked() == false)
            {
                if (Table_model_gif->item(currentRowNumber, 1)->text().toLower().contains("finished"))
                {
                    emit Send_progressbar_Add();
                    continue;
                }
            }
            if (Table_model_gif->item(currentRowNumber, 1)->text().toLower().contains("deleted"))
            {
                emit Send_progressbar_Add();
                continue;
            }
            //============== 判断权限 =====================
            if (file_isFilesFolderWritable_row_gif(currentRowNumber) == false)
            {
                emit Send_progressbar_Add();
                continue;
            }
            //============ 自动跳过高分辨率文件 ===================
            if (Image_Gif_AutoSkip_CustRes(currentRowNumber, true))
            {
                emit Send_Table_gif_ChangeStatus_rowNumInt_statusQString(currentRowNumber, "Skipped");
                continue;
            }
            //=========
            switch (GIFEngine)
            {
            case 0:
            {
                Waifu2x_NCNN_Vulkan_GIF(currentRowNumber);
                break;
            }
            case 1:
            {
                Waifu2x_Converter_GIF(currentRowNumber);
                break;
            }
            case 2:
            {
                SRMD_NCNN_Vulkan_GIF(currentRowNumber);
                break;
            }
            case 3:
            {
                Anime4k_GIF(currentRowNumber);
                break;
            }
            case 4:
            {
                Waifu2x_Caffe_GIF(currentRowNumber);
                break;
            }
            case 5:
            {
                Realsr_NCNN_Vulkan_GIF(currentRowNumber);
                break;
            }
            case 6:
            {
                RealESRGAN_NCNN_Vulkan_GIF(currentRowNumber);
                break;
            }
            case 7:
            {
                RealCUGAN_NCNN_Vulkan_GIF(currentRowNumber);
                break;
            }
            }
        }
    }
    //=========================================================
    //                   视频 线程调度
    //===========================================================
    if (rowCount_video > 0)
    {
        int VideoEngine = ui->comboBox_Engine_Video->currentIndex();
        for (int currentRowNumber = 0; currentRowNumber < rowCount_video; currentRowNumber++)
        {
            if (waifu2x_STOP)
            {
                waifu2x_STOP_confirm = true;
                return 0; // 如果启用stop位,则直接return
            }
            //=============== 判断状态 ================
            if (ui->checkBox_ReProcFinFiles->isChecked() == false)
            {
                if (Table_model_video->item(currentRowNumber, 1)->text().toLower().contains("finished"))
                {
                    emit Send_progressbar_Add();
                    continue;
                }
            }
            if (Table_model_video->item(currentRowNumber, 1)->text().toLower().contains("deleted"))
            {
                emit Send_progressbar_Add();
                continue;
            }
            //============== 判断权限 =====================
            if (file_isFilesFolderWritable_row_video(currentRowNumber) == false)
            {
                emit Send_progressbar_Add();
                continue;
            }
            //============ 自动跳过高分辨率文件 ===================
            if (Video_AutoSkip_CustRes(currentRowNumber))
            {
                emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(currentRowNumber, "Skipped");
                continue;
            }
            //============= 获取时长, 判断是否需要分段处理 =================
            switch (VideoEngine)
            {
            case 0:
            {
                if (video_isNeedProcessBySegment(currentRowNumber))
                {
                    Waifu2x_NCNN_Vulkan_Video_BySegment(currentRowNumber);
                }
                else
                {
                    Waifu2x_NCNN_Vulkan_Video(currentRowNumber);
                }
                break;
            }
            case 1:
            {
                if (video_isNeedProcessBySegment(currentRowNumber))
                {
                    Waifu2x_Converter_Video_BySegment(currentRowNumber);
                }
                else
                {
                    Waifu2x_Converter_Video(currentRowNumber);
                }
                break;
            }
            case 2:
            {
                if (video_isNeedProcessBySegment(currentRowNumber))
                {
                    Anime4k_Video_BySegment(currentRowNumber);
                }
                else
                {
                    Anime4k_Video(currentRowNumber);
                }
                break;
            }
            case 3:
            {
                if (video_isNeedProcessBySegment(currentRowNumber))
                {
                    SRMD_NCNN_Vulkan_Video_BySegment(currentRowNumber);
                }
                else
                {
                    SRMD_NCNN_Vulkan_Video(currentRowNumber);
                }
                break;
            }
            case 4:
            {
                if (video_isNeedProcessBySegment(currentRowNumber))
                {
                    Waifu2x_Caffe_Video_BySegment(currentRowNumber);
                }
                else
                {
                    Waifu2x_Caffe_Video(currentRowNumber);
                }
                break;
            }
            case 5:
            {
                if (video_isNeedProcessBySegment(currentRowNumber))
                {
                    Realsr_NCNN_Vulkan_Video_BySegment(currentRowNumber);
                }
                else
                {
                    Realsr_NCNN_Vulkan_Video(currentRowNumber);
                }
                break;
            }
            }
        }
    }
    if (waifu2x_STOP)
    {
        waifu2x_STOP_confirm = true;
        return 0; // 如果启用stop位,则直接return
    }
    emit Send_Waifu2x_Finished();
    return 0;
}

void MainWindow::Waifu2x_Finished()
{
    //==================== 进度条 =================================
    progressbar_SetToMax(Progressbar_MaxVal);
    //=============================================================
    emit Send_TextBrowser_NewMessage("Process finished.");
    emit Send_SystemTray_NewMessage("Process finished.");
    //============================================================
    Waifu2x_Finished_manual();
}

void MainWindow::Waifu2x_Finished_manual()
{
    TimeCostTimer->stop();
    //================== 界面恢复 ===============================
    ui->pushButton_BrowserFile->setEnabled(1);
    ui->comboBox_ImageSaveFormat->setEnabled(1);
    on_comboBox_ImageSaveFormat_currentIndexChanged(1);
    emit Send_Table_EnableSorting(1); // 启用文件列表的排序功能
    RemoveFile_FilesList_QAction_FileList->setEnabled(1);
    Apply_CustRes_QAction_FileList->setEnabled(1);
    Cancel_CustRes_QAction_FileList->setEnabled(1);
    this->setAcceptDrops(1);
    ui->pushButton_Stop->setVisible(0);
    ui->pushButton_Start->setVisible(1);
    if (ui->checkBox_ReplaceOriginalFile->isChecked() == false)
    {
        ui->groupBox_OutPut->setEnabled(1);
        OutputSettingsArea_setEnabled(true);
        ui->checkBox_DelOriginal->setEnabled(1);
    }
    ui->pushButton_ClearList->setEnabled(1);
    ui->pushButton_RemoveItem->setEnabled(1);
    ui->groupBox_Engine->setEnabled(1);
    ui->groupBox_ScaleRaton_DenoiseLevel->setEnabled(1);
    ui->checkBox_OptGIF->setEnabled(1);
    ui->checkBox_ReProcFinFiles->setEnabled(1);
    ui->pushButton_compatibilityTest->setEnabled(1);
    ui->pushButton_ReadFileList->setEnabled(1);
    ui->groupBox_video_settings->setEnabled(1);
    if (ui->checkBox_DelOriginal->isChecked())
        QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->setEnabled(1);
    ui->pushButton_ForceRetry->setVisible(0);
    ui->groupBox_AudioDenoise->setEnabled(1);
    ui->checkBox_ProcessVideoBySegment->setEnabled(1);
    if (ui->checkBox_ProcessVideoBySegment->isChecked())
    {
        ui->spinBox_SegmentDuration->setEnabled(1);
    }
    on_comboBox_model_vulkan_currentIndexChanged(0);
    isWaifu2xCaffeEnabled(); // 判断是否启用caffe图片风格combobox
    emit Send_CurrentFileProgress_Stop();
    ui->checkBox_PreProcessImage->setEnabled(1);
    ui->checkBox_AlwaysPreProcessAlphaPNG->setEnabled(1);
    checkBox_ReplaceOriginalFile_setEnabled_True_Self();
    ui->checkBox_AutoDetectAlphaChannel->setEnabled(1);
    ui->groupBox_CustRes->setEnabled(1);
    //=================== 数值恢复 ================================
    ThreadNumMax = 0;
    ThreadNumRunning = 0;
    Progressbar_MaxVal = 0;
    Progressbar_CurrentVal = 0;
    TaskNumTotal = 0;
    TaskNumFinished = 0;
    NewTaskFinished = false;
    ETA = 0;
    TimeCost = 0;
    ForceRetryCount = 1;
}

/*
移动文件到输出路径
*/
void MainWindow::MoveFileToOutputPath(QString Orginal, QString SourceFilePath)
{
    MoveFile_QMutex.lock();
    if (QFile::exists(Orginal) == false)
    {
        emit Send_TextBrowser_NewMessage("Error! Original file [" + Orginal + "] does not exists.");
        MoveFile_QMutex.unlock();
        return;
    }
    QString Target_fullpPath = "";
    QString Target_fileName = "";
    QString Target_folder = "";
    // 确定文件名
    if (ui->checkBox_OutPath_KeepOriginalFileName->isChecked())
    {
        QFileInfo fileinfo_source(SourceFilePath);
        QString file_name = file_getBaseName(SourceFilePath);
        QFileInfo fileinfo_Orginal(Orginal);
        QString file_ext = fileinfo_Orginal.suffix();
        Target_fileName = file_name + "." + file_ext;
    }
    else
    {
        QFileInfo fileinfo_Orginal(Orginal);
        Target_fileName = fileinfo_Orginal.fileName();
    }
    // 确定输出文件夹
    if (ui->checkBox_KeepParentFolder->isChecked())
    {
        QFileInfo fileinfo_SourceFilePath(SourceFilePath);
        QString folder_path_SourceFilePath = file_getFolderPath(fileinfo_SourceFilePath);
        QStringList folder_path_SourceFilePath_QStringList = folder_path_SourceFilePath.split("/");
        QString ParentFolderName = folder_path_SourceFilePath_QStringList.last();
        // 判断上级文件夹是不是磁盘根目录,若是则将文件夹名改为[盘符 Drive]
        if (ParentFolderName.contains(":"))
        {
            ParentFolderName = ParentFolderName.replace(":", " Drive");
        }
        Target_folder = OutPutFolder_main + "/" + ParentFolderName;
        file_mkDir(Target_folder);
    }
    else
    {
        Target_folder = OutPutFolder_main;
    }
    // 组装生成完整路径
    Target_fullpPath = Target_folder + "/" + Target_fileName;
    // 判断输出路径是否有和目标文件重名的 以及 是否启用了直接覆盖
    if (QFile::exists(Target_fullpPath) && (ui->checkBox_OutPath_Overwrite->isChecked() == false))
    {
        int suffix_int = 1;
        QString Target_fullpPath_tmp = "";
        while (true)
        {
            QFileInfo fileinfo_tmp(Target_fullpPath);
            QString file_name = file_getBaseName(Target_fullpPath);
            QString file_ext = fileinfo_tmp.suffix();
            QString file_path = file_getFolderPath(fileinfo_tmp);
            Target_fullpPath_tmp = file_path + "/" + file_name + "_" + QString::number(suffix_int, 10) + "." + file_ext;
            if (!QFile::exists(Target_fullpPath_tmp))
                break;
            suffix_int++;
        }
        Target_fullpPath = Target_fullpPath_tmp;
    }
    if (ui->checkBox_OutPath_Overwrite->isChecked() == true)
    {
        QFile::remove(Target_fullpPath);
    }
    if (QFile::rename(Orginal, Target_fullpPath) == false)
    {
        emit Send_TextBrowser_NewMessage("Error! Failed to move [" + Orginal + "] to [" + Target_fullpPath + "]");
    }
    MoveFile_QMutex.unlock();
}
/*
块大小调节 NCNN-Vulkan & Converter
*/
/*
块大小调整按钮 +
*/
int MainWindow::AddTileSize_NCNNVulkan_Converter(int OrginalTileSize)
{
    int TileSize = OrginalTileSize;
    // 判断是否为2的幂数
    if ((TileSize & (TileSize - 1)) == 0)
    {
        TileSize++;
    }
    if ((TileSize & (TileSize - 1)) != 0)
    {
        for (int intNum = 1; true; intNum++)
        {
            int pow_ = pow(2, intNum);
            if (pow_ > 999999999)
            {
                return OrginalTileSize;
            }
            if (pow_ >= TileSize)
            {
                return pow_;
            }
        }
    }
    return OrginalTileSize + 1;
}
/*
块大小调整按钮 -
*/
int MainWindow::MinusTileSize_NCNNVulkan_Converter(int OrginalTileSize)
{
    int TileSize = OrginalTileSize;
    // 判断是否为2的幂数
    if ((TileSize & (TileSize - 1)) == 0)
    {
        TileSize--;
    }
    if ((TileSize & (TileSize - 1)) != 0)
    {
        for (int intNum = 1; true; intNum++)
        {
            int pow_ = pow(2, intNum);
            if (pow_ > 999999999)
            {
                return OrginalTileSize;
            }
            if (pow_ >= TileSize)
            {
                return pow(2, intNum - 1);
            }
        }
    }
    return OrginalTileSize - 1;
}
/*
预读取引擎设定
*/
void MainWindow::PreLoad_Engines_Settings()
{
    //======================
    // Waifu2x-NCNN-Vulkan
    //======================
    if (ui->comboBox_Engine_Image->currentIndex() == 0 || ui->comboBox_Engine_GIF->currentIndex() == 0 || ui->comboBox_Engine_Video->currentIndex() == 0)
    {
        Waifu2x_NCNN_Vulkan_PreLoad_Settings_Str = Waifu2x_NCNN_Vulkan_PreLoad_Settings();
    }
    //======================
    // Waifu2x-Converter
    //======================
    if (ui->comboBox_Engine_Image->currentIndex() == 1 || ui->comboBox_Engine_GIF->currentIndex() == 1 || ui->comboBox_Engine_Video->currentIndex() == 1)
    {
        Waifu2xConverter_PreLoad_Settings_Str = Waifu2xConverter_PreLoad_Settings();
    }
    //======================
    // SRMD-NCNN-Vulkan
    //======================
    if (ui->comboBox_Engine_Image->currentIndex() == 2 || ui->comboBox_Engine_GIF->currentIndex() == 2 || ui->comboBox_Engine_Video->currentIndex() == 3)
    {
        SrmdNcnnVulkan_PreLoad_Settings_Str = SrmdNcnnVulkan_PreLoad_Settings();
    }
    //======================
    // Anime4k
    //======================
    if (ui->comboBox_Engine_Image->currentIndex() == 3 || ui->comboBox_Engine_GIF->currentIndex() == 3 || ui->comboBox_Engine_Video->currentIndex() == 2)
    {
        Anime4k_PreLoad_Settings_Str = Anime4k_PreLoad_Settings();
        // 预加载HDN等级
        if (ui->checkBox_ACNet_Anime4K->isChecked() && ui->checkBox_HDNMode_Anime4k->isChecked())
        {
            int HDNDenoiseLevel_value = 0;
            if (ui->comboBox_Engine_Image->currentIndex() == 3) // 图片
            {
                HDNDenoiseLevel_value = ui->spinBox_DenoiseLevel_image->value();
                if (HDNDenoiseLevel_value >= 1 && HDNDenoiseLevel_value <= 3)
                {
                    HDNDenoiseLevel_image.clear();
                    HDNDenoiseLevel_image.append("-L");
                    HDNDenoiseLevel_image.append(QString::number(HDNDenoiseLevel_value, 10));
                }
            }
            if (ui->comboBox_Engine_GIF->currentIndex() == 3) // GIF
            {
                HDNDenoiseLevel_value = ui->spinBox_DenoiseLevel_gif->value();
                if (HDNDenoiseLevel_value >= 1 && HDNDenoiseLevel_value <= 3)
                {
                    HDNDenoiseLevel_gif.clear();
                    HDNDenoiseLevel_gif.append("-L");
                    HDNDenoiseLevel_gif.append(QString::number(HDNDenoiseLevel_value, 10));
                }
            }
            if (ui->comboBox_Engine_Video->currentIndex() == 2) // 视频
            {
                HDNDenoiseLevel_value = ui->spinBox_DenoiseLevel_video->value();
                if (HDNDenoiseLevel_value >= 1 && HDNDenoiseLevel_value <= 3)
                {
                    HDNDenoiseLevel_video.clear();
                    HDNDenoiseLevel_video.append("-L");
                    HDNDenoiseLevel_video.append(QString::number(HDNDenoiseLevel_value, 10));
                }
            }
        }
    }
    //======================
    // Waifu2x-Caffe
    //======================
    if (isWaifu2xCaffeEnabled())
    {
        Waifu2xCaffe_PreLoad_Settings_Str = Waifu2xCaffe_PreLoad_Settings();
    }
    //======================
    // Realsr-NCNN-Vulkan
    //======================
    if (ui->comboBox_Engine_Image->currentIndex() == 5 || ui->comboBox_Engine_GIF->currentIndex() == 5 || ui->comboBox_Engine_Video->currentIndex() == 5)
    {
        Realsr_NCNN_Vulkan_PreLoad_Settings_Str = Realsr_NCNN_Vulkan_PreLoad_Settings();
    }
    //======================
    // RealESRGAN-NCNN-Vulkan
    //======================
    if (ui->comboBox_Engine_Image->currentIndex() == 5 || ui->comboBox_Engine_GIF->currentIndex() == 5 || ui->comboBox_Engine_Video->currentIndex() == 5)
    {
        RealESRGAN_NCNN_Vulkan_PreLoad_Settings_Str = RealESRGAN_NCNN_Vulkan_PreLoad_Settings();
    }
    //======================
    // RealCUGAN-NCNN-Vulkan
    //======================
    if (ui->comboBox_Engine_Image->currentIndex() == 5 || ui->comboBox_Engine_GIF->currentIndex() == 5 || ui->comboBox_Engine_Video->currentIndex() == 5)
    {
        RealCUGAN_NCNN_Vulkan_PreLoad_Settings_Str = RealCUGAN_NCNN_Vulkan_PreLoad_Settings();
    }
}
