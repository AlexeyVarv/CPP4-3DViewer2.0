#include "geometryadditions.h"

std::vector<int> convertToTriangles(const std::vector<Polygon>& polygons) {
  std::vector<int> triangleIndices;

  for (const auto& polygon : polygons) {
    if (polygon.size() == 3) {
      // Треугольник: просто добавляем индексы
      triangleIndices.insert(triangleIndices.end(), polygon.begin(),
                             polygon.end());
    } else if (polygon.size() == 4) {
      // Четырехугольник: разбиваем на два треугольника
      int v0 = polygon[0];
      int v1 = polygon[1];
      int v2 = polygon[2];
      int v3 = polygon[3];

      triangleIndices.push_back(v0);
      triangleIndices.push_back(v1);
      triangleIndices.push_back(v2);

      triangleIndices.push_back(v0);
      triangleIndices.push_back(v2);
      triangleIndices.push_back(v3);
    } else {
      // Для полигонов с более чем 4 вершинами можно использовать триангуляцию
      // (например, алгоритм "ear clipping").
      throw std::runtime_error("Unsupported polygon type");
    }
  }

  return triangleIndices;
}

std::vector<int> convertTrianglesToLines(
    const std::vector<int>& triangleIndices) {
  std::vector<int> lineIndices;

  for (size_t i = 0; i < triangleIndices.size(); i += 3) {
    int v0 = triangleIndices[i];
    int v1 = triangleIndices[i + 1];
    int v2 = triangleIndices[i + 2];

    // Добавляем три ребра треугольника
    lineIndices.push_back(v0);
    lineIndices.push_back(v1);

    lineIndices.push_back(v1);
    lineIndices.push_back(v2);

    lineIndices.push_back(v2);
    lineIndices.push_back(v0);
  }

  return lineIndices;
}

std::vector<int> convertToLines(const std::vector<Polygon>& polygons) {
  std::vector<int> lineIndices;

  for (const auto& polygon : polygons) {
    size_t numVertices = polygon.size();

    if (numVertices < 2) {
      // Полигон с менее чем 2 вершинами не может быть преобразован в линии
      throw std::runtime_error(
          "Polygon must have at least 2 vertices to form lines.");
    }

    // Создаем линии между последовательными вершинами
    for (size_t i = 0; i < numVertices; ++i) {
      int v0 = polygon[i];
      int v1 = polygon[(i + 1) % numVertices];  // Замыкаем полигон (последняя
                                                // вершина соединяется с первой)

      lineIndices.push_back(v0);
      lineIndices.push_back(v1);
    }
  }

  return lineIndices;
}