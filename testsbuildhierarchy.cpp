#include "testsbuildhierarchy.h"
#include <QtTest>

testsBuildHierarchy::testsBuildHierarchy(QObject *parent) : QObject(parent)
{}

QSet<Property> testsBuildHierarchy::createProperties(const QMap <QString, QVector<int>> &prop)
{
    QSet <Property> properties;
    for (auto it1 = prop.begin(); it1 != prop.end(); ++it1)
    {
        properties.insert(Property(it1.key(), it1.value()));
    }
    return properties;
}

void testsBuildHierarchy::emptyClasses()
{
    QSet <Class> classes;
    QMap <QString, QMap <QString, int>> matrix;

    buildHierarchy(classes, matrix);

    QVERIFY(matrix.isEmpty());
}

void testsBuildHierarchy::singleClass()
{
    QSet <Class> classes;
    Class human;
    human.setclassName("человек");
    classes.insert(human);

    QMap <QString, QMap <QString, int>> matrix;
    QMap <QString, QMap <QString, int>> expMatrix;
    expMatrix["человек"] = {{"человек", 0}};

    buildHierarchy(classes,matrix);

    QCOMPARE(matrix, expMatrix);
}

void testsBuildHierarchy::twoIndependentClasses()
{
    QSet <Class> classes;
    Class human;
    human.setclassName("человек");
    human.setproperties(createProperties({{"фио", {}}}));
    classes.insert(human);
    Class animal;
    animal.setclassName("животное");
    animal.setproperties(createProperties({{"вид", {}}}));
    classes.insert(animal);

    QMap <QString, QMap <QString, int>> matrix;
    QMap <QString, QMap <QString, int>> expMatrix;
    expMatrix = {{"человек", {{"человек", 0},{"животное", 0}}},
                 {"животное", {{"человек", 0}, {"животное", 0}}}};

    buildHierarchy(classes,matrix);

    QCOMPARE(matrix, expMatrix);
}

void testsBuildHierarchy::simpleInheritance()
{
    QSet <Class> classes;
    Class human;
    human.setclassName("человек");
    human.setproperties(createProperties({{"фио", {}}}));
    classes.insert(human);
    Class student;
    student.setclassName("студент");
    student.setproperties(createProperties({{"фио", {}}, {"курс", {}}}));
    classes.insert(student);

    QMap <QString, QMap <QString, int>> matrix;
    QMap <QString, QMap <QString, int>> expMatrix;
    expMatrix = {{"человек", {{"человек", 0},{"студент", 0}}},
                {"студент", {{"человек", 1}, {"студент", 0}}}};

    buildHierarchy(classes,matrix);

    QCOMPARE(matrix, expMatrix);
}

void testsBuildHierarchy::chainInheritance()
{
    QSet <Class> classes;
    Class human;
    human.setclassName("человек");
    human.setproperties(createProperties({{"фио", {}}}));
    classes.insert(human);
    Class student;
    student.setclassName("студент");
    student.setproperties(createProperties({{"фио", {}}, {"курс", {}}}));
    classes.insert(student);
    Class nadezkin;
    nadezkin.setclassName("надежкин");
    nadezkin.setproperties({{"фио", {}}, {"курс", {2}}});
    classes.insert(nadezkin);

    QMap <QString, QMap <QString, int>> matrix;
    QMap <QString, QMap <QString, int>> expMatrix;
    expMatrix = {{"человек", {{"человек", 0},{"студент", 0}, {"надежкин", 0}}},
                {"студент", {{"человек", 1}, {"студент", 0}, {"надежкин", 0}}},
                {"надежкин", {{"человек", 0}, {"студент", 1}, {"надежкин", 0}}}};

    buildHierarchy(classes,matrix);

    QCOMPARE(matrix, expMatrix);
}

void testsBuildHierarchy::multipleInheritance()
{
    QSet <Class> classes;
    Class military;
    military.setclassName("военный");
    military.setproperties(createProperties({{"фио", {}},{"звание", {}}}));
    classes.insert(military);
    Class student;
    student.setclassName("студент");
    student.setproperties(createProperties({{"фио", {}}, {"курс", {}}}));
    classes.insert(student);
    Class student_military;
    student_military.setclassName("студент_военного_училища");
    student_military.setproperties({{"фио", {}}, {"курс", {}}, {"звание", {}}});
    classes.insert(student_military);

    QMap <QString, QMap <QString, int>> matrix;
    QMap <QString, QMap <QString, int>> expMatrix;
    expMatrix = {{"военный", {{"военный", 0},{"студент", 0}, {"студент_военного_училища", 0}}},
                 {"студент", {{"военный", 0}, {"студент", 0}, {"студент_военного_училища", 0}}},
                 {"студент_военного_училища", {{"военный", 1}, {"студент", 1}, {"студент_военного_училища", 0}}}};

    buildHierarchy(classes,matrix);

    QCOMPARE(matrix, expMatrix);
}

void testsBuildHierarchy::multiLevelInheritance()
{
    QSet <Class> classes;
    Class human;
    human.setclassName("человек");
    human.setproperties(createProperties({{"фио", {}}}));
    classes.insert(human);
    Class student;
    student.setclassName("студент");
    student.setproperties(createProperties({{"фио", {}}, {"курс", {}}}));
    classes.insert(student);
    Class military;
    military.setclassName("военный");
    military.setproperties(createProperties({{"фио", {}},{"звание", {}}}));
    classes.insert(military);
    Class student_military;
    student_military.setclassName("студент_военного_училища");
    student_military.setproperties({{"фио", {}}, {"курс", {}}, {"звание", {}}});
    classes.insert(student_military);

    QMap <QString, QMap <QString, int>> matrix;
    QMap <QString, QMap <QString, int>> expMatrix;
    expMatrix = {{"человек", {{"человек", 0},{"военный", 0},{"студент", 0}, {"студент_военного_училища", 0}}},
                 {"военный", {{"человек", 1},{"военный", 0},{"студент", 0}, {"студент_военного_училища", 0}}},
                 {"студент", {{"человек", 1},{"военный", 0}, {"студент", 0}, {"студент_военного_училища", 0}}},
                 {"студент_военного_училища", {{"человек", 0},{"военный", 1}, {"студент", 1}, {"студент_военного_училища", 0}}}};

    buildHierarchy(classes,matrix);

    QCOMPARE(matrix, expMatrix);
}

void testsBuildHierarchy::multipleHierarchies()
{
    QSet <Class> classes;
    Class transport;
    transport.setclassName("транспорт");
    transport.setproperties(createProperties({{"способ_передвижения", {}}}));
    classes.insert(transport);
    Class human;
    human.setclassName("человек");
    human.setproperties(createProperties({{"фио", {}}}));
    classes.insert(human);
    Class student;
    student.setclassName("студент");
    student.setproperties(createProperties({{"фио", {}}, {"курс", {}}}));
    classes.insert(student);
    Class military;
    military.setclassName("военный");
    military.setproperties(createProperties({{"фио", {}},{"звание", {}}}));
    classes.insert(military);
    Class trolleybus;
    trolleybus.setclassName("троллейбус");
    trolleybus.setproperties(createProperties({{"способ_передвижения", {}}, {"билет", {}}}));
    classes.insert(trolleybus);

    QMap <QString, QMap <QString, int>> matrix;
    QMap <QString, QMap <QString, int>> expMatrix;
    expMatrix = {{"транспорт", {{"транспорт", 0},{"человек", 0},{"военный", 0},{"студент", 0},{"троллейбус", 0}}},
                 {"человек", {{"транспорт", 0},{"человек", 0},{"военный", 0},{"студент", 0},{"троллейбус", 0}}},
                 {"военный", {{"транспорт", 0},{"человек", 1},{"военный", 0},{"студент", 0},{"троллейбус", 0}}},
                 {"студент", {{"транспорт", 0},{"человек", 1},{"военный", 0},{"студент", 0},{"троллейбус", 0}}},
                 {"троллейбус", {{"транспорт", 1},{"человек", 0},{"военный", 0},{"студент", 0},{"троллейбус", 0}}}};

    buildHierarchy(classes,matrix);

    QCOMPARE(matrix, expMatrix);
}
