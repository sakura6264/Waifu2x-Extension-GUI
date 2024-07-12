
#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_pushButton_compatibilityTest_clicked()
{
    ui->tab_Home->setEnabled(0);
    ui->tab_EngineSettings->setEnabled(0);
    ui->tab_VideoSettings->setEnabled(0);
    ui->tab_AdditionalSettings->setEnabled(0);
    ui->pushButton_compatibilityTest->setEnabled(0);
    ui->tabWidget->setCurrentIndex(5);
    ui->pushButton_compatibilityTest->setText("Testing, please wait...");
    Init_progressBar_CompatibilityTest();
    QtConcurrent::run(this, &MainWindow::Waifu2x_Compatibility_Test);
}

int MainWindow::Waifu2x_Compatibility_Test()
{
    emit Send_TextBrowser_NewMessage("Compatibility test is ongoing, please wait.");
    //===============
    QString InputPath = Current_Path + "/Compatibility_Test/Compatibility_Test.jpg";
    QString OutputPath = Current_Path + "/Compatibility_Test/res.png";
    QFile::remove(OutputPath);
    //==========================================
    //         waifu2x-ncnn-vulkan 最新版
    //==========================================
    QString Waifu2x_folder_path = Current_Path + "/waifu2x-ncnn-vulkan";
    QString program = Waifu2x_folder_path + "/waifu2x-ncnn-vulkan.exe";
    QString model_path = Waifu2x_folder_path + "/models-upconv_7_anime_style_art_rgb";
    QProcess *Waifu2x_vulkan = new QProcess();
    QStringList args_waifu2x_nccn_vulkan = {"-i", InputPath, "-o", OutputPath, "-s", "2", "-n", "0", "-t", "32", "-m", model_path, "-j", "1:1:1"};
    for (int CompatTest_retry = 0; CompatTest_retry < 3; CompatTest_retry++)
    {
        Waifu2x_vulkan->start(program, args_waifu2x_nccn_vulkan);
        if(waitForProcess(Waifu2x_vulkan) == TerminatedByFlag) {
            delete Waifu2x_vulkan;
            return 0;
        }

        if (QFile::exists(OutputPath))
            break;
    }
    delete Waifu2x_vulkan;
    if (QFile::exists(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-ncnn-vulkan(Latest Version): Yes");
        isCompatible_Waifu2x_NCNN_Vulkan_NEW = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-ncnn-vulkan(Latest Version): No.");
        isCompatible_Waifu2x_NCNN_Vulkan_NEW = false;
    }
    QFile::remove(OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //            waifu2x-converter
    //==========================================
    Waifu2x_folder_path = Current_Path + "/waifu2x-converter";
    program = Waifu2x_folder_path + "/waifu2x-converter-cpp.exe";
    model_path = Waifu2x_folder_path + "/models_rgb";
    QStringList Denoise_cmd = {"--noise-level", "1"};
    QStringList args_waifu2x_converter = {"-i", InputPath, "-o", OutputPath, "--scale-ratio", "2"};
    QStringList args_waifu2x_converter_add = {"--block-size", "32", "--model-dir", model_path};
    args_waifu2x_converter.append(Denoise_cmd);
    args_waifu2x_converter.append(args_waifu2x_converter_add);
    QProcess *Waifu2x_converter = new QProcess();
    for (int CompatTest_retry = 0; CompatTest_retry < 3; CompatTest_retry++)
    {
        Waifu2x_converter->start(program, args_waifu2x_converter);
        if(waitForProcess(Waifu2x_converter) == TerminatedByFlag) {
            delete Waifu2x_converter;
            return 0;
        }

        if (QFile::exists(OutputPath))
            break;
    }
    delete Waifu2x_converter;
    if (QFile::exists(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-converter: Yes.");
        isCompatible_Waifu2x_Converter = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-converter: No.");
        isCompatible_Waifu2x_Converter = false;
    }
    QFile::remove(OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //            Anime4k(CPU Mode)
    //==========================================
    Current_Path = qApp->applicationDirPath();
    QString Anime4k_folder_path = Current_Path + "/Anime4K";
    program = Anime4k_folder_path + "/Anime4K.exe";
    QStringList args_anime4k_cpu = {"-i", InputPath, "-o", OutputPath, "-z", "2"};
    QProcess *Waifu2x_anime4k = new QProcess();
    for (int CompatTest_retry = 0; CompatTest_retry < 3; CompatTest_retry++)
    {
        Waifu2x_anime4k->start(program, args_anime4k_cpu);
        if(waitForProcess(Waifu2x_anime4k) == TerminatedByFlag) {
            delete Waifu2x_anime4k;
            return 0;
        }
        if (QFile::exists(OutputPath))
            break;
    }
    delete Waifu2x_anime4k;
    if (QFile::exists(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with Anime4k(CPU Mode): Yes.");
        isCompatible_Anime4k_CPU = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with Anime4k(CPU Mode): No.");
        isCompatible_Anime4k_CPU = false;
    }
    QFile::remove(OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //            Anime4k(GPU Mode)
    //==========================================
    Current_Path = qApp->applicationDirPath();
    program = Anime4k_folder_path + "/Anime4K.exe";
    QStringList args_anime4k_gpu = {"-i", InputPath, "-o", OutputPath, "-z", "2", "-q"};
    QProcess *Waifu2x_anime4k_gpu = new QProcess();
    for (int CompatTest_retry = 0; CompatTest_retry < 3; CompatTest_retry++)
    {
        Waifu2x_anime4k_gpu->start(program, args_anime4k_gpu);
        if(waitForProcess(Waifu2x_anime4k_gpu) == TerminatedByFlag) {
            delete Waifu2x_anime4k_gpu;
            return 0;
        }
        if (QFile::exists(OutputPath))
            break;
    }
    delete Waifu2x_anime4k_gpu;
    if (QFile::exists(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with Anime4k(GPU Mode): Yes.");
        isCompatible_Anime4k_GPU = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with Anime4k(GPU Mode): No.");
        isCompatible_Anime4k_GPU = false;
    }
    QFile::remove(OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //            SRMD-ncnn-vulkan
    //==========================================
    Waifu2x_folder_path = Current_Path + "/srmd-ncnn-vulkan";
    program = Waifu2x_folder_path + "/srmd-ncnn-vulkan.exe";
    model_path = Waifu2x_folder_path + "/models-srmd";
    QProcess *SRMD_NCNN_VULKAN = new QProcess();
    QStringList args_SRMD_NCNN_VULKAN = {"-i", InputPath, "-o", OutputPath, "-s", "2", "-n", "0", "-t", "32", "-m", model_path, "-j", "1:1:1"};
    for (int CompatTest_retry = 0; CompatTest_retry < 3; CompatTest_retry++)
    {
        SRMD_NCNN_VULKAN->start(program, args_SRMD_NCNN_VULKAN);
        if(waitForProcess(SRMD_NCNN_VULKAN) == TerminatedByFlag) {
            delete SRMD_NCNN_VULKAN;
            return 0;
        }
        if (QFile::exists(OutputPath))
            break;
    }
    delete SRMD_NCNN_VULKAN;
    if (QFile::exists(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with srmd-ncnn-vulkan: Yes");
        isCompatible_SRMD_NCNN_Vulkan = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with srmd-ncnn-vulkan: No.");
        isCompatible_SRMD_NCNN_Vulkan = false;
    }
    QFile::remove(OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //            Waifu2x-Caffe(CPU)
    //==========================================
    Waifu2x_folder_path = Current_Path + "/waifu2x-caffe";
    program = Waifu2x_folder_path + "/waifu2x-caffe.exe";
    model_path = Waifu2x_folder_path + "/models/upconv_7_anime_style_art_rgb";
    QProcess *Waifu2x_Caffe_CPU_qprocess = new QProcess();
    QStringList args_Waifu2x_Caffe_CPU = {"-i", InputPath, "-o", OutputPath, "-p", "cpu", "-m", "noise_scale", "-s", "2", "-n", "1", "-c", "32", "-b", "1", "--model_dir", model_path};
    for (int CompatTest_retry = 0; CompatTest_retry < 3; CompatTest_retry++)
    {
        Waifu2x_Caffe_CPU_qprocess->start(program, args_Waifu2x_Caffe_CPU);
        if(waitForProcess(Waifu2x_Caffe_CPU_qprocess) == TerminatedByFlag) {
            delete Waifu2x_Caffe_CPU_qprocess;
            return 0;
        }
        if (QFile::exists(OutputPath))
            break;
    }
    delete Waifu2x_Caffe_CPU_qprocess;
    if (QFile::exists(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-caffe(CPU): Yes.");
        isCompatible_Waifu2x_Caffe_CPU = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-caffe(CPU): No.");
        isCompatible_Waifu2x_Caffe_CPU = false;
    }
    QFile::remove(OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //            Waifu2x-Caffe(GPU)
    //==========================================
    Waifu2x_folder_path = Current_Path + "/waifu2x-caffe";
    program = Waifu2x_folder_path + "/waifu2x-caffe.exe";
    model_path = Waifu2x_folder_path + "/models/upconv_7_anime_style_art_rgb";
    QProcess *Waifu2x_Caffe_GPU_qprocess = new QProcess();
    QStringList args_Waifu2x_Caffe_GPU = {"-i", InputPath, "-o", OutputPath, "-p", "gpu", "-m", "noise_scale", "-s", "2", "-n", "1", "-c", "32", "-b", "1", "--model_dir", model_path};
    for (int CompatTest_retry = 0; CompatTest_retry < 3; CompatTest_retry++)
    {
        Waifu2x_Caffe_GPU_qprocess->start(program, args_Waifu2x_Caffe_GPU);
        if(waitForProcess(Waifu2x_Caffe_GPU_qprocess) == TerminatedByFlag) {
            delete Waifu2x_Caffe_GPU_qprocess;
            return 0;
        }
        if (QFile::exists(OutputPath))
            break;
    }
    delete Waifu2x_Caffe_GPU_qprocess;
    if (QFile::exists(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-caffe(GPU): Yes.");
        isCompatible_Waifu2x_Caffe_GPU = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-caffe(GPU): No.");
        isCompatible_Waifu2x_Caffe_GPU = false;
    }
    QFile::remove(OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //            Waifu2x-Caffe(cuDNN)
    //==========================================
    Waifu2x_folder_path = Current_Path + "/waifu2x-caffe";
    program = Waifu2x_folder_path + "/waifu2x-caffe.exe";
    model_path = Waifu2x_folder_path + "/models/upconv_7_anime_style_art_rgb";
    QProcess *Waifu2x_Caffe_cuDNN_qprocess = new QProcess();
    QStringList args_Waifu2x_Caffe_cuDNN = {"-i", InputPath, "-o", OutputPath, "-p", "cudnn", "-m", "noise_scale", "-s", "2", "-n", "1", "-c", "32", "-b", "1", "--model_dir", model_path};
    for (int CompatTest_retry = 0; CompatTest_retry < 3; CompatTest_retry++)
    {
        Waifu2x_Caffe_cuDNN_qprocess->start(program, args_Waifu2x_Caffe_cuDNN);
        if(waitForProcess(Waifu2x_Caffe_cuDNN_qprocess) == TerminatedByFlag) {
            delete Waifu2x_Caffe_cuDNN_qprocess;
            return 0;
        }
        if (QFile::exists(OutputPath))
            break;
    }
    delete Waifu2x_Caffe_cuDNN_qprocess;
    if (QFile::exists(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-caffe(cuDNN): Yes.");
        isCompatible_Waifu2x_Caffe_cuDNN = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with waifu2x-caffe(cuDNN): No.");
        isCompatible_Waifu2x_Caffe_cuDNN = false;
    }
    QFile::remove(OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //            RealSR-ncnn-vulkan
    //==========================================
    Waifu2x_folder_path = Current_Path + "/realsr-ncnn-vulkan";
    program = Waifu2x_folder_path + "/realsr-ncnn-vulkan.exe";
    model_path = Waifu2x_folder_path + "/models-DF2K_JPEG";
    QProcess *realsr_ncnn_vulkan_qprocess = new QProcess();
    QStringList args_realsr_ncnn_vulkan = {"-i", InputPath, "-o", OutputPath, "-s", "4", "-t", "32", "-m", model_path};
    for (int CompatTest_retry = 0; CompatTest_retry < 3; CompatTest_retry++)
    {
        realsr_ncnn_vulkan_qprocess->start(program, args_realsr_ncnn_vulkan);
        if(waitForProcess(realsr_ncnn_vulkan_qprocess) == TerminatedByFlag) {
            delete realsr_ncnn_vulkan_qprocess;
            return 0;
        }
        if (QFile::exists(OutputPath))
            break;
    }
    delete realsr_ncnn_vulkan_qprocess;
    if (QFile::exists(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with Realsr-ncnn-vulkan: Yes.");
        isCompatible_Realsr_NCNN_Vulkan = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with Realsr-ncnn-vulkan: No.");
        isCompatible_Realsr_NCNN_Vulkan = false;
    }
    QFile::remove(OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //            realesrgan-ncnn-vulkan
    //==========================================
    Waifu2x_folder_path = Current_Path + "/realesrgan-ncnn-vulkan";
    program = Waifu2x_folder_path + "/realesrgan-ncnn-vulkan.exe";
    model_path = Waifu2x_folder_path + "/models";
    QString model_name = "realesr-animevideov3";
    QProcess *RealESRGAN_ncnn_vulkan_qprocess = new QProcess();
    QStringList args_RealESRGAN_ncnn_vulkan = {"-i", InputPath, "-o", OutputPath, "-s", "4", "-t", "32", "-m", model_path,"-n",model_name};
    for (int CompatTest_retry = 0; CompatTest_retry < 3; CompatTest_retry++)
    {
        RealESRGAN_ncnn_vulkan_qprocess->start(program, args_RealESRGAN_ncnn_vulkan);
        if(waitForProcess(RealESRGAN_ncnn_vulkan_qprocess) == TerminatedByFlag) {
            delete RealESRGAN_ncnn_vulkan_qprocess;
            return 0;
        }
        if (QFile::exists(OutputPath))
            break;
    }
    delete RealESRGAN_ncnn_vulkan_qprocess;
    if (QFile::exists(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with RealESRGAN-ncnn-vulkan: Yes.");
        isCompatible_RealESRGAN_NCNN_Vulkan = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with RealESRGAN-ncnn-vulkan: No.");
        isCompatible_RealESRGAN_NCNN_Vulkan = false;
    }
    QFile::remove(OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //            realcugan-ncnn-vulkan
    //==========================================
    Waifu2x_folder_path = Current_Path + "/realcugan-ncnn-vulkan";
    program = Waifu2x_folder_path + "/realcugan-ncnn-vulkan.exe";
    model_path = Waifu2x_folder_path + "/models-se";
    QProcess *RealCUGAN_ncnn_vulkan_qprocess = new QProcess();
    QStringList args_RealCUGAN_ncnn_vulkan = {"-i", InputPath, "-o", OutputPath, "-s", "4", "-t", "32", "-m", model_path};
    for (int CompatTest_retry = 0; CompatTest_retry < 3; CompatTest_retry++)
    {
        RealCUGAN_ncnn_vulkan_qprocess->start(program, args_RealCUGAN_ncnn_vulkan);
        if(waitForProcess(RealCUGAN_ncnn_vulkan_qprocess) == TerminatedByFlag) {
            delete RealCUGAN_ncnn_vulkan_qprocess;
            return 0;
        }
        if (QFile::exists(OutputPath))
            break;
    }
    delete RealCUGAN_ncnn_vulkan_qprocess;
    if (QFile::exists(OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with RealCUGAN-ncnn-vulkan: Yes.");
        isCompatible_RealCUGAN_NCNN_Vulkan = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with RealCUGAN-ncnn-vulkan: No.");
        isCompatible_RealCUGAN_NCNN_Vulkan = false;
    }
    QFile::remove(OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //                  FFmpeg
    //==========================================
    QString ffmpeg_VideoPath = Current_Path + "/Compatibility_Test/CompatibilityTest_Video.mp4";
    QString ffmpeg_AudioPath = Current_Path + "/Compatibility_Test/CompatibilityTest_Video_audio.wav";
    QString ffmpeg_path = Current_Path + "/ffmpeg.exe";
    QFile::remove(ffmpeg_AudioPath);
    QProcess *ffmpeg_QProcess = new QProcess();
    QStringList args_ffmpeg = {"-y", "-i", ffmpeg_VideoPath, ffmpeg_AudioPath};
    ffmpeg_QProcess->start(ffmpeg_path, args_ffmpeg);
    if(waitForProcess(ffmpeg_QProcess) == TerminatedByFlag) {
        delete ffmpeg_QProcess;
        return 0;
    }
    delete ffmpeg_QProcess;
    if (QFile::exists(ffmpeg_AudioPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with FFmpeg: Yes.");
        isCompatible_FFmpeg = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with FFmpeg: No.");
        isCompatible_FFmpeg = false;
    }
    QFile::remove(ffmpeg_AudioPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //                 FFprobe
    //==========================================
    QString FFprobe_VideoPath = Current_Path + "/Compatibility_Test/CompatibilityTest_Video.mp4";
    //========================= 调用ffprobe读取视频信息 ======================
    QProcess *FFprobe_Get_Duration_process = new QProcess();
    QString FFprobe_path = Current_Path + "/ffprobe.exe";
    QStringList FFprobe_args = {"-i", FFprobe_VideoPath, "-v", "quiet", "-print_format", "json", "-show_format"};
    FFprobe_Get_Duration_process->start(FFprobe_path, FFprobe_args);
    if(waitForProcess(FFprobe_Get_Duration_process) == TerminatedByFlag) {
        delete FFprobe_Get_Duration_process;
        return 0;
    }
    //============= 保存ffprobe输出的json格式文本 =============
    QString ffprobe_output_str = FFprobe_Get_Duration_process->readAllStandardOutput().toLower();
    delete FFprobe_Get_Duration_process;
    //===
    if (ffprobe_output_str.contains("\"duration\""))
    {
        emit Send_TextBrowser_NewMessage("Compatible with FFprobe: Yes.");
        isCompatible_FFprobe = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with FFprobe: No.");
        isCompatible_FFprobe = false;
    }
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //                 ImageMagick
    //==========================================
    // convert
    QString convert_InputPath = Current_Path + "/Compatibility_Test/Compatibility_Test.jpg";
    QString convert_OutputPath = Current_Path + "/Compatibility_Test/convert_res.bmp";
    QString convert_program = Current_Path + "/convert.exe";
    QFile::remove(convert_OutputPath);
    QProcess* convert_QProcess = new QProcess();
    QStringList convert_args = {convert_InputPath, convert_OutputPath};
    convert_QProcess->start(convert_program, convert_args);
    if(waitForProcess(convert_QProcess) == TerminatedByFlag) {
        delete convert_QProcess;
        return 0;
    }
    delete convert_QProcess;
    bool imagemagick_convert_compatible = QFile::exists(convert_OutputPath);
    bool imagemagick_identfy_compatible = false;
    // identify
    QString identify_program = Current_Path + "/identify.exe";
    QStringList args = {"-format", "%w:%h;success;", convert_InputPath};
    QProcess *identfy_QProcess = new QProcess();
    identfy_QProcess->start(identify_program, args);
    if(waitForProcess(identfy_QProcess) == TerminatedByFlag) {
        delete identfy_QProcess;
        return 0;
    }
    QString identify_output = identfy_QProcess->readAllStandardOutput().trimmed();
    delete identfy_QProcess;
    if (identify_output.contains("success"))
    {
        QStringList Res_strList = identify_output.split(";").at(0).split(":");
        int width = Res_strList.at(0).toInt();
        int height = Res_strList.at(1).toInt();
        if (height > 0 && width > 0)
        {
            imagemagick_identfy_compatible = true;
        }
    }
    if (imagemagick_convert_compatible&&imagemagick_identfy_compatible)
    {
        emit Send_TextBrowser_NewMessage("Compatible with ImageMagick: Yes.");
        isCompatible_ImageMagick = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with ImageMagick: No.");
        isCompatible_ImageMagick = false;
    }
    QFile::remove(convert_OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //                 Gifsicle
    //==========================================
    QString Gifsicle_InputPath = Current_Path + "/Compatibility_Test/CompatibilityTest_GIF.gif";
    QString Gifsicle_OutputPath = Current_Path + "/Compatibility_Test/CompatibilityTest_GIF_RES.gif";
    QFile::remove(Gifsicle_OutputPath);
    //===
    QString Gifsicle_program = Current_Path + "/gifsicle.exe";
    QStringList Gifsicle_args = {"-O3", "-i", Gifsicle_InputPath, "-o", Gifsicle_OutputPath};
    QProcess *Gifsicle_CompressGIF = new QProcess();
    Gifsicle_CompressGIF->start(Gifsicle_program, Gifsicle_args);
    if(waitForProcess(Gifsicle_CompressGIF) == TerminatedByFlag) {
        delete Gifsicle_CompressGIF;
        return 0;
    }
    delete Gifsicle_CompressGIF;
    //===
    if (QFile::exists(Gifsicle_OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with Gifsicle: Yes.");
        isCompatible_Gifsicle = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with Gifsicle: No.");
        isCompatible_Gifsicle = false;
    }
    QFile::remove(Gifsicle_OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //==========================================
    //                 SoX
    //==========================================
    QString SoX_InputPath = Current_Path + "/Compatibility_Test/CompatibilityTest_Sound.wav";
    QString SoX_OutputPath = Current_Path + "/Compatibility_Test/TestTemp_DenoiseProfile.dp";
    QFile::remove(SoX_OutputPath);
    //===
    QString SoX_program = Current_Path + "/SoX/sox.exe";
    QProcess *SoX_QProcess = new QProcess();
    QStringList SoX_args = {SoX_InputPath, "-n", "noiseprof", SoX_OutputPath};
    SoX_QProcess->start(SoX_program, SoX_args);
    if(waitForProcess(SoX_QProcess) == TerminatedByFlag) {
        delete SoX_QProcess;
        return 0;
    }
    delete SoX_QProcess;
    //===
    if (QFile::exists(SoX_OutputPath))
    {
        emit Send_TextBrowser_NewMessage("Compatible with SoX: Yes.");
        isCompatible_SoX = true;
    }
    else
    {
        emit Send_TextBrowser_NewMessage("Compatible with SoX: No.");
        isCompatible_SoX = false;
    }
    QFile::remove(SoX_OutputPath);
    emit Send_Add_progressBar_CompatibilityTest();
    //=================
    // 杀死滞留的进程
    //=================
    taskKill("Anime4K.exe");
    taskKill("waifu2x-ncnn-vulkan.exe");
    taskKill("waifu2x-converter-cpp.exe");
    taskKill("srmd-ncnn-vulkan.exe");
    taskKill("waifu2x-caffe.exe");
    taskKill("realsr-ncnn-vulkan.exe");
    taskKill("realesrgan-ncnn-vulkan.exe");
    taskKill("realcugan-ncnn-vulkan.exe");
    //================
    // 测试结束
    //================
    emit Send_TextBrowser_NewMessage("Compatibility test is complete!");
    emit Send_SystemTray_NewMessage("Compatibility test is complete!");
    emit Send_Waifu2x_Compatibility_Test_finished();
    return 0;
}

int MainWindow::Waifu2x_Compatibility_Test_finished()
{
    // 更改checkbox状态以显示测试结果
    ui->checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW->setChecked(isCompatible_Waifu2x_NCNN_Vulkan_NEW);
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
    // 解除界面管制
    Finish_progressBar_CompatibilityTest();
    ui->tab_Home->setEnabled(1);
    ui->tab_EngineSettings->setEnabled(1);
    ui->tab_VideoSettings->setEnabled(1);
    ui->tab_AdditionalSettings->setEnabled(1);
    ui->pushButton_compatibilityTest->setEnabled(1);
    ui->pushButton_compatibilityTest->setText("Start compatibility test");
    ui->tabWidget->setCurrentIndex(5);
    /*
    判断是否有必要部件不兼容,如果有则弹出提示
    */
    if (isCompatible_FFmpeg == false || isCompatible_FFprobe == false || isCompatible_ImageMagick == false || isCompatible_Gifsicle == false || isCompatible_SoX == false)
    {
        QMessageBox::warning(this,"Notification","One of the essential plugins is not compatible with your PC.");
    }
    //========== 提示是否需要自动调整引擎设定 ==========
    QMessageBox Msg(QMessageBox::Question, QString("Notification"), QString("Do you need the software to automatically adjust the engine settings for you based on the compatibility test results?"));
    Msg.setIcon(QMessageBox::Information);
    QAbstractButton *pYesBtn = Msg.addButton(QString("Yes"), QMessageBox::YesRole);
    QAbstractButton *pNoBtn = Msg.addButton(QString("No"), QMessageBox::NoRole);
    Msg.exec();
    if (Msg.clickedButton() == pYesBtn)
    {
        /*
         * 协助用户调整引擎设定:
         */
        //========== 检查realcugan-ncnn-vulkan 最新版 的兼容性 ===============
        if (isCompatible_RealCUGAN_NCNN_Vulkan == true)
        {
            ui->comboBox_Engine_Image->setCurrentIndex(7);
            ui->comboBox_Engine_GIF->setCurrentIndex(7);
            ui->comboBox_Engine_Video->setCurrentIndex(7);
            on_comboBox_Engine_Image_currentIndexChanged(0);
            on_comboBox_Engine_GIF_currentIndexChanged(0);
            on_comboBox_Engine_Video_currentIndexChanged(0);
            //====
            ui->tabWidget->setCurrentIndex(0);
            return 0;
        }
        //========== 检查realesrgan-ncnn-vulkan 最新版 的兼容性 ===============
        if (isCompatible_RealESRGAN_NCNN_Vulkan == true)
        {
            ui->comboBox_Engine_Image->setCurrentIndex(6);
            ui->comboBox_Engine_GIF->setCurrentIndex(6);
            ui->comboBox_Engine_Video->setCurrentIndex(6);
            on_comboBox_Engine_Image_currentIndexChanged(0);
            on_comboBox_Engine_GIF_currentIndexChanged(0);
            on_comboBox_Engine_Video_currentIndexChanged(0);
            //====
            ui->tabWidget->setCurrentIndex(0);
            return 0;
        }
        //========== 检查waifu2x-ncnn-vulkan 最新版 的兼容性 ===============
        if (isCompatible_Waifu2x_NCNN_Vulkan_NEW == true)
        {
            ui->comboBox_Engine_Image->setCurrentIndex(0);
            ui->comboBox_Engine_GIF->setCurrentIndex(0);
            ui->comboBox_Engine_Video->setCurrentIndex(0);
            on_comboBox_Engine_Image_currentIndexChanged(0);
            on_comboBox_Engine_GIF_currentIndexChanged(0);
            on_comboBox_Engine_Video_currentIndexChanged(0);
            //====
            ui->tabWidget->setCurrentIndex(0);
            return 0;
        }
        //======================= 检查Waifu2x_Caffe_GPU的兼容性 ===================
        if (isCompatible_Waifu2x_Caffe_GPU)
        {
            ui->comboBox_Engine_Image->setCurrentIndex(4);
            ui->comboBox_Engine_GIF->setCurrentIndex(4);
            ui->comboBox_Engine_Video->setCurrentIndex(4);
            on_comboBox_Engine_Image_currentIndexChanged(0);
            on_comboBox_Engine_GIF_currentIndexChanged(0);
            on_comboBox_Engine_Video_currentIndexChanged(0);
            //====
            ui->comboBox_ProcessMode_Waifu2xCaffe->setCurrentIndex(1);
            ui->tabWidget->setCurrentIndex(0);
            return 0;
        }
        //======================= 检查Waifu2x_Caffe_cuDNN的兼容性 ===================
        if (isCompatible_Waifu2x_Caffe_cuDNN)
        {
            ui->comboBox_Engine_Image->setCurrentIndex(4);
            ui->comboBox_Engine_GIF->setCurrentIndex(4);
            ui->comboBox_Engine_Video->setCurrentIndex(4);
            on_comboBox_Engine_Image_currentIndexChanged(0);
            on_comboBox_Engine_GIF_currentIndexChanged(0);
            on_comboBox_Engine_Video_currentIndexChanged(0);
            //====
            ui->comboBox_ProcessMode_Waifu2xCaffe->setCurrentIndex(2);
            ui->tabWidget->setCurrentIndex(0);
            return 0;
        }
        //======================= 检查SRMD-NCNN-Vulkan的兼容性 ===================
        if (isCompatible_SRMD_NCNN_Vulkan == true)
        {
            ui->comboBox_Engine_Image->setCurrentIndex(2);
            ui->comboBox_Engine_GIF->setCurrentIndex(2);
            ui->comboBox_Engine_Video->setCurrentIndex(3);
            on_comboBox_Engine_Image_currentIndexChanged(0);
            on_comboBox_Engine_GIF_currentIndexChanged(0);
            on_comboBox_Engine_Video_currentIndexChanged(0);
            ui->tabWidget->setCurrentIndex(0);
            return 0;
        }
        //======================= 检查waifu2x-converter的兼容性 ===================
        if (isCompatible_Waifu2x_Converter == true)
        {
            ui->comboBox_Engine_Image->setCurrentIndex(1);
            ui->comboBox_Engine_GIF->setCurrentIndex(1);
            ui->comboBox_Engine_Video->setCurrentIndex(1);
            on_comboBox_Engine_Image_currentIndexChanged(0);
            on_comboBox_Engine_GIF_currentIndexChanged(0);
            on_comboBox_Engine_Video_currentIndexChanged(0);
            ui->tabWidget->setCurrentIndex(0);
            return 0;
        }
        //======================= 检查Anime4K的兼容性 ===================
        if (isCompatible_Anime4k_GPU == true)
        {
            isShowAnime4kWarning = false;
            ui->comboBox_Engine_Image->setCurrentIndex(3);
            ui->comboBox_Engine_GIF->setCurrentIndex(3);
            ui->comboBox_Engine_Video->setCurrentIndex(2);
            on_comboBox_Engine_Image_currentIndexChanged(0);
            on_comboBox_Engine_GIF_currentIndexChanged(0);
            on_comboBox_Engine_Video_currentIndexChanged(0);
            //=====
            ui->checkBox_GPUMode_Anime4K->setChecked(1);
            on_checkBox_GPUMode_Anime4K_stateChanged(0);
            ui->tabWidget->setCurrentIndex(0);
            return 0;
        }
        if (isCompatible_Anime4k_CPU == true)
        {
            isShowAnime4kWarning = false;
            ui->comboBox_Engine_Image->setCurrentIndex(3);
            ui->comboBox_Engine_GIF->setCurrentIndex(3);
            ui->comboBox_Engine_Video->setCurrentIndex(2);
            on_comboBox_Engine_Image_currentIndexChanged(0);
            on_comboBox_Engine_GIF_currentIndexChanged(0);
            on_comboBox_Engine_Video_currentIndexChanged(0);
            //=====
            ui->checkBox_GPUMode_Anime4K->setChecked(0);
            on_checkBox_GPUMode_Anime4K_stateChanged(0);
            ui->tabWidget->setCurrentIndex(0);
            return 0;
        }
        //======================= 检查Waifu2x_Caffe_CPU的兼容性 ===================
        if (isCompatible_Waifu2x_Caffe_CPU)
        {
            ui->comboBox_Engine_Image->setCurrentIndex(4);
            ui->comboBox_Engine_GIF->setCurrentIndex(4);
            ui->comboBox_Engine_Video->setCurrentIndex(4);
            on_comboBox_Engine_Image_currentIndexChanged(0);
            on_comboBox_Engine_GIF_currentIndexChanged(0);
            on_comboBox_Engine_Video_currentIndexChanged(0);
            //====
            ui->comboBox_ProcessMode_Waifu2xCaffe->setCurrentIndex(0);
            ui->tabWidget->setCurrentIndex(0);
            return 0;
        }
        //======================= 检查Realsr_NCNN_Vulkan的兼容性 ===================
        if (isCompatible_Realsr_NCNN_Vulkan)
        {
            ui->comboBox_Engine_Image->setCurrentIndex(5);
            ui->comboBox_Engine_GIF->setCurrentIndex(5);
            ui->comboBox_Engine_Video->setCurrentIndex(5);
            on_comboBox_Engine_Image_currentIndexChanged(0);
            on_comboBox_Engine_GIF_currentIndexChanged(0);
            on_comboBox_Engine_Video_currentIndexChanged(0);
            //====
            ui->tabWidget->setCurrentIndex(0);
            return 0;
        }
        // 啥引擎都不兼容,提示用户自行修复兼容性问题
        QMessageBox::warning(this,"Notification","All engines are not compatible with your computer.");
        return 0;
    }
    if (Msg.clickedButton() == pNoBtn)
    {
        // 提醒用户检查测试结果

        QMessageBox::information(this,"Notification","The compatibility test has been completed.");
        return 0;
    }
    //===============
    return 0;
}

// 初始化 -兼容性测试进度条
void MainWindow::Init_progressBar_CompatibilityTest()
{
    ui->progressBar_CompatibilityTest->setEnabled(1);
    ui->progressBar_CompatibilityTest->setVisible(1);
    ui->progressBar_CompatibilityTest->setRange(0, 17);
    ui->progressBar_CompatibilityTest->setValue(0);
}
// 进度+1 -兼容性测试进度条
void MainWindow::Add_progressBar_CompatibilityTest()
{
    int CurrentValue = ui->progressBar_CompatibilityTest->value();
    ui->progressBar_CompatibilityTest->setValue(CurrentValue + 1);
}
// 兼容性测试完成后的操作 -兼容性测试进度条
void MainWindow::Finish_progressBar_CompatibilityTest()
{
    ui->progressBar_CompatibilityTest->setVisible(0);
}
