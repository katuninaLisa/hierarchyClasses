/*!
 * \file property.h
 * \brief Заголовочный файл для класса Property
 *\file property.cpp
 *\brief Реализация методов класса Property
 *
 * Этот файл содержит объявление класса Property, который представляет
 * хранилище данных о свойствах классов
 * набор значений каждого свойства
 * методы для работы со свойствами классов
 * операции сравнения свойств.
 */

#ifndef PROPERTY_H
#define PROPERTY_H
#include <QSet>
#include <QMap>
#include <QHash>

/*! \brief Класс-хранилище данных свойства
 *
 *  Содержит информацию о свойстве:
 *  - Имя свойства
 *  - Множество целочисленных значений
 *
 *  Класс не содержит бизнес-логики, только данные.
 */
class Property
{
public:
    /*! \brief Конструктор по умолчанию. Создает пустое свойство */
    Property();

    /*! \brief Конструктор свойства с именем и значениями
     *  \param[in] name - имя свойства
     *  \param[in] val - вектор значений свойства
     */
    Property (const QString &name, const QVector <int> &val);

    /*! \brief Конструктор свойства с именем и одним значением
     *  \param[in] name - имя свойства
     *  \param[in] val - целочисленное значение свойства
     */
    Property (const QString &name, int val);

    /*! \brief Конструктор свойства только с именем
     *  \param[in] name - имя свойства
     */
    Property (const QString &name);

    /*! \brief Получить имя свойства
     *  \return имя свойства в виде строки
     */
    QString getPropertyName() const;

    /*! \brief Получить значения свойства
     *  \return вектор целочисленных значений свойства
     */
    QVector<int> getValues() const;

    /*! \brief Установить имя свойства
     *  \param[in] propert - новое имя свойства
     */
    void setpropertyName(QString propert);

    /*! \brief Добавить значение свойства
     *  \param[in] val - новое значение свойства
     */
    void setvalues(int val);

    /*! \brief Оператор сравнения свойств
     *  \param[in] other - свойство для сравнения
     *  \return true если свойства равны, false в противном случае
     */
    bool operator==(const Property& other) const;

    /*! \brief Проверка наличия значений у свойства
     *  \return true если свойство содержит значения, false если пустое
     */
    bool valuePropertyExist() const;

private:
    //!< Имя свойства
    QString propertyName;

    //!< Список значений свойства
    QVector <int> values;
};

/*!
 * \brief Вычисляет хеш-значение для объекта класса Property
 * \param[in] key - объект свойства, для которого вычисляется хеш
 * \param[in] seed - начальное значение хеша (по умолчанию 0)
 * \return Вычисленное хеш-значение типа uint
 */
inline uint qHash(const Property &key, uint seed = 0) {
    uint hash = ::qHash(key.getPropertyName().toUtf8(), seed);
    QVector<int> values = key.getValues();
    for (int i = 0; i < values.size(); ++i) {
        hash ^= qHash(values.at(i), hash);
    }
    return hash;
}

#endif // PROPERTY_H
