/*!
 * \file class.h
 * \brief Заголовочный файл для класса Class
 *\file class.cpp
 *\brief Реализация методов класса Class
 *
 * Этот файл содержит объявление класса Class, который представляет
 * хранилище данных о классах и их свойствах
 * методы для работы с иерархией классов
 * операции сравнения и проверки отношений между классами.
 */

#ifndef CLASS_H
#define CLASS_H
#include <QSet>
#include "property.h"
#include <QMetaType>

/*! \brief Класс-хранилище данных классов
 *
 *  Содержит информацию о классе:
 *  - Имя класса
 *  - Список свойств
 *
 *  Класс не содержит бизнес-логики, только данные.
 */
class Class
{
public:
    /*! \brief Конструктор по умолчанию. Создает пустую запись */
    Class();

    /*! \brief Конструктор записи с именем
     *  \param[in] name - уникальное имя записи
     */
    Class (const QString& name);

    /*! \brief Конструктор записи с именем и свойствами
     *  \param[in] name - уникальное имя записи
     *  \param[in] prop - набор свойств класса
     */
    Class (const QString& name, Property& prop);

    /*! \brief getName - Возвращает имя записи
     *  \return Имя записи
     */
    QString getName() const;

    /*!
     * \brief getProperties - Получить свойства класса
     * \return набор свойств класса
     */
    QSet<Property> getProperties() const;

    /*! \brief setclassName - Установить имя класса
     *  \param[in] name - новое имя класса
     */
    void setclassName(QString name);

    /*! \brief setproperties - Установить свойства класса
     *  \param[in] prop - новый набор свойств класса
     */
    void setproperties(QSet <Property> prop);

    /*! \brief Оператор сравнения классов
     *  \param[in] other - класс для сравнения
     *  \return true если классы равны, false в противном случае
     */
    bool operator==(const Class&other) const;

    /*! \brief Оператор неравенства классов
     *  \param[in] other - класс для сравнения
     *  \return true если классы не равны, false в противном случае
     */
    bool operator!=(const Class&other) const;

    /*! \brief Оператор присваивания
     *  \param[in] other - класс для копирования
     *  \return true если присваивание выполнено успешно
     */
    bool operator=(const Class&other);

    /*! \brief Проверка, является ли класс подклассом
     *  \param[in] other - потенциальный родительский класс
     *  \return true если текущий класс является подклассом other
     */
    bool isSubClass(const Class&other) const;

private:
    //! Имя класса
    QString className;

    //! Список свойств класса
    QSet<Property> properties;
};

/*!
 * \brief Вычисляет хеш-значение для объекта класса Class
 * \param[in] key - объект класса, для которого вычисляется хеш
 * \param[in] seed - начальное значение хеша (по умолчанию 0)
 * \return Вычисленное хеш-значение типа uint
 */
inline uint qHash(const Class &key, uint seed = 0) {
    uint hash = ::qHash(key.getName().toUtf8(), seed);

    QSet<Property> properties = key.getProperties();
    QSet<Property>::const_iterator it;
    for (it = properties.constBegin(); it != properties.constEnd(); ++it) {
        const Property &prop = *it;
        hash ^= qHash(prop, hash);  // Явно передаем текущий hash как seed
    }

    return hash;
}

#endif // CLASS_H
