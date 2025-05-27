#include "tests_parsexmlcontent.h"
#include <QtTest>

tests_parseXMLcontent::tests_parseXMLcontent(QObject *parent) : QObject(parent) {}

void tests_parseXMLcontent::tests_parse_data()
{
    //Создаем колонки-параметры метода
    QTest::addColumn<QByteArray>("xmlcontent");
    QTest::addColumn<QSet<Class>>("expectedClass");
    QTest::addColumn<Errors>("expectedError");
    QTest::addColumn<bool>("expectError");

    //Создаем строки-тесты
    QTest::newRow("emptyFile") << QByteArray("") << QSet<Class> () << Errors("","","","","","",empty_file) << true; // Пустой файл

    //Корректный XML файл
    QSet <Class> classes;
    Property prop("скорость", 100);
    Class car("машина", prop);
    classes.insert(car);
    QTest::newRow("correctXML") << QByteArray("<classes><class name=\"машина\"><property name=\"скорость\"><value>100</value></property></class></classes>")
                                << classes
                                << Errors("","","","","","",static_cast<error_type>(0))
                                << false;

    //Отсутствует корневой тег <classes>
    QTest::newRow("missingRootClassesTag") << QByteArray("<class name=\"животное\"><property name=\"конечности\"><value>4</value></property></class>")
                                << QSet<Class> ()
                                << Errors("classes","","","","","",missed_tag)
                                << true;

    //Некорректная вложенность тегов (в property вложен class)
    QTest::newRow("propertyContainsClassTag") << QByteArray("<classes><property name=\"возраст\"> <value>10</value><class name=\"человек\"></class></property></classes>")
                                           << QSet<Class> ()
                                           << Errors("property","","","","","",incorrect_order_tags)
                                           << true;

    //Некорректная вложенность тегов (в class вложен value без тега property)
    QTest::newRow("classContainsValueTagWithoutProperty") << QByteArray("<classes ><class name=\"человек\"><value>20</value><property name=\"возраст\"></property></class></classes>")
                                              << QSet<Class> ()
                                              << Errors("value","","","","","",incorrect_order_tags)
                                              << true;

    //Недопустимое имя класса
    QTest::newRow("invalidClassName") << QByteArray("<classes><class name=\"мой@класс\"><property name=\"айди\"><value>10</value></property></class></classes>")
                      << QSet<Class> ()
                      << Errors("class","","","мой@класс","","",invalid_characters_attribute)
                      << true;

    //Превышена максимальная длина имени класса
    QTest::newRow("tooLongClassName") << QByteArray("<classes><class name=\"ооооооочень_дллинноеееееее_название_классааааааааааааа\"><property name=\"температура\"><value>25</value></property></class></classes>")
                      << QSet<Class> ()
                      << Errors("class","","","ооооооочень_дллинноеееееее_название_классааааааааааааа","","",invalid_length_name_class)
                      << true;

    //Дублирование имени класса
    QSet <Class> exp_classes;
    Property book_prop("страница", 205);
    Class book("книга", book_prop);
    exp_classes.insert(book);
    QTest::newRow("duplicationNameClasses") << QByteArray("<classes><class name=\"книга\"><property name=\"страница\"><value>205</value></property></class><class name=\"книга\"><property name=\"автор\"><value>205</value></property></class></classes>")
                      << exp_classes
                      << Errors("","","","книга","","",duplication_class)
                      << true;


    //Недопустимый тип значения свойства
    QTest::newRow("invalidPropertyValueType") << QByteArray("<classes><class name=\"студент\"><property name=\"оценка\"><value>2_5</value></property></class></classes>")
                      << QSet<Class> ()
                      << Errors("value","","2_5","студент","","оценка",invalid_type_value)
                      << true;


    //Превышено количество классов
    QTest::newRow("tooManyClasses") << QByteArray("<classes>""<class name=\"автомобиль\"></class>""<class name=\"бабочка\"></class>"
                                                  "<class name=\"велосипед\"></class>""<class name=\"гараж\"></class>""<class name=\"дом\"></class>"
                                                  "<class name=\"ёлка\"></class>""<class name=\"железо\"></class>""<class name=\"здание\"></class>"
                                                  "<class name=\"игрушка\"></class>""<class name=\"книга\"></class>""<class name=\"лампа\"></class>"
                                                  "<class name=\"машина\"></class>""<class name=\"ноутбук\"></class>"
                                                  "<class name=\"окно\"></class>""<class name=\"птица\"></class>"
                                                  "<class name=\"ручка\"></class>""<class name=\"солнце\"></class>"
                                                  "<class name=\"телефон\"></class>""<class name=\"улица\"></class>"
                                                  "<class name=\"фонарь\"></class>""<class name=\"холодильник\"></class>"
                                                  "<class name=\"цветок\"></class>""<class name=\"часы\"></class>"
                                                  "<class name=\"школа\"></class>""<class name=\"щит\"></class>"
                                                  "<class name=\"экран\"></class>""<class name=\"юла\"></class>"
                                                  "<class name=\"яблоко\"></class>""<class name=\"арбуз\"></class>"
                                                  "<class name=\"балкон\"></class>""<class name=\"ветер\"></class>"
                                                  "<class name=\"гора\"></class>""<class name=\"диван\"></class>"
                                                  "<class name=\"ежевика\"></class>""<class name=\"жираф\"></class>"
                                                  "<class name=\"зонт\"></class>""<class name=\"история\"></class>"
                                                  "<class name=\"камень\"></class>""<class name=\"лимон\"></class>"
                                                  "<class name=\"мебель\"></class>""<class name=\"небо\"></class>"
                                                  "<class name=\"облако\"></class>""<class name=\"поле\"></class>"
                                                  "<class name=\"река\"></class>""<class name=\"самолет\"></class>"
                                                  "<class name=\"трава\"></class>""<class name=\"узор\"></class>"
                                                  "<class name=\"флаг\"></class>""<class name=\"хлеб\"></class>"
                                                  "<class name=\"цифра\"></class>""<class name=\"человек\"></class>"
                                                  "</classes>")
                      << QSet<Class> ()
                      << Errors("","","51","","","",invalid_count_of_classes)
                      << true;


    //Синтаксическая ошибка в XML
    QTest::newRow("syntaxErrorXML") << QByteArray("<classes><class name=\"робот\"><property name=\"батарея\"><value>25</value></class></classes>")
                      << QSet<Class> ()
                      << Errors("","","","","","",xml_syntax_error)
                      << true;


    //Дублирование имени свойств в классе
    QTest::newRow("duplicationPropertyInClass") << QByteArray("<classes><class name=\"машина\"><property name=\"скорость\"><value>100</value></property><property name=\"скорость\"><value>20</value></property></class></classes>")
                      << QSet<Class> ()
                      << Errors("property","","","машина","","скорость",duplication_property)
                      << true;


    //Дублирование содержимого классов
    QSet <Class> exp_classes2;
    Property book_prop2("страница", 205);
    Class book2("книга", book_prop2);
    exp_classes2.insert(book2);
    QTest::newRow("duplicationContentOfClass") << QByteArray("<classes><class name=\"книга\"><property name=\"страница\"><value>205</value></property></class><class name=\"евгений_онегин\"><property name=\"страница\"><value>205</value></property></class></classes>")
                      << exp_classes2
                      << Errors("class","","","книга","евгений_онегин","",duplication_content_of_class)
                      << true;

    //Имя свойства класса содержит недопустимые символы
    QTest::newRow("invalidPropertyName") << QByteArray("<classes><class name=\"машина\"><property name=\"speed\"></property></class></classes>")
                      << QSet<Class> ()
                      << Errors("property","","","машина","","speed",invalid_characters_attribute)
                      << true;

    //Недопустимый атрибут для имени класса
    QTest::newRow("invalidClassNameAttribute") << QByteArray("<classes><class n=\"машина\"><property name=\"скорость\"></property></class></classes>")
                      << QSet<Class> ()
                      << Errors("class","n","","","","",invalid_name_attribute)
                      << true;

    //Недопустимый атрибут для имени свойства
    QTest::newRow("invalidPropertyNameAttribute") << QByteArray("<classes><class name=\"машина\"><property n=\"скорость\"></property></class></classes>")
                      << QSet<Class> ()
                      << Errors("property","n","","","","",invalid_name_attribute)
                      << true;

    //Превышена максимальная длина имени свойства
    QTest::newRow("tooLongLengthNameProperty") << QByteArray("<classes><class name=\"машина\"><property name=\"скорость_скорость_скорость_скорость_скорость_скорость\"></property></class></classes>")
                      << QSet<Class> ()
                      << Errors("property","","","машина","","скорость_скорость_скорость_скорость_скорость_скорость",invalid_length_name_property)
                      << true;

    //Значение свойства вне допустимого диапазона
    QTest::newRow("PropertyValueOutOfRange") << QByteArray("<classes><class name=\"машина\"><property name=\"скорость\"><value>1020202</value></property></class></classes>")
                                               << QSet<Class> ()
                                               << Errors("value","","1020202","машина","","скорость",invalid_range_value)
                                               << true;

    //Недопустимое имя тега
    QTest::newRow("invalidNameTag") << QByteArray("<classes><class name=\"машина\"><prop name=\"скорость\"></prop></class></classes>")
                      << QSet<Class> ()
                      << Errors("prop","","","","","",invalid_name_tag)
                      << true;

    //Превышено количество свойств в классе
    QTest::newRow("tooManyPropertiesInClass") << QByteArray("<classes><class name=\"а\"><property name=\"б\"></property><property name=\"в\">"
                                                            "</property><property name=\"г\"></property><property name=\"д\"></property><property name=\"е\">"
                                                            "</property><property name=\"ё\"></property><property name=\"ж\"></property><property name=\"з\">"
                                                            "</property><property name=\"и\"></property><property name=\"й\"></property><property name=\"к\">"
                                                            "</property><property name=\"л\"></property><property name=\"м\"></property><property name=\"н\">"
                                                            "</property><property name=\"о\"></property><property name=\"п\"></property><property name=\"р\">"
                                                            "</property><property name=\"с\"></property><property name=\"т\"></property><property name=\"у\">"
                                                            "</property><property name=\"ф\"></property><property name=\"х\"></property><property name=\"ц\">"
                                                            "</property><property name=\"ч\"></property><property name=\"ш\"></property><property name=\"щ\">"
                                                            "</property><property name=\"ъ\"></property><property name=\"ы\"></property><property name=\"ь\">"
                                                            "</property><property name=\"э\"></property><property name=\"ю\"></property><property name=\"я\"></property></class></classes>")
                      << QSet<Class> ()
                      << Errors("","","","а","","",invalid_count_properties_in_class)
                      << true;

    //В теге пропущен атрибут
    QTest::newRow("missedAttributeInTag") << QByteArray("<classes><class name=\"машина\"><property></property></class></classes>")
                      << QSet<Class> ()
                      << Errors("property","","","","","",missed_attribute)
                      << true;
}

void tests_parseXMLcontent::tests_parse()
{
    QFETCH(QByteArray, xmlcontent);
    QFETCH(QSet<Class>, expectedClass);
    QFETCH(Errors, expectedError);
    QFETCH(bool, expectError);

    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;
    ParseXMLcontent(xmlcontent, allowedTags, list_of_errors, classes);

    QVERIFY2(classes == expectedClass, "Классы не совпадают");

    if (expectError)
    {
        QVERIFY2(list_of_errors.size() == 1, "Expected 1 error");

        Errors error = *list_of_errors.begin();

        QVERIFY2(error.type == expectedError.type, "Don't match type of error");
        QVERIFY2(error.attribute == expectedError.attribute, "Don't match type of error");
        QVERIFY2(error.className == expectedError.className, "Don't match type of error");
        QVERIFY2(error.nameTag == expectedError.nameTag, "Don't match type of error");
        QVERIFY2(error.otherClassName == expectedError.otherClassName, "Don't match type of error");
        QVERIFY2(error.value == expectedError.value, "Don't match type of error");
        QVERIFY2(error.propertyName == expectedError.propertyName, "Don't match type of error");
    }
    else
    {
        QVERIFY2(list_of_errors.isEmpty(), "List of errors doesn't empty!");
    }
}
