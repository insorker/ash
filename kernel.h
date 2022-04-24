#ifndef KERNEL_H
#define KERNEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QProcess>

class Kernel : public QWidget
{
    Q_OBJECT

public:
    Kernel(QWidget *parent = nullptr);

protected:
    // set the font of the cmd
    void setFont(QFont font);

    // set PS1
    void setPS1(QString PS1);

    // read text from cmdin when entering '\n'
    void readCmdin();

    // append text to the end of cmdout
    void writeCmdout(QString text);

    // clear the text of cmdout
    void clearCmdout();

    // parse the text from cmdin
    QStringList parse(QString text);

    // run process
    void runcmd(QString file, QStringList args);

    // run build-in cmd
    virtual bool runcmdBuildin(QString file, QStringList args);

private:
    void processInit();
    void processStdout();
    void processStderr();
    void processError(QProcess::ProcessError error);
    void processFinished();

private:
    QString _PS1;
    QString _lastCmd;

    QVBoxLayout     *_layout;
    QLineEdit       *_cmdin;
    QPlainTextEdit  *_cmdout;

    QProcess        *_fgProcess;

signals:
    void sigProcessFinished();
};

#endif // KERNEL_H
