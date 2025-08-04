/**
 * @file viewersettings.h
 * @brief Класс ViewerSettings — управление настройками визуализации.
 *
 * Обеспечивает хранение и загрузку настроек рендеринга: перспектива, цвета,
 * размеры линий и точек, и пр.
 */

#ifndef VIEWERSETTINGS_H
#define VIEWERSETTINGS_H

#include <QColor>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QSettings>

namespace s21 {
/**
 * @class ViewerSettings
 * @brief Хранит и управляет пользовательскими настройками визуализации.
 *
 * Настройки сериализуются в JSON-файл и автоматически загружаются при старте.
 * Используются для QML-связывания через Q_PROPERTY.
 */
class ViewerSettings : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool isPerspective READ isPerspective WRITE setIsPerspective NOTIFY
                 settingsChanged)
  Q_PROPERTY(bool showVertices READ showVertices WRITE setShowVertices NOTIFY
                 settingsChanged)
  Q_PROPERTY(bool isSolidLines READ isSolidLines WRITE setSolidLines NOTIFY
                 settingsChanged)
  Q_PROPERTY(
      QColor lineColor READ lineColor WRITE setLineColor NOTIFY settingsChanged)
  Q_PROPERTY(QColor vertexColor READ vertexColor WRITE setVertexColor NOTIFY
                 settingsChanged)
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE
                 setBackgroundColor NOTIFY settingsChanged)
  Q_PROPERTY(
      float pointSize READ pointSize WRITE setPointSize NOTIFY settingsChanged)
  Q_PROPERTY(
      float lineWidth READ lineWidth WRITE setLineWidth NOTIFY settingsChanged)

 public:
  /**
   * @brief Конструктор по умолчанию, автоматически загружает настройки.
   * @param parent Родительский QObject.
   */
  explicit ViewerSettings(QObject *parent = nullptr) : QObject(parent) {
    loadSettings();
  }

  /**
   * Определяет режим проекции:
   * true — перспектива, false — ортографический.
   */
  Q_INVOKABLE bool isPerspective() const { return m_isPerspective; }
  Q_INVOKABLE void setIsPerspective(bool perspective) {
    if (m_isPerspective != perspective) {
      m_isPerspective = perspective;
      saveSettings();
      emit settingsChanged();
    }
  }

  /**
   * Показывать вершины на модели.
   * true — отображать вершины;
   * false — не отображать.
   */
  Q_INVOKABLE bool showVertices() const { return m_showVertices; }
  Q_INVOKABLE void setShowVertices(bool show) {
    if (m_showVertices != show) {
      m_showVertices = show;
      saveSettings();
      emit settingsChanged();
    }
  }

  /**
   * Отображать линии как сплошные (true) или пунктирные (false).
   */
  Q_INVOKABLE bool isSolidLines() const { return m_solidLines; }
  Q_INVOKABLE void setSolidLines(bool linesType) {
    if (m_solidLines != linesType) {
      m_solidLines = linesType;
      saveSettings();
      emit settingsChanged();
    }
  }

  /**
   * Цвет линий.
   */
  Q_INVOKABLE QColor lineColor() const { return m_lineColor; }
  Q_INVOKABLE void setLineColor(const QColor &color) {
    if (m_lineColor != color) {
      m_lineColor = color;
      saveSettings();
      emit settingsChanged();
    }
  }

  /**
   * Цвет вершин.
   */
  Q_INVOKABLE QColor vertexColor() const { return m_vertexColor; }
  Q_INVOKABLE void setVertexColor(const QColor &color) {
    if (m_vertexColor != color) {
      m_vertexColor = color;
      saveSettings();
      emit settingsChanged();
    }
  }

  /**
   * Цвет фона сцены.
   */
  Q_INVOKABLE QColor backgroundColor() const { return m_backgroundColor; }
  Q_INVOKABLE void setBackgroundColor(const QColor &color) {
    if (m_backgroundColor != color) {
      m_backgroundColor = color;
      saveSettings();
      emit settingsChanged();
    }
  }

  /**
   * Размер вершин.
   */
  Q_INVOKABLE float pointSize() const { return m_pointSize; }
  Q_INVOKABLE void setPointSize(float size) {
    if (m_pointSize != size) {
      m_pointSize = size;
      saveSettings();
      emit settingsChanged();
    }
  }

  /**
   * Толщина линий.
   */
  Q_INVOKABLE float lineWidth() const { return m_lineWidth; }
  Q_INVOKABLE void setLineWidth(float width) {
    if (m_lineWidth != width) {
      m_lineWidth = width;
      saveSettings();
      emit settingsChanged();
    }
  }

 signals:
  /**
   * @brief Сигнал, отдающийся при изменении любой настройки.
   */
  void settingsChanged();

 private:
  bool m_isPerspective = true;
  bool m_showVertices = false;
  bool m_solidLines = true;
  QColor m_lineColor = "#42a5f5";
  QColor m_vertexColor = "#ff0000";
  QColor m_backgroundColor = "#ffffff";
  float m_pointSize = 10.0f;  // Размер вершин по умолчанию
  float m_lineWidth = 1.0f;  // Толщина линий по умолчанию

  void saveSettings() { saveSettingsToJson(); }

  void loadSettings() { loadSettingsFromJson(); }

  /**
   * @brief Возвращает путь к файлу настроек.
   */
  QString getSettingsFilePath() {
    return QCoreApplication::applicationDirPath() + "/settings.json";
  }

  /**
   * @brief Сохраняет текущие настройки в JSON-файл.
   */
  void saveSettingsToJson() {
    QJsonObject json;
    json["isPerspective"] = m_isPerspective;
    json["showVertices"] = m_showVertices;
    json["solidLines"] = m_solidLines;
    json["lineColor"] = m_lineColor.name();
    json["vertexColor"] = m_vertexColor.name();
    json["backgroundColor"] = m_backgroundColor.name();
    json["pointSize"] = m_pointSize;
    json["lineWidth"] = m_lineWidth;

    QJsonDocument doc(json);
    QFile file(getSettingsFilePath());
    if (file.open(QIODevice::WriteOnly)) {
      file.write(doc.toJson());
      file.close();
    } else {
      qWarning() << "Failed to open settings file for writing.";
    }
  }

  /**
   * @brief Загружает настройки из JSON-файла.
   */
  void loadSettingsFromJson() {
    QFile file(getSettingsFilePath());
    if (file.open(QIODevice::ReadOnly)) {
      QByteArray data = file.readAll();
      QJsonDocument doc = QJsonDocument::fromJson(data);
      if (doc.isObject()) {
        QJsonObject json = doc.object();
        m_isPerspective = json["isPerspective"].toBool(true);
        m_showVertices = json["showVertices"].toBool(false);
        m_solidLines = json["dashedLines"].toBool(false);
        m_lineColor = QColor(json["lineColor"].toString("#42a5f5"));
        m_vertexColor = QColor(json["vertexColor"].toString("#ff0000"));
        m_backgroundColor = QColor(json["backgroundColor"].toString("#ffffff"));
        m_pointSize =
            json["pointSize"].toDouble(10.0);  // Значение по умолчанию: 10.0
        m_lineWidth =
            json["lineWidth"].toDouble(1.0);  // Значение по умолчанию: 1.0
      }
      file.close();
    } else {
      qWarning()
          << "Failed to open settings file for reading. Using default values.";
    }
  }
};
}  // namespace s21
#endif  // VIEWERSETTINGS_H
