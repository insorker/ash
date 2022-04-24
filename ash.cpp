#include "ash.h"
#include <QSettings>
#include <QTextCodec>

Ash::Ash(QWidget *parent)
    : Kernel(parent)
{
    resize(640, 480);
    setWindowTitle("ash");
    setFont(QFont("Helvetica [Cronyx]", 14));
    setPS1("ash -> ");

    readFunc();
    readEnvp();
}

bool Ash::runcmdBuildin(QString file, QStringList args)
{
    if (runcmdFunc(file, args)) { emit sigProcessFinished(); return true; }
    if (runcmdEnvp(file, args)) { emit sigProcessFinished(); return true; }
    return false;
}

bool Ash::runcmdFunc(QString file, QStringList args)
{
    if (file == _cmdFunc[0]) { ls();         return true; }
    if (file == _cmdFunc[1]) { clear();      return true; }
    if (file == _cmdFunc[2]) { echo(args);   return true; }
    return false;
}

bool Ash::runcmdEnvp(QString file, QStringList args)
{
    if (_cmdEnvp.find(file) != _cmdEnvp.end()) {
        runcmd(_cmdEnvp[file], args);
    }
    return false;
}

void Ash::readFunc()
{
    _cmdFunc.push_back("ls");
    _cmdFunc.push_back("clear");
    _cmdFunc.push_back("echo");
}

void Ash::readEnvp()
{
    QSettings setting("envp.ini", QSettings::IniFormat);
    setting.setIniCodec(QTextCodec::codecForName("UTF-8"));

    QStringList allKeys = setting.allKeys();
    for (int i = 0; i < allKeys.size(); i ++ ) {
        QString key = allKeys.value(i);
        _cmdEnvp[key] = setting.value(key).toString();
    }
}

void Ash::ls()
{
    writeCmdout("===> func cmd\n");
    for (auto &name : _cmdFunc) {
        writeCmdout(name + "\n");
    }

    writeCmdout("===> envp cmd\n");
    QStringList list;
    QHash<QString, QString>::const_iterator it = _cmdEnvp.constBegin();
    while (it != _cmdEnvp.constEnd()) {
        list.append(it.key());
        ++ it;
    }
    list.sort();
    for (auto &name : list) {
        writeCmdout(name + " ");
    }
    writeCmdout("\n");
}

void Ash::clear()
{
    clearCmdout();
}

void Ash::echo(QStringList args)
{
    for (auto &s : args) {
        writeCmdout(s + " ");
    }
    writeCmdout("\n");
}
