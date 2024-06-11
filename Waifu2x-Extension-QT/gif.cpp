
#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
获取gif帧间隔时间
*/
int MainWindow::Gif_getDuration(QString gifPath)
{
    //========================= 调用ffprobe读取视频信息 ======================
    QProcess *Get_GifAvgFPS_process = new QProcess();
    QString ffprobe_path = Current_Path + "/ffprobe.exe";
    QStringList Get_GifAvgFPS_args = {"-i", gifPath, "-select_streams", "v", "-show_streams", "-v", "quiet", "-print_format", "json", "-show_format"};
    Get_GifAvgFPS_process->start(ffprobe_path, Get_GifAvgFPS_args);
    if(waitForProcess(Get_GifAvgFPS_process,INF_WAIT)==TerminatedByFlag){
        delete Get_GifAvgFPS_process;
        return 0;
    }
    //============= save json =============
    QString ffprobe_output_str = Get_GifAvgFPS_process->readAllStandardOutput();
    delete Get_GifAvgFPS_process;
    // read json
    QJsonDocument json_output = QJsonDocument::fromJson(ffprobe_output_str.toUtf8());
    if(!json_output.isObject()){
        return 0;
    }
    if(!json_output["streams"].isArray()){
        return 0;
    }
    QJsonArray streams = json_output["streams"].toArray();
    if(streams.isEmpty()||!streams.at(0).isObject()) {
        return 0;
    }
    QJsonObject stream = streams.at(0).toObject();
    if(!stream["avg_frame_rate"].isString()) {
        return 0;
    }
    QString FPS_Division = stream["avg_frame_rate"].toString().trimmed();
    //=======================
    int Duration = 0;
    if (FPS_Division != "")
    {
        QStringList FPS_Nums = FPS_Division.split("/");
        if (FPS_Nums.count() == 2)
        {
            double FPS_Num_0 = FPS_Nums.at(0).toDouble();
            double FPS_Num_1 = FPS_Nums.at(1).toDouble();
            if (FPS_Num_0 > 0 && FPS_Num_1 > 0)
            {
                double Duration_double = 100 / (FPS_Num_0 / FPS_Num_1);
                Duration = Duration_double;
                if (Duration_double > Duration)
                {
                    Duration++;
                }
            }
        }
    }
    if (Duration <= 0)
    {
        QMovie movie(gifPath);
        movie.setSpeed(1);
        movie.start();
        movie.stop();
        Duration = ((movie.nextFrameDelay() / 100) + 1) / 10;
    }
    return Duration;
}
/*
获取gif帧数量的位数
*/
int MainWindow::Gif_getFrameDigits(QString gifPath)
{
    QMovie movie(gifPath);
    int FrameCount = 1 + (int)log10(movie.frameCount()); // 获取frame位数
    return FrameCount;
}
/*
拆分gif
*/
void MainWindow::Gif_splitGif(QString gifPath, QString SplitFramesFolderPath)
{
    emit Send_TextBrowser_NewMessage("Start splitting GIF:[" + gifPath + "]");
    int FrameDigits = Gif_getFrameDigits(gifPath);
    // 删除并新建帧文件夹
    file_DelDir(SplitFramesFolderPath);
    file_mkDir(SplitFramesFolderPath);
    // 开始用convert处理
    QString program = Current_Path + "/convert.exe";
    QStringList args = {gifPath, "-coalesce", SplitFramesFolderPath + "/%0" + QString::number(FrameDigits, 10) + "d.png"};
    QProcess *SplitGIF = new QProcess();
    SplitGIF->start(program, args);
    if(waitForProcess(SplitGIF,INF_WAIT)==TerminatedByFlag){
        delete SplitGIF;
        return;
    }
    delete SplitGIF;
    emit Send_TextBrowser_NewMessage("Finish splitting GIF:[" + gifPath + "]");
}
/*
组装gif
*/
void MainWindow::Gif_assembleGif(QString ResGifPath, QString ScaledFramesPath, int Duration, bool CustRes_isEnabled, int CustRes_height, int CustRes_width)
{
    emit Send_TextBrowser_NewMessage("Start to assemble GIF:[" + ResGifPath + "]");
    //===============================
    QStringList resize_cmd;
    if (CustRes_isEnabled)
    {
        if (CustRes_AspectRatioMode == Qt::IgnoreAspectRatio)
        {
            resize_cmd.append("-resize");
            resize_cmd.append(QString::number(CustRes_width, 10) + "x" + QString::number(CustRes_height, 10) + "!");
        }
        if (CustRes_AspectRatioMode == Qt::KeepAspectRatio)
        {
            resize_cmd.append("-resize");
            resize_cmd.append(QString::number(CustRes_width, 10) + "x" + QString::number(CustRes_height, 10));
        }
        if (CustRes_AspectRatioMode == Qt::KeepAspectRatioByExpanding)
        {
            if (CustRes_width > CustRes_height)
            {
                resize_cmd.append("-resize");
                resize_cmd.append(QString::number(CustRes_width, 10));
            }
            else
            {
                resize_cmd.append("-resize");
                resize_cmd.append("x" + QString::number(CustRes_height, 10));
            }
        }
    }
    QString program = Current_Path + "/convert.exe";
    QStringList args_append = {"-delay", QString::number(Duration, 10), "-loop", "0"};
    QStringList args(resize_cmd);
    args.append(args_append);
    args.append(ScaledFramesPath + "/*png");
    args.append(ResGifPath);
    QProcess *AssembleGIF = new QProcess();
    AssembleGIF->start(program, args);
    if(waitForProcess(AssembleGIF,INF_WAIT)==TerminatedByFlag){
        delete AssembleGIF;
        return;
    }
    delete AssembleGIF;
    //======= 纠正文件名称错误(当 结果gif文件路径内有 % 符号时) ======
    if (QFile::exists(ResGifPath) == false)
    {
        QFileInfo fileinfo(ResGifPath);
        QString file_name = file_getBaseName(ResGifPath);
        QString file_ext = fileinfo.suffix();
        QString file_path = file_getFolderPath(fileinfo);
        QString ActualResGifPath = file_path + "/" + file_name + "-0." + file_ext;
        if (QFile::exists(ActualResGifPath) == true)
        {
            QFile::rename(ActualResGifPath, ResGifPath);
        }
    }
    //===========
    emit Send_TextBrowser_NewMessage("Finish assembling GIF:[" + ResGifPath + "]");
}
/*
压缩gif
*/
QString MainWindow::Gif_compressGif(QString gifPath, QString gifPath_compressd)
{
    emit Send_TextBrowser_NewMessage("Starting to optimize GIF:[" + gifPath + "]");
    //=====
    QString program = Current_Path + "/gifsicle.exe";
    QStringList args = {"-O3", "-i", gifPath, "-o", gifPath_compressd};
    QProcess *CompressGIF = new QProcess();
    CompressGIF->start(program, args);
    if(waitForProcess(CompressGIF,INF_WAIT)==TerminatedByFlag){
        delete CompressGIF;
        return QString();
    }
    delete CompressGIF;
    //======
    // 判断是否生成压缩后的gif
    if (QFile::exists(gifPath_compressd) == false)
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + gifPath + "]. Error: [Can't optimize gif.]");
        return gifPath; // 返回源文件路径
    }
    //======
    // 比较文件大小,判断压缩是否有效
    QFileInfo gifPath_QFileInfo(gifPath);
    QFileInfo gifPath_compressd_QFileInfo(gifPath_compressd);
    if ((gifPath_compressd_QFileInfo.size() < gifPath_QFileInfo.size()) == false)
    {
        emit Send_TextBrowser_NewMessage("Failed to optimize gif [" + gifPath + "] to reduce storage usage, the optimized gif file will be deleted.");
        QFile::remove(gifPath_compressd);
        return gifPath; // 返回源文件路径
    }
    //======
    QFile::remove(gifPath);
    emit Send_TextBrowser_NewMessage("Finish optimizing GIF:[" + gifPath + "]");
    return gifPath_compressd; // 返回处理完成的文件路径
}
