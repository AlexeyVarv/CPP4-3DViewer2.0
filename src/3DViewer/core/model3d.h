/**
 * @file model3d.h
 * @brief Классы для хранения и обработки 3D-модели: вершины, грани (полигоны),
 * модель.
 */

#ifndef MODEL_3D_H
#define MODEL_3D_H

#include <QMetaType>
#include <QtQml>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace s21 {

/**
 * @brief Класс, представляющий вершину в 3D пространстве.
 */
class Vertex {
 public:
  float x, y, z;

  /**
   * @brief Конструктор по умолчанию и с параметрами.
   */
  Vertex(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

  /**
   * @brief Конструктор копирования.
   */
  Vertex(const Vertex& other) : x(other.x), y(other.y), z(other.z) {}

  /**
   * @brief Оператор присваивания.
   */
  Vertex& operator=(const Vertex& other) {
    if (this != &other) {  // Защита от самоприсваивания
      x = other.x;
      y = other.y;
      z = other.z;
    }
    return *this;
  }

  /**
   * @brief Вычитание двух вершин.
   */
  Vertex operator-(const Vertex& other) const {
    return Vertex(x - other.x, y - other.y, z - other.z);
  }

  /**
   * @brief Сложение двух вершин.
   */
  Vertex operator+(const Vertex& other) const {
    return Vertex(x + other.x, y + other.y, z + other.z);
  }

  /**
   * @brief Сложение с присваиванием.
   */
  Vertex& operator+=(const Vertex& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;  // Возвращаем ссылку на текущий объект
  }

  /**
   * @brief Вычитание с присваиванием.
   */
  Vertex& operator-=(const Vertex& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;  // Возвращаем ссылку на текущий объект
  }

  /**
   * @brief Умножение на скаляр с присваиванием.
   */
  Vertex& operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;  // Возвращаем ссылку на текущий объект
  }

  /**
   * @brief Деление с присваиванием.
   */
  Vertex& operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;  // Возвращаем ссылку на текущий объект
  }

  /**
   * @brief Печать координат вершины в консоль.
   */
  void printV() const {
    std::cout << "Vertex(" << x << ", " << y << ", " << z << ")\n";
  }
};  // class Vertex

/**
 * @brief Класс, представляющий грань (полигон) модели.
 */
class Polygon {
 public:
  std::vector<int> vertexIndices;

  /**
   * @brief Конструктор с инициализацией индексов вершин.
   */
  Polygon(const std::vector<int>& indices) : vertexIndices(indices) {}

  /**
   * @brief Печать полигона.
   */
  void printP() const {
    std::cout << "Polygon: ";
    for (int index : vertexIndices) {
      std::cout << index << " ";
    }
    std::cout << "\n";
  }

  /**
   * @brief Возвращает количество вершин в полигоне.
   */
  size_t size() const { return vertexIndices.size(); }

  /**
   * @brief Проверка на пустоту.
   */
  bool empty() const { return vertexIndices.empty(); }

  /**
   * @brief Итератор для перебора индексов вершин.
   */
  std::vector<int>::iterator begin() { return vertexIndices.begin(); }

  /**
   * @brief Итератор для перебора индексов вершин.
   */
  std::vector<int>::iterator end() { return vertexIndices.end(); }

  /**
   * @brief Итератор для перебора индексов вершин.
   */
  std::vector<int>::const_iterator begin() const {
    return vertexIndices.cbegin();
  }

  /**
   * @brief Итератор для перебора индексов вершин.
   */
  std::vector<int>::const_iterator end() const { return vertexIndices.cend(); }

  /**
   * @brief Доступ к индексу вершины по индексу.
   * @throw std::out_of_range если индекс вне диапазона.
   */
  int& operator[](size_t index) {
    if (index >= vertexIndices.size()) {
      throw std::out_of_range("Index out of range in Polygon");
    }
    return vertexIndices[index];
  }

  /**
   * @brief Доступ к индексу вершины по индексу. Константная версия.
   * @throw std::out_of_range если индекс вне диапазона.
   */
  const int& operator[](size_t index) const {
    if (index >= vertexIndices.size()) {
      throw std::out_of_range("Index out of range in Polygon");
    }
    return vertexIndices[index];
  }
};  // class Polygon

/**
 * @brief Базовый класс для хранения и обработки 3D-модели.
 */
class Model3D {
  Q_GADGET
 public:
  std::vector<Vertex> vertices;   ///< Все вершины модели
  std::vector<Polygon> polygons;  ///< Все полигоны модели
  std::vector<Vertex>
      currentPosition;  ///< Текущая позиция вершин (учитывая трансформации)
  Vertex previousShift;  ///< Предыдущее смещение модели

 public:
  /**
   * @brief Конструктор по умолччанию.
   */
  Model3D() = default;

  /**
   * @brief Деструктор.
   */
  ~Model3D() = default;

 public:
  /**
   * @brief Центрирует модель по центру координат.
   */
  void centerModel() {
    if (vertices.empty()) return;

    Vertex center = calculateCenter();

    // Сдвигаем все вершины к центру координат
    for (auto& vertex : vertices) {
      vertex -= center;
    }

    // Обновляем оригинальные вершины
    currentPosition = vertices;
  }

  /**
   * @brief Масштабирует модель в заданный размер и центрирует.
   */
  void normalizeModel() {
    if (!(vertices.empty())) {
      // 1. Находим границы модели
      float minX = std::numeric_limits<float>::max();
      float minY = std::numeric_limits<float>::max();
      float minZ = std::numeric_limits<float>::max();
      float maxX = std::numeric_limits<float>::lowest();
      float maxY = std::numeric_limits<float>::lowest();
      float maxZ = std::numeric_limits<float>::lowest();

      for (const auto& vertex : vertices) {
        minX = std::min(minX, vertex.x);
        minY = std::min(minY, vertex.y);
        minZ = std::min(minZ, vertex.z);
        maxX = std::max(maxX, vertex.x);
        maxY = std::max(maxY, vertex.y);
        maxZ = std::max(maxZ, vertex.z);
      }

      // 2. Вычисляем размеры модели
      float sizeX = maxX - minX;
      float sizeY = maxY - minY;
      float sizeZ = maxZ - minZ;
      float maxSize = std::max({sizeX, sizeY, sizeZ});

      // 3. Вычисляем центр
      Vertex center;
      center.x = (minX + maxX) / 2.0f;
      center.y = (minY + maxY) / 2.0f;
      center.z = (minZ + maxZ) / 2.0f;

      // 4. Масштабируем и центрируем
      float targetSize = 12.0f;  // Желаемый размер (можно регулировать)
      float scaleFactor = targetSize / maxSize;

      for (auto& vertex : vertices) {
        // Центрируем
        vertex -= center;

        // Масштабируем
        vertex *= scaleFactor;
      }

      currentPosition =
          vertices;  // установка начальной позиции в центра координат
      previousShift = {0, 0, 0};
    }
  }

  /**
   * @brief Добавить вершину в модель.
   */
  void addVertex(const Vertex& vertex) { vertices.push_back(vertex); }

  /**
   * @brief Получить все вершины модели.
   */
  const std::vector<Vertex>& getVertices() const { return vertices; }

  /**
   * @brief Добавить полигон в модель.
   * @throw std::out_of_range если индекс вершины недопустим.
   */
  void addPolygon(const Polygon& polygon) {
    for (int index : polygon.vertexIndices) {
      if (index < 0 || index >= static_cast<int>(vertices.size())) {
        throw std::out_of_range("Invalid vertex index in polygon");
      }
    }
    polygons.push_back(polygon);
  }

  /**
   * @brief Получить все полигоны модели.
   */
  const std::vector<Polygon>& getPolygons() const { return polygons; }

  /**
   * @brief Получить вершину по индексу.
   * @throw std::out_of_range если индекс недопустим.
   */
  const Vertex& getVetrex(int index) const {
    if (index < 0 || index >= static_cast<int>(vertices.size())) {
      throw std::out_of_range("Invalid vertex index");
    }
    return vertices[index];
  }

  /**
   * @brief Получить полигон по индексу.
   * @throw std::out_of_range если индекс недопустим.
   */
  const Polygon& getPolygon(int index) const {
    if (index < 0 || index >= static_cast<int>(polygons.size())) {
      throw std::out_of_range("Invalid polygon index");
    }
    return polygons[index];
  }

  /**
   * @brief Вычисляет центр модели.
   */
  Vertex calculateCenter() const {
    Vertex center = {0, 0, 0};
    for (const auto& vertex : vertices) {
      center += vertex;
    }

    size_t count = vertices.size();
    center /= count;

    return center;
  }

  /**
   * @brief Поворачивает модель на заданные углы по осям.
   * @param angleX Угол поворота по X
   * @param angleY Угол поворота по Y
   * @param angleZ Угол поворота по Z
   */
  void rotateModel(float angleX, float angleY, float angleZ) {
    // Вычисляем центр модели
    vertices = currentPosition;  // Начальная позиция поворота равна точка
                                 // перемещения модели
    Vertex center = calculateCenter();

    // Преобразование углов из градусов в радианы
    float radX = angleX * M_PI / 180.0f;
    float radY = angleY * M_PI / 180.0f;
    float radZ = angleZ * M_PI / 180.0f;

    // Вычисление значений sin и cos для каждого угла
    float cosX = std::cos(radX), sinX = std::sin(radX);
    float cosY = std::cos(radY), sinY = std::sin(radY);
    float cosZ = std::cos(radZ), sinZ = std::sin(radZ);

    // Перемещаем модель так, чтобы её центр совпал с началом координат
    for (auto& vertex : vertices) {
      vertex -= center;
    }

    // Применяем поворот к каждой вершине
    for (auto& vertex : vertices) {
      float x = vertex.x, y = vertex.y, z = vertex.z;

      // Поворот вокруг оси X
      float newY = y * cosX - z * sinX;
      float newZ = y * sinX + z * cosX;
      y = newY;
      z = newZ;

      // Поворот вокруг оси Y
      float newX = x * cosY + z * sinY;
      newZ = -x * sinY + z * cosY;
      x = newX;
      z = newZ;

      // Поворот вокруг оси Z
      newX = x * cosZ - y * sinZ;
      newY = x * sinZ + y * cosZ;
      x = newX;
      y = newY;

      // Обновляем координаты вершины
      vertex.x = x;
      vertex.y = y;
      vertex.z = z;
    }

    // Возвращаем модель в исходное положение
    for (auto& vertex : vertices) {
      vertex += center;
    }
  }

  /**
   * @brief Смещает модель на заданное значение.
   * @param shiftX Смещение по X
   * @param shiftY Смещение по Y
   * @param shiftZ Смещение по Z
   */
  void shiftModel(float shiftX, float shiftY, float shiftZ) {
    Vertex tempShift = {shiftX, shiftY, shiftZ};
    Vertex currentShift = tempShift - previousShift;
    //  Вычисляем центр модели
    for (auto& vertex : vertices) {
      vertex += currentShift;
    }
    previousShift = tempShift;
    currentPosition =
        vertices;  // устанавливаем точку для указаная положения модели
  }

  /**
   * @brief Очищает модель (удаляет все вершины и грани).
   */
  void clear() {
    vertices.clear();
    currentPosition.clear();
    polygons.clear();
  }

  /**
   * @brief Выводит информацию о модели в консоль.
   */
  void printModelInfo() const {
    std::cout << "Model contains " << vertices.size() << " vertices and "
              << polygons.size() << " polygons.\n";
    for (auto polygon : polygons) {
      polygon.printP();
    }
  }
};  // class Model3D

}  // namespace s21

Q_DECLARE_METATYPE(s21::Model3D*)

#endif