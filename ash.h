#ifndef ASH_H
#define ASH_H

#include "kernel.h"

class Ash : public Kernel
{
    Q_OBJECT
    enum BUILDIN_TYPE { NONE, LS, CLEAR, ECHO };

public:
    Ash(QWidget *parent = nullptr);

protected:
    bool runcmdBuildin(QString file, QStringList args) override;
    bool runcmdFunc(QString file, QStringList args);
    bool runcmdEnvp(QString file, QStringList args);

    // init
    void readFunc();
    void readEnvp();

    // cmd func
    void ls();
    void clear();
    void echo(QStringList args);

private:
    QVector<QString>        _cmdFunc;
    QHash<QString, QString> _cmdEnvp;
};

#endif // ASH_H
