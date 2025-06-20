/*!
 * \file tests_classHierarchyDOT.h
 * \brief Заголовочный файл для класса tests_classHierarchyDOT
 *\file tests_classHierarchyDOT.cpp
 *\brief Реализация тестов для функции classHierarchyDOT
 */

#ifndef TESTS_CLASSHIERARCHYDOT_H
#define TESTS_CLASSHIERARCHYDOT_H
#include "class.h"
#include "property.h"
#include <QObject>

extern QString classHierarchyDOT(QMap <QString, QMap <QString, int>> &matrix);

/*!
 * \brief Класс tests_classHierarchyDOT предназначен для проверки функции classHierarchyDOT
 *
 * Этот класс предоставляет набор тестов для проверки корректности работы функции classHierarchyDOT,
 *  которая преобразует матрицу связей классов в DOT-формат.
 */
class tests_classHierarchyDOT: public QObject
{
    Q_OBJECT
public:
    explicit tests_classHierarchyDOT(QObject *parent = nullptr);

private slots:

    /*!
     * \brief Предоставляет заполнение данными тестовых ситуаций
     *
     * Этот метод определяет тестовые ситуации, устанавливая различные значения
     * для входного параметра (QMap <QString, QMap <QString, int>> &matrix) и ожидаемых результатов.
     */
    void tests_data();

    /*!
     * \brief Выполняет запуск тестовых ситуаций с использованием предоставленных данных.
     *
     * Этот метод вызывается для каждой строки данных, предоставленной методом tests_data().
     * Он извлекает входные значения, выполняет формирование строки DOT-формата
     * и сравнивает полученные результаты с ожидаемыми.
     */
    void tests();
};

//! \brief Псевдоним типа для матрицы связей классов
typedef QMap <QString, QMap <QString, int>> newQMapName;

#endif // TESTS_CLASSHIERARCHYDOT_H
