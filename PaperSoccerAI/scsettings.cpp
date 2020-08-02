#include "scsettings.h"
#include "ui_scsettings.h"
#include <QGraphicsSceneMouseEvent>

// ===============================
// ScreenCapScene class
// ===============================

ScreenCapScene::ScreenCapScene(QObject *parent) : QGraphicsScene(parent) {}

void ScreenCapScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    QPoint pos = event->buttonDownScenePos(Qt::LeftButton).toPoint();
    emit topLeftSelected(pos.x(), pos.y());
  }
  QGraphicsScene::mousePressEvent(event);
}

void ScreenCapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QPoint pos = event->scenePos().toPoint();
  emit mousePosChanged(pos.x(), pos.y());
  QGraphicsScene::mouseMoveEvent(event);
}

void ScreenCapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    QPoint pos = event->scenePos().toPoint();
    emit bottomRightSelected(pos.x(), pos.y());
  }
  QGraphicsScene::mouseReleaseEvent(event);
}

// ===============================
// ScreenCaptureSettings class
// ===============================

ScreenCaptureSettings::ScreenCaptureSettings(QWidget *parent)
    : QDialog(parent), ui(new Ui::ScreenCaptureSettings),
      screen(QGuiApplication::primaryScreen()),
      capture_area(0, 0, screen->size().rwidth(), screen->size().rheight()) {
  ui->setupUi(this);

  // Set up scene
  ScreenCapScene *scene = new ScreenCapScene(this);
  ui->graphicsView->setScene(scene);

  // Set up capture pixmap
  captured_pixmap = new QGraphicsPixmapItem(screen->grabWindow(0));
  ui->graphicsView->scene()->addItem(captured_pixmap);

  // Set up capture rect
  QPen pen(Qt::red);
  pen.setWidth(5);
  capture_area_item = new QGraphicsRectItem(capture_area);
  capture_area_item->setPen(pen);
  scene->addItem(capture_area_item);

  // Get available screens
  getScreens();

  // Connect signals
  connect(ui->okbutton, SIGNAL(released()), this, SLOT(confirmSelection()));
  connect(ui->updatebutton, SIGNAL(released()), this, SLOT(captureScreen()));
  connect(scene, SIGNAL(mousePosChanged(int, int)), this,
          SLOT(mousePosUpdate(int, int)));
  connect(scene, SIGNAL(topLeftSelected(int, int)), this,
          SLOT(setTopLeft(int, int)));
  connect(scene, SIGNAL(bottomRightSelected(int, int)), this,
          SLOT(setBottomRight(int, int)));
  connect(ui->screencombo, SIGNAL(currentIndexChanged(int)), this,
          SLOT(setScreen(int)));
  connect(ui->screencombo, SIGNAL(currentIndexChanged(int)), this,
          SLOT(captureScreen()));
}

ScreenCaptureSettings::~ScreenCaptureSettings() { delete ui; }

void ScreenCaptureSettings::confirmSelection() {
  emit selectionConfirmed(screen, capture_area);
  hide();
}

void ScreenCaptureSettings::getScreens() {
  QList<QScreen *> screens = QGuiApplication::screens();

  ui->screencombo->clear();
  for (auto *screen : screens)
    ui->screencombo->addItem(screen->name());
}

void ScreenCaptureSettings::setScreen(int index) {
  if (index >= 0 && index < QGuiApplication::screens().size())
    screen = QGuiApplication::screens()[index];
}

void ScreenCaptureSettings::captureScreen() {
  // qDebug() << QGuiApplication::screens();

  auto dimensions_rect =
      QRect(0, 0, screen->size().width(), screen->size().height());
  captured_pixmap->setPixmap(screen->grabWindow(0));
  ui->graphicsView->fitInView(dimensions_rect,
                              Qt::AspectRatioMode::KeepAspectRatio);
}

void ScreenCaptureSettings::mousePosUpdate(int x, int y) {
  auto x1 = capture_area.left();
  auto y1 = capture_area.top();
  auto x2 = capture_area.right();
  auto y2 = capture_area.bottom();
  auto w = abs(capture_area.width());
  auto h = abs(capture_area.height());
  QString text = "(" + QString::number(x1) + "," + QString::number(y1) + "),(" +
                 QString::number(x2) + "," + QString::number(y2) + ") => " +
                 QString::number(w) + "x" + QString::number(h) + "px";
  ui->selectionlabel->setText(text);

  capture_area =
      QRect(QPoint(capture_area.left(), capture_area.top()), QPoint(x, y));
  capture_area_item->setRect(capture_area);
}

void ScreenCaptureSettings::setTopLeft(int x, int y) {
  capture_area =
      QRect(QPoint(x, y), QPoint(capture_area.right(), capture_area.bottom()));
  capture_area_item->setRect(capture_area);
}

void ScreenCaptureSettings::setBottomRight(int x, int y) {
  capture_area =
      QRect(QPoint(capture_area.left(), capture_area.top()), QPoint(x, y));
  capture_area_item->setRect(capture_area);
}
