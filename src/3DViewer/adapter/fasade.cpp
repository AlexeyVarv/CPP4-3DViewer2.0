#include "fasade.h"

using namespace s21;

Facade::Facade(QObject* parent) : QObject(parent) {}

void Facade::loadModel(const QString& filePath) {
  loader.loadModel(model, filePath);
}

void Facade::rotateModel(float angleX, float angleY, float angleZ) {
  model.rotateModel(angleX, angleY, angleZ);
  emit geometryUpdated();
}

void Facade::shiftModel(float angleX, float angleY, float angleZ) {
  model.shiftModel(angleX, angleY, angleZ);
  emit geometryUpdated();
}

LinesGeometry* Facade::createLinesView() {
  auto geometry = new LinesGeometry();
  geometry->updateGeometry(model);
  geometry->updateVertices(model);
  geometry->setPrimitiveType(QQuick3DGeometry::PrimitiveType::Lines);
  currentgeometry = geometry;
  emit vertexCountChanged();
  emit polygonCountChanged();
  return geometry;
}

LinesGeometry* Facade::createVerticesView() {
  auto geometry = new LinesGeometry();
  geometry->updateVertices(model);
  geometry->setPrimitiveType(QQuick3DGeometry::PrimitiveType::Points);
  currentgeometry = geometry;
  emit vertexCountChanged();
  return geometry;
}

int Facade::vertexCount() const {
  if (currentgeometry)
    return currentgeometry->vertexCount();
  else
    return 0;
}

int Facade::polygonCount() const {
  if (currentgeometry)
    return currentgeometry->polygonCount();
  else
    return 0;
}

void Facade::saveRenderImage(QObject* item, const QString& path) {
  saver.saveRenderImage(item, path);
}

void Facade::startGifRecording(QObject* item) { saver.startGifRecording(item); }