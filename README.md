Demo application using QGIS Quick for iOS devices
=================================================

<b> Experimental! </b>

First follow instruction on https://github.com/lutraconsulting/OSGeo4iOS to 
build QgsQuick library. 

Copy config.pri.default to config.pri and update with your paths to OSGeo4iOS

Open qgsquickios.pro in QT Creator and setup you iOS Kit.

Connect you iPad device

Build on iOS target and deploy

Notes
=====

Only memory provider works, it is not possible to use any other provider ATM.
