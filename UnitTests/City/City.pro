QT += testlib

TARGET = citytest

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
  citytest.cc \
	../../Game/city.cc

HEADERS += \
  ../../Game/city.hh

INCLUDEPATH +=\
  ../../Game \
	../../Course/CourseLib
