
#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
保存设置
删除原设置文件,保存设置
*/
int MainWindow::Settings_Save()
{
    QString settings_ini = Current_Path + "/settings.ini";
    QFile::remove(settings_ini);
    //=================
    QSettings *configIniWrite = new QSettings(settings_ini, QSettings::IniFormat);
    configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));
    //================= 添加警告 =========================
    configIniWrite->setValue("/Warning/Edit", "Carefully!!!");
    //=======  存储放大值和降噪值  =================================
    configIniWrite->setValue("/settings/ImageScaleRatio", ui->spinBox_ScaleRatio_image->value());
    configIniWrite->setValue("/settings/GIFScaleRatio", ui->spinBox_ScaleRatio_gif->value());
    configIniWrite->setValue("/settings/VideoScaleRatio", ui->spinBox_ScaleRatio_video->value());
    configIniWrite->setValue("/settings/ImageDenoiseLevel", ui->spinBox_DenoiseLevel_image->value());
    configIniWrite->setValue("/settings/GIFDenoiseLevel", ui->spinBox_DenoiseLevel_gif->value());
    configIniWrite->setValue("/settings/VideoDenoiseLevel", ui->spinBox_DenoiseLevel_video->value());
    //============ 存储自定义宽度和高度及设置 ============================
    configIniWrite->setValue("/settings/CustResWidth", ui->spinBox_CustRes_width->value());
    configIniWrite->setValue("/settings/CustResHeight", ui->spinBox_CustRes_height->value());
    configIniWrite->setValue("/settings/CustResAspectRatioMode", ui->comboBox_AspectRatio_custRes->currentIndex());
    //============ 存储线程数量 ====================================
    configIniWrite->setValue("/settings/ImageThreadNum", ui->spinBox_ThreadNum_image->value());
    configIniWrite->setValue("/settings/GIFThreadNumInternal", ui->spinBox_ThreadNum_gif_internal->value());
    configIniWrite->setValue("/settings/VideoThreadNumInternal", ui->spinBox_ThreadNum_video_internal->value());
    //================== 存储引擎设置 =========================
    configIniWrite->setValue("/settings/ImageEngine", ui->comboBox_Engine_Image->currentIndex());
    configIniWrite->setValue("/settings/GIFEngine", ui->comboBox_Engine_GIF->currentIndex());
    configIniWrite->setValue("/settings/VideoEngine", ui->comboBox_Engine_Video->currentIndex());
    configIniWrite->setValue("/settings/ImageStyle", ui->comboBox_ImageStyle->currentIndex());
    configIniWrite->setValue("/settings/ModelVulkan", ui->comboBox_model_vulkan->currentIndex());
    configIniWrite->setValue("/settings/TileSize", ui->spinBox_TileSize->value());
    configIniWrite->setValue("/settings/BlockSizeConverter", ui->spinBox_BlockSize_converter->value());
    configIniWrite->setValue("/settings/DisableGPUConverter", ui->checkBox_DisableGPU_converter->isChecked());
    configIniWrite->setValue("/settings/ForceOpenCLConverter", ui->checkBox_ForceOpenCL_converter->isChecked());
    configIniWrite->setValue("/settings/TTAVulkan", ui->checkBox_TTA_vulkan->isChecked());
    configIniWrite->setValue("/settings/TTAConverter", ui->checkBox_TTA_converter->isChecked());
    configIniWrite->setValue("/settings/TTA_SRMD", ui->checkBox_TTA_srmd->isChecked());
    configIniWrite->setValue("/settings/TileSize_SRMD", ui->spinBox_TileSize_srmd->value());
    configIniWrite->setValue("/settings/Version_Waifu2xNCNNVulkan", ui->comboBox_version_Waifu2xNCNNVulkan->currentIndex());
    //===
    configIniWrite->setValue("/settings/TTA_Waifu2xCaffe", ui->checkBox_TTA_Waifu2xCaffe->isChecked());
    configIniWrite->setValue("/settings/Model_2D_Waifu2xCaffe", ui->comboBox_Model_2D_Waifu2xCaffe->currentIndex());
    configIniWrite->setValue("/settings/Model_3D_Waifu2xCaffe", ui->comboBox_Model_3D_Waifu2xCaffe->currentIndex());
    configIniWrite->setValue("/settings/ProcessMode_Waifu2xCaffe", ui->comboBox_ProcessMode_Waifu2xCaffe->currentIndex());
    configIniWrite->setValue("/settings/BatchSize_Waifu2xCaffe", ui->spinBox_BatchSize_Waifu2xCaffe->value());
    configIniWrite->setValue("/settings/GPUID_Waifu2xCaffe", ui->spinBox_GPUID_Waifu2xCaffe->value());
    configIniWrite->setValue("/settings/SplitSize_Waifu2xCaffe", ui->spinBox_SplitSize_Waifu2xCaffe->value());
    configIniWrite->setValue("/settings/checkBox_EnableMultiGPU_Waifu2xCaffe", ui->checkBox_EnableMultiGPU_Waifu2xCaffe->isChecked());
    configIniWrite->setValue("/settings/lineEdit_MultiGPUInfo_Waifu2xCaffe", ui->lineEdit_MultiGPUInfo_Waifu2xCaffe->text());
    //===
    configIniWrite->setValue("/settings/checkBox_TTA_RealsrNCNNVulkan", ui->checkBox_TTA_RealsrNCNNVulkan->isChecked());
    configIniWrite->setValue("/settings/comboBox_Model_RealsrNCNNVulkan", ui->comboBox_Model_RealsrNCNNVulkan->currentIndex());
    configIniWrite->setValue("/settings/spinBox_TileSize_RealsrNCNNVulkan", ui->spinBox_TileSize_RealsrNCNNVulkan->value());
    //===
    configIniWrite->setValue("/settings/checkBox_TTA_RealESRGANNCNNVulkan", ui->checkBox_TTA_RealESRGANNCNNVulkan->isChecked());
    configIniWrite->setValue("/settings/comboBox_Model_RealESRGANNCNNVulkan", ui->comboBox_Model_RealESRGANNCNNVulkan->currentIndex());
    configIniWrite->setValue("/settings/spinBox_TileSize_RealESRGANNCNNVulkan", ui->spinBox_TileSize_RealESRGANNCNNVulkan->value());
    //===
    configIniWrite->setValue("/settings/checkBox_TTA_RealCUGANNCNNVulkan", ui->checkBox_TTA_RealCUGANNCNNVulkan->isChecked());
    configIniWrite->setValue("/settings/comboBox_Model_RealCUGANNCNNVulkan", ui->comboBox_Model_RealCUGANNCNNVulkan->currentIndex());
    configIniWrite->setValue("/settings/spinBox_TileSize_RealCUGANNCNNVulkan", ui->spinBox_TileSize_RealCUGANNCNNVulkan->value());
    // GPU ID List
    configIniWrite->setValue("/settings/CurrentGPUID_Waifu2xNCNNVulkan", ui->comboBox_GPUID->currentIndex());
    configIniWrite->setValue("/settings/Available_GPUID_Waifu2xNCNNVulkan", intoReadable(Available_GPUID));
    configIniWrite->setValue("/settings/GPUIDs_List_MultiGPU_Waifu2xNCNNVulkan", intoReadable(GPUIDs_List_MultiGPU_Waifu2xNCNNVulkan));
    configIniWrite->setValue("/settings/checkBox_MultiGPU_Waifu2xNCNNVulkan", ui->checkBox_MultiGPU_Waifu2xNCNNVulkan->isChecked());
    //==
    configIniWrite->setValue("/settings/comboBox_GPUID_RealsrNCNNVulkan", ui->comboBox_GPUID_RealsrNCNNVulkan->currentIndex());
    configIniWrite->setValue("/settings/Available_GPUID_Realsr_ncnn_vulkan", intoReadable(Available_GPUID_Realsr_ncnn_vulkan));
    configIniWrite->setValue("/settings/GPUIDs_List_MultiGPU_RealsrNcnnVulkan", intoReadable(GPUIDs_List_MultiGPU_RealsrNcnnVulkan));
    configIniWrite->setValue("/settings/checkBox_MultiGPU_RealsrNcnnVulkan", ui->checkBox_MultiGPU_RealsrNcnnVulkan->isChecked());
    //==
    configIniWrite->setValue("/settings/comboBox_GPUID_RealESRGANNCNNVulkan", ui->comboBox_GPUID_RealESRGANNCNNVulkan->currentIndex());
    configIniWrite->setValue("/settings/Available_GPUID_RealESRGAN_ncnn_vulkan", intoReadable(Available_GPUID_RealESRGAN_ncnn_vulkan));
    configIniWrite->setValue("/settings/GPUIDs_List_MultiGPU_RealESRGANNcnnVulkan", intoReadable(GPUIDs_List_MultiGPU_RealESRGANNcnnVulkan));
    configIniWrite->setValue("/settings/checkBox_MultiGPU_RealESRGANNcnnVulkan", ui->checkBox_MultiGPU_RealESRGANNcnnVulkan->isChecked());
    //==
    configIniWrite->setValue("/settings/comboBox_GPUID_RealCUGANNCNNVulkan", ui->comboBox_GPUID_RealCUGANNCNNVulkan->currentIndex());
    configIniWrite->setValue("/settings/Available_GPUID_RealCUGAN_ncnn_vulkan", intoReadable(Available_GPUID_RealCUGAN_ncnn_vulkan));
    configIniWrite->setValue("/settings/GPUIDs_List_MultiGPU_RealCUGANNcnnVulkan", intoReadable(GPUIDs_List_MultiGPU_RealCUGANNcnnVulkan));
    configIniWrite->setValue("/settings/checkBox_MultiGPU_RealCUGANNcnnVulkan", ui->checkBox_MultiGPU_RealCUGANNcnnVulkan->isChecked());
    //==
    configIniWrite->setValue("/settings/comboBox_TargetProcessor_converter", ui->comboBox_TargetProcessor_converter->currentIndex());
    configIniWrite->setValue("/settings/Available_ProcessorList_converter", intoReadable(Available_ProcessorList_converter));
    configIniWrite->setValue("/settings/GPUIDs_List_MultiGPU_Waifu2xConverter", intoReadable(GPUIDs_List_MultiGPU_Waifu2xConverter));
    configIniWrite->setValue("/settings/checkBox_MultiGPU_Waifu2xConverter", ui->checkBox_MultiGPU_Waifu2xConverter->isChecked());
    //==
    configIniWrite->setValue("/settings/comboBox_GPUID_srmd", ui->comboBox_GPUID_srmd->currentIndex());
    configIniWrite->setValue("/settings/Available_GPUID_srmd", intoReadable(Available_GPUID_srmd));
    configIniWrite->setValue("/settings/GPUIDs_List_MultiGPU_SrmdNcnnVulkan", intoReadable(GPUIDs_List_MultiGPU_SrmdNcnnVulkan));
    configIniWrite->setValue("/settings/checkBox_MultiGPU_SrmdNCNNVulkan", ui->checkBox_MultiGPU_SrmdNCNNVulkan->isChecked());
    //================== 存储 扩展名 =================================
    configIniWrite->setValue("/settings/ImageEXT", ui->Ext_image->text());
    configIniWrite->setValue("/settings/VideoEXT", ui->Ext_video->text());
    //=================== 存储 杂项设置 =================================
    configIniWrite->setValue("/settings/checkBox_AutoSkip_CustomRes", ui->checkBox_AutoSkip_CustomRes->isChecked());
    configIniWrite->setValue("/settings/checkBox_AlwaysPreProcessAlphaPNG", ui->checkBox_AlwaysPreProcessAlphaPNG->isChecked());
    configIniWrite->setValue("/settings/spinBox_ImageQualityLevel", ui->spinBox_ImageQualityLevel->value());
    configIniWrite->setValue("/settings/comboBox_ImageSaveFormat", ui->comboBox_ImageSaveFormat->currentIndex());
    configIniWrite->setValue("/settings/checkBox_KeepParentFolder", ui->checkBox_KeepParentFolder->isChecked());
    configIniWrite->setValue("/settings/checkBox_MinimizeToTaskbar", ui->checkBox_MinimizeToTaskbar->isChecked());
    configIniWrite->setValue("/settings/checkBox_custres_isAll", ui->checkBox_custres_isAll->isChecked());
    configIniWrite->setValue("/settings/DelOriginal", ui->checkBox_DelOriginal->isChecked());
    configIniWrite->setValue("/settings/OptGIF", ui->checkBox_OptGIF->isChecked());
    configIniWrite->setValue("/settings/ReProFinFiles", ui->checkBox_ReProcFinFiles->isChecked());
    configIniWrite->setValue("/settings/ShowInterPro", ui->checkBox_ShowInterPro->isChecked());
    configIniWrite->setValue("/settings/FileListAutoScroll", ui->checkBox_FileListAutoSlide->isChecked());
    configIniWrite->setValue("/settings/AutoSaveSettings", ui->checkBox_AutoSaveSettings->isChecked());
    configIniWrite->setValue("/settings/AlwaysHideSettings", ui->checkBox_AlwaysHideSettings->isChecked());
    configIniWrite->setValue("/settings/AlwaysHideTextBrowser", ui->checkBox_AlwaysHideTextBrowser->isChecked());
    configIniWrite->setValue("/settings/ScanSubFolders", ui->checkBox_ScanSubFolders->isChecked());
    configIniWrite->setValue("/settings/InteractiveFileList", ui->checkBox_FileList_Interactive->isChecked());
    configIniWrite->setValue("/settings/RetryTimes", ui->spinBox_retry->value());
    configIniWrite->setValue("/settings/AutoDetectAlphaChannel", ui->checkBox_AutoDetectAlphaChannel->isChecked());
    configIniWrite->setValue("/settings/PromptWhenExit", ui->checkBox_PromptWhenExit->isChecked());
    configIniWrite->setValue("/settings/KeepVideoCache", ui->checkBox_KeepVideoCache->isChecked());
    configIniWrite->setValue("/settings/checkBox_ReplaceOriginalFile", ui->checkBox_ReplaceOriginalFile->isChecked());
    //===
    configIniWrite->setValue("/settings/QAction_checkBox_MoveToRecycleBin_checkBox_ReplaceOriginalFile", QAction_checkBox_MoveToRecycleBin_checkBox_ReplaceOriginalFile->isChecked());
    //===
    configIniWrite->setValue("/settings/QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal", QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->isChecked());
    //===
    configIniWrite->setValue("/settings/ProcessVideoBySegment", ui->checkBox_ProcessVideoBySegment->isChecked());
    configIniWrite->setValue("/settings/SegmentDuration", ui->spinBox_SegmentDuration->value());
    //=====
    configIniWrite->setValue("/settings/AudioDenoise", ui->checkBox_AudioDenoise->isChecked());
    configIniWrite->setValue("/settings/AudioDenoiseLevel", ui->doubleSpinBox_AudioDenoiseLevel->value());
    //=====
    configIniWrite->setValue("/settings/checkBox_PreProcessImage", ui->checkBox_PreProcessImage->isChecked());
    //=================== 存储视频设置 ===========================
    configIniWrite->setValue("/settings/VideoSettingsIsEnabled", ui->groupBox_video_settings->isChecked());
    configIniWrite->setValue("/settings/EncoderVideo", ui->lineEdit_encoder_vid->text());
    configIniWrite->setValue("/settings/EncoderAudio", ui->lineEdit_encoder_audio->text());
    configIniWrite->setValue("/settings/PixelFormat", ui->lineEdit_pixformat->text());
    configIniWrite->setValue("/settings/BitrateVideo", ui->spinBox_bitrate_vid->value());
    configIniWrite->setValue("/settings/BitrateAudio", ui->spinBox_bitrate_audio->value());
    configIniWrite->setValue("/settings/ExtraCommandOutput", ui->lineEdit_ExCommand_output->text());
    //===
    configIniWrite->setValue("/settings/BitrateVideo2mp4", ui->spinBox_bitrate_vid_2mp4->value());
    configIniWrite->setValue("/settings/BitrateAudio2mp4", ui->spinBox_bitrate_audio_2mp4->value());
    configIniWrite->setValue("/settings/vcodecCopy", ui->checkBox_vcodec_copy_2mp4->isChecked());
    configIniWrite->setValue("/settings/acodecCopy", ui->checkBox_acodec_copy_2mp4->isChecked());
    configIniWrite->setValue("/settings/ExtraCommand2mp4", ui->lineEdit_ExCommand_2mp4->text());
    //==================== 存储输出路径设置 ========================
    configIniWrite->setValue("/settings/OutPutPath", ui->lineEdit_outputPath->text());
    configIniWrite->setValue("/settings/OutPutPathIsEnabled", ui->checkBox_OutPath_isEnabled->isChecked());
    configIniWrite->setValue("/settings/checkBox_OutPath_KeepOriginalFileName", ui->checkBox_OutPath_KeepOriginalFileName->isChecked());
    configIniWrite->setValue("/settings/checkBox_OutPath_Overwrite", ui->checkBox_OutPath_Overwrite->isChecked());
    //=================== 存储Anime4k设置 =============================
    configIniWrite->setValue("/settings/spinBox_OpenCLCommandQueues_A4k", ui->spinBox_OpenCLCommandQueues_A4k->value());
    configIniWrite->setValue("/settings/checkBox_OpenCLParallelIO_A4k", ui->checkBox_OpenCLParallelIO_A4k->isChecked());
    configIniWrite->setValue("/settings/comboBox_GPGPUModel_A4k", ui->comboBox_GPGPUModel_A4k->currentIndex());
    configIniWrite->setValue("/settings/checkBox_HDNMode_Anime4k", ui->checkBox_HDNMode_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_FastMode_Anime4K", ui->checkBox_FastMode_Anime4K->isChecked());
    configIniWrite->setValue("/settings/checkBox_ACNet_Anime4K", ui->checkBox_ACNet_Anime4K->isChecked());
    configIniWrite->setValue("/settings/checkBox_GPUMode_Anime4K", ui->checkBox_GPUMode_Anime4K->isChecked());
    configIniWrite->setValue("/settings/spinBox_Passes_Anime4K", ui->spinBox_Passes_Anime4K->value());
    configIniWrite->setValue("/settings/spinBox_PushColorCount_Anime4K", ui->spinBox_PushColorCount_Anime4K->value());
    configIniWrite->setValue("/settings/doubleSpinBox_PushColorStrength_Anime4K", ui->doubleSpinBox_PushColorStrength_Anime4K->value());
    configIniWrite->setValue("/settings/doubleSpinBox_PushGradientStrength_Anime4K", ui->doubleSpinBox_PushGradientStrength_Anime4K->value());
    configIniWrite->setValue("/settings/checkBox_SpecifyGPU_Anime4k", ui->checkBox_SpecifyGPU_Anime4k->isChecked());
    configIniWrite->setValue("/settings/lineEdit_GPUs_Anime4k", ui->lineEdit_GPUs_Anime4k->text());
    // Pre-Processing
    configIniWrite->setValue("/settings/checkBox_EnablePreProcessing_Anime4k", ui->checkBox_EnablePreProcessing_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_MedianBlur_Pre_Anime4k", ui->checkBox_MedianBlur_Pre_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_MeanBlur_Pre_Anime4k", ui->checkBox_MeanBlur_Pre_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_CASSharping_Pre_Anime4k", ui->checkBox_CASSharping_Pre_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_GaussianBlurWeak_Pre_Anime4k", ui->checkBox_GaussianBlurWeak_Pre_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_GaussianBlur_Pre_Anime4k", ui->checkBox_GaussianBlur_Pre_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_BilateralFilter_Pre_Anime4k", ui->checkBox_BilateralFilter_Pre_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_BilateralFilterFaster_Pre_Anime4k", ui->checkBox_BilateralFilterFaster_Pre_Anime4k->isChecked());
    // Post-Processing
    configIniWrite->setValue("/settings/checkBox_EnablePostProcessing_Anime4k", ui->checkBox_EnablePostProcessing_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_MedianBlur_Post_Anime4k", ui->checkBox_MedianBlur_Post_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_MeanBlur_Post_Anime4k", ui->checkBox_MeanBlur_Post_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_CASSharping_Post_Anime4k", ui->checkBox_CASSharping_Post_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_GaussianBlurWeak_Post_Anime4k", ui->checkBox_GaussianBlurWeak_Post_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_GaussianBlur_Post_Anime4k", ui->checkBox_GaussianBlur_Post_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_BilateralFilter_Post_Anime4k", ui->checkBox_BilateralFilter_Post_Anime4k->isChecked());
    configIniWrite->setValue("/settings/checkBox_BilateralFilterFaster_Post_Anime4k", ui->checkBox_BilateralFilterFaster_Post_Anime4k->isChecked());
    //========================= 存储兼容性测试结果 ================
    configIniWrite->setValue("/settings/checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW", ui->checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW_FP16P", ui->checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW_FP16P->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_Waifu2x_Converter", ui->checkBox_isCompatible_Waifu2x_Converter->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_SRMD_NCNN_Vulkan", ui->checkBox_isCompatible_SRMD_NCNN_Vulkan->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_Anime4k_CPU", ui->checkBox_isCompatible_Anime4k_CPU->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_Anime4k_GPU", ui->checkBox_isCompatible_Anime4k_GPU->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_FFmpeg", ui->checkBox_isCompatible_FFmpeg->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_FFprobe", ui->checkBox_isCompatible_FFprobe->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_ImageMagick", ui->checkBox_isCompatible_ImageMagick->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_Gifsicle", ui->checkBox_isCompatible_Gifsicle->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_SoX", ui->checkBox_isCompatible_SoX->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_Waifu2x_Caffe_CPU", ui->checkBox_isCompatible_Waifu2x_Caffe_CPU->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_Waifu2x_Caffe_GPU", ui->checkBox_isCompatible_Waifu2x_Caffe_GPU->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_Waifu2x_Caffe_cuDNN", ui->checkBox_isCompatible_Waifu2x_Caffe_cuDNN->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_Realsr_NCNN_Vulkan", ui->checkBox_isCompatible_Realsr_NCNN_Vulkan->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_RealESRGAN_NCNN_Vulkan", ui->checkBox_isCompatible_RealESRGAN_NCNN_Vulkan->isChecked());
    configIniWrite->setValue("/settings/checkBox_isCompatible_RealCUGAN_NCNN_Vulkan", ui->checkBox_isCompatible_RealCUGAN_NCNN_Vulkan->isChecked());
    //========
    delete configIniWrite;
    return 0;
}
/*
读取&应用设置
如果设置文件不存在则生成默认设置文件,读取设置,应用设置
*/
int MainWindow::Settings_Read_Apply()
{
    QString settings_ini = Current_Path + "/settings.ini";
    if (!QFile::exists(settings_ini))
    {
        QAction_checkBox_MoveToRecycleBin_checkBox_ReplaceOriginalFile->setChecked(1);
        QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->setChecked(1);
        Settings_Save();
        Settings_Read_Apply();
        return 0;
    }
    //=================
    QSettings *configIniRead = new QSettings(settings_ini, QSettings::IniFormat);
    configIniRead->setIniCodec(QTextCodec::codecForName("UTF-8"));
    //=======  加载放大值和降噪值  ======
    ui->spinBox_ScaleRatio_image->setValue(Settings_Read_value("/settings/ImageScaleRatio").toInt());
    ui->spinBox_ScaleRatio_gif->setValue(Settings_Read_value("/settings/GIFScaleRatio").toInt());
    ui->spinBox_ScaleRatio_video->setValue(Settings_Read_value("/settings/VideoScaleRatio").toInt());
    //============= 加载自定义宽度和高度 ============================
    ui->spinBox_CustRes_width->setValue(Settings_Read_value("/settings/CustResWidth").toInt());
    ui->spinBox_CustRes_height->setValue(Settings_Read_value("/settings/CustResHeight").toInt());
    ui->comboBox_AspectRatio_custRes->setCurrentIndex(Settings_Read_value("/settings/CustResAspectRatioMode").toInt());
    //============ 加载 线程数量 ==================================
    ui->spinBox_ThreadNum_image->setValue(Settings_Read_value("/settings/ImageThreadNum").toInt());
    ui->spinBox_ThreadNum_gif_internal->setValue(Settings_Read_value("/settings/GIFThreadNumInternal").toInt());
    ui->spinBox_ThreadNum_video_internal->setValue(Settings_Read_value("/settings/VideoThreadNumInternal").toInt());
    //================ 加载 引擎设置 ================================
    isShowAnime4kWarning = false;
    ui->comboBox_Engine_Image->setCurrentIndex(Settings_Read_value("/settings/ImageEngine").toInt());
    ui->comboBox_Engine_GIF->setCurrentIndex(Settings_Read_value("/settings/GIFEngine").toInt());
    ui->comboBox_Engine_Video->setCurrentIndex(Settings_Read_value("/settings/VideoEngine").toInt());
    ui->comboBox_ImageStyle->setCurrentIndex(Settings_Read_value("/settings/ImageStyle").toInt());
    ui->comboBox_model_vulkan->setCurrentIndex(Settings_Read_value("/settings/ModelVulkan").toInt());
    ui->spinBox_TileSize->setValue(Settings_Read_value("/settings/TileSize").toInt());
    ui->spinBox_BlockSize_converter->setValue(Settings_Read_value("/settings/BlockSizeConverter").toInt());
    ui->checkBox_DisableGPU_converter->setChecked(Settings_Read_value("/settings/DisableGPUConverter").toBool());
    ui->checkBox_ForceOpenCL_converter->setChecked(Settings_Read_value("/settings/ForceOpenCLConverter").toBool());
    ui->checkBox_TTA_vulkan->setChecked(Settings_Read_value("/settings/TTAVulkan").toBool());
    ui->checkBox_TTA_converter->setChecked(Settings_Read_value("/settings/TTAConverter").toBool());
    ui->checkBox_TTA_srmd->setChecked(Settings_Read_value("/settings/TTA_SRMD").toBool());
    ui->spinBox_TileSize_srmd->setValue(Settings_Read_value("/settings/TileSize_SRMD").toInt());
    ui->comboBox_version_Waifu2xNCNNVulkan->setCurrentIndex(Settings_Read_value("/settings/Version_Waifu2xNCNNVulkan").toInt());
    //===
    ui->checkBox_TTA_RealsrNCNNVulkan->setChecked(Settings_Read_value("/settings/checkBox_TTA_RealsrNCNNVulkan").toBool());
    ui->comboBox_Model_RealsrNCNNVulkan->setCurrentIndex(Settings_Read_value("/settings/comboBox_Model_RealsrNCNNVulkan").toInt());
    ui->spinBox_TileSize_RealsrNCNNVulkan->setValue(Settings_Read_value("/settings/spinBox_TileSize_RealsrNCNNVulkan").toInt());
    //===
    ui->checkBox_TTA_RealESRGANNCNNVulkan->setChecked(Settings_Read_value("/settings/checkBox_TTA_RealESRGANNCNNVulkan").toBool());
    ui->comboBox_Model_RealESRGANNCNNVulkan->setCurrentIndex(Settings_Read_value("/settings/comboBox_Model_RealESRGANNCNNVulkan").toInt());
    ui->spinBox_TileSize_RealESRGANNCNNVulkan->setValue(Settings_Read_value("/settings/spinBox_TileSize_RealESRGANNCNNVulkan").toInt());
    //===
    ui->checkBox_TTA_RealCUGANNCNNVulkan->setChecked(Settings_Read_value("/settings/checkBox_TTA_RealCUGANNCNNVulkan").toBool());
    ui->comboBox_Model_RealCUGANNCNNVulkan->setCurrentIndex(Settings_Read_value("/settings/comboBox_Model_RealCUGANNCNNVulkan").toInt());
    ui->spinBox_TileSize_RealCUGANNCNNVulkan->setValue(Settings_Read_value("/settings/spinBox_TileSize_RealCUGANNCNNVulkan").toInt());
    //===
    ui->checkBox_TTA_Waifu2xCaffe->setChecked(Settings_Read_value("/settings/TTA_Waifu2xCaffe").toBool());
    ui->comboBox_Model_2D_Waifu2xCaffe->setCurrentIndex(Settings_Read_value("/settings/Model_2D_Waifu2xCaffe").toInt());
    ui->comboBox_Model_3D_Waifu2xCaffe->setCurrentIndex(Settings_Read_value("/settings/Model_3D_Waifu2xCaffe").toInt());
    ui->comboBox_ProcessMode_Waifu2xCaffe->setCurrentIndex(Settings_Read_value("/settings/ProcessMode_Waifu2xCaffe").toInt());
    ui->spinBox_BatchSize_Waifu2xCaffe->setValue(Settings_Read_value("/settings/BatchSize_Waifu2xCaffe").toInt());
    ui->spinBox_GPUID_Waifu2xCaffe->setValue(Settings_Read_value("/settings/GPUID_Waifu2xCaffe").toInt());
    ui->spinBox_SplitSize_Waifu2xCaffe->setValue(Settings_Read_value("/settings/SplitSize_Waifu2xCaffe").toInt());
    ui->checkBox_EnableMultiGPU_Waifu2xCaffe->setChecked(Settings_Read_value("/settings/checkBox_EnableMultiGPU_Waifu2xCaffe").toBool());
    ui->lineEdit_MultiGPUInfo_Waifu2xCaffe->setText(Settings_Read_value("/settings/lineEdit_MultiGPUInfo_Waifu2xCaffe").toString());
    // GPU ID List
    // Waifu2x-NCNN-Vulkan
    Available_GPUID = Settings_Read_value("/settings/Available_GPUID_Waifu2xNCNNVulkan").toStringList();
    Waifu2x_DetectGPU_finished();
    ui->comboBox_GPUID->setCurrentIndex(Settings_Read_value("/settings/CurrentGPUID_Waifu2xNCNNVulkan").toInt());
    // 读取多显卡设定
    GPUIDs_List_MultiGPU_Waifu2xNCNNVulkan = Settings_Read_value("/settings/GPUIDs_List_MultiGPU_Waifu2xNCNNVulkan").value<QList<QMap<QString, QString>>>();
    if (GPUIDs_List_MultiGPU_Waifu2xNCNNVulkan.isEmpty() == false)
    {
        QMap<QString, QString> GPUInfo_waifu2xNcnnVulkan = GPUIDs_List_MultiGPU_Waifu2xNCNNVulkan.at(ui->comboBox_GPUIDs_MultiGPU_Waifu2xNCNNVulkan->currentIndex());
        ui->checkBox_isEnable_CurrentGPU_MultiGPU_Waifu2xNCNNVulkan->setChecked(GPUInfo_waifu2xNcnnVulkan["isEnabled"] == "true");
        ui->spinBox_TileSize_CurrentGPU_MultiGPU_Waifu2xNCNNVulkan->setValue(GPUInfo_waifu2xNcnnVulkan["TileSize"].toInt());
    }
    ui->checkBox_MultiGPU_Waifu2xNCNNVulkan->setChecked(Settings_Read_value("/settings/checkBox_MultiGPU_Waifu2xNCNNVulkan").toBool());
    // Realsr_ncnn_vulkan
    Available_GPUID_Realsr_ncnn_vulkan = Settings_Read_value("/settings/Available_GPUID_Realsr_ncnn_vulkan").toStringList();
    Realsr_ncnn_vulkan_DetectGPU_finished();
    ui->comboBox_GPUID_RealsrNCNNVulkan->setCurrentIndex(Settings_Read_value("/settings/comboBox_GPUID_RealsrNCNNVulkan").toInt());
    // RealESRGAN_ncnn_vulkan
    Available_GPUID_RealESRGAN_ncnn_vulkan = Settings_Read_value("/settings/Available_GPUID_RealESRGAN_ncnn_vulkan").toStringList();
    RealESRGAN_ncnn_vulkan_DetectGPU_finished();
    ui->comboBox_GPUID_RealESRGANNCNNVulkan->setCurrentIndex(Settings_Read_value("/settings/comboBox_GPUID_RealESRGANNCNNVulkan").toInt());
    // RealCUGAN_ncnn_vulkan
    Available_GPUID_RealCUGAN_ncnn_vulkan = Settings_Read_value("/settings/Available_GPUID_RealCUGAN_ncnn_vulkan").toStringList();
    RealCUGAN_ncnn_vulkan_DetectGPU_finished();
    ui->comboBox_GPUID_RealCUGANNCNNVulkan->setCurrentIndex(Settings_Read_value("/settings/comboBox_GPUID_RealCUGANNCNNVulkan").toInt());
    // 读取多显卡设定
    GPUIDs_List_MultiGPU_RealsrNcnnVulkan = Settings_Read_value("/settings/GPUIDs_List_MultiGPU_RealsrNcnnVulkan").value<QList<QMap<QString, QString>>>();
    if (GPUIDs_List_MultiGPU_RealsrNcnnVulkan.isEmpty() == false)
    {
        QMap<QString, QString> GPUInfo_RealsrNcnnVulkan = GPUIDs_List_MultiGPU_RealsrNcnnVulkan.at(ui->comboBox_GPUIDs_MultiGPU_RealsrNcnnVulkan->currentIndex());
        ui->checkBox_isEnable_CurrentGPU_MultiGPU_RealsrNcnnVulkan->setChecked(GPUInfo_RealsrNcnnVulkan["isEnabled"] == "true");
        ui->spinBox_TileSize_CurrentGPU_MultiGPU_RealsrNcnnVulkan->setValue(GPUInfo_RealsrNcnnVulkan["TileSize"].toInt());
    }
    ui->checkBox_MultiGPU_RealsrNcnnVulkan->setChecked(Settings_Read_value("/settings/checkBox_MultiGPU_RealsrNcnnVulkan").toBool());
    // 读取多显卡设定
    GPUIDs_List_MultiGPU_RealESRGANNcnnVulkan = Settings_Read_value("/settings/GPUIDs_List_MultiGPU_RealESRGANNcnnVulkan").value<QList<QMap<QString, QString>>>();
    if (GPUIDs_List_MultiGPU_RealESRGANNcnnVulkan.isEmpty() == false)
    {
        QMap<QString, QString> GPUInfo_RealESRGANNcnnVulkan = GPUIDs_List_MultiGPU_RealESRGANNcnnVulkan.at(ui->comboBox_GPUIDs_MultiGPU_RealESRGANNcnnVulkan->currentIndex());
        ui->checkBox_isEnable_CurrentGPU_MultiGPU_RealESRGANNcnnVulkan->setChecked(GPUInfo_RealESRGANNcnnVulkan["isEnabled"] == "true");
        ui->spinBox_TileSize_CurrentGPU_MultiGPU_RealESRGANNcnnVulkan->setValue(GPUInfo_RealESRGANNcnnVulkan["TileSize"].toInt());
    }
    ui->checkBox_MultiGPU_RealESRGANNcnnVulkan->setChecked(Settings_Read_value("/settings/checkBox_MultiGPU_RealESRGANNcnnVulkan").toBool());
    // 读取多显卡设定
    GPUIDs_List_MultiGPU_RealCUGANNcnnVulkan = Settings_Read_value("/settings/GPUIDs_List_MultiGPU_RealCUGANNcnnVulkan").value<QList<QMap<QString, QString>>>();
    if (GPUIDs_List_MultiGPU_RealCUGANNcnnVulkan.isEmpty() == false)
    {
        QMap<QString, QString> GPUInfo_RealCUGANNcnnVulkan = GPUIDs_List_MultiGPU_RealCUGANNcnnVulkan.at(ui->comboBox_GPUIDs_MultiGPU_RealCUGANNcnnVulkan->currentIndex());
        ui->checkBox_isEnable_CurrentGPU_MultiGPU_RealCUGANNcnnVulkan->setChecked(GPUInfo_RealCUGANNcnnVulkan["isEnabled"] == "true");
        ui->spinBox_TileSize_CurrentGPU_MultiGPU_RealCUGANNcnnVulkan->setValue(GPUInfo_RealCUGANNcnnVulkan["TileSize"].toInt());
    }
    ui->checkBox_MultiGPU_RealCUGANNcnnVulkan->setChecked(Settings_Read_value("/settings/checkBox_MultiGPU_RealCUGANNcnnVulkan").toBool());
    // Waifu2x-Converter
    Available_ProcessorList_converter = Settings_Read_value("/settings/Available_ProcessorList_converter").toStringList();
    Waifu2x_DumpProcessorList_converter_finished();
    ui->comboBox_TargetProcessor_converter->setCurrentIndex(Settings_Read_value("/settings/comboBox_TargetProcessor_converter").toInt());
    on_comboBox_TargetProcessor_converter_currentIndexChanged(0);
    // 读取多显卡设定
    GPUIDs_List_MultiGPU_Waifu2xConverter = Settings_Read_value("/settings/GPUIDs_List_MultiGPU_Waifu2xConverter").value<QList<QMap<QString, QString>>>();
    if (GPUIDs_List_MultiGPU_Waifu2xConverter.isEmpty() == false)
    {
        QMap<QString, QString> GPUInfo_Waifu2xConverter = GPUIDs_List_MultiGPU_Waifu2xConverter.at(ui->comboBox_GPUIDs_MultiGPU_Waifu2xConverter->currentIndex());
        ui->checkBox_isEnable_CurrentGPU_MultiGPU_Waifu2xConverter->setChecked(GPUInfo_Waifu2xConverter["isEnabled"] == "true");
        ui->spinBox_TileSize_CurrentGPU_MultiGPU_Waifu2xConverter->setValue(GPUInfo_Waifu2xConverter["TileSize"].toInt());
    }
    ui->checkBox_MultiGPU_Waifu2xConverter->setChecked(Settings_Read_value("/settings/checkBox_MultiGPU_Waifu2xConverter").toBool());
    // SRMD-NCNN-Vulkan
    Available_GPUID_srmd = Settings_Read_value("/settings/Available_GPUID_srmd").toStringList();
    SRMD_DetectGPU_finished();
    ui->comboBox_GPUID_srmd->setCurrentIndex(Settings_Read_value("/settings/comboBox_GPUID_srmd").toInt());
    // 读取多显卡设定
    GPUIDs_List_MultiGPU_SrmdNcnnVulkan = Settings_Read_value("/settings/GPUIDs_List_MultiGPU_SrmdNcnnVulkan").value<QList<QMap<QString, QString>>>();
    if (GPUIDs_List_MultiGPU_SrmdNcnnVulkan.isEmpty() == false)
    {
        QMap<QString, QString> GPUInfo_SrmdNcnnVulkan = GPUIDs_List_MultiGPU_SrmdNcnnVulkan.at(ui->comboBox_GPUIDs_MultiGPU_SrmdNCNNVulkan->currentIndex());
        ui->checkBox_isEnable_CurrentGPU_MultiGPU_SrmdNCNNVulkan->setChecked(GPUInfo_SrmdNcnnVulkan["isEnabled"] == "true");
        ui->spinBox_TileSize_CurrentGPU_MultiGPU_SrmdNCNNVulkan->setValue(GPUInfo_SrmdNcnnVulkan["TileSize"].toInt());
    }
    ui->checkBox_MultiGPU_SrmdNCNNVulkan->setChecked(Settings_Read_value("/settings/checkBox_MultiGPU_SrmdNCNNVulkan").toBool());
    //================= 加载 扩展名 ===========================
    ui->Ext_image->setText(Settings_Read_value("/settings/ImageEXT").toString());
    ui->Ext_video->setText(Settings_Read_value("/settings/VideoEXT").toString());
    //================== 加载 杂项设置 ==================================
    ui->checkBox_AutoSkip_CustomRes->setChecked(Settings_Read_value("/settings/checkBox_AutoSkip_CustomRes").toBool());
    ui->checkBox_AlwaysPreProcessAlphaPNG->setChecked(Settings_Read_value("/settings/checkBox_AlwaysPreProcessAlphaPNG").toBool());
    ui->spinBox_ImageQualityLevel->setValue(Settings_Read_value("/settings/spinBox_ImageQualityLevel").toInt());
    ui->comboBox_ImageSaveFormat->setCurrentIndex(Settings_Read_value("/settings/comboBox_ImageSaveFormat").toInt());
    ui->checkBox_KeepParentFolder->setChecked(Settings_Read_value("/settings/checkBox_KeepParentFolder").toBool());
    ui->checkBox_MinimizeToTaskbar->setChecked(Settings_Read_value("/settings/checkBox_MinimizeToTaskbar").toBool());
    ui->checkBox_custres_isAll->setChecked(Settings_Read_value("/settings/checkBox_custres_isAll").toBool());
    ui->checkBox_DelOriginal->setChecked(Settings_Read_value("/settings/DelOriginal").toBool());
    ui->checkBox_OptGIF->setChecked(Settings_Read_value("/settings/OptGIF").toBool());
    ui->checkBox_ReProcFinFiles->setChecked(Settings_Read_value("/settings/ReProFinFiles").toBool());
    ui->checkBox_ShowInterPro->setChecked(Settings_Read_value("/settings/ShowInterPro").toBool());
    ui->checkBox_FileListAutoSlide->setChecked(Settings_Read_value("/settings/FileListAutoScroll").toBool());
    ui->checkBox_AutoSaveSettings->setChecked(Settings_Read_value("/settings/AutoSaveSettings").toBool());
    ui->checkBox_AlwaysHideSettings->setChecked(Settings_Read_value("/settings/AlwaysHideSettings").toBool());
    ui->checkBox_AlwaysHideTextBrowser->setChecked(Settings_Read_value("/settings/AlwaysHideTextBrowser").toBool());
    ui->checkBox_ScanSubFolders->setChecked(Settings_Read_value("/settings/ScanSubFolders").toBool());
    ui->checkBox_FileList_Interactive->setChecked(Settings_Read_value("/settings/InteractiveFileList").toBool());
    ui->spinBox_retry->setValue(Settings_Read_value("/settings/RetryTimes").toInt());
    ui->checkBox_AutoDetectAlphaChannel->setChecked(Settings_Read_value("/settings/AutoDetectAlphaChannel").toBool());
    ui->checkBox_PromptWhenExit->setChecked(Settings_Read_value("/settings/PromptWhenExit").toBool());
    ui->checkBox_KeepVideoCache->setChecked(Settings_Read_value("/settings/KeepVideoCache").toBool());
    ui->checkBox_ReplaceOriginalFile->setChecked(Settings_Read_value("/settings/checkBox_ReplaceOriginalFile").toBool());
    QAction_checkBox_MoveToRecycleBin_checkBox_ReplaceOriginalFile->setChecked(Settings_Read_value("/settings/QAction_checkBox_MoveToRecycleBin_checkBox_ReplaceOriginalFile").toBool());
    QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->setChecked(Settings_Read_value("/settings/QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal").toBool());
    //===
    ui->checkBox_ProcessVideoBySegment->setChecked(Settings_Read_value("/settings/ProcessVideoBySegment").toBool());
    ui->spinBox_SegmentDuration->setValue(Settings_Read_value("/settings/SegmentDuration").toInt());
    //=========
    ui->checkBox_AudioDenoise->setChecked(Settings_Read_value("/settings/AudioDenoise").toBool());
    ui->doubleSpinBox_AudioDenoiseLevel->setValue(Settings_Read_value("/settings/AudioDenoiseLevel").toDouble());
    //=========
    ui->checkBox_PreProcessImage->setChecked(Settings_Read_value("/settings/checkBox_PreProcessImage").toBool());
    //=================== 加载视频设置 ===========================
    ui->groupBox_video_settings->setChecked(Settings_Read_value("/settings/VideoSettingsIsEnabled").toBool());
    //===
    ui->lineEdit_encoder_vid->setText(Settings_Read_value("/settings/EncoderVideo").toString());
    ui->lineEdit_encoder_audio->setText(Settings_Read_value("/settings/EncoderAudio").toString());
    ui->lineEdit_pixformat->setText(Settings_Read_value("/settings/PixelFormat").toString());
    ui->spinBox_bitrate_vid->setValue(Settings_Read_value("/settings/BitrateVideo").toInt());
    ui->spinBox_bitrate_audio->setValue(Settings_Read_value("/settings/BitrateAudio").toInt());
    ui->lineEdit_ExCommand_output->setText(Settings_Read_value("/settings/ExtraCommandOutput").toString());
    //===
    ui->spinBox_bitrate_vid_2mp4->setValue(Settings_Read_value("/settings/BitrateVideo2mp4").toInt());
    ui->spinBox_bitrate_audio_2mp4->setValue(Settings_Read_value("/settings/BitrateAudio2mp4").toInt());
    ui->checkBox_vcodec_copy_2mp4->setChecked(Settings_Read_value("/settings/vcodecCopy").toBool());
    ui->checkBox_acodec_copy_2mp4->setChecked(Settings_Read_value("/settings/acodecCopy").toBool());
    ui->lineEdit_ExCommand_2mp4->setText(Settings_Read_value("/settings/ExtraCommand2mp4").toString());
    //=============== 加载输出路径设置 ===========================
    ui->lineEdit_outputPath->setText(Settings_Read_value("/settings/OutPutPath").toString());
    ui->checkBox_OutPath_isEnabled->setChecked(Settings_Read_value("/settings/OutPutPathIsEnabled").toBool());
    ui->checkBox_OutPath_KeepOriginalFileName->setChecked(Settings_Read_value("/settings/checkBox_OutPath_KeepOriginalFileName").toBool());
    ui->checkBox_OutPath_Overwrite->setChecked(Settings_Read_value("/settings/checkBox_OutPath_Overwrite").toBool());
    //================== 加载Anime4k设置 ===================================
    ui->spinBox_OpenCLCommandQueues_A4k->setValue(Settings_Read_value("/settings/spinBox_OpenCLCommandQueues_A4k").toInt());
    ui->checkBox_OpenCLParallelIO_A4k->setChecked(Settings_Read_value("/settings/checkBox_OpenCLParallelIO_A4k").toBool());
    ui->comboBox_GPGPUModel_A4k->setCurrentIndex(Settings_Read_value("/settings/comboBox_GPGPUModel_A4k").toInt());
    ui->checkBox_HDNMode_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_HDNMode_Anime4k").toBool());
    ui->checkBox_FastMode_Anime4K->setChecked(Settings_Read_value("/settings/checkBox_FastMode_Anime4K").toBool());
    ui->checkBox_ACNet_Anime4K->setChecked(Settings_Read_value("/settings/checkBox_ACNet_Anime4K").toBool());
    ui->checkBox_GPUMode_Anime4K->setChecked(Settings_Read_value("/settings/checkBox_GPUMode_Anime4K").toBool());
    ui->spinBox_Passes_Anime4K->setValue(Settings_Read_value("/settings/spinBox_Passes_Anime4K").toInt());
    ui->spinBox_PushColorCount_Anime4K->setValue(Settings_Read_value("/settings/spinBox_PushColorCount_Anime4K").toInt());
    ui->doubleSpinBox_PushColorStrength_Anime4K->setValue(Settings_Read_value("/settings/doubleSpinBox_PushColorStrength_Anime4K").toDouble());
    ui->doubleSpinBox_PushGradientStrength_Anime4K->setValue(Settings_Read_value("/settings/doubleSpinBox_PushGradientStrength_Anime4K").toDouble());
    ui->checkBox_SpecifyGPU_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_SpecifyGPU_Anime4k").toBool());
    ui->lineEdit_GPUs_Anime4k->setText(Settings_Read_value("/settings/lineEdit_GPUs_Anime4k").toString());
    // Pre-Processing
    ui->checkBox_EnablePreProcessing_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_EnablePreProcessing_Anime4k").toBool());
    ui->checkBox_MedianBlur_Pre_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_MedianBlur_Pre_Anime4k").toBool());
    ui->checkBox_MeanBlur_Pre_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_MeanBlur_Pre_Anime4k").toBool());
    ui->checkBox_CASSharping_Pre_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_CASSharping_Pre_Anime4k").toBool());
    ui->checkBox_GaussianBlurWeak_Pre_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_GaussianBlurWeak_Pre_Anime4k").toBool());
    ui->checkBox_GaussianBlur_Pre_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_GaussianBlur_Pre_Anime4k").toBool());
    ui->checkBox_BilateralFilter_Pre_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_BilateralFilter_Pre_Anime4k").toBool());
    ui->checkBox_BilateralFilterFaster_Pre_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_BilateralFilterFaster_Pre_Anime4k").toBool());
    // Post-Processing
    ui->checkBox_EnablePostProcessing_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_EnablePostProcessing_Anime4k").toBool());
    ui->checkBox_MedianBlur_Post_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_MedianBlur_Post_Anime4k").toBool());
    ui->checkBox_MeanBlur_Post_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_MeanBlur_Post_Anime4k").toBool());
    ui->checkBox_CASSharping_Post_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_CASSharping_Post_Anime4k").toBool());
    ui->checkBox_GaussianBlurWeak_Post_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_GaussianBlurWeak_Post_Anime4k").toBool());
    ui->checkBox_GaussianBlur_Post_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_GaussianBlur_Post_Anime4k").toBool());
    ui->checkBox_BilateralFilter_Post_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_BilateralFilter_Post_Anime4k").toBool());
    ui->checkBox_BilateralFilterFaster_Post_Anime4k->setChecked(Settings_Read_value("/settings/checkBox_BilateralFilterFaster_Post_Anime4k").toBool());
    //===================== 加载兼容性测试结果 ============================
    isCompatible_Waifu2x_NCNN_Vulkan_NEW = Settings_Read_value("/settings/checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW").toBool();
    isCompatible_Waifu2x_NCNN_Vulkan_NEW_FP16P = Settings_Read_value("/settings/checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW_FP16P").toBool();
    isCompatible_Waifu2x_Converter = Settings_Read_value("/settings/checkBox_isCompatible_Waifu2x_Converter").toBool();
    isCompatible_SRMD_NCNN_Vulkan = Settings_Read_value("/settings/checkBox_isCompatible_SRMD_NCNN_Vulkan").toBool();
    isCompatible_Anime4k_CPU = Settings_Read_value("/settings/checkBox_isCompatible_Anime4k_CPU").toBool();
    isCompatible_Anime4k_GPU = Settings_Read_value("/settings/checkBox_isCompatible_Anime4k_GPU").toBool();
    isCompatible_FFmpeg = Settings_Read_value("/settings/checkBox_isCompatible_FFmpeg").toBool();
    isCompatible_FFprobe = Settings_Read_value("/settings/checkBox_isCompatible_FFprobe").toBool();
    isCompatible_ImageMagick = Settings_Read_value("/settings/checkBox_isCompatible_ImageMagick").toBool();
    isCompatible_Gifsicle = Settings_Read_value("/settings/checkBox_isCompatible_Gifsicle").toBool();
    isCompatible_SoX = Settings_Read_value("/settings/checkBox_isCompatible_SoX").toBool();
    isCompatible_Waifu2x_Caffe_CPU = Settings_Read_value("/settings/checkBox_isCompatible_Waifu2x_Caffe_CPU").toBool();
    isCompatible_Waifu2x_Caffe_GPU = Settings_Read_value("/settings/checkBox_isCompatible_Waifu2x_Caffe_GPU").toBool();
    isCompatible_Waifu2x_Caffe_cuDNN = Settings_Read_value("/settings/checkBox_isCompatible_Waifu2x_Caffe_cuDNN").toBool();
    isCompatible_Realsr_NCNN_Vulkan = Settings_Read_value("/settings/checkBox_isCompatible_Realsr_NCNN_Vulkan").toBool();
    isCompatible_RealESRGAN_NCNN_Vulkan = Settings_Read_value("/settings/checkBox_isCompatible_RealESRGAN_NCNN_Vulkan").toBool();
    isCompatible_RealCUGAN_NCNN_Vulkan = Settings_Read_value("/settings/checkBox_isCompatible_RealCUGAN_NCNN_Vulkan").toBool();
    //===
    ui->checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW->setChecked(isCompatible_Waifu2x_NCNN_Vulkan_NEW);
    ui->checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW_FP16P->setChecked(isCompatible_Waifu2x_NCNN_Vulkan_NEW_FP16P);
    ui->checkBox_isCompatible_Waifu2x_Converter->setChecked(isCompatible_Waifu2x_Converter);
    ui->checkBox_isCompatible_SRMD_NCNN_Vulkan->setChecked(isCompatible_SRMD_NCNN_Vulkan);
    ui->checkBox_isCompatible_Anime4k_CPU->setChecked(isCompatible_Anime4k_CPU);
    ui->checkBox_isCompatible_Anime4k_GPU->setChecked(isCompatible_Anime4k_GPU);
    ui->checkBox_isCompatible_FFmpeg->setChecked(isCompatible_FFmpeg);
    ui->checkBox_isCompatible_FFprobe->setChecked(isCompatible_FFprobe);
    ui->checkBox_isCompatible_ImageMagick->setChecked(isCompatible_ImageMagick);
    ui->checkBox_isCompatible_Gifsicle->setChecked(isCompatible_Gifsicle);
    ui->checkBox_isCompatible_SoX->setChecked(isCompatible_SoX);
    ui->checkBox_isCompatible_Waifu2x_Caffe_CPU->setChecked(isCompatible_Waifu2x_Caffe_CPU);
    ui->checkBox_isCompatible_Waifu2x_Caffe_GPU->setChecked(isCompatible_Waifu2x_Caffe_GPU);
    ui->checkBox_isCompatible_Waifu2x_Caffe_cuDNN->setChecked(isCompatible_Waifu2x_Caffe_cuDNN);
    ui->checkBox_isCompatible_Realsr_NCNN_Vulkan->setChecked(isCompatible_Realsr_NCNN_Vulkan);
    ui->checkBox_isCompatible_RealESRGAN_NCNN_Vulkan->setChecked(isCompatible_RealESRGAN_NCNN_Vulkan);
    ui->checkBox_isCompatible_RealCUGAN_NCNN_Vulkan->setChecked(isCompatible_RealCUGAN_NCNN_Vulkan);
    //====================================================
    on_groupBox_video_settings_clicked();
    on_checkBox_AlwaysHideSettings_stateChanged(0);
    on_checkBox_AlwaysHideTextBrowser_stateChanged(0);
    on_checkBox_DelOriginal_stateChanged(0);
    on_checkBox_FileList_Interactive_stateChanged(0);
    on_checkBox_OutPath_isEnabled_stateChanged(0);
    on_checkBox_AudioDenoise_stateChanged(0);
    on_checkBox_ProcessVideoBySegment_stateChanged(0);
    on_checkBox_EnablePreProcessing_Anime4k_stateChanged(0);
    on_checkBox_EnablePostProcessing_Anime4k_stateChanged(0);
    on_checkBox_SpecifyGPU_Anime4k_stateChanged(0);
    on_checkBox_GPUMode_Anime4K_stateChanged(0);
    on_checkBox_ShowInterPro_stateChanged(0);
    //====
    on_comboBox_version_Waifu2xNCNNVulkan_currentIndexChanged(0);
    on_comboBox_Engine_GIF_currentIndexChanged(0);
    isShowAnime4kWarning = false;
    on_comboBox_Engine_Image_currentIndexChanged(0);
    on_comboBox_Engine_Video_currentIndexChanged(0);
    on_comboBox_ImageStyle_currentIndexChanged(0);
    on_comboBox_model_vulkan_currentIndexChanged(0);
    ui->spinBox_DenoiseLevel_image->setValue(Settings_Read_value("/settings/ImageDenoiseLevel").toInt());
    ui->spinBox_DenoiseLevel_gif->setValue(Settings_Read_value("/settings/GIFDenoiseLevel").toInt());
    ui->spinBox_DenoiseLevel_video->setValue(Settings_Read_value("/settings/VideoDenoiseLevel").toInt());
    //=====
    on_comboBox_AspectRatio_custRes_currentIndexChanged(0);
    //=====
    Init_Table();
    //====
    on_checkBox_acodec_copy_2mp4_stateChanged(1);
    on_checkBox_vcodec_copy_2mp4_stateChanged(1);
    Settings_Save();
    //==================================
    delete configIniRead;
    return 0;
}

QVariant MainWindow::Settings_Read_value(QString Key)
{
    QString settings_ini_new = Current_Path + "/settings.ini";
    QSettings *configIniRead_new = new QSettings(settings_ini_new, QSettings::IniFormat);
    configIniRead_new->setIniCodec(QTextCodec::codecForName("UTF-8"));
    // check if special type
    QVariant return_value = decodeReadable(configIniRead_new->value(Key));
    delete configIniRead_new;
    return return_value;
}

/*
保存设置pushbutton
保存设置,弹窗
*/
void MainWindow::on_pushButton_SaveSettings_clicked()
{
    Settings_Save();
    QMessageBox::information(this,"Notification","Settings saved successfully!");
}
/*
重置设置
删除设置文件,重置标记=true,弹窗
*/
void MainWindow::on_pushButton_ResetSettings_clicked()
{
    QMessageBox Msg(QMessageBox::Question, QString("Warning"), QString("Do you really wanna RESET all the settings?"));
    Msg.setIcon(QMessageBox::Warning);
    Msg.addButton(QString("YES"), QMessageBox::YesRole);
    QAbstractButton *pNoBtn = Msg.addButton(QString("NO"), QMessageBox::NoRole);
    Msg.exec();
    if (Msg.clickedButton() == pNoBtn)
        return;
    //============
    QString settings_ini = Current_Path + "/settings.ini";
    QFile::remove(settings_ini);
    Settings_isReseted = true;
    //============
    QMessageBox::information(this,"Notification","The settings file has been reset, please restart the software manually for the default settings to take effect.");
}
