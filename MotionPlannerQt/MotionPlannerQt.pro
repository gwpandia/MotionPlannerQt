#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T21:46:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MotionPlannerQt
TEMPLATE = app

SOURCES += \
    ../src/BFS.cpp \
    ../src/CollisionDetector.cpp \
    ../src/Configuration.cpp \
    ../src/Constant.cpp \
    ../src/ControlPoint.cpp \
    ../src/CoordinateTransform.cpp \
    ../src/FindPathRunner.cpp \
    ../src/main.cpp \
    ../src/MainFrame.cpp \
    ../src/MyCanvas.cpp \
    ../src/Obstacle.cpp \
    ../src/ObstacleBitmap.cpp \
    ../src/PathAnimator.cpp \
    ../src/PathSmoother.cpp \
    ../src/PotentialField.cpp \
    ../src/PotentialFieldCanvas.cpp \
    ../src/PotentialFieldFrame.cpp \
    ../src/Robot.cpp \
    ../src/SceneReader.cpp \
    ../src/SimpleModel.cpp \
    ../src/VisitedCanvas.cpp \
    ../src/VisitedFrame.cpp

HEADERS  += \
    ../src/BFS.h \
    ../src/CollisionDetector.h \
    ../src/Configuration.h \
    ../src/Constant.h \
    ../src/ControlPoint.h \
    ../src/CoordinateTransform.h \
    ../src/FindPathRunner.h \
    ../src/IMStrTok.h \
    ../src/MainFrame.h \
    ../src/MyCanvas.h \
    ../src/Obstacle.h \
    ../src/ObstacleBitmap.h \
    ../src/PathAnimator.h \
    ../src/PathSmoother.h \
    ../src/PotentialField.h \
    ../src/PotentialFieldCanvas.h \
    ../src/PotentialFieldFrame.h \
    ../src/resource.h \
    ../src/Robot.h \
    ../src/SceneReader.h \
    ../src/SimpleModel.h \
    ../src/Util.h \
    ../src/VisitedCanvas.h \
    ../src/VisitedFrame.h

RESOURCES += \
    ../src/icon.qrc

DISTFILES += \
    ../src/im-logo2.png \
    ../src/im-logo2.ico \
    ../src/icon.rc

#win32: LIBS += -L$$PWD/D:/LibrarySDK/atbb44_20160413oss/lib/intel64/vc14/ -ltbb

#INCLUDEPATH += $$PWD/D:/LibrarySDK/tbb44_20160413oss/include
#DEPENDPATH += $$PWD/D:/LibrarySDK/tbb44_20160413oss/include
