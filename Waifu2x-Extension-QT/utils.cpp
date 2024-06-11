#include "utils.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

void taskKill(QString name)
{
    // use win32 api as a replacement of start(taskkill)
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe = {sizeof(PROCESSENTRY32)};
    BOOL bret = Process32First(snapshot, &pe);
    while (bret)
    {
        if (QString::fromWCharArray(pe.szExeFile) == name)
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
        }
        bret = Process32Next(snapshot, &pe);
    }
    CloseHandle(snapshot);
}

QString cmdGetText(QString cmd, QStringList args)
{
    // get command text output
    QProcess process;
    process.start(cmd, args);
    process.waitForStarted(3000);
    process.waitForFinished(3000);
    if (process.exitStatus() == QProcess::NormalExit)
    {
        return process.readAllStandardOutput();
    }
    return "ERROR";
}

QStringList taskGetList()
{
    // win32 api, replacement of start(tasklist)
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe = {sizeof(PROCESSENTRY32)};
    BOOL bret = Process32First(snapshot, &pe);
    QStringList list;
    list.append(QString::fromWCharArray(pe.szExeFile));
    while (bret)
    {
        list.append(QString::fromWCharArray(pe.szExeFile));
        bret = Process32Next(snapshot, &pe);
    }
    return list;
}

bool taskExist(QString name)
{
    QStringList list = taskGetList();
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i) == name)
        {
            return true;
        }
    }
    return false;
}

bool fallbackOpenFile(QString path)
{
    // use win32 api instead of qt openuri
    HANDLE hexec = ShellExecute(0, 0, (LPCWSTR)path.utf16(), 0, 0, SW_SHOW);
    if (hexec == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    return true;
}

QString intoReadable(QList<QMap<QString, QString>> value)
{
    // convert some type of value into $TYPE${JSON} to write a readable text into settings.ini
    QJsonArray json;
    for (int listindex = 0; listindex < value.length(); listindex++)
    {
        QList<QString> keys = value[listindex].keys();
        QJsonObject subjson;
        for (int mapindex = 0; mapindex < keys.length(); mapindex++)
        {
            QString k = keys[mapindex];
            subjson.insert(k, value[listindex][k]);
        }
        json.append(subjson);
    }
    QJsonDocument jsontxt = QJsonDocument(json);
    QString formattedjson = jsontxt.toJson(QJsonDocument::Compact);
    QString output = "$MAPLIST$" + formattedjson;
    return output;
}

QString intoReadable(QStringList value)
{
    QJsonArray json;
    for (int i = 0; i < value.length(); i++)
    {
        json.append(value[i]);
    }
    QJsonDocument jsontxt = QJsonDocument(json);
    QString formattedjson = jsontxt.toJson(QJsonDocument::Compact);
    QString output = "$LIST$" + formattedjson;
    return output;
}

bool checkReadableHead(QVariant value)
{
    if (value.type() == QVariant::String)
    {
        return checkReadableHead(value.toString());
    }
    return false;
}

bool checkReadableHead(QString value)
{
    bool isreadable = false;
    QStringList head = {"$MAPLIST$", "$LIST$"};
    for (int i = 0; i < head.length(); i++)
    {
        if (value.startsWith(head[i]))
        {
            isreadable = true;
            break;
        }
    }
    return isreadable;
}

QVariant decodeReadable(QString value)
{
    if (value.startsWith("$MAPLIST$"))
    {
        value.remove("$MAPLIST$");
        QJsonDocument json = QJsonDocument::fromJson(value.toUtf8());
        QJsonArray array = json.array();
        QList<QMap<QString, QString>> output;
        for (int i = 0; i < array.size(); i++)
        {
            QMap<QString, QString> map;
            QJsonObject obj = array[i].toObject();
            QStringList keys = obj.keys();
            for (int j = 0; j < keys.length(); j++)
            {
                map.insert(keys[j], obj[keys[j]].toString());
            }
            output.append(map);
        }
        return QVariant::fromValue(output);
    }
    else if (value.startsWith("$LIST$"))
    {
        value.remove("$LIST$");
        QJsonDocument json = QJsonDocument::fromJson(value.toUtf8());
        QJsonArray array = json.array();
        QStringList output;
        for (int i = 0; i < array.size(); i++)
        {
            output.append(array[i].toString());
        }
        return QVariant::fromValue(output);
    }
    return value;
}

QVariant decodeReadable(QVariant value)
{
    if (value.type() == QVariant::String)
    {
        return decodeReadable(value.toString());
    }
    return value;
}

QStringList commandResolve(QString cmd)
{
    QStringList Extra_command;
    QStringList extra_command_list = cmd.simplified().split(QRegExp("\\s"));
    for (int index = 0; index < extra_command_list.length(); index++)
    {
        QString extra_str = extra_command_list[index].simplified();
        if (!extra_str.isEmpty())
        {
            Extra_command.append(extra_str);
        }
    }
    return Extra_command;
}
