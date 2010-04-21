# -------------------------------------------------
# Project created by QtCreator 2010-04-20T12:08:57
# -------------------------------------------------
QT += network \
    sql \
    xml
QT -= gui
TARGET = SSS_Server
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    client.cpp \
    server.cpp
HEADERS += client.h \
    server.h
