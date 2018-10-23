TEMPLATE = app
TARGET = qgsquickios

include(config.pri)

QGIS_QML_DIR = $${QGIS_INSTALL_PATH}/QGIS.app/Contents/MacOS/qml
QGIS_PREFIX_PATH = $${QGIS_INSTALL_PATH}/QGIS.app/Contents/MacOS
QGIS_QUICK_FRAMEWORK = $${QGIS_INSTALL_PATH}/QGIS.app/Contents/MacOS/lib/qgis_quick.framework
QGIS_CORE_FRAMEWORK = $${QGIS_INSTALL_PATH}/QGIS.app/Contents/Frameworks/qgis_core.framework

exists($${QGIS_CORE_FRAMEWORK}/qgis_core) {
   message("Building from QGIS: $${QGIS_INSTALL_PATH}")
} else {
   error("Missing qgis_core Framework in $${QGIS_CORE_FRAMEWORK}/qgis_core")
}

INCLUDEPATH += \
  $${QGIS_QUICK_FRAMEWORK}/Headers \
  $${QGIS_CORE_FRAMEWORK}/Headers


QGIS_LIB_DIR = $${QGIS_INSTALL_PATH}/lib
QGIS_PROVIDER_DIR = $${QGIS_INSTALL_PATH}/QGIS.app/Contents/PlugIns/qgis
QGIS_QML_DIR = $${QGIS_INSTALL_PATH}/QGIS.app/Contents/MacOS/qml
QT_LIBS_DIR = $$dirname(QMAKE_QMAKE)/../lib

CONFIG -= bitcode
CONFIG += static

DEFINES += "CORE_EXPORT="
DEFINES += "QUICK_EXPORT="

CONFIG(debug, debug|release) {
  DEFINES += "QGIS_PREFIX_PATH=$${QGIS_PREFIX_PATH}"
}


DEFINES += QT_NO_SSL

# TODO is this needed with all static libs?
QMAKE_RPATHDIR += @executable_path/../Frameworks

QT += core qml quick xml positioning quickcontrols2 svg
QT += quick sql concurrent

QTPLUGIN += qsvg qsvgicon qtsensors_ios qios qtposition_cl qsqlite

LIBS += -L$${QGIS_LIB_DIR} -L$${QGIS_PROVIDER_DIR} -L$${QGIS_QML_DIR}/QgsQuick/ -L$${QGIS_QML_DIR}/../lib/
LIBS += -lgeos -lqt5keychain -lqca-qt5 -lgdal
LIBS += -lexpat -lgeos_c -lgsl -lgslcblas -lcharset -lfreexl
LIBS += -ltiff -lgdal -lproj -lspatialindex -lpq -lspatialite -lqca-qt5 -ltasn1
LIBS += -lzip -liconv -lbz2
LIBS += -lqgis_quick_plugin

LIBS += -F$${QGIS_INSTALL_PATH}/QGIS.app/Contents/MacOS/lib  \
        -F$${QGIS_INSTALL_PATH}/QGIS.app/Contents/Frameworks

LIBS += -framework qgis_quick \
        -framework qgis_core

HEADERS +=
SOURCES += main.cpp

RESOURCES += \
  $$QGIS_QML_DIR/QgsQuick/qgsquick.qrc \
  qml.qrc
