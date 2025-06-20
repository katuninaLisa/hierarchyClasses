/*!
 * \file testsBuildHierarchy.h
 * \brief Заголовочный файл для класса testsBuildHierarchy
 *\file testsBuildHierarchy.cpp
 *\brief Реализация тестов для функции buildHierarchy
 */

#ifndef TESTSBUILDHIERARCHY_H
#define TESTSBUILDHIERARCHY_H
#include <QObject>
#include "class.h"
#include "property.h"
#include <QVector>

extern void buildHierarchy(const QSet<Class>&, QMap<QString, QMap<QString, int>>&);

/*!
 * \brief Класс testsBuildHierarchy предназначен для проверки функции buildHierarchy
 *
 * Этот класс предоставляет набор тестов для проверки корректности работы функции buildHierarchy,
 *  которая строит иерархии классов.
 */
class testsBuildHierarchy : public QObject
{
    Q_OBJECT
public:
    explicit testsBuildHierarchy(QObject *parent = nullptr);

private:
    /*! \brief Создает набор свойств для тестовых классов
     *  \param[in] props - список свойств и их значений
     *  \return Набор объектов Property
     */
    QSet<Property> createProperties(const QMap<QString, QVector<int>>& props);

private slots:

    /*!
     * \brief Предоставляет заполнение данными тестовых ситуаций
     *
     * Этот метод определяет тестовые ситуации, устанавливая различные значения
     * для входных параметров (const QSet<Class>&, QMap<QString, QMap<QString, int>>&) и ожидаемых результатов.
     */
    void testBuildHierarchy_data();

    /*!
     * \brief Выполняет запуск тестовых ситуаций с использованием предоставленных данных.
     *
     * Этот метод вызывается для каждой строки данных, предоставленной методом testBuildHierarchy_data().
     * Он извлекает входные значения, выполняет формирование матрицы смежности
     * и сравнивает полученные результаты с ожидаемыми.
     */
    void testBuildHierarchy();
};

//! \brief Псевдоним типа для матрицы связей классов
typedef QMap <QString, QMap <QString, int>> newQMapName;

//! \brief Псевдоним типа для набора классов
typedef QSet<Class> newQSetName;

#endif // TESTSBUILDHIERARCHY_H
