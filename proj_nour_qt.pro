QT       += core gui sql network printsupport widgets axcontainer serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Candidat.cpp \
    OffreEmploi.cpp \
    QrCode.cpp \
    connection.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    smtp.cpp

HEADERS += \
    Candidat.h \
    OffreEmploi.h \
    QrCode.h \
    connection.h \
    mainwindow.h \
    qcustomplot.h \
    smtp.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rsc.qrc
