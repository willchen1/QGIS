/***************************************************************************
     testqgsgeometryutils.cpp
     --------------------------------------
    Date                 : 16 Jan 2015
    Copyright            : (C) 2015 by Marco Hugentobler
    Email                : marco at sourcepole dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgstest.h"
#include <QObject>
#include "qgsgeometryutils.h"
#include "qgslinestring.h"
#include "qgspolygon.h"
#include "qgsmultipolygon.h"

class TestQgsGeometryUtils: public QObject
{
    Q_OBJECT
  public:
    QString elemToString( const QDomElement &elem ) const;

  private slots:
    void testExtractLinestrings();
    void testCircleClockwise_data();
    void testCircleClockwise();
    void testAngleOnCircle_data();
    void testAngleOnCircle();
    void testLeftOfLine_data();
    void testLeftOfLine();
    void testSegmentMidPoint_data();
    void testSegmentMidPoint();
    void testSegmentMidPointCenter();
    void testCircleLength_data();
    void testCircleLength();
    void testNormalizedAngle_data();
    void testNormalizedAngle();
    void testLineAngle_data();
    void testLineAngle();
    void testLinePerpendicularAngle_data();
    void testLinePerpendicularAngle();
    void testAverageAngle_data();
    void testAverageAngle();
    void testDistanceToVertex();
    void testVerticesAtDistance();
    void testCircleCenterRadius_data();
    void testCircleCenterRadius();
    void testSqrDistToLine();
    void testAngleThreePoints();
    void testMidPoint();
    void testGradient();
    void testCoefficients();
    void testPerpendicularSegment();
    void testClosestPoint();
    void testlinesIntersection3D();
    void testSegmentIntersection();
    void testLineCircleIntersection();
    void testCircleCircleIntersection();
    void testTangentPointAndCircle();
    void testCircleCircleOuterTangents();
    void testGml();
    void testInterpolatePointOnLineQgsPoint();
    void testInterpolatePointOnLine();
    void testInterpolatePointOnLineByValue();
    void testPointOnLineWithDistance();
    void interpolatePointOnArc();
};


QString TestQgsGeometryUtils::elemToString( const QDomElement &elem ) const
{
  QString s;
  QTextStream stream( &s );
  elem.save( stream, -1 );

  return s;
}

void TestQgsGeometryUtils::testExtractLinestrings()
{
  QgsLineString *outerRing1 = new QgsLineString();
  outerRing1->setPoints( QVector<QgsPoint>() << QgsPoint( 1, 1 ) << QgsPoint( 1, 2 ) << QgsPoint( 2, 2 ) << QgsPoint( 2, 1 ) << QgsPoint( 1, 1 ) );
  QgsPolygon *polygon1 = new QgsPolygon();
  polygon1->setExteriorRing( outerRing1 );

  QgsLineString *outerRing2 = new QgsLineString();
  outerRing2->setPoints( QVector<QgsPoint>() << QgsPoint( 10, 10 ) << QgsPoint( 10, 20 ) << QgsPoint( 20, 20 ) << QgsPoint( 20, 10 ) << QgsPoint( 10, 10 ) );
  QgsPolygon *polygon2 = new QgsPolygon();
  polygon2->setExteriorRing( outerRing2 );

  QgsLineString *innerRing2 = new QgsLineString();
  innerRing2->setPoints( QVector<QgsPoint>() << QgsPoint( 14, 14 ) << QgsPoint( 14, 16 ) << QgsPoint( 16, 16 ) << QgsPoint( 16, 14 ) << QgsPoint( 14, 14 ) );
  polygon2->setInteriorRings( QVector<QgsCurve *>() << innerRing2 );

  QgsMultiPolygon mpg;
  mpg.addGeometry( polygon1 );
  mpg.addGeometry( polygon2 );

  QVector<QgsLineString *> linestrings = QgsGeometryUtils::extractLineStrings( &mpg );
  QCOMPARE( linestrings.count(), 3 );
  qDeleteAll( linestrings );
}

void TestQgsGeometryUtils::testLeftOfLine_data()
{
  QTest::addColumn<double>( "x" );
  QTest::addColumn<double>( "y" );
  QTest::addColumn<double>( "x1" );
  QTest::addColumn<double>( "y1" );
  QTest::addColumn<double>( "x2" );
  QTest::addColumn<double>( "y2" );
  QTest::addColumn<bool>( "expectedResult" );

  QTest::newRow( "leftOfLine1" ) << 0.0 << 0.0 << 0.0 << 2.0 << 1.0 << 0.0 << false;
  QTest::newRow( "leftOfLine1" ) << 0.0 << 0.0 << 1.0 << 0.0 << 0.0 << 2.0 << true;
}

void TestQgsGeometryUtils::testLeftOfLine()
{
  QFETCH( double, x );
  QFETCH( double, y );
  QFETCH( double, x1 );
  QFETCH( double, y1 );
  QFETCH( double, x2 );
  QFETCH( double, y2 );
  QFETCH( bool, expectedResult );

  QCOMPARE( QgsGeometryUtils::leftOfLine( x, y, x1, y1, x2, y2 ) < 0, expectedResult );
}

void TestQgsGeometryUtils::testCircleClockwise_data()
{
  QTest::addColumn<double>( "angle1" );
  QTest::addColumn<double>( "angle2" );
  QTest::addColumn<double>( "angle3" );
  QTest::addColumn<bool>( "expectedResult" );

  QTest::newRow( "circleClockwise1" ) << 10.0 << 300.0 << 270.0 << true;
  QTest::newRow( "circleClockwise2" ) << 270.0 << 300.0 << 10.0 << false;
  QTest::newRow( "circleClockwise3" ) << 260.0 << 245.0 << 243.0 << true;
  QTest::newRow( "circleClockwise4" ) << -90.0 << 0.0 << 90.0 << false;
  QTest::newRow( "circleClockwise5" ) << 0.0 << 90.0 << 180.0 << false;
}

void TestQgsGeometryUtils::testCircleClockwise()
{
  QFETCH( double, angle1 );
  QFETCH( double, angle2 );
  QFETCH( double, angle3 );
  QFETCH( bool, expectedResult );

  QCOMPARE( QgsGeometryUtils::circleClockwise( angle1, angle2, angle3 ), expectedResult );
}

void TestQgsGeometryUtils::testAngleOnCircle_data()
{
  QTest::addColumn<double>( "angle" );
  QTest::addColumn<double>( "angle1" );
  QTest::addColumn<double>( "angle2" );
  QTest::addColumn<double>( "angle3" );
  QTest::addColumn<bool>( "expectedResult" );

  QTest::newRow( "angleOnCircle1" ) << 280.0 << 10.0 << 300.0 << 270.0 << true;
  QTest::newRow( "angleOnCircle2" ) << 235.0 << 10.0 << 300.0 << 270.0 << false;
  QTest::newRow( "angleOnCircle3" ) << 280.0 << 45.0 << 175.0 << 35.0 << true;
  QTest::newRow( "angleOnCircle4" ) << 40.0 << 45.0 << 175.0 << 35.0 << false;
  QTest::newRow( "angleOnCircle5" ) << 335.0 << 350.0 << 175.0 << 320.0 << false;
  QTest::newRow( "angleOnCircle6" ) << 170.0 << 140.0 << 355.0 << 205.0 << false;
}

void TestQgsGeometryUtils::testAngleOnCircle()
{
  QFETCH( double, angle );
  QFETCH( double, angle1 );
  QFETCH( double, angle2 );
  QFETCH( double, angle3 );
  QFETCH( bool, expectedResult );

  QCOMPARE( QgsGeometryUtils::angleOnCircle( angle, angle1, angle2, angle3 ), expectedResult );
}

void TestQgsGeometryUtils::testSegmentMidPoint_data()
{
  QTest::addColumn<double>( "pt1x" );
  QTest::addColumn<double>( "pt1y" );
  QTest::addColumn<double>( "pt2x" );
  QTest::addColumn<double>( "pt2y" );
  QTest::addColumn<double>( "radius" );
  QTest::addColumn<bool>( "left" );
  QTest::addColumn<double>( "expectedX" );
  QTest::addColumn<double>( "expectedY" );

  QTest::newRow( "testSegmentMidPoint1" ) << 0.0 << 0.0 << 1.0 << 0.0 << 0.5 << true << 0.5 << 0.5;
}

void TestQgsGeometryUtils::testSegmentMidPoint()
{
  QFETCH( double, pt1x );
  QFETCH( double, pt1y );
  QFETCH( double, pt2x );
  QFETCH( double, pt2y );
  QFETCH( double, radius );
  QFETCH( bool, left );
  QFETCH( double, expectedX );
  QFETCH( double, expectedY );

  QgsPoint midPoint;
  bool ok = QgsGeometryUtils::segmentMidPoint( QgsPoint( pt1x, pt1y ), QgsPoint( pt2x, pt2y ),
            midPoint, radius, left );

  QVERIFY( ok );
  QGSCOMPARENEAR( midPoint.x(), expectedX, 4 * std::numeric_limits<double>::epsilon() );
  QGSCOMPARENEAR( midPoint.y(), expectedY, 4 * std::numeric_limits<double>::epsilon() );
}

void TestQgsGeometryUtils::testSegmentMidPointCenter()
{
  QgsPoint mid = QgsGeometryUtils::segmentMidPointFromCenter( QgsPoint( 10, 21 ), QgsPoint( 11, 20 ), QgsPoint( 10, 20 ) );
  QGSCOMPARENEAR( mid.x(), 10.7071, 0.0001 );
  QGSCOMPARENEAR( mid.y(), 20.7071, 0.0001 );
  QgsGeometryUtils::segmentMidPointFromCenter( QgsPoint( 10, 21 ), QgsPoint( 11, 20 ), QgsPoint( 10, 20 ), false );
  QGSCOMPARENEAR( mid.x(), 10.7071, 0.0001 );
  QGSCOMPARENEAR( mid.y(), 20.7071, 0.0001 );
  mid = QgsGeometryUtils::segmentMidPointFromCenter( QgsPoint( 10, 21 ), QgsPoint( 9, 20 ), QgsPoint( 10, 20 ) );
  QGSCOMPARENEAR( mid.x(), 9.292893, 0.0001 );
  QGSCOMPARENEAR( mid.y(), 20.7071, 0.0001 );
  mid = QgsGeometryUtils::segmentMidPointFromCenter( QgsPoint( 10, 21 ), QgsPoint( 10, 19 ), QgsPoint( 10, 20 ) );
  QGSCOMPARENEAR( mid.x(), 11.0, 0.0001 );
  QGSCOMPARENEAR( mid.y(), 20.0, 0.0001 );
  mid = QgsGeometryUtils::segmentMidPointFromCenter( QgsPoint( 10, 21 ), QgsPoint( 10, 21 ), QgsPoint( 10, 20 ) );
  QGSCOMPARENEAR( mid.x(), 10.0, 0.0001 );
  QGSCOMPARENEAR( mid.y(), 21.0, 0.0001 );
  mid = QgsGeometryUtils::segmentMidPointFromCenter( QgsPoint( 10, 21 ), QgsPoint( 10, 21 ), QgsPoint( 10, 21 ) );
  QGSCOMPARENEAR( mid.x(), 10.0, 0.0001 );
  QGSCOMPARENEAR( mid.y(), 21.0, 0.0001 );
  mid = QgsGeometryUtils::segmentMidPointFromCenter( QgsPoint( 10, 21, 3, 4 ), QgsPoint( 11, 20, 5, 6 ), QgsPoint( 10, 20, 7, 8 ) );
  QGSCOMPARENEAR( mid.x(), 10.7071, 0.0001 );
  QGSCOMPARENEAR( mid.y(), 20.7071, 0.0001 );
  QCOMPARE( mid.z(), 7.0 );
  QCOMPARE( mid.m(), 8.0 );
}

void TestQgsGeometryUtils::testCircleLength_data()
{
  QTest::addColumn<double>( "x1" );
  QTest::addColumn<double>( "y1" );
  QTest::addColumn<double>( "x2" );
  QTest::addColumn<double>( "y2" );
  QTest::addColumn<double>( "x3" );
  QTest::addColumn<double>( "y3" );
  QTest::addColumn<double>( "expected" );

  QTest::newRow( "circleLength1" ) << 1.0 << 0.0 << -1.0 << 0.0 << 0.0 << 1.0 << ( 2 * M_PI * 0.75 );
  QTest::newRow( "circleLength2" ) << 0.0 << 1.0 << -1.0 << 0.0 << 1.0 << 0.0 << ( 2 * M_PI * 0.75 );
}

void TestQgsGeometryUtils::testCircleLength()
{
  QFETCH( double, x1 );
  QFETCH( double, y1 );
  QFETCH( double, x2 );
  QFETCH( double, y2 );
  QFETCH( double, x3 );
  QFETCH( double, y3 );
  QFETCH( double, expected );

  QGSCOMPARENEAR( expected, QgsGeometryUtils::circleLength( x1, y1, x2, y2, x3, y3 ), 4 * std::numeric_limits<double>::epsilon() );
}

void TestQgsGeometryUtils::testNormalizedAngle_data()
{
  QTest::addColumn<double>( "input" );
  QTest::addColumn<double>( "expected" );
  QTest::newRow( "normalizedAngle 0" ) << 0.0 << 0.0;
  QTest::newRow( "normalizedAngle 1.5708" ) << 1.5708 << 1.5708;
  QTest::newRow( "normalizedAngle 3.1416" ) << 3.1416 << 3.1416;
  QTest::newRow( "normalizedAngle 4.7124" ) << 4.7124 << 4.7124;
  QTest::newRow( "normalizedAngle 2 * M_PI" ) << 2 * M_PI << 0.0;
  QTest::newRow( "normalizedAngle 6.80678" ) << 6.80678 << 0.5236;
  QTest::newRow( "normalizedAngle 12.5664" ) << 12.5664 << 0.0;
  QTest::newRow( "normalizedAngle 12.7409" ) << 12.7409 << 0.174533;
  QTest::newRow( "normalizedAngle -0.174533" ) << -0.174533 << 6.10865;
  QTest::newRow( "normalizedAngle -6.28318" ) << -6.28318 << 0.0;
  QTest::newRow( "normalizedAngle -6.45772" ) << -6.45772 << 6.10865;
  QTest::newRow( "normalizedAngle -13.2645" ) << -13.2645 << 5.58505;
}

void TestQgsGeometryUtils::testNormalizedAngle()
{
  QFETCH( double, input );
  QFETCH( double, expected );
  QGSCOMPARENEAR( expected, QgsGeometryUtils::normalizedAngle( input ), 0.0001 );
}

void TestQgsGeometryUtils::testLineAngle_data()
{
  QTest::addColumn<double>( "x1" );
  QTest::addColumn<double>( "y1" );
  QTest::addColumn<double>( "x2" );
  QTest::addColumn<double>( "y2" );
  QTest::addColumn<double>( "expected" );

  QTest::newRow( "lineAngle undefined" ) << 0.0 << 0.0 << 0.0 << 0.0 << -99999.0; //value is unimportant, we just don't want a crash
  QTest::newRow( "lineAngle1" ) << 0.0 << 0.0 << 10.0 << 10.0 << 45.0;
  QTest::newRow( "lineAngle2" ) << 0.0 << 0.0 << 10.0 << 0.0 << 90.0;
  QTest::newRow( "lineAngle3" ) << 0.0 << 0.0 << 10.0 << -10.0 << 135.0;
  QTest::newRow( "lineAngle4" ) << 0.0 << 0.0 << 0.0 << -10.0 << 180.0;
  QTest::newRow( "lineAngle5" ) << 0.0 << 0.0 << -10.0 << -10.0 << 225.0;
  QTest::newRow( "lineAngle6" ) << 0.0 << 0.0 << -10.0 << 0.0 << 270.0;
  QTest::newRow( "lineAngle7" ) << 0.0 << 0.0 << -10.0 << 10.0 << 315.0;
  QTest::newRow( "lineAngle8" ) << 0.0 << 0.0 << 0.0 << 10.0 << 0.0;
}

void TestQgsGeometryUtils::testLineAngle()
{
  QFETCH( double, x1 );
  QFETCH( double, y1 );
  QFETCH( double, x2 );
  QFETCH( double, y2 );
  QFETCH( double, expected );

  double lineAngle = QgsGeometryUtils::lineAngle( x1, y1, x2, y2 ) * 180 / M_PI;
  if ( expected > -99999 )
    QGSCOMPARENEAR( lineAngle, expected, 4 * std::numeric_limits<double>::epsilon() );
}

void TestQgsGeometryUtils::testLinePerpendicularAngle_data()
{
  QTest::addColumn<double>( "x1" );
  QTest::addColumn<double>( "y1" );
  QTest::addColumn<double>( "x2" );
  QTest::addColumn<double>( "y2" );
  QTest::addColumn<double>( "expected" );

  QTest::newRow( "linePerpendicularAngle undefined" ) << 0.0 << 0.0 << 0.0 << 0.0 << -99999.0; //value is unimportant, we just don't want a crash
  QTest::newRow( "linePerpendicularAngle1" ) << 0.0 << 0.0 << 10.0 << 10.0 << 135.0;
  QTest::newRow( "linePerpendicularAngle2" ) << 0.0 << 0.0 << 10.0 << 0.0 << 180.0;
  QTest::newRow( "linePerpendicularAngle3" ) << 0.0 << 0.0 << 10.0 << -10.0 << 225.0;
  QTest::newRow( "linePerpendicularAngle4" ) << 0.0 << 0.0 << 0.0 << -10.0 << 270.0;
  QTest::newRow( "linePerpendicularAngle5" ) << 0.0 << 0.0 << -10.0 << -10.0 << 315.0;
  QTest::newRow( "linePerpendicularAngle6" ) << 0.0 << 0.0 << -10.0 << 0.0 << 0.0;
  QTest::newRow( "linePerpendicularAngle7" ) << 0.0 << 0.0 << -10.0 << 10.0 << 45.0;
  QTest::newRow( "linePerpendicularAngle8" ) << 0.0 << 0.0 << 0.0 << 10.0 << 90.0;
}

void TestQgsGeometryUtils::testLinePerpendicularAngle()
{
  QFETCH( double, x1 );
  QFETCH( double, y1 );
  QFETCH( double, x2 );
  QFETCH( double, y2 );
  QFETCH( double, expected );

  double pAngle = QgsGeometryUtils::linePerpendicularAngle( x1, y1, x2, y2 ) * 180 / M_PI;
  if ( expected > -99999 )
    QGSCOMPARENEAR( pAngle, expected, 0.01 );
}

void TestQgsGeometryUtils::testAverageAngle_data()
{
  QTest::addColumn<double>( "angle1" );
  QTest::addColumn<double>( "angle2" );
  QTest::addColumn<double>( "expected" );

  QTest::newRow( "testAverageAngle1" ) << 0.0 << 0.0 << 0.0;
  QTest::newRow( "testAverageAngle2" ) << 0.0 << 360.0 << 0.0;
  QTest::newRow( "testAverageAngle3" ) << 0.0 << 720.0 << 0.0;
  QTest::newRow( "testAverageAngle4" ) << 360.0 << 0.0 << 0.0;
  QTest::newRow( "testAverageAngle5" ) << -360.0 << 0.0 << 0.0;
  QTest::newRow( "testAverageAngle6" ) << -360.0 << -360.0 << 0.0;
  QTest::newRow( "testAverageAngle7" ) << 0.0 << 180.0 << 90.0;
  QTest::newRow( "testAverageAngle8" ) << 0.0 << -179.999999999999 << 270.0;
  QTest::newRow( "testAverageAngle9" ) << 315.0 << 270.0 << 292.5;
  QTest::newRow( "testAverageAngle10" ) << 45.0 << 135.0 << 90.0;
  QTest::newRow( "testAverageAngle11" ) << 315.0 << 45.0 << 0.0;
  QTest::newRow( "testAverageAngle12" ) << 45.0 << 315.0 << 0.0;
  QTest::newRow( "testAverageAngle13" ) << 315.0 << 270.0 << 292.5;
  QTest::newRow( "testAverageAngle14" ) << 140.0 << 240.0 << 190.0;
  QTest::newRow( "testAverageAngle15" ) << 240.0 << 140.0 << 190.0;
}

void TestQgsGeometryUtils::testAverageAngle()
{
  QFETCH( double, angle1 );
  QFETCH( double, angle2 );
  QFETCH( double, expected );

  double averageAngle = QgsGeometryUtils::averageAngle( angle1 * M_PI / 180.0, angle2 * M_PI / 180.0 ) * 180.0 / M_PI;
  QGSCOMPARENEAR( averageAngle, expected, 0.0000000001 );
}

void TestQgsGeometryUtils::testDistanceToVertex()
{
  //test with linestring
  QgsLineString *outerRing1 = new QgsLineString();
  outerRing1->setPoints( QVector<QgsPoint>() << QgsPoint( 1, 1 ) << QgsPoint( 1, 2 ) << QgsPoint( 2, 2 ) << QgsPoint( 2, 1 ) << QgsPoint( 1, 1 ) );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( *outerRing1, QgsVertexId( 0, 0, 0 ) ), 0.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( *outerRing1, QgsVertexId( 0, 0, 1 ) ), 1.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( *outerRing1, QgsVertexId( 0, 0, 2 ) ), 2.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( *outerRing1, QgsVertexId( 0, 0, 3 ) ), 3.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( *outerRing1, QgsVertexId( 0, 0, 4 ) ), 4.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( *outerRing1, QgsVertexId( 0, 0, 5 ) ), -1.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( *outerRing1, QgsVertexId( 0, 1, 1 ) ), -1.0 );

  //test with polygon
  QgsPolygon polygon1;
  polygon1.setExteriorRing( outerRing1 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( polygon1, QgsVertexId( 0, 0, 0 ) ), 0.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( polygon1, QgsVertexId( 0, 0, 1 ) ), 1.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( polygon1, QgsVertexId( 0, 0, 2 ) ), 2.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( polygon1, QgsVertexId( 0, 0, 3 ) ), 3.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( polygon1, QgsVertexId( 0, 0, 4 ) ), 4.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( polygon1, QgsVertexId( 0, 0, 5 ) ), -1.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( polygon1, QgsVertexId( 0, 1, 1 ) ), -1.0 );

  //test with point
  QgsPoint point( 1, 2 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( point, QgsVertexId( 0, 0, 0 ) ), 0.0 );
  QCOMPARE( QgsGeometryUtils::distanceToVertex( point, QgsVertexId( 0, 0, 1 ) ), -1.0 );
}

void TestQgsGeometryUtils::testVerticesAtDistance()
{
  //test with linestring
  QgsLineString *outerRing1 = new QgsLineString();
  QgsVertexId previous;
  QgsVertexId next;
  QVERIFY( !QgsGeometryUtils::verticesAtDistance( *outerRing1, .5, previous, next ) );

  outerRing1->setPoints( QVector<QgsPoint>() << QgsPoint( 1, 1 ) << QgsPoint( 1, 2 ) << QgsPoint( 2, 2 ) << QgsPoint( 2, 1 ) << QgsPoint( 3, 1 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( *outerRing1, .5, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 0 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 1 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( *outerRing1, 1.5, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 1 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 2 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( *outerRing1, 2.5, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 2 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 3 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( *outerRing1, 3.5, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 3 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 4 ) );
  QVERIFY( ! QgsGeometryUtils::verticesAtDistance( *outerRing1, 4.5, previous, next ) );

  // test exact hits
  QVERIFY( QgsGeometryUtils::verticesAtDistance( *outerRing1, 0, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 0 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 0 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( *outerRing1, 1, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 1 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 1 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( *outerRing1, 2, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 2 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 2 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( *outerRing1, 3, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 3 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 3 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( *outerRing1, 4, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 4 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 4 ) );
  delete outerRing1;

  // test closed line
  QgsLineString *closedRing1 = new QgsLineString();
  closedRing1->setPoints( QVector<QgsPoint>() << QgsPoint( 1, 1 ) << QgsPoint( 1, 2 ) << QgsPoint( 2, 2 ) << QgsPoint( 2, 1 ) << QgsPoint( 1, 1 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( *closedRing1, 0, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 0 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 0 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( *closedRing1, 4, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 4 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 4 ) );

  // test with polygon
  QgsPolygon polygon1;
  polygon1.setExteriorRing( closedRing1 );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( polygon1, .5, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 0 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 1 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( polygon1, 1.5, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 1 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 2 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( polygon1, 2.5, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 2 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 3 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( polygon1, 3.5, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 3 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 4 ) );
  QVERIFY( ! QgsGeometryUtils::verticesAtDistance( polygon1, 4.5, previous, next ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( polygon1, 0, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 0 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 0 ) );
  QVERIFY( QgsGeometryUtils::verticesAtDistance( polygon1, 4, previous, next ) );
  QCOMPARE( previous, QgsVertexId( 0, 0, 4 ) );
  QCOMPARE( next, QgsVertexId( 0, 0, 4 ) );

  //test with point
  QgsPoint point( 1, 2 );
  QVERIFY( !QgsGeometryUtils::verticesAtDistance( point, .5, previous, next ) );
}

void TestQgsGeometryUtils::testCircleCenterRadius_data()
{
  QTest::addColumn<double>( "x1" );
  QTest::addColumn<double>( "y1" );
  QTest::addColumn<double>( "x2" );
  QTest::addColumn<double>( "y2" );
  QTest::addColumn<double>( "x3" );
  QTest::addColumn<double>( "y3" );
  QTest::addColumn<double>( "expectedRadius" );
  QTest::addColumn<double>( "expectedCenterX" );
  QTest::addColumn<double>( "expectedCenterY" );

  QTest::newRow( "circleCenterRadius1" ) << 1.0 << 1.0 << 5.0 << 7.0 << 1.0 << 1.0 << std::sqrt( 13.0 ) << 3.0 << 4.0;
  QTest::newRow( "circleCenterRadius1" ) << 0.0 << 2.0 << 2.0 << 2.0 << 0.0 << 2.0 << 1.0 << 1.0 << 2.0;
}

void TestQgsGeometryUtils::testCircleCenterRadius()
{
  QFETCH( double, x1 );
  QFETCH( double, y1 );
  QFETCH( double, x2 );
  QFETCH( double, y2 );
  QFETCH( double, x3 );
  QFETCH( double, y3 );
  QFETCH( double, expectedRadius );
  QFETCH( double, expectedCenterX );
  QFETCH( double, expectedCenterY );

  double radius, centerX, centerY;
  QgsGeometryUtils::circleCenterRadius( QgsPoint( x1, y1 ), QgsPoint( x2, y2 ), QgsPoint( x3, y3 ), radius, centerX, centerY );
  QGSCOMPARENEAR( expectedRadius, radius, 4 * std::numeric_limits<double>::epsilon() );
  QGSCOMPARENEAR( expectedCenterX, centerX, 4 * std::numeric_limits<double>::epsilon() );
  QGSCOMPARENEAR( expectedCenterY, centerY, 4 * std::numeric_limits<double>::epsilon() );
}

//QgsGeometryUtils::sqrDistToLine
void TestQgsGeometryUtils::testSqrDistToLine()
{

  // See https://issues.qgis.org/issues/13952#note-26
  QgsPointXY qp( 771938, 6.95593e+06 );
  QgsPointXY p1( 771946, 6.95593e+06 );
  QgsPointXY p2( 771904, 6.95595e+06 );
  double rx = 0, ry = 0;
  double epsilon = 1e-18;
  double sqrDist = QgsGeometryUtils::sqrDistToLine( qp.x(), qp.y(),
                   p1.x(), p1.y(),
                   p2.x(), p2.y(),
                   rx, ry, epsilon );
  QGSCOMPARENEAR( sqrDist, 11.83, 0.01 );
}

void TestQgsGeometryUtils::testAngleThreePoints()
{
  QgsPointXY p1( 0, 0 );
  QgsPointXY p2( 1, 0 );
  QgsPointXY p3( 1, 1 );
  QGSCOMPARENEAR( QgsGeometryUtils::angleBetweenThreePoints( p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y() ), M_PI / 2.0, 0.00000001 );
  p3 = QgsPointXY( 1, -1 );
  QGSCOMPARENEAR( QgsGeometryUtils::angleBetweenThreePoints( p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y() ), 3 * M_PI / 2.0, 0.00000001 );
  p3 = QgsPointXY( 2, 0 );
  QGSCOMPARENEAR( QgsGeometryUtils::angleBetweenThreePoints( p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y() ), M_PI, 0.00000001 );
  p3 = QgsPointXY( 0, 0 );
  QGSCOMPARENEAR( QgsGeometryUtils::angleBetweenThreePoints( p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y() ), 0.0, 0.00000001 );
  p3 = QgsPointXY( 1, 0 );
  //undefined, but want no crash
  ( void )QgsGeometryUtils::angleBetweenThreePoints( p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y() );
  p2 = QgsPointXY( 0, 0 );
  ( void )QgsGeometryUtils::angleBetweenThreePoints( p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y() );
}

void TestQgsGeometryUtils::testMidPoint()
{
  QgsPoint p1( 4, 6 );
  QCOMPARE( QgsGeometryUtils::midpoint( p1, QgsPoint( 2, 2 ) ), QgsPoint( 3, 4 ) );
  QCOMPARE( QgsGeometryUtils::midpoint( QgsPoint( 4, 6, 0 ), QgsPoint( QgsWkbTypes::PointZ, 2, 2, 2 ) ), QgsPoint( QgsWkbTypes::PointZ, 3, 4, 1 ) );
  QCOMPARE( QgsGeometryUtils::midpoint( QgsPoint( QgsWkbTypes::PointM, 4, 6, 0, 0 ), QgsPoint( QgsWkbTypes::PointM, 2, 2, 0, 2 ) ), QgsPoint( QgsWkbTypes::PointM, 3, 4, 0, 1 ) );
  QCOMPARE( QgsGeometryUtils::midpoint( QgsPoint( QgsWkbTypes::PointZM, 4, 6, 0, 0 ), QgsPoint( QgsWkbTypes::PointZM, 2, 2, 2, 2 ) ), QgsPoint( QgsWkbTypes::PointZM, 3, 4, 1, 1 ) );
}

void TestQgsGeometryUtils::testGradient()
{
  QVERIFY( QgsGeometryUtils::gradient( QgsPoint( 4, 6 ), QgsPoint( 4, 8 ) ) == INFINITY );
  QGSCOMPARENEAR( QgsGeometryUtils::gradient( QgsPoint( 2, 8 ), QgsPoint( 3, 20 ) ), 12, 0.00000001 );
  QGSCOMPARENEAR( QgsGeometryUtils::gradient( QgsPoint( 2, -88 ), QgsPoint( 4, -4 ) ), 42, 0.00000001 );
  QGSCOMPARENEAR( QgsGeometryUtils::gradient( QgsPoint( 4, 6 ), QgsPoint( 8, 6 ) ), 0, 0.00000001 );
}

void TestQgsGeometryUtils::testCoefficients()
{
  double a, b, c;

  // pt1.x == pt2.x
  QgsGeometryUtils::coefficients( QgsPoint( 4, 6 ), QgsPoint( 4, 8 ), a, b, c );
  QGSCOMPARENEAR( a, 1, 0.00000001 );
  QGSCOMPARENEAR( b, 0, 0.00000001 );
  QGSCOMPARENEAR( c, -4, 0.00000001 );

  // pt1.y == pt2.y
  QgsGeometryUtils::coefficients( QgsPoint( 6, 4 ), QgsPoint( 8, 4 ), a, b, c );
  QGSCOMPARENEAR( a, 0, 0.00000001 );
  QGSCOMPARENEAR( b, 1, 0.00000001 );
  QGSCOMPARENEAR( c, -4, 0.00000001 );

  // else
  QgsGeometryUtils::coefficients( QgsPoint( 6, 4 ), QgsPoint( 4, 8 ), a, b, c );
  QGSCOMPARENEAR( a, -4, 0.00000001 );
  QGSCOMPARENEAR( b, -2, 0.00000001 );
  QGSCOMPARENEAR( c, 32, 0.00000001 );
  QgsGeometryUtils::coefficients( QgsPoint( -4, -2 ), QgsPoint( 4, 2 ), a, b, c );
  QGSCOMPARENEAR( a, -4, 0.00000001 );
  QGSCOMPARENEAR( b, 8, 0.00000001 );
  QGSCOMPARENEAR( c, 0, 0.00000001 );
}
void TestQgsGeometryUtils::testPerpendicularSegment()
{
  QgsPoint p1( 3, 13 );
  QgsPoint s1( 2, 3 );
  QgsPoint s2( 7, 11 );

  QgsLineString line_r = QgsGeometryUtils::perpendicularSegment( p1, s1, s2 );

  // default case
  QgsLineString line;
  line.addVertex( p1 );
  line.addVertex( QgsPoint( 6.7753, 10.6404 ) );
  QGSCOMPARENEARPOINT( line.pointN( 0 ), line_r.pointN( 0 ), 0.0001 );
  QGSCOMPARENEARPOINT( line.pointN( 1 ), line_r.pointN( 1 ), 0.0001 );

  // perpendicular line don't intersect segment
  line.clear();
  p1 = QgsPoint( 11, 11 );
  line_r = QgsGeometryUtils::perpendicularSegment( p1, s1, s2 );
  line.addVertex( p1 );
  line.addVertex( QgsPoint( 8.1236, 12.7978 ) );
  QGSCOMPARENEARPOINT( line.pointN( 0 ), line_r.pointN( 0 ), 0.0001 );
  QGSCOMPARENEARPOINT( line.pointN( 1 ), line_r.pointN( 1 ), 0.0001 );

  // horizontal
  s1 = QgsPoint( -3, 3 );
  s2 = QgsPoint( 2, 3 );
  line.clear();
  p1 = QgsPoint( 3, 13 );
  line_r = QgsGeometryUtils::perpendicularSegment( p1, s1, s2 );
  line.addVertex( p1 );
  line.addVertex( QgsPoint( 3, 3 ) );
  QCOMPARE( line.pointN( 0 ), line_r.pointN( 0 ) );
  QCOMPARE( line.pointN( 1 ), line_r.pointN( 1 ) );

  // vertical
  s1 = QgsPoint( 3, 13 );
  s2 = QgsPoint( 3, 3 );
  line.clear();
  p1 = QgsPoint( -7, 8 );
  line_r = QgsGeometryUtils::perpendicularSegment( p1, s1, s2 );
  line.addVertex( p1 );
  line.addVertex( QgsPoint( 3, 8 ) );
  QCOMPARE( line.pointN( 0 ), line_r.pointN( 0 ) );
  QCOMPARE( line.pointN( 1 ), line_r.pointN( 1 ) );
}

void TestQgsGeometryUtils::testClosestPoint()
{
  QgsLineString linestringZ( QVector<QgsPoint>()
                             << QgsPoint( 1, 1, 1 )
                             << QgsPoint( 1, 3, 2 ) );

  QgsPoint pt1 = QgsGeometryUtils::closestPoint( linestringZ, QgsPoint( 1, 0 ) );
  QGSCOMPARENEAR( pt1.z(), 1, 0.0001 );
  QVERIFY( std::isnan( pt1.m() ) );

  QgsLineString linestringM( QVector<QgsPoint>()
                             << QgsPoint( 1, 1, std::numeric_limits<double>::quiet_NaN(), 1 )
                             << QgsPoint( 1, 3, std::numeric_limits<double>::quiet_NaN(), 2 ) );

  QgsPoint pt2 = QgsGeometryUtils::closestPoint( linestringM, QgsPoint( 1, 4 ) );
  QVERIFY( std::isnan( pt2.z() ) );
  QGSCOMPARENEAR( pt2.m(), 2, 0.0001 );

  QgsLineString linestringZM( QVector<QgsPoint>()
                              << QgsPoint( 1, 1, 1, 1 )
                              << QgsPoint( 1, 3, 2, 2 ) );

  QgsPoint pt3 = QgsGeometryUtils::closestPoint( linestringZM, QgsPoint( 2, 2 ) );
  QGSCOMPARENEAR( pt3.z(), 1.5, 0.0001 );
  QGSCOMPARENEAR( pt3.m(), 1.5, 0.0001 );

  QgsLineString linestringDuplicatedPoint( QVector<QgsPoint>()
      << QgsPoint( 1, 1, 1, 1 )
      << QgsPoint( 1, 1, 1, 1 )
      << QgsPoint( 1, 3, 2, 2 ) );

  QgsPoint pt4 = QgsGeometryUtils::closestPoint( linestringDuplicatedPoint, QgsPoint( 1, 0 ) );
  QGSCOMPARENEAR( pt4.z(), 1, 0.0001 );
  QGSCOMPARENEAR( pt4.m(), 1, 0.0001 );
}

void TestQgsGeometryUtils::testlinesIntersection3D()
{
  QgsVector3D x;
  QVERIFY( QgsGeometryUtils::linesIntersection3D( QgsVector3D( 0, 0, 10 ), QgsVector3D( 5, 0, 10 ), QgsVector3D( 2, 1, 10 ), QgsVector3D( 2, 3, 10 ), x ) );
  QVERIFY( x == QgsVector3D( 2.0, 0.0, 10.0 ) );

  QVERIFY( QgsGeometryUtils::linesIntersection3D( QgsVector3D( 0, 0, 10 ), QgsVector3D( 5, 0, 10 ), QgsVector3D( 2, 1, 10 ), QgsVector3D( 2, 0, 10 ), x ) );
  QVERIFY( x == QgsVector3D( 2.0, 0.0, 10.0 ) );

  QVERIFY( QgsGeometryUtils::linesIntersection3D( QgsVector3D( 0, 0, 10 ), QgsVector3D( 5, 0, 10 ), QgsVector3D( 0, 1, 10 ), QgsVector3D( 0, 3, 10 ), x ) );
  QVERIFY( x == QgsVector3D( 0.0, 0.0, 10.0 ) );

  QVERIFY( QgsGeometryUtils::linesIntersection3D( QgsVector3D( 0, 0, 10 ), QgsVector3D( 5, 0, 10 ), QgsVector3D( 0, 1, 10 ), QgsVector3D( 0, 0, 10 ), x ) );
  QVERIFY( x == QgsVector3D( 0.0, 0.0, 10.0 ) );

  QVERIFY( QgsGeometryUtils::linesIntersection3D( QgsVector3D( 0, 0, 10 ), QgsVector3D( 5, 0, 10 ), QgsVector3D( 5, 1, 10 ), QgsVector3D( 5, 3, 10 ), x ) );
  QVERIFY( x == QgsVector3D( 5.0, 0.0, 10.0 ) );

  QVERIFY( QgsGeometryUtils::linesIntersection3D( QgsVector3D( 0, 0, 10 ), QgsVector3D( 5, 0, 10 ), QgsVector3D( 5, 1, 10 ), QgsVector3D( 5, 0, 10 ), x ) );
  QVERIFY( x == QgsVector3D( 5.0, 0.0, 10.0 ) );

  QVERIFY( QgsGeometryUtils::linesIntersection3D( QgsVector3D( 1, 1, 10 ), QgsVector3D( 2, 2, 10 ), QgsVector3D( 3, 1, 10 ), QgsVector3D( 3, 2, 10 ), x ) );
  QVERIFY( x == QgsVector3D( 3.0, 3.0, 10.0 ) );

  QVERIFY( QgsGeometryUtils::linesIntersection3D( QgsVector3D( 1, 1, 10 ), QgsVector3D( 2, 2, 10 ), QgsVector3D( 3, 2, 10 ), QgsVector3D( 3, 1, 10 ), x ) );
  QVERIFY( x == QgsVector3D( 3.0, 3.0, 10.0 ) );

  QVERIFY( QgsGeometryUtils::linesIntersection3D( QgsVector3D( 5, 5, 5 ), QgsVector3D( 0, 0, 0 ), QgsVector3D( 0, 5, 5 ), QgsVector3D( 5, 0, 0 ), x ) );
  QVERIFY( x == QgsVector3D( 2.5, 2.5, 2.5 ) );

  QVERIFY( QgsGeometryUtils::linesIntersection3D( QgsVector3D( 2.5, 2.5, 2.5 ), QgsVector3D( 0, 5, 0 ), QgsVector3D( 2.5, 2.5, 2.5 ), QgsVector3D( 5, 0, 0 ), x ) );
  QVERIFY( x == QgsVector3D( 2.5, 2.5, 2.5 ) );

  QVERIFY( QgsGeometryUtils::linesIntersection3D( QgsVector3D( 2.5, 2.5, 2.5 ), QgsVector3D( 5, 0, 0 ), QgsVector3D( 0, 5, 5 ), QgsVector3D( 5, 5, 5 ), x ) );
  QVERIFY( x == QgsVector3D( 0.0, 5.0, 5.0 ) );

}

void TestQgsGeometryUtils::testSegmentIntersection()
{
  const double epsilon = 1e-8;
  bool intersection = false, isIntersect = false;
  QgsPoint inter;
  // null
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 5, 5 ), QgsPoint( 5, 5 ), QgsPoint( 1, 1 ), QgsPoint( 1, 0 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( !isIntersect );
  QVERIFY( inter == QgsPoint() );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 1 ), QgsPoint( 5, 5 ), QgsPoint( 5, 5 ), inter, isIntersect, epsilon, true );
  QVERIFY( !intersection );
  QVERIFY( !isIntersect );
  QVERIFY( inter == QgsPoint() );
  // parallel
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 1 ), QgsPoint( 1, 1 ), QgsPoint( 1, 0 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( !isIntersect );
  QVERIFY( inter == QgsPoint() );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 1 ), QgsPoint( 1, 1 ), QgsPoint( 1, 0 ), inter, isIntersect, epsilon, true );
  QVERIFY( !intersection );
  QVERIFY( !isIntersect );
  QVERIFY( inter == QgsPoint() );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 1, 1 ), QgsPoint( 0, 1 ), QgsPoint( 1, 2 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( !isIntersect );
  QVERIFY( inter == QgsPoint() );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 5, 5 ), QgsPoint( 1, 1 ), QgsPoint( -1, -1 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( !isIntersect );
  QVERIFY( inter == QgsPoint() );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 5, 5 ), QgsPoint( 1, 1 ), QgsPoint( 0, 0 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( !isIntersect );
  QVERIFY( inter == QgsPoint() );
  // contigus
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 0, 5 ), QgsPoint( 1, 5 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 5 ) );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 0, 5 ), QgsPoint( 1, 5 ), inter, isIntersect, epsilon, true );
  QVERIFY( intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 5 ) );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 5 ), QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 1, 5 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 5 ) );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 5 ), QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 1, 5 ), inter, isIntersect, epsilon, true );
  QVERIFY( intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 5 ) );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 1, 5 ), QgsPoint( 0, 5 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 5 ) );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 1, 5 ), QgsPoint( 0, 5 ), inter, isIntersect, epsilon, true );
  QVERIFY( intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 5 ) );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 5 ), QgsPoint( 0, 0 ), QgsPoint( 1, 5 ), QgsPoint( 0, 5 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 5 ) );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 5 ), QgsPoint( 0, 0 ), QgsPoint( 1, 5 ), QgsPoint( 0, 5 ), inter, isIntersect, epsilon, true );
  QVERIFY( intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 5 ) );
  // colinear
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 0, 5 ), QgsPoint( 0, 6 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( !isIntersect );
  QVERIFY( inter == QgsPoint() );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 0, 5 ), QgsPoint( 0, 6 ), inter, isIntersect, epsilon, true );
  QVERIFY( !intersection );
  QVERIFY( !isIntersect );
  QVERIFY( inter == QgsPoint() );
  // improper
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 0, 2 ), QgsPoint( 1, 5 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 2 ) );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 2 ), QgsPoint( 1, 5 ), QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), inter, isIntersect, epsilon, true );
  QVERIFY( intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 2 ) );

  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 1, 5 ), QgsPoint( 0, 2 ), QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 2 ) );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 1, 5 ), QgsPoint( 0, 2 ), QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), inter, isIntersect, epsilon, true );
  QVERIFY( intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 2 ) );

  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 0, 2 ), QgsPoint( 1, 5 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 2 ) );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 0, 2 ), QgsPoint( 1, 5 ), inter, isIntersect, epsilon, true );
  QVERIFY( intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 2 ) );

  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 1, 5 ), QgsPoint( 0, 2 ), inter, isIntersect, epsilon );
  QVERIFY( !intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 2 ) );
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, 0 ), QgsPoint( 0, 5 ), QgsPoint( 1, 5 ), QgsPoint( 0, 2 ), inter, isIntersect, epsilon, true );
  QVERIFY( intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 2 ) );
  // normal
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, -5 ), QgsPoint( 0, 5 ), QgsPoint( 2, 0 ), QgsPoint( -1, 0 ), inter, isIntersect, epsilon );
  QVERIFY( intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 0 ) );
  inter = QgsPoint();
  intersection = QgsGeometryUtils::segmentIntersection( QgsPoint( 0, -5 ), QgsPoint( 0, 5 ), QgsPoint( 2, 0 ), QgsPoint( -1, 0 ), inter, isIntersect, epsilon, true );
  QVERIFY( intersection );
  QVERIFY( isIntersect );
  QVERIFY( inter == QgsPoint( 0, 0 ) );
}

void TestQgsGeometryUtils::testLineCircleIntersection()
{
  QgsPointXY center = QgsPoint( 2, 2 );
  double radius = 2.0;
  QgsPointXY linePoint1 = QgsPoint( 0, 2 );
  QgsPointXY linePoint2 = QgsPoint( 2, 2 );
  QgsPointXY intersection = QgsPoint( 3, 3 );
  bool isIntersection = QgsGeometryUtils::lineCircleIntersection( center, radius, linePoint1, linePoint2, intersection );
  QVERIFY( isIntersection );
  QVERIFY( intersection == QgsPointXY( 4, 2 ) );

  linePoint1 = QgsPoint( 5, 0 );
  linePoint2 = QgsPoint( 5, 2 );
  isIntersection = QgsGeometryUtils::lineCircleIntersection( center, radius, linePoint1, linePoint2, intersection );
  QVERIFY( !isIntersection );
}

void TestQgsGeometryUtils::testCircleCircleIntersection()
{
  QgsPointXY int1;
  QgsPointXY int2;

  // no intersections
  QCOMPARE( QgsGeometryUtils::circleCircleIntersections( QgsPointXY( 0, 0 ), 1, QgsPointXY( 2, 0 ), 0.5, int1, int2 ), 0 );
  QCOMPARE( QgsGeometryUtils::circleCircleIntersections( QgsPointXY( 0, 0 ), 1, QgsPointXY( 0.5, 0.1 ), 0.2, int1, int2 ), 0 );
  // one intersection
  QCOMPARE( QgsGeometryUtils::circleCircleIntersections( QgsPointXY( 0, 0 ), 1, QgsPointXY( 3, 0 ), 2, int1, int2 ), 1 );
  QCOMPARE( int1, QgsPointXY( 1, 0 ) );
  QCOMPARE( int2, QgsPointXY( 1, 0 ) );
  // two intersections
  QCOMPARE( QgsGeometryUtils::circleCircleIntersections( QgsPointXY( 5, 3 ), 2, QgsPointXY( 7, -1 ), 4, int1, int2 ), 2 );
  QGSCOMPARENEAR( int1.x(), 3.8, 0.001 );
  QGSCOMPARENEAR( int1.y(), 1.4, 0.001 );
  QGSCOMPARENEAR( int2.x(), 7.0, 0.001 );
  QGSCOMPARENEAR( int2.y(), 3.0, 0.001 );
}

void TestQgsGeometryUtils::testTangentPointAndCircle()
{
  QgsPointXY t1;
  QgsPointXY t2;
  QVERIFY( !QgsGeometryUtils::tangentPointAndCircle( QgsPointXY( 1, 2 ), 4, QgsPointXY( 1, 2 ), t1, t2 ) );
  QVERIFY( QgsGeometryUtils::tangentPointAndCircle( QgsPointXY( 1, 2 ), 4, QgsPointXY( 8, 4 ), t1, t2 ) );
  QGSCOMPARENEAR( t1.x(), 4.03, 0.01 );
  QGSCOMPARENEAR( t1.y(), -0.61, 0.01 );
  QGSCOMPARENEAR( t2.x(), 2.2, 0.01 );
  QGSCOMPARENEAR( t2.y(), 5.82, 0.01 );
}

void TestQgsGeometryUtils::testCircleCircleOuterTangents()
{
  QgsPointXY l1p1;
  QgsPointXY l1p2;
  QgsPointXY l2p1;
  QgsPointXY l2p2;

  // no tangents
  QCOMPARE( QgsGeometryUtils::circleCircleOuterTangents( QgsPointXY( 1, 2 ), 4, QgsPointXY( 2, 3 ), 1, l1p1, l1p2, l2p1, l2p2 ), 0 );

  // tangents
  QCOMPARE( QgsGeometryUtils::circleCircleOuterTangents( QgsPointXY( 1, 2 ), 1, QgsPointXY( 10, 3 ), 4, l1p1, l1p2, l2p1, l2p2 ), 2 );
  QGSCOMPARENEAR( l1p1.x(), 0.566, 0.01 );
  QGSCOMPARENEAR( l1p1.y(), 2.901, 0.01 );
  QGSCOMPARENEAR( l1p2.x(), 8.266, 0.01 );
  QGSCOMPARENEAR( l1p2.y(), 6.604, 0.01 );
  QGSCOMPARENEAR( l2p1.x(), 0.7749, 0.01 );
  QGSCOMPARENEAR( l2p1.y(), 1.025, 0.01 );
  QGSCOMPARENEAR( l2p2.x(), 9.099, 0.01 );
  QGSCOMPARENEAR( l2p2.y(), -0.897, 0.01 );

  // larger circle first
  QCOMPARE( QgsGeometryUtils::circleCircleOuterTangents( QgsPointXY( 10, 3 ), 4, QgsPointXY( 1, 2 ), 1, l1p1, l1p2, l2p1, l2p2 ), 2 );
  QGSCOMPARENEAR( l1p1.x(), 0.566, 0.01 );
  QGSCOMPARENEAR( l1p1.y(), 2.901, 0.01 );
  QGSCOMPARENEAR( l1p2.x(), 8.266, 0.01 );
  QGSCOMPARENEAR( l1p2.y(), 6.604, 0.01 );
  QGSCOMPARENEAR( l2p1.x(), 0.7749, 0.01 );
  QGSCOMPARENEAR( l2p1.y(), 1.025, 0.01 );
  QGSCOMPARENEAR( l2p2.x(), 9.099, 0.01 );
  QGSCOMPARENEAR( l2p2.y(), -0.897, 0.01 );
}

void TestQgsGeometryUtils::testGml()
{
  QgsPoint point = QgsPoint( 1, 2 );
  QDomDocument doc;
  QDomElement elm = QgsGeometryUtils::pointsToGML2( QgsPointSequence( ) << point, doc, 2, QStringLiteral( "gml" ) );
  QString expectedGML2( QStringLiteral( "<coordinates xmlns=\"gml\" cs=\",\" ts=\" \">1,2</coordinates>" ) );
  QGSCOMPAREGML( elemToString( elm ), expectedGML2 );
  elm = QgsGeometryUtils::pointsToGML2( QgsPointSequence( ) << point, doc, 2, QStringLiteral( "gml" ), QgsAbstractGeometry::AxisOrder::YX );
  QString expectedGML2_inverted( QStringLiteral( "<coordinates xmlns=\"gml\" cs=\",\" ts=\" \">2,1</coordinates>" ) );
  QGSCOMPAREGML( elemToString( elm ), expectedGML2_inverted );

  elm = QgsGeometryUtils::pointsToGML3( QgsPointSequence( ) << point, doc, 2, QStringLiteral( "gml" ), false, QgsAbstractGeometry::AxisOrder::XY );
  QString expectedGML3( QStringLiteral( "<posList xmlns=\"gml\" srsDimension=\"2\">1 2</posList>" ) );
  QGSCOMPAREGML( elemToString( elm ), expectedGML3 );
  elm = QgsGeometryUtils::pointsToGML3( QgsPointSequence( ) << point, doc, 2, QStringLiteral( "gml" ), false, QgsAbstractGeometry::AxisOrder::YX );
  QString expectedGML3_inverted( QStringLiteral( "<posList xmlns=\"gml\" srsDimension=\"2\">2 1</posList>" ) );
  QGSCOMPAREGML( elemToString( elm ), expectedGML3_inverted );
}

void TestQgsGeometryUtils::testInterpolatePointOnLineQgsPoint()
{
  QgsPoint p = QgsGeometryUtils::interpolatePointOnLine( QgsPoint( 0, 0 ), QgsPoint( 10, 0 ), 0 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 0.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( QgsPoint( 0, 0 ), QgsPoint( 10, 0 ), 1 );
  QCOMPARE( p.x(), 10.0 );
  QCOMPARE( p.y(), 0.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( QgsPoint( 0, 0 ), QgsPoint( 0, 10 ), 0 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 0.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( QgsPoint( 0, 0 ), QgsPoint( 0, 10 ), 1 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 10.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( QgsPoint( 0, 0 ), QgsPoint( -10, -6 ), 0.5 );
  QCOMPARE( p.x(), -5.0 );
  QCOMPARE( p.y(), -3.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( QgsPoint( 0, 0 ), QgsPoint( -10, -6 ), 0.2 );
  QCOMPARE( p.x(), -2.0 );
  QCOMPARE( p.y(), -1.2 );
  p = QgsGeometryUtils::interpolatePointOnLine( QgsPoint( 0, 0 ), QgsPoint( -10, -6 ), 2 );
  QCOMPARE( p.x(), -20.0 );
  QCOMPARE( p.y(), -12.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( QgsPoint( 0, 0 ), QgsPoint( -10, -6 ), -1 );
  QCOMPARE( p.x(), 10.0 );
  QCOMPARE( p.y(), 6.0 );
  // with m
  p = QgsGeometryUtils::interpolatePointOnLine( QgsPoint( 0, 0, 0, 5, QgsWkbTypes::PointM ), QgsPoint( -10, -6, 0, 10, QgsWkbTypes::PointM ), 0.4 );
  QCOMPARE( p.wkbType(), QgsWkbTypes::PointM );
  QCOMPARE( p.x(), -4.0 );
  QCOMPARE( p.y(), -2.4 );
  QCOMPARE( p.m(), 7.0 );
  // with z
  p = QgsGeometryUtils::interpolatePointOnLine( QgsPoint( 0, 0, 5, 0, QgsWkbTypes::PointZ ), QgsPoint( -10, -6, 10, 0, QgsWkbTypes::PointZ ), 0.4 );
  QCOMPARE( p.wkbType(), QgsWkbTypes::PointZ );
  QCOMPARE( p.x(), -4.0 );
  QCOMPARE( p.y(), -2.4 );
  QCOMPARE( p.z(), 7.0 );
  // with zm
  p = QgsGeometryUtils::interpolatePointOnLine( QgsPoint( 0, 0, 5, 10, QgsWkbTypes::PointZM ), QgsPoint( -10, -6, 10, 5, QgsWkbTypes::PointZM ), 0.4 );
  QCOMPARE( p.wkbType(), QgsWkbTypes::PointZM );
  QCOMPARE( p.x(), -4.0 );
  QCOMPARE( p.y(), -2.4 );
  QCOMPARE( p.z(), 7.0 );
  QCOMPARE( p.m(), 8.0 );
}

void TestQgsGeometryUtils::testInterpolatePointOnLine()
{
  QgsPointXY p = QgsGeometryUtils::interpolatePointOnLine( 0, 0, 10, 0, 0 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 0.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( 0, 0, 10, 0, 1 );
  QCOMPARE( p.x(), 10.0 );
  QCOMPARE( p.y(), 0.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( 0, 0, 0, 10, 0 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 0.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( 0, 0, 0, 10, 1 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 10.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( 0, 0, -10, -6, 0.5 );
  QCOMPARE( p.x(), -5.0 );
  QCOMPARE( p.y(), -3.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( 0, 0, -10, -6, 0.2 );
  QCOMPARE( p.x(), -2.0 );
  QCOMPARE( p.y(), -1.2 );
  p = QgsGeometryUtils::interpolatePointOnLine( 0, 0, -10, -6, 2 );
  QCOMPARE( p.x(), -20.0 );
  QCOMPARE( p.y(), -12.0 );
  p = QgsGeometryUtils::interpolatePointOnLine( 0, 0, -10, -6, -1 );
  QCOMPARE( p.x(), 10.0 );
  QCOMPARE( p.y(), 6.0 );
}

void TestQgsGeometryUtils::testInterpolatePointOnLineByValue()
{
  QgsPointXY p = QgsGeometryUtils::interpolatePointOnLineByValue( 0, 0, 0, 10, 0, 1, 0 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 0.0 );
  p = QgsGeometryUtils::interpolatePointOnLineByValue( 0, 0, 0, 10, 0, 1, 1 );
  QCOMPARE( p.x(), 10.0 );
  QCOMPARE( p.y(), 0.0 );
  p = QgsGeometryUtils::interpolatePointOnLineByValue( 0, 0, 5, 0, 10, 15, 5 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 0.0 );
  p = QgsGeometryUtils::interpolatePointOnLineByValue( 0, 0, 15, 0, 10, 5, 5 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 10.0 );
  p = QgsGeometryUtils::interpolatePointOnLineByValue( 0, 0, 1, -10, -6, 3, 2 );
  QCOMPARE( p.x(), -5.0 );
  QCOMPARE( p.y(), -3.0 );
  p = QgsGeometryUtils::interpolatePointOnLineByValue( 0, 0, 1, -10, -6, 3, 1.4 );
  QCOMPARE( p.x(), -2.0 );
  QCOMPARE( p.y(), -1.2 );
  p = QgsGeometryUtils::interpolatePointOnLineByValue( 0, 0, 3, -10, -6, 1, 2 );
  QCOMPARE( p.x(), -5.0 );
  QCOMPARE( p.y(), -3.0 );
  p = QgsGeometryUtils::interpolatePointOnLineByValue( 0, 0, 1, -10, -6, 3, -1 );
  QCOMPARE( p.x(), 10.0 );
  QCOMPARE( p.y(), 6.0 );
  p = QgsGeometryUtils::interpolatePointOnLineByValue( 0, 0, 1, -10, -6, 3, 5 );
  QCOMPARE( p.x(), -20.0 );
  QCOMPARE( p.y(), -12.0 );
  // v1 == v2, test for no crash
  p = QgsGeometryUtils::interpolatePointOnLineByValue( 0, 0, 1, -10, -6, 1, 1 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 0.0 );
}

void TestQgsGeometryUtils::testPointOnLineWithDistance()
{
  QgsPoint p = QgsGeometryUtils::pointOnLineWithDistance( QgsPoint( 0, 0 ), QgsPoint( 10, 0 ), 0 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 0.0 );
  double x;
  double y;
  QgsGeometryUtils::pointOnLineWithDistance( 0, 0, 10, 0, 0, x, y );
  QCOMPARE( x, 0.0 );
  QCOMPARE( y, 0.0 );

  p = QgsGeometryUtils::pointOnLineWithDistance( QgsPoint( 2, 3 ),  QgsPoint( 12, 3 ), 10 );
  QCOMPARE( p.x(), 12.0 );
  QCOMPARE( p.y(), 3.0 );
  QgsGeometryUtils::pointOnLineWithDistance( 2, 3,  12, 3, 10, x, y );
  QCOMPARE( x, 12.0 );
  QCOMPARE( y, 3.0 );

  p = QgsGeometryUtils::pointOnLineWithDistance( QgsPoint( 0, 0 ),  QgsPoint( 0, 10 ), 0 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 0.0 );
  QgsGeometryUtils::pointOnLineWithDistance( 0, 0,  0, 10, 0, x, y );
  QCOMPARE( x, 0.0 );
  QCOMPARE( y, 0.0 );

  p = QgsGeometryUtils::pointOnLineWithDistance( QgsPoint( 0, 0 ),  QgsPoint( 0, 10 ), 10 );
  QCOMPARE( p.x(), 0.0 );
  QCOMPARE( p.y(), 10.0 );
  QgsGeometryUtils::pointOnLineWithDistance( 0, 0,  0, 10, 10, x, y );
  QCOMPARE( x, 0.0 );
  QCOMPARE( y, 10.0 );

  p = QgsGeometryUtils::pointOnLineWithDistance( QgsPoint( 2, 1 ),  QgsPoint( -8, -5 ), 5 );
  QGSCOMPARENEAR( p.x(), -2.28746, 0.0001 );
  QGSCOMPARENEAR( p.y(), -1.57248, 0.0001 );
  QgsGeometryUtils::pointOnLineWithDistance( 2, 1, -8, -5, 5, x, y );
  QGSCOMPARENEAR( x, -2.28746, 0.0001 );
  QGSCOMPARENEAR( y, -1.57248, 0.0001 );
  double z, z1, z2;
  double m, m1, m2;
  z1 = 12;
  z2 = 2;
  m1 = 11;
  m2 = 15;
  QgsGeometryUtils::pointOnLineWithDistance( 2, 1, -8, -5, 5, x, y, &z1, &z2, &z );
  QGSCOMPARENEAR( x, -2.28746, 0.0001 );
  QGSCOMPARENEAR( y, -1.57248, 0.0001 );
  QGSCOMPARENEAR( z, 7.712535, 0.0001 );
  QgsGeometryUtils::pointOnLineWithDistance( 2, 1, -8, -5, 5, x, y, nullptr, nullptr, nullptr, &m1, &m2, &m );
  QGSCOMPARENEAR( x, -2.28746, 0.0001 );
  QGSCOMPARENEAR( y, -1.57248, 0.0001 );
  QGSCOMPARENEAR( m, 12.714986, 0.0001 );
  z = 0;
  m = 0;
  QgsGeometryUtils::pointOnLineWithDistance( 2, 1, -8, -5, 5, x, y, &z1, &z2, &z, &m1, &m2, &m );
  QGSCOMPARENEAR( x, -2.28746, 0.0001 );
  QGSCOMPARENEAR( y, -1.57248, 0.0001 );
  QGSCOMPARENEAR( z, 7.712535, 0.0001 );
  QGSCOMPARENEAR( m, 12.714986, 0.0001 );

  p = QgsGeometryUtils::pointOnLineWithDistance( QgsPoint( 0, 0 ),  QgsPoint( -10, -6 ), 2 );
  QGSCOMPARENEAR( p.x(), -1.71499, 0.0001 );
  QGSCOMPARENEAR( p.y(), -1.02899, 0.0001 );
  QgsGeometryUtils::pointOnLineWithDistance( 0, 0, -10, -6, 2, x, y );
  QGSCOMPARENEAR( x, -1.71499, 0.0001 );
  QGSCOMPARENEAR( y, -1.02899, 0.0001 );

  p = QgsGeometryUtils::pointOnLineWithDistance( QgsPoint( 0, 0 ),  QgsPoint( -10, -6 ), 20 );
  QGSCOMPARENEAR( p.x(), -17.1499, 0.0001 );
  QGSCOMPARENEAR( p.y(), -10.2899, 0.0001 );
  QgsGeometryUtils::pointOnLineWithDistance( 0, 0, -10, -6, 20, x, y );
  QGSCOMPARENEAR( x, -17.1499, 0.0001 );
  QGSCOMPARENEAR( y, -10.2899, 0.0001 );

  p = QgsGeometryUtils::pointOnLineWithDistance( QgsPoint( 0, 0 ),  QgsPoint( -10, -6 ), -10 );
  QGSCOMPARENEAR( p.x(), 8.57493, 0.0001 );
  QGSCOMPARENEAR( p.y(), 5.14496, 0.0001 );
  QgsGeometryUtils::pointOnLineWithDistance( 0, 0, -10, -6, -10, x, y );
  QGSCOMPARENEAR( x, 8.57493, 0.0001 );
  QGSCOMPARENEAR( y, 5.14496, 0.0001 );

}

void TestQgsGeometryUtils::interpolatePointOnArc()
{
  QgsPoint p;
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0, 1, 2 ), QgsPoint( 11, 1, 3, 4 ), QgsPoint( 12, 0, 13, 14 ), 0 );
  QGSCOMPARENEAR( p.x(), 10.0, 0.00001 );
  QGSCOMPARENEAR( p.y(), 0.0, 0.00001 );
  QGSCOMPARENEAR( p.z(), 1.0, 0.00001 );
  QGSCOMPARENEAR( p.m(), 2.0, 0.00001 );
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0, 1, 2 ), QgsPoint( 11, 1, 3, 4 ), QgsPoint( 12, 0, 13, 14 ), 1 );
  QGSCOMPARENEAR( p.x(), 10.459698, 0.00001 );
  QGSCOMPARENEAR( p.y(),  0.841471, 0.00001 );
  QGSCOMPARENEAR( p.z(), 2.273240, 0.00001 );
  QGSCOMPARENEAR( p.m(), 3.273240, 0.00001 );
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0, 1, 2 ), QgsPoint( 11, 1, 3, 4 ), QgsPoint( 12, 0, 13, 14 ), 2 );
  QGSCOMPARENEAR( p.x(), 11.416147, 0.00001 );
  QGSCOMPARENEAR( p.y(), 0.909297, 0.00001 );
  QGSCOMPARENEAR( p.z(), 5.732395, 0.00001 );
  QGSCOMPARENEAR( p.m(), 6.732395, 0.00001 );
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0 ), QgsPoint( 11, 1 ), QgsPoint( 12, 0 ), 3.141592 );
  QGSCOMPARENEAR( p.x(), 12.0, 0.00001 );
  QGSCOMPARENEAR( p.y(), 0.0, 0.00001 );
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0 ), QgsPoint( 11, 1 ), QgsPoint( 12, 0 ), 3.2 );
  QGSCOMPARENEAR( p.x(), 11.998295, 0.00001 );
  QGSCOMPARENEAR( p.y(), -0.058374, 0.00001 );
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0 ), QgsPoint( 11, 1 ), QgsPoint( 12, 0 ), 5 );
  QGSCOMPARENEAR( p.x(), 10.716338, 0.00001 );
  QGSCOMPARENEAR( p.y(), -0.958924, 0.00001 );
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0, 1, 2 ), QgsPoint( 11, 1, 3, 4 ), QgsPoint( 12, 0, 13, 14 ), 3.141592 * 2 );
  QGSCOMPARENEAR( p.x(), 10, 0.00001 );
  QGSCOMPARENEAR( p.y(), 0, 0.00001 );
  QGSCOMPARENEAR( p.z(), 32.99999, 0.00001 );
  QGSCOMPARENEAR( p.m(), 33.999992, 0.00001 );
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0 ), QgsPoint( 8, 2 ), QgsPoint( 6, 0 ), 0 );
  QGSCOMPARENEAR( p.x(), 10.0, 0.00001 );
  QGSCOMPARENEAR( p.y(), 0.0, 0.00001 );
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0 ), QgsPoint( 8, 2 ), QgsPoint( 6, 0 ), 1 );
  QGSCOMPARENEAR( p.x(), 9.755165, 0.00001 );
  QGSCOMPARENEAR( p.y(), 0.958851, 0.00001 );
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0 ), QgsPoint( 8, 2 ), QgsPoint( 6, 0 ), 3.141592 );
  QGSCOMPARENEAR( p.x(), 8.0, 0.00001 );
  QGSCOMPARENEAR( p.y(), 2.0, 0.00001 );
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0 ), QgsPoint( 8, 2 ), QgsPoint( 6, 0 ), 3.141592 * 2 );
  QGSCOMPARENEAR( p.x(), 6.0, 0.00001 );
  QGSCOMPARENEAR( p.y(), 0.0, 0.00001 );
  p = QgsGeometryUtils::interpolatePointOnArc( QgsPoint( 10, 0 ), QgsPoint( 8, 2 ), QgsPoint( 6, 0 ), 3.141592 * 3 );
  QGSCOMPARENEAR( p.x(), 8.0, 0.00001 );
  QGSCOMPARENEAR( p.y(), -2.0, 0.00001 );
}

QGSTEST_MAIN( TestQgsGeometryUtils )
#include "testqgsgeometryutils.moc"
