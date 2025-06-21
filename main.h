/*!
 \mainpage Программа для построения иерархии классов
 *
 * \section  Введение
 * Данная программа предназначена для построения иерархии классов, на основе заданных классов.
 * Программа должна анализировать XML-описание классов, строить их иерархию наследования и сохранять результат в виде кода в формате DOT.
 *
 *Для запуска программы:
 *1. Программу необходимо сначала скомпилировать
 *
 *2. Далее открыть папку debug, расположенную в данной директории
 *
 *3. Далее в папке открыть cmd
 *
 *Пример запуска программы из командной строки:
 *buildHierarchyOfClasses.exe input.xml output.dot
 *
 *
 *Получение результата от программы:
 *
 *Для изменения иерархии классов необходимо отредактировать файл input.xml. Если классы будут с ошибками, они отобразятся на консоли, иначе программа запишет результат в файл output.dot, расположенный в той же папке.
 *
* \file main.h - заголовочный файл, содержащий описание функций программы
* \file main.cpp - файл содержит всю реализацию программы
*/

#ifndef MAIN_H
#define MAIN_H
#include "class.h"
#include "property.h"
#include <QtTest>
#include "testsbuildhierarchy.h"
#include "tests_parsexmlcontent.h"
#include "tests_classhierarchydot.h"
#include "errors.h"

/*!
 * \brief createMatrix - функция для инициализации матрицы смежности
 * \param matrix[out] - матрица смежности
 * \param classes[in] - список классов
 */
void createMatrix(QMap <QString, QMap<QString,int>> &matrix, const QSet <Class> &classes);

/*!
 * \brief eliminate_class_duplication - функция для удаления дублирующих связей в матрице
 * \param matrix[out] - матрица смежности
 */
void eliminate_class_duplication(QMap <QString, QMap<QString,int>> &matrix);

/*!
 * \brief buildHierarchy - функция построения иерархии классов
 * \param classes[in] - список классов
 * \param matrix[out] - результирующая матрица связей
 */
void buildHierarchy(const QSet<Class> &classes, QMap <QString, QMap<QString,int>> &matrix);

/*!
 * \brief processValueElement - обработка тега value в XML
 * \param xml[in] - XML-ридер
 * \param list_of_errors[out] - список ошибок
 * \param properties[out] - свойства класса
 * \param className[in] - имя текущего класса
 * \param propertyName[in] - имя свойства
 * \param allowedTags[in] - допустимые теги
 */
void processValueElement(QXmlStreamReader& xml,QSet<Errors>& list_of_errors, Property& properties, QString className,QString propertyName, QStringList &allowedTags);

/*!
 * \brief processPropertyElement - обработка тега property в XML
 * \param xml[in] - XML-ридер
 * \param regexName[in] - регулярное выражение для валидации имен
 * \param list_of_errors[out] - список ошибок
 * \param prop[out] - список свойств
 * \param className[in] - имя текущего класса
 * \param allowedTags[in] - допустимые теги
 */
void processPropertyElement(QXmlStreamReader& xml, QRegularExpression& regexName,QSet<Errors>& list_of_errors, QSet<Property>& prop, QString className, QStringList &allowedTags);

/*!
 * \brief processClassElement - обработка тега class в XML
 * \param xml[in] - XML-ридер
 * \param allowedTags[in] - допустимые теги
 * \param list_of_errors[out] - список ошибок
 * \param tempClasses[out] - временный список классов
 */
void processClassElement(QXmlStreamReader& xml,QStringList& allowedTags,QSet<Errors>& list_of_errors, QSet<Class>& tempClasses);

/*!
 * \brief ParseXMLcontent - парсинг XML-содержимого
 * \param fileContent[in] - содержимое XML-файла
 * \param allowedTags[in] - допустимые теги
 * \param list_of_errors[out] - список ошибок
 * \param classes[out] - результирующий список классов
 */
void ParseXMLcontent(const QByteArray &fileContent, QStringList& allowedTags, QSet<Errors >& list_of_errors, QSet <Class> &classes);

/*!
 * \brief inputXMLfile - чтение и обработка входного XML-файла
 * \param filePath[in] - путь к XML-файлу
 * \param allowedTags[in] - допустимые теги
 * \param list_of_errors[out] - список ошибок
 * \param classes[out] - результирующий список классов
 * \return 0 в случае успеха, -1 при ошибке
 */
int inputXMLfile(const QString& filePath, QStringList& allowedTags, QSet<Errors >& list_of_errors, QSet <Class> &classes);

/*!
 * \brief classHierarchyDOT - генерация DOT-представления иерархии
 * \param matrix[in] - матрица связей классов
 * \return строка с описанием иерархии в формате DOT
 */
QString classHierarchyDOT(QMap <QString, QMap <QString, int>> &matrix);

/*!
 * \brief OutputFile - запись результата в файл
 * \param filePath[in] - путь к выходному файлу
 * \param list_of_errors[out] - список ошибок
 * \param matrix[in] - матрица связей классов
 * \return 0 в случае успеха, -1 при ошибке
 */
int OutputFile(const QString &filePath, QSet <Errors> &list_of_errors, QMap<QString, QMap<QString, int>> &matrix);


#endif // MAIN_H
