#include "kernel.h"
#include <QTextCodec>
#include <QDebug>

Kernel::Kernel(QWidget *parent)
    : QWidget(parent)
{
    _cmdin  = new QLineEdit(this);
    _cmdout = new QPlainTextEdit(this);

    _layout = new QVBoxLayout(this);
    _layout->addWidget(_cmdout);
    _layout->addWidget(_cmdin);
    _layout->setStretch(0, 0);
    _layout->setStretch(1, 1);
    _layout->setSpacing(0);
    _layout->setContentsMargins(0, 0, 0, 0);

    setLayout(_layout);
    setPS1("$ ");
    processInit();

    connect(_cmdin, &QLineEdit::returnPressed, this, &Kernel::readCmdin);
    connect(_fgProcess, &QProcess::readyReadStandardOutput, this, &Kernel::processStdout);
    connect(_fgProcess, &QProcess::readyReadStandardError, this, &Kernel::processStderr);
    connect(_fgProcess, &QProcess::errorOccurred, this, &Kernel::processError);
    connect(_fgProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &Kernel::processFinished);
    connect(this, &Kernel::sigProcessFinished, this, &Kernel::processFinished);
}

void Kernel::setFont(QFont font)
{
    _cmdin->setFont(font);
    _cmdout->setFont(font);
}

void Kernel::setPS1(QString PS1)
{
    _PS1 = PS1;
    _cmdin->setPlaceholderText(_PS1);
    clearCmdout();
    writeCmdout(_PS1);
}

void Kernel::readCmdin()
{
    // read the input
    _lastCmd = _cmdin->text(); _cmdin->clear();
    writeCmdout(_lastCmd + '\n');

    if (_fgProcess->state() == QProcess::NotRunning) {
        // if no process is running in the fg, run process
        QStringList args = parse(_lastCmd);
        QString file = args[0];
        args.removeFirst();

        runcmd(file, args);
    }
    else {
        // write to process's stdin
        _fgProcess->write((_lastCmd + "\n").toUtf8());
    }

    _cmdin->setFocus();
}

void Kernel::writeCmdout(QString text)
{
    _cmdout->moveCursor(QTextCursor::End);
    _cmdout->insertPlainText(text);
}

void Kernel::clearCmdout()
{
    _cmdout->clear();
}

QStringList Kernel::parse(QString text)
{
    return text.split(" ");
}

void Kernel::runcmd(QString file, QStringList args)
{
    if (runcmdBuildin(file, args)) { return; }
    _fgProcess->start(file, args);
}

bool Kernel::runcmdBuildin(QString, QStringList)
{
    return false;
}

void Kernel::processInit()
{
    _fgProcess = new QProcess(this);
}

void Kernel::processStdout()
{
    writeCmdout(QTextCodec::codecForName("GBK")->toUnicode(_fgProcess->readAllStandardOutput()));
}

void Kernel::processStderr()
{
    writeCmdout(QTextCodec::codecForName("GBK")->toUnicode(_fgProcess->readAllStandardError()));
}

void Kernel::processError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart) {
        writeCmdout("ash: command not found: " + _lastCmd + '\n');
        processFinished();
    }
}

void Kernel::processFinished()
{
    writeCmdout(_PS1);
}
