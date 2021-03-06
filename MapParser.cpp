#include "MapParser.h"
MapParser::MapParser(QString &filename) : _left(INT_MAX), _right(INT_MIN), _top(INT_MAX), _bottom(INT_MIN) {
  QFile polygonFile;
  QJsonParseError jsonError;
  polygonFile.setFileName(filename);
  polygonFile.open(QIODevice::ReadOnly | QIODevice::Text);
  jsonObject = QJsonDocument::fromJson(polygonFile.readAll(), &jsonError).object();
  polygonFile.close();
  _polygons = getPoints();
}

QVector<QPolygon> MapParser::getPoints() {
  if (!_polygons.empty()) return _polygons;
  QJsonArray state_arr = jsonObject.value("fields").toObject().value("geo_shape").toObject()["coordinates"].toArray();
  if (jsonObject.value("fields").toObject().value("geo_shape").toObject()["type"].toString() == "Polygon") {
    QPolygon polygon;
        foreach (const QJsonValue &coord, state_arr[0].toArray()) {
        QJsonArray parsed_point = coord.toArray();
        int x, y;
        double xx, yy;
        xx = parsed_point[0].toDouble();
        yy = parsed_point[1].toDouble();
        PointProcessor::get_point(x, y, xx, yy);
        _left = min(_left, xx); _right = max(_right, xx);
        _top = min(_top, yy); _bottom = max(_bottom, yy);
        polygon << QPoint(x, y);
      }
    _polygons.push_back(polygon);
  } else {
        foreach (const QJsonValue &poly, state_arr) {
        QPolygon polygon;
            foreach (const QJsonValue &coord, poly.toArray()[0].toArray()) {
            QJsonArray parsed_point = coord.toArray();
            int x, y;
            double xx, yy;
            xx = parsed_point[0].toDouble();
            yy = parsed_point[1].toDouble();
            PointProcessor::get_point(x, y, xx, yy);
            _left = min(_left, xx); _right = max(_right, xx);
            _top = min(_top, yy); _bottom = max(_bottom, yy);
            polygon << QPoint(x, y);
          }
        _polygons.push_back(polygon);
      }
  }
  return _polygons;
}

double MapParser::left() { return _left; }
double MapParser::right() { return _right; }
double MapParser::top() { return _top; }
double MapParser::bottom() { return _bottom; }

double MapParser::min(double a, double b) { return (a < b) ? a : b; }
double MapParser::max(double a, double b) { return (a > b) ? a : b; }
