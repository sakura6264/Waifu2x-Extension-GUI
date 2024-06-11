
#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
托盘图标的实现参考了来自简书的文章:
<<Qt5 程序系统托盘>>--小Q_wang
https://www.jianshu.com/p/a000044f1f4a
*/

/*
初始化托盘图标
*/
void MainWindow::Init_SystemTrayIcon()
{
    // 初始化图标
    systemTray->setIcon(*MainIcon_QIcon);
    systemTray->setToolTip("Waifu2x-Extension-GUI\nRight-click to show the menu.");
    // 初始化点击动作
    connect(systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    // 初始化菜单动作
    minimumAct_SystemTrayIcon->setText("Hide");
    minimumAct_SystemTrayIcon->setToolTip("Use the middle mouse button to click the\ntaskbar icon to quickly hide the window.");
    //===
    restoreAct_SystemTrayIcon->setText("Show");
    restoreAct_SystemTrayIcon->setToolTip("Use the left mouse button to click the taskbar icon to quickly\ndisplay the window, double-click to maximize the window.");
    //===
    quitAct_SystemTrayIcon->setText("Exit");
    //===
    BackgroundModeAct_SystemTrayIcon->setText("Background mode");
    BackgroundModeAct_SystemTrayIcon->setToolTip("Set the number of threads to \"1\" to reduce background resource usage.");
    //===
    connect(minimumAct_SystemTrayIcon, SIGNAL(triggered()), this, SLOT(SystemTray_hide_self()));
    connect(restoreAct_SystemTrayIcon, SIGNAL(triggered()), this, SLOT(SystemTray_showNormal_self()));
    connect(quitAct_SystemTrayIcon, SIGNAL(triggered()), this, SLOT(close()), Qt::UniqueConnection);
    connect(BackgroundModeAct_SystemTrayIcon, SIGNAL(triggered()), this, SLOT(EnableBackgroundMode_SystemTray()));
    // 初始化菜单选项
    pContextMenu->setToolTipsVisible(1);
    pContextMenu->setToolTipDuration(-1);
    pContextMenu->addAction(minimumAct_SystemTrayIcon);
    pContextMenu->addAction(restoreAct_SystemTrayIcon);
    pContextMenu->addSeparator();
    pContextMenu->addAction(BackgroundModeAct_SystemTrayIcon);
    pContextMenu->addSeparator();
    pContextMenu->addAction(quitAct_SystemTrayIcon);
    systemTray->setContextMenu(pContextMenu);
    // 显示托盘图标
    systemTray->show();
}
/*
托盘图标点击动作
*/
void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        // 单击托盘图标,正常大小显示(如果之前是全屏,那么会变回正常窗口)
        this->showNormal();
        this->activateWindow();
        this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        break;
    case QSystemTrayIcon::DoubleClick:
        // 双击托盘图标,全屏显示
        this->showMaximized();
        this->activateWindow();
        this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        break;
    case QSystemTrayIcon::MiddleClick:
        // 鼠标中键点击托盘图标,隱藏窗口
        this->hide();
        break;
    default:
        break;
    }
}
/*
启用后台模式
(把所有线程数量改到 1 来减少后台资源占用)
*/
void MainWindow::EnableBackgroundMode_SystemTray()
{
    ui->spinBox_ThreadNum_image->setValue(1);
    ui->spinBox_ThreadNum_gif_internal->setValue(1);
    ui->spinBox_ThreadNum_video_internal->setValue(1);
    pContextMenu->hide();
}
/*
托盘消息
*/
void MainWindow::SystemTray_NewMessage(QString message)
{
    systemTray->showMessage("Notification", message, *MainIcon_QIcon, 5000);
}

void MainWindow::SystemTray_hide_self()
{
    this->hide();
    pContextMenu->hide();
}

void MainWindow::SystemTray_showNormal_self()
{
    this->showNormal();
    pContextMenu->hide();
}
