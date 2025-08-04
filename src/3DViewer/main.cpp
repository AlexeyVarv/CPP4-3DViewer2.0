#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include "adapter/fasade.h"

using namespace s21;

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  Facade facade;
  ViewerSettings appSettings;
  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("facade", &facade);
  engine.rootContext()->setContextProperty("appSettings", &appSettings);

  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  if (engine.rootObjects().isEmpty()) return -1;

  return app.exec();
}
