#include "liba.h"
#include <QSettings>
#include <QTextCodec>
#include <QDebug>

Liba::Liba()
{
    QSettings setting("envp.ini", QSettings::IniFormat);
    setting.setIniCodec(QTextCodec::codecForName("UTF-8"));

    QStringList allKeys = setting.allKeys();
    for (int i = 0; i < allKeys.size(); i ++ ) {
        QString key = allKeys.value(i);
        _hash[key] = setting.value(key).toString();
    }
}

QString Liba::get(QString lib)
{
    if (_hash.find(lib) != _hash.end()) {
        return _hash[lib];
    }
    return "";
}

QStringList Liba::getAll()
{
    QStringList list;
    QHash<QString, QString>::const_iterator it = _hash.constBegin();
    while (it != _hash.constEnd()) {
        list.append(it.key());
        ++ it;
    }
    return list;
}
