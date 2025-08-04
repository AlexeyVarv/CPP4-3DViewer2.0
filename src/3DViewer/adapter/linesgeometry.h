/**
 * @file linesgeometry.h
 * @brief Класс LinesGeometry — реализация отображения модели в виде линий.
 *
 * Наследуется от GeometryPrototype и используется для генерации линейной
 * (реберной) геометрии модели, отображаемой в QML через QQuick3DGeometry.
 */

#ifndef LINESGEOMETRY_H
#define LINESGEOMETRY_H

#include <QByteArray>
#include <QQuick3DGeometry>
#include <QVector3D>

#include "../core/model3d.h"
#include "geometryadditions.h"
#include "geometryprototype.h"

namespace s21 {

/**
 * @class LinesGeometry
 * @brief Генератор линейной геометрии (режим wireframe) для 3D-моделей.
 *
 * Этот класс преобразует полигоны модели в рёбра и настраивает буферы вершин
 * и индексов для отображения в QQuick3D.
 */
class LinesGeometry : public GeometryPrototype {
  Q_OBJECT
 public:
  /**
   * @brief Конструктор класса LinesGeometry.
   * @param parent Родительский объект QQuick3DObject.
   */
  explicit LinesGeometry(QQuick3DObject *parent = nullptr);

  /**
   * @brief Обновляет геометрию модели (вершины и индексы).
   * @param model Модель, которую нужно отобразить.
   */
  void updateGeometry(const Model3D &model);

  /**
   * @brief Обновляет только вершины модели (без пересоздания индексов).
   * @param model Модель, вершины которой нужно обновить.
   */
  void updateVertices(const Model3D &model);

  /**
   * @brief Возвращает количество вершин.
   */
  int vertexCount() const { return m_vertexCount; }

  /**
   * @brief Возвращает количество полигонов (ребер).
   */
  int polygonCount() const { return m_polygonCount; }

 private:
  /**
   * @brief Заполняет массив данных вершин.
   */
  void populateVertexData() override;

  /**
   * @brief Заполняет массив индексов линий.
   */
  void populateIndexData() override;

  /**
   * @brief Указатель на модель, используемую для генерации геометрии.
   */
  const Model3D *m_model = nullptr;  // Указатель на модель
};                                   // class LinesGeometry

}  // namespace s21

#endif  // LINESGEOMETRY_H