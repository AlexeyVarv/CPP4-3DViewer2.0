/**
 * @file modelloader.h
 * @brief Класс ModelLoader — загрузчик моделей формата .obj.
 */

#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "../io/objloader.h"
#include "model3d.h"

namespace s21 {

/**
 * @class ModelLoader
 * @brief Загружает 3D-модель из .obj-файла и нормализует её.
 *
 * Использует вспомогательный класс ObjParser для разбора содержимого
 * .obj-файла.
 */
class ModelLoader : public QObject {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор по умолчанию.
   * @param parent Родительский QObject.
   */
  explicit ModelLoader(QObject *parent = nullptr) : QObject(parent) {}

  /**
   * @brief Загружает модель из .obj-файла по указанному пути.
   * @param model Ссылка на объект Model3D, в который будет загружена модель.
   * @param filePath Путь к файлу (включая file://...).
   */
  Q_INVOKABLE void loadModel(Model3D &model, const QString &filePath) {
    //std::cout << "Start load" << std::endl;
    QString localPath = convertToLocalPath(filePath);  // Преобразуем путь
    if (!localPath.isEmpty()) {
      model.clear();
      ObjParser::loadObj(localPath.toStdString(), model);
      model.normalizeModel();  // Вместо centerModel()
    } else {
      qWarning() << "Failed to convert file path:" << filePath;
    }
  }

 private:
  /**
   * @brief Преобразует путь file:// в локальный путь.
   * @param filePath Путь к файлу в формате URL.
   * @return Строка локального пути.
   */
  QString convertToLocalPath(const QString &filePath) {
    QUrl url(filePath);  // Создаем объект QUrl из строки пути
    if (url.isLocalFile()) {
      return url.toLocalFile();  // Преобразуем в локальный путь
    }
    qWarning() << "Invalid file path format:" << filePath;
    return QString();  // Возвращаем пустую строку, если путь некорректен
  }
};  // class ModelLoader

}  // namespace s21

#endif