#ifndef TESTS_PARSEXMLCONTENT_H
#define TESTS_PARSEXMLCONTENT_H
#include "class.h"
#include "property.h"
#include "errors.h"
#include <QObject>

extern void ParseXMLcontent(const QByteArray&, QStringList&, QSet<Errors >&, QSet <Class>&);

class tests_parseXMLcontent : public QObject
{
    Q_OBJECT
public:
    explicit tests_parseXMLcontent(QObject *parent = nullptr);
private slots:
    void emptyFile(); //Пустой файл
    void correctXML(); //Корректный XML
    void missingRootClassesTag(); //Отсутствует корневой тег <classes>
    void propertyContainsClassTag(); //Некорректная вложенность тегов (в property вложен class)
    void classContainsValueTagWithoutProperty(); //Некорректная вложенность тегов (в class вложен value без тега property)
    void invalidClassName(); //Недопустимое имя класса
    void tooLongClassName(); //Превышена максимальная длина имени класса
    void duplicationNameClasses(); //Дублирование имени класса
    void invalidPropertyValueType(); //Недопустимый тип значения свойства
    void tooManyClasses(); //Превышено количество классов
    void syntaxErrorXML(); //Синтаксическая ошибка в XML
    void duplicationPropertyInClass(); //Дублирование имени свойств в классе
    void duplicationContentOfClass(); //Дублирование содержимого классов
    void invalidPropertyName(); //Имя свойства класса содержит недопустимые символы
    void invalidClassNameAttribute(); //Недопустимый атрибут для имени класса
    void invalidPropertyNameAttribute(); //Недопустимый атрибут для имени свойства
    void tooManyPropertiesInClass(); //Превышено количество свойств в классе
    void tooLongLengthNameProperty(); //Превышена максимальная длина имени свойства
    void PropertyValueOutOfRange(); //Значение свойства вне допустимого диапазона
    void invalidNameTag(); //Недопустимое имя тега
    void missedAttributeInTag(); // В теге пропущен атрибут
};



#endif // TESTS_PARSEXMLCONTENT_H
