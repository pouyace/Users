QT += gui widgets network sql core
TEMPLATE = app

HEADERS += \
    FileUserData.h \
    abstractuser.h \
    abstractuserverifier.h \
    adminuser.h \
    deviceenumerator.h \
    fileuserverifier.h \
    guestuser.h \
    mainwindow.h \
    masterfileuserdata.h \
    masterpcsverifier.h \
    masterusercreator.h \
    netaddverifier.h \
    netconnectionenumerator.h \
    postgresqlverifier.h \
    shareduser.h \
    signinform.h \
    signupform.h \
    usersigner.h

SOURCES += \
    abstractuser.cpp \
    abstractuserverifier.cpp \
    adminuser.cpp \
    deviceenumerator.cpp \
    fileuserverifier.cpp \
    guestuser.cpp \
    main.cpp \
    mainwindow.cpp \
    masterfileuserdata.cpp \
    masterpcsverifier.cpp \
    masterusercreator.cpp \
    netaddverifier.cpp \
    netconnectionenumerator.cpp \
    postgresqlverifier.cpp \
    shareduser.cpp \
    signinform.cpp \
    signupform.cpp \
    usersigner.cpp

FORMS += \
    mainwindow.ui \
    masterusercreator.ui \
    signinform.ui \
    signupform.ui

unix|win32: LIBS += -lMPR -lnetapi32


RESOURCES += \
    MasterPCsResource/MasterPCsResource.qrc \
    Style.qrc

QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:"level:'requireAdminstrator'"
