﻿
#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Waifu2x_Caffe_Image(int rowNum, bool ReProcess_MissingAlphaChannel)
{
    //============================= 读取设置 ================================
    int ScaleRatio = ui->spinBox_ScaleRatio_image->value();
    int DenoiseLevel = ui->spinBox_DenoiseLevel_image->value();
    bool DelOriginal = (ui->checkBox_DelOriginal->isChecked() || ui->checkBox_ReplaceOriginalFile->isChecked());
    QString OutPutPath_Final = "";
    //========================= 拆解map得到参数 =============================
    QString status = "Processing";
    emit Send_Table_image_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    QString SourceFile_fullPath = Table_model_image->item(rowNum, 2)->text();
    if (!QFile::exists(SourceFile_fullPath))
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [File does not exist.]");
        status = "Failed";
        emit Send_Table_image_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        emit Send_progressbar_Add();
        mutex_ThreadNumRunning.lock();
        ThreadNumRunning--;
        mutex_ThreadNumRunning.unlock(); // 线程数量统计-1
        return 0;
    }
    //========= 转换到 PNG =========
    QString SourceFile_fullPath_Original = SourceFile_fullPath;
    SourceFile_fullPath = Imgae_PreProcess(SourceFile_fullPath_Original, ReProcess_MissingAlphaChannel);
    //=======================================================
    bool CustRes_isEnabled = false;
    int CustRes_height = 0;
    int CustRes_width = 0;
    if (CustRes_isContained(SourceFile_fullPath_Original))
    {
        CustRes_isEnabled = true;
        QMap<QString, QString> Res_map = CustRes_getResMap(SourceFile_fullPath_Original); // res_map["fullpath"],["height"],["width"]
        ScaleRatio = CustRes_CalNewScaleRatio(SourceFile_fullPath_Original, Res_map["height"].toInt(), Res_map["width"].toInt());
        if (ScaleRatio == 0)
        {
            emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath_Original + "]. Error: [The resolution of the source file cannot be read, so the image cannot be scaled to a custom resolution.]");
            status = "Failed";
            emit Send_Table_image_ChangeStatus_rowNumInt_statusQString(rowNum, status);
            emit Send_progressbar_Add();
            mutex_ThreadNumRunning.lock();
            ThreadNumRunning--;
            mutex_ThreadNumRunning.unlock(); // 线程数量统计-1
            return 0;
        }
        CustRes_height = Res_map["height"].toInt();
        CustRes_width = Res_map["width"].toInt();
    }
    //=======================================================
    QFileInfo fileinfo(SourceFile_fullPath);
    QString file_name = file_getBaseName(SourceFile_fullPath);
    QString file_ext = fileinfo.suffix();
    QString file_path = file_getFolderPath(fileinfo);
    QString OutPut_Path = file_path + "/" + file_name + "_waifu2x_" + QString::number(ScaleRatio, 10) + "x_" + QString::number(DenoiseLevel, 10) + "n_" + file_ext + ".png";
    //============================== 放大 =======================================
    QString Waifu2x_folder_path = Current_Path + "/waifu2x-caffe";
    QString program = Waifu2x_folder_path + "/waifu2x-caffe.exe";
    //====
    QStringList ImageProcessingModeCMD;
    if (DenoiseLevel == -1)
    {
        ImageProcessingModeCMD.append("-m");
        ImageProcessingModeCMD.append("scale");
        ImageProcessingModeCMD.append("-s");
        ImageProcessingModeCMD.append(QString::number(ScaleRatio, 10));
    }
    else
    {
        int DenoiseLevel_tmp = 0;
        if ((ui->comboBox_Model_2D_Waifu2xCaffe->currentIndex() == 0) && (DenoiseLevel == 0) && (ui->comboBox_ImageStyle_Waifu2xCaffe->currentIndex() == 0))
        {
            DenoiseLevel_tmp = 1;
        }
        else
        {
            DenoiseLevel_tmp = DenoiseLevel;
        }
        ImageProcessingModeCMD.append("-m");
        ImageProcessingModeCMD.append("noise_scale");
        ImageProcessingModeCMD.append("-s");
        ImageProcessingModeCMD.append(QString::number(ScaleRatio, 10));
        ImageProcessingModeCMD.append("-n");
        ImageProcessingModeCMD.append(QString::number(DenoiseLevel_tmp, 10));
    }
    //====
    QStringList args = {"-i", SourceFile_fullPath, "-o", OutPut_Path};
    args.append(ImageProcessingModeCMD);
    args.append(Waifu2x_Caffe_ReadSettings());
    //========
    for (int retry = 0; retry < (ui->spinBox_retry->value() + ForceRetryCount); retry++)
    {
        QProcess *Waifu2x = new QProcess();
        Waifu2x->start(program, args);
        ProcessWaitInfo info = waitForProcessWaifu2xEx(Waifu2x,INF_WAIT);
        if(info == TerminatedByFlag) {
            delete Waifu2x;
            return 0;
        } else if (info == StoppedByExFlag) {
            Waifu2x->close();
            QFile::remove(OutPut_Path);
            status = "Interrupted";
            emit Send_Table_image_ChangeStatus_rowNumInt_statusQString(rowNum, status);
            mutex_ThreadNumRunning.lock();
            ThreadNumRunning--;
            mutex_ThreadNumRunning.unlock();
            delete Waifu2x;
            return 0;
        }
        delete Waifu2x;
        if (QFile::exists(OutPut_Path))
        {
            break;
        }
        else
        {
            if (retry == ui->spinBox_retry->value() + (ForceRetryCount - 1))
                break;
            Delay_sec_sleep(5);
            emit Send_TextBrowser_NewMessage("Automatic retry, please wait.");
        }
    }
    //========
    if (!QFile::exists(OutPut_Path))
    {
        if (SourceFile_fullPath_Original != SourceFile_fullPath)
        {
            QFile::remove(SourceFile_fullPath);
            SourceFile_fullPath = SourceFile_fullPath_Original;
        }
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to scale the picture.]");
        status = "Failed";
        emit Send_Table_image_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        emit Send_progressbar_Add();
        mutex_ThreadNumRunning.lock();
        ThreadNumRunning--;
        mutex_ThreadNumRunning.unlock();
        return 0;
    }
    OutPutPath_Final = OutPut_Path;
    //============================ 调整大小 ====================================================
    if (CustRes_isEnabled)
    {
        int New_height = 0;
        int New_width = 0;
        if (CustRes_isEnabled)
        {
            New_height = CustRes_height;
            New_width = CustRes_width;
        }
        else
        {
            QMap<QString, int> res_map_OriginalFile = Image_Gif_Read_Resolution(SourceFile_fullPath);
            New_height = res_map_OriginalFile["height"] * ScaleRatio;
            New_width = res_map_OriginalFile["width"] * ScaleRatio;
        }
        QImage qimage_adj(OutPut_Path);
        // 读取放大后的图片并调整大小
        QImage qimage_adj_scaled = qimage_adj.scaled(New_width, New_height, CustRes_AspectRatioMode, Qt::SmoothTransformation);
        QImageWriter qimageW_adj;
        qimageW_adj.setFormat("png");
        qimageW_adj.setFileName(OutPut_Path);
        if (qimageW_adj.canWrite())
        {
            qimageW_adj.write(qimage_adj_scaled);
        }
        if (!QFile::exists(OutPut_Path))
        {
            emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to resize the scaled picture to the target size]");
            status = "Failed";
            emit Send_Table_image_ChangeStatus_rowNumInt_statusQString(rowNum, status);
            emit Send_progressbar_Add();
            mutex_ThreadNumRunning.lock();
            ThreadNumRunning--;
            mutex_ThreadNumRunning.unlock(); // 线程数量统计-1
            return 0;
        }
        OutPutPath_Final = OutPut_Path;
    }
    if (CustRes_isEnabled)
    {
        QString OutPut_Path_CustRes = file_path + "/" + file_name + "_waifu2x_" + QString::number(CustRes_width, 10) + "x" + QString::number(CustRes_height, 10) + "_" + QString::number(DenoiseLevel, 10) + "n_" + file_ext + ".png";
        QFile::rename(OutPut_Path, OutPut_Path_CustRes);
        OutPut_Path = OutPut_Path_CustRes;
        OutPutPath_Final = OutPut_Path;
    }
    //=========================== 另存为JPG&压缩JPG ===========================================
    OutPutPath_Final = SaveImageAs_FormatAndQuality(SourceFile_fullPath_Original, OutPutPath_Final, ScaleRatio, true, DenoiseLevel);
    //================== 检查是否丢失了透明通道 =====================
    if (ReProcess_MissingAlphaChannel == false)
    {
        QImage QImage_source(SourceFile_fullPath_Original);
        QImage QImage_Final(OutPutPath_Final);
        if (QImage_source.hasAlphaChannel() == true && QImage_Final.hasAlphaChannel() == false && ui->checkBox_AutoDetectAlphaChannel->isChecked())
        {
            QFile::remove(OutPutPath_Final);
            emit Send_TextBrowser_NewMessage("Since the Alpha channel in the resulting image is lost, this image will be reprocessed to fix it:[" + SourceFile_fullPath_Original + "].\nIf the reprocessing happens a lot, you should consider enable [Always pre-process images with Alpha Channel] in [Additional settings].");
            return Waifu2x_Caffe_Image(rowNum, true);
        }
    }
    //============================= 删除原文件 &  & 更新table status ============================
    if (SourceFile_fullPath_Original != SourceFile_fullPath)
    {
        QFile::remove(SourceFile_fullPath);
        SourceFile_fullPath = SourceFile_fullPath_Original;
    }
    if (DelOriginal)
    {
        if (ReplaceOriginalFile(SourceFile_fullPath, OutPutPath_Final) == false)
        {
            if (QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->isChecked())
            {
                file_MoveToTrash(SourceFile_fullPath);
            }
            else
            {
                QFile::remove(SourceFile_fullPath);
            }
        }
        status = "Finished, original file deleted";
        emit Send_Table_image_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    }
    else
    {
        status = "Finished";
        emit Send_Table_image_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    }
    //========== 移动到输出路径 =========
    if (ui->checkBox_OutPath_isEnabled->isChecked())
    {
        MoveFileToOutputPath(OutPutPath_Final, SourceFile_fullPath);
    }
    //============================ 更新进度条 =================================
    emit Send_progressbar_Add();
    //=========================== 更新线程数量统计==============================
    mutex_ThreadNumRunning.lock();
    ThreadNumRunning--;
    mutex_ThreadNumRunning.unlock(); // 线程数量统计-1
    return 0;
}

int MainWindow::Waifu2x_Caffe_GIF(int rowNum)
{
    //============================= 读取设置 ================================
    int ScaleRatio = ui->spinBox_ScaleRatio_gif->value();
    int DenoiseLevel = ui->spinBox_DenoiseLevel_gif->value();
    bool DelOriginal = (ui->checkBox_DelOriginal->isChecked() || ui->checkBox_ReplaceOriginalFile->isChecked());
    bool OptGIF = ui->checkBox_OptGIF->isChecked();
    int Sub_gif_ThreadNumRunning = 0;
    QString OutPutPath_Final = "";
    //========================= 拆解map得到参数 =============================
    QString status = "Processing";
    emit Send_Table_gif_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    QString SourceFile_fullPath = Table_model_gif->item(rowNum, 2)->text();
    if (!QFile::exists(SourceFile_fullPath))
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [File does not exist.]");
        status = "Failed";
        emit Send_Table_gif_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        emit Send_progressbar_Add();
        return 0;
    }
    //==========================
    bool CustRes_isEnabled = false;
    int CustRes_height = 0;
    int CustRes_width = 0;
    if (CustRes_isContained(SourceFile_fullPath))
    {
        CustRes_isEnabled = true;
        QMap<QString, QString> Res_map = CustRes_getResMap(SourceFile_fullPath); // res_map["fullpath"],["height"],["width"]
        CustRes_height = Res_map["height"].toInt();
        CustRes_width = Res_map["width"].toInt();
    }
    //==========================
    QFileInfo fileinfo(SourceFile_fullPath);
    QString file_name = file_getBaseName(SourceFile_fullPath);
    QString file_ext = fileinfo.suffix();
    QString file_path = file_getFolderPath(fileinfo);
    QString ResGIFPath = file_path + "/" + file_name + "_waifu2x_" + QString::number(ScaleRatio, 10) + "x_" + QString::number(DenoiseLevel, 10) + "n.gif";
    //=========================== 获取帧时间 ====================================
    int GIF_Duration = Gif_getDuration(SourceFile_fullPath);
    if (GIF_Duration == 0) // 检查是否成功获取duration
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Can't get Duration value of GIF file.]");
        status = "Failed";
        emit Send_Table_gif_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        // file_DelDir(SplitFramesFolderPath);
        emit Send_progressbar_Add();
        return 0; // 如果启用stop位,则直接return
    }
    //============================== 拆分 ==========================================
    QString SplitFramesFolderPath = file_path + "/" + file_name + "_splitFrames_waifu2x"; // 拆分后存储frame的文件夹
    Gif_splitGif(SourceFile_fullPath, SplitFramesFolderPath);
    //============================== 扫描获取文件名 ===============================
    QStringList Frame_fileName_list = file_getFileNames_in_Folder_nofilter(SplitFramesFolderPath);
    if (Frame_fileName_list.isEmpty()) // 检查是否成功拆分gif
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Can't split GIF into frames.]");
        status = "Failed";
        emit Send_Table_gif_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        file_DelDir(SplitFramesFolderPath);
        emit Send_progressbar_Add();
        return 0; // 如果启用stop位,则直接return
    }
    //===========建立存储放大后frame的文件夹===========
    QString ScaledFramesFolderPath = SplitFramesFolderPath + "/scaled";
    if (file_isDirExist(ScaledFramesFolderPath))
    {
        file_DelDir(ScaledFramesFolderPath);
        file_mkDir(ScaledFramesFolderPath);
    }
    else
    {
        file_mkDir(ScaledFramesFolderPath);
    }
    //==========开始放大==========================
    if (ui->checkBox_ShowInterPro->isChecked())
    {
        emit Send_CurrentFileProgress_Start(file_name + "." + file_ext, Frame_fileName_list.size());
    }
    //===============
    QMap<QString, QString> Sub_Thread_info;
    Sub_Thread_info["SplitFramesFolderPath"] = SplitFramesFolderPath;
    Sub_Thread_info["ScaledFramesFolderPath"] = ScaledFramesFolderPath;
    Sub_Thread_info["SourceFile_fullPath"] = SourceFile_fullPath;
    //===============
    //===============
    bool Frame_failed = false;
    //===============
    for (int i = 0; i < Frame_fileName_list.size(); i++)
    {
        if (ui->checkBox_ShowInterPro->isChecked())
        {
            emit Send_CurrentFileProgress_progressbar_Add();
        }
        int Sub_gif_ThreadNumMax = ui->spinBox_ThreadNum_gif_internal->value();
        if (waifu2x_STOP)
        {
            while (Sub_gif_ThreadNumRunning > 0)
            {
                Delay_msec_sleep(500);
            }
            file_DelDir(SplitFramesFolderPath);
            status = "Interrupted";
            emit Send_Table_gif_ChangeStatus_rowNumInt_statusQString(rowNum, status);
            return 0; // 如果启用stop位,则直接return
        }
        mutex_SubThreadNumRunning.lock();
        Sub_gif_ThreadNumRunning++;
        mutex_SubThreadNumRunning.unlock();
        QString Frame_fileName = Frame_fileName_list.at(i);
        Sub_Thread_info["Frame_fileName"] = Frame_fileName;
        QtConcurrent::run(this, &MainWindow::Waifu2x_Caffe_GIF_scale, Sub_Thread_info, &Sub_gif_ThreadNumRunning, &Frame_failed);
        while (Sub_gif_ThreadNumRunning >= Sub_gif_ThreadNumMax)
        {
            Delay_msec_sleep(500);
        }
        if (Frame_failed)
        {
            emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Failed to scale frames.]");
            status = "Failed";
            emit Send_Table_gif_ChangeStatus_rowNumInt_statusQString(rowNum, status);
            file_DelDir(SplitFramesFolderPath);
            emit Send_progressbar_Add();
            return 0; // 如果启用stop位,则直接return
        }
    }
    while (Sub_gif_ThreadNumRunning != 0)
    {
        Delay_msec_sleep(500);
    }
    emit Send_CurrentFileProgress_Stop();
    //======================= 检查是否成功放大所有帧 ===========================
    QStringList Frame_fileName_list_scaled = file_getFileNames_in_Folder_nofilter(ScaledFramesFolderPath);
    if (Frame_fileName_list.count() != Frame_fileName_list_scaled.count())
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Failed to scale frames.]");
        status = "Failed";
        emit Send_Table_gif_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        file_DelDir(SplitFramesFolderPath);
        emit Send_progressbar_Add();
        return 0; // 如果启用stop位,则直接return
    }
    //======================================== 组装 ======================================================
    if (CustRes_isEnabled)
    {
        ResGIFPath = file_path + "/" + file_name + "_waifu2x_" + QString::number(CustRes_width, 10) + "x" + QString::number(CustRes_height, 10) + "_" + QString::number(DenoiseLevel, 10) + "n.gif";
    }
    Gif_assembleGif(ResGIFPath, ScaledFramesFolderPath, GIF_Duration, CustRes_isEnabled, CustRes_height, CustRes_width);
    if (!QFile::exists(ResGIFPath))
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to assemble gif.]");
        status = "Failed";
        emit Send_Table_gif_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        file_DelDir(SplitFramesFolderPath);
        emit Send_progressbar_Add();
        return 0; // 如果启用stop位,则直接return
    }
    OutPutPath_Final = ResGIFPath;
    //======================================= 优化gif ===================================================
    if (OptGIF)
    {
        QString ResGIFPath_compressed = "";
        if (CustRes_isEnabled)
        {
            ResGIFPath_compressed = file_path + "/" + file_name + "_waifu2x_" + QString::number(CustRes_width, 10) + "x" + QString::number(CustRes_height, 10) + "_" + QString::number(DenoiseLevel, 10) + "n_opt.gif";
        }
        else
        {
            ResGIFPath_compressed = file_path + "/" + file_name + "_waifu2x_" + QString::number(ScaleRatio, 10) + "x_" + QString::number(DenoiseLevel, 10) + "n_opt.gif";
        }
        OutPutPath_Final = Gif_compressGif(ResGIFPath, ResGIFPath_compressed);
    }
    //============================== 删除缓存文件 ====================================================
    file_DelDir(SplitFramesFolderPath);
    //============================= 删除原文件 &  & 更新table status ============================
    if (DelOriginal)
    {
        if (ReplaceOriginalFile(SourceFile_fullPath, OutPutPath_Final) == false)
        {
            if (QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->isChecked())
            {
                file_MoveToTrash(SourceFile_fullPath);
            }
            else
            {
                QFile::remove(SourceFile_fullPath);
            }
        }
        status = "Finished, original file deleted";
        emit Send_Table_gif_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    }
    else
    {
        status = "Finished";
        emit Send_Table_gif_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    }
    //========== 移动到输出路径 =========
    if (ui->checkBox_OutPath_isEnabled->isChecked())
    {
        MoveFileToOutputPath(OutPutPath_Final, SourceFile_fullPath);
    }
    //============================ 更新进度条 =================================
    emit Send_progressbar_Add();
    //===========================  ==============================
    return 0;
}

int MainWindow::Waifu2x_Caffe_GIF_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_gif_ThreadNumRunning, bool *Frame_failed)
{
    QString SplitFramesFolderPath = Sub_Thread_info["SplitFramesFolderPath"];
    QString ScaledFramesFolderPath = Sub_Thread_info["ScaledFramesFolderPath"];
    QString SourceFile_fullPath = Sub_Thread_info["SourceFile_fullPath"];
    QString Frame_fileName = Sub_Thread_info["Frame_fileName"];
    //===========
    int ScaleRatio = ui->spinBox_ScaleRatio_gif->value();
    int DenoiseLevel = ui->spinBox_DenoiseLevel_gif->value();
    //========================================================================
    QString Waifu2x_folder_path = Current_Path + "/waifu2x-caffe";
    QString program = Waifu2x_folder_path + "/waifu2x-caffe.exe";
    QString InputPath = SplitFramesFolderPath + "/" + Frame_fileName;
    QString OutputPath = ScaledFramesFolderPath + "/" + Frame_fileName;
    //======
    if (CustRes_isContained(SourceFile_fullPath))
    {
        QMap<QString, QString> Res_map = CustRes_getResMap(SourceFile_fullPath); // res_map["fullpath"],["height"],["width"]
        ScaleRatio = CustRes_CalNewScaleRatio(InputPath, Res_map["height"].toInt(), Res_map["width"].toInt());
        if (ScaleRatio == 0)
        {
            emit Send_TextBrowser_NewMessage("Error occured when processing [" + InputPath + "]. Error: [The resolution of the source file cannot be read, so the image cannot be scaled to a custom resolution.]");
            mutex_SubThreadNumRunning.lock();
            *Sub_gif_ThreadNumRunning = *Sub_gif_ThreadNumRunning - 1;
            mutex_SubThreadNumRunning.unlock();
            return 0;
        }
    }
    //====
    QStringList ImageProcessingModeCMD;
    if (DenoiseLevel == -1)
    {
        ImageProcessingModeCMD.append("-m");
        ImageProcessingModeCMD.append("scale");
        ImageProcessingModeCMD.append("-s");
        ImageProcessingModeCMD.append(QString::number(ScaleRatio, 10));
    }
    else
    {
        int DenoiseLevel_tmp = 0;
        if ((ui->comboBox_Model_2D_Waifu2xCaffe->currentIndex() == 0) && (DenoiseLevel == 0) && (ui->comboBox_ImageStyle_Waifu2xCaffe->currentIndex() == 0))
        {
            DenoiseLevel_tmp = 1;
        }
        else
        {
            DenoiseLevel_tmp = DenoiseLevel;
        }
        ImageProcessingModeCMD.append("-m");
        ImageProcessingModeCMD.append("noise_scale");
        ImageProcessingModeCMD.append("-s");
        ImageProcessingModeCMD.append(QString::number(ScaleRatio, 10));
        ImageProcessingModeCMD.append("-n");
        ImageProcessingModeCMD.append(QString::number(DenoiseLevel_tmp, 10));
    }
    //=======
    for (int retry = 0; retry < (ui->spinBox_retry->value() + ForceRetryCount); retry++)
    {
        QStringList args = {"-i", InputPath, "-o", OutputPath};
        args.append(ImageProcessingModeCMD);
        args.append(Waifu2x_Caffe_ReadSettings());
        QProcess *Waifu2x = new QProcess();
        Waifu2x->start(program, args);
        ProcessWaitInfo info = waitForProcessWaifu2xEx(Waifu2x,INF_WAIT);
        if(info == TerminatedByFlag) {
            delete Waifu2x;
            return 0;
        } else if (info == StoppedByExFlag) {
            Waifu2x->close();
            mutex_SubThreadNumRunning.lock();
            *Sub_gif_ThreadNumRunning = *Sub_gif_ThreadNumRunning - 1;
            mutex_SubThreadNumRunning.unlock();
            delete Waifu2x;
            return 0;
        }
        delete Waifu2x;
        if (QFile::exists(OutputPath))
        {
            break;
        }
        else
        {
            if (retry == ui->spinBox_retry->value() + (ForceRetryCount - 1))
                break;
            Delay_sec_sleep(5);
            emit Send_TextBrowser_NewMessage("Automatic retry, please wait.");
        }
    }
    //========================
    QFile::remove(InputPath);
    if (QFile::exists(OutputPath) == false)
        *Frame_failed = true;
    //=======
    mutex_SubThreadNumRunning.lock();
    *Sub_gif_ThreadNumRunning = *Sub_gif_ThreadNumRunning - 1;
    mutex_SubThreadNumRunning.unlock();
    //========
    return 0;
}
/*
正常處理視頻(不分段)
*/
int MainWindow::Waifu2x_Caffe_Video(int rowNum)
{
    //============================= 读取设置 ================================
    int ScaleRatio = ui->spinBox_ScaleRatio_video->value();
    int DenoiseLevel = ui->spinBox_DenoiseLevel_video->value();
    bool DelOriginal = (ui->checkBox_DelOriginal->isChecked() || ui->checkBox_ReplaceOriginalFile->isChecked());
    bool isCacheExists = false;
    bool isVideoConfigChanged = true;
    int Sub_video_ThreadNumRunning = 0;
    QString OutPutPath_Final = "";
    //========================= 拆解map得到参数 =============================
    QString status = "Processing";
    emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    QString SourceFile_fullPath = Table_model_video->item(rowNum, 2)->text();
    if (!QFile::exists(SourceFile_fullPath))
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [File does not exist.]");
        status = "Failed";
        emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        emit Send_progressbar_Add();
        return 0;
    }
    //==========================
    bool CustRes_isEnabled = false;
    int CustRes_height = 0;
    int CustRes_width = 0;
    if (CustRes_isContained(SourceFile_fullPath))
    {
        CustRes_isEnabled = true;
        QMap<QString, QString> Res_map = CustRes_getResMap(SourceFile_fullPath); // res_map["fullpath"],["height"],["width"]
        CustRes_height = Res_map["height"].toInt();
        CustRes_width = Res_map["width"].toInt();
    }
    //==========================
    QFileInfo fileinfo(SourceFile_fullPath);
    QString file_name = file_getBaseName(SourceFile_fullPath);
    QString file_ext = fileinfo.suffix();
    QString file_path = file_getFolderPath(fileinfo);
    //===================================================================
    // 生成mp4
    QString video_mp4_fullpath = video_To_CFRMp4(SourceFile_fullPath);
    if (!QFile::exists(video_mp4_fullpath)) // 检查是否成功生成mp4
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Cannot convert video format to mp4.]");
        status = "Failed";
        emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        emit Send_progressbar_Add();
        return 0; // 如果启用stop位,则直接return
    }
    //=================
    QString AudioPath = file_path + "/audio_" + file_name + "_" + file_ext + "_waifu2x.wav";               // 音频
    QString SplitFramesFolderPath = file_path + "/" + file_name + "_" + file_ext + "_splitFrames_waifu2x"; // 拆分后存储frame的文件夹
    QString ScaledFramesFolderPath = SplitFramesFolderPath + "/scaled";                                    // 存储放大后的帧
    //==========================
    //   检测之前的视频配置文件
    //==========================
    QString VideoConfiguration_fullPath = file_path + "/VideoConfiguration_" + file_name + "_" + file_ext + ".ini";
    if (QFile::exists(VideoConfiguration_fullPath))
    {
        QSettings configIniRead(VideoConfiguration_fullPath, QSettings::IniFormat);
        configIniRead.setIniCodec(QTextCodec::codecForName("UTF-8"));
        //=================== 加载之前存储的视频信息 =========================
        int ScaleRatio_old = configIniRead.value("/VideoConfiguration/ScaleRatio").toInt();
        int DenoiseLevel_old = configIniRead.value("/VideoConfiguration/DenoiseLevel").toInt();
        bool CustRes_isEnabled_old = configIniRead.value("/VideoConfiguration/CustRes_isEnabled").toBool();
        int CustRes_height_old = configIniRead.value("/VideoConfiguration/CustRes_height").toInt();
        int CustRes_width_old = configIniRead.value("/VideoConfiguration/CustRes_width").toInt();
        QString EngineName_old = configIniRead.value("/VideoConfiguration/EngineName").toString();
        bool isProcessBySegment_old = configIniRead.value("/VideoConfiguration/isProcessBySegment").toBool();
        //=================== 比对信息 ================================
        if (EngineName_old == "waifu2x-caffe")
        {
            if (CustRes_isEnabled_old == false && CustRes_isEnabled == false)
            {
                if (ScaleRatio_old != ScaleRatio || DenoiseLevel_old != DenoiseLevel)
                {
                    isVideoConfigChanged = true;
                }
                else
                {
                    isVideoConfigChanged = false;
                }
            }
            else
            {
                if (CustRes_isEnabled_old == true && CustRes_isEnabled == true)
                {
                    if (CustRes_height_old != CustRes_height || CustRes_width_old != CustRes_width)
                    {
                        isVideoConfigChanged = true;
                    }
                    else
                    {
                        isVideoConfigChanged = false;
                    }
                }
                else
                {
                    isVideoConfigChanged = true;
                }
            }
            if (isProcessBySegment_old == true)
            {
                isVideoConfigChanged = true;
            }
        }
        else
        {
            isVideoConfigChanged = true;
        }
    }
    else
    {
        emit Send_video_write_VideoConfiguration(VideoConfiguration_fullPath, ScaleRatio, DenoiseLevel, CustRes_isEnabled, CustRes_height, CustRes_width, "waifu2x-caffe", false, "", "");
    }
    //=======================
    //   检测缓存是否存在
    //=======================
    if (QFile::exists(video_mp4_fullpath) && file_isDirExist(SplitFramesFolderPath) && file_isDirExist(ScaledFramesFolderPath) && QFile::exists(VideoConfiguration_fullPath))
    {
        if (!isVideoConfigChanged)
        {
            isCacheExists = true;
            emit Send_TextBrowser_NewMessage("The previous video cache file is detected and processing of the previous video cache will continue. If you want to restart processing of the current video:[" + SourceFile_fullPath + "], delete the cache manually.");
        }
        else
        {
            isCacheExists = false;
            //========
            QFile::remove(VideoConfiguration_fullPath);
            file_DelDir(SplitFramesFolderPath);
            QFile::remove(AudioPath);
            emit Send_video_write_VideoConfiguration(VideoConfiguration_fullPath, ScaleRatio, DenoiseLevel, CustRes_isEnabled, CustRes_height, CustRes_width, "waifu2x-caffe", false, "", "");
            //=======
            emit Send_TextBrowser_NewMessage("The previous video cache file was detected, but because you changed the settings about the video resolution or denoise level, the previous cache will be deleted and processing of the video:[" + SourceFile_fullPath + "] will restart.");
        }
    }
    else
    {
        isCacheExists = false;
        //========
        QFile::remove(VideoConfiguration_fullPath);
        file_DelDir(SplitFramesFolderPath);
        QFile::remove(AudioPath);
        emit Send_video_write_VideoConfiguration(VideoConfiguration_fullPath, ScaleRatio, DenoiseLevel, CustRes_isEnabled, CustRes_height, CustRes_width, "waifu2x-caffe", false, "", "");
        //========
    }
    if (!isCacheExists)
    {
        //============================== 拆分 ==========================================
        if (file_isDirExist(SplitFramesFolderPath))
        {
            file_DelDir(SplitFramesFolderPath);
            file_mkDir(SplitFramesFolderPath);
        }
        else
        {
            file_mkDir(SplitFramesFolderPath);
        }
        QFile::remove(AudioPath);
        video_video2images(video_mp4_fullpath, SplitFramesFolderPath, AudioPath);
    }
    //============================== 扫描获取文件名 ===============================
    QStringList Frame_fileName_list = file_getFileNames_in_Folder_nofilter(SplitFramesFolderPath);
    if (!isCacheExists)
    {
        if (Frame_fileName_list.isEmpty()) // 检查是否成功拆分为帧
        {
            emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to split video into pictures.]");
            status = "Failed";
            emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
            emit Send_progressbar_Add();
            return 0; // 如果启用stop位,则直接return
        }
    }
    //============================== 放大 =======================================
    //===========建立存储放大后frame的文件夹===========
    if (!isCacheExists)
    {
        if (file_isDirExist(ScaledFramesFolderPath))
        {
            file_DelDir(ScaledFramesFolderPath);
            file_mkDir(ScaledFramesFolderPath);
        }
        else
        {
            file_mkDir(ScaledFramesFolderPath);
        }
    }
    //==========开始放大==========================
    if (ui->checkBox_ShowInterPro->isChecked())
    {
        emit Send_CurrentFileProgress_Start(file_name + "." + file_ext, Frame_fileName_list.size());
    }
    //===============
    QMap<QString, QString> Sub_Thread_info;
    Sub_Thread_info["SplitFramesFolderPath"] = SplitFramesFolderPath;
    Sub_Thread_info["ScaledFramesFolderPath"] = ScaledFramesFolderPath;
    Sub_Thread_info["SourceFile_fullPath"] = SourceFile_fullPath;
    //===============
    //===============
    bool Frame_failed = false;
    //===============
    for (int i = 0; i < Frame_fileName_list.size(); i++)
    {
        if (ui->checkBox_ShowInterPro->isChecked())
        {
            emit Send_CurrentFileProgress_progressbar_Add();
        }
        int Sub_video_ThreadNumMax = ui->spinBox_ThreadNum_video_internal->value();
        if (waifu2x_STOP)
        {
            while (Sub_video_ThreadNumRunning > 0)
            {
                Delay_msec_sleep(500);
            }
            status = "Interrupted";
            emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
            return 0; // 如果启用stop位,则直接return
        }
        mutex_SubThreadNumRunning.lock();
        Sub_video_ThreadNumRunning++;
        mutex_SubThreadNumRunning.unlock();
        QString Frame_fileName = Frame_fileName_list.at(i);
        Sub_Thread_info["Frame_fileName"] = Frame_fileName;
        QtConcurrent::run(this, &MainWindow::Waifu2x_Caffe_Video_scale, Sub_Thread_info, &Sub_video_ThreadNumRunning, &Frame_failed);
        while (Sub_video_ThreadNumRunning >= Sub_video_ThreadNumMax)
        {
            Delay_msec_sleep(500);
        }
        if (Frame_failed)
        {
            emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to scale all frames.]");
            status = "Failed";
            emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
            emit Send_progressbar_Add();
            return 0; // 如果启用stop位,则直接return
        }
    }
    while (Sub_video_ThreadNumRunning != 0)
    {
        Delay_msec_sleep(500);
    }
    emit Send_CurrentFileProgress_Stop();
    //================ 扫描放大后的帧文件数量,判断是否放大成功 =======================
    QStringList Frame_fileName_list_scaled = file_getFileNames_in_Folder_nofilter(ScaledFramesFolderPath);
    if (Frame_fileName_list.count() > Frame_fileName_list_scaled.count())
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to scale all frames.]");
        status = "Failed";
        emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        emit Send_progressbar_Add();
        return 0; // 如果启用stop位,则直接return
    }
    //======================================== 组装 ======================================================
    QString video_mp4_scaled_fullpath = "";
    if (CustRes_isEnabled)
    {
        video_mp4_scaled_fullpath = file_path + "/" + file_name + "_waifu2x_" + QString::number(CustRes_width, 10) + "x" + QString::number(CustRes_height, 10) + "_" + QString::number(DenoiseLevel, 10) + "n" + "_" + file_ext + ".mp4";
    }
    else
    {
        video_mp4_scaled_fullpath = file_path + "/" + file_name + "_waifu2x_" + QString::number(ScaleRatio, 10) + "x_" + QString::number(DenoiseLevel, 10) + "n" + "_" + file_ext + ".mp4";
    }
    QFile::remove(video_mp4_scaled_fullpath);
    video_images2video(video_mp4_fullpath, video_mp4_scaled_fullpath, ScaledFramesFolderPath, AudioPath, CustRes_isEnabled, CustRes_height, CustRes_width);
    if (!QFile::exists(video_mp4_scaled_fullpath)) // 检查是否成功成功生成视频
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to assemble pictures into videos.]");
        status = "Failed";
        emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        emit Send_progressbar_Add();
        return 0; // 如果启用stop位,则直接return
    }
    OutPutPath_Final = video_mp4_scaled_fullpath;
    //============================== 删除缓存文件 ====================================================
    if (ui->checkBox_KeepVideoCache->isChecked() == false)
    {
        QFile::remove(VideoConfiguration_fullPath);
        file_DelDir(SplitFramesFolderPath);
        QFile::remove(AudioPath);
        if (SourceFile_fullPath != video_mp4_fullpath)
            QFile::remove(video_mp4_fullpath);
    }
    else
    {
        DelOriginal = false;
    }
    //============================= 删除原文件 &  & 更新table status ============================
    if (DelOriginal)
    {
        if (ReplaceOriginalFile(SourceFile_fullPath, OutPutPath_Final) == false)
        {
            if (QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->isChecked())
            {
                file_MoveToTrash(SourceFile_fullPath);
            }
            else
            {
                QFile::remove(SourceFile_fullPath);
            }
        }
        status = "Finished, original file deleted";
        emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    }
    else
    {
        status = "Finished";
        emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    }
    //========== 移动到输出路径 =========
    if (ui->checkBox_OutPath_isEnabled->isChecked())
    {
        MoveFileToOutputPath(OutPutPath_Final, SourceFile_fullPath);
    }
    //============================ 更新进度条 =================================
    emit Send_progressbar_Add();
    //===========================  ==============================
    return 0;
}

/*
#### 分段处理 ####
调用Waifu2x-caffe处理视频
读取设置,拆分,创建放大子线程,组装视频
*/
int MainWindow::Waifu2x_Caffe_Video_BySegment(int rowNum)
{
    //============================= 读取设置 ================================
    int ScaleRatio = ui->spinBox_ScaleRatio_video->value();
    int DenoiseLevel = ui->spinBox_DenoiseLevel_video->value();
    bool DelOriginal = (ui->checkBox_DelOriginal->isChecked() || ui->checkBox_ReplaceOriginalFile->isChecked());
    bool isCacheExists = false;
    bool isVideoConfigChanged = true;
    int Sub_video_ThreadNumRunning = 0;
    QString OutPutPath_Final = "";
    int SegmentDuration = ui->spinBox_SegmentDuration->value();
    //========================= 拆解map得到参数 =============================
    QString status = "Processing";
    emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    QString SourceFile_fullPath = Table_model_video->item(rowNum, 2)->text();
    if (!QFile::exists(SourceFile_fullPath))
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [File does not exist.]");
        status = "Failed";
        emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        emit Send_progressbar_Add();
        return 0;
    }
    //==========================
    bool CustRes_isEnabled = false;
    int CustRes_height = 0;
    int CustRes_width = 0;
    if (CustRes_isContained(SourceFile_fullPath))
    {
        CustRes_isEnabled = true;
        QMap<QString, QString> Res_map = CustRes_getResMap(SourceFile_fullPath); // res_map["fullpath"],["height"],["width"]
        CustRes_height = Res_map["height"].toInt();
        CustRes_width = Res_map["width"].toInt();
    }
    //==========================
    QFileInfo fileinfo(SourceFile_fullPath);
    QString file_name = file_getBaseName(SourceFile_fullPath);
    QString file_ext = fileinfo.suffix();
    QString file_path = file_getFolderPath(fileinfo);
    //===================================================================
    // 生成mp4
    QString video_mp4_fullpath = video_To_CFRMp4(SourceFile_fullPath);
    if (!QFile::exists(video_mp4_fullpath)) // 检查是否成功生成mp4
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Cannot convert video format to mp4.]");
        status = "Failed";
        emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        emit Send_progressbar_Add();
        return 0; // 如果启用stop位,则直接return
    }
    //=================
    QString AudioPath = file_path + "/audio_" + file_name + "_" + file_ext + "_waifu2x.wav";               // 音频
    QString SplitFramesFolderPath = file_path + "/" + file_name + "_" + file_ext + "_splitFrames_waifu2x"; // 拆分后存储frame的文件夹
    QString ScaledFramesFolderPath = SplitFramesFolderPath + "/scaled";                                    // 存储放大后的帧
    //===
    QString VideoClipsFolderPath = ""; // 存储视频片段的文件夹(完整路径)
    QString DateStr = "";
    do
    {
        DateStr = video_getClipsFolderNo();
        VideoClipsFolderPath = file_path + "/" + DateStr + "_VideoClips"; // 存储视频片段的文件夹(完整路径)
    } while (file_isDirExist(VideoClipsFolderPath));
    QString VideoClipsFolderName = DateStr + "_VideoClips"; // 存储视频片段的文件夹(名称)
    //==========================
    //   检测之前的视频配置文件
    //==========================
    QString VideoConfiguration_fullPath = file_path + "/VideoConfiguration_" + file_name + "_" + file_ext + ".ini";
    if (QFile::exists(VideoConfiguration_fullPath))
    {
        QSettings configIniRead(VideoConfiguration_fullPath, QSettings::IniFormat);
        configIniRead.setIniCodec(QTextCodec::codecForName("UTF-8"));
        //=================== 加载之前存储的视频信息 =========================
        int ScaleRatio_old = configIniRead.value("/VideoConfiguration/ScaleRatio").toInt();
        int DenoiseLevel_old = configIniRead.value("/VideoConfiguration/DenoiseLevel").toInt();
        bool CustRes_isEnabled_old = configIniRead.value("/VideoConfiguration/CustRes_isEnabled").toBool();
        int CustRes_height_old = configIniRead.value("/VideoConfiguration/CustRes_height").toInt();
        int CustRes_width_old = configIniRead.value("/VideoConfiguration/CustRes_width").toInt();
        QString EngineName_old = configIniRead.value("/VideoConfiguration/EngineName").toString();
        bool isProcessBySegment_old = configIniRead.value("/VideoConfiguration/isProcessBySegment").toBool();
        //=================== 比对信息 ================================
        if (EngineName_old == "waifu2x-caffe")
        {
            if (CustRes_isEnabled_old == false && CustRes_isEnabled == false)
            {
                if (ScaleRatio_old != ScaleRatio || DenoiseLevel_old != DenoiseLevel)
                {
                    isVideoConfigChanged = true;
                }
                else
                {
                    isVideoConfigChanged = false;
                }
            }
            else
            {
                if (CustRes_isEnabled_old == true && CustRes_isEnabled == true)
                {
                    if (CustRes_height_old != CustRes_height || CustRes_width_old != CustRes_width)
                    {
                        isVideoConfigChanged = true;
                    }
                    else
                    {
                        isVideoConfigChanged = false;
                    }
                }
                else
                {
                    isVideoConfigChanged = true;
                }
            }
            if (isProcessBySegment_old == false)
            {
                isVideoConfigChanged = true;
            }
        }
        else
        {
            isVideoConfigChanged = true;
        }
        //============ 修正文件夹名称 =============
        QString VideoClipsFolderPath_old = configIniRead.value("/VideoConfiguration/VideoClipsFolderPath").toString();
        QString VideoClipsFolderName_old = configIniRead.value("/VideoConfiguration/VideoClipsFolderName").toString();
        file_mkDir(VideoClipsFolderPath_old);
        if (file_isDirExist(VideoClipsFolderPath_old) == true)
        {
            VideoClipsFolderPath = VideoClipsFolderPath_old;
            VideoClipsFolderName = VideoClipsFolderName_old;
        }
    }
    else
    {
        emit Send_video_write_VideoConfiguration(VideoConfiguration_fullPath, ScaleRatio, DenoiseLevel, CustRes_isEnabled, CustRes_height, CustRes_width, "waifu2x-caffe", true, VideoClipsFolderPath, VideoClipsFolderName);
    }
    //=======================
    //   检测缓存是否存在
    //=======================
    if (file_isDirExist(SplitFramesFolderPath))
    {
        if (!isVideoConfigChanged)
        {
            isCacheExists = true;
            emit Send_TextBrowser_NewMessage("The previous video cache file is detected and processing of the previous video cache will continue. If you want to restart processing of the current video:[" + SourceFile_fullPath + "], delete the cache manually.");
        }
        else
        {
            isCacheExists = false;
            //========
            QFile::remove(VideoConfiguration_fullPath);
            file_DelDir(SplitFramesFolderPath);
            file_DelDir(VideoClipsFolderPath);
            QFile::remove(AudioPath);
            emit Send_video_write_VideoConfiguration(VideoConfiguration_fullPath, ScaleRatio, DenoiseLevel, CustRes_isEnabled, CustRes_height, CustRes_width, "waifu2x-caffe", true, VideoClipsFolderPath, VideoClipsFolderName);
            //=======
            emit Send_TextBrowser_NewMessage("The previous video cache file was detected, but because you changed the settings about the video resolution or denoise level, the previous cache will be deleted and processing of the video:[" + SourceFile_fullPath + "] will restart.");
        }
    }
    else
    {
        isCacheExists = false;
        //========
        QFile::remove(VideoConfiguration_fullPath);
        file_DelDir(SplitFramesFolderPath);
        file_DelDir(VideoClipsFolderPath);
        QFile::remove(AudioPath);
        emit Send_video_write_VideoConfiguration(VideoConfiguration_fullPath, ScaleRatio, DenoiseLevel, CustRes_isEnabled, CustRes_height, CustRes_width, "waifu2x-caffe", true, VideoClipsFolderPath, VideoClipsFolderName);
        //========
    }
    /*====================================
                  提取音频
    ======================================*/
    if (!QFile::exists(AudioPath))
    {
        video_get_audio(video_mp4_fullpath, AudioPath);
    }
    //================================== 开始分段处理视频 =================================================
    int StartTime = 0; // 起始时间(秒)
    int VideoDuration = video_get_duration(video_mp4_fullpath);
    bool isSplitComplete = false;
    bool isScaleComplete = false;
    /*
    ============================================
                  开始之前先读取进度
    ============================================
    */
    int OLD_SegmentDuration = -1;
    bool read_OLD_SegmentDuration = false;
    if (QFile::exists(VideoConfiguration_fullPath))
    {
        QSettings configIniRead(VideoConfiguration_fullPath, QSettings::IniFormat);
        configIniRead.setIniCodec(QTextCodec::codecForName("UTF-8"));
        //=================== 加载进度 =========================
        StartTime = configIniRead.value("/Progress/StartTime").toInt();
        isSplitComplete = configIniRead.value("/Progress/isSplitComplete").toBool();
        isScaleComplete = configIniRead.value("/Progress/isScaleComplete").toBool();
        OLD_SegmentDuration = configIniRead.value("/Progress/OLDSegmentDuration").toInt();
    }
    if (OLD_SegmentDuration > 0)
    {
        read_OLD_SegmentDuration = true;
    }
    /*
    加载进度条
    */
    int SegmentDuration_tmp_progressbar = 0;
    if (read_OLD_SegmentDuration)
    {
        SegmentDuration_tmp_progressbar = OLD_SegmentDuration;
    }
    else
    {
        SegmentDuration_tmp_progressbar = SegmentDuration;
    }
    if (ui->checkBox_ShowInterPro->isChecked() && VideoDuration > SegmentDuration_tmp_progressbar)
    {
        emit Send_CurrentFileProgress_Start(file_name + "." + file_ext, VideoDuration);
        if (StartTime > 0)
        {
            emit Send_CurrentFileProgress_progressbar_Add_SegmentDuration(StartTime);
        }
    }
    /*
    ============================================
                    正式开始处理
    ============================================
    */
    while (VideoDuration > StartTime)
    {
        /*==========================
                计算视频片段时间
        ==========================*/
        int SegmentDuration_tmp = 0;
        int TimeLeft_tmp = VideoDuration - StartTime;
        if ((TimeLeft_tmp) >= SegmentDuration)
        {
            SegmentDuration_tmp = SegmentDuration;
        }
        else
        {
            SegmentDuration_tmp = TimeLeft_tmp;
        }
        if (read_OLD_SegmentDuration)
        {
            SegmentDuration_tmp = OLD_SegmentDuration;
            read_OLD_SegmentDuration = false;
        }
        /*==========================
                 拆分视频片段
        ==========================*/
        if (isSplitComplete == false)
        {
            if (file_isDirExist(SplitFramesFolderPath))
            {
                file_DelDir(SplitFramesFolderPath);
                file_mkDir(SplitFramesFolderPath);
            }
            else
            {
                file_mkDir(SplitFramesFolderPath);
            }
            video_video2images_ProcessBySegment(video_mp4_fullpath, SplitFramesFolderPath, StartTime, SegmentDuration_tmp);
        }
        /*==========================
               处理视频片段的帧
        ==========================*/
        if (isScaleComplete == false)
        {
            //============================== 扫描获取文件名 ===============================
            QStringList Frame_fileName_list = file_getFileNames_in_Folder_nofilter(SplitFramesFolderPath);
            if (isSplitComplete == false)
            {
                if (Frame_fileName_list.isEmpty()) // 检查是否成功拆分为帧
                {
                    emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to split video into pictures.]");
                    status = "Failed";
                    emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
                    emit Send_progressbar_Add();
                    return 0; // 如果启用stop位,则直接return
                }
            }
            /*
            记录进度
            帧拆分成功
            */
            emit Send_video_write_Progress_ProcessBySegment(VideoConfiguration_fullPath, StartTime, true, false, SegmentDuration_tmp);
            //============================== 放大 =======================================
            //===========建立存储放大后frame的文件夹===========
            if (isSplitComplete == false)
            {
                if (file_isDirExist(ScaledFramesFolderPath))
                {
                    file_DelDir(ScaledFramesFolderPath);
                    file_mkDir(ScaledFramesFolderPath);
                }
                else
                {
                    file_mkDir(ScaledFramesFolderPath);
                }
            }
            //==========开始放大==========================
            int InterPro_total = 0;
            int InterPro_now = 0;
            if (ui->checkBox_ShowInterPro->isChecked())
            {
                InterPro_total = Frame_fileName_list.size();
            }
            //===============
            QMap<QString, QString> Sub_Thread_info;
            Sub_Thread_info["SplitFramesFolderPath"] = SplitFramesFolderPath;
            Sub_Thread_info["ScaledFramesFolderPath"] = ScaledFramesFolderPath;
            Sub_Thread_info["SourceFile_fullPath"] = SourceFile_fullPath;
            //===============
            bool Frame_failed = false;
            //===============
            for (int i = 0; i < Frame_fileName_list.size(); i++)
            {
                if (ui->checkBox_ShowInterPro->isChecked())
                {
                    InterPro_now++;
                    emit Send_TextBrowser_NewMessage("File name:[" + SourceFile_fullPath + "]  Scale and Denoise progress:[" + QString::number(InterPro_now, 10) + "/" + QString::number(InterPro_total, 10) + "] Duration progress:[" + QString::number(StartTime, 10) + "s/" + QString::number(VideoDuration, 10) + "s]");
                }
                int Sub_video_ThreadNumMax = ui->spinBox_ThreadNum_video_internal->value();
                if (waifu2x_STOP)
                {
                    while (Sub_video_ThreadNumRunning > 0)
                    {
                        Delay_msec_sleep(500);
                    }
                    status = "Interrupted";
                    emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
                    return 0; // 如果启用stop位,则直接return
                }
                mutex_SubThreadNumRunning.lock();
                Sub_video_ThreadNumRunning++;
                mutex_SubThreadNumRunning.unlock();
                QString Frame_fileName = Frame_fileName_list.at(i);
                Sub_Thread_info["Frame_fileName"] = Frame_fileName;
                QtConcurrent::run(this, &MainWindow::Waifu2x_Caffe_Video_scale, Sub_Thread_info, &Sub_video_ThreadNumRunning, &Frame_failed);
                while (Sub_video_ThreadNumRunning >= Sub_video_ThreadNumMax)
                {
                    Delay_msec_sleep(500);
                }
                if (Frame_failed)
                {
                    emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to scale all frames.]");
                    status = "Failed";
                    emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
                    emit Send_progressbar_Add();
                    return 0; // 如果启用stop位,则直接return
                }
            }
            while (Sub_video_ThreadNumRunning != 0)
            {
                Delay_msec_sleep(500);
            }
            //================ 扫描放大后的帧文件数量,判断是否放大成功 =======================
            QStringList Frame_fileName_list_scaled = file_getFileNames_in_Folder_nofilter(ScaledFramesFolderPath);
            if (Frame_fileName_list_scaled.count() < Frame_fileName_list.count())
            {
                emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to scale all frames.]");
                status = "Failed";
                emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
                emit Send_progressbar_Add();
                return 0; // 如果启用stop位,则直接return
            }
            /*
            记录进度
            帧处理成功
            */
            emit Send_video_write_Progress_ProcessBySegment(VideoConfiguration_fullPath, StartTime, true, true, SegmentDuration_tmp);
        }
        /*==========================
            组装视频片段(由帧到视频)
        ==========================*/
        if (!file_isDirExist(VideoClipsFolderPath))
        {
            file_mkDir(VideoClipsFolderPath);
        }
        int VideoClipNo = (StartTime + SegmentDuration) / SegmentDuration;
        QString video_mp4_scaled_clip_fullpath = VideoClipsFolderPath + "/" + QString::number(VideoClipNo, 10) + ".mp4";
        QFile::remove(video_mp4_scaled_clip_fullpath);
        video_images2video(video_mp4_fullpath, video_mp4_scaled_clip_fullpath, ScaledFramesFolderPath, "", CustRes_isEnabled, CustRes_height, CustRes_width);
        if (!QFile::exists(video_mp4_scaled_clip_fullpath)) // 检查是否成功成功生成视频
        {
            emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to assemble pictures into videos.]");
            status = "Failed";
            emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
            emit Send_progressbar_Add();
            return 0; // 如果启用stop位,则直接return
        }
        /*==========================
        处理完当前片段,时间后移,记录起始时间
        ==========================*/
        if (ui->checkBox_ShowInterPro->isChecked())
        {
            emit Send_CurrentFileProgress_progressbar_Add_SegmentDuration(SegmentDuration_tmp);
        }
        StartTime += SegmentDuration_tmp;
        isSplitComplete = false;
        isScaleComplete = false;
        emit Send_video_write_Progress_ProcessBySegment(VideoConfiguration_fullPath, StartTime, false, false, -1);
    }
    emit Send_CurrentFileProgress_Stop();
    //======================================================
    //                    组装(片段到成片)
    //======================================================
    QString video_mp4_scaled_fullpath = "";
    if (CustRes_isEnabled)
    {
        video_mp4_scaled_fullpath = file_path + "/" + file_name + "_waifu2x_" + QString::number(CustRes_width, 10) + "x" + QString::number(CustRes_height, 10) + "_" + QString::number(DenoiseLevel, 10) + "n" + "_" + file_ext + ".mp4";
    }
    else
    {
        video_mp4_scaled_fullpath = file_path + "/" + file_name + "_waifu2x_" + QString::number(ScaleRatio, 10) + "x_" + QString::number(DenoiseLevel, 10) + "n" + "_" + file_ext + ".mp4";
    }
    QFile::remove(video_mp4_scaled_fullpath);
    video_AssembleVideoClips(VideoClipsFolderPath, VideoClipsFolderName, video_mp4_scaled_fullpath, AudioPath);
    if (!QFile::exists(video_mp4_scaled_fullpath)) // 检查是否成功生成视频
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + SourceFile_fullPath + "]. Error: [Unable to assemble video clips.]");
        status = "Failed";
        emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
        emit Send_progressbar_Add();
        return 0; // 如果启用stop位,则直接return
    }
    OutPutPath_Final = video_mp4_scaled_fullpath;
    //============================== 删除缓存文件 ====================================================
    if (ui->checkBox_KeepVideoCache->isChecked() == false)
    {
        QFile::remove(VideoConfiguration_fullPath);
        file_DelDir(SplitFramesFolderPath);
        file_DelDir(VideoClipsFolderPath);
        QFile::remove(AudioPath);
        if (SourceFile_fullPath != video_mp4_fullpath)
            QFile::remove(video_mp4_fullpath);
    }
    else
    {
        DelOriginal = false;
    }
    //============================= 删除原文件 &  & 更新table status ============================
    if (DelOriginal)
    {
        if (ReplaceOriginalFile(SourceFile_fullPath, OutPutPath_Final) == false)
        {
            if (QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->isChecked())
            {
                file_MoveToTrash(SourceFile_fullPath);
            }
            else
            {
                QFile::remove(SourceFile_fullPath);
            }
        }
        status = "Finished, original file deleted";
        emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    }
    else
    {
        status = "Finished";
        emit Send_Table_video_ChangeStatus_rowNumInt_statusQString(rowNum, status);
    }
    //========== 移动到输出路径 =========
    if (ui->checkBox_OutPath_isEnabled->isChecked())
    {
        MoveFileToOutputPath(OutPutPath_Final, SourceFile_fullPath);
    }
    //============================ 更新进度条 =================================
    emit Send_progressbar_Add();
    //===========================  ==============================
    return 0;
}

int MainWindow::Waifu2x_Caffe_Video_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_video_ThreadNumRunning, bool *Frame_failed)
{
    QString SplitFramesFolderPath = Sub_Thread_info["SplitFramesFolderPath"];
    QString ScaledFramesFolderPath = Sub_Thread_info["ScaledFramesFolderPath"];
    QString SourceFile_fullPath = Sub_Thread_info["SourceFile_fullPath"];
    QString Frame_fileName = Sub_Thread_info["Frame_fileName"];
    //===========
    int ScaleRatio = ui->spinBox_ScaleRatio_video->value();
    int DenoiseLevel = ui->spinBox_DenoiseLevel_video->value();
    QString Frame_fileFullPath = SplitFramesFolderPath + "/" + Frame_fileName;
    //========================================================================
    QString Waifu2x_folder_path = Current_Path + "/waifu2x-caffe";
    QString program = Waifu2x_folder_path + "/waifu2x-caffe.exe";
    QString InputPath = SplitFramesFolderPath + "/" + Frame_fileName;
    QString OutputPath = ScaledFramesFolderPath + "/" + Frame_fileName;
    //======
    if (CustRes_isContained(SourceFile_fullPath))
    {
        QMap<QString, QString> Res_map = CustRes_getResMap(SourceFile_fullPath); // res_map["fullpath"],["height"],["width"]
        ScaleRatio = CustRes_CalNewScaleRatio(InputPath, Res_map["height"].toInt(), Res_map["width"].toInt());
        if (ScaleRatio == 0)
        {
            emit Send_TextBrowser_NewMessage("Error occured when processing [" + InputPath + "]. Error: [The resolution of the source file cannot be read, so the image cannot be scaled to a custom resolution.]");
            mutex_SubThreadNumRunning.lock();
            *Sub_video_ThreadNumRunning = *Sub_video_ThreadNumRunning - 1;
            mutex_SubThreadNumRunning.unlock();
            return 0;
        }
    }
    //====
    QStringList ImageProcessingModeCMD;
    if (DenoiseLevel == -1)
    {
        ImageProcessingModeCMD.append("-m");
        ImageProcessingModeCMD.append("scale");
        ImageProcessingModeCMD.append("-s");
        ImageProcessingModeCMD.append(QString::number(ScaleRatio, 10));
    }
    else
    {
        int DenoiseLevel_tmp = 0;
        if ((ui->comboBox_Model_2D_Waifu2xCaffe->currentIndex() == 0) && (DenoiseLevel == 0) && (ui->comboBox_ImageStyle_Waifu2xCaffe->currentIndex() == 0))
        {
            DenoiseLevel_tmp = 1;
        }
        else
        {
            DenoiseLevel_tmp = DenoiseLevel;
        }
        ImageProcessingModeCMD.append("-m");
        ImageProcessingModeCMD.append("noise_scale");
        ImageProcessingModeCMD.append("-s");
        ImageProcessingModeCMD.append(QString::number(ScaleRatio, 10));
        ImageProcessingModeCMD.append("-n");
        ImageProcessingModeCMD.append(QString::number(DenoiseLevel_tmp, 10));
    }
    //=======
    for (int retry = 0; retry < (ui->spinBox_retry->value() + ForceRetryCount); retry++)
    {
        QStringList args = {"-i", InputPath, "-o", OutputPath};
        args.append(ImageProcessingModeCMD);
        args.append(Waifu2x_Caffe_ReadSettings());
        QProcess *Waifu2x = new QProcess();
        Waifu2x->start(program, args);
        ProcessWaitInfo info = waitForProcessWaifu2xEx(Waifu2x,INF_WAIT);
        if(info == TerminatedByFlag) {
            delete Waifu2x;
            return 0;
        } else if (info == StoppedByExFlag) {
            Waifu2x->close();
            mutex_SubThreadNumRunning.lock();
            *Sub_video_ThreadNumRunning = *Sub_video_ThreadNumRunning - 1;
            mutex_SubThreadNumRunning.unlock();
            delete Waifu2x;
            return 0;
        }
        delete Waifu2x;
        if (QFile::exists(OutputPath))
        {
            break;
        }
        else
        {
            if (retry == ui->spinBox_retry->value() + (ForceRetryCount - 1))
                break;
            Delay_sec_sleep(5);
            emit Send_TextBrowser_NewMessage("Automatic retry, please wait.");
        }
    }
    //========
    if (QFile::exists(OutputPath) == true)
        QFile::remove(Frame_fileFullPath);
    if (QFile::exists(OutputPath) == false)
        *Frame_failed = true;
    //========
    mutex_SubThreadNumRunning.lock();
    *Sub_video_ThreadNumRunning = *Sub_video_ThreadNumRunning - 1;
    mutex_SubThreadNumRunning.unlock();
    //========
    return 0;
}
/*
Waifu2x_Caffe
预读取引擎设定
*/
QStringList MainWindow::Waifu2xCaffe_PreLoad_Settings()
{
    QStringList Waifu2x_Caffe_Settings_str;
    // 处理模式
    switch (ui->comboBox_ProcessMode_Waifu2xCaffe->currentIndex())
    {
    case 0:
    {
        Waifu2x_Caffe_Settings_str.append("-p");
        Waifu2x_Caffe_Settings_str.append("-cpu");
        break;
    }
    case 1:
    {
        Waifu2x_Caffe_Settings_str.append("-p");
        Waifu2x_Caffe_Settings_str.append("-gpu");
        break;
    }
    case 2:
    {
        Waifu2x_Caffe_Settings_str.append("-p");
        Waifu2x_Caffe_Settings_str.append("-cudnn");
        break;
    }
    }
    // 模型
    QString model_folder_path = Current_Path + "/waifu2x-caffe/models/";
    int ImageStyle = ui->comboBox_ImageStyle_Waifu2xCaffe->currentIndex();
    switch (ImageStyle)
    {
    case 0:
    {
        switch (ui->comboBox_Model_2D_Waifu2xCaffe->currentIndex())
        {
        case 0:
        {
            Waifu2x_Caffe_Settings_str.append("--model_dir");
            Waifu2x_Caffe_Settings_str.append(model_folder_path + "anime_style_art");
            break;
        }
        case 1:
        {
            Waifu2x_Caffe_Settings_str.append("--model_dir");
            Waifu2x_Caffe_Settings_str.append(model_folder_path + "anime_style_art_rgb");
            break;
        }
        case 2:
        {
            Waifu2x_Caffe_Settings_str.append("--model_dir");
            Waifu2x_Caffe_Settings_str.append(model_folder_path + "cunet");
            break;
        }
        case 3:
        {
            Waifu2x_Caffe_Settings_str.append("--model_dir");
            Waifu2x_Caffe_Settings_str.append(model_folder_path + "photo");
            break;
        }
        case 4:
        {
            Waifu2x_Caffe_Settings_str.append("--model_dir");
            Waifu2x_Caffe_Settings_str.append(model_folder_path + "upconv_7_anime_style_art_rgb");
            break;
        }
        case 5:
        {
            Waifu2x_Caffe_Settings_str.append("--model_dir");
            Waifu2x_Caffe_Settings_str.append(model_folder_path + "upconv_7_photo");
            break;
        }
        case 6:
        {
            Waifu2x_Caffe_Settings_str.append("--model_dir");
            Waifu2x_Caffe_Settings_str.append(model_folder_path + "upresnet10");
            break;
        }
        }
        break;
    }
    case 1:
    {
        switch (ui->comboBox_Model_3D_Waifu2xCaffe->currentIndex())
        {
        case 0:
        {
            Waifu2x_Caffe_Settings_str.append("--model_dir");
            Waifu2x_Caffe_Settings_str.append(model_folder_path + "upconv_7_photo");
            break;
        }
        case 1:
        {
            Waifu2x_Caffe_Settings_str.append("--model_dir");
            Waifu2x_Caffe_Settings_str.append(model_folder_path + "photo");
            break;
        }
        }
        break;
    }
    }
    // TTA
    if (ui->checkBox_TTA_Waifu2xCaffe->isChecked())
    {
        Waifu2x_Caffe_Settings_str.append("-t");
        Waifu2x_Caffe_Settings_str.append("1");
    }
    else
    {
        Waifu2x_Caffe_Settings_str.append("-t");
        Waifu2x_Caffe_Settings_str.append("0");
    }
    // 显卡设定
    if (ui->checkBox_EnableMultiGPU_Waifu2xCaffe->isChecked() == false)
    {
        // 单个显卡
        // GPU ID
        if (ui->comboBox_ProcessMode_Waifu2xCaffe->currentIndex() > 0)
        {
            Waifu2x_Caffe_Settings_str.append("--gpu");
            Waifu2x_Caffe_Settings_str.append(QString::number(ui->spinBox_GPUID_Waifu2xCaffe->value(), 10));
        }
        // Batch size
        Waifu2x_Caffe_Settings_str.append("-b");
        Waifu2x_Caffe_Settings_str.append(QString::number(ui->spinBox_BatchSize_Waifu2xCaffe->value(), 10));
        // Split size
        Waifu2x_Caffe_Settings_str.append("-c");
        Waifu2x_Caffe_Settings_str.append(QString::number(ui->spinBox_SplitSize_Waifu2xCaffe->value(), 10));
    }
    return Waifu2x_Caffe_Settings_str;
}
/*
Waifu2x_Caffe
读取配置生成配置string
*/
QStringList MainWindow::Waifu2x_Caffe_ReadSettings()
{
    QStringList Waifu2x_Caffe_Settings_str;
    Waifu2x_Caffe_Settings_str.append(Waifu2xCaffe_PreLoad_Settings_Str);
    // 显卡设定
    if (ui->checkBox_EnableMultiGPU_Waifu2xCaffe->isChecked())
    {
        // 多显卡
        Waifu2x_Caffe_Settings_str.append(Waifu2xCaffe_GetGPUInfo());
    }
    return Waifu2x_Caffe_Settings_str;
}
// 判断是否启用了caffe引擎
bool MainWindow::isWaifu2xCaffeEnabled()
{
    if (ui->comboBox_Engine_Image->currentIndex() != 4 && ui->comboBox_Engine_GIF->currentIndex() != 4 && ui->comboBox_Engine_Video->currentIndex() != 4)
    {
        ui->label_ImageStyle_W2xCaffe->setVisible(0);
        ui->comboBox_ImageStyle_Waifu2xCaffe->setVisible(0);
        ui->comboBox_ImageStyle_Waifu2xCaffe->setEnabled(0);
        return false;
    }
    else
    {
        ui->label_ImageStyle_W2xCaffe->setVisible(1);
        ui->comboBox_ImageStyle_Waifu2xCaffe->setVisible(1);
        ui->comboBox_ImageStyle_Waifu2xCaffe->setEnabled(1);
        return true;
    }
}
/*
删除caffe引擎生成的错误日志
*/
void MainWindow::DeleteErrorLog_Waifu2xCaffe()
{
    QStringList FileNameList = file_getFileNames_in_Folder_nofilter(Current_Path); // 读取合法的文件名
    QString Full_Path_File = "";
    if (!FileNameList.isEmpty())
    {
        for (int i = 0; i < FileNameList.size(); i++)
        {
            QString tmp = FileNameList.at(i);
            Full_Path_File = Current_Path + "/" + tmp;
            if (Full_Path_File.contains("error_log_"))
            {
                QFile::remove(Full_Path_File);
            }
        }
    }
}

void MainWindow::on_checkBox_EnableMultiGPU_Waifu2xCaffe_stateChanged(int arg1)
{
    if (ui->checkBox_EnableMultiGPU_Waifu2xCaffe->isChecked())
    {
        ui->lineEdit_MultiGPUInfo_Waifu2xCaffe->setEnabled(1);
        ui->pushButton_VerifyGPUsConfig_Waifu2xCaffe->setEnabled(1);
        //===
        ui->pushButton_SplitSize_Add_Waifu2xCaffe->setEnabled(0);
        ui->pushButton_SplitSize_Minus_Waifu2xCaffe->setEnabled(0);
        ui->spinBox_BatchSize_Waifu2xCaffe->setEnabled(0);
        ui->spinBox_GPUID_Waifu2xCaffe->setEnabled(0);
        ui->spinBox_SplitSize_Waifu2xCaffe->setEnabled(0);
    }
    else
    {
        ui->lineEdit_MultiGPUInfo_Waifu2xCaffe->setEnabled(0);
        ui->pushButton_VerifyGPUsConfig_Waifu2xCaffe->setEnabled(0);
        //===
        ui->pushButton_SplitSize_Add_Waifu2xCaffe->setEnabled(1);
        ui->pushButton_SplitSize_Minus_Waifu2xCaffe->setEnabled(1);
        ui->spinBox_BatchSize_Waifu2xCaffe->setEnabled(1);
        ui->spinBox_GPUID_Waifu2xCaffe->setEnabled(1);
        ui->spinBox_SplitSize_Waifu2xCaffe->setEnabled(1);
    }
}

void MainWindow::on_comboBox_ProcessMode_Waifu2xCaffe_currentIndexChanged(int index)
{
    if (ui->comboBox_ProcessMode_Waifu2xCaffe->currentIndex() == 0)
    {
        ui->checkBox_EnableMultiGPU_Waifu2xCaffe->setEnabled(0);
        ui->checkBox_EnableMultiGPU_Waifu2xCaffe->setChecked(0);
    }
    else
    {
        ui->checkBox_EnableMultiGPU_Waifu2xCaffe->setEnabled(1);
    }
}
// 读取多显卡设定
QStringList MainWindow::Waifu2xCaffe_GetGPUInfo()
{
    GetGPUInfo_QMutex_Waifu2xCaffe.lock();
    //====
    QStringList GPU_List = ui->lineEdit_MultiGPUInfo_Waifu2xCaffe->text().simplified().remove(QRegExp("\\s")).split(":");
    GPU_List.removeDuplicates();
    GPU_List.removeAll("");
    //====
    int MAX_GPU_ID_Waifu2xCaffe = GPU_List.count() - 1;
    if (GPU_ID_Waifu2xCaffe_GetGPUInfo > MAX_GPU_ID_Waifu2xCaffe)
    {
        GPU_ID_Waifu2xCaffe_GetGPUInfo = 0;
    }
    //======
    QStringList GPUInfo;
    QStringList GPUID_BatchSize_SplitSize = GPU_List.at(GPU_ID_Waifu2xCaffe_GetGPUInfo).split(",");
    if (GPUID_BatchSize_SplitSize.count() == 3)
    {
        GPUInfo.append("--gpu");
        GPUInfo.append(GPUID_BatchSize_SplitSize.at(0).trimmed());
        GPUInfo.append("-b");
        GPUInfo.append(GPUID_BatchSize_SplitSize.at(1).trimmed());
        GPUInfo.append("-c");
        GPUInfo.append(GPUID_BatchSize_SplitSize.at(2).trimmed());
    }
    else
    {
        GPUInfo.append("--gpu");
        GPUInfo.append("0");
        GPUInfo.append("-b");
        GPUInfo.append("1");
        GPUInfo.append("-c");
        GPUInfo.append("64");
    }
    //======
    GPU_ID_Waifu2xCaffe_GetGPUInfo++;
    if (GPU_ID_Waifu2xCaffe_GetGPUInfo > MAX_GPU_ID_Waifu2xCaffe)
    {
        GPU_ID_Waifu2xCaffe_GetGPUInfo = 0;
    }
    //======
    GetGPUInfo_QMutex_Waifu2xCaffe.unlock();
    return GPUInfo;
}

void MainWindow::on_lineEdit_MultiGPUInfo_Waifu2xCaffe_editingFinished()
{
    QString Format_str = "0123456789:,";
    QString GPUs_str = ui->lineEdit_MultiGPUInfo_Waifu2xCaffe->text().simplified().remove(QRegExp("\\s"));
    for (int i = 0; i < GPUs_str.length(); i++)
    {
        if (Format_str.contains(GPUs_str.at(i)) == false)
        {
            GPUs_str.remove(GPUs_str.at(i));
        }
    }
    ui->lineEdit_MultiGPUInfo_Waifu2xCaffe->setText(GPUs_str);
}

void MainWindow::on_pushButton_VerifyGPUsConfig_Waifu2xCaffe_clicked()
{
    on_lineEdit_MultiGPUInfo_Waifu2xCaffe_editingFinished();
    QStringList GPU_List = ui->lineEdit_MultiGPUInfo_Waifu2xCaffe->text().simplified().remove(QRegExp("\\s")).split(":");
    GPU_List.removeDuplicates();
    GPU_List.removeAll("");
    //======
    QString VerRes = "";
    //======
    for (int i = 0; i < GPU_List.count(); i++)
    {
        QStringList GPUID_BatchSize_SplitSize = GPU_List.at(i).split(",");
        if (GPUID_BatchSize_SplitSize.count() == 3)
        {
            VerRes.append("GPU [" + QString::number(i, 10) + "]: ID:[" + GPUID_BatchSize_SplitSize.at(0).trimmed() + "]" + " Batch Size:[" + GPUID_BatchSize_SplitSize.at(1).trimmed() + "]" + " Split Size:[" + GPUID_BatchSize_SplitSize.at(2).trimmed() + "]\n\n");
        }
        else
        {
            VerRes.append("Error\n\n");
        }
    }
    //=====
    if (VerRes == "")
    {
        VerRes = "Current configuration is wrong or empty.";
    }
    //======
    QtConcurrent::run(this,&MainWindow::showTextExt,QString("GPUs List according to your configuration"),VerRes);
    //======
    emit Send_TextBrowser_NewMessage("\nWaifu2x-caffe GPUs List(user configuration):\n" + VerRes);
}
