# -*- coding: utf-8 -*-
"""QGIS Unit tests for QgsLayoutItemMap.

.. note:: This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
"""
__author__ = '(C) 2017 Nyall Dawson'
__date__ = '20/10/2017'
__copyright__ = 'Copyright 2017, The QGIS Project'
# This will get replaced with a git SHA1 when you do a git archive
__revision__ = '$Format:%H$'

import qgis  # NOQA

import os

from qgis.PyQt.QtCore import QFileInfo, QRectF, QDir
from qgis.PyQt.QtXml import QDomDocument
from qgis.PyQt.QtGui import QPainter, QColor

from qgis.core import (QgsLayoutItemMap,
                       QgsRectangle,
                       QgsRasterLayer,
                       QgsVectorLayer,
                       QgsLayout,
                       QgsMapSettings,
                       QgsProject,
                       QgsMultiBandColorRenderer,
                       QgsCoordinateReferenceSystem,
                       QgsTextFormat,
                       QgsFontUtils,
                       QgsPalLayerSettings,
                       QgsNullSymbolRenderer,
                       QgsPoint,
                       QgsFeature,
                       QgsVectorLayerSimpleLabeling,
                       QgsLabelingEngineSettings,
                       QgsLayoutMeasurement,
                       QgsUnitTypes,
                       QgsLayoutObject,
                       QgsProperty)

from qgis.testing import start_app, unittest
from utilities import unitTestDataPath
from qgslayoutchecker import QgsLayoutChecker
from test_qgslayoutitem import LayoutItemTestCase

start_app()
TEST_DATA_DIR = unitTestDataPath()


class TestQgsLayoutMap(unittest.TestCase, LayoutItemTestCase):

    @classmethod
    def setUpClass(cls):
        cls.item_class = QgsLayoutItemMap

    def setUp(self):
        self.report = "<h1>Python QgsLayoutItemMap Tests</h1>\n"

    def tearDown(self):
        report_file_path = "%s/qgistest.html" % QDir.tempPath()
        with open(report_file_path, 'a') as report_file:
            report_file.write(self.report)

    def __init__(self, methodName):
        """Run once on class initialization."""
        unittest.TestCase.__init__(self, methodName)
        myPath = os.path.join(TEST_DATA_DIR, 'rgb256x256.png')
        rasterFileInfo = QFileInfo(myPath)
        self.raster_layer = QgsRasterLayer(rasterFileInfo.filePath(),
                                           rasterFileInfo.completeBaseName())
        rasterRenderer = QgsMultiBandColorRenderer(
            self.raster_layer.dataProvider(), 1, 2, 3)
        self.raster_layer.setRenderer(rasterRenderer)

        myPath = os.path.join(TEST_DATA_DIR, 'points.shp')
        vector_file_info = QFileInfo(myPath)
        self.vector_layer = QgsVectorLayer(vector_file_info.filePath(),
                                           vector_file_info.completeBaseName(), 'ogr')
        assert self.vector_layer.isValid()

        # pipe = mRasterLayer.pipe()
        # assert pipe.set(rasterRenderer), 'Cannot set pipe renderer'
        QgsProject.instance().addMapLayers([self.raster_layer, self.vector_layer])

        # create layout with layout map
        self.layout = QgsLayout(QgsProject.instance())
        self.layout.initializeDefaults()
        self.map = QgsLayoutItemMap(self.layout)
        self.map.attemptSetSceneRect(QRectF(20, 20, 200, 100))
        self.map.setFrameEnabled(True)
        self.map.setLayers([self.raster_layer])
        self.layout.addLayoutItem(self.map)

    def testOverviewMap(self):
        overviewMap = QgsLayoutItemMap(self.layout)
        overviewMap.attemptSetSceneRect(QRectF(20, 130, 70, 70))
        overviewMap.setFrameEnabled(True)
        overviewMap.setLayers([self.raster_layer])
        self.layout.addLayoutItem(overviewMap)
        # zoom in
        myRectangle = QgsRectangle(96, -152, 160, -120)
        self.map.setExtent(myRectangle)
        myRectangle2 = QgsRectangle(0, -256, 256, 0)
        overviewMap.setExtent(myRectangle2)
        overviewMap.overview().setLinkedMap(self.map)
        checker = QgsLayoutChecker('composermap_overview', self.layout)
        checker.setColorTolerance(6)
        checker.setControlPathPrefix("composer_mapoverview")
        myTestResult, myMessage = checker.testLayout()
        self.report += checker.report()
        self.layout.removeLayoutItem(overviewMap)
        assert myTestResult, myMessage

    def testOverviewMapBlend(self):
        overviewMap = QgsLayoutItemMap(self.layout)
        overviewMap.attemptSetSceneRect(QRectF(20, 130, 70, 70))
        overviewMap.setFrameEnabled(True)
        overviewMap.setLayers([self.raster_layer])
        self.layout.addLayoutItem(overviewMap)
        # zoom in
        myRectangle = QgsRectangle(96, -152, 160, -120)
        self.map.setExtent(myRectangle)
        myRectangle2 = QgsRectangle(0, -256, 256, 0)
        overviewMap.setExtent(myRectangle2)
        overviewMap.overview().setLinkedMap(self.map)
        overviewMap.overview().setBlendMode(QPainter.CompositionMode_Multiply)
        checker = QgsLayoutChecker('composermap_overview_blending', self.layout)
        checker.setControlPathPrefix("composer_mapoverview")
        myTestResult, myMessage = checker.testLayout()
        self.report += checker.report()
        self.layout.removeLayoutItem(overviewMap)
        assert myTestResult, myMessage

    def testOverviewMapInvert(self):
        overviewMap = QgsLayoutItemMap(self.layout)
        overviewMap.attemptSetSceneRect(QRectF(20, 130, 70, 70))
        overviewMap.setFrameEnabled(True)
        overviewMap.setLayers([self.raster_layer])
        self.layout.addLayoutItem(overviewMap)
        # zoom in
        myRectangle = QgsRectangle(96, -152, 160, -120)
        self.map.setExtent(myRectangle)
        myRectangle2 = QgsRectangle(0, -256, 256, 0)
        overviewMap.setExtent(myRectangle2)
        overviewMap.overview().setLinkedMap(self.map)
        overviewMap.overview().setInverted(True)
        checker = QgsLayoutChecker('composermap_overview_invert', self.layout)
        checker.setControlPathPrefix("composer_mapoverview")
        myTestResult, myMessage = checker.testLayout()
        self.report += checker.report()
        self.layout.removeLayoutItem(overviewMap)
        assert myTestResult, myMessage

    def testOverviewMapCenter(self):
        overviewMap = QgsLayoutItemMap(self.layout)
        overviewMap.attemptSetSceneRect(QRectF(20, 130, 70, 70))
        overviewMap.setFrameEnabled(True)
        overviewMap.setLayers([self.raster_layer])
        self.layout.addLayoutItem(overviewMap)
        # zoom in
        myRectangle = QgsRectangle(192, -288, 320, -224)
        self.map.setExtent(myRectangle)
        myRectangle2 = QgsRectangle(0, -256, 256, 0)
        overviewMap.setExtent(myRectangle2)
        overviewMap.overview().setLinkedMap(self.map)
        overviewMap.overview().setInverted(False)
        overviewMap.overview().setCentered(True)
        checker = QgsLayoutChecker('composermap_overview_center', self.layout)
        checker.setControlPathPrefix("composer_mapoverview")
        myTestResult, myMessage = checker.testLayout()
        self.report += checker.report()
        self.layout.removeLayoutItem(overviewMap)
        assert myTestResult, myMessage

    def testMapCrs(self):
        # create layout with layout map
        map_settings = QgsMapSettings()
        map_settings.setLayers([self.vector_layer])
        layout = QgsLayout(QgsProject.instance())
        layout.initializeDefaults()

        # check that new maps inherit project CRS
        QgsProject.instance().setCrs(QgsCoordinateReferenceSystem('EPSG:4326'))
        map = QgsLayoutItemMap(layout)
        map.attemptSetSceneRect(QRectF(20, 20, 200, 100))
        map.setFrameEnabled(True)
        rectangle = QgsRectangle(-13838977, 2369660, -8672298, 6250909)
        map.setExtent(rectangle)
        map.setLayers([self.vector_layer])
        layout.addLayoutItem(map)

        self.assertEqual(map.crs().authid(), 'EPSG:4326')
        self.assertFalse(map.presetCrs().isValid())

        # overwrite CRS
        map.setCrs(QgsCoordinateReferenceSystem('EPSG:3857'))
        self.assertEqual(map.crs().authid(), 'EPSG:3857')
        self.assertEqual(map.presetCrs().authid(), 'EPSG:3857')

        checker = QgsLayoutChecker('composermap_crs3857', layout)
        checker.setControlPathPrefix("composer_map")
        result, message = checker.testLayout()
        self.report += checker.report()
        self.assertTrue(result, message)

        # overwrite CRS
        map.setCrs(QgsCoordinateReferenceSystem('EPSG:4326'))
        self.assertEqual(map.presetCrs().authid(), 'EPSG:4326')
        self.assertEqual(map.crs().authid(), 'EPSG:4326')
        rectangle = QgsRectangle(-124, 17, -78, 52)
        map.zoomToExtent(rectangle)
        checker = QgsLayoutChecker('composermap_crs4326', layout)
        checker.setControlPathPrefix("composer_map")
        result, message = checker.testLayout()
        self.report += checker.report()
        self.assertTrue(result, message)

        # change back to project CRS
        map.setCrs(QgsCoordinateReferenceSystem())
        self.assertEqual(map.crs().authid(), 'EPSG:4326')
        self.assertFalse(map.presetCrs().isValid())

    def testContainsAdvancedEffects(self):
        map_settings = QgsMapSettings()
        map_settings.setLayers([self.vector_layer])
        layout = QgsLayout(QgsProject.instance())
        map = QgsLayoutItemMap(layout)

        self.assertFalse(map.containsAdvancedEffects())
        self.vector_layer.setBlendMode(QPainter.CompositionMode_Darken)
        result = map.containsAdvancedEffects()
        self.vector_layer.setBlendMode(QPainter.CompositionMode_SourceOver)
        self.assertTrue(result)

    def testRasterization(self):
        map_settings = QgsMapSettings()
        map_settings.setLayers([self.vector_layer])
        layout = QgsLayout(QgsProject.instance())
        map = QgsLayoutItemMap(layout)

        self.assertFalse(map.requiresRasterization())
        self.vector_layer.setBlendMode(QPainter.CompositionMode_Darken)
        self.assertFalse(map.requiresRasterization())
        self.assertTrue(map.containsAdvancedEffects())

        map.setBackgroundEnabled(False)
        self.assertTrue(map.requiresRasterization())
        map.setBackgroundEnabled(True)
        map.setBackgroundColor(QColor(1, 1, 1, 1))
        self.assertTrue(map.requiresRasterization())

        self.vector_layer.setBlendMode(QPainter.CompositionMode_SourceOver)

    def testLabelMargin(self):
        """
        Test rendering map item with a label margin set
        """
        format = QgsTextFormat()
        format.setFont(QgsFontUtils.getStandardTestFont("Bold"))
        format.setSize(20)
        format.setNamedStyle("Bold")
        format.setColor(QColor(0, 0, 0))
        settings = QgsPalLayerSettings()
        settings.setFormat(format)
        settings.fieldName = "'X'"
        settings.isExpression = True
        settings.placement = QgsPalLayerSettings.OverPoint

        vl = QgsVectorLayer("Point?crs=epsg:4326&field=id:integer", "vl", "memory")
        vl.setRenderer(QgsNullSymbolRenderer())
        f = QgsFeature(vl.fields(), 1)
        for x in range(15):
            for y in range(15):
                f.setGeometry(QgsPoint(x, y))
                vl.dataProvider().addFeature(f)

        vl.setLabeling(QgsVectorLayerSimpleLabeling(settings))
        vl.setLabelsEnabled(True)

        p = QgsProject()

        engine_settings = QgsLabelingEngineSettings()
        engine_settings.setFlag(QgsLabelingEngineSettings.UsePartialCandidates, False)
        engine_settings.setFlag(QgsLabelingEngineSettings.DrawLabelRectOnly, True)
        p.setLabelingEngineSettings(engine_settings)

        p.addMapLayer(vl)
        layout = QgsLayout(p)
        layout.initializeDefaults()
        p.setCrs(QgsCoordinateReferenceSystem('EPSG:4326'))
        map = QgsLayoutItemMap(layout)
        map.attemptSetSceneRect(QRectF(10, 10, 180, 180))
        map.setFrameEnabled(True)
        map.zoomToExtent(vl.extent())
        map.setLayers([vl])
        layout.addLayoutItem(map)

        checker = QgsLayoutChecker('composermap_label_nomargin', layout)
        checker.setControlPathPrefix("composer_map")
        result, message = checker.testLayout()
        self.report += checker.report()
        self.assertTrue(result, message)

        map.setLabelMargin(QgsLayoutMeasurement(15, QgsUnitTypes.LayoutMillimeters))
        checker = QgsLayoutChecker('composermap_label_margin', layout)
        checker.setControlPathPrefix("composer_map")
        result, message = checker.testLayout()
        self.report += checker.report()
        self.assertTrue(result, message)

        map.setLabelMargin(QgsLayoutMeasurement(3, QgsUnitTypes.LayoutCentimeters))
        checker = QgsLayoutChecker('composermap_label_cm_margin', layout)
        checker.setControlPathPrefix("composer_map")
        result, message = checker.testLayout()
        self.report += checker.report()
        self.assertTrue(result, message)

        map.setMapRotation(45)
        map.zoomToExtent(vl.extent())
        map.setScale(map.scale() * 1.2)
        checker = QgsLayoutChecker('composermap_rotated_label_margin', layout)
        checker.setControlPathPrefix("composer_map")
        result, message = checker.testLayout()
        self.report += checker.report()
        self.assertTrue(result, message)

        # data defined
        map.setMapRotation(0)
        map.zoomToExtent(vl.extent())
        map.dataDefinedProperties().setProperty(QgsLayoutObject.MapLabelMargin, QgsProperty.fromExpression('1+3'))
        map.refresh()
        checker = QgsLayoutChecker('composermap_dd_label_margin', layout)
        checker.setControlPathPrefix("composer_map")
        result, message = checker.testLayout()
        self.report += checker.report()
        self.assertTrue(result, message)

    def testPartialLabels(self):
        """
        Test rendering map item with a show partial labels flag
        """
        format = QgsTextFormat()
        format.setFont(QgsFontUtils.getStandardTestFont("Bold"))
        format.setSize(20)
        format.setNamedStyle("Bold")
        format.setColor(QColor(0, 0, 0))
        settings = QgsPalLayerSettings()
        settings.setFormat(format)
        settings.fieldName = "'X'"
        settings.isExpression = True
        settings.placement = QgsPalLayerSettings.OverPoint

        vl = QgsVectorLayer("Point?crs=epsg:4326&field=id:integer", "vl", "memory")
        vl.setRenderer(QgsNullSymbolRenderer())
        f = QgsFeature(vl.fields(), 1)
        for x in range(15):
            for y in range(15):
                f.setGeometry(QgsPoint(x, y))
                vl.dataProvider().addFeature(f)

        vl.setLabeling(QgsVectorLayerSimpleLabeling(settings))
        vl.setLabelsEnabled(True)

        p = QgsProject()

        engine_settings = QgsLabelingEngineSettings()
        engine_settings.setFlag(QgsLabelingEngineSettings.UsePartialCandidates, False)
        engine_settings.setFlag(QgsLabelingEngineSettings.DrawLabelRectOnly, True)
        p.setLabelingEngineSettings(engine_settings)

        p.addMapLayer(vl)
        layout = QgsLayout(p)
        layout.initializeDefaults()
        p.setCrs(QgsCoordinateReferenceSystem('EPSG:4326'))
        map = QgsLayoutItemMap(layout)
        map.attemptSetSceneRect(QRectF(10, 10, 180, 180))
        map.setFrameEnabled(True)
        map.zoomToExtent(vl.extent())
        map.setLayers([vl])
        layout.addLayoutItem(map)

        # default should always be to hide partial labels
        self.assertFalse(map.mapFlags() & QgsLayoutItemMap.ShowPartialLabels)

        # hiding partial labels (the default)
        map.setMapFlags(QgsLayoutItemMap.MapItemFlags())
        checker = QgsLayoutChecker('composermap_label_nomargin', layout)
        checker.setControlPathPrefix("composer_map")
        result, message = checker.testLayout()
        self.report += checker.report()
        self.assertTrue(result, message)

        # showing partial labels
        map.setMapFlags(QgsLayoutItemMap.ShowPartialLabels)
        checker = QgsLayoutChecker('composermap_show_partial_labels', layout)
        checker.setControlPathPrefix("composer_map")
        result, message = checker.testLayout()
        self.report += checker.report()
        self.assertTrue(result, message)


if __name__ == '__main__':
    unittest.main()
