
QT       += core gui
QT       += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.

SOURCES += \
    Anime4K.cpp \
    CompatibilityTest.cpp \
    Current_File_Progress.cpp \
    CustomResolution.cpp \
    Right-click_Menu.cpp \
    SystemTrayIcon.cpp \
    files.cpp \
    gif.cpp \
    image.cpp \
    main.cpp \
    mainwindow.cpp \
    progressBar.cpp \
    realsr_ncnn_vulkan.cpp \
    realesrgan_ncnn_vulkan.cpp \
    realcugan_ncnn_vulkan.cpp \
    settings.cpp \
    srmd_ncnn_vulkan.cpp \
    table.cpp \
    textBrowser.cpp \
    utils.cpp \
    video.cpp \
    waifu2x.cpp \
    waifu2x_caffe.cpp \
    waifu2x_converter.cpp \
    waifu2x_ncnn_vulkan.cpp

HEADERS += \
    mainwindow.h \
    utils.h

FORMS += \
    mainwindow.ui

TARGET = Waifu2x-Extension-GUI

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Style/icon.qrc \
    Style/Breeze.qrc \
    Style/font.qrc \
    Style/splash.qrc

RC_ICONS = Style/icon/icon.ico
