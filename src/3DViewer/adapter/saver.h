/**
 * @file saver.h
 * @brief Определение класса Saver — утилиты для сохранения изображений и записи
 * GIF-анимаций из QML-сцены.
 *
 * Класс `Saver` предоставляет методы для сохранения текущего состояния
 * визуального элемента QML в виде .bmp и .jpeg файлов, а также для записи
 * последовательности кадров в GIF-анимацию с помощью утилиты ImageMagick.
 *
 * Все сохранённые данные размещаются в папке `screencasts`, создаваемой при
 * необходимости.
 *
 * Требования:
 * - ImageMagick должен быть установлен в системе (доступ к утилите `convert`).
 */

#ifndef SAVER_H
#define SAVER_H

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QQuickItem>
#include <QQuickItemGrabResult>
#include <QTimer>

namespace s21 {

/**
 * @class Saver
 * @brief Класс для сохранения изображений и записи GIF-анимаций из QML-сцены.
 *
 * Класс `Saver` предоставляет интерфейс для захвата изображений с QML-объектов
 * (`QQuickItem`) и их сохранения с помощью внешней утилиты ImageMagick.
 *
 * @note Для работы функции записи GIF необходимо, чтобы утилита `convert`
 *       из ImageMagick была установлена и доступна через системный `PATH`.
 */
class Saver : public QObject {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор класса Saver.
   *
   * Инициализирует объект и подготавливает внутренний таймер, используемый для
   * записи кадров GIF. Принимает опционального родителя QObject.
   *
   * @param parent Родительский объект, передаваемый в базовый класс QObject.
   */
  explicit Saver(QObject* parent = nullptr);

  /**
   * @brief Сохраняет текущее изображение из элемента QML в виде PNG-файла.
   *
   * Создает директорию `screencasts`, если она отсутствует. Делает захват
   * изображения с указанного QQuickItem, перекрашивает фон в белый, и сохраняет
   * результат в указанный путь.
   *
   * @param item Указатель на QML-объект (обычно `QQuickItem*`), из которого
   * будет захвачено изображение.
   * @param path Относительный путь и имя файла (в папке `screencasts`), в
   * который нужно сохранить изображение.
   */
  Q_INVOKABLE void saveRenderImage(QObject* item, const QString& path);

  /**
   * @brief Запускает запись GIF-анимации с указанного элемента.
   *
   * Сохраняет 50 кадров (по 10 кадров в секунду, в течение 5 секунд) из объекта
   * QML (обычно `QQuickItem`). Кадры сохраняются в директорию `screencasts`,
   * после чего вызывается метод finishGifRecording().
   *
   * @param item Указатель на QML-объект, с которого будет происходить захват
   * кадров.
   */
  Q_INVOKABLE void startGifRecording(QObject* item);

 private slots:
  /**
   * @brief Захватывает один кадр для анимации GIF.
   *
   * Выполняет захват изображения с `gifItem_`, масштабирует его до 640x480,
   * центрирует на белом фоне, и сохраняет как PNG-файл `frame_XXX.png` в
   * директории `screencasts`. При достижении общего количества кадров
   * (`totalFrames_`) останавливает таймер и завершает запись GIF.
   */
  void recordGifFrame();

  /**
   * @brief Завершает запись GIF-анимации и сохраняет её с помощью ImageMagick.
   *
   * Объединяет PNG-кадры, сохранённые ранее в директории `screencasts`, в один
   * GIF-файл с помощью команды `convert`. Также удаляет временные PNG-файлы
   * после создания GIF.
   *
   * @warning Требуется установленный ImageMagick (`convert` должен быть
   * доступен в PATH).
   */
  void finishGifRecording();

 private:
  QTimer* gifTimer_ = nullptr;
  QObject* gifItem_ = nullptr;
  int frameCounter_ = 0;
  const int totalFrames_ = 50;
};  // class Saver

}  // namespace s21

#endif  // SAVER_H
