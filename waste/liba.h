#ifndef LIBA_H
#define LIBA_H

#include <QString>
#include <QHash>

class Liba
{
public:
    Liba();

    QString get(QString lib);
    QStringList getAll();

private:
    QHash<QString, QString> _hash;
};

#endif // LIBA_H
