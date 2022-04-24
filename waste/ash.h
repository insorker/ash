#ifndef ASH_H
#define ASH_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QProcess>
#include "liba.h"

class Ash : public QWidget
{
    Q_OBJECT

public:
    Ash(QWidget *parent = nullptr);

public:
    // build-in cmd
    void initBuildincmd();
    bool runBuildincmd(QString arg0, QStringList args);
    void ls();
    void clear();
    void echo(QStringList args);

    // liba
    Liba _liba;

protected:
    // syscall
    void ashout();
    void ashstdout();
    void ashstderror();
    void ashoutReset();
    void ashoutClear();
    void ashProcessError(QProcess::ProcessError error);
    void ashoutInsert(QString text);
    void ashoutAppend(QString text);

    void runcmd(QString args);

private:
    QString PS1;
    QFont _font;

    QVBoxLayout *_layout;
    QPlainTextEdit *_ashout;
    QLineEdit *_ashin;

    QString _processName;
    QProcess *_process;

    QVector<QPair<QString, BUILDIN_TYPE>> _buildincmd;
};

#endif // ASH_H
