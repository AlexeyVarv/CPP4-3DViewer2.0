#include "linesgeometry.h"

using namespace s21;

LinesGeometry::LinesGeometry(QQuick3DObject* parent)
    : GeometryPrototype(parent) {
  m_primitive = QQuick3DGeometry::PrimitiveType::Lines;
}

void LinesGeometry::updateGeometry(const Model3D& model) {
  m_model = &model;  // Сохраняем ссылку на модель
  setupGeometry();   // Вызываем настройку геометрии
}

void LinesGeometry::updateVertices(const Model3D& model) {
  m_model = &model;  // Сохраняем ссылку на модель
  setupVertices();   // Вызываем настройку вершин
}

void LinesGeometry::populateVertexData() {
  if (!m_model) return;

  // Создание массива вершин
  m_vertexData.resize(m_model->vertices.size() * 5 * sizeof(float));
  float* vertexPtr = reinterpret_cast<float*>(m_vertexData.data());
  for (const auto& vertex : m_model->vertices) {
    *vertexPtr++ = vertex.x;
    *vertexPtr++ = vertex.y;
    *vertexPtr++ = vertex.z;
    *vertexPtr++ = vertex.x;
    *vertexPtr++ = vertex.y;
    m_vertexCount++;
  }
}

void LinesGeometry::populateIndexData() {
  if (!m_model) return;

  // Преобразование полигонов в треугольники
  std::vector<int> indices = convertToLines(m_model->polygons);
  m_polygonCount = indices.size() / 3;

  // Создание массива индексов
  m_indexData.resize(indices.size() * sizeof(int));
  memcpy(m_indexData.data(), indices.data(), m_indexData.size());
}