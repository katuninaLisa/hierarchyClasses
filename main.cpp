#include <QCoreApplication>
#include "class.h"
#include "property.h"
#include <QtTest>
#include "testsbuildhierarchy.h"
#include "tests_parsexmlcontent.h"
#include "tests_classhierarchydot.h"
#include "errors.h"

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

void ParseXMLcontent(const QByteArray &fileContent, QStringList& allowedTags, QSet<Errors >& list_of_errors, QSet <Class> &classes)
{
    bool isEmpty = false;
    if (fileContent.isEmpty() || fileContent.trimmed().isEmpty()) // если файл пустой или в файле только пробелы
    {
        Errors error("","","","","","",empty_file);
        list_of_errors.insert(error);
        isEmpty = true;
    }

    QXmlStreamReader xml(fileContent);

    int countClasses = 0; // количество классов
    bool hasRoot = false; // наличие корневого тега
    QRegularExpression regexName("^[а-яё_]+$"); // регулярное выражение - из каких букв может состоять название
    QRegularExpression regexValue("^[0-9]+$"); // регулярное выражение для значения свойства
    int error_in_list = 0; // кол-во ошибок до получения тега
    int countTagClasses = 0; //количество тегов classes
    QString previousTag; // предыдущий тег
    QSet <Class> tempClasses; // временный список классов

    // Пока не конец файла
    while (!xml.atEnd() && !xml.hasError() && !isEmpty)
    {
        //Читаем следующий элемент
        QXmlStreamReader::TokenType token = xml.readNext();
        //Если текущий элемент - начало тега
        if (token == QXmlStreamReader::StartElement)
        {
            QString tagName = xml.name().toString(); //получаем название тега

            if (tagName == "classes")
            {
                hasRoot = true;
                countTagClasses++;
            }
            if (!hasRoot) //если отсутствует корневой элемент classes
            {
                Errors error("classes","","","","","",missed_tag);
                list_of_errors.insert(error);
            }

            if (!allowedTags.contains(tagName)) // если название тега - неккоректно
            {
                Errors error(tagName,"","","","","",invalid_name_tag);
                list_of_errors.insert(error);
            }

            if (tagName == "class") //обрабатываем тег <class>
            {
                countClasses++;

                Class curClass;

                if (xml.attributes().count() == 0) // если пропущен атрибут
                {
                    Errors error(tagName,"","","","","",missed_attribute);
                    list_of_errors.insert(error);
                }

                QString className = xml.attributes().value("name").toString(); // получаем название класса
                if (className.isEmpty()) // если название атрибута - неккоректно
                {
                    Errors error(tagName,xml.attributes().at(0).name().toString(),"","","","",invalid_name_attribute);
                    list_of_errors.insert(error);
                }

                if (className.length() > 30) // если длина имени класса больше 30
                {
                    Errors error(tagName,"","",className,"","",invalid_length_name_class);
                    list_of_errors.insert(error);
                }

                if (!regexName.match(className).hasMatch() && !className.isEmpty()) // если в названии класса есть недопустимые символы
                {
                    Errors error(tagName,"","",className,"","",invalid_characters_attribute);
                    list_of_errors.insert(error);
                }

                bool foun = false;
                for (const Class &clas : tempClasses) // если есть дублирующие названия классов
                {
                    if (clas.getName() == className)
                    {
                        foun = true;
                    }
                }
                if (foun)
                {
                    Errors error("","","",className,"","",duplication_class);
                    list_of_errors.insert(error);
                }

                curClass.setclassName(className);
                QSet <Property> prop;

                while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "class") && !xml.atEnd()) // Обрабатываем свойства класса
                {
                    xml.readNext(); // переходим к след. тегу

                    if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name().toString() == "property")
                    {
                        QString propertyName = xml.attributes().value("name").toString();

                        Property properties;
                        properties.setpropertyName(propertyName);

                        if (xml.attributes().count() == 0) // если пропущен атрибут
                        {
                            Errors error(xml.name().toString(),"","","","","",missed_attribute);
                            list_of_errors.insert(error);
                        }

                        if (propertyName.isEmpty() && xml.attributes().count() != 0) // если недопустипое имя атрибута
                        {
                            Errors error(xml.name().toString(),xml.attributes().at(0).name().toString(),"","","","",invalid_name_attribute);
                            list_of_errors.insert(error);
                        }

                        if (propertyName.length() > 30) // если длина имени свойства больше 30
                        {
                            Errors error(xml.name().toString(),"","",className,"",propertyName,invalid_length_name_property);
                            list_of_errors.insert(error);
                        }

                        if (!regexName.match(propertyName).hasMatch() && !propertyName.isEmpty()) // если в имени свойства присутствуют недопустимые символы
                        {
                            Errors error(xml.name().toString(),"","",className,"",propertyName,invalid_characters_attribute);
                            list_of_errors.insert(error);
                        }

                        bool exist = false; // если есть дублирующие название свойств в классе
                        for (const Property &p : prop)
                        {
                            if (p.getPropertyName() == propertyName)
                                exist = true;
                        }
                        if (exist)
                        {
                            Errors error(xml.name().toString(),"","",className,"",propertyName,duplication_property);
                            list_of_errors.insert(error);
                        }

                        while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "property") && !xml.atEnd()) // Обрабатываем значения свойства
                        {
                            xml.readNext(); // переходим к след тегу

                            if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name().toString() == "value")
                            {
                                QString value = xml.readElementText();

                                QStringList values = value.split(',', Qt::SkipEmptyParts);

                                for (const QString val: values)
                                {
                                    if (!regexValue.match(val).hasMatch()) // если значение свойства имеет недопустимый тип
                                    {
                                        Errors error("value","",val,className,"",propertyName,invalid_type_value);
                                        list_of_errors.insert(error);
                                    }
                                    else
                                    {
                                        if (val.toInt() < 1 || val.toInt() > 1000) // если значение свойства вне допустимого диапазона
                                        {
                                            Errors error("value","",val,className,"",propertyName,invalid_range_value);
                                            list_of_errors.insert(error);
                                        }
                                        else
                                            properties.setvalues(val.toInt()); // значение - верное
                                    }
                                }
                            }
                            else if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name().toString() != "value")
                            {
                                if (!allowedTags.contains(xml.name().toString())) // если название тега - неккоректно
                                {
                                    Errors error(xml.name().toString(),"","","","","",invalid_name_tag);
                                    list_of_errors.insert(error);
                                }
                                else
                                {
                                    Errors error(xml.name().toString(), "","","","","",incorrect_order_tags);
                                    list_of_errors.insert(error);
                                }
                            }
                        }

                        if (prop.count() > 30) //если кол-во свойств в классе вне допустимого диапазона
                        {
                            Errors error("","","",className,"","",invalid_count_properties_in_class);
                            list_of_errors.insert(error);
                        }

                        prop.insert(properties); // добавляем новое свойство
                    }
                    else if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name().toString() != "property")
                    {
                        if (!allowedTags.contains(xml.name().toString())) // если название тега - неккоректно
                        {
                            Errors error(xml.name().toString(),"","","","","",invalid_name_tag);
                            list_of_errors.insert(error);
                        }
                        else
                        {
                            Errors error(xml.name().toString(), "","","","","",incorrect_order_tags);
                            list_of_errors.insert(error);
                        }
                    }
                }

                if (list_of_errors.count() == error_in_list) // если ошибок нет
                {
                    bool foundProp = false; QString otherClass;
                    for (const Class &allClass : tempClasses) // проверка на содержимое классов
                    {
                        if (allClass.getProperties() == prop && foundProp == false && !allClass.getProperties().isEmpty() && !prop.isEmpty())
                        {
                            foundProp = true;
                            otherClass = allClass.getName();
                        }
                    }
                    if (foundProp) // если найдены классы с полностью идентичными свойствами
                    {
                        Errors error(tagName,"","",otherClass,className,"",duplication_content_of_class);
                        list_of_errors.insert(error);
                    }
                    else //иначе добавляем класс
                    {
                        curClass.setproperties(prop);
                        tempClasses.insert(curClass);
                    }
                }
                else
                    error_in_list = list_of_errors.count();
            }
            else
            {
                if (tagName == "property" && previousTag != "class")
                {
                    Errors error(tagName, "","","","","",incorrect_order_tags);
                    list_of_errors.insert(error);
                }
                if (tagName == "value" && previousTag != "property")
                {
                    Errors error(tagName, "","","","","",incorrect_order_tags);
                    list_of_errors.insert(error);
                }
                if (tagName == "classes" && countTagClasses > 1)
                {
                    Errors error(tagName, "","","","","",incorrect_order_tags);
                    list_of_errors.insert(error);
                }
            }
            previousTag = tagName;
        }
    }
    if (countClasses > 50) //если количество классов больше 50
    {
        Errors error("","",QString::number(countClasses),"","","",invalid_count_of_classes);
        list_of_errors.insert(error);
    }

    if (xml.hasError())// Если есть ошибки парсинга
    {
        Errors error("","","","","","",xml_syntax_error);
        list_of_errors.insert(error);
    }
    else
    {
        classes = tempClasses;
    }
}

int inputXMLfile(const QString& filePath, QStringList& allowedTags, QSet<Errors >& list_of_errors, QSet <Class> &classes)
{
    // Открыть файл
    QFile file(filePath);

    // Если не удалось успешно открыть входной файл
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Errors error("","","","","","",invalid_input_file_path);
        list_of_errors.insert(error);
    }
    //Иначе
    else
    {
        // Считать содержимое файла
        QByteArray fileContent = file.readAll();
        // Распарсить текст из XML файла(ParseXMLcontent)
        ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);
    }
    //Закрыть файл
    file.close();
    //Вернуть успешность завершения функции
    return 0;
}

QString classHierarchyDOT(QMap <QString, QMap <QString, int>> &matrix)
{
    // Записать заголовок DOT-файла
    QString result = "digraph ClassHierarchy {";

    //Добавление узлов (классов)
    QMap<QString, QMap<QString, int>>::const_iterator it1 = matrix.constBegin();
    for (it1; it1 != matrix.constEnd(); ++it1)
    {
        result += QString(it1.key()) + ";";
    }

    //Добавление связей между классами
    QMap<QString, QMap<QString, int>>::const_iterator it = matrix.constBegin();
    for (it; it != matrix.constEnd(); ++it)
    {
        QMap<QString, int> row = it.value();
        QMap<QString, int>::const_iterator it2 = row.constBegin();
        for (it2; it2 != row.constEnd(); ++it2)
        {
            if (it2.value() == 1)
            {
                result += QString(it.key()) + "->" + QString(it2.key()) + ";";
            }
        }
    }

    //Завершение DOT-файла
    result += "}";

    //Вернуть запись связей между классами
    return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    tests_parseXMLcontent tests;
    QTest::qExec(&tests, argc, argv);

    return 0;
}
