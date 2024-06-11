﻿
#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
Apply自定义分辨率
*/
int MainWindow::CustRes_SetCustRes()
{
    if (ui->checkBox_custres_isAll->isChecked() && EnableApply2All_CustRes)
    {
        int row_count_image = Table_image_get_rowNum();
        int row_count_gif = Table_gif_get_rowNum();
        int row_count_video = Table_video_get_rowNum();
        //====
        if ((row_count_image + row_count_gif + row_count_video) == 0)
        {
            QMessageBox::warning(this,"Error","No items are in the list.");
            return 0;
        }
        //====
        for (int i = 0; i < row_count_image; i++)
        {
            QMap<QString, QString> res_map;
            // 读取文件信息
            QString SourceFile_fullPath = Table_model_image->item(i, 2)->text();
            CustRes_remove(SourceFile_fullPath); // 移除原来的设定,防止重复
            res_map["fullpath"] = SourceFile_fullPath;
            res_map["height"] = QString::number(ui->spinBox_CustRes_height->value(), 10);
            res_map["width"] = QString::number(ui->spinBox_CustRes_width->value(), 10);
            Custom_resolution_list.append(res_map);
            Table_image_CustRes_rowNumInt_HeightQString_WidthQString(i, res_map["height"], res_map["width"]);
        }
        //=====
        for (int i = 0; i < row_count_gif; i++)
        {
            QMap<QString, QString> res_map;
            // 读取文件信息
            QString SourceFile_fullPath = Table_model_gif->item(i, 2)->text();
            CustRes_remove(SourceFile_fullPath); // 移除原来的设定,防止重复
            res_map["fullpath"] = SourceFile_fullPath;
            res_map["height"] = QString::number(ui->spinBox_CustRes_height->value(), 10);
            res_map["width"] = QString::number(ui->spinBox_CustRes_width->value(), 10);
            Custom_resolution_list.append(res_map);
            Table_gif_CustRes_rowNumInt_HeightQString_WidthQString(i, res_map["height"], res_map["width"]);
        }
        //========
        if (row_count_video == 0) // 如果列表里没视频,直接return
        {
            return 0;
        }
        // 读取自定义分辨率设定
        int CustRes_width_video = ui->spinBox_CustRes_width->value();
        int CustRes_height_video = ui->spinBox_CustRes_height->value();
        // 判断自定义分辨率是否包含奇数
        if (ui->spinBox_CustRes_height->value() % 2 != 0 || ui->spinBox_CustRes_width->value() % 2 != 0)
        {
            QMessageBox::warning(this,"Warning","Cannot set odd height or width.\n\nAuto Correct Start.");
            // 自动校正分辨率
            if (CustRes_height_video % 2 != 0)
                CustRes_height_video++;
            if (CustRes_width_video % 2 != 0)
                CustRes_width_video++;
        }
        // 开始apply自定义分辨率设定
        for (int i = 0; i < row_count_video; i++)
        {
            QMap<QString, QString> res_map;
            // 读取文件信息
            QString SourceFile_fullPath = Table_model_video->item(i, 2)->text();
            CustRes_remove(SourceFile_fullPath); // 移除原来的设定,防止重复
            res_map["fullpath"] = SourceFile_fullPath;
            res_map["height"] = QString::number(CustRes_height_video, 10);
            res_map["width"] = QString::number(CustRes_width_video, 10);
            Custom_resolution_list.append(res_map);
            Table_video_CustRes_rowNumInt_HeightQString_WidthQString(i, res_map["height"], res_map["width"]);
        }
        return 0;
    }
    // 如果没有选中任何row,则直接return
    if (curRow_image == -1 && curRow_video == -1 && curRow_gif == -1)
    {
        QMessageBox::warning(this,"Error","No items are currently selected.");
        return 0;
    }
    if (curRow_image >= 0) // 如果已选中图片
    {
        QMap<QString, QString> res_map;
        // 读取文件信息
        QString SourceFile_fullPath = Table_model_image->item(curRow_image, 2)->text();
        CustRes_remove(SourceFile_fullPath); // 移除原来的设定,防止重复
        res_map["fullpath"] = SourceFile_fullPath;
        res_map["height"] = QString::number(ui->spinBox_CustRes_height->value(), 10);
        res_map["width"] = QString::number(ui->spinBox_CustRes_width->value(), 10);
        Custom_resolution_list.append(res_map);
        Table_image_CustRes_rowNumInt_HeightQString_WidthQString(curRow_image, res_map["height"], res_map["width"]);
        return 0;
    }
    if (curRow_gif >= 0)
    {
        QMap<QString, QString> res_map;
        // 读取文件信息
        QString SourceFile_fullPath = Table_model_gif->item(curRow_gif, 2)->text();
        CustRes_remove(SourceFile_fullPath); // 移除原来的设定,防止重复
        res_map["fullpath"] = SourceFile_fullPath;
        res_map["height"] = QString::number(ui->spinBox_CustRes_height->value(), 10);
        res_map["width"] = QString::number(ui->spinBox_CustRes_width->value(), 10);
        Custom_resolution_list.append(res_map);
        Table_gif_CustRes_rowNumInt_HeightQString_WidthQString(curRow_gif, res_map["height"], res_map["width"]);
        return 0;
    }
    if (curRow_video >= 0)
    {
        // 读取自定义分辨率设定
        int CustRes_width_video = ui->spinBox_CustRes_width->value();
        int CustRes_height_video = ui->spinBox_CustRes_height->value();
        // 判断自定义分辨率是否包含奇数
        if (ui->spinBox_CustRes_height->value() % 2 != 0 || ui->spinBox_CustRes_width->value() % 2 != 0)
        {
            QMessageBox::warning(this,"Warning","Cannot set odd height or width.\n\nAuto Correct Start.");
            // 自动校正分辨率
            if (CustRes_height_video % 2 != 0)
                CustRes_height_video++;
            if (CustRes_width_video % 2 != 0)
                CustRes_width_video++;
        }
        //====
        QMap<QString, QString> res_map;
        // 读取文件信息
        QString SourceFile_fullPath = Table_model_video->item(curRow_video, 2)->text();
        CustRes_remove(SourceFile_fullPath); // 移除原来的设定,防止重复
        res_map["fullpath"] = SourceFile_fullPath;
        res_map["height"] = QString::number(CustRes_height_video, 10);
        res_map["width"] = QString::number(CustRes_width_video, 10);
        Custom_resolution_list.append(res_map);
        Table_video_CustRes_rowNumInt_HeightQString_WidthQString(curRow_video, res_map["height"], res_map["width"]);
        return 0;
    }
    return 0;
}
/*
取消 自定义分辨率设定
*/
int MainWindow::CustRes_CancelCustRes()
{
    if (ui->checkBox_custres_isAll->isChecked() && EnableApply2All_CustRes)
    {
        int row_count_image = Table_image_get_rowNum();
        int row_count_gif = Table_gif_get_rowNum();
        int row_count_video = Table_video_get_rowNum();
        if ((row_count_image + row_count_gif + row_count_video) == 0)
        {
            QMessageBox CustRes_NoItem;
            CustRes_NoItem.setWindowTitle("Error");
            CustRes_NoItem.setText("No items are in the list.");
            CustRes_NoItem.setIcon(QMessageBox::Warning);
            CustRes_NoItem.setModal(true);
            CustRes_NoItem.show();
            return 0;
        }
        //====
        for (int i = 0; i < row_count_image; i++)
        {
            QString SourceFile_fullPath = Table_model_image->item(i, 2)->text();
            CustRes_remove(SourceFile_fullPath);     // 从自定义分辨率列表移除
            Table_image_CustRes_Cancel_rowNumInt(i); // 清空指定row的自定义分辨率
        }
        for (int i = 0; i < row_count_gif; i++)
        {
            QString SourceFile_fullPath = Table_model_gif->item(i, 2)->text();
            CustRes_remove(SourceFile_fullPath);   // 从自定义分辨率列表移除
            Table_gif_CustRes_Cancel_rowNumInt(i); // 清空指定row的自定义分辨率
        }
        for (int i = 0; i < row_count_video; i++)
        {
            QString SourceFile_fullPath = Table_model_video->item(i, 2)->text();
            CustRes_remove(SourceFile_fullPath);     // 从自定义分辨率列表移除
            Table_video_CustRes_Cancel_rowNumInt(i); // 清空指定row的自定义分辨率
        }
        return 0;
    }
    // 如果没有任何选中的,则弹窗后return
    if (curRow_image == -1 && curRow_video == -1 && curRow_gif == -1)
    {
        QMessageBox::warning(this,"Error","No items are currently selected.");
        return 0;
    }
    if (curRow_image >= 0) // 如果已选中图片
    {
        QString SourceFile_fullPath = Table_model_image->item(curRow_image, 2)->text();
        CustRes_remove(SourceFile_fullPath);                // 从自定义分辨率列表移除
        Table_image_CustRes_Cancel_rowNumInt(curRow_image); // 清空指定row的自定义分辨率
        return 0;
    }
    if (curRow_gif >= 0)
    {
        QString SourceFile_fullPath = Table_model_gif->item(curRow_gif, 2)->text();
        CustRes_remove(SourceFile_fullPath);            // 从自定义分辨率列表移除
        Table_gif_CustRes_Cancel_rowNumInt(curRow_gif); // 清空指定row的自定义分辨率
        return 0;
    }
    if (curRow_video >= 0)
    {
        QString SourceFile_fullPath = Table_model_video->item(curRow_video, 2)->text();
        CustRes_remove(SourceFile_fullPath);                // 从自定义分辨率列表移除
        Table_video_CustRes_Cancel_rowNumInt(curRow_video); // 清空指定row的自定义分辨率
        return 0;
    }
    return 0;
}

void MainWindow::CustRes_remove(QString fullpath)
{
    for (int i = 0; i < Custom_resolution_list.size(); i++)
    {
        QMap<QString, QString> map_res = Custom_resolution_list.at(i);
        if (map_res["fullpath"] == fullpath)
        {
            Custom_resolution_list.removeAll(map_res);
        }
    }
}

bool MainWindow::CustRes_isContained(QString fullpath)
{
    for (int i = 0; i < Custom_resolution_list.size(); i++)
    {
        QMap<QString, QString> map_res = Custom_resolution_list.at(i);
        if (map_res["fullpath"] == fullpath)
        {
            return true;
        }
    }
    return false;
}

QMap<QString, QString> MainWindow::CustRes_getResMap(QString fullpath)
{
    for (int i = 0; i < Custom_resolution_list.size(); i++)
    {
        QMap<QString, QString> map_res = Custom_resolution_list.at(i);
        if (map_res["fullpath"] == fullpath)
        {
            return map_res;
        }
    }
    QMap<QString, QString> empty;
    empty.clear();
    return empty;
}

int MainWindow::CustRes_CalNewScaleRatio(QString fullpath, int Height_new, int width_new)
{
    QImage qimage_original;
    qimage_original.load(fullpath);
    int original_height = qimage_original.height();
    int original_width = qimage_original.width();
    if (original_height <= 0 || original_width <= 0)
    {
        emit Send_TextBrowser_NewMessage("Error occured when processing [" + fullpath + "]  [ Unable to get source image resolution. ]");
        return 0;
    }
    //=====================分别计算高和宽的放大倍数=======================
    //==== 高 ======
    int ScaleRatio_height;
    double ScaleRatio_height_double = (double)Height_new / (double)original_height;
    if ((ScaleRatio_height_double - (int)ScaleRatio_height_double) > 0)
    {
        ScaleRatio_height = (int)(ScaleRatio_height_double) + 1;
    }
    else
    {
        ScaleRatio_height = (int)(ScaleRatio_height_double);
    }
    //==== 宽 ======
    int ScaleRatio_width;
    double ScaleRatio_width_double = (double)width_new / (double)original_width;
    if ((ScaleRatio_width_double - (int)ScaleRatio_width_double) > 0)
    {
        ScaleRatio_width = (int)(ScaleRatio_width_double) + 1;
    }
    else
    {
        ScaleRatio_width = (int)(ScaleRatio_width_double);
    }
    //========================比较决定取哪个放大倍数值返回=====================
    if ((ScaleRatio_height <= 1) && (ScaleRatio_width <= 1))
    {
        return 1;
    }
    if (ScaleRatio_height >= ScaleRatio_width)
    {
        return ScaleRatio_height;
    }
    else
    {
        return ScaleRatio_width;
    }
}
