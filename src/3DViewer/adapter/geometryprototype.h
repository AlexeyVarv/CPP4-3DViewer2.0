/**
 * @file geometryprototype.h
 * @brief Базовый абстрактный класс геометрии для QML-рендеринга 3D-моделей.
 */

#ifndef GEOMETRYPROTOTYPE_H
#define GEOMETRYPROTOTYPE_H

#include <QByteArray>
#include <QQuick3DGeometry>
#include <QVector3D>

namespace s21 {

/**
 * @class GeometryPrototype
 * @brief Абстрактный базовый класс для генерации геометрии в QML через
 * QQuick3DGeometry.
 *
 * Наследуется от QQuick3DGeometry. Используется для создания пользовательской
 * геометрии в 3D-сцене с возможностью переопределения данных вершин и индексов.
 */
class GeometryPrototype : public QQuick3DGeometry {
  Q_OBJECT
 public:
  /**
   * @brief Конструктор базовой геометрии.
   * @param parent Родительский QQuick3DObject.
   */
  explicit GeometryPrototype(QQuick3DObject *parent = nullptr);

  /**
   * @brief Полная настройка геометрии: вершины, индексы, атрибуты.
   */
  void setupGeometry();

  /**
   * @brief Настройка только вершинной геометрии (например, для отображения
   * точек).
   */
  void setupVertices();

 protected:
  QByteArray m_vertexData;
  QByteArray m_indexData;
  QQuick3DGeometry::PrimitiveType m_primitive;

  int m_vertexCount = 0;
  int m_polygonCount = 0;

  /**
   * @brief Метод для заполнения массива данных вершин.
   */
  virtual void populateVertexData() = 0;

  /**
   * @brief Метод для заполнения массива индексов.
   */
  virtual void populateIndexData() = 0;
};  // class GeometryPrototype

}  // namespace s21

#endif