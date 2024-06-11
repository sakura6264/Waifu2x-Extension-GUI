#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>
#include <QVariant>

void taskKill(QString name);
QString cmdGetText(QString cmd, QStringList args);
QStringList taskGetList();
bool taskExist(QString name);
bool fallbackOpenFile(QString path);
QString intoReadable(QList<QMap<QString, QString>> value);
QString intoReadable(QStringList value);
bool checkReadableHead(QString value);
bool checkReadableHead(QVariant value);
QVariant decodeReadable(QString value);
QVariant decodeReadable(QVariant value);
QStringList commandResolve(QString cmd);

#define INF_WAIT -114514

#endif // UTILS_H
