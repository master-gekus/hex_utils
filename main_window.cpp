#include "main_window.h"
#include "./ui_main_window.h"

#include <QSettings>

#include "converters.h"

#define MAIN_WINDOW_GROUP QStringLiteral("Main Window")
#define GEOMETRY_KEY QStringLiteral("Geometry")
#define STATE_KEY QStringLiteral("State")

namespace {
const converter converters[] = {
  {QStringLiteral("Plain text (Unicode)"), from_unicode, to_unicode},
  {QStringLiteral("Plain text (UTF-8)"), from_utf8, to_utf8},
  {QStringLiteral("Hexadecimal"), from_hex, to_hex},
  {QStringLiteral("Base64"), from_base64, to_base64},
};

} // namespace {

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  fm_{nullptr},
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QSettings settings;
  settings.beginGroup(MAIN_WINDOW_GROUP);
  restoreGeometry(settings.value(GEOMETRY_KEY).toByteArray());
  restoreState(settings.value(STATE_KEY).toByteArray());

  int sourceIndex{3};
  int resultIndex{1};

  for (size_t i = 0; i < sizeof (converters) / sizeof (converters[0]); ++i) {
    ui->comboSourceType->addItem(converters[i].name_);
    ui->comboResultType->addItem(converters[i].name_);
  }

  ui->comboSourceType->setCurrentIndex(sourceIndex);
  ui->comboResultType->setCurrentIndex(resultIndex);

  fm_ = new QFontMetrics{ui->editResult->font()};

  connect(ui->comboSourceType, SIGNAL(currentIndexChanged(QString)), this, SLOT(convert()), Qt::QueuedConnection);
  connect(ui->comboResultType, SIGNAL(currentIndexChanged(QString)), this, SLOT(convert()), Qt::QueuedConnection);
  connect(ui->editSource, SIGNAL(textChanged()), this, SLOT(convert()), Qt::QueuedConnection);
  QMetaObject::invokeMethod(this, &MainWindow::convert, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
  QSettings settings;
  settings.beginGroup(MAIN_WINDOW_GROUP);
  settings.setValue(GEOMETRY_KEY, saveGeometry());
  settings.setValue(STATE_KEY, saveState());

  delete fm_;
  delete ui;
}

void MainWindow::convert()
{
  int sourceIndex{ui->comboSourceType->currentIndex()};
  int resultIndex{ui->comboResultType->currentIndex()};

  if ((0 > sourceIndex) || (0 > resultIndex)) {
    qDebug("MainWindow::convert() Source or result type is not selected.");
    return;
  }

  QString source{ui->editSource->toPlainText()};
  QByteArray source_bin{converters[sourceIndex].from_(source)};
  QString result{converters[resultIndex].to_(source_bin)};

  if (source.isEmpty()) {
    ui->groupSource->setTitle(QStringLiteral("Source"));
  }
  else {
    ui->groupSource->setTitle(QStringLiteral("Source (%1 chars, %2 bytes)").arg(source.size()).arg(source_bin.size()));
  }

  if (result.isEmpty()) {
    ui->groupResult->setTitle(QStringLiteral("Result"));
  }
  else {
    ui->groupResult->setTitle(QStringLiteral("Result (%1 chars)").arg(result.size()));
  }

  for (int i = 0; i < result.size(); ++i) {
    if ((!result[i].isPrint()) || (!fm_->inFont(result[i]))) {
      result[i] = L'.';
    }
  }

  ui->editResult->setPlainText(result);
}
