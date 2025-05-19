#ifndef ERRORS_H
#define ERRORS_H
#include <QString>

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

#endif // ERRORS_H
