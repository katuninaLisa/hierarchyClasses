#include <QCoreApplication>
#include "class.h"
#include "property.h"
#include <QtTest>
#include "testsbuildhierarchy.h"

enum error_type
{
    invalid_input_file_path,
    invalid_output_file_path,
    xml_syntax_error,
    invalid_name_tag,
    missed_tag,
    invalid_name_attribute,
    missed_attribute,
    invalid_characters_attribute,
    invalid_type_value,
    invalid_range_value,
    invalid_length_name_class,
    invalid_length_name_property,
    invalid_count_properties_in_class,
    invalid_count_of_classes,
    incorrect_order_tags,
    duplication_class,
    duplication_property,
    duplication_content_of_class,
    empty_file
};

struct Errors
{
    QString nameTag;
    QString attribute;
    QString value;
    QString className;
    QString otherClassName;
    QString propertyName;
    error_type type;
    QString error() const
    {
        QString message;
        switch (type){
            case invalid_input_file_path:
                message = "Ошибка. Входной файл не найден";
            case invalid_output_file_path:
                message = "Ошибка. Выходной файл не найден";
            case xml_syntax_error:
                message = "Ошибка: Некорректный формат XML-файла. Проверьте синтаксис и структуру.";
            case invalid_name_tag:
                message = QString("Ошибка: Неккоректное название тега <%1>").arg(nameTag);
            case missed_tag:
                message = QString("Ошибка: Отсутствует тег <%1>.").arg(nameTag);
            case invalid_name_attribute:
                message = QString("Ошибка: Необходимо использовать атрибут 'name' в теге <%1>, а не '%2'").arg(nameTag).arg(attribute);
            case missed_attribute:
                message = QString("Ошибка: В теге <%1> пропущен аттрибут").arg(nameTag);
            case invalid_characters_attribute:{
                QString text;
                if (!propertyName.isEmpty())
                    text = QString("%1 класс %2").arg(propertyName).arg(className);
                else
                    text = QString("класс %1").arg(className);
                message = QString("Ошибка: %1 содержит недопустимые символы. Используйте только русские буквы, и нижние подчеркивания.").arg(text);
            }
            case invalid_type_value:
                message = QString("Ошибка: Недопустимый тип значения класса <%1> свойства <%2>. Ожидается целое число.").arg(className).arg(propertyName);
            case invalid_range_value:
                message = QString("Ошибка: Значение свойства <%1> класса <%2> вне допустимого диапазона: от 1 до 1000. Значение свойства: %3.").arg(propertyName).arg(className).arg(value);
            case invalid_length_name_class:
                message = QString("Ошибка: Превышена максимальная длина класса <%1>. Максимальная длина: 30.").arg(className);
            case invalid_length_name_property:
                message = QString("Ошибка: Превышена максимальная длина свойства <%1>. Максимальная длина: 30.").arg(propertyName);
            case invalid_count_properties_in_class:
                message = QString("Ошибка: Количество свойств в классе <%1> должно быть от 1 до 30.").arg(className);
            case invalid_count_of_classes:
                message = QString("Ошибка: Количество классов должно быть от 1 до 50.");
            case incorrect_order_tags:{
                QString text;
                if (nameTag == "class")
                    text = "classes";
                else if (nameTag == "property")
                    text = "class";
                else if (nameTag == "value")
                    text = "property";
                message = QString("Ошибка: Неправильный порядок тегов: тег %1 должен быть вложен в %2.").arg(nameTag).arg(text);
            }
            case duplication_class:
                message = QString("Ошибка: Невозможно построить иерархию классов. Обнаружено дублирование классов %1.").arg(className);
            case duplication_property:
                message = QString("Ошибка: Невозможно построить иерархию классов. Обнаружено дублирование свойств %1 класса %2.").arg(propertyName).arg(className);
            case duplication_content_of_class:
                message = QString("Ошибка: Невозможно построить иерархию классов. Обнаружено дублирование содержимого классов %1 и %2.").arg(className).arg(otherClassName);
            case empty_file:
                message = "Ошибка: Пустой файл.";
        }
        return message;
    }
};


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

    testsBuildHierarchy tests;
    QTest::qExec(&tests, argc, argv);

    return 0;
}
