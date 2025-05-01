#include <QCoreApplication>
#include "class.h"
#include "property.h"

void createMatrix(QMap <QString, QMap<QString,int>> &matrix, const QSet <Class> &classes)
{
    QStringList classNames;
    for (const Class &rec :classes)
    {
        classNames.append(rec.getName());
    }

    for (const Class &rec : classes)
    {
        QMap<QString,int> row;
        for (const QString col : classNames)
        {
            row[col] = 0;
        }
        matrix[rec.getName()] = row;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return 0;
}
