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
    void tests_parse_data(); // данные для тестов
    void tests_parse(); // тесты
};



#endif // TESTS_PARSEXMLCONTENT_H
