QT += core testlib
QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle
CONFIG -= debug_and_release debug_and_release_target

MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp/obj
RCC_DIR = ./tmp/rcc
UI_DIR = ./tmp/ui
DESTDIR = $$BUILD_DIR

SOURCES += \
    command_parser.cpp
