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

    Errors(const QString& n, const QString& a, const QString& v, const QString& c, const QString& o, const QString& p, error_type t):
        nameTag(n), attribute(a), value(v), className(c), otherClassName(o), propertyName(p), type(t)
    {}

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
                message = QString("Ошибка: Количество классов должно быть от 1 до 50. Фактическое кол-во классов: %1").arg(value);
            case incorrect_order_tags:{
                QString text;
                if (nameTag == "class")
                    text = "classes";
                else if (nameTag == "property")
                    text = "class";
                else if (nameTag == "value")
                    text = "property";s
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

    bool operator==(const Errors &other) const {
        return type == other.type && nameTag == other.nameTag && attribute == other.attribute &&
            value == other.value && className == other.className && propertyName == other.propertyName &&
            otherClassName == other.otherClassName;
    }
};

inline uint qHash(const Errors &error, uint seed = 0) {
    return qHash(error.type, seed) ^ qHash(error.nameTag, seed) ^ qHash(error.attribute, seed) ^
           qHash(error.value, seed) ^ qHash(error.className, seed) ^ qHash(error.propertyName, seed) ^
           qHash(error.otherClassName, seed);
}

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
    QXmlStreamReader xml(fileContent);

    int countClasses = 0;
    bool hasRoot = false;
    QRegularExpression regexName("^[а-яё_]+$"); // регулярное выражение - из каких букв может состоять название
    QRegularExpression regexValue("^[0-9]+$"); // регулярное выражение для значения свойства
    int error_in_list = 0;
    // Пока не конец файла
    while (!xml.atEnd() && !xml.hasError())
    {
        //Читаем следующий элемент
        QXmlStreamReader::TokenType token = xml.readNext();
        //Если текущий элемент - начало тега
        if (token == QXmlStreamReader::StartElement)
        {
            QString tagName = xml.name().toString(); //получаем название тега

            if (tagName == "classes")
                hasRoot = true;
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
                for (const Class &clas : classes) // если есть дублирующие названия классов
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

                        if (propertyName.isEmpty()) // если недопустипое имя атрибута
                        {
                            Errors error(tagName,xml.attributes().at(0).name().toString(),"","","","",invalid_name_attribute);
                            list_of_errors.insert(error);
                        }

                        if (propertyName.length() > 30) // если длина имени свойства больше 30
                        {
                            Errors error(tagName,"","",className,"",propertyName,invalid_length_name_property);
                            list_of_errors.insert(error);
                        }

                        if (!regexName.match(propertyName).hasMatch()) // если в имени свойства присутствуют недопустимые символы
                        {
                            Errors error(tagName,"","",className,"",propertyName,invalid_characters_attribute);
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
                            Errors error(tagName,"","",className,"",propertyName,duplication_property);
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
                                    if (val.toInt() < 1 || val.toInt() > 1000) // если значение свойства вне допустимого диапазона
                                    {
                                        Errors error("","",val,className,"",propertyName,invalid_range_value);
                                        list_of_errors.insert(error);
                                    }

                                    if (!regexValue.match(val).hasMatch()) // если значение свойства имеет недопустимый тип
                                    {
                                        Errors error("","",val,className,"",propertyName,invalid_type_value);
                                        list_of_errors.insert(error);
                                    }
                                    else
                                        properties.setvalues(val.toInt());
                                }
                            }
                        }

                        if (prop.count() > 30) //если кол-во свойств в классе вне допустимого диапазона
                        {
                            Errors error("","","",className,"","",invalid_count_properties_in_class);
                            list_of_errors.insert(error);
                        }
                        else
                            prop.insert(properties); // добавляем новое свойство
                    }
                }

                if (list_of_errors.count() == error_in_list) // если ошибок нет
                {
                    bool foundProp = false; QString otherClass;
                    for (const Class &allClass : classes)
                    {
                        if (allClass.getProperties() == curClass.getProperties() && foundProp == false)
                        {
                            foundProp = true;
                            otherClass = allClass.getName();
                        }
                    }
                    if (foundProp) // если найдены классы с полностью идентичными свойства
                    {
                        Errors error("","","",className,otherClass,"",duplication_content_of_class);
                        list_of_errors.insert(error);
                    }
                    else if (!prop.isEmpty()) //если есть свойства в классе
                    {
                        curClass.setproperties(prop);
                        classes.insert(curClass);
                    }
                }
                else
                    error_in_list = list_of_errors.count();
            }
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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //testsBuildHierarchy tests;
    //QTest::qExec(&tests, argc, argv);

    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    return 0;
}
