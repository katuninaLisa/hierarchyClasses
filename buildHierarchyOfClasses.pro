QT = core testlib

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        class.cpp \
        main.cpp \
        property.cpp \
        tests_classhierarchydot.cpp \
        tests_parsexmlcontent.cpp \
        testsbuildhierarchy.cpp

QT += core testlib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    class.h \
    errors.h \
    property.h \
    tests_classhierarchydot.h \
    tests_parsexmlcontent.h \
    testsbuildhierarchy.h
