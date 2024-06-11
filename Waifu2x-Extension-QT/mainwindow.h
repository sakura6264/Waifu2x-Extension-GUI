﻿
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QDir>
#include <QList>
#include <QMap>
#include <QtConcurrent>
#include <QProcess>
#include <QFutureWatcher>
#include <QMovie>
#include <QImage>
#include <QImageWriter>
#include <QFile>
#include <QVariant>
#include <windows.h>
#include <QTime>
#include <QDesktopServices>
#include <QSize>
#include <QTimer>
#include <QTextCursor>
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>
#include <QScreen>
#include <QCloseEvent>
#include <QFileDialog>
#include <QTextCodec>
#include <QMutex>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMetaType>
#include <QRandomGenerator>
#include <QScrollBar>
#include <QThread>
#include <QRegExp>
#include "utils.h"

typedef QList<QMap<QString, QString>> QList_QMap_QStrQStr;
Q_DECLARE_METATYPE(QList_QMap_QStrQStr)

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void changeEvent(QEvent *e);
    //=======
    QIcon *MainIcon_QIcon = new QIcon(":/icon/icon/icon_main.png");
    //=======
    QString Current_Path = qApp->applicationDirPath(); // 当前路径
    //=======wecho.exe=====
    QString WECHO_Path;
    void showTextExt(QString title, QString msg);
    //=================================  File 文件=================================
    void dragEnterEvent(QDragEnterEvent *event); // 拖放文件event
    void dropEvent(QDropEvent *event);
    void Read_urls(QList<QUrl> urls);
    void Read_Input_paths_BrowserFile(QStringList Input_path_List);
    bool AddNew_gif = false;                                  // 判断是否有新增文件-gif
    bool AddNew_image = false;                                // 判断是否有新增文件-图片
    bool AddNew_video = false;                                // 判断是否有新增文件-视频
    void Add_File_Folder(QString Full_Path);                  // 添加文件or文件夹(判断一个路径是文件还是文件夹,然后处理判断类型添加到table和file list)
    void Add_File_Folder_IncludeSubFolder(QString Full_Path); // 添加文件文件夹(扫描子文件夹

    QStringList getFileNames_IncludeSubFolder(QString path);         // 读取文件列表, 包括文件夹
    int FileList_Add(QString fileName, QString SourceFile_fullPath); // 直接向file list和tableview添加文件

    // 判断一个文件是否已存在于一个文件列表中(输入list和完整路径,然后判断返回bool)
    bool Deduplicate_filelist(QString SourceFile_fullPath);

    bool file_isDirExist(QString SourceFile_fullPath); // 判断文件夹是否存在
    void file_mkDir(QString SourceFile_fullPath);      // 创建文件夹
    bool file_isDirEmpty(QString FolderPath);
    QStringList file_getFileNames_in_Folder_nofilter(QString path); // 读取文件夹内文件列表并返回(无过滤器)
    bool file_DelDir(const QString &path);                          // 删除文件夹(无论是否为空,强制删除)
    QString file_getBaseName(QString path);                         // 获取basename

    void file_MoveToTrash(QString file); // 移动到回收站

    void MoveFileToOutputPath(QString Orginal, QString SourceFilePath); // 移动文件
    QMutex MoveFile_QMutex;

    QString file_getFolderPath(QFileInfo fileInfo); // 获取文件夹路径(去除末尾的"/")

    bool file_isDirWritable(QString DirPath); // 检查文件夹是否可写入

    // 检查当前行的文件所在的文件夹是否可写入
    bool file_isFilesFolderWritable_row_image(int rowNum);
    bool file_isFilesFolderWritable_row_video(int rowNum);
    bool file_isFilesFolderWritable_row_gif(int rowNum);

    bool file_OpenFolder(QString FolderPath);    // 调用系统资源管理器打开文件夹.
    bool file_OpenFilesFolder(QString FilePath); // 打开文件所在的文件夹
    bool file_OpenFile(QString FilePath);
    //=================================  Table =================================
    void ui_tableViews_setUpdatesEnabled(bool isEnabled); // 启用/禁用 文件列表table的UI更新

    void Init_Table(); // 初始化三个tableview
    QStandardItemModel *Table_model_image = new QStandardItemModel();
    QStandardItemModel *Table_model_video = new QStandardItemModel();
    QStandardItemModel *Table_model_gif = new QStandardItemModel();
    // 取消指定row的自定义分辨率
    void Table_image_CustRes_Cancel_rowNumInt(int rowNum);
    void Table_gif_CustRes_Cancel_rowNumInt(int rowNum);
    void Table_video_CustRes_Cancel_rowNumInt(int rowNum);

    void Table_ChangeAllStatusToWaiting(); // 将所有row的状态改为waiting
    QMutex QMutex_Table_ChangeAllStatusToWaiting;

    void Table_Clear(); // 清空tableview
    // 获取下一个row值(用于插入新数据
    int Table_image_get_rowNum();
    int Table_gif_get_rowNum();
    int Table_video_get_rowNum();

    // 根据输入的table,返回成对的[完整路径]和[状态]:[fullpath]=status;
    QMap<QString, QString> Table_Read_status_fullpath(QStandardItemModel *Table_model);
    // 当前选中的rowNum
    int curRow_image = -1;
    int curRow_gif = -1;
    int curRow_video = -1;

    int Table_Save_Current_Table_Filelist(); // 保存当前文件列表(包括table

    int Table_Read_Saved_Table_Filelist(); // 读取保存的文件列表

    int Table_Save_Current_Table_Filelist_Watchdog(); // 保存文件列表的看门狗线程,确保文件成功保存

    bool Table_insert_finished = false;
    QMutex mutex_Table_insert_finished; // 监管是否完成表格插入的bool值的mutex

    QMutex mutex_Table_insert; // 监管表格插入的mutex
    //================================= Waifu2x ====================================
    QString OutPutFolder_main = ""; // 总输出文件夹
    int Waifu2xMainThread();        // waifu2x总线程,负责读取文件列表,调度waifu2x放大线程
    //==========================
    QString Waifu2x_ncnn_vulkan_FolderPath = "";
    QString Waifu2x_ncnn_vulkan_ProgramPath = "";
    //===
    int Waifu2x_NCNN_Vulkan_Image(int rowNum, bool ReProcess_MissingAlphaChannel); // vulkan放大图片线程
    // vulakn放大GIF线程:1.主线程,拆分,调度放大子线程,组装&压缩;2.放大子线程,负责放大所有帧以及调整大小
    int Waifu2x_NCNN_Vulkan_GIF(int rowNum);
    int Waifu2x_NCNN_Vulkan_GIF_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_gif_ThreadNumRunning, bool *Frame_failed);
    // vulkan放大视频线程:1.主线程,拆分,调度放大子线程,组装;2.放大子线程,负责放大所有帧以及调整大小
    int Waifu2x_NCNN_Vulkan_Video(int rowNum);
    int Waifu2x_NCNN_Vulkan_Video_BySegment(int rowNum);
    int Waifu2x_NCNN_Vulkan_Video_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_video_ThreadNumRunning, bool *Frame_failed);
    QStringList Waifu2x_NCNN_Vulkan_ReadSettings();
    //===
    int Realsr_NCNN_Vulkan_Image(int rowNum, bool ReProcess_MissingAlphaChannel); // Realsr放大图片线程
    // Realsr放大GIF线程:1.主线程,拆分,调度放大子线程,组装&压缩;2.放大子线程,负责放大所有帧以及调整大小
    int Realsr_NCNN_Vulkan_GIF(int rowNum);
    int Realsr_NCNN_Vulkan_GIF_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_gif_ThreadNumRunning, bool *Frame_failed);
    // Realsr放大视频线程:1.主线程,拆分,调度放大子线程,组装;2.放大子线程,负责放大所有帧以及调整大小
    int Realsr_NCNN_Vulkan_Video(int rowNum);
    int Realsr_NCNN_Vulkan_Video_BySegment(int rowNum);
    int Realsr_NCNN_Vulkan_Video_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_video_ThreadNumRunning, bool *Frame_failed);
    QStringList Realsr_NCNN_Vulkan_ReadSettings();
    //===
    int RealESRGAN_NCNN_Vulkan_Image(int rowNum, bool ReProcess_MissingAlphaChannel); // RealESRGAN放大图片线程
    // RealESRGAN放大GIF线程:1.主线程,拆分,调度放大子线程,组装&压缩;2.放大子线程,负责放大所有帧以及调整大小
    int RealESRGAN_NCNN_Vulkan_GIF(int rowNum);
    int RealESRGAN_NCNN_Vulkan_GIF_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_gif_ThreadNumRunning, bool *Frame_failed);
    // RealESRGAN放大视频线程:1.主线程,拆分,调度放大子线程,组装;2.放大子线程,负责放大所有帧以及调整大小
    int RealESRGAN_NCNN_Vulkan_Video(int rowNum);
    int RealESRGAN_NCNN_Vulkan_Video_BySegment(int rowNum);
    int RealESRGAN_NCNN_Vulkan_Video_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_video_ThreadNumRunning, bool *Frame_failed);
    QStringList RealESRGAN_NCNN_Vulkan_ReadSettings();
    //===
    int RealCUGAN_NCNN_Vulkan_Image(int rowNum, bool ReProcess_MissingAlphaChannel); // RealCUGAN放大图片线程
    // RealCUGAN放大GIF线程:1.主线程,拆分,调度放大子线程,组装&压缩;2.放大子线程,负责放大所有帧以及调整大小
    int RealCUGAN_NCNN_Vulkan_GIF(int rowNum);
    int RealCUGAN_NCNN_Vulkan_GIF_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_gif_ThreadNumRunning, bool *Frame_failed);
    // RealCUGAN放大视频线程:1.主线程,拆分,调度放大子线程,组装;2.放大子线程,负责放大所有帧以及调整大小
    int RealCUGAN_NCNN_Vulkan_Video(int rowNum);
    int RealCUGAN_NCNN_Vulkan_Video_BySegment(int rowNum);
    int RealCUGAN_NCNN_Vulkan_Video_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_video_ThreadNumRunning, bool *Frame_failed);
    QStringList RealCUGAN_NCNN_Vulkan_ReadSettings();
    //=========================
    int Anime4k_Image(int rowNum, bool ReProcess_MissingAlphaChannel);
    int Anime4k_GIF(int rowNum);
    int Anime4k_GIF_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_gif_ThreadNumRunning, bool *Frame_failed);
    // Anime4k放大视频线程:1.主线程,拆分,调度放大子线程,组装;2.放大子线程,负责放大所有帧以及调整大小
    int Anime4k_Video(int rowNum);
    int Anime4k_Video_BySegment(int rowNum);
    int Anime4k_Video_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_video_ThreadNumRunning, bool *Frame_failed);
    QStringList Anime4k_ReadSettings(bool PreserveAlphaChannel);
    void DenoiseLevelSpinboxSetting_Anime4k();
    QString Anime4k_ProgramPath = Current_Path + "/Anime4K/Anime4K.exe";
    //=================================
    int Waifu2x_Converter_Image(int rowNum, bool ReProcess_MissingAlphaChannel); // Converter放大图片线程
    // Converter放大GIF线程:1.主线程,拆分,调度放大子线程,组装&压缩;2.放大子线程,负责放大所有帧以及调整大小
    int Waifu2x_Converter_GIF(int rowNum);
    int Waifu2x_Converter_GIF_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_gif_ThreadNumRunning, bool *Frame_failed);
    // Converter放大视频线程:1.主线程,拆分,调度放大子线程,组装;2.放大子线程,负责放大所有帧以及调整大小
    int Waifu2x_Converter_Video(int rowNum);
    int Waifu2x_Converter_Video_BySegment(int rowNum);
    int Waifu2x_Converter_Video_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_video_ThreadNumRunning, bool *Frame_failed);
    QStringList Waifu2xConverter_ReadSettings();
    //===================================
    int SRMD_NCNN_Vulkan_Image(int rowNum, bool ReProcess_MissingAlphaChannel); // SRMD放大图片线程
    // SRMD放大GIF线程:1.主线程,拆分,调度放大子线程,组装&压缩;2.放大子线程,负责放大所有帧以及调整大小
    int SRMD_NCNN_Vulkan_GIF(int rowNum);
    int SRMD_NCNN_Vulkan_GIF_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_gif_ThreadNumRunning, bool *Frame_failed);
    // SRMD放大视频线程:1.主线程,拆分,调度放大子线程,组装;2.放大子线程,负责放大所有帧以及调整大小
    int SRMD_NCNN_Vulkan_Video(int rowNum);
    int SRMD_NCNN_Vulkan_Video_BySegment(int rowNum);
    int SRMD_NCNN_Vulkan_Video_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_video_ThreadNumRunning, bool *Frame_failed);
    QStringList SrmdNcnnVulkan_ReadSettings();
    //=================================
    int Waifu2x_Caffe_Image(int rowNum, bool ReProcess_MissingAlphaChannel); // Caffe放大图片线程
    // Caffe放大GIF线程:1.主线程,拆分,调度放大子线程,组装&压缩;2.放大子线程,负责放大所有帧以及调整大小
    int Waifu2x_Caffe_GIF(int rowNum);
    int Waifu2x_Caffe_GIF_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_gif_ThreadNumRunning, bool *Frame_failed);
    // Caffe放大视频线程:1.主线程,拆分,调度放大子线程,组装;2.放大子线程,负责放大所有帧以及调整大小
    int Waifu2x_Caffe_Video(int rowNum);
    int Waifu2x_Caffe_Video_BySegment(int rowNum);
    int Waifu2x_Caffe_Video_scale(QMap<QString, QString> Sub_Thread_info, int *Sub_video_ThreadNumRunning, bool *Frame_failed);
    QStringList Waifu2x_Caffe_ReadSettings();
    bool isWaifu2xCaffeEnabled();
    void DeleteErrorLog_Waifu2xCaffe();

    void Wait_waifu2x_stop();          // 等待waifu2x主线程完全停止所有子线程的看门狗线程
    bool waifu2x_STOP = false;         // 负责通知waifu2x主线程及其子线程的停止信号
    bool waifu2x_STOP_confirm = false; // 返回给waifu2x停止看门狗的信号

    int ThreadNumMax = 0;     // waifu2x放大线程最大值
    int ThreadNumRunning = 0; // 正在运行的waifu2x线程数量

    QMutex mutex_ThreadNumRunning;    // 监管总线程数量的mutex
    QMutex mutex_SubThreadNumRunning; // 监管内部线程数量的mutex

    int Waifu2x_DetectGPU();     // 检测可用gpu(for vulkan)
    QStringList Available_GPUID; // 可用GPU ID列表

    int Waifu2x_DumpProcessorList_converter();
    int Core_num = 0;
    QStringList Available_ProcessorList_converter;
    QStringList Processor_converter_STR;

    int SRMD_DetectGPU();             // 检测可用gpu(for srmd)
    QStringList Available_GPUID_srmd; // 可用GPU ID列表
    QStringList GPU_ID_STR_SRMD;      // 向srmd命令行cmd插入的gpuid命令,如果auto则为空

    int Realsr_ncnn_vulkan_DetectGPU();             // 检测可用gpu(for realsr)
    QStringList Available_GPUID_Realsr_ncnn_vulkan; // 可用GPU ID列表

    int RealESRGAN_ncnn_vulkan_DetectGPU();             // 检测可用gpu(for realesrgan)
    QStringList Available_GPUID_RealESRGAN_ncnn_vulkan; // 可用GPU ID列表

    int RealCUGAN_ncnn_vulkan_DetectGPU();             // 检测可用gpu(for realesrgan)
    QStringList Available_GPUID_RealCUGAN_ncnn_vulkan; // 可用GPU ID列表

    void ListGPUs_Anime4k(); // 列出可用显卡 Anime4k
    //======================== 图片处理 ================================
    bool Image_Gif_AutoSkip_CustRes(int rowNum, bool isGif);
    QMap<QString, int> Image_Gif_Read_Resolution(QString SourceFileFullPath); // 获取图片&GIF分辨率
    bool Imgae_hasAlphaChannel(int rowNum);
    QString Imgae_PreProcess(QString ImagePath, bool ReProcess_AlphaChannel);
    QString SaveImageAs_FormatAndQuality(QString OriginalSourceImage_fullPath, QString ScaledImage_fullPath, int ScaleRatio, bool isDenoiseLevelEnabled, int DenoiseLevel);
    //================================================================
    int Waifu2x_Compatibility_Test(); // 引擎兼容性检测
    // 初始化 -兼容性测试进度条
    void Init_progressBar_CompatibilityTest();
    // 兼容性测试完成后的操作 -兼容性测试进度条
    void Finish_progressBar_CompatibilityTest();
    // 兼容性检测
    bool isCompatible_Waifu2x_NCNN_Vulkan_NEW = false;
    bool isCompatible_Waifu2x_NCNN_Vulkan_NEW_FP16P = false;
    bool isCompatible_SRMD_NCNN_Vulkan = false;
    bool isCompatible_Waifu2x_Converter = false;
    bool isCompatible_Anime4k_CPU = false;
    bool isCompatible_Anime4k_GPU = false;
    bool isCompatible_FFmpeg = false;
    bool isCompatible_FFprobe = false;
    bool isCompatible_ImageMagick = false;
    bool isCompatible_Gifsicle = false;
    bool isCompatible_SoX = false;
    bool isCompatible_Waifu2x_Caffe_CPU = false;
    bool isCompatible_Waifu2x_Caffe_GPU = false;
    bool isCompatible_Waifu2x_Caffe_cuDNN = false;
    bool isCompatible_Realsr_NCNN_Vulkan = false;
    bool isCompatible_RealESRGAN_NCNN_Vulkan = false;
    bool isCompatible_RealCUGAN_NCNN_Vulkan = false;
    //============================== 多显卡 ==========================================
    // waifu2x-ncnn-vulkan
    int GPU_ID_Waifu2x_NCNN_Vulkan_MultiGPU = 0;
    QMap<QString, QString> Waifu2x_NCNN_Vulkan_MultiGPU();
    QMutex MultiGPU_QMutex_Waifu2xNCNNVulkan;
    QList<QMap<QString, QString>> GPUIDs_List_MultiGPU_Waifu2xNCNNVulkan;
    void AddGPU_MultiGPU_Waifu2xNCNNVulkan(QString GPUID);

    // SRMD-ncnn-vulkan
    int GPU_ID_SrmdNcnnVulkan_MultiGPU = 0;
    QMap<QString, QString> SrmdNcnnVulkan_MultiGPU();
    QMutex MultiGPU_QMutex_SrmdNcnnVulkan;
    QList<QMap<QString, QString>> GPUIDs_List_MultiGPU_SrmdNcnnVulkan;
    void AddGPU_MultiGPU_SrmdNcnnVulkan(QString GPUID);

    // RealsrNcnnVulkan
    int GPU_ID_RealsrNcnnVulkan_MultiGPU = 0;
    QMap<QString, QString> RealsrNcnnVulkan_MultiGPU();
    QMutex MultiGPU_QMutex_RealsrNcnnVulkan;
    QList<QMap<QString, QString>> GPUIDs_List_MultiGPU_RealsrNcnnVulkan;
    void AddGPU_MultiGPU_RealsrNcnnVulkan(QString GPUID);

    // RealESRGANNcnnVulkan
    int GPU_ID_RealESRGANNcnnVulkan_MultiGPU = 0;
    QMap<QString, QString> RealESRGANNcnnVulkan_MultiGPU();
    QMutex MultiGPU_QMutex_RealESRGANNcnnVulkan;
    QList<QMap<QString, QString>> GPUIDs_List_MultiGPU_RealESRGANNcnnVulkan;
    void AddGPU_MultiGPU_RealESRGANNcnnVulkan(QString GPUID);

    // RealCUGANNcnnVulkan
    int GPU_ID_RealCUGANNcnnVulkan_MultiGPU = 0;
    QMap<QString, QString> RealCUGANNcnnVulkan_MultiGPU();
    QMutex MultiGPU_QMutex_RealCUGANNcnnVulkan;
    QList<QMap<QString, QString>> GPUIDs_List_MultiGPU_RealCUGANNcnnVulkan;
    void AddGPU_MultiGPU_RealCUGANNcnnVulkan(QString GPUID);

    // Waifu2xConverter
    int GPU_ID_Waifu2xConverter_MultiGPU = 0;
    QMap<QString, QString> Waifu2xConverter_MultiGPU();
    QMutex MultiGPU_QMutex_Waifu2xConverter;
    QList<QMap<QString, QString>> GPUIDs_List_MultiGPU_Waifu2xConverter;
    void AddGPU_MultiGPU_Waifu2xConverter(QString GPUID);

    // Anime4k
    int GPU_ID_Anime4k_GetGPUInfo = 0;
    QStringList Anime4k_GetGPUInfo();
    QMutex GetGPUInfo_QMutex_Anime4k;

    // Waifu2x-caffe
    int GPU_ID_Waifu2xCaffe_GetGPUInfo = 0;
    QStringList Waifu2xCaffe_GetGPUInfo();
    QMutex GetGPUInfo_QMutex_Waifu2xCaffe;
    //================================ progressbar ===================================
    int Progressbar_MaxVal = 0;            // 进度条最大值
    int Progressbar_CurrentVal = 0;        // 进度条当前值
    void progressbar_clear();              // 清空进度条
    void progressbar_SetToMax(int maxval); // 将进度条设定到最大值
    //=============================== textbrowser===============================
    void TextBrowser_StartMes(); // 输出启动信息
    //================================ gif ====================================
    int Gif_getDuration(QString gifPath);                                                                                                            // 获取帧间隔时长
    int Gif_getFrameDigits(QString gifPath);                                                                                                         // 获取帧数量的位数
    void Gif_splitGif(QString gifPath, QString SplitFramesFolderPath);                                                                               // 拆分gif
    void Gif_assembleGif(QString ResGifPath, QString ScaledFramesPath, int Duration, bool CustRes_isEnabled, int CustRes_height, int CustRes_width); // 组装gif
    QString Gif_compressGif(QString gifPath, QString gifPath_compressd);                                                                             // 压缩gif
    //================================= video ===============================
    bool Video_AutoSkip_CustRes(int rowNum);
    int video_UseRes2CalculateBitrate(QString VideoFileFullPath);       // 根据视频的分辨率计算他应该被分配的比特率
    QMap<QString, int> video_get_Resolution(QString VideoFileFullPath); // 获取视频的帧率
    QString video_get_fps(QString videoPath);                           // 获取视频fps
    int video_get_frameNumDigits(QString videoPath);                    // 获取帧数量的位数
    // 判断视频是否可变帧率
    bool video_isVFR(QString videoPath);
    // 拆分视频
    void video_video2images(QString VideoPath, QString FrameFolderPath, QString AudioPath);
    // 组装视频
    int video_images2video(QString VideoPath, QString video_mp4_scaled_fullpath, QString ScaledFrameFolderPath, QString AudioPath, bool CustRes_isEnabled, int CustRes_height, int CustRes_width);
    // 读取输出视频的设定参数
    QStringList video_ReadSettings_OutputVid(QString AudioPath);
    // 获取视频比特率
    QString video_get_bitrate(QString videoPath);
    // 获取视频比特率(根据分辨率计算)
    QString video_get_bitrate_AccordingToRes_FrameFolder(QString ScaledFrameFolderPath);
    // 音频降噪
    QString video_AudioDenoise(QString OriginalAudioPath);
    // 获取时长(秒)
    int video_get_duration(QString videoPath);
    // 转换为mp4
    QString video_To_CFRMp4(QString VideoPath);
    // 提取音频
    void video_get_audio(QString VideoPath, QString AudioPath);
    // 拆分视频(分段)
    void video_video2images_ProcessBySegment(QString VideoPath, QString FrameFolderPath, int StartTime, int SegmentDuration);
    // 组装视频(mp4片段到成片)
    void video_AssembleVideoClips(QString VideoClipsFolderPath, QString VideoClipsFolderName, QString video_mp4_scaled_fullpath, QString AudioPath);
    // 生成视频片段文件夹编号
    QString video_getClipsFolderNo();
    QMutex MultiLine_ErrorOutput_QMutex;

    bool video_isNeedProcessBySegment(int rowNum); // 根据视频时长,判断是否需要分段处理
    //============================   custom res  ====================================
    // 自定义分辨率列表
    QList<QMap<QString, QString>> Custom_resolution_list;                          // res_map["fullpath"],["height"],["width"]
    void CustRes_remove(QString fullpath);                                         // 根据完整路径,移除自定义分辨率内条目
    bool CustRes_isContained(QString fullpath);                                    // 检查是否包括某文件
    QMap<QString, QString> CustRes_getResMap(QString fullpath);                    // 读取指定文件的自定义分辨率值
    int CustRes_CalNewScaleRatio(QString fullpath, int Height_new, int width_new); // 计算新的放大倍数
    int CustRes_SetCustRes();                                                      // 设定自定义分辨率
    int CustRes_CancelCustRes();                                                   // 取消自定义分辨率

    Qt::AspectRatioMode CustRes_AspectRatioMode = Qt::IgnoreAspectRatio; // 自定义分辨率的纵横比策略
    //======================== 设置 ===========================================
    int Settings_Read_Apply();       // 读取与apply设置
    bool Settings_isReseted = false; // 是否重置设置标记
    QVariant Settings_Read_value(QString Key);
    //================================ Other =======================================
    int ForceRetryCount = 1;

    void OutputSettingsArea_setEnabled(bool isEnabled);

    bool isFirstTimeStart = false;

    bool isForceRetryClicked = false;
    QMutex isForceRetryClicked_QMutex;
    void DelTrash_ForceRetry_Anime4k(QString OutPut_Path);
    void isForceRetryClicked_SetTrue_Block_Anime4k();

    // 阻塞延时(安全
    void Delay_sec_sleep(int time);
    void Delay_msec_sleep(int time);

    QTimer *TimeCostTimer;                          // 计算耗时的timer
    long unsigned int TimeCost = 0;                 // 已消耗时间
    QString Seconds2hms(long unsigned int seconds); // 秒 转 时:分:秒
    int TaskNumTotal = 0;                           // 总任务数量(需要处理的文件)
    int TaskNumFinished = 0;                        // 处理完的文件数量
    bool NewTaskFinished = false;                   // 新任务被完成之标记
    long unsigned int ETA = 0;                      // ETA时间(s)

    bool isSettingsHide = false; // 是否隐藏主页的设置groupbox

    bool isShowAnime4kWarning = true;

    bool DownloadTo(QString OnlineLink, QString LocalPath);
    //=========== 关闭窗口时执行的代码 ===============
    void closeEvent(QCloseEvent *event); // 关闭事件
    // void Close_self();//包含所有关闭时执行的代码
    bool QProcess_stop = false;        // 所有QProcess停止标记
    // process waiter. if msec <0 this will wait forever.
    // wait time = ( msec / period + 1 ) * period
    enum ProcessWaitInfo {
        TerminatedByFlag, Successful,Timeout,Crash,StoppedByExFlag
    };
    ProcessWaitInfo waitForProcess(QProcess* proc,int start_time = 30000, int run_time = INF_WAIT);
    // ex version. check waifu2x_STOP flag too.
    // if stopped by waifu2x flag, won't kill the process.
    ProcessWaitInfo waitForProcessWaifu2xEx(QProcess* proc,int start_time = 30000, int run_time = INF_WAIT);
    QFuture<int> Waifu2xMain;          // 监视waifu2x主线程
    int Force_close();                 // 强制关闭自己
    bool isAlreadyClosed = false;
    //================== 处理当前文件的进度 =========================
    long unsigned int TimeCost_CurrentFile = 0;
    int TaskNumTotal_CurrentFile = 0;
    int TaskNumFinished_CurrentFile = 0;
    bool NewTaskFinished_CurrentFile = false;
    long unsigned int ETA_CurrentFile = 0;
    bool isStart_CurrentFile = false;
    //=============================================
    void Tip_FirstTimeStart();
    //================== 托盘图标 =================
    void Init_SystemTrayIcon();
    QSystemTrayIcon *systemTray = new QSystemTrayIcon(this);
    QMenu *pContextMenu = new QMenu(this);
    QAction *minimumAct_SystemTrayIcon = new QAction(this);
    QAction *restoreAct_SystemTrayIcon = new QAction(this);
    QAction *quitAct_SystemTrayIcon = new QAction(this);
    QAction *BackgroundModeAct_SystemTrayIcon = new QAction(this);
    //================= 输出路径 lineEdit 的右键菜单 ==============
    void Init_ActionsMenu_lineEdit_outputPath();
    QAction *OpenFolder_lineEdit_outputPath = new QAction(this);
    //================= 文件列表的右键菜单 ====================
    void Init_ActionsMenu_FilesList();
    QAction *OpenFile_QAction_FileList = new QAction(this);
    QAction *OpenFilesFolder_QAction_FileList = new QAction(this);
    QAction *RemoveFile_FilesList_QAction_FileList = new QAction(this);
    bool EnableApply2All_CustRes = true;
    QAction *Apply_CustRes_QAction_FileList = new QAction(this);
    QAction *Cancel_CustRes_QAction_FileList = new QAction(this);
    void OpenSelectedFile_FailedWarning_FilesList();
    //================ 移除条目 按钮的右键菜单=======================
    void Init_ActionsMenu_pushButton_RemoveItem();
    QAction *RemoveALL_image = new QAction(this);
    QAction *RemoveALL_gif = new QAction(this);
    QAction *RemoveALL_video = new QAction(this);
    //=====================================================
    //===================== 直接替换源文件 =====================
    void checkBox_ReplaceOriginalFile_setEnabled_True_Self();
    bool ReplaceOriginalFile(QString original_fullpath, QString output_fullpath);
    void Init_ActionsMenu_checkBox_ReplaceOriginalFile(); // 替换源文件 的右键菜单
    QAction *QAction_checkBox_MoveToRecycleBin_checkBox_ReplaceOriginalFile = new QAction(this);
    //===================== 删除原文件的右键菜单 ===========================
    void Init_ActionsMenu_checkBox_DelOriginal(); // 替换源文件 的右键菜单
    QAction *QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal = new QAction(this);
    //===================== 事件过滤器 =====================
    bool eventFilter(QObject *target, QEvent *event);
    //===================== 块大小调整 ======================
    int AddTileSize_NCNNVulkan_Converter(int OrginalTileSize);
    int MinusTileSize_NCNNVulkan_Converter(int OrginalTileSize);
    //==================== 预读取引擎设定 ==================
    void PreLoad_Engines_Settings();
    //===
    QStringList Waifu2x_NCNN_Vulkan_PreLoad_Settings();
    QStringList Waifu2x_NCNN_Vulkan_PreLoad_Settings_Str;
    //===
    QStringList SrmdNcnnVulkan_PreLoad_Settings();
    QStringList SrmdNcnnVulkan_PreLoad_Settings_Str;
    //===
    QStringList Realsr_NCNN_Vulkan_PreLoad_Settings();
    QStringList Realsr_NCNN_Vulkan_PreLoad_Settings_Str;
    //===
    QStringList RealESRGAN_NCNN_Vulkan_PreLoad_Settings();
    QStringList RealESRGAN_NCNN_Vulkan_PreLoad_Settings_Str;
    //===
    QStringList RealCUGAN_NCNN_Vulkan_PreLoad_Settings();
    QStringList RealCUGAN_NCNN_Vulkan_PreLoad_Settings_Str;
    //===
    QStringList Waifu2xCaffe_PreLoad_Settings();
    QStringList Waifu2xCaffe_PreLoad_Settings_Str;
    //===
    QStringList Waifu2xConverter_PreLoad_Settings();
    QStringList Waifu2xConverter_PreLoad_Settings_Str;
    //===
    QStringList Anime4k_PreLoad_Settings();
    QStringList Anime4k_PreLoad_Settings_Str;
    QStringList HDNDenoiseLevel_image;
    QStringList HDNDenoiseLevel_gif;
    QStringList HDNDenoiseLevel_video;
    //================== 计算临时放大倍率 ====================
    int Calculate_Temporary_ScaleRatio_W2xNCNNVulkan(int ScaleRatio);
    //=============
    ~MainWindow();

public slots:
    void Table_EnableSorting(bool EnableSorting);

    void Apply_CustRes_QAction_FileList_slot();
    void Cancel_CustRes_QAction_FileList_slot();

    void RemoveALL_image_slot();
    void RemoveALL_gif_slot();
    void RemoveALL_video_slot();

    void Add_progressBar_CompatibilityTest(); // 进度+1 -兼容性测试进度条

    void OpenSelectedFilesFolder_FilesList();
    void OpenSelectedFile_FilesList();

    void OpenOutputFolder();

    void SetEnable_pushButton_ForceRetry_self();

    void SystemTray_hide_self();
    void SystemTray_showNormal_self();
    void SystemTray_NewMessage(QString message);
    void EnableBackgroundMode_SystemTray();
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

    void progressbar_setRange_min_max(int min, int max); // 进度条设定min和max
    void progressbar_Add();                              // 进度条进度+1

    // 根据row修改指定row的状态
    void Table_image_ChangeStatus_rowNumInt_statusQString(int rowNum, QString status);
    void Table_gif_ChangeStatus_rowNumInt_statusQString(int rowNum, QString status);
    void Table_video_ChangeStatus_rowNumInt_statusQString(int rowNum, QString status);

    void Waifu2x_Finished();        // 自动结束时调用的代码(会自动接着调用manual
    void Waifu2x_Finished_manual(); // 手动停止后调用的结束代码

    void TextBrowser_NewMessage(QString message); // Textbrowser发送新消息"[时间] 消息"

    void TimeSlot(); // 计时槽函数

    int Waifu2x_Compatibility_Test_finished(); // 兼容性检测结束后执行的槽函数

    int Waifu2x_DetectGPU_finished(); // 检测可用gpu结束后的执行的槽函数

    int Realsr_ncnn_vulkan_DetectGPU_finished(); // 检测可用gpu结束后的执行的槽函数

    int RealESRGAN_ncnn_vulkan_DetectGPU_finished(); // 检测可用gpu结束后的执行的槽函数

    int RealCUGAN_ncnn_vulkan_DetectGPU_finished(); // 检测可用gpu结束后的执行的槽函数

    int Table_FileCount_reload(); // 重载table下的文件数量计数

    // 向table插入文件名和fullpath
    void Table_image_insert_fileName_fullPath(QString fileName, QString SourceFile_fullPath);
    void Table_gif_insert_fileName_fullPath(QString fileName, QString SourceFile_fullPath);
    void Table_video_insert_fileName_fullPath(QString fileName, QString SourceFile_fullPath);

    // 向table插入自定义分辨率值
    void Table_image_CustRes_rowNumInt_HeightQString_WidthQString(int rowNum, QString height, QString width);
    void Table_gif_CustRes_rowNumInt_HeightQString_WidthQString(int rowNum, QString height, QString width);
    void Table_video_CustRes_rowNumInt_HeightQString_WidthQString(int rowNum, QString height, QString width);

    // 读取&保存文件列表与table后执行的代码
    int Table_Read_Saved_Table_Filelist_Finished();
    int Table_Save_Current_Table_Filelist_Finished();

    void on_pushButton_ClearList_clicked(); // 清空列表

    int Waifu2x_DumpProcessorList_converter_finished();

    void Read_urls_finfished();

    void SRMD_DetectGPU_finished();

    void video_write_VideoConfiguration(QString VideoConfiguration_fullPath, int ScaleRatio, int DenoiseLevel, bool CustRes_isEnabled, int CustRes_height, int CustRes_width, QString EngineName, bool isProcessBySegment, QString VideoClipsFolderPath, QString VideoClipsFolderName);

    int Settings_Save(); // 保存设置

    // 存储进度
    void video_write_Progress_ProcessBySegment(QString VideoConfiguration_fullPath, int StartTime, bool isSplitComplete, bool isScaleComplete, int OLDSegmentDuration);

    //================== 处理当前文件的进度 =========================
    void CurrentFileProgress_Start(QString FileName, int FrameNum);
    void CurrentFileProgress_Stop();
    void CurrentFileProgress_progressbar_Add();
    void CurrentFileProgress_progressbar_Add_SegmentDuration(int SegmentDuration);

private slots:
    void on_pushButton_Start_clicked();

    void on_pushButton_Stop_clicked();

    int on_pushButton_RemoveItem_clicked();

    void on_comboBox_Engine_Image_currentIndexChanged(int index);

    void on_comboBox_Engine_GIF_currentIndexChanged(int index);

    void on_comboBox_Engine_Video_currentIndexChanged(int index);

    void on_pushButton_clear_textbrowser_clicked();

    void on_pushButton_compatibilityTest_clicked();

    void on_pushButton_CustRes_apply_clicked();

    void on_pushButton_CustRes_cancel_clicked();

    void on_pushButton_HideSettings_clicked();

    void on_pushButton_DetectGPU_clicked();

    void on_pushButton_SaveSettings_clicked();

    void on_pushButton_ResetSettings_clicked();

    void on_pushButton_SaveFileList_clicked();

    void on_pushButton_ReadFileList_clicked();

    void on_Ext_image_editingFinished();

    void on_Ext_video_editingFinished();

    void on_checkBox_AutoSaveSettings_clicked();

    void on_pushButton_about_clicked();

    void on_comboBox_AspectRatio_custRes_currentIndexChanged(int index);

    void on_checkBox_AlwaysHideSettings_stateChanged(int arg1);

    void on_pushButton_BrowserFile_clicked();

    void on_pushButton_HideTextBro_clicked();

    void on_checkBox_AlwaysHideTextBrowser_stateChanged(int arg1);

    void on_pushButton_DumpProcessorList_converter_clicked();

    void on_comboBox_TargetProcessor_converter_currentIndexChanged(int index);

    void on_Ext_image_textChanged(const QString &arg1);

    void on_Ext_video_textChanged(const QString &arg1);

    void on_comboBox_model_vulkan_currentIndexChanged(int index);

    void on_comboBox_ImageStyle_currentIndexChanged(int index);

    void on_pushButton_ResetVideoSettings_clicked();

    void on_lineEdit_encoder_vid_textChanged(const QString &arg1);

    void on_lineEdit_encoder_audio_textChanged(const QString &arg1);

    void on_lineEdit_pixformat_textChanged(const QString &arg1);

    void on_checkBox_vcodec_copy_2mp4_stateChanged(int arg1);

    void on_checkBox_acodec_copy_2mp4_stateChanged(int arg1);

    void on_pushButton_encodersList_clicked();

    void on_checkBox_DelOriginal_stateChanged(int arg1);

    void on_checkBox_FileList_Interactive_stateChanged(int arg1);

    void on_checkBox_OutPath_isEnabled_stateChanged(int arg1);

    void on_pushButton_ForceRetry_clicked();

    void on_pushButton_DetectGPUID_srmd_clicked();

    void on_checkBox_AudioDenoise_stateChanged(int arg1);

    void on_tabWidget_currentChanged(int index);

    void on_checkBox_ProcessVideoBySegment_stateChanged(int arg1);

    void on_comboBox_version_Waifu2xNCNNVulkan_currentIndexChanged(int index);

    void on_checkBox_EnablePreProcessing_Anime4k_stateChanged(int arg1);

    void on_checkBox_EnablePostProcessing_Anime4k_stateChanged(int arg1);

    void on_checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW_clicked();

    void on_checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW_FP16P_clicked();

    void on_checkBox_isCompatible_SRMD_NCNN_Vulkan_clicked();

    void on_checkBox_isCompatible_Waifu2x_Converter_clicked();

    void on_checkBox_isCompatible_Anime4k_CPU_clicked();

    void on_checkBox_isCompatible_Anime4k_GPU_clicked();

    void on_checkBox_SpecifyGPU_Anime4k_stateChanged(int arg1);

    void on_pushButton_ListGPUs_Anime4k_clicked();

    void on_checkBox_isCompatible_FFmpeg_clicked();

    void on_checkBox_isCompatible_FFprobe_clicked();

    void on_checkBox_isCompatible_ImageMagick_clicked();

    void on_checkBox_isCompatible_Gifsicle_clicked();

    void on_checkBox_isCompatible_SoX_clicked();

    void on_checkBox_GPUMode_Anime4K_stateChanged(int arg1);

    void on_checkBox_ShowInterPro_stateChanged(int arg1);

    void on_checkBox_isCompatible_Waifu2x_Caffe_CPU_clicked();

    void on_checkBox_isCompatible_Waifu2x_Caffe_GPU_clicked();

    void on_checkBox_isCompatible_Waifu2x_Caffe_cuDNN_clicked();

    void on_pushButton_SplitSize_Add_Waifu2xCaffe_clicked();

    void on_pushButton_SplitSize_Minus_Waifu2xCaffe_clicked();

    void on_checkBox_isCompatible_Realsr_NCNN_Vulkan_clicked();

    void on_pushButton_DetectGPU_RealsrNCNNVulkan_clicked();

    void on_checkBox_isCompatible_RealESRGAN_NCNN_Vulkan_clicked();

    void on_pushButton_DetectGPU_RealESRGANNCNNVulkan_clicked();

    void on_checkBox_isCompatible_RealCUGAN_NCNN_Vulkan_clicked();

    void on_pushButton_DetectGPU_RealCUGANNCNNVulkan_clicked();

    void on_checkBox_ACNet_Anime4K_stateChanged(int arg1);

    void on_checkBox_MultiGPU_Waifu2xNCNNVulkan_clicked();

    void on_comboBox_GPUIDs_MultiGPU_Waifu2xNCNNVulkan_currentIndexChanged(int index);

    void on_spinBox_TileSize_CurrentGPU_MultiGPU_Waifu2xNCNNVulkan_valueChanged(int arg1);

    void on_checkBox_isEnable_CurrentGPU_MultiGPU_Waifu2xNCNNVulkan_clicked();

    void on_checkBox_MultiGPU_Waifu2xNCNNVulkan_stateChanged(int arg1);

    void on_checkBox_MultiGPU_SrmdNCNNVulkan_stateChanged(int arg1);

    void on_checkBox_MultiGPU_SrmdNCNNVulkan_clicked();

    void on_comboBox_GPUIDs_MultiGPU_SrmdNCNNVulkan_currentIndexChanged(int index);

    void on_checkBox_isEnable_CurrentGPU_MultiGPU_SrmdNCNNVulkan_clicked();

    void on_spinBox_TileSize_CurrentGPU_MultiGPU_SrmdNCNNVulkan_valueChanged(int arg1);

    void on_checkBox_MultiGPU_RealsrNcnnVulkan_stateChanged(int arg1);

    void on_checkBox_MultiGPU_RealsrNcnnVulkan_clicked();

    void on_comboBox_GPUIDs_MultiGPU_RealsrNcnnVulkan_currentIndexChanged(int index);

    void on_checkBox_isEnable_CurrentGPU_MultiGPU_RealsrNcnnVulkan_clicked();

    void on_spinBox_TileSize_CurrentGPU_MultiGPU_RealsrNcnnVulkan_valueChanged(int arg1);

    void on_checkBox_MultiGPU_RealESRGANNcnnVulkan_stateChanged(int arg1);

    void on_checkBox_MultiGPU_RealESRGANNcnnVulkan_clicked();

    void on_comboBox_GPUIDs_MultiGPU_RealESRGANNcnnVulkan_currentIndexChanged(int index);

    void on_checkBox_isEnable_CurrentGPU_MultiGPU_RealESRGANNcnnVulkan_clicked();

    void on_spinBox_TileSize_CurrentGPU_MultiGPU_RealESRGANNcnnVulkan_valueChanged(int arg1);

    void on_checkBox_MultiGPU_RealCUGANNcnnVulkan_stateChanged(int arg1);

    void on_checkBox_MultiGPU_RealCUGANNcnnVulkan_clicked();

    void on_comboBox_GPUIDs_MultiGPU_RealCUGANNcnnVulkan_currentIndexChanged(int index);

    void on_checkBox_isEnable_CurrentGPU_MultiGPU_RealCUGANNcnnVulkan_clicked();

    void on_spinBox_TileSize_CurrentGPU_MultiGPU_RealCUGANNcnnVulkan_valueChanged(int arg1);

    void on_checkBox_MultiGPU_Waifu2xConverter_clicked();

    void on_checkBox_MultiGPU_Waifu2xConverter_stateChanged(int arg1);

    void on_comboBox_GPUIDs_MultiGPU_Waifu2xConverter_currentIndexChanged(int index);

    void on_checkBox_isEnable_CurrentGPU_MultiGPU_Waifu2xConverter_clicked();

    void on_spinBox_TileSize_CurrentGPU_MultiGPU_Waifu2xConverter_valueChanged(int arg1);

    void on_checkBox_EnableMultiGPU_Waifu2xCaffe_stateChanged(int arg1);

    void on_comboBox_ProcessMode_Waifu2xCaffe_currentIndexChanged(int index);

    void on_lineEdit_GPUs_Anime4k_editingFinished();

    void on_lineEdit_MultiGPUInfo_Waifu2xCaffe_editingFinished();

    void on_pushButton_VerifyGPUsConfig_Anime4k_clicked();

    void on_pushButton_VerifyGPUsConfig_Waifu2xCaffe_clicked();

    void on_tableView_image_doubleClicked(const QModelIndex &index);

    void on_tableView_gif_doubleClicked(const QModelIndex &index);

    void on_tableView_video_doubleClicked(const QModelIndex &index);

    void on_pushButton_ShowMultiGPUSettings_Waifu2xNCNNVulkan_clicked();

    void on_pushButton_ShowMultiGPUSettings_Waifu2xConverter_clicked();

    void on_pushButton_ShowMultiGPUSettings_SrmdNCNNVulkan_clicked();

    void on_pushButton_ShowMultiGPUSettings_RealsrNcnnVulkan_clicked();

    void on_pushButton_ShowMultiGPUSettings_RealESRGANNcnnVulkan_clicked();

    void on_pushButton_ShowMultiGPUSettings_RealCUGANNcnnVulkan_clicked();

    void on_checkBox_HDNMode_Anime4k_stateChanged(int arg1);

    void on_tableView_image_pressed(const QModelIndex &index);

    void on_tableView_gif_pressed(const QModelIndex &index);

    void on_tableView_video_pressed(const QModelIndex &index);

    void on_checkBox_ReplaceOriginalFile_stateChanged(int arg1);

    void on_comboBox_ImageSaveFormat_currentIndexChanged(int index);

    void on_pushButton_ResizeFilesListSplitter_clicked();

    void on_pushButton_TileSize_Add_W2xNCNNVulkan_clicked();

    void on_pushButton_TileSize_Minus_W2xNCNNVulkan_clicked();

    void on_pushButton_BlockSize_Add_W2xConverter_clicked();

    void on_pushButton_BlockSize_Minus_W2xConverter_clicked();

    void on_pushButton_Add_TileSize_SrmdNCNNVulkan_clicked();

    void on_pushButton_Minus_TileSize_SrmdNCNNVulkan_clicked();

    void on_pushButton_Add_TileSize_RealsrNCNNVulkan_clicked();

    void on_pushButton_Minus_TileSize_RealsrNCNNVulkan_clicked();

    void on_pushButton_Add_TileSize_RealESRGANNCNNVulkan_clicked();

    void on_pushButton_Minus_TileSize_RealESRGANNCNNVulkan_clicked();

    void on_pushButton_Add_TileSize_RealCUGANNCNNVulkan_clicked();

    void on_pushButton_Minus_TileSize_RealCUGANNCNNVulkan_clicked();

    void on_comboBox_GPGPUModel_A4k_currentIndexChanged(int index);

    void on_checkBox_DisableGPU_converter_stateChanged(int arg1);

    void on_groupBox_video_settings_clicked();

    void on_pushButton_view_textbrowser_clicked();

signals:
    void Send_Table_EnableSorting(bool EnableSorting);

    void Send_Add_progressBar_CompatibilityTest(); // 进度+1 -兼容性测试进度条

    void Send_SetEnable_pushButton_ForceRetry_self();

    void Send_SystemTray_NewMessage(QString message);

    void Send_PrograssBar_Range_min_max(int, int);
    void Send_progressbar_Add();

    void Send_Table_image_ChangeStatus_rowNumInt_statusQString(int, QString);
    void Send_Table_gif_ChangeStatus_rowNumInt_statusQString(int, QString);
    void Send_Table_video_ChangeStatus_rowNumInt_statusQString(int, QString);

    void Send_Waifu2x_Finished();
    void Send_Waifu2x_Finished_manual();

    void Send_TextBrowser_NewMessage(QString);

    void Send_Waifu2x_Compatibility_Test_finished();

    void Send_Waifu2x_DetectGPU_finished();

    void Send_Realsr_ncnn_vulkan_DetectGPU_finished();

    void Send_RealESRGAN_ncnn_vulkan_DetectGPU_finished();

    void Send_RealCUGAN_ncnn_vulkan_DetectGPU_finished();

    void Send_Table_FileCount_reload();

    void Send_Table_image_insert_fileName_fullPath(QString fileName, QString SourceFile_fullPath);
    void Send_Table_gif_insert_fileName_fullPath(QString fileName, QString SourceFile_fullPath);
    void Send_Table_video_insert_fileName_fullPath(QString fileName, QString SourceFile_fullPath);

    void Send_Table_image_CustRes_rowNumInt_HeightQString_WidthQString(int rowNum, QString height, QString width);
    void Send_Table_gif_CustRes_rowNumInt_HeightQString_WidthQString(int rowNum, QString height, QString width);
    void Send_Table_video_CustRes_rowNumInt_HeightQString_WidthQString(int rowNum, QString height, QString width);

    void Send_Table_Read_Saved_Table_Filelist_Finished();
    void Send_Table_Save_Current_Table_Filelist_Finished();

    void Send_Waifu2x_DumpProcessorList_converter_finished();

    void Send_Read_urls_finfished();

    void Send_SRMD_DetectGPU_finished();

    void Send_video_write_VideoConfiguration(QString VideoConfiguration_fullPath, int ScaleRatio, int DenoiseLevel, bool CustRes_isEnabled, int CustRes_height, int CustRes_width, QString EngineName, bool isProcessBySegment, QString VideoClipsFolderPath, QString VideoClipsFolderName);

    void Send_Settings_Save();

    void Send_video_write_Progress_ProcessBySegment(QString VideoConfiguration_fullPath, int StartTime, bool isSplitComplete, bool isScaleComplete, int OLDSegmentDuration);

    //================== 处理当前文件的进度 =========================
    void Send_CurrentFileProgress_Start(QString FileName, int FrameNum);
    void Send_CurrentFileProgress_Stop();
    void Send_CurrentFileProgress_progressbar_Add();
    void Send_CurrentFileProgress_progressbar_Add_SegmentDuration(int SegmentDuration);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
