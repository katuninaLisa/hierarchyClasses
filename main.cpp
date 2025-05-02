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

void eliminate_class_duplication(QMap <QString, QMap<QString,int>> &matrix)
{
    for (auto it1 = matrix.begin(); it1 != matrix.end(); ++it1)
    {
        QString subClass = it1.key();
        QMap <QString,int> row = it1.value();
        for (auto it2 = row.begin(); it2 != row.end(); ++it2)
        {
            if (it2.value() == 1)
            {
                QString superClass = it2.key();
                for (auto it3 = matrix.begin(); it3 != matrix.end(); ++it3) //получаем каждый класс для проверки
                {
                    QString checkClass = it3.key();
                    if (matrix[subClass].value(checkClass,0) == 1 && matrix[superClass].value(checkClass,0) == 1)
                    {
                        matrix[subClass][checkClass] = 0;
                    }
                }
            }

        }
    }
}

void buildHierarchy(const QSet<Class> &classes, QMap <QString, QMap<QString,int>> &matrix)
{
    createMatrix(matrix, classes);
    for (const Class &child : classes)
    {
        for (const Class &parent : classes)
        {
            if (child.getName() != parent.getName())
            {
                if (child.isSubClass(parent))
                {
                    matrix[child.getName()][parent.getName()] = 1;
                }
            }
        }
    }
    eliminate_class_duplication(matrix);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return 0;
}
