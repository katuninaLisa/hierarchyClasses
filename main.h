#ifndef MAIN_H
#define MAIN_H
#include "class.h"
#include "property.h"
#include <QtTest>
#include "testsbuildhierarchy.h"
#include "tests_parsexmlcontent.h"
#include "tests_classhierarchydot.h"
#include "errors.h"

void createMatrix(QMap <QString, QMap<QString,int>> &matrix, const QSet <Class> &classes);
void eliminate_class_duplication(QMap <QString, QMap<QString,int>> &matrix);
void buildHierarchy(const QSet<Class> &classes, QMap <QString, QMap<QString,int>> &matrix);
void processValueElement(QXmlStreamReader& xml,QSet<Errors>& list_of_errors, Property& properties, QString className,QString propertyName, QStringList &allowedTags);
void processPropertyElement(QXmlStreamReader& xml, QRegularExpression& regexName,QSet<Errors>& list_of_errors, QSet<Property>& prop, QString className, QStringList &allowedTags);
void processClassElement(QXmlStreamReader& xml,QStringList& allowedTags,QSet<Errors>& list_of_errors, QSet<Class>& tempClasses, int countClasses);
void ParseXMLcontent(const QByteArray &fileContent, QStringList& allowedTags, QSet<Errors >& list_of_errors, QSet <Class> &classes);
int inputXMLfile(const QString& filePath, QStringList& allowedTags, QSet<Errors >& list_of_errors, QSet <Class> &classes);
QString classHierarchyDOT(QMap <QString, QMap <QString, int>> &matrix);
int OutputFile(const QString &filePath, QSet <Errors> &list_of_errors, QMap<QString, QMap<QString, int>> &matrix);


#endif // MAIN_H
