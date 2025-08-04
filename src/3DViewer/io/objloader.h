/**
 * @file objloader.h
 * @brief Класс для загрузки 3D-моделей из .obj-файлов.
 */

#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <fstream>
#include <sstream>
#include <string>

#include "../core/model3d.h"

namespace s21 {

/**
 * @brief Класс, реализующий загрузку моделей из .obj файлов.
 */
class ObjParser {
 public:
  /**
   * @brief Загружает модель из OBJ-файла.
   * @param filename Имя файла
   * @param model Ссылка на объект модели для загрузки
   * @return true если загрузка успешна, false — в случае ошибки
   */
  static bool loadObj(const std::string& filename, Model3D& model) {
    //std::cout << "Start parsing" << std::endl;
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Error: Cannot open file " << filename << "\n";
      return false;
    }

    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      std::string type;
      iss >> type;

      if (type == "v") {  // Вершина
        float x, y, z;
        iss >> x >> y >> z;
        model.addVertex(Vertex(x, y, z));
      } else if (type == "f") {  // Полигон
        std::vector<int> vertexIndices;
        std::string indexStr;
        while (iss >> indexStr) {
          // Разбиваем строку по символу '/'
          size_t pos = indexStr.find('/');
          int vertexIndex =
              std::stoi(indexStr.substr(0, pos));  // Берем только первую часть

          // Преобразуем индексацию с 1 в индексацию с 0
          if (vertexIndex < 0) {
            vertexIndex += model.vertices.size() + 1;  // Отрицательные индексы
          }
          vertexIndices.push_back(vertexIndex - 1);
        }
        model.addPolygon(Polygon(vertexIndices));
      }
      // Игнорируем другие команды (например, vn, vt)
    }

    file.close();
    return true;
  }
};  // class ObjParser

}  // namespace s21

#endif  // OBJ_LOADER_H
