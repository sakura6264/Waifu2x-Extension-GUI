#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
判断视频是否是可变帧率的
*/
bool MainWindow::video_isVFR(QString videoPath)
{
    //========================= 调用ffprobe读取视频信息 ======================
    QProcess *Get_VideoFPS_process = new QProcess();
    QString program = Current_Path + "/ffprobe.exe";
    QStringList args = {"-i", videoPath, "-select_streams", "v", "-show_streams", "-v", "quiet", "-print_format", "json", "-show_format"};
    Get_VideoFPS_process->start(program, args);
    if(waitForProcess(Get_VideoFPS_process,INF_WAIT)==TerminatedByFlag){
        delete Get_VideoFPS_process;
        return false;
    }
    //============= save json =============
    QString ffprobe_output_str = Get_VideoFPS_process->readAllStandardOutput();
    delete Get_VideoFPS_process;
    // read json
    QJsonDocument json_output = QJsonDocument::fromJson(ffprobe_output_str.toUtf8());
    if(!json_output.isObject()){
        return false;
    }
    if(!json_output["streams"].isArray()){
        return false;
    }
    QJsonArray streams = json_output["streams"].toArray();
    if(streams.isEmpty()||!streams.at(0).isObject()) {
        return false;
    }
    QJsonObject stream = streams.at(0).toObject();
    if(!stream["avg_frame_rate"].isString() || !stream["r_frame_rate"].isString()) {
        return false;
    }
    QString avg_frame_rate = stream["avg_frame_rate"].toString().trimmed();
    QString r_frame_rate = stream["r_frame_rate"].toString().trimmed();
    if (avg_frame_rate != "" && r_frame_rate != "")
    {
        return (avg_frame_rate != r_frame_rate);
    }
    else
    {
        return false;
    }
}
/*
根据分辨率判断是否跳过
true = 跳过
*/
bool MainWindow::Video_AutoSkip_CustRes(int rowNum)
{
    if (ui->checkBox_AutoSkip_CustomRes->isChecked() == false)
        return false;
    QString SourceFile_fullPath = Table_model_video->item(rowNum, 2)->text();
    if (CustRes_isContained(SourceFile_fullPath))
    {
        int CustRes_height = 0;
        int CustRes_width = 0;
        QMap<QString, QString> Res_map = CustRes_getResMap(SourceFile_fullPath); // res_map["fullpath"],["height"],["width"]
        CustRes_height = Res_map["height"].toInt();
        CustRes_width = Res_map["width"].toInt();
        //=========================
        QMap<QString, int> res_map = video_get_Resolution(SourceFile_fullPath);
        int original_height = res_map["height"];
        int original_width = res_map["width"];
        if (original_height <= 0 || original_width <= 0) // 判断是否读取失败
        {
            return false;
        }
        //==========================
        if ((CustRes_height * CustRes_width) <= (original_height * original_width))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
/*
直接读取视频 分辨率 然后用 自有算法 计算其应该有的比特率
单位为k
*/
int MainWindow::video_UseRes2CalculateBitrate(QString VideoFileFullPath)
{
    QMap<QString, int> res_map = video_get_Resolution(VideoFileFullPath);
    int original_height = res_map["height"];
    int original_width = res_map["width"];
    if (original_height <= 0 || original_width <= 0)
    {
        return 0;
    }
    if (original_height <= original_width)
    {
        return original_height * 6;
    }
    else
    {
        return original_width * 6;
    }
}

/*
直接获取视频的分辨率
*/
QMap<QString, int> MainWindow::video_get_Resolution(QString VideoFileFullPath)
{
    QMap<QString, int> empty;
    empty["height"] = 0;
    empty["width"] = 0;
    emit Send_TextBrowser_NewMessage("Get resolution of the video:[" + VideoFileFullPath + "]");
    //========================= 调用ffprobe读取视频信息 ======================
    QProcess *Get_resolution_process = new QProcess();
    QString program_ffprobe = Current_Path + "/ffprobe.exe";
    QStringList args_ffprobe = {"-i", VideoFileFullPath, "-select_streams", "v", "-show_streams", "-v", "quiet", "-print_format", "json", "-show_format"};
    Get_resolution_process->start(program_ffprobe, args_ffprobe);
    if(waitForProcess(Get_resolution_process,INF_WAIT)==TerminatedByFlag){
        delete Get_resolution_process;
        return empty;
    }
    //============= save json =============
    QString ffprobe_output_str = Get_resolution_process->readAllStandardOutput();
    delete Get_resolution_process;
    // read json
    QJsonDocument json_output = QJsonDocument::fromJson(ffprobe_output_str.toUtf8());
    if(!json_output.isObject()){
        return empty;
    }
    if(!json_output["streams"].isArray()){
        return empty;
    }
    QJsonArray streams = json_output["streams"].toArray();
    if(streams.isEmpty()||!streams.at(0).isObject()) {
        return empty;
    }
    QJsonObject stream = streams.at(0).toObject();
    if(!stream["width"].isString() || !stream["height"].isString()) {
        return empty;
    }
    QString width_str = stream["width"].toString().trimmed();
    QString height_str = stream["height"].toString().trimmed();
    //=======================
    if (width_str != "" && height_str != "")
    {
        int width_int = width_str.toInt();
        int height_int = height_str.toInt();
        if (width_int > 0 && height_int > 0)
        {
            QMap<QString, int> res_map;
            res_map["height"] = height_int;
            res_map["width"] = width_int;
            return res_map;
        }
    }
    //========================================================================================
    QString FrameImageFullPath = "";
    FrameImageFullPath = file_getBaseName(VideoFileFullPath) + "_GetVideoRes_W2xEX.jpg";
    QFile::remove(FrameImageFullPath);
    QString program_ffmpeg = Current_Path + "/ffmpeg.exe";
    QStringList args_ffmpeg = {"-ss", "0", "-i", VideoFileFullPath, "-vframes", "1", FrameImageFullPath};
    QProcess *vid = new QProcess();
    vid->start(program_ffmpeg, args_ffmpeg);
    if(waitForProcess(vid,INF_WAIT)==TerminatedByFlag){
        delete vid;
        return empty;
    }
    delete vid;
    QImage qimage_original;
    qimage_original.load(FrameImageFullPath);
    int original_height = qimage_original.height();
    int original_width = qimage_original.width();
    QFile::remove(FrameImageFullPath);
    if (original_height <= 0 || original_width <= 0)
    {
        emit Send_TextBrowser_NewMessage("ERROR! Unable to read the resolution of the video. [" + VideoFileFullPath + "]");
        return empty;
    }
    //==============================
    QMap<QString, int> res_map;
    // 读取文件信息
    res_map["height"] = original_height;
    res_map["width"] = original_width;
    return res_map;
}

/*
根据视频时长,判断是否需要分段处理
*/
bool MainWindow::video_isNeedProcessBySegment(int rowNum)
{
    if (ui->checkBox_ProcessVideoBySegment->isChecked() == false)
        return false; // 如果没启用分段处理,直接返回false
    QString VideoFile = Table_model_video->item(rowNum, 2)->text();
    if (video_get_duration(VideoFile) > ui->spinBox_SegmentDuration->value())
    {
        return true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("This video is too short, so segment processing is automatically disabled.[" + VideoFile + "]");
        return false;
    }
}

/*
生成视频片段文件夹编号
*/
QString MainWindow::video_getClipsFolderNo()
{
    QString current_date = QDateTime::currentDateTime().toString("yyMMddhhmmss");
    return current_date;
}
/*
组装视频(从mp4片段组装)
*/
void MainWindow::video_AssembleVideoClips(QString VideoClipsFolderPath, QString VideoClipsFolderName, QString video_mp4_scaled_fullpath, QString AudioPath)
{
    emit Send_TextBrowser_NewMessage("Start assembling video with clips:[" + video_mp4_scaled_fullpath + "]");
    //=================
    QStringList encoder_audio_cmd;
    QStringList bitrate_audio_cmd;
    //=======
    if (ui->groupBox_video_settings->isChecked())
    {
        if (ui->lineEdit_encoder_audio->text() != "")
        {
            encoder_audio_cmd.append("-c:a");
            encoder_audio_cmd.append(ui->lineEdit_encoder_audio->text());
        }

        if (ui->spinBox_bitrate_audio->value() > 0)
        {
            bitrate_audio_cmd.append("-b:a");
            bitrate_audio_cmd.append(QString::number(ui->spinBox_bitrate_audio->value(), 10) + "k");
        }
    }
    //==============================
    QStringList VideoClips_Scan_list = file_getFileNames_in_Folder_nofilter(VideoClipsFolderPath);
    int VideoClipsNum = VideoClips_Scan_list.count();
    QStringList VideoClips_fileName_list;
    VideoClips_fileName_list.clear();
    QFileInfo vfinfo(video_mp4_scaled_fullpath);
    QString video_dir = file_getFolderPath(video_mp4_scaled_fullpath);
    /*
    生成视频片段文件完整路径QStringList
    */
    for (int VideoNameNo = 1; VideoNameNo <= VideoClipsNum; VideoNameNo++)
    {
        QString VideoClip_FullPath_tmp = VideoClipsFolderPath + "/" + QString::number(VideoNameNo, 10) + ".mp4";
        if (QFile::exists(VideoClip_FullPath_tmp))
        {
            VideoClips_fileName_list.append(VideoClip_FullPath_tmp);
        }
    }
    // 获取一个有效的mp4片段文件完整路径
    QString Mp4Clip_forReadInfo = VideoClips_fileName_list.at(0);
    /*
    生成文件列表QString
    */
    QString FFMpegFileList_QString = "";
    for (int CurrentVideoClipNo = 0; CurrentVideoClipNo < VideoClips_fileName_list.size(); CurrentVideoClipNo++)
    {
        QString VideoClip_fullPath = VideoClips_fileName_list.at(CurrentVideoClipNo);
        QString Line = "file \'" + VideoClip_fullPath + "\'\n";
        FFMpegFileList_QString.append(Line);
    }
    //================ 将文件列表写入文件保存 ================
    QFileInfo videoFileInfo(video_mp4_scaled_fullpath);
    QString Path_FFMpegFileList = "";
    int FileNo = 0;
    do
    {
        FileNo++;
        Path_FFMpegFileList = video_dir + "/" + file_getBaseName(video_mp4_scaled_fullpath) + "_fileList_" + QString::number(FileNo, 10) + ".txt";
    } while (QFile::exists(Path_FFMpegFileList));
    //=========
    QFile FFMpegFileList(Path_FFMpegFileList);
    FFMpegFileList.remove();
    if (FFMpegFileList.open(QIODevice::ReadWrite | QIODevice::Text)) // QIODevice::ReadWrite支持读写
    {
        QTextStream stream(&FFMpegFileList);
        stream << FFMpegFileList_QString;
    }
    FFMpegFileList.close();
    //========
    /*
    组装视频
    */
    QString ffmpeg_path = Current_Path + "/ffmpeg.exe";
    bool Del_DenoisedAudio = false;
    //=============== 音频降噪 ========================
    if ((ui->checkBox_AudioDenoise->isChecked()) && QFile::exists(AudioPath))
    {
        QString AudioPath_tmp = video_AudioDenoise(AudioPath);
        if (AudioPath_tmp != AudioPath)
        {
            AudioPath = AudioPath_tmp;
            Del_DenoisedAudio = true;
        }
    }
    //================= 获取比特率 =================
    QStringList bitrate_video_cmd;
    if (ui->spinBox_bitrate_vid->value() > 0 && ui->groupBox_video_settings->isChecked())
    {
        bitrate_video_cmd.append("-b:v");
        bitrate_video_cmd.append(QString::number(ui->spinBox_bitrate_vid->value(), 10) + "k");
    }
    else
    {
        int BitRate = video_UseRes2CalculateBitrate(Mp4Clip_forReadInfo);
        if (BitRate != 0)
        {
            bitrate_video_cmd.append("-b:v");
            bitrate_video_cmd.append(QString::number(BitRate, 10) + "k");
        }
    }
    //================= 读取视频编码器设定 ==============
    QStringList encoder_video_cmd;
    if (ui->groupBox_video_settings->isChecked() && ui->lineEdit_encoder_vid->text() != "")
    {
        encoder_video_cmd.append("-c:v");
        encoder_video_cmd.append(ui->lineEdit_encoder_vid->text()); // 图像编码器
    }
    //================ 获取fps =====================
    QStringList fps_video_cmd;
    QString fps = video_get_fps(Mp4Clip_forReadInfo).trimmed();
    if (fps != "0.0")
    {
        fps_video_cmd.append("-r");
        fps_video_cmd.append(fps);
    }
    //================= 开始处理 =============================
    QStringList ARGS = {"-y", "-f", "concat", "-safe", "0"};
    ARGS.append(fps_video_cmd);
    ARGS.append("-i");
    ARGS.append(Path_FFMpegFileList);
    if (QFile::exists(AudioPath))
    {
        ARGS.append(fps_video_cmd);
        ARGS.append("-i");
        ARGS.append(AudioPath);
        ARGS.append(fps_video_cmd);
        ARGS.append(bitrate_video_cmd);
        ARGS.append(encoder_video_cmd);
        ARGS.append(fps_video_cmd);
        ARGS.append(encoder_audio_cmd);
        ARGS.append(bitrate_audio_cmd);
    }
    else
    {
        ARGS.append(bitrate_video_cmd);
        ARGS.append(encoder_video_cmd);
        ARGS.append(fps_video_cmd);
    }
    ARGS.append(video_mp4_scaled_fullpath);
    QProcess *AssembleVideo = new QProcess();
    AssembleVideo->start(ffmpeg_path, ARGS);
    if(waitForProcess(AssembleVideo,INF_WAIT)==TerminatedByFlag){
        delete AssembleVideo;
        return;
    }
    // 检查是否发生错误
    if (!QFile::exists(video_mp4_scaled_fullpath)) // 检查是否成功生成视频
    {
        MultiLine_ErrorOutput_QMutex.lock();
        emit Send_TextBrowser_NewMessage("Error output for FFmpeg when processing:[" + video_mp4_scaled_fullpath + "]");
        emit Send_TextBrowser_NewMessage("\n--------------------------------------");
        // 标准输出
        emit Send_TextBrowser_NewMessage(AssembleVideo->readAllStandardOutput());
        // 错误输出
        emit Send_TextBrowser_NewMessage(AssembleVideo->readAllStandardError());
        emit Send_TextBrowser_NewMessage("\n--------------------------------------");
        MultiLine_ErrorOutput_QMutex.unlock();
    }
    delete AssembleVideo;
    QFile::remove(Path_FFMpegFileList); // 删除文件列表
    //===================
    if (Del_DenoisedAudio)
        QFile::remove(AudioPath);
    //==============================
    emit Send_TextBrowser_NewMessage("Finish assembling video with clips:[" + video_mp4_scaled_fullpath + "]");
}
/*
将视频拆分到帧(分段的)
*/
void MainWindow::video_video2images_ProcessBySegment(QString VideoPath, QString FrameFolderPath, int StartTime, int SegmentDuration)
{
    emit Send_TextBrowser_NewMessage("Start splitting video: [" + VideoPath + "]");
    //=================
    QString ffmpeg_path = Current_Path + "/ffmpeg.exe";
    QString video_mp4_fullpath = VideoPath;
    //================ 获取fps =====================
    QStringList fps_video_cmd;
    QString fps = video_get_fps(video_mp4_fullpath).trimmed();
    if (fps != "0.0")
    {
        fps_video_cmd.append("-r");
        fps_video_cmd.append(fps);
    }
    //=====================
    int FrameNumDigits = video_get_frameNumDigits(video_mp4_fullpath);
    if (FrameNumDigits == 0)
        return;
    //=====================
    QProcess *video_splitFrame = new QProcess();
    QStringList ARGS;
    ARGS.append("-y");
    ARGS.append(fps_video_cmd);
    ARGS.append("-i");
    ARGS.append(video_mp4_fullpath);
    ARGS.append("-ss");
    ARGS.append(QString::number(StartTime, 10));
    ARGS.append("-t");
    ARGS.append(QString::number(SegmentDuration, 10));
    ARGS.append(fps_video_cmd);
    ARGS.append(FrameFolderPath.replace("%", "%%") + "/%0" + QString::number(FrameNumDigits, 10) + "d.png");
    video_splitFrame->start(ffmpeg_path, ARGS);
    if(waitForProcess(video_splitFrame,INF_WAIT)==TerminatedByFlag){
        delete video_splitFrame;
        return;
    }
    delete video_splitFrame;
    //====================================
    emit Send_TextBrowser_NewMessage("Finish splitting video: [" + VideoPath + "]");
}

/*
提取视频的音频
*/
void MainWindow::video_get_audio(QString VideoPath, QString AudioPath)
{
    emit Send_TextBrowser_NewMessage("Extract audio from video: [" + VideoPath + "]");
    //==============================================
    QString ffmpeg_path = Current_Path + "/ffmpeg.exe";
    QFile::remove(AudioPath);
    QProcess *video_splitSound = new QProcess();
    QStringList args = {"-y", "-i", VideoPath, AudioPath};
    video_splitSound->start(ffmpeg_path, args);
    if(waitForProcess(video_splitSound,INF_WAIT)==TerminatedByFlag){
        delete video_splitSound;
        return;
    }
    delete video_splitSound;
    if (QFile::exists(AudioPath))
    {
        emit Send_TextBrowser_NewMessage("Successfully extracted audio from video: [" + VideoPath + "]");
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Failed to extract audio from video: [" + VideoPath + "] This video might be a silent video, so will continue to process this video.");
    }
}
/*
将视频转换为mp4
*/
QString MainWindow::video_To_CFRMp4(QString VideoPath)
{
    QFileInfo vfinfo(VideoPath);
    QString video_ext = vfinfo.suffix();
    bool isVFR = false;
    //==============
    if (video_ext == "mp4")
    {
        if (video_isVFR(VideoPath) == false)
        {
            return VideoPath;
        }
        else
        {
            isVFR = true;
        }
    }
    QString video_dir = file_getFolderPath(vfinfo);
    QString video_filename = file_getBaseName(VideoPath);
    QString video_mp4_fullpath = video_dir + "/" + video_filename + "_" + video_ext + ".mp4";
    if (QFile::exists(video_mp4_fullpath))
        return video_mp4_fullpath;
    //=================
    emit Send_TextBrowser_NewMessage("Start converting video: [" + VideoPath + "] to mp4");
    QString ffmpeg_path = Current_Path + "/ffmpeg.exe";
    QFile::remove(video_mp4_fullpath);
    QStringList vcodec_copy_cmd;
    QStringList acodec_copy_cmd;
    QStringList bitrate_vid_cmd;
    QStringList bitrate_audio_cmd;
    QStringList Extra_command;
    QStringList bitrate_OverAll;
    if (ui->groupBox_video_settings->isChecked())
    {
        Extra_command.clear();
        Extra_command.append(commandResolve(ui->lineEdit_ExCommand_2mp4->text()));
        if (ui->checkBox_vcodec_copy_2mp4->isChecked() && isVFR == false)
        {
            vcodec_copy_cmd.append("-vcodec");
            vcodec_copy_cmd.append("copy");
        }
        else
        {
            if (ui->spinBox_bitrate_vid_2mp4->value() > 0 && ui->spinBox_bitrate_audio_2mp4->value() > 0)
            {
                bitrate_vid_cmd.append("-b:v");
                bitrate_vid_cmd.append(QString::number(ui->spinBox_bitrate_vid_2mp4->value(), 10) + "k");
            }
        }
        if (ui->checkBox_acodec_copy_2mp4->isChecked() && isVFR == false)
        {
            acodec_copy_cmd.append("-acodec");
            acodec_copy_cmd.append("copy");
        }
        else
        {
            if (ui->spinBox_bitrate_vid_2mp4->value() > 0 && ui->spinBox_bitrate_audio_2mp4->value() > 0)
            {
                bitrate_audio_cmd.append("-b:a");
                bitrate_audio_cmd.append(QString::number(ui->spinBox_bitrate_audio_2mp4->value(), 10) + "k");
            }
        }
    }
    if ((ui->groupBox_video_settings->isChecked() == false) || (ui->spinBox_bitrate_vid_2mp4->value() <= 0 || ui->spinBox_bitrate_audio_2mp4->value() <= 0))
    {
        QString BitRate = video_get_bitrate(VideoPath);
        if (BitRate != "")
        {
            bitrate_OverAll.append("-b");
            bitrate_OverAll.append(BitRate);
        }
    }
    //=====
    QStringList args = {"-y", "-i", VideoPath, "-vsync", "1"};
    args.append(vcodec_copy_cmd);
    args.append(acodec_copy_cmd);
    args.append(bitrate_vid_cmd);
    args.append(bitrate_audio_cmd);
    args.append(bitrate_OverAll);
    args.append(Extra_command);
    args.append(video_mp4_fullpath);
    QProcess *video_tomp4 = new QProcess();
    video_tomp4->start(ffmpeg_path, args);
    if(waitForProcess(video_tomp4,INF_WAIT)==TerminatedByFlag){
        delete video_tomp4;
        return "null";
    }
    delete video_tomp4;
    //======
    if (QFile::exists(video_mp4_fullpath))
    {
        emit Send_TextBrowser_NewMessage("Successfully converted video: [" + VideoPath + "] to mp4");
        return video_mp4_fullpath;
    }
    return "null";
}
//===============
// 获取时长(秒)
//===============
int MainWindow::video_get_duration(QString videoPath)
{
    emit Send_TextBrowser_NewMessage("Get duration of the video:[" + videoPath + "]");
    //========================= 调用ffprobe读取视频信息 ======================
    QString ffprobe_path = Current_Path + "/ffprobe.exe";
    QStringList args = {"-i", videoPath, "-select_streams", "v", "-show_streams", "-v", "quiet", "-print_format", "json", "-show_format"};
    QProcess *Get_Duration_process = new QProcess();
    Get_Duration_process->start(ffprobe_path, args);
    if(waitForProcess(Get_Duration_process,INF_WAIT)==TerminatedByFlag){
        delete Get_Duration_process;
        return 0;
    }
    //============= save json =============
    QString ffprobe_output_str = Get_Duration_process->readAllStandardOutput();
    delete Get_Duration_process;
    // read json
    QJsonDocument json_output = QJsonDocument::fromJson(ffprobe_output_str.toUtf8());
    if(!json_output.isObject()){
        emit Send_TextBrowser_NewMessage("ERROR! Unable to get the duration of the [" + videoPath + "].");
        return 0;
    }
    if(!json_output["format"].isObject()){
        emit Send_TextBrowser_NewMessage("ERROR! Unable to get the duration of the [" + videoPath + "].");
        return 0;
    }
    QJsonObject format = json_output["format"].toObject();
    if(!format["duration"].isString()) {
        emit Send_TextBrowser_NewMessage("ERROR! Unable to get the duration of the [" + videoPath + "].");
        return 0;
    }
    QString Duration = format["duration"].toString().trimmed();
    //=======================
    if (Duration == "")
    {
        emit Send_TextBrowser_NewMessage("ERROR! Unable to get the duration of the [" + videoPath + "].");
        return 0;
    }
    //=======================
    double Duration_double = Duration.toDouble();
    int Duration_int = (int)Duration_double;
    //=====================
    return Duration_int;
}
/*
音频降噪
*/
QString MainWindow::video_AudioDenoise(QString OriginalAudioPath)
{
    /*
    sox 输入音频.wav -n noiseprof 噪音分析.prof
    sox 输入音频.wav 输出音频.wav noisered 噪音分析.prof 0.21
    */
    emit Send_TextBrowser_NewMessage("Starting to denoise audio.[" + OriginalAudioPath + "]");
    //===========
    QFileInfo fileinfo(OriginalAudioPath);
    QString file_name = file_getBaseName(OriginalAudioPath);
    QString file_ext = fileinfo.suffix();
    QString file_path = file_getFolderPath(fileinfo);
    //================
    QString program = Current_Path + "/SoX/sox.exe";
    QString DenoiseProfile = file_path + "/" + file_name + "_DenoiseProfile.dp";
    QString DenoisedAudio = file_path + "/" + file_name + "_Denoised." + file_ext;
    double DenoiseLevel = ui->doubleSpinBox_AudioDenoiseLevel->value();
    //================
    QStringList args_sub0 = {OriginalAudioPath, "-n", "noiseprof", DenoiseProfile};
    QProcess *vid_sub0 = new QProcess();
    vid_sub0->start(program, args_sub0);
    if(waitForProcess(vid_sub0,INF_WAIT)==TerminatedByFlag){
        delete vid_sub0;
        return QString();
    }
    delete vid_sub0;
    //================
    QStringList args_sub1 = {OriginalAudioPath, DenoisedAudio, "noisered", DenoiseProfile, QString::number(DenoiseLevel)};
    QProcess *vid_sub1 = new QProcess();
    vid_sub1->start(program, args_sub1);
    if(waitForProcess(vid_sub1,INF_WAIT)==TerminatedByFlag){
        delete vid_sub1;
        return QString();
    }
    //================
    if (QFile::exists(DenoisedAudio))
    {
        emit Send_TextBrowser_NewMessage("Successfully denoise audio.[" + OriginalAudioPath + "]");
        QFile::remove(DenoiseProfile);
        return DenoisedAudio;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Error! Unable to denoise audio.[" + OriginalAudioPath + "]");
        return OriginalAudioPath;
    }
}
/*
保存进度
*/
void MainWindow::video_write_Progress_ProcessBySegment(QString VideoConfiguration_fullPath, int StartTime, bool isSplitComplete, bool isScaleComplete, int OLDSegmentDuration)
{
    QSettings *configIniWrite = new QSettings(VideoConfiguration_fullPath, QSettings::IniFormat);
    configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));
    //==================== 存储进度 ==================================
    configIniWrite->setValue("/Progress/StartTime", StartTime);
    configIniWrite->setValue("/Progress/isSplitComplete", isSplitComplete);
    configIniWrite->setValue("/Progress/isScaleComplete", isScaleComplete);
    configIniWrite->setValue("/Progress/OLDSegmentDuration", OLDSegmentDuration);
    delete configIniWrite;
}
/*
保存视频配置
*/
void MainWindow::video_write_VideoConfiguration(QString VideoConfiguration_fullPath, int ScaleRatio, int DenoiseLevel, bool CustRes_isEnabled, int CustRes_height, int CustRes_width, QString EngineName, bool isProcessBySegment, QString VideoClipsFolderPath, QString VideoClipsFolderName)
{
    QSettings *configIniWrite = new QSettings(VideoConfiguration_fullPath, QSettings::IniFormat);
    configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));
    //================= 添加警告 =========================
    configIniWrite->setValue("/Warning/Edit", "Carefully!!!");
    //==================== 存储视频信息 ==================================
    configIniWrite->setValue("/VideoConfiguration/ScaleRatio", ScaleRatio);
    configIniWrite->setValue("/VideoConfiguration/DenoiseLevel", DenoiseLevel);
    configIniWrite->setValue("/VideoConfiguration/CustRes_isEnabled", CustRes_isEnabled);
    configIniWrite->setValue("/VideoConfiguration/CustRes_height", CustRes_height);
    configIniWrite->setValue("/VideoConfiguration/CustRes_width", CustRes_width);
    configIniWrite->setValue("/VideoConfiguration/EngineName", EngineName);
    configIniWrite->setValue("/VideoConfiguration/isProcessBySegment", isProcessBySegment);
    configIniWrite->setValue("/VideoConfiguration/VideoClipsFolderPath", VideoClipsFolderPath);
    configIniWrite->setValue("/VideoConfiguration/VideoClipsFolderName", VideoClipsFolderName);
    //==================== 存储进度 ==================================
    configIniWrite->setValue("/Progress/StartTime", 0);
    configIniWrite->setValue("/Progress/isSplitComplete", false);
    configIniWrite->setValue("/Progress/isScaleComplete", false);
    delete configIniWrite;
}

QString MainWindow::video_get_bitrate_AccordingToRes_FrameFolder(QString ScaledFrameFolderPath)
{
    QStringList flist = file_getFileNames_in_Folder_nofilter(ScaledFrameFolderPath);
    QString Full_Path_File = "";
    if (!flist.isEmpty())
    {
        for (int i = 0; i < flist.size(); i++)
        {
            QString tmp = flist.at(i);
            Full_Path_File = ScaledFrameFolderPath + "/" + tmp;
            QFileInfo finfo(Full_Path_File);
            if (finfo.suffix() == "png")
                break;
        }
    }
    QImage qimage_original;
    qimage_original.load(Full_Path_File);
    int original_height = qimage_original.height();
    int original_width = qimage_original.width();
    if (original_height <= 0 || original_width <= 0)
    {
        return "";
    }
    if (original_height <= original_width)
    {
        return QString::number(original_height * 6, 10);
    }
    else
    {
        return QString::number(original_width * 6, 10);
    }
}
/*
获取视频比特率
*/
QString MainWindow::video_get_bitrate(QString videoPath)
{
    emit Send_TextBrowser_NewMessage("Get bitrate of the video:[" + videoPath + "]");
    //========================= 调用ffprobe读取视频信息 ======================
    QProcess *Get_Bitrate_process = new QProcess();
    QString ffprobe_path = Current_Path + "/ffprobe.exe";
    QStringList args = {"-i", videoPath, "-select_streams", "v", "-show_streams", "-v", "quiet", "-print_format", "json", "-show_format"};
    Get_Bitrate_process->start(ffprobe_path, args);
    if(waitForProcess(Get_Bitrate_process,INF_WAIT)==TerminatedByFlag){
        delete Get_Bitrate_process;
        return QString();
    }
    //============= save json =============
    QString ffprobe_output_str = Get_Bitrate_process->readAllStandardOutput();
    delete Get_Bitrate_process;
    // read json
    QJsonDocument json_output = QJsonDocument::fromJson(ffprobe_output_str.toUtf8());
    if(!json_output.isObject()){
        emit Send_TextBrowser_NewMessage("Warning! Unable to get the bitrate of the [" + videoPath + "]. The bit rate automatically allocated by ffmpeg will be used.");
        return QString();
    }
    if(!json_output["format"].isObject()){
        emit Send_TextBrowser_NewMessage("Warning! Unable to get the bitrate of the [" + videoPath + "]. The bit rate automatically allocated by ffmpeg will be used.");
        return QString();
    }
    QJsonObject format =json_output["format"].toObject();
    if(!format["bit_rate"].isString()){
        emit Send_TextBrowser_NewMessage("Warning! Unable to get the bitrate of the [" + videoPath + "]. The bit rate automatically allocated by ffmpeg will be used.");
        return QString();
    }
    QString BitRate = format["bit_rate"].toString().trimmed();
    //=======================
    if (BitRate == "")
        emit Send_TextBrowser_NewMessage("Warning! Unable to get the bitrate of the [" + videoPath + "]. The bit rate automatically allocated by ffmpeg will be used.");
    return BitRate;
}
/*
获取视频FPS
*/
QString MainWindow::video_get_fps(QString videoPath)
{
    //========================= 调用ffprobe读取视频信息 ======================
    QProcess *Get_VideoFPS_process = new QProcess();
    QString ffprobe_path = Current_Path + "/ffprobe.exe";
    QStringList args = {"-i", videoPath, "-select_streams", "v", "-show_streams", "-v", "quiet", "-print_format", "json", "-show_format"};
    Get_VideoFPS_process->start(ffprobe_path, args);
    if(waitForProcess(Get_VideoFPS_process,INF_WAIT)==TerminatedByFlag){
        delete Get_VideoFPS_process;
        return "0.0";
    }
    //============= save json =============
    QString ffprobe_output_str = Get_VideoFPS_process->readAllStandardOutput();
    delete Get_VideoFPS_process;
    // read json
    QJsonDocument json_output = QJsonDocument::fromJson(ffprobe_output_str.toUtf8());
    if(!json_output.isObject()){
        return "0.0";
    }
    if(!json_output["streams"].isArray()){
        return "0.0";
    }
    QJsonArray streams = json_output["streams"].toArray();
    if(streams.isEmpty()||!streams.at(0).isObject()) {
        return "0.0";
    }
    QJsonObject stream = streams.at(0).toObject();
    if(!stream["avg_frame_rate"].isString()) {
        return "0.0";
    }
    QString FPS_Division = stream["avg_frame_rate"].toString().trimmed();
    //=======================
    if (FPS_Division == "")
    {
        return "0.0";
    }
    //=======================
    QStringList FPS_Nums = FPS_Division.split("/");
    if (FPS_Nums.count() != 2)
    {
        return "0.0";
    }
    double FPS_Num_0 = FPS_Nums.at(0).toDouble();
    double FPS_Num_1 = FPS_Nums.at(1).toDouble();
    if (FPS_Num_0 <= 0 || FPS_Num_1 <= 0)
    {
        return "0.0";
    }
    return FPS_Division;
}

int MainWindow::video_get_frameNumDigits(QString videoPath)
{
    //========================= 调用ffprobe读取视频信息 ======================
    QProcess *Get_VideoFrameNumDigits_process = new QProcess();
    QString ffprobe_path = Current_Path + "/ffprobe.exe";
    QStringList args = {"-i", videoPath, "-select_streams", "v", "-show_streams", "-v", "quiet", "-print_format", "json", "-show_format"};
    Get_VideoFrameNumDigits_process->start(ffprobe_path, args);
    if(waitForProcess(Get_VideoFrameNumDigits_process,INF_WAIT)==TerminatedByFlag){
        delete Get_VideoFrameNumDigits_process;
        return 0;
    }
    //============= save json =============
    QString ffprobe_output_str = Get_VideoFrameNumDigits_process->readAllStandardOutput();
    delete Get_VideoFrameNumDigits_process;
    // read json
    QJsonDocument json_output = QJsonDocument::fromJson(ffprobe_output_str.toUtf8());
    if(!json_output.isObject()){
        emit Send_TextBrowser_NewMessage("ERROR! Unable to read the number of frames of the video: [" + videoPath + "]");
        return 0;
    }
    if(!json_output["streams"].isArray()){
        emit Send_TextBrowser_NewMessage("ERROR! Unable to read the number of frames of the video: [" + videoPath + "]");
        return 0;
    }
    QJsonArray streams = json_output["streams"].toArray();
    if(streams.isEmpty()||!streams.at(0).isObject()) {
        emit Send_TextBrowser_NewMessage("ERROR! Unable to read the number of frames of the video: [" + videoPath + "]");
        return 0;
    }
    QJsonObject stream = streams.at(0).toObject();
    if(!stream["nb_frames"].isString()) {
        emit Send_TextBrowser_NewMessage("ERROR! Unable to read the number of frames of the video: [" + videoPath + "]");
        return 0;
    }
    QString FrameNum_Str = stream["nb_frames"].toString().trimmed();
    int FrameNum = FrameNum_Str.toInt();
    if (FrameNum < 1)
    {
        emit Send_TextBrowser_NewMessage("ERROR! Unable to read the number of frames of the video: [" + videoPath + "]");
        return 0;
    }
    int frameNumDigits = 1 + (int)log10(FrameNum);
    return frameNumDigits;
}

void MainWindow::video_video2images(QString VideoPath, QString FrameFolderPath, QString AudioPath)
{
    emit Send_TextBrowser_NewMessage("Start splitting video: [" + VideoPath + "]");
    //=================
    QString ffmpeg_path = Current_Path + "/ffmpeg.exe";
    QString video_mp4_fullpath = VideoPath;
    //================ 获取fps =====================
    QStringList fps_video_cmd;
    QString fps = video_get_fps(video_mp4_fullpath).trimmed();
    if (fps != "0.0")
    {
        fps_video_cmd.append("-r");
        fps_video_cmd.append(fps);
    }
    //=====================
    int FrameNumDigits = video_get_frameNumDigits(video_mp4_fullpath);
    if (FrameNumDigits == 0)
        return;
    QProcess *video_splitFrame = new QProcess();
    QStringList args;
    args.append("-y");
    args.append(fps_video_cmd);
    args.append("-i");
    args.append(video_mp4_fullpath);
    args.append(fps_video_cmd);
    args.append(FrameFolderPath.replace("%", "%%") + "/%0" + QString::number(FrameNumDigits, 10) + "d.png");
    video_splitFrame->start(ffmpeg_path, args);
    if(waitForProcess(video_splitFrame,INF_WAIT)==TerminatedByFlag){
        delete video_splitFrame;
        return;
    }
    delete video_splitFrame;
    video_get_audio(video_mp4_fullpath, AudioPath); // 拆分音频
    //====================================
    emit Send_TextBrowser_NewMessage("Finish splitting video: [" + VideoPath + "]");
}

int MainWindow::video_images2video(QString VideoPath, QString video_mp4_scaled_fullpath, QString ScaledFrameFolderPath, QString AudioPath, bool CustRes_isEnabled, int CustRes_height, int CustRes_width)
{
    emit Send_TextBrowser_NewMessage("Start assembling video:[" + VideoPath + "]");
    bool Del_DenoisedAudio = false;
    //=================
    QStringList bitrate_video_cmd;
    //=======
    if (ui->groupBox_video_settings->isChecked() && (ui->spinBox_bitrate_vid->value() > 0))
    {
        bitrate_video_cmd.append("-b:v");
        bitrate_video_cmd.append(QString::number(ui->spinBox_bitrate_vid->value(), 10) + "k");
    }
    else
    {
        QString BitRate = video_get_bitrate_AccordingToRes_FrameFolder(ScaledFrameFolderPath);
        if (BitRate != "")
        {
            bitrate_video_cmd.append("-b:v");
            bitrate_video_cmd.append(BitRate);
        }
    }
    //================ 自定义分辨率 ======================
    QStringList resize_cmd;
    if (CustRes_isEnabled)
    {
        //============= 如果没有自定义视频参数, 则根据自定义分辨率再计算一次比特率 ==========
        if (ui->groupBox_video_settings->isChecked() == false)
        {
            int small_res = 0;
            if (CustRes_width <= CustRes_height)
            {
                small_res = CustRes_width;
            }
            else
            {
                small_res = CustRes_height;
            }
            bitrate_video_cmd.clear();
            bitrate_video_cmd.append("-b:v");
            bitrate_video_cmd.append(QString::number(small_res * 6, 10) + "k");
        }
        //=================================================================
        if (CustRes_AspectRatioMode == Qt::IgnoreAspectRatio)
        {
            resize_cmd.clear();
            resize_cmd.append("-vf");
            resize_cmd.append("scale=" + QString::number(CustRes_width, 10) + ":" + QString::number(CustRes_height, 10));
        }
        if (CustRes_AspectRatioMode == Qt::KeepAspectRatio)
        {
            if (CustRes_width >= CustRes_height)
            {
                resize_cmd.clear();
                resize_cmd.append("-vf");
                resize_cmd.append("scale=-2:" + QString::number(CustRes_height, 10));
            }
            else
            {
                resize_cmd.clear();
                resize_cmd.append("-vf");
                resize_cmd.append("scale=" + QString::number(CustRes_width, 10) + ":-2");
            }
        }
        if (CustRes_AspectRatioMode == Qt::KeepAspectRatioByExpanding)
        {
            if (CustRes_width >= CustRes_height)
            {
                resize_cmd.clear();
                resize_cmd.append("-vf");
                resize_cmd.append("scale=" + QString::number(CustRes_width, 10) + ":-2");
            }
            else
            {
                resize_cmd.clear();
                resize_cmd.append("-vf");
                resize_cmd.append("scale=-2:" + QString::number(CustRes_height, 10));
            }
        }
    }
    QString ffmpeg_path = Current_Path + "/ffmpeg.exe";
    int FrameNumDigits = video_get_frameNumDigits(VideoPath);
    if (FrameNumDigits == 0)
        return 0;
    QFileInfo vfinfo(VideoPath);
    QString video_dir = file_getFolderPath(vfinfo);
    QString video_filename = file_getBaseName(VideoPath);
    QString video_ext = vfinfo.suffix();
    //=========== 获取fps ===========
    QString fps = video_get_fps(VideoPath).trimmed();
    if (fps == "0.0")
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + VideoPath + "]. Error: [Unable to get video frame rate.]");
        return 0;
    }
    //=============== 音频降噪 ========================
    if ((ui->checkBox_AudioDenoise->isChecked()) && QFile::exists(AudioPath))
    {
        QString AudioPath_tmp = video_AudioDenoise(AudioPath);
        if (AudioPath_tmp != AudioPath)
        {
            AudioPath = AudioPath_tmp;
            Del_DenoisedAudio = true;
        }
    }
    //================= 开始处理 =============================
    QStringList ARGS = {"-y", "-f", "image2", "-framerate", fps, "-r", fps, "-i", ScaledFrameFolderPath.replace("%", "%%") + "/%0" + QString::number(FrameNumDigits, 10) + "d.png"};
    if (QFile::exists(AudioPath))
    {
        ARGS.append("-i");
        ARGS.append(AudioPath);
    }
    ARGS.append("-r");
    ARGS.append(fps);
    ARGS.append(bitrate_video_cmd);
    ARGS.append(resize_cmd);
    ARGS.append(video_ReadSettings_OutputVid(AudioPath));
    ARGS.append("-r");
    ARGS.append(fps);
    ARGS.append(video_mp4_scaled_fullpath);
    QProcess *images2video = new QProcess();
    images2video->start(ffmpeg_path, ARGS);
    if(waitForProcess(images2video,INF_WAIT)==TerminatedByFlag){
        delete images2video;
        return 0;
    }
    delete images2video;
    //===================
    if (Del_DenoisedAudio)
        QFile::remove(AudioPath);
    //==============================
    emit Send_TextBrowser_NewMessage("Finish assembling video:[" + VideoPath + "]");
    return 0;
}

QStringList MainWindow::video_ReadSettings_OutputVid(QString AudioPath)
{
    QStringList OutputVideoSettings;
    //====
    if (ui->groupBox_video_settings->isChecked())
    {
        if (ui->lineEdit_encoder_vid->text() != "")
        {
            OutputVideoSettings.append("-c:v");
            OutputVideoSettings.append(ui->lineEdit_encoder_vid->text()); // 图像编码器
        }
        //========
        if (QFile::exists(AudioPath))
        {
            if (ui->lineEdit_encoder_audio->text() != "")
            {
                OutputVideoSettings.append("-c:a");
                OutputVideoSettings.append(ui->lineEdit_encoder_audio->text()); // 音频编码器
            }
            //=========
            if (ui->spinBox_bitrate_audio->value() > 0)
            {
                OutputVideoSettings.append("-b:a");
                OutputVideoSettings.append(QString::number(ui->spinBox_bitrate_audio->value(), 10) + "k"); // 音频比特率
            }
        }
        //=========
        if (ui->lineEdit_pixformat->text() != "")
        {
            OutputVideoSettings.append("-pix_fmt");
            OutputVideoSettings.append(ui->lineEdit_pixformat->text()); // pixel format
        }
        else
        {
            OutputVideoSettings.append("-pix_fmt");
            OutputVideoSettings.append("yuv420p"); // pixel format
        }
        //===========
        if (ui->lineEdit_ExCommand_output->text() != "")
        {
            OutputVideoSettings.append(commandResolve(ui->lineEdit_ExCommand_output->text())); // 附加指令
        }
    }
    //=========
    else
    {
        OutputVideoSettings.append("-pix_fmt");
        OutputVideoSettings.append("yuv420p"); // pixel format
    }
    //=======
    return OutputVideoSettings;
}
