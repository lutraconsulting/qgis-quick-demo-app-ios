/***************************************************************************
  main.cpp
  --------------------------------------
  Date                 : Oct 2018
  Copyright            : (C) 2018 by Peter Petrik
  Email                : zilolv at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define STR1(x)  #x
#define STR(x)  STR1(x)

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QtDebug>
#include <QQmlError>
#include <QDesktopWidget>
#include <QQmlContext>
#include <QQuickWindow>
#include <qqml.h>
#include "qgsquickutils.h"
#include "qgsproject.h"
#include "qgsvectorlayer.h"
#include "qgsquickplugin.h"
#include "qgslayertree.h"

int main(int argc, char *argv[])
{
  QgsApplication app(argc, argv, true);
  QgsApplication::init();
  QgsApplication::initQgis();
  if (!QgsApplication::createDatabase())
    qDebug("Can't create qgis user DB!!!");

  // REQUIRED FOR IOS  - to load QgsQuick C++ classes
  QgsQuickPlugin plugin;
  plugin.registerTypes("QgsQuick");

  QQmlEngine engine;
  // REQUIRED FOR IOS - to load QgsQuick/*.qml files defined in qmldir
  engine.addImportPath("qrc:///");


  QgsVectorLayer* mVL3 = new QgsVectorLayer( QStringLiteral( "Point" ), QStringLiteral( "Point Layer" ), QStringLiteral( "memory" ) );
  {
    QgsVectorDataProvider *pr = mVL3->dataProvider();
    QList<QgsField> attrs;
    attrs << QgsField( QStringLiteral( "test_attr" ), QVariant::Int );
    pr->addAttributes( attrs );

    QgsFields fields;
    fields.append( attrs.back() );

    QList<QgsFeature> features;

    for (int i = 0; i< 100; ++i)
    {
      QgsFeature f1( fields, 1 );
      f1.setAttribute( 0, 1 );
      QgsGeometry f1G = QgsGeometry::fromPointXY( QgsPointXY( i, i ) );
      f1.setGeometry( f1G );
      features << f1;
    }
    pr->addFeatures( features );
    mVL3->updateFields();
  }
  QgsProject::instance()->addMapLayer( mVL3 );

  engine.addImportPath( QgsApplication::qmlImportPath() );
  engine.rootContext()->setContextProperty( "__project", QgsProject::instance() );
  engine.rootContext()->setContextProperty( "__layers", QVariant::fromValue( QgsProject::instance()->layerTreeRoot()->layerOrder() ) );

  QQmlComponent component(&engine, QUrl("qrc:/main.qml"));
  QObject *object = component.create();

  if (!component.errors().isEmpty()) {
      qDebug("%s", QgsApplication::showSettings().toLocal8Bit().data());

      qDebug() << "****************************************";
      qDebug() << "*****        QML errors:           *****";
      qDebug() << "****************************************";
      for(const QQmlError& error: component.errors()) {
        qDebug() << "  " << error;
      }
      qDebug() << "****************************************";
      qDebug() << "****************************************";
  }

  if( object == 0 )
  {
      qDebug() << "FATAL ERROR: unable to create mymap.qml";
      return EXIT_FAILURE;
  }

  // Set up the QSettings environment must be done after qapp is created
  QCoreApplication::setOrganizationName( "Lutra Consulting" );
  QCoreApplication::setOrganizationDomain( "lutraconsulting.co.uk" );
  QCoreApplication::setApplicationName( "IOSQUICKTEST" );
  QCoreApplication::setApplicationVersion("0.1");

  QgsApplication::messageLog()->logMessage(QgsQuickUtils().dumpScreenInfo());
  return app.exec();
}


