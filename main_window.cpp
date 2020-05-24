#include "main_window.h"
#include "./ui_main_window.h"

#include <QSettings>

#define MAIN_WINDOW_GROUP QStringLiteral("Main Window")
#define GEOMETRY_KEY QStringLiteral("Geometry")
#define STATE_KEY QStringLiteral("State")

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QSettings settings;
  settings.beginGroup(MAIN_WINDOW_GROUP);
  restoreGeometry(settings.value(GEOMETRY_KEY).toByteArray());
  restoreState(settings.value(STATE_KEY).toByteArray());
}

MainWindow::~MainWindow()
{
  QSettings settings;
  settings.beginGroup(MAIN_WINDOW_GROUP);
  settings.setValue(GEOMETRY_KEY, saveGeometry());
  settings.setValue(STATE_KEY, saveState());

  delete ui;
}

