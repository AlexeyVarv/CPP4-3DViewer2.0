#include "saver.h"

using namespace s21;

Saver::Saver(QObject* parent) : QObject(parent) {}

void Saver::saveRenderImage(QObject* item, const QString& path) {
  QString folderPath = "screencasts";
  QDir().mkpath(folderPath);

  QQuickItem* quickItem = qobject_cast<QQuickItem*>(item);
  if (!quickItem) return;

  auto grabResult = quickItem->grabToImage();
  if (!grabResult) return;

  QObject::connect(grabResult.data(), &QQuickItemGrabResult::ready, this,
                   [grabResult, path, folderPath]() {
                     QImage image = grabResult->image();
                     QImage opaque(image.size(), QImage::Format_RGB32);
                     QPainter painter(&opaque);
                     painter.drawImage(0, 0, image);
                     painter.end();

                     QString finalPath = folderPath + "/" + path;
                     if (!opaque.save(finalPath)) {
                       qWarning() << "Failed to save image to:" << finalPath;
                     } else {
                       qDebug() << "Saved image to:" << finalPath;
                     }
                   });
}

void Saver::startGifRecording(QObject* item) {
  gifItem_ = item;
  frameCounter_ = 0;
  QDir().mkpath("screencasts");

  if (!gifTimer_) {
    gifTimer_ = new QTimer(this);
    connect(gifTimer_, &QTimer::timeout, this, &Saver::recordGifFrame);
  }

  gifTimer_->start(100);
}

void Saver::recordGifFrame() {
  if (frameCounter_ >= totalFrames_) {
    gifTimer_->stop();
    finishGifRecording();
    return;
  }

  QQuickItem* quickItem = qobject_cast<QQuickItem*>(gifItem_);
  if (!quickItem) return;

  auto grabResult = quickItem->grabToImage();
  if (!grabResult) return;

  int currentFrame = frameCounter_;
  connect(grabResult.data(), &QQuickItemGrabResult::ready, this,
          [grabResult, currentFrame]() {
            QImage source = grabResult->image();
            QImage scaled = source.scaled(640, 480, Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation);

            QImage final(640, 480, QImage::Format_RGB32);
            final.fill(Qt::white);

            QPainter painter(&final);
            int x = (640 - scaled.width()) / 2;
            int y = (480 - scaled.height()) / 2;
            painter.drawImage(x, y, scaled);
            painter.end();

            QString path = QString("screencasts/frame_%1.png")
                               .arg(currentFrame, 3, 10, QChar('0'));
            final.save(path);
          });

  frameCounter_++;
}

void Saver::finishGifRecording() {
  QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
  QString outputPath = QString("screencasts/screencast_%1.gif").arg(timestamp);

  QString command =
      QString("convert -delay 10 -loop 0 screencasts/frame_*.png %1")
          .arg(outputPath);
  int result = system(command.toUtf8().constData());

  if (result == 0) {
    qDebug() << "GIF saved to:" << outputPath;
  } else {
    qWarning() << "Failed to create GIF. Is ImageMagick installed?";
  }

  QDir dir("screencasts");
  QStringList frames = dir.entryList(QStringList() << "frame_*.png");
  for (const QString& file : frames) {
    dir.remove(file);
  }
}
