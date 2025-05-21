#include "tests_parsexmlcontent.h"
#include <QtTest>

tests_parseXMLcontent::tests_parseXMLcontent(QObject *parent) : QObject(parent) {}

void tests_parseXMLcontent::emptyFile()
{
    const QByteArray fileContent = "";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("","","","","","",empty_file);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::correctXML()
{
    const QByteArray fileContent = "<classes><class name=\"машина\"><property name=\"скорость\"><value>100</value></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Class> exp_classes;
    Property animalprop("скорость", 100);
    Class car("машина", animalprop);
    exp_classes.insert(car);

    QVERIFY(list_of_errors.isEmpty());
    QVERIFY(exp_classes == classes);
}

void tests_parseXMLcontent::missingRootClassesTag()
{
    const QByteArray fileContent = "<class name=\"животное\"><property name=\"конечности\"><value>4</value></property></class>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("classes","","","","","",missed_tag);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::propertyContainsClassTag()
{
    const QByteArray fileContent = "<classes><property name=\"возраст\"> <value>10</value><class name=\"человек\"></class></property></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("property","","","","","",incorrect_order_tags);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::classContainsValueTagWithoutProperty()
{
    const QByteArray fileContent = "<classes ><class name=\"человек\"><value>20</value><property name=\"возраст\"></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("value","","","","","",incorrect_order_tags);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::invalidClassName()
{
    const QByteArray fileContent = "<classes><class name=\"мой@класс\"><property name=\"айди\"><value>10</value></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("class","","","мой@класс","","",invalid_characters_attribute);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::tooLongClassName()
{
    const QByteArray fileContent = "<classes><class name=\"ооооооочень_дллинноеееееее_название_классааааааааааааа\"><property name=\"температура\"><value>25</value></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("class","","","ооооооочень_дллинноеееееее_название_классааааааааааааа","","",invalid_length_name_class);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::duplicationNameClasses()
{
    const QByteArray fileContent = "<classes><class name=\"книга\"><property name=\"страница\"><value>205</value></property></class><class name=\"книга\"><property name=\"автор\"><value>205</value></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Class> exp_classes;
    Property book_prop("страница", 205);
    Class book("книга", book_prop);
    exp_classes.insert(book);

    QSet <Errors> exp_list_of_errors;
    Errors error("","","","книга","","",duplication_class);
    exp_list_of_errors.insert(error);

    QVERIFY(classes == exp_classes);
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::invalidPropertyValueType()
{
    const QByteArray fileContent = "<classes><class name=\"студент\"><property name=\"оценка\"><value>2_5</value></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("value","","2_5","студент","","оценка",invalid_type_value);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::tooManyClasses()
{
    const QByteArray fileContent = "<classes>""<class name=\"автомобиль\"></class>""<class name=\"бабочка\"></class>"
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
    "</classes>";

    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet<Class> exp_classes;
    const QStringList classNames = {
        "автомобиль", "бабочка", "велосипед", "гараж", "дом",
        "ёлка", "железо", "здание", "игрушка", "книга",
        "лампа", "машина", "ноутбук", "окно", "птица",
        "ручка", "солнце", "телефон", "улица", "фонарь",
        "холодильник", "цветок", "часы", "школа", "щит",
        "экран", "юла", "яблоко", "арбуз", "балкон",
        "ветер", "гора", "диван", "ежевика", "жираф",
        "зонт", "история", "камень", "лимон", "мебель",
        "небо", "облако", "поле", "река", "самолет",
        "трава", "узор", "флаг", "хлеб", "цифра",
        "человек"
    };

    for (const QString& name : classNames) {
        Class cls(name);
        exp_classes.insert(cls);
    }

    QSet <Errors> exp_list_of_errors;
    Errors error("","","51","","","",invalid_count_of_classes);
    exp_list_of_errors.insert(error);

    QVERIFY(classes == exp_classes);
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::syntaxErrorXML()
{
    const QByteArray fileContent = "<classes><class name=\"робот\"><property name=\"батарея\"><value>25</value></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("","","","","","",xml_syntax_error);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::duplicationPropertyInClass()
{
    const QByteArray fileContent = "<classes><class name=\"машина\"><property name=\"скорость\"><value>100</value></property><property name=\"скорость\"><value>20</value></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("property","","","машина","","скорость",duplication_property);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::duplicationContentOfClass()
{
    const QByteArray fileContent = "<classes><class name=\"книга\"><property name=\"страница\"><value>205</value></property></class><class name=\"евгений_онегин\"><property name=\"страница\"><value>205</value></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Class> exp_classes;
    Property book_prop("страница", 205);
    Class book("книга", book_prop);
    exp_classes.insert(book);

    QSet <Errors> exp_list_of_errors;
    Errors error("class","","","книга","евгений_онегин","",duplication_content_of_class);
    exp_list_of_errors.insert(error);

    QVERIFY(classes == exp_classes);
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::invalidPropertyName()
{
    const QByteArray fileContent = "<classes><class name=\"машина\"><property name=\"speed\"></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("property","","","машина","","speed",invalid_characters_attribute);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::invalidClassNameAttribute()
{
    const QByteArray fileContent = "<classes><class n=\"машина\"><property name=\"скорость\"></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("class","n","","","","",invalid_name_attribute);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::invalidPropertyNameAttribute()
{
    const QByteArray fileContent = "<classes><class name=\"машина\"><property n=\"скорость\"></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("property","n","","","","",invalid_name_attribute);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::tooLongLengthNameProperty()
{
    const QByteArray fileContent = "<classes><class name=\"машина\"><property name=\"скорость_скорость_скорость_скорость_скорость_скорость\"></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("property","","","машина","","скорость_скорость_скорость_скорость_скорость_скорость",invalid_length_name_property);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::PropertyValueOutOfRange()
{
    const QByteArray fileContent = "<classes><class name=\"машина\"><property name=\"скорость\"><value>1020202</value></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("value","","1020202","машина","","скорость",invalid_range_value);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::invalidNameTag()
{
    const QByteArray fileContent = "<classes><class name=\"машина\"><prop name=\"скорость\"></prop></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("prop","","","","","",invalid_name_tag);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::tooManyPropertiesInClass()
{
    const QByteArray fileContent = "<classes><class name=\"а\"><property name=\"б\"></property><property name=\"в\">"
                                   "</property><property name=\"г\"></property><property name=\"д\"></property><property name=\"е\">"
                                   "</property><property name=\"ё\"></property><property name=\"ж\"></property><property name=\"з\">"
                                   "</property><property name=\"и\"></property><property name=\"й\"></property><property name=\"к\">"
                                   "</property><property name=\"л\"></property><property name=\"м\"></property><property name=\"н\">"
                                   "</property><property name=\"о\"></property><property name=\"п\"></property><property name=\"р\">"
                                   "</property><property name=\"с\"></property><property name=\"т\"></property><property name=\"у\">"
                                   "</property><property name=\"ф\"></property><property name=\"х\"></property><property name=\"ц\">"
                                   "</property><property name=\"ч\"></property><property name=\"ш\"></property><property name=\"щ\">"
                                   "</property><property name=\"ъ\"></property><property name=\"ы\"></property><property name=\"ь\">"
                                   "</property><property name=\"э\"></property><property name=\"ю\"></property><property name=\"я\"></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("","","","а","","",invalid_count_properties_in_class);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}

void tests_parseXMLcontent::missedAttributeInTag()
{
    const QByteArray fileContent = "<classes><class name=\"машина\"><property></property></class></classes>";
    QStringList allowedTags = {"classes", "class", "property", "value"};
    QSet <Errors> list_of_errors;
    QSet <Class> classes;

    ParseXMLcontent(fileContent, allowedTags, list_of_errors, classes);

    QSet <Errors> exp_list_of_errors;
    Errors error("property","","","","","",missed_attribute);
    exp_list_of_errors.insert(error);

    QVERIFY(classes.isEmpty());
    QVERIFY(exp_list_of_errors == list_of_errors);
}
