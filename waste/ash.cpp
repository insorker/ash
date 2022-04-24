#include "ash.h"
#include <QTextCodec>
#include <QDebug>

Ash::Ash(QWidget *parent)
    : QWidget(parent),
      PS1("ash -> ")
{
    resize(640, 480);
    setWindowTitle("ash");

    _font.setPointSize(14);
    _ashout = new QPlainTextEdit(this);
    _ashout->setFont(_font);
    _ashin  = new QLineEdit(this);
    _ashin->setFont(_font);
    _ashin->setPlaceholderText(PS1);

    _layout = new QVBoxLayout(this);
    _layout->addWidget(_ashout);
    _layout->addWidget(_ashin);
    _layout->setStretch(0, 0);
    _layout->setStretch(1, 1);
    _layout->setSpacing(0);
    _layout->setContentsMargins(0, 0, 0, 0);

    setLayout(_layout);

    _process = new QProcess(this);

    initBuildincmd();
    ashoutReset();

    connect(_ashin, &QLineEdit::returnPressed, this, &Ash::ashout);
    connect(_process, &QProcess::readyReadStandardOutput, this, &Ash::ashstdout);
    connect(_process, &QProcess::readyReadStandardError, this, &Ash::ashstderror);
    connect(_process, &QProcess::errorOccurred, this, &Ash::ashProcessError);
    connect(_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &Ash::ashoutReset);
}

void Ash::ashout()
{
    QString text = _ashin->text();
    _ashin->clear();

    if (_process->state() == QProcess::NotRunning) {
        ashoutInsert(text + "\n");
        runcmd(text);
    }
    else {
        ashoutInsert(text + "\n");
        _process->write((text + "\r\n").toUtf8());
    }

    if (_process->state() == QProcess::NotRunning) {
        ashoutInsert(PS1);
    }

    _ashout->moveCursor(QTextCursor::End);
    _ashin->setFocus();
}

void Ash::ashstdout()
{
    ashoutInsert(QTextCodec::codecForName("GBK")->toUnicode(_process->readAllStandardOutput()));
}

void Ash::ashstderror()
{
    ashoutInsert(QTextCodec::codecForName("GBK")->toUnicode(_process->readAllStandardOutput()));
}

void Ash::ashoutReset()
{
    ashoutAppend(PS1);
}

void Ash::ashoutClear()
{
    _ashout->clear();
}

// text
void Ash::ashoutInsert(QString text)
{
    _ashout->moveCursor(QTextCursor::End);
    _ashout->insertPlainText(text);
}

// '\n' + text
void Ash::ashoutAppend(QString text)
{
    _ashout->moveCursor(QTextCursor::End);
    _ashout->appendPlainText(text);
}

void Ash::ashProcessError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart) {
        ashoutInsert("ash: command not found: " + _processName + "\n");
    }
}

void Ash::runcmd(QString args)
{
    QStringList argsList = args.split(" ");
    QString arg0 = argsList[0];
    _processName = arg0;
    argsList.removeFirst();

    if (runBuildincmd(arg0, argsList)) {

    }
    else if (_liba.get(arg0) != "") {
        _process->start(_liba.get(arg0), argsList);
    }
    else _process->start(arg0, argsList);
}

void Ash::initBuildincmd()
{
    _buildincmd.push_back({ "ls",       LS      });
    _buildincmd.push_back({ "clear",    CLEAR   });
    _buildincmd.push_back({ "echo",     ECHO    });
}

bool Ash::runBuildincmd(QString arg0, QStringList args)
{
    BUILDIN_TYPE type = NONE;

    for (int i = 0; i < _buildincmd.size(); i ++ ) {
        auto &cmd = _buildincmd[i];
        if (cmd.first == arg0) {
            type = cmd.second;
        }
    }

    switch (type) {
    case LS:    ls();           return true;
    case CLEAR: clear();        return true;
    case ECHO:  echo(args);     return true;
    case NONE:                  return false;
    }

    return false;
}

void Ash::ls()
{
    // build-in cmd
    ashoutInsert("===> build-in cmd\n");
    for (auto &i : _buildincmd) {
        ashoutInsert(i.first + " ");
    }
    ashoutInsert("\n");

    // liba cmd
    ashoutInsert("===> liba cmd\n");
    for (auto &i : _liba.getAll()) {
        ashoutInsert(i + " ");
    }
    ashoutInsert("\n");
}

void Ash::clear()
{
    ashoutClear();
}

void Ash::echo(QStringList args)
{
    for (auto &s : args) {
        ashoutInsert(s + " ");
    }
    ashoutAppend("");
}
