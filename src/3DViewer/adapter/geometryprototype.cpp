#include "geometryprototype.h"

using namespace s21;

GeometryPrototype::GeometryPrototype(QQuick3DObject *parent)
    : QQuick3DGeometry(parent) {}

void GeometryPrototype::setupGeometry() {
  m_indexData.clear();  // Очищаем предыдущую геометрию

  // Установка данных индексов
  populateIndexData();
  setIndexData(m_indexData);

  // Добавление атрибута индексов
  addAttribute(
      QQuick3DGeometry::Attribute::IndexSemantic,
      0,                                      // Offset
      QQuick3DGeometry::Attribute::U32Type);  // Тип данных для индексов

  // Установка границ модели
  setBounds(QVector3D(-1, -1, -1), QVector3D(1, 1, 1));

  update();  // Обновление геометрии
}

void GeometryPrototype::setupVertices() {
  m_vertexData.clear();

  // Создаем массив вершин
  addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
               0,                                      // Offset
               QQuick3DGeometry::Attribute::F32Type);  // Тип данных
  addAttribute(QQuick3DGeometry::Attribute::TexCoord0Semantic,
               3 * sizeof(float), QQuick3DGeometry::Attribute::F32Type);  // UV
  // Установка шага для атрибута позиций вершин
  setStride(sizeof(float) * 5);  // Шаг равен размеру одной вершины (3 float)

  // Установка данных вершин
  populateVertexData();
  setVertexData(m_vertexData);

  //  Установка границ модели
  setBounds(QVector3D(-1, -1, -1), QVector3D(1, 1, 1));

  update();
}