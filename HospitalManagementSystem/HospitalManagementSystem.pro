QT += widgets sql

CONFIG += c++17

TARGET = HospitalManagementSystem
TEMPLATE = app

SOURCES += \
    main.cpp \
    domain/doctor.cpp \
    domain/patient.cpp \
    domain/hospital.cpp \
    domain/consultationsession.cpp \
    domain/proximityengine.cpp \
    database/databasemanager.cpp \
    manager/systemmanager.cpp \
    gui/mainwindow.cpp \
    gui/dashboardpage.cpp \
    gui/hospitalspage.cpp \
    gui/doctorspage.cpp \
    gui/registerpatientpage.cpp \
    gui/patientspage.cpp \
    gui/aboutpage.cpp \
    gui/consultationresultdialog.cpp

HEADERS += \
    domain/person.h \
    domain/doctor.h \
    domain/patient.h \
    domain/hospital.h \
    domain/consultationsession.h \
    domain/proximityengine.h \
    database/databasemanager.h \
    manager/systemmanager.h \
    gui/mainwindow.h \
    gui/dashboardpage.h \
    gui/hospitalspage.h \
    gui/doctorspage.h \
    gui/registerpatientpage.h \
    gui/patientspage.h \
    gui/aboutpage.h \
    gui/consultationresultdialog.h
