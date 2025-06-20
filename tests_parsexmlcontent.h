/*!
 * \file tests_parseXMLcontent.h
 * \brief Заголовочный файл для класса tests_parseXMLcontent
 *\file tests_parseXMLcontent.cpp
 *\brief Реализация тестов для функции parseXMLcontent
 */

#ifndef TESTS_PARSEXMLCONTENT_H
#define TESTS_PARSEXMLCONTENT_H
#include "class.h"
#include "property.h"
#include "errors.h"
#include <QObject>

extern void ParseXMLcontent(const QByteArray&, QStringList&, QSet<Errors >&, QSet <Class>&);

/*!
 * \brief Класс tests_parseXMLcontent предназначен для проверки функции ParseXMLcontent
 *
 * Этот класс предоставляет набор тестов для проверки корректности работы функции ParseXMLcontent,
 *  которая проверяет корректность XML-структур.
 */
class tests_parseXMLcontent : public QObject
{
    Q_OBJECT
public:
    explicit tests_parseXMLcontent(QObject *parent = nullptr);
private slots:

    /*!
     * \brief Предоставляет заполнение данными тестовых ситуаций
     *
     * Этот метод определяет тестовые ситуации, устанавливая различные значения
     * для входных параметров (const QByteArray&, QStringList&, QSet<Errors >&, QSet <Class>&) и ожидаемых результатов.
     */
    void tests_parse_data(); // данные для тестов

    /*!
     * \brief Выполняет запуск тестовых ситуаций с использованием предоставленных данных.
     *
     * Этот метод вызывается для каждой строки данных, предоставленной методом tests_parse_data().
     * Он проверяет входные значения, корректность обработки ошибок
     * и сравнивает полученные результаты с ожидаемыми.
     */
    void tests_parse(); // тесты
};

#endif // TESTS_PARSEXMLCONTENT_H
