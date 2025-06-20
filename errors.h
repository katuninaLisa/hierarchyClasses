/*!
 * \file errors.h
 * \brief Заголовочный файл для структуры Errors
 *
 * Этот файл содержит объявление структуры Errors, который представляет
 * хранилище о типах ошибок
 * методы для работы со структурой
 */

#ifndef ERRORS_H
#define ERRORS_H
#include <QString>
#include <QMetaType>

/*! \brief Типы ошибок приложения
 *
 *  Перечисление всех возможных ошибок обработки классов:
 *  - Ошибки файлов и путей
 *  - Ошибки синтаксиса XML
 *  - Ошибки валидации данных
 *  - Ошибки уникальности элементов
 */
enum error_type
{
    invalid_input_file_path,               //!< Некорректный путь к входному файлу
    invalid_output_file_path,              //!< Некорректный путь к выходному файлу
    incorrect_extension_input_file_path,   //!< Некорректное расширение входного файла
    incorrect_extension_output_file_path,  //!< Некорректное расширение выходного файла
    xml_syntax_error,                      //!< Синтаксическая ошибка в XML
    invalid_name_tag,                      //!< Недопустимое имя тега
    missed_tag,                            //!< Отсутствует тег
    invalid_name_attribute,                //!< Недопустимое имя атрибута
    missed_attribute,                      //!< Отсутствует атрибут
    invalid_characters_attribute,          //!< Недопустимые символы в атрибуте
    invalid_type_value,                    //!< Некорректный тип значения
    invalid_range_value,                   //!< Значение вне допустимого диапазона
    invalid_length_name_class,             //!< Некорректная длина имени класса
    invalid_length_name_property,          //!< Некорректная длина имени свойства
    invalid_count_properties_in_class,     //!< Некорректное количество свойств
    invalid_count_of_classes,              //!< Некорректное количество классов
    incorrect_order_tags,                  //!< Неправильная вложенность тегов
    duplication_class,                     //!< Дублирование классов
    duplication_property,                  //!< Дублирование свойств
    duplication_content_of_class,          //!< Дублирование содержимого классов
    empty_file                             //!< Пустой файл
};

/*! \brief Структура для хранения информации об ошибке
 *
 *  Содержит все детали ошибки для формирования
 *  понятного сообщения пользователю.
 */
struct Errors
{
    QString nameTag;        //!< Имя тега, связанного с ошибкой
    QString attribute;      //!< Атрибут, связанный с ошибкой
    QString value;          //!< Значение, вызвавшее ошибку
    QString className;      //!< Имя класса, связанного с ошибкой
    QString otherClassName; //!< Дополнительное имя класса (для ошибок отношений)
    QString propertyName;   //!< Имя свойства, связанного с ошибкой
    error_type type;        //!< Тип ошибки

    /*! \brief Конструктор структуры ошибки
     *  \param[in] n - имя тега
     *  \param[in] a - атрибут
     *  \param[in] v - значение
     *  \param[in] c - имя класса
     *  \param[in] o - другое имя класса
     *  \param[in] p - имя свойства
     *  \param[in] t - тип ошибки
     */
    Errors(const QString& n, const QString& a, const QString& v, const QString& c, const QString& o, const QString& p, error_type t):
        nameTag(n), attribute(a), value(v), className(c), otherClassName(o), propertyName(p), type(t)
    {}

    /*! \brief Формирует сообщение об ошибке
     *  \return Понятное текстовое описание ошибки
     */
    QString error() const
    {
        QString message;
        switch (type){
        case invalid_input_file_path:
            message = "Ошибка. Входной файл не найден";
            break;
        case invalid_output_file_path:
            message = "Ошибка. Выходной файл не найден";
            break;
        case incorrect_extension_input_file_path:
            message = "Ошибка. Неккоректное расширение входного файла";
            break;
        case incorrect_extension_output_file_path:
            message = "Ошибка. Неккоректное расширение выходного файла";
            break;
        case xml_syntax_error:
            message = "Ошибка: Некорректный формат XML-файла. Проверьте синтаксис и структуру.";
            break;
        case invalid_name_tag:
            message = QString("Ошибка: Неккоректное название тега <%1>").arg(nameTag);
            break;
        case missed_tag:
            message = QString("Ошибка: Отсутствует тег <%1>.").arg(nameTag);
            break;
        case invalid_name_attribute:
            message = QString("Ошибка: Необходимо использовать атрибут 'name' в теге <%1>, а не '%2'").arg(nameTag).arg(attribute);
            break;
        case missed_attribute:
            message = QString("Ошибка: В теге %1 пропущен аттрибут").arg(nameTag);
            break;
        case invalid_characters_attribute:{
            QString text;
            if (!propertyName.isEmpty())
                text = QString("%1 класс %2").arg(propertyName).arg(className);
            else
                text = QString("класс %1").arg(className);
            message = QString("Ошибка: %1 содержит недопустимые символы. Используйте только русские буквы, и нижние подчеркивания.").arg(text);
            break;
        }
        case invalid_type_value:
            message = QString("Ошибка: Недопустимый тип значения класса <%1> свойства <%2>. Ожидается целое число.").arg(className).arg(propertyName);
            break;
        case invalid_range_value:
            message = QString("Ошибка: Значение свойства <%1> класса <%2> вне допустимого диапазона: от 1 до 1000. Значение свойства: %3.").arg(propertyName).arg(className).arg(value);
            break;
        case invalid_length_name_class:
            message = QString("Ошибка: Превышена максимальная длина класса <%1>. Максимальная длина: 30.").arg(className);
            break;
        case invalid_length_name_property:
            message = QString("Ошибка: Превышена максимальная длина свойства <%1>. Максимальная длина: 30.").arg(propertyName);
            break;
        case invalid_count_properties_in_class:
            message = QString("Ошибка: Количество свойств в классе <%1> должно быть от 1 до 30.").arg(className);
            break;
        case invalid_count_of_classes:
            message = QString("Ошибка: Количество классов должно быть от 1 до 50. Фактическое кол-во классов: %1").arg(value);
            break;
        case incorrect_order_tags:{
            QString text;
            if (nameTag == "class")
                text = "вложен в classes";
            else if (nameTag == "property")
                text = "вложен в class";
            else if (nameTag == "value")
                text = "вложен в property";
            else if (nameTag == "classes")
                text = "корневым";
            message = QString("Ошибка: Неправильный порядок тегов: тег %1 должен быть %2.").arg(nameTag).arg(text);
            break;
        }
        case duplication_class:
            message = QString("Ошибка: Невозможно построить иерархию классов. Обнаружено дублирование классов %1.").arg(className);
            break;
        case duplication_property:
            message = QString("Ошибка: Невозможно построить иерархию классов. Обнаружено дублирование свойств %1 класса %2.").arg(propertyName).arg(className);
            break;
        case duplication_content_of_class:
            message = QString("Ошибка: Невозможно построить иерархию классов. Обнаружено дублирование содержимого классов %1 и %2.").arg(className).arg(otherClassName);
            break;
        case empty_file:
            message = "Ошибка: Пустой файл.";
            break;
        }
        return message;
    }

    /*! \brief Оператор сравнения ошибок
     *  \param[in] other - ошибка для сравнения
     *  \return true если ошибки идентичны
     */
    bool operator==(const Errors &other) const {
        return type == other.type && nameTag == other.nameTag && attribute == other.attribute &&
               value == other.value && className == other.className && propertyName == other.propertyName &&
               otherClassName == other.otherClassName;
    }

    /*! \brief Оператор неравенства ошибок
     *  \param[in] other - ошибка для сравнения
     *  \return true если ошибки отличаются
     */
    bool operator!=(const Errors &other) const {
        return !(*this == other);
    }
};

/*!
 * \brief Хеш-функция для структуры Errors
 * \param[in] error - ошибка для хеширования
 * \param[in] seed - начальное значение хеша
 * \return Уникальный хеш на основе всех полей ошибки
 */
inline uint qHash(const Errors &error, uint seed = 0)
{
    uint hash = qHash(static_cast<int>(error.type), seed);
    hash ^= ::qHash(error.nameTag.toUtf8(), hash);
    hash ^= ::qHash(error.attribute.toUtf8(), hash);
    hash ^= ::qHash(error.value.toUtf8(), hash);
    hash ^= ::qHash(error.className.toUtf8(), hash);
    hash ^= ::qHash(error.propertyName.toUtf8(), hash);
    hash ^= ::qHash(error.otherClassName.toUtf8(), hash);
    return hash;
}

#endif // ERRORS_H
