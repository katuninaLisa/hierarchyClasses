#include <QCoreApplication>
#include "main.h"

int main(int argc, char *argv[])
{
    system("chcp 1251>nul");
    QTest::qExec(new tests_parseXMLcontent);
    QTest::qExec(new testsBuildHierarchy);
    QTest::qExec(new tests_classHierarchyDOT);

    QCoreApplication a(argc, argv);

    // Считать из параметров командной строки путь к входному файлу
    QString inputFilePath = argv[1];
    // Считать из параметров командной строки путь для выходных файлов
    QString outputFilePath = argv[2];

    //Список разрешенных тегов
    QStringList allowedTags = {"classes", "class", "property", "value"};
    //Список классов
    QSet <Class> classes;
    //Список ошибок
    QSet <Errors> list_of_errors;

    // Считать данные из входного файла (inputXMLfile)
    if (!inputFilePath.isEmpty())
        int input = inputXMLfile(inputFilePath, allowedTags, list_of_errors, classes);
    else
    {
        Errors error("","","","","","",invalid_input_file_path);
        list_of_errors.insert(error);
    }

    int result = 0; // результат выполнения программы

    // Если удалось считать данные без ошибок (list_of_errors - пустой)
    if (list_of_errors.isEmpty())
    {
        QMap <QString, QMap<QString, int>> matrix; // матрица, в которой будут храниться все связи
        // Создать иерархию классов
        buildHierarchy(classes,matrix);
        // Загрузить структуру иерархии в файл
        result = OutputFile(outputFilePath, list_of_errors, matrix);
    }

    if (!list_of_errors.isEmpty() || result == -1)
    {
        //Вернуть все обнаруженные ошибки
        for (const auto& error : list_of_errors)
        {
            qInfo() << error.error();
        }
        return -1;
    }

    // Завершить работу программы
    return 0;
}

void createMatrix(QMap <QString, QMap<QString,int>> &matrix, const QSet <Class> &classes)
{
    //Создаем пустой список названий классов
    QStringList classNames;
    //Для каждого класса
    for (const Class &rec :classes)
    {
        //Заносим название класса в список
        classNames.append(rec.getName());
    }
    //Для каждого класса
    for (const Class &rec : classes)
    {
        //Создаем пустую строку - контейнер
        QMap<QString,int> row;
        //Заполняем строку нулями
        for (const QString& col : classNames)
        {
            row[col] = 0;
        }
        //Добавляем строку в матрицу
        matrix[rec.getName()] = row;
    }
}

void eliminate_class_duplication(QMap <QString, QMap<QString,int>> &matrix)
{
    // Для всех классов
    for (auto it1 = matrix.begin(); it1 != matrix.end(); ++it1)
    {
        QString subClass = it1.key();
        QMap <QString,int> row = it1.value();
        // Для каждого класса текущего класса
        for (auto it2 = row.begin(); it2 != row.end(); ++it2)
        {
            // Если у класса есть родитель
            if (it2.value() == 1)
            {
                QString superClass = it2.key();
                // Для каждого класса
                for (auto it3 = matrix.begin(); it3 != matrix.end(); ++it3) //получаем каждый класс для проверки
                {
                    QString checkClass = it3.key();
                    // Если у родительского класса и дочернего совпадает родитель
                    if (matrix[subClass].value(checkClass,0) == 1 && matrix[superClass].value(checkClass,0) == 1)
                    {
                        // Удаляем у дочернего связь с этим классом
                        matrix[subClass][checkClass] = 0;
                    }
                }
            }

        }
    }
}

void buildHierarchy(const QSet<Class> &classes, QMap <QString, QMap<QString,int>> &matrix)
{
    // Инициализируем матрицу нулями (createMatrix)
    createMatrix(matrix, classes);
    // Для всех классов
    for (const Class &child : classes)
    {
        // Для каждого класса текущего класса
        for (const Class &parent : classes)
        {
            // Если два класса  не равны
            if (child.getName() != parent.getName())
            {
                // Если один класс является подклассом другого
                if (child.isSubClass(parent))
                {
                    // Считаем, что данный столбец класса равен 1
                    matrix[child.getName()][parent.getName()] = 1;
                }
            }
        }
    }
    // Исключаем дублирующие классы (eliminate_class_duplication)
    eliminate_class_duplication(matrix);
}

void processValueElement(QXmlStreamReader& xml,QSet<Errors>& list_of_errors, Property& properties, QString className,QString propertyName, QStringList &allowedTags)
{
    static const QRegularExpression regexValue("^[0-9]+$"); // регулярное выражение для значения свойства

    if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name().toString() == "value")
    {
        QString value = xml.readElementText();

        QStringList values = value.split(',');
        values.removeAll(QString(""));

        for (const QString& val: values)
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

void processPropertyElement(QXmlStreamReader& xml, QRegularExpression& regexName,QSet<Errors>& list_of_errors, QSet<Property>& prop, QString className, QStringList &allowedTags)
{
    if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name().toString() == "property")
    {
        QString propertyName = xml.attributes().value("name").toString();

        Property properties(propertyName);

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

            processValueElement(xml, list_of_errors, properties, className, propertyName, allowedTags);
        }
        if (properties.getPropertyName().length() != 0)
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

void processClassElement(QXmlStreamReader& xml,QStringList& allowedTags,QSet<Errors>& list_of_errors, QSet<Class>& tempClasses, int countClasses)
{
    QRegularExpression regexName("^[а-яё_]+$"); // регулярное выражение - из каких букв может состоять название
    int error_in_list = list_of_errors.count(); // кол-во ошибок до получения тега

    QString className = xml.attributes().value("name").toString(); // получаем название класса

    Class curClass(className);

    if (xml.attributes().count() == 0) // если пропущен атрибут
    {
        Errors error("class","","","","","",missed_attribute);
        list_of_errors.insert(error);
    }

    if (className.isEmpty() && xml.attributes().count() != 0) // если название атрибута - неккоректно
    {
        Errors error("class",xml.attributes().at(0).name().toString(),"","","","",invalid_name_attribute);
        list_of_errors.insert(error);
    }

    if (className.length() > 30) // если длина имени класса больше 30
    {
        Errors error("class","","",className,"","",invalid_length_name_class);
        list_of_errors.insert(error);
    }

    if (!regexName.match(className).hasMatch() && !className.isEmpty()) // если в названии класса есть недопустимые символы
    {
        Errors error("class","","",className,"","",invalid_characters_attribute);
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

    QSet <Property> prop;

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name().toString() == "class") && !xml.atEnd()) // Обрабатываем свойства класса
    {
        xml.readNext(); // переходим к след. тегу

        processPropertyElement(xml, regexName, list_of_errors, prop, className, allowedTags);
    }

    if (prop.count() > 30) //если кол-во свойств в классе вне допустимого диапазона
    {
        Errors error("","","",className,"","",invalid_count_properties_in_class);
        list_of_errors.insert(error);
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
            Errors error("class","","",otherClass,className,"",duplication_content_of_class);
            list_of_errors.insert(error);
        }
        else if(className.length() != 0) //иначе добавляем класс
        {
            curClass.setproperties(prop);
            tempClasses.insert(curClass);
        }
    }
    else
        error_in_list = list_of_errors.count();
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
                countClasses++; // увеличиваем кол-во классов

                processClassElement(xml, allowedTags, list_of_errors, tempClasses, countClasses);
            }
            else
            {
                if (tagName == "property" && previousTag != "class")
                {
                    Errors error(tagName, "","","","","",incorrect_order_tags);
                    list_of_errors.insert(error);
                    // Пропускаем невалидный тег
                    xml.skipCurrentElement();
                }
                if (tagName == "value" && previousTag != "property")
                {
                    Errors error(tagName, "","","","","",incorrect_order_tags);
                    list_of_errors.insert(error);
                    // Пропускаем невалидный тег
                    xml.skipCurrentElement();
                }
                if (tagName == "classes" && countTagClasses > 1)
                {
                    Errors error(tagName, "","","","","",incorrect_order_tags);
                    list_of_errors.insert(error);
                    // Пропускаем невалидный тег
                    xml.skipCurrentElement();
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
    else if (xml.hasError())// Если есть ошибки парсинга
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
    QFileInfo fileInf(filePath);
    if(fileInf.suffix() != "xml") //если расширение неправильное
    {
        Errors error("","","","","","",incorrect_extension_input_file_path);
        list_of_errors.insert(error);
        return -1;
    }
    //Открыть файл
    QFile file(filePath);

    // Если не удалось успешно открыть входной файл
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Errors error("","","","","","",invalid_input_file_path);
        list_of_errors.insert(error);
        return -1;
    }
    //Иначе
    else
    {
        // Считать содержимое файла
        QByteArray fileContent = file.readAll();
        // Распарсить текст из XML файла(ParseXMLcontent)
        ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);
    }
    //Вернуть успешность завершения функции
    return 0;
}

QString classHierarchyDOT(QMap <QString, QMap <QString, int>> &matrix)
{
    // Записать заголовок DOT-файла
    QString result = "digraph ClassHierarchy {";

    //Добавление узлов (классов)
    QMap<QString, QMap<QString, int>>::const_iterator it1 = matrix.constBegin();
    for (; it1 != matrix.constEnd(); ++it1)
    {
        result += QString(it1.key()) + ";";
    }

    //Добавление связей между классами
    QMap<QString, QMap<QString, int>>::const_iterator it = matrix.constBegin();
    for (; it != matrix.constEnd(); ++it)
    {
        QMap<QString, int> row = it.value();
        QMap<QString, int>::const_iterator it2 = row.constBegin();
        for (; it2 != row.constEnd(); ++it2)
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

int OutputFile(const QString &filePath, QSet <Errors> &list_of_errors, QMap<QString, QMap<QString, int>> &matrix)
{
    QFileInfo fileInf(filePath);
    //Если расширение выходного файла неверное (не .dot), то…
    if(fileInf.suffix() != "dot")
    {
        Errors error("","","","","","",incorrect_extension_output_file_path);
        list_of_errors.insert(error);
        return -1;
    }
    // Создать указанный выходной файл
    QFile file(filePath);

    //Если не удалось успешно открыть выходной файл
    if (!file.open(QIODevice::WriteOnly))
    {
        //Создать новый объект ошибки
        Errors error("","","","","","",invalid_output_file_path);
        list_of_errors.insert(error);
        return -1;
    }
    // Добавить в выходной файл информацию о связях между классами (classHierarchyDOT)
    QString result = classHierarchyDOT(matrix);

    QTextStream out(&file);
    out.setEncoding(QStringConverter::System);
    out << result;

    //Вернуть успешность завершения функции
    return 0;
}
