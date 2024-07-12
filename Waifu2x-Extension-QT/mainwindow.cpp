
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    WECHO_Path = Current_Path + "/wecho.exe";
    ui->setupUi(this);
    qRegisterMetaTypeStreamOperators<QList_QMap_QStrQStr>("QList_QMap_QStrQStr");
    //==============
    this->setWindowTitle("Waifu2x-Extension-GUI ");
    //==============
    ui->tabWidget->setCurrentIndex(0); // 显示home tab
    on_tabWidget_currentChanged(0);
    ui->tabWidget_Engines->setCurrentIndex(0);
    this->setAcceptDrops(true);              // mainwindow接收drop
    Init_Table();                            // 初始化table
    ui->groupBox_CurrentFile->setVisible(0); // 隐藏当前文件进度
    ui->pushButton_Stop->setVisible(0);
    ui->pushButton_ForceRetry->setVisible(0);
    ui->progressBar_CompatibilityTest->setVisible(0);
    //=================== 初始隐藏所有table和禁用按钮 ======================
    ui->tableView_image->setVisible(0);
    ui->tableView_gif->setVisible(0);
    ui->tableView_video->setVisible(0);
    Table_FileCount_reload(); // 重载文件列表下的文件数量统计
    //==============
    Init_ActionsMenu_checkBox_ReplaceOriginalFile(); // 第一次初始化[替换原文件]的右键菜单(需要在载入设定前设置为checkable
    Init_ActionsMenu_checkBox_DelOriginal();
    //=========安装事件过滤器==========
    ui->tableView_image->installEventFilter(this);
    ui->tableView_gif->installEventFilter(this);
    ui->tableView_video->installEventFilter(this);
    //===========================================
    connect(this, SIGNAL(Send_Table_EnableSorting(bool)), this, SLOT(Table_EnableSorting(bool)));
    connect(this, SIGNAL(Send_Add_progressBar_CompatibilityTest()), this, SLOT(Add_progressBar_CompatibilityTest()));
    connect(this, SIGNAL(Send_SetEnable_pushButton_ForceRetry_self()), this, SLOT(SetEnable_pushButton_ForceRetry_self()));
    connect(this, SIGNAL(Send_SystemTray_NewMessage(QString)), this, SLOT(SystemTray_NewMessage(QString)));
    connect(this, SIGNAL(Send_PrograssBar_Range_min_max(int, int)), this, SLOT(progressbar_setRange_min_max(int, int)));
    connect(this, SIGNAL(Send_progressbar_Add()), this, SLOT(progressbar_Add()));
    connect(this, SIGNAL(Send_Table_image_ChangeStatus_rowNumInt_statusQString(int, QString)), this, SLOT(Table_image_ChangeStatus_rowNumInt_statusQString(int, QString)));
    connect(this, SIGNAL(Send_Table_gif_ChangeStatus_rowNumInt_statusQString(int, QString)), this, SLOT(Table_gif_ChangeStatus_rowNumInt_statusQString(int, QString)));
    connect(this, SIGNAL(Send_Table_video_ChangeStatus_rowNumInt_statusQString(int, QString)), this, SLOT(Table_video_ChangeStatus_rowNumInt_statusQString(int, QString)));
    connect(this, SIGNAL(Send_Table_FileCount_reload()), this, SLOT(Table_FileCount_reload()));
    connect(this, SIGNAL(Send_Table_image_insert_fileName_fullPath(QString, QString)), this, SLOT(Table_image_insert_fileName_fullPath(QString, QString)));
    connect(this, SIGNAL(Send_Table_gif_insert_fileName_fullPath(QString, QString)), this, SLOT(Table_gif_insert_fileName_fullPath(QString, QString)));
    connect(this, SIGNAL(Send_Table_video_insert_fileName_fullPath(QString, QString)), this, SLOT(Table_video_insert_fileName_fullPath(QString, QString)));
    connect(this, SIGNAL(Send_Table_image_CustRes_rowNumInt_HeightQString_WidthQString(int, QString, QString)), this, SLOT(Table_image_CustRes_rowNumInt_HeightQString_WidthQString(int, QString, QString)));
    connect(this, SIGNAL(Send_Table_gif_CustRes_rowNumInt_HeightQString_WidthQString(int, QString, QString)), this, SLOT(Table_gif_CustRes_rowNumInt_HeightQString_WidthQString(int, QString, QString)));
    connect(this, SIGNAL(Send_Table_video_CustRes_rowNumInt_HeightQString_WidthQString(int, QString, QString)), this, SLOT(Table_video_CustRes_rowNumInt_HeightQString_WidthQString(int, QString, QString)));
    connect(this, SIGNAL(Send_Table_Read_Saved_Table_Filelist_Finished()), this, SLOT(Table_Read_Saved_Table_Filelist_Finished()));
    connect(this, SIGNAL(Send_Table_Save_Current_Table_Filelist_Finished()), this, SLOT(Table_Save_Current_Table_Filelist_Finished()));
    connect(this, SIGNAL(Send_Waifu2x_Finished()), this, SLOT(Waifu2x_Finished()));
    connect(this, SIGNAL(Send_Waifu2x_Finished_manual()), this, SLOT(Waifu2x_Finished_manual()));
    connect(this, SIGNAL(Send_TextBrowser_NewMessage(QString)), this, SLOT(TextBrowser_NewMessage(QString)));
    connect(this, SIGNAL(Send_Waifu2x_Compatibility_Test_finished()), this, SLOT(Waifu2x_Compatibility_Test_finished()));
    connect(this, SIGNAL(Send_Waifu2x_DetectGPU_finished()), this, SLOT(Waifu2x_DetectGPU_finished()));
    connect(this, SIGNAL(Send_Realsr_ncnn_vulkan_DetectGPU_finished()), this, SLOT(Realsr_ncnn_vulkan_DetectGPU_finished()));
    connect(this, SIGNAL(Send_RealESRGAN_ncnn_vulkan_DetectGPU_finished()), this, SLOT(RealESRGAN_ncnn_vulkan_DetectGPU_finished()));
    connect(this, SIGNAL(Send_RealCUGAN_ncnn_vulkan_DetectGPU_finished()), this, SLOT(RealCUGAN_ncnn_vulkan_DetectGPU_finished()));
    connect(this, SIGNAL(Send_Waifu2x_DumpProcessorList_converter_finished()), this, SLOT(Waifu2x_DumpProcessorList_converter_finished()));
    connect(this, SIGNAL(Send_Read_urls_finfished()), this, SLOT(Read_urls_finfished()));
    connect(this, SIGNAL(Send_SRMD_DetectGPU_finished()), this, SLOT(SRMD_DetectGPU_finished()));
    connect(this, SIGNAL(Send_video_write_VideoConfiguration(QString, int, int, bool, int, int, QString, bool, QString, QString)), this, SLOT(video_write_VideoConfiguration(QString, int, int, bool, int, int, QString, bool, QString, QString)));
    connect(this, SIGNAL(Send_Settings_Save()), this, SLOT(Settings_Save()));
    connect(this, SIGNAL(Send_video_write_Progress_ProcessBySegment(QString, int, bool, bool, int)), this, SLOT(video_write_Progress_ProcessBySegment(QString, int, bool, bool, int)));
    connect(this, SIGNAL(Send_CurrentFileProgress_Start(QString, int)), this, SLOT(CurrentFileProgress_Start(QString, int)));
    connect(this, SIGNAL(Send_CurrentFileProgress_Stop()), this, SLOT(CurrentFileProgress_Stop()));
    connect(this, SIGNAL(Send_CurrentFileProgress_progressbar_Add()), this, SLOT(CurrentFileProgress_progressbar_Add()));
    connect(this, SIGNAL(Send_CurrentFileProgress_progressbar_Add_SegmentDuration(int)), this, SLOT(CurrentFileProgress_progressbar_Add_SegmentDuration(int)));
    //======
    TimeCostTimer = new QTimer();
    connect(TimeCostTimer, SIGNAL(timeout()), this, SLOT(TimeSlot()));
    //==================================================
    Settings_Read_Apply(); // 读取与应用设置
    //=====================================
    QtConcurrent::run(this, &MainWindow::DeleteErrorLog_Waifu2xCaffe); // 删除Waifu2xCaffe生成的错误日志
    //====================================
    TextBrowser_StartMes(); // 显示启动msg
    //===================================
    Tip_FirstTimeStart();
    //==============
    /*
    校验软件是否对所在目录有写权限
    */
    if (file_isDirWritable(Current_Path) == false)
    {
        QMessageBox Msg(QMessageBox::Question, QString("Error"), QString("It is detected that this software lacks the necessary permissions to run."
                                                                         "\n\nPlease close this software and start this software again after giving this software administrator permission. "
                                                                         "Or reinstall the software into a directory that can run normally without administrator rights.\n\nOtherwise, this software may not work properly."));
        Msg.setIcon(QMessageBox::Warning);
        Msg.addButton(QString("OK"), QMessageBox::NoRole);
        Msg.exec();
    }
    //==============
    Init_SystemTrayIcon();                  // 初始化托盘图标
    Init_ActionsMenu_lineEdit_outputPath(); // 初始化 输出路径 lineEDIT的右键菜单
    Init_ActionsMenu_FilesList();
    Init_ActionsMenu_pushButton_RemoveItem();
    Init_ActionsMenu_checkBox_ReplaceOriginalFile(); // 第二次初始化[替换原文件]的右键菜单(载入语言设置
    Init_ActionsMenu_checkBox_DelOriginal();
    //==============
    this->showNormal();
    this->activateWindow();
    this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
}

MainWindow::~MainWindow()
{
    taskKill("Anime4K.exe");
    taskKill("waifu2x-ncnn-vulkan.exe");
    taskKill("waifu2x-converter-cpp.exe");
    taskKill("srmd-ncnn-vulkan.exe");
    taskKill("waifu2x-caffe.exe");
    taskKill("realsr-ncnn-vulkan.exe");
    taskKill("realesrgan-ncnn-vulkan.exe");
    taskKill("realcugan-ncnn-vulkan.exe");
    delete TimeCostTimer;
    delete MainIcon_QIcon;
    delete Table_model_image;
    delete Table_model_video;
    delete Table_model_gif;
    delete pContextMenu;
    delete minimumAct_SystemTrayIcon;
    delete restoreAct_SystemTrayIcon;
    delete quitAct_SystemTrayIcon;
    delete BackgroundModeAct_SystemTrayIcon;
    delete OpenFolder_lineEdit_outputPath;
    delete OpenFile_QAction_FileList;
    delete OpenFilesFolder_QAction_FileList;
    delete RemoveFile_FilesList_QAction_FileList;
    delete Apply_CustRes_QAction_FileList;
    delete Cancel_CustRes_QAction_FileList;
    delete RemoveALL_image;
    delete RemoveALL_video;
    delete RemoveALL_gif;
    delete QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal;
    delete QAction_checkBox_MoveToRecycleBin_checkBox_ReplaceOriginalFile;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isAlreadyClosed) // 解决QT退出时重复调用closeEvent的问题
    {
        event->accept();
        return;
    }
    //=============== 询问是否退出 =======================
    if (ui->checkBox_PromptWhenExit->isChecked())
    {
        QMessageBox Msg(QMessageBox::Question, QString("Notification"), QString("Do you really wanna exit Waifu2x-Extension-GUI ?"));
        Msg.setIcon(QMessageBox::Question);
        QAbstractButton *pYesBtn = Msg.addButton(QString("YES"), QMessageBox::YesRole);
        QAbstractButton *pNoBtn = Msg.addButton(QString("NO"), QMessageBox::NoRole);
        Msg.exec();
        if (Msg.clickedButton() == pNoBtn)
        {
            event->ignore();
            return;
        }
        if (Msg.clickedButton() == pYesBtn)
            isAlreadyClosed = true;
    }
    //=============================
    systemTray->hide();
    this->hide();
    QApplication::setQuitOnLastWindowClosed(true); // 無窗口時不再保持運行
    QApplication::closeAllWindows();
    //====
    bool AutoSaveSettings = ui->checkBox_AutoSaveSettings->isChecked();
    if (AutoSaveSettings && (!Settings_isReseted))
    {
        Settings_Save();
    }
    QProcess_stop = true;
    Waifu2xMain.cancel();

}

MainWindow::ProcessWaitInfo MainWindow::waitForProcess(QProcess *proc, int start_time, int run_time)
{
    const int wait_period = 100;
    // wait for start
    int start_periods = 0;
    while(!proc->waitForStarted(wait_period)) {
        // force stop
        if (QProcess_stop) {
            proc->kill();
            return TerminatedByFlag;
        }
        // check if it has already started or stop
        QProcess::ProcessState pstate = proc->state();
        if(pstate == QProcess::ProcessState::Starting) {
            start_periods ++ ;
        }
        else if(pstate == QProcess::ProcessState::Running) {
            break;
        }
        else if(pstate == QProcess::ProcessState::NotRunning) {
            QProcess::ExitStatus exit_status = proc->exitStatus();
            if (exit_status == QProcess::ExitStatus::NormalExit) {
                return Successful;
            }
            else {
                return Crash;
            }
        }
        // check if timeout
        if(start_time > 0) {
            if(start_periods*wait_period>start_time){
                // time out
                // force kill
                proc -> kill();
                return Timeout;
            }
        }
    }
    int run_periods = 0;
    while(!proc->waitForFinished(wait_period)) {
        // force stop
        if (QProcess_stop) {
            proc->kill();
            return TerminatedByFlag;
        }
        // check if it has stop
        QProcess::ProcessState pstate = proc->state();
        if(pstate == QProcess::ProcessState::Starting) {
            // must not run this
            return Crash;
        }
        else if(pstate == QProcess::ProcessState::Running) {
            run_periods ++ ;
        }
        else if(pstate == QProcess::ProcessState::NotRunning) {
            break;
        }
        // check if timeout
        if(run_time > 0) {
            if(run_periods*wait_period>run_time){
                // time out
                // force kill
                proc -> kill();
                return Timeout;
            }
        }
    }
    // proc must finish here.
    QProcess::ExitStatus exit_status = proc->exitStatus();
    if (exit_status == QProcess::ExitStatus::NormalExit) {
        return Successful;
    }
    else {
        return Crash;
    }
}

MainWindow::ProcessWaitInfo MainWindow::waitForProcessWaifu2xEx(QProcess *proc, int start_time, int run_time)
{
    const int wait_period = 100;
    // wait for start
    int start_periods = 0;
    while(!proc->waitForStarted(wait_period)) {
        // force stop
        if (QProcess_stop) {
            proc->kill();
            return TerminatedByFlag;
        }
        if (waifu2x_STOP) {
            // won't do any cleanup
            return StoppedByExFlag;
        }
        // check if it has already started or stop
        QProcess::ProcessState pstate = proc->state();
        if(pstate == QProcess::ProcessState::Starting) {
            start_periods ++ ;
        }
        else if(pstate == QProcess::ProcessState::Running) {
            break;
        }
        else if(pstate == QProcess::ProcessState::NotRunning) {
            QProcess::ExitStatus exit_status = proc->exitStatus();
            if (exit_status == QProcess::ExitStatus::NormalExit) {
                return Successful;
            }
            else {
                return Crash;
            }
        }
        // check if timeout
        if(start_time > 0) {
            if(start_periods*wait_period>start_time){
                // time out
                // force kill
                proc -> kill();
                return Timeout;
            }
        }
    }
    int run_periods = 0;
    while(!proc->waitForFinished(wait_period)) {
        // force stop
        if (QProcess_stop) {
            proc->kill();
            return TerminatedByFlag;
        }
        if (waifu2x_STOP) {
            // won't do any cleanup
            return StoppedByExFlag;
        }
        // check if it has stop
        QProcess::ProcessState pstate = proc->state();
        if(pstate == QProcess::ProcessState::Starting) {
            // must not run this
            return Crash;
        }
        else if(pstate == QProcess::ProcessState::Running) {
            run_periods ++ ;
        }
        else if(pstate == QProcess::ProcessState::NotRunning) {
            break;
        }
        // check if timeout
        if(run_time > 0) {
            if(run_periods*wait_period>run_time){
                // time out
                // force kill
                proc -> kill();
                return Timeout;
            }
        }
    }
    // proc must finish here.
    QProcess::ExitStatus exit_status = proc->exitStatus();
    if (exit_status == QProcess::ExitStatus::NormalExit) {
        return Successful;
    }
    else {
        return Crash;
    }
}

int MainWindow::Force_close()
{
    QApplication::quit();
    return 0;
}
/*
最小化
*/
void MainWindow::changeEvent(QEvent *e)
{
    if ((e->type() == QEvent::WindowStateChange) && this->isMinimized())
    {
        if (ui->checkBox_MinimizeToTaskbar->isChecked())
        {
            this->hide();
        }
    }
}

void MainWindow::TimeSlot()
{
    TimeCost++;
    //====================总进度==================
    QString TimeCostStr = "Time taken:[" + Seconds2hms(TimeCost) + "]";
    ui->label_TimeCost->setText(TimeCostStr);
    if (ui->label_TimeRemain_CurrentFile->isVisible())
    {
        int TaskNumFinished_tmp = TaskNumFinished;
        long unsigned int TimeCost_tmp = TimeCost;
        int TaskNumTotal_tmp = TaskNumTotal;
        if (TaskNumFinished_tmp > 0 && TimeCost_tmp > 0 && TaskNumTotal_tmp > 0)
        {
            if (NewTaskFinished)
            {
                NewTaskFinished = false;
                long unsigned int avgTimeCost = TimeCost_tmp / TaskNumFinished_tmp;
                ETA = int(avgTimeCost * (TaskNumTotal_tmp - TaskNumFinished_tmp));
            }
            else
            {
                if (ETA > 1)
                {
                    ETA--;
                }
            }
            QString TimeRemainingStr = "Time remaining:[" + Seconds2hms(ETA) + "]";
            ui->label_TimeRemain->setText(TimeRemainingStr);
            QDateTime time = QDateTime::currentDateTime();
            long unsigned int Time_t = time.toTime_t();
            Time_t += ETA;
            time = QDateTime::fromTime_t(Time_t);
            QString Current_Time = time.toString("hh:mm:ss");
            QString ETA_str = "ETA:[" + Current_Time + "]";
            ui->label_ETA->setText(ETA_str);
        }
    }
    //====================当前文件=================
    if (isStart_CurrentFile)
    {
        TimeCost_CurrentFile++;
        QString TimeCostStr_CurrentFile = "Time taken:[" + Seconds2hms(TimeCost_CurrentFile) + "]";
        ui->label_TimeCost_CurrentFile->setText(TimeCostStr_CurrentFile);
        int TaskNumFinished_tmp_CurrentFile = TaskNumFinished_CurrentFile;
        long unsigned int TimeCost_tmp_CurrentFile = TimeCost_CurrentFile;
        int TaskNumTotal_tmp_CurrentFile = TaskNumTotal_CurrentFile;
        if (TaskNumFinished_tmp_CurrentFile > 0 && TimeCost_tmp_CurrentFile > 0 && TaskNumTotal_tmp_CurrentFile > 0)
        {
            if (NewTaskFinished_CurrentFile)
            {
                NewTaskFinished_CurrentFile = false;
                long unsigned int avgTimeCost_CurrentFile = TimeCost_tmp_CurrentFile / TaskNumFinished_tmp_CurrentFile;
                ETA_CurrentFile = int(avgTimeCost_CurrentFile * (TaskNumTotal_tmp_CurrentFile - TaskNumFinished_tmp_CurrentFile));
            }
            else
            {
                if (ETA_CurrentFile > 1)
                {
                    ETA_CurrentFile--;
                }
            }
            QString TimeRemainingStr_CurrentFile = "Time remaining:[" + Seconds2hms(ETA_CurrentFile) + "]";
            ui->label_TimeRemain_CurrentFile->setText(TimeRemainingStr_CurrentFile);
            QDateTime time_CurrentFile = QDateTime::currentDateTime();
            long unsigned int Time_t_CurrentFile = time_CurrentFile.toTime_t();
            Time_t_CurrentFile += ETA_CurrentFile;
            time_CurrentFile = QDateTime::fromTime_t(Time_t_CurrentFile);
            QString Current_Time_CurrentFile = time_CurrentFile.toString("hh:mm:ss");
            QString ETA_str_CurrentFile = "ETA:[" + Current_Time_CurrentFile + "]";
            ui->label_ETA_CurrentFile->setText(ETA_str_CurrentFile);
        }
    }
}
QString MainWindow::Seconds2hms(long unsigned int seconds)
{
    if (seconds <= 0)
        return "0:0:0";
    long unsigned int mm = 60;
    long unsigned int hh = mm * 60;
    long unsigned int hour = seconds / hh;
    long unsigned int min = (seconds - hour * hh) / mm;
    long unsigned int sec = seconds - hour * hh - min * mm;
    return QString::number(hour, 10) + ":" + QString::number(min, 10) + ":" + QString::number(sec, 10);
}

void MainWindow::on_pushButton_ClearList_clicked()
{
    curRow_image = -1;
    curRow_gif = -1;
    curRow_video = -1;
    Table_Clear();
    Custom_resolution_list.clear();
    ui->label_DropFile->setVisible(1);
    ui->tableView_gif->setVisible(0);
    ui->tableView_image->setVisible(0);
    ui->tableView_video->setVisible(0);
    Table_FileCount_reload();
    progressbar_clear();
}

/*
停止处理键
*/
void MainWindow::on_pushButton_Stop_clicked()
{
    TimeCostTimer->stop();
    ui->pushButton_Stop->setVisible(0); // 隐藏stop button
    waifu2x_STOP = true;
    emit Send_TextBrowser_NewMessage("Trying to stop, please wait...");
    QFuture<void> f1 = QtConcurrent::run(this, &MainWindow::Wait_waifu2x_stop);
}
/*
等待处理线程完全停止
*/
void MainWindow::Wait_waifu2x_stop()
{
    while (true)
    {
        if (waifu2x_STOP_confirm || ThreadNumRunning == 0)
        {
            waifu2x_STOP_confirm = false;
            emit Send_TextBrowser_NewMessage("Processing of files has stopped.");
            break;
        }
        Delay_msec_sleep(300);
    }
    Waifu2xMain.cancel();
    Waifu2xMain.waitForFinished();
    emit Send_Waifu2x_Finished_manual();
}
/*
从tableview移除item
*/
int MainWindow::on_pushButton_RemoveItem_clicked()
{
    if (curRow_image == -1 && curRow_video == -1 && curRow_gif == -1)
    {
        ui->tableView_image->clearSelection();
        ui->tableView_gif->clearSelection();
        ui->tableView_video->clearSelection();
        //=====
        QMessageBox::warning(this,"Warning","No items are currently selected.");
        //=====
        return 0;
    }
    //==========================
    if (curRow_image >= 0)
    {
        CustRes_remove(Table_model_image->item(curRow_image, 2)->text());
        ui->tableView_image->setUpdatesEnabled(false);
        Table_model_image->removeRow(curRow_image);
        ui->tableView_image->setUpdatesEnabled(true);
        curRow_image = -1;
        ui->tableView_image->clearSelection();
    }
    //============================================================
    if (curRow_video >= 0)
    {
        CustRes_remove(Table_model_video->item(curRow_video, 2)->text());
        ui->tableView_video->setUpdatesEnabled(false);
        Table_model_video->removeRow(curRow_video);
        ui->tableView_video->setUpdatesEnabled(true);
        curRow_video = -1;
        ui->tableView_video->clearSelection();
    }
    //============================================================
    if (curRow_gif >= 0)
    {
        CustRes_remove(Table_model_gif->item(curRow_gif, 2)->text());
        ui->tableView_gif->setUpdatesEnabled(false);
        Table_model_gif->removeRow(curRow_gif);
        ui->tableView_gif->setUpdatesEnabled(true);
        curRow_gif = -1;
        ui->tableView_gif->clearSelection();
    }
    //==================================================
    if (Table_model_gif->rowCount() == 0)
    {
        ui->tableView_gif->setVisible(0);
    }
    if (Table_model_image->rowCount() == 0)
    {
        ui->tableView_image->setVisible(0);
    }
    if (Table_model_video->rowCount() == 0)
    {
        ui->tableView_video->setVisible(0);
    }
    //===================================================
    if (Table_model_gif->rowCount() == 0 && Table_model_image->rowCount() == 0 && Table_model_video->rowCount() == 0)
    {
        on_pushButton_ClearList_clicked();
    }
    Table_FileCount_reload();
    //============
    return 0;
}

//==========================================================
/*
============= 安全的阻塞延时 =====================
*/
void MainWindow::Delay_sec_sleep(int time)
{
    QThread::sleep(time);
}

void MainWindow::Delay_msec_sleep(int time)
{
    QThread::msleep(time);
}
//==========================================================

void MainWindow::on_comboBox_Engine_Image_currentIndexChanged(int index)
{
    switch (ui->comboBox_Engine_Image->currentIndex())
    {
    case 0:
    {
        ui->spinBox_DenoiseLevel_image->setRange(-1, 3);
        ui->spinBox_DenoiseLevel_image->setValue(2);
        ui->spinBox_DenoiseLevel_image->setEnabled(1);
        ui->spinBox_DenoiseLevel_image->setToolTip("Range:-1(No noise reduction)~3");
        ui->label_ImageDenoiseLevel->setToolTip("Range:-1(No noise reduction)~3");
        break;
    }
    case 1:
    {
        ui->spinBox_DenoiseLevel_image->setRange(0, 3);
        ui->spinBox_DenoiseLevel_image->setValue(2);
        ui->spinBox_DenoiseLevel_image->setEnabled(1);
        ui->spinBox_DenoiseLevel_image->setToolTip("Range:0(No noise reduction)~3");
        ui->label_ImageDenoiseLevel->setToolTip("Range:0(No noise reduction)~3");
        break;
    }
    case 2:
    {
        ui->spinBox_DenoiseLevel_image->setRange(-1, 10);
        ui->spinBox_DenoiseLevel_image->setValue(4);
        ui->spinBox_DenoiseLevel_image->setEnabled(1);
        ui->spinBox_DenoiseLevel_image->setToolTip("Range:-1(No noise reduction)~10");
        ui->label_ImageDenoiseLevel->setToolTip("Range:-1(No noise reduction)~10");
        break;
    }
    case 3:
    {
        DenoiseLevelSpinboxSetting_Anime4k();
        break;
    }
    case 4:
    {
        ui->spinBox_DenoiseLevel_image->setRange(-1, 3);
        ui->spinBox_DenoiseLevel_image->setValue(2);
        ui->spinBox_DenoiseLevel_image->setEnabled(1);
        ui->spinBox_DenoiseLevel_image->setToolTip("Range:-1(No noise reduction)~3");
        ui->label_ImageDenoiseLevel->setToolTip("Range:-1(No noise reduction)~3");
        break;
    }
    case 5:
    {
        ui->spinBox_DenoiseLevel_image->setRange(0, 1);
        ui->spinBox_DenoiseLevel_image->setValue(1);
        ui->spinBox_DenoiseLevel_image->setEnabled(0);
        ui->spinBox_DenoiseLevel_image->setToolTip("Realsr-ncnn-vulkan engine will denoise automatically.");
        ui->label_ImageDenoiseLevel->setToolTip("Realsr-ncnn-vulkan engine will denoise automatically.");
        break;
    }
    case 6:
    {
        ui->spinBox_DenoiseLevel_image->setRange(0, 1);
        ui->spinBox_DenoiseLevel_image->setValue(1);
        ui->spinBox_DenoiseLevel_image->setEnabled(0);
        ui->spinBox_DenoiseLevel_image->setToolTip("RealESRGAN-ncnn-vulkan engine will denoise automatically.");
        ui->label_ImageDenoiseLevel->setToolTip("RealESRGAN-ncnn-vulkan engine will denoise automatically.");
        break;
    }
    case 7:
    {
        ui->spinBox_DenoiseLevel_image->setRange(0, 1);
        ui->spinBox_DenoiseLevel_image->setValue(1);
        ui->spinBox_DenoiseLevel_image->setEnabled(0);
        ui->spinBox_DenoiseLevel_image->setToolTip("RealCUGAN-ncnn-vulkan engine will denoise automatically.");
        ui->label_ImageDenoiseLevel->setToolTip("RealCUGAN-ncnn-vulkan engine will denoise automatically.");
        break;
    }
    }
    isShowAnime4kWarning = true;
    on_comboBox_model_vulkan_currentIndexChanged(0);
    isWaifu2xCaffeEnabled(); // 判断是否启用caffe图片风格设定
}

void MainWindow::on_comboBox_Engine_GIF_currentIndexChanged(int index)
{
    switch (ui->comboBox_Engine_GIF->currentIndex())
    {
    case 0:
    {
        ui->spinBox_DenoiseLevel_gif->setRange(-1, 3);
        ui->spinBox_DenoiseLevel_gif->setValue(2);
        ui->spinBox_DenoiseLevel_gif->setEnabled(1);
        ui->spinBox_DenoiseLevel_gif->setToolTip("Range:-1(No noise reduction)~3");
        ui->label_GIFDenoiseLevel->setToolTip("Range:-1(No noise reduction)~3");
        break;
    }
    case 1:
    {
        ui->spinBox_DenoiseLevel_gif->setRange(0, 3);
        ui->spinBox_DenoiseLevel_gif->setValue(2);
        ui->spinBox_DenoiseLevel_gif->setEnabled(1);
        ui->spinBox_DenoiseLevel_gif->setToolTip("Range:0(No noise reduction)~3");
        ui->label_GIFDenoiseLevel->setToolTip("Range:0(No noise reduction)~3");
        break;
    }
    case 2:
    {
        ui->spinBox_DenoiseLevel_gif->setRange(-1, 10);
        ui->spinBox_DenoiseLevel_gif->setValue(4);
        ui->spinBox_DenoiseLevel_gif->setEnabled(1);
        ui->spinBox_DenoiseLevel_gif->setToolTip("Range:-1(No noise reduction)~10");
        ui->label_GIFDenoiseLevel->setToolTip("Range:-1(No noise reduction)~10");
        break;
    }
    case 3:
    {
        DenoiseLevelSpinboxSetting_Anime4k();
        break;
    }
    case 4:
    {
        ui->spinBox_DenoiseLevel_gif->setRange(-1, 3);
        ui->spinBox_DenoiseLevel_gif->setValue(2);
        ui->spinBox_DenoiseLevel_gif->setEnabled(1);
        ui->spinBox_DenoiseLevel_gif->setToolTip("Range:-1(No noise reduction)~3");
        ui->label_GIFDenoiseLevel->setToolTip("Range:-1(No noise reduction)~3");
        break;
    }
    case 5:
    {
        ui->spinBox_DenoiseLevel_gif->setRange(0, 1);
        ui->spinBox_DenoiseLevel_gif->setValue(1);
        ui->spinBox_DenoiseLevel_gif->setEnabled(0);
        ui->spinBox_DenoiseLevel_gif->setToolTip("Realsr-ncnn-vulkan engine will denoise automatically.");
        ui->label_GIFDenoiseLevel->setToolTip("Realsr-ncnn-vulkan engine will denoise automatically.");
        break;
    }
    case 6:
    {
        ui->spinBox_DenoiseLevel_image->setRange(0, 1);
        ui->spinBox_DenoiseLevel_image->setValue(1);
        ui->spinBox_DenoiseLevel_image->setEnabled(0);
        ui->spinBox_DenoiseLevel_image->setToolTip("RealESRGAN-ncnn-vulkan engine will denoise automatically.");
        ui->label_ImageDenoiseLevel->setToolTip("RealESRGAN-ncnn-vulkan engine will denoise automatically.");
        break;
    }
    case 7:
    {
        ui->spinBox_DenoiseLevel_image->setRange(0, 1);
        ui->spinBox_DenoiseLevel_image->setValue(1);
        ui->spinBox_DenoiseLevel_image->setEnabled(0);
        ui->spinBox_DenoiseLevel_image->setToolTip("RealCUGAN-ncnn-vulkan engine will denoise automatically.");
        ui->label_ImageDenoiseLevel->setToolTip("RealCUGAN-ncnn-vulkan engine will denoise automatically.");
        break;
    }
    }
    on_comboBox_model_vulkan_currentIndexChanged(0);
    isWaifu2xCaffeEnabled(); // 判断是否启用caffe图片风格设定
}

void MainWindow::on_comboBox_Engine_Video_currentIndexChanged(int index)
{
    switch (ui->comboBox_Engine_Video->currentIndex())
    {
    case 0:
    {
        ui->spinBox_DenoiseLevel_video->setRange(-1, 3);
        ui->spinBox_DenoiseLevel_video->setValue(2);
        ui->spinBox_DenoiseLevel_video->setEnabled(1);
        ui->spinBox_DenoiseLevel_video->setToolTip("Range:-1(No noise reduction)~3");
        ui->label_VideoDenoiseLevel->setToolTip("Range:-1(No noise reduction)~3");
        break;
    }
    case 1:
    {
        ui->spinBox_DenoiseLevel_video->setRange(0, 3);
        ui->spinBox_DenoiseLevel_video->setValue(2);
        ui->spinBox_DenoiseLevel_video->setEnabled(1);
        ui->spinBox_DenoiseLevel_video->setToolTip("Range:0(No noise reduction)~3");
        ui->label_VideoDenoiseLevel->setToolTip("Range:0(No noise reduction)~3");
        break;
    }
    case 2:
    {
        DenoiseLevelSpinboxSetting_Anime4k();
        break;
    }
    case 3:
    {
        ui->spinBox_DenoiseLevel_video->setRange(-1, 10);
        ui->spinBox_DenoiseLevel_video->setValue(4);
        ui->spinBox_DenoiseLevel_video->setEnabled(1);
        ui->spinBox_DenoiseLevel_video->setToolTip("Range:-1(No noise reduction)~10");
        ui->label_VideoDenoiseLevel->setToolTip("Range:-1(No noise reduction)~10");
        break;
    }
    case 4:
    {
        ui->spinBox_DenoiseLevel_video->setRange(-1, 3);
        ui->spinBox_DenoiseLevel_video->setValue(2);
        ui->spinBox_DenoiseLevel_video->setEnabled(1);
        ui->spinBox_DenoiseLevel_video->setToolTip("Range:-1(No noise reduction)~3");
        ui->label_VideoDenoiseLevel->setToolTip("Range:-1(No noise reduction)~3");
        break;
    }
    case 5:
    {
        ui->spinBox_DenoiseLevel_video->setRange(0, 1);
        ui->spinBox_DenoiseLevel_video->setValue(1);
        ui->spinBox_DenoiseLevel_video->setEnabled(0);
        ui->spinBox_DenoiseLevel_video->setToolTip("Realsr-ncnn-vulkan engine will denoise automatically.");
        ui->label_VideoDenoiseLevel->setToolTip("Realsr-ncnn-vulkan engine will denoise automatically.");
        break;
    }
    case 6:
    {
        ui->spinBox_DenoiseLevel_image->setRange(0, 1);
        ui->spinBox_DenoiseLevel_image->setValue(1);
        ui->spinBox_DenoiseLevel_image->setEnabled(0);
        ui->spinBox_DenoiseLevel_image->setToolTip("RealESRGAN-ncnn-vulkan engine will denoise automatically.");
        ui->label_ImageDenoiseLevel->setToolTip("RealESRGAN-ncnn-vulkan engine will denoise automatically.");
        break;
    }
    case 7:
    {
        ui->spinBox_DenoiseLevel_image->setRange(0, 1);
        ui->spinBox_DenoiseLevel_image->setValue(1);
        ui->spinBox_DenoiseLevel_image->setEnabled(0);
        ui->spinBox_DenoiseLevel_image->setToolTip("RealCUGAN-ncnn-vulkan engine will denoise automatically.");
        ui->label_ImageDenoiseLevel->setToolTip("RealCUGAN-ncnn-vulkan engine will denoise automatically.");
        break;
    }
    }
    on_comboBox_model_vulkan_currentIndexChanged(0);
    isWaifu2xCaffeEnabled(); // 判断是否启用caffe图片风格设定
}

void MainWindow::on_pushButton_clear_textbrowser_clicked()
{
    ui->textBrowser->clear();
    TextBrowser_StartMes();
}

void MainWindow::on_pushButton_CustRes_apply_clicked()
{
    ui->pushButton_CustRes_apply->setEnabled(0);
    CustRes_SetCustRes();
    ui->pushButton_CustRes_apply->setEnabled(1);
}

void MainWindow::on_pushButton_CustRes_cancel_clicked()
{
    ui->pushButton_CustRes_cancel->setEnabled(0);
    CustRes_CancelCustRes();
    ui->pushButton_CustRes_cancel->setEnabled(1);
}

void MainWindow::on_pushButton_HideSettings_clicked()
{
    if (ui->groupBox_Setting->isVisible())
    {
        ui->groupBox_Setting->setVisible(0);
        isSettingsHide = true;
        ui->pushButton_HideSettings->setText("Show settings");
    }
    else
    {
        ui->groupBox_Setting->setVisible(1);
        isSettingsHide = false;
        ui->pushButton_HideSettings->setText("Hide settings");
    }
}

/*
改变语言设置
*/

void MainWindow::on_pushButton_ReadFileList_clicked()
{
    QString Table_FileList_ini = Current_Path + "/Table_FileList.ini";
    if (QFile::exists(Table_FileList_ini))
    {
        ui_tableViews_setUpdatesEnabled(false);
        this->setAcceptDrops(0);             // 禁止drop file
        ui->pushButton_Start->setEnabled(0); // 禁用start button
        ui->pushButton_CustRes_cancel->setEnabled(0);
        ui->pushButton_CustRes_apply->setEnabled(0);
        ui->pushButton_ReadFileList->setEnabled(0);
        ui->pushButton_SaveFileList->setEnabled(0);
        ui->pushButton_BrowserFile->setEnabled(0);
        on_pushButton_ClearList_clicked();
        Send_TextBrowser_NewMessage("Please wait while reading the file.");
        ui->label_DropFile->setText("Loading list, please wait.");
        QtConcurrent::run(this, &MainWindow::Table_Read_Saved_Table_Filelist);
    }
    else
    {
        QMessageBox::warning(this,"Error","Cannot find the saved Files List!");
    }
}

void MainWindow::on_Ext_image_editingFinished()
{
    QString ext_image_str = ui->Ext_image->text();
    ext_image_str = ext_image_str.trimmed().simplified().remove(QRegExp("\\s")).replace(":gif:", ":");
    ui->Ext_image->setText(ext_image_str);
}

void MainWindow::on_Ext_video_editingFinished()
{
    QString ext_video_str = ui->Ext_video->text();
    ext_video_str = ext_video_str.simplified().remove(QRegExp("\\s")).replace(":gif:", ":");
    ui->Ext_video->setText(ext_video_str);
}

void MainWindow::on_checkBox_AutoSaveSettings_clicked()
{
    QString settings_ini = Current_Path + "/settings.ini";
    if (QFile::exists(settings_ini))
    {
        QSettings configIniWrite(settings_ini, QSettings::IniFormat);
        configIniWrite.setValue("/settings/AutoSaveSettings", ui->checkBox_AutoSaveSettings->isChecked());
    }
}

void MainWindow::on_pushButton_about_clicked()
{
    QApplication::aboutQt();
}

void MainWindow::on_comboBox_AspectRatio_custRes_currentIndexChanged(int index)
{
    int CurrentIndex = ui->comboBox_AspectRatio_custRes->currentIndex();
    switch (CurrentIndex)
    {
    case 0:
    {
        CustRes_AspectRatioMode = Qt::IgnoreAspectRatio;
        break;
    }
    case 1:
    {
        CustRes_AspectRatioMode = Qt::KeepAspectRatio;
        break;
    }
    case 2:
    {
        CustRes_AspectRatioMode = Qt::KeepAspectRatioByExpanding;
        break;
    }
    }
}

void MainWindow::on_checkBox_AlwaysHideSettings_stateChanged(int arg1)
{
    if (ui->checkBox_AlwaysHideSettings->isChecked())
    {
        ui->groupBox_Setting->setVisible(0);
        ui->pushButton_HideSettings->setText("Show settings");
        isSettingsHide = true;
    }
}
/*
==================================================================================================
                                      浏览与添加文本
==================================================================================================
*/
void MainWindow::on_pushButton_BrowserFile_clicked()
{
    QString Last_browsed_path = Current_Path + "/LastBrowsedPath.ini";
    //======== 生成 扩展名过滤 字符串 =========
    QStringList nameFilters;
    nameFilters.append("*.gif");
    QString Ext_image_str = ui->Ext_image->text();
    QStringList nameFilters_image = Ext_image_str.split(":");
    nameFilters_image.removeAll("gif");
    for (int i = 0; i < nameFilters_image.size(); ++i)
    {
        QString tmp = nameFilters_image.at(i).trimmed();
        if (tmp == "")
            continue;
        tmp = "*." + tmp;
        nameFilters.append(tmp);
    }
    QString Ext_video_str = ui->Ext_video->text();
    QStringList nameFilters_video = Ext_video_str.split(":");
    nameFilters_video.removeAll("gif");
    for (int i = 0; i < nameFilters_video.size(); ++i)
    {
        QString tmp = nameFilters_video.at(i).trimmed();
        if (tmp == "")
            continue;
        tmp = "*." + tmp;
        nameFilters.append(tmp);
    }
    QString nameFilters_QString = "";
    for (int i = 0; i < nameFilters.size(); ++i)
    {
        QString tmp = nameFilters.at(i).trimmed();
        nameFilters_QString = nameFilters_QString + " " + tmp;
    }
    //=====================================================
    QString BrowserStartPath = ""; // 浏览文件时的起始文件夹
    //=========== 读取上一次浏览的文件夹 ===========================
    if (QFile::exists(Last_browsed_path))
    {
        QSettings configIniRead(Last_browsed_path, QSettings::IniFormat);
        configIniRead.setIniCodec(QTextCodec::codecForName("UTF-8"));
        BrowserStartPath = configIniRead.value("/Path").toString();
        if (!QFile::exists(BrowserStartPath))
            BrowserStartPath = "";
    }
    //===========================================================
    QStringList Input_path_List = QFileDialog::getOpenFileNames(this, "Select files", BrowserStartPath, "All file(" + nameFilters_QString + ")");
    if (Input_path_List.isEmpty())
    {
        return;
    }
    //================== 记住上一次浏览的文件夹 =======================
    QFile::remove(Last_browsed_path);
    QSettings configIniWrite(Last_browsed_path, QSettings::IniFormat);
    configIniWrite.setIniCodec(QTextCodec::codecForName("UTF-8"));
    configIniWrite.setValue("/Warning/Edit", "Carefully!!!");
    QFileInfo lastPath(Input_path_List.at(0));
    QString folder_lastPath = file_getFolderPath(lastPath);
    configIniWrite.setValue("/Path", folder_lastPath);
    //===============================================================
    AddNew_gif = false;
    AddNew_image = false;
    AddNew_video = false;
    //================== 界面管制 ========================
    ui_tableViews_setUpdatesEnabled(false);
    ui->groupBox_Setting->setEnabled(0);
    ui->groupBox_FileList->setEnabled(0);
    ui->groupBox_InputExt->setEnabled(0);
    ui->pushButton_Start->setEnabled(0);
    ui->checkBox_ScanSubFolders->setEnabled(0);
    this->setAcceptDrops(0);
    ui->label_DropFile->setText("Adding files, please wait.");
    emit Send_TextBrowser_NewMessage("Adding files, please wait.");
    //===================================================
    QtConcurrent::run(this, &MainWindow::Read_Input_paths_BrowserFile, Input_path_List);
}
/*
读取 路径与添加文件
*/
void MainWindow::Read_Input_paths_BrowserFile(QStringList Input_path_List)
{
    Progressbar_MaxVal = Input_path_List.size();
    Progressbar_CurrentVal = 0;
    emit Send_PrograssBar_Range_min_max(0, Progressbar_MaxVal);
    foreach (QString Input_path, Input_path_List)
    {
        Input_path = Input_path.trimmed();
        if (QFile::exists(Input_path) == false)
            continue;
        Add_File_Folder(Input_path);
        emit Send_progressbar_Add();
    }
    emit Send_Read_urls_finfished();
}

void MainWindow::on_pushButton_HideTextBro_clicked()
{
    if (ui->textBrowser->isVisible())
    {
        ui->textBrowser->setVisible(0);
        ui->pushButton_HideTextBro->setText("Show Text Browser");
    }
    else
    {
        ui->textBrowser->setVisible(1);
        ui->pushButton_HideTextBro->setText("Hide Text Browser");
    }
}

void MainWindow::on_checkBox_AlwaysHideTextBrowser_stateChanged(int arg1)
{
    if (ui->checkBox_AlwaysHideTextBrowser->isChecked())
    {
        ui->textBrowser->setVisible(0);
        ui->pushButton_HideTextBro->setText("Show Text Browser");
    }
}

void MainWindow::on_Ext_image_textChanged(const QString &arg1)
{
    QString lower = ui->Ext_image->text().toLower();
    ui->Ext_image->setText(lower);
}

void MainWindow::on_Ext_video_textChanged(const QString &arg1)
{
    QString lower = ui->Ext_video->text().toLower();
    ui->Ext_video->setText(lower);
}

void MainWindow::on_comboBox_model_vulkan_currentIndexChanged(int index)
{
    if (ui->comboBox_model_vulkan->currentIndex() == 0)
    {
        ui->comboBox_ImageStyle->setEnabled(1);
        ui->label_ImageStyle_W2xNCNNVulkan->setVisible(1);
        ui->comboBox_ImageStyle->setVisible(1);
        if (ui->comboBox_Engine_Image->currentIndex() != 0 && ui->comboBox_Engine_GIF->currentIndex() != 0 && ui->comboBox_Engine_Video->currentIndex() != 0)
        {
            ui->comboBox_ImageStyle->setEnabled(0);
            ui->label_ImageStyle_W2xNCNNVulkan->setVisible(0);
            ui->comboBox_ImageStyle->setVisible(0);
        }
    }
    if (ui->comboBox_model_vulkan->currentIndex() == 1)
    {
        ui->comboBox_ImageStyle->setEnabled(0);
        ui->label_ImageStyle_W2xNCNNVulkan->setVisible(0);
        ui->comboBox_ImageStyle->setVisible(0);
    }
}

void MainWindow::on_comboBox_ImageStyle_currentIndexChanged(int index)
{
    if (ui->comboBox_ImageStyle->currentIndex() == 0)
    {
        ui->comboBox_model_vulkan->setEnabled(1);
    }
    if (ui->comboBox_ImageStyle->currentIndex() == 1)
    {
        ui->comboBox_model_vulkan->setEnabled(0);
    }
}

void MainWindow::on_pushButton_ResetVideoSettings_clicked()
{
    ui->lineEdit_pixformat->setText("yuv420p");
    ui->lineEdit_encoder_vid->setText("libx264");
    ui->lineEdit_encoder_audio->setText("aac");
    ui->spinBox_bitrate_vid->setValue(6000);
    ui->spinBox_bitrate_audio->setValue(320);
    //====
    ui->spinBox_bitrate_vid_2mp4->setValue(2500);
    ui->spinBox_bitrate_audio_2mp4->setValue(320);
    ui->checkBox_acodec_copy_2mp4->setChecked(0);
    ui->checkBox_vcodec_copy_2mp4->setChecked(0);
    ui->spinBox_bitrate_vid_2mp4->setEnabled(1);
    ui->spinBox_bitrate_audio_2mp4->setEnabled(1);
    //====
    ui->lineEdit_ExCommand_2mp4->setText("");
    ui->lineEdit_ExCommand_output->setText("");
}

void MainWindow::on_lineEdit_encoder_vid_textChanged(const QString &arg1)
{
    QString tmp = ui->lineEdit_encoder_vid->text().trimmed();
    ui->lineEdit_encoder_vid->setText(tmp);
}

void MainWindow::on_lineEdit_encoder_audio_textChanged(const QString &arg1)
{
    QString tmp = ui->lineEdit_encoder_audio->text().trimmed();
    ui->lineEdit_encoder_audio->setText(tmp);
}

void MainWindow::on_lineEdit_pixformat_textChanged(const QString &arg1)
{
    QString tmp = ui->lineEdit_pixformat->text().trimmed();
    ui->lineEdit_pixformat->setText(tmp);
}

void MainWindow::on_checkBox_vcodec_copy_2mp4_stateChanged(int arg1)
{
    if (ui->checkBox_vcodec_copy_2mp4->isChecked())
    {
        ui->spinBox_bitrate_vid_2mp4->setEnabled(0);
    }
    else
    {
        ui->spinBox_bitrate_vid_2mp4->setEnabled(1);
    }
}

void MainWindow::on_checkBox_acodec_copy_2mp4_stateChanged(int arg1)
{
    if (ui->checkBox_acodec_copy_2mp4->isChecked())
    {
        ui->spinBox_bitrate_audio_2mp4->setEnabled(0);
    }
    else
    {
        ui->spinBox_bitrate_audio_2mp4->setEnabled(1);
    }
}

void MainWindow::on_pushButton_encodersList_clicked()
{
    QString ffmpeg = Current_Path + "/ffmpeg.exe";
    QStringList args = {"-hide_banner", "-loglevel", "panic", "-encoders"};
    QString text = cmdGetText(ffmpeg, args);
    QtConcurrent::run(this, &MainWindow::showTextExt, QString("FFmpeg encoders list"), text);
}

void MainWindow::Tip_FirstTimeStart()
{
    QString FirstTimeStart = Current_Path + "/FirstTimeStart";
    if (QFile::exists(FirstTimeStart))
    {
        isFirstTimeStart = false;
        return;
    }
    else
    {
        isFirstTimeStart = true;
        //======
        QMessageBox::information(this,"Notification","It is detected that this is the first time you have started the software, so the compatibility test will be performed automatically. Please wait for a while, then check the test result.");
        //=======
        QFile file(FirstTimeStart);
        file.remove();
        if (file.open(QIODevice::ReadWrite | QIODevice::Text)) // QIODevice::ReadWrite支持读写
        {
            QTextStream stream(&file);
            stream << "PLACEHOLDER";
        }
        //=======
        on_pushButton_clear_textbrowser_clicked();
        on_pushButton_compatibilityTest_clicked();
    }
}

void MainWindow::on_checkBox_DelOriginal_stateChanged(int arg1)
{
    if (ui->checkBox_DelOriginal->isChecked())
    {
        QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->setEnabled(1);
        ui->checkBox_ReplaceOriginalFile->setEnabled(0);
        ui->checkBox_ReplaceOriginalFile->setChecked(0);
    }
    else
    {
        QAction_checkBox_MoveToRecycleBin_checkBox_DelOriginal->setEnabled(0);
        checkBox_ReplaceOriginalFile_setEnabled_True_Self();
    }
}

void MainWindow::on_checkBox_FileList_Interactive_stateChanged(int arg1)
{
    if (ui->checkBox_FileList_Interactive->isChecked())
    {
        ui->tableView_image->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        ui->tableView_gif->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        ui->tableView_video->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }
    else
    {
        ui->tableView_image->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView_gif->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView_video->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
}

void MainWindow::on_checkBox_OutPath_isEnabled_stateChanged(int arg1)
{
    if (ui->checkBox_OutPath_isEnabled->isChecked())
    {
        ui->lineEdit_outputPath->setEnabled(1);
        ui->pushButton_browser_path->setEnabled(1);
        ui->checkBox_OutPath_KeepOriginalFileName->setEnabled(1);
        ui->checkBox_KeepParentFolder->setEnabled(1);
        ui->checkBox_OutPath_Overwrite->setEnabled(1);
        ui->checkBox_ReplaceOriginalFile->setEnabled(0);
        ui->checkBox_ReplaceOriginalFile->setChecked(0);
    }
    else
    {
        ui->lineEdit_outputPath->setEnabled(0);
        ui->pushButton_browser_path->setEnabled(0);
        ui->checkBox_OutPath_KeepOriginalFileName->setEnabled(0);
        ui->checkBox_KeepParentFolder->setEnabled(0);
        ui->checkBox_OutPath_Overwrite->setEnabled(0);
        checkBox_ReplaceOriginalFile_setEnabled_True_Self();
    }
}

// 强制重试
void MainWindow::on_pushButton_ForceRetry_clicked()
{
    ui->pushButton_ForceRetry->setEnabled(0);
    //========
    QtConcurrent::run(this, &MainWindow::isForceRetryClicked_SetTrue_Block_Anime4k); // block a4k引擎线程 防止无效图片污染缓存
    //========
    ForceRetryCount++;
    //========
    taskKill("Anime4K.exe");
    taskKill("waifu2x-ncnn-vulkan.exe");
    taskKill("waifu2x-converter-cpp.exe");
    taskKill("srmd-ncnn-vulkan.exe");
    taskKill("waifu2x-caffe.exe");
    taskKill("realsr-ncnn-vulkan.exe");
    taskKill("realesrgan-ncnn-vulkan.exe");
    taskKill("realcugan-ncnn-vulkan.exe");
    //========
    emit Send_TextBrowser_NewMessage("Force retry.");
    return;
}
// 激活强制重试按钮
void MainWindow::SetEnable_pushButton_ForceRetry_self()
{
    ui->pushButton_ForceRetry->setEnabled(1);
    return;
}

void MainWindow::on_checkBox_AudioDenoise_stateChanged(int arg1)
{
    if (ui->checkBox_AudioDenoise->isChecked())
    {
        ui->doubleSpinBox_AudioDenoiseLevel->setEnabled(1);
    }
    else
    {
        ui->doubleSpinBox_AudioDenoiseLevel->setEnabled(0);
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (ui->tabWidget->currentIndex())
    {
    case 0:
    {

        // tab 1
        ui->groupBox_Progress->setVisible(1);
        ui->splitter_2->setVisible(1);
        if (isSettingsHide == false)
        {
            ui->groupBox_Setting->setVisible(1);
        }
        // tab 2
        ui->groupBox_Engine->setVisible(0);
        ui->groupBox_NumOfThreads->setVisible(0);
        // tab 3
        ui->groupBox_AudioDenoise->setVisible(0);
        ui->groupBox_video_settings->setVisible(0);
        // tab 4
        ui->groupBox_8->setVisible(0);
        ui->groupBox_InputExt->setVisible(0);
        ui->groupBox_other_1->setVisible(0);
        // tab 5
        ui->groupBox_CompatibilityTestRes->setVisible(0);
        ui->pushButton_compatibilityTest->setVisible(0);
        break;
    }
    case 1:
    {

        // tab 1
        ui->groupBox_Progress->setVisible(0);
        ui->splitter_2->setVisible(0);
        // tab 2
        ui->groupBox_Engine->setVisible(1);
        ui->groupBox_NumOfThreads->setVisible(1);
        // tab 3
        ui->groupBox_AudioDenoise->setVisible(0);
        ui->groupBox_video_settings->setVisible(0);
        // tab 4
        ui->groupBox_8->setVisible(0);
        ui->groupBox_InputExt->setVisible(0);
        ui->groupBox_other_1->setVisible(0);
        // tab 5
        ui->groupBox_CompatibilityTestRes->setVisible(0);
        ui->pushButton_compatibilityTest->setVisible(0);
        break;
    }
    case 2:
    {

        // tab 1
        ui->groupBox_Progress->setVisible(0);
        ui->splitter_2->setVisible(0);
        // tab 2
        ui->groupBox_Engine->setVisible(0);
        ui->groupBox_NumOfThreads->setVisible(0);
        // tab 3
        ui->groupBox_AudioDenoise->setVisible(1);
        ui->groupBox_video_settings->setVisible(1);
        // tab 4
        ui->groupBox_8->setVisible(0);
        ui->groupBox_InputExt->setVisible(0);
        ui->groupBox_other_1->setVisible(0);
        // tab 5
        ui->groupBox_CompatibilityTestRes->setVisible(0);
        ui->pushButton_compatibilityTest->setVisible(0);
        break;
    }
    case 3:
    {

        // tab 1
        ui->groupBox_Progress->setVisible(0);
        ui->splitter_2->setVisible(0);
        // tab 2
        ui->groupBox_Engine->setVisible(0);
        ui->groupBox_NumOfThreads->setVisible(0);
        // tab 3
        ui->groupBox_AudioDenoise->setVisible(0);
        ui->groupBox_video_settings->setVisible(0);
        // tab 4
        ui->groupBox_8->setVisible(1);
        ui->groupBox_InputExt->setVisible(1);
        ui->groupBox_other_1->setVisible(1);
        // tab 5
        ui->groupBox_CompatibilityTestRes->setVisible(0);
        ui->pushButton_compatibilityTest->setVisible(0);
        break;
    }
    case 4:
    {

        // tab 1
        ui->groupBox_Progress->setVisible(0);
        ui->splitter_2->setVisible(0);
        // tab 2
        ui->groupBox_Engine->setVisible(0);
        ui->groupBox_NumOfThreads->setVisible(0);
        // tab 3
        ui->groupBox_AudioDenoise->setVisible(0);
        ui->groupBox_video_settings->setVisible(0);
        // tab 4
        ui->groupBox_8->setVisible(0);
        ui->groupBox_InputExt->setVisible(0);
        ui->groupBox_other_1->setVisible(0);
        // tab 5
        ui->groupBox_CompatibilityTestRes->setVisible(1);
        ui->pushButton_compatibilityTest->setVisible(1);
        break;
    }
    }
}

void MainWindow::on_checkBox_ProcessVideoBySegment_stateChanged(int arg1)
{
    if (ui->checkBox_ProcessVideoBySegment->isChecked())
    {
        ui->label_SegmentDuration->setEnabled(1);
        ui->spinBox_SegmentDuration->setEnabled(1);
    }
    else
    {
        ui->label_SegmentDuration->setEnabled(0);
        ui->spinBox_SegmentDuration->setEnabled(0);
    }
}


void MainWindow::on_checkBox_EnablePreProcessing_Anime4k_stateChanged(int arg1)
{
    if (ui->checkBox_EnablePreProcessing_Anime4k->isChecked())
    {
        ui->checkBox_MedianBlur_Pre_Anime4k->setEnabled(1);
        ui->checkBox_MeanBlur_Pre_Anime4k->setEnabled(1);
        ui->checkBox_CASSharping_Pre_Anime4k->setEnabled(1);
        ui->checkBox_GaussianBlurWeak_Pre_Anime4k->setEnabled(1);
        ui->checkBox_GaussianBlur_Pre_Anime4k->setEnabled(1);
        ui->checkBox_BilateralFilter_Pre_Anime4k->setEnabled(1);
        ui->checkBox_BilateralFilterFaster_Pre_Anime4k->setEnabled(1);
    }
    else
    {
        ui->checkBox_MedianBlur_Pre_Anime4k->setEnabled(0);
        ui->checkBox_MeanBlur_Pre_Anime4k->setEnabled(0);
        ui->checkBox_CASSharping_Pre_Anime4k->setEnabled(0);
        ui->checkBox_GaussianBlurWeak_Pre_Anime4k->setEnabled(0);
        ui->checkBox_GaussianBlur_Pre_Anime4k->setEnabled(0);
        ui->checkBox_BilateralFilter_Pre_Anime4k->setEnabled(0);
        ui->checkBox_BilateralFilterFaster_Pre_Anime4k->setEnabled(0);
    }
}

void MainWindow::on_checkBox_EnablePostProcessing_Anime4k_stateChanged(int arg1)
{
    if (ui->checkBox_EnablePostProcessing_Anime4k->isChecked())
    {
        ui->checkBox_MedianBlur_Post_Anime4k->setEnabled(1);
        ui->checkBox_MeanBlur_Post_Anime4k->setEnabled(1);
        ui->checkBox_CASSharping_Post_Anime4k->setEnabled(1);
        ui->checkBox_GaussianBlurWeak_Post_Anime4k->setEnabled(1);
        ui->checkBox_GaussianBlur_Post_Anime4k->setEnabled(1);
        ui->checkBox_BilateralFilter_Post_Anime4k->setEnabled(1);
        ui->checkBox_BilateralFilterFaster_Post_Anime4k->setEnabled(1);
    }
    else
    {
        ui->checkBox_MedianBlur_Post_Anime4k->setEnabled(0);
        ui->checkBox_MeanBlur_Post_Anime4k->setEnabled(0);
        ui->checkBox_CASSharping_Post_Anime4k->setEnabled(0);
        ui->checkBox_GaussianBlurWeak_Post_Anime4k->setEnabled(0);
        ui->checkBox_GaussianBlur_Post_Anime4k->setEnabled(0);
        ui->checkBox_BilateralFilter_Post_Anime4k->setEnabled(0);
        ui->checkBox_BilateralFilterFaster_Post_Anime4k->setEnabled(0);
    }
}

void MainWindow::on_checkBox_SpecifyGPU_Anime4k_stateChanged(int arg1)
{
    if (ui->checkBox_SpecifyGPU_Anime4k->isChecked())
    {
        ui->lineEdit_GPUs_Anime4k->setEnabled(1);
        ui->pushButton_ListGPUs_Anime4k->setEnabled(1);
        ui->pushButton_VerifyGPUsConfig_Anime4k->setEnabled(1);
    }
    else
    {
        ui->lineEdit_GPUs_Anime4k->setEnabled(0);
        ui->pushButton_ListGPUs_Anime4k->setEnabled(0);
        ui->pushButton_VerifyGPUsConfig_Anime4k->setEnabled(0);
    }
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_NCNN_Vulkan_NEW->setChecked(isCompatible_Waifu2x_NCNN_Vulkan_NEW);
}

void MainWindow::on_checkBox_isCompatible_SRMD_NCNN_Vulkan_clicked()
{
    ui->checkBox_isCompatible_SRMD_NCNN_Vulkan->setChecked(isCompatible_SRMD_NCNN_Vulkan);
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_Converter_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_Converter->setChecked(isCompatible_Waifu2x_Converter);
}

void MainWindow::on_checkBox_isCompatible_Anime4k_CPU_clicked()
{
    ui->checkBox_isCompatible_Anime4k_CPU->setChecked(isCompatible_Anime4k_CPU);
}

void MainWindow::on_checkBox_isCompatible_Anime4k_GPU_clicked()
{
    ui->checkBox_isCompatible_Anime4k_GPU->setChecked(isCompatible_Anime4k_GPU);
}

void MainWindow::on_checkBox_isCompatible_FFmpeg_clicked()
{
    ui->checkBox_isCompatible_FFmpeg->setChecked(isCompatible_FFmpeg);
}

void MainWindow::on_checkBox_isCompatible_FFprobe_clicked()
{
    ui->checkBox_isCompatible_FFprobe->setChecked(isCompatible_FFprobe);
}

void MainWindow::on_checkBox_isCompatible_ImageMagick_clicked()
{
    ui->checkBox_isCompatible_ImageMagick->setChecked(isCompatible_ImageMagick);
}

void MainWindow::on_checkBox_isCompatible_Gifsicle_clicked()
{
    ui->checkBox_isCompatible_Gifsicle->setChecked(isCompatible_Gifsicle);
}

void MainWindow::on_checkBox_isCompatible_SoX_clicked()
{
    ui->checkBox_isCompatible_SoX->setChecked(isCompatible_SoX);
}

void MainWindow::on_checkBox_GPUMode_Anime4K_stateChanged(int arg1)
{
    if (ui->checkBox_GPUMode_Anime4K->isChecked())
    {
        ui->checkBox_SpecifyGPU_Anime4k->setEnabled(1);
        ui->comboBox_GPGPUModel_A4k->setEnabled(1);
        on_comboBox_GPGPUModel_A4k_currentIndexChanged(1);
    }
    else
    {
        ui->checkBox_SpecifyGPU_Anime4k->setEnabled(0);
        ui->checkBox_SpecifyGPU_Anime4k->setChecked(0);
        ui->comboBox_GPGPUModel_A4k->setEnabled(0);
        on_comboBox_GPGPUModel_A4k_currentIndexChanged(1);
    }
}

void MainWindow::on_checkBox_ShowInterPro_stateChanged(int arg1)
{
    if (ui->checkBox_ShowInterPro->isChecked() == false)
    {
        emit Send_CurrentFileProgress_Stop();
    }
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_Caffe_CPU_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_Caffe_CPU->setChecked(isCompatible_Waifu2x_Caffe_CPU);
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_Caffe_GPU_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_Caffe_GPU->setChecked(isCompatible_Waifu2x_Caffe_GPU);
}

void MainWindow::on_checkBox_isCompatible_Waifu2x_Caffe_cuDNN_clicked()
{
    ui->checkBox_isCompatible_Waifu2x_Caffe_cuDNN->setChecked(isCompatible_Waifu2x_Caffe_cuDNN);
}

void MainWindow::on_pushButton_SplitSize_Add_Waifu2xCaffe_clicked()
{
    int VAL = ui->spinBox_SplitSize_Waifu2xCaffe->value() * 2;
    if (VAL <= 999999999)
    {
        ui->spinBox_SplitSize_Waifu2xCaffe->setValue(VAL);
    }
}

void MainWindow::on_pushButton_SplitSize_Minus_Waifu2xCaffe_clicked()
{
    int VAL = ui->spinBox_SplitSize_Waifu2xCaffe->value() / 2;
    if (VAL >= 2)
    {
        ui->spinBox_SplitSize_Waifu2xCaffe->setValue(VAL);
    }
}

void MainWindow::on_checkBox_isCompatible_Realsr_NCNN_Vulkan_clicked()
{
    ui->checkBox_isCompatible_Realsr_NCNN_Vulkan->setChecked(isCompatible_Realsr_NCNN_Vulkan);
}

void MainWindow::on_checkBox_isCompatible_RealESRGAN_NCNN_Vulkan_clicked()
{
    ui->checkBox_isCompatible_RealESRGAN_NCNN_Vulkan->setChecked(isCompatible_Realsr_NCNN_Vulkan);
}

void MainWindow::on_checkBox_isCompatible_RealCUGAN_NCNN_Vulkan_clicked()
{
    ui->checkBox_isCompatible_RealCUGAN_NCNN_Vulkan->setChecked(isCompatible_Realsr_NCNN_Vulkan);
}

void MainWindow::on_checkBox_ACNet_Anime4K_stateChanged(int arg1)
{
    if (ui->checkBox_ACNet_Anime4K->isChecked())
    {
        ui->checkBox_HDNMode_Anime4k->setEnabled(1);
        ui->groupBox_PostProcessing_Anime4k->setEnabled(0);
        ui->groupBox_PreProcessing_Anime4k->setEnabled(0);
        ui->doubleSpinBox_PushColorStrength_Anime4K->setEnabled(0);
        ui->doubleSpinBox_PushGradientStrength_Anime4K->setEnabled(0);
        ui->spinBox_Passes_Anime4K->setEnabled(0);
        ui->spinBox_PushColorCount_Anime4K->setEnabled(0);
    }
    else
    {
        ui->checkBox_HDNMode_Anime4k->setEnabled(0);
        on_checkBox_GPUMode_Anime4K_stateChanged(0);
        on_checkBox_SpecifyGPU_Anime4k_stateChanged(0);
        ui->groupBox_PostProcessing_Anime4k->setEnabled(1);
        ui->groupBox_PreProcessing_Anime4k->setEnabled(1);
        ui->doubleSpinBox_PushColorStrength_Anime4K->setEnabled(1);
        ui->doubleSpinBox_PushGradientStrength_Anime4K->setEnabled(1);
        ui->spinBox_Passes_Anime4K->setEnabled(1);
        ui->spinBox_PushColorCount_Anime4K->setEnabled(1);
    }
    DenoiseLevelSpinboxSetting_Anime4k();
}

void MainWindow::on_checkBox_HDNMode_Anime4k_stateChanged(int arg1)
{
    DenoiseLevelSpinboxSetting_Anime4k();
}

void MainWindow::on_checkBox_ReplaceOriginalFile_stateChanged(int arg1)
{
    if (ui->checkBox_ReplaceOriginalFile->isChecked())
    {
        ui->groupBox_OutPut->setEnabled(0);
        ui->checkBox_OutPath_isEnabled->setChecked(0);
        ui->checkBox_DelOriginal->setEnabled(0);
    }
    else
    {
        ui->groupBox_OutPut->setEnabled(1);
        ui->checkBox_DelOriginal->setEnabled(1);
    }
}

void MainWindow::checkBox_ReplaceOriginalFile_setEnabled_True_Self()
{
    if (ui->checkBox_DelOriginal->isChecked() == false && ui->checkBox_OutPath_isEnabled->isChecked() == false)
    {
        ui->checkBox_ReplaceOriginalFile->setEnabled(1);
    }
}

bool MainWindow::ReplaceOriginalFile(QString original_fullpath, QString output_fullpath)
{
    // 检查是否启用替换源文件,以及输出文件是否存在
    if (ui->checkBox_ReplaceOriginalFile->isChecked() == false || QFile::exists(output_fullpath) == false)
        return false;
    //=================
    QFileInfo fileinfo_original_fullpath(original_fullpath);
    QFileInfo fileinfo_output_fullpath(output_fullpath);
    QString file_name = file_getBaseName(original_fullpath);            // 获取源文件的文件名
    QString file_ext = fileinfo_output_fullpath.suffix();               // 获取输出文件的后辍
    QString file_path = file_getFolderPath(fileinfo_original_fullpath); // 获取源文件的文件路径
    //=================
    QString Target_fullpath = file_path + "/" + file_name + "." + file_ext;
    //=================
    if (QAction_checkBox_MoveToRecycleBin_checkBox_ReplaceOriginalFile->isChecked())
    {
        file_MoveToTrash(original_fullpath);
    }
    else
    {
        QFile::remove(original_fullpath);
    }
    //=================
    if (QFile::rename(output_fullpath, Target_fullpath) == false)
    {
        emit Send_TextBrowser_NewMessage("Error! Failed to move [" + output_fullpath + "] to [" + Target_fullpath + "]");
    }
    return true;
}

void MainWindow::OutputSettingsArea_setEnabled(bool isEnabled)
{
    ui->scrollArea_outputPathSettings->setEnabled(isEnabled);
    ui->lineEdit_outputPath->setClearButtonEnabled(isEnabled);
    if (isEnabled == true)
    {
        ui->lineEdit_outputPath->setFocusPolicy(Qt::StrongFocus);
    }
    else
    {
        ui->lineEdit_outputPath->setFocusPolicy(Qt::NoFocus);
    }
}

// 事件过滤器
bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    //=============================
    // 按下 Delete 移除文件列表里的文件
    //=============================
    if (target == ui->tableView_image || target == ui->tableView_gif || target == ui->tableView_video)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Delete)
            {
                on_pushButton_RemoveItem_clicked();
                return true;
            }
            if ((keyEvent->modifiers() == Qt::ControlModifier) && (keyEvent->key() == Qt::Key_A))
            {
                if (Apply_CustRes_QAction_FileList->isEnabled())
                {
                    EnableApply2All_CustRes = false;
                    on_pushButton_CustRes_apply_clicked();
                    EnableApply2All_CustRes = true;
                }
                return true;
            }
            if ((keyEvent->modifiers() == Qt::ControlModifier) && (keyEvent->key() == Qt::Key_C))
            {
                if (Cancel_CustRes_QAction_FileList->isEnabled())
                {
                    EnableApply2All_CustRes = false;
                    on_pushButton_CustRes_cancel_clicked();
                    EnableApply2All_CustRes = true;
                }
                return true;
            }
        }
    }
    //==============
    return false;
}

void MainWindow::on_pushButton_ResizeFilesListSplitter_clicked()
{
    ui->splitter_FilesList->setSizes(QList<int>() << 1 << 1 << 1);
}

void MainWindow::on_comboBox_GPGPUModel_A4k_currentIndexChanged(int index)
{
    if (ui->comboBox_GPGPUModel_A4k->currentText().toLower().trimmed() == "opencl" && ui->checkBox_GPUMode_Anime4K->isChecked())
    {
        ui->spinBox_OpenCLCommandQueues_A4k->setEnabled(1);
        ui->checkBox_OpenCLParallelIO_A4k->setEnabled(1);
    }
    else
    {
        ui->spinBox_OpenCLCommandQueues_A4k->setEnabled(0);
        ui->checkBox_OpenCLParallelIO_A4k->setEnabled(0);
    }
}

void MainWindow::on_checkBox_DisableGPU_converter_stateChanged(int arg1)
{
    if (ui->checkBox_DisableGPU_converter->isChecked())
    {
        ui->checkBox_MultiGPU_Waifu2xConverter->setChecked(0);
        ui->comboBox_TargetProcessor_converter->setEnabled(0);
        ui->comboBox_TargetProcessor_converter->setCurrentIndex(0);
        ui->checkBox_MultiGPU_Waifu2xConverter->setEnabled(0);
    }
    else
    {
        ui->comboBox_TargetProcessor_converter->setEnabled(1);
        ui->checkBox_MultiGPU_Waifu2xConverter->setEnabled(1);
    }
}

void MainWindow::on_groupBox_video_settings_clicked()
{
    if (ui->groupBox_video_settings->isChecked())
    {
        ui->groupBox_OutputVideoSettings->setEnabled(1);
        ui->groupBox_ToMp4VideoSettings->setEnabled(1);
        ui->pushButton_encodersList->setEnabled(1);
        ui->pushButton_ResetVideoSettings->setEnabled(1);
    }
    else
    {
        ui->groupBox_OutputVideoSettings->setEnabled(0);
        ui->groupBox_ToMp4VideoSettings->setEnabled(0);
        ui->pushButton_encodersList->setEnabled(0);
        ui->pushButton_ResetVideoSettings->setEnabled(0);
    }
}

void MainWindow::showTextExt(QString title, QString msg)
{
    // use external program to show message
    // use qt message box may cause crash, and it is not powerful
    QProcess *p = new QProcess;
    QStringList args = {title, msg};
    p->start(WECHO_Path, args);
    waitForProcess(p);
    delete p;
}

void MainWindow::on_pushButton_view_textbrowser_clicked()
{
    QtConcurrent::run(this,&MainWindow::showTextExt,QString("View Log"),ui->textBrowser->toPlainText());
}


void MainWindow::on_pushButton_browser_path_clicked()
{
    QString Last_browsed_path = Current_Path + "/LastBrowsedPath.ini";
    QString BrowserStartPath = ""; // 浏览文件时的起始文件夹
    //=========== 读取上一次浏览的文件夹 ===========================
    if (QFile::exists(Last_browsed_path))
    {
        QSettings configIniRead(Last_browsed_path, QSettings::IniFormat);
        configIniRead.setIniCodec(QTextCodec::codecForName("UTF-8"));
        BrowserStartPath = configIniRead.value("/Path").toString();
        if (!QFile::exists(BrowserStartPath))
            BrowserStartPath = "";
    }
    //===========================================================
    QString output_path = QFileDialog::getExistingDirectory(this, "Select Directory", BrowserStartPath);
    if (output_path.isNull() || output_path.isEmpty())
    {
        return;
    }
    //================== 记住上一次浏览的文件夹 =======================
    QFile::remove(Last_browsed_path);
    QSettings configIniWrite(Last_browsed_path, QSettings::IniFormat);
    configIniWrite.setIniCodec(QTextCodec::codecForName("UTF-8"));
    configIniWrite.setValue("/Warning/Edit", "Carefully!!!");
    configIniWrite.setValue("/Path", output_path);
    //===============================================================
    ui->lineEdit_outputPath->setText(output_path);
}

