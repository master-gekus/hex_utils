#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QFontMetrics;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void convert();

private:
  QFontMetrics* fm_;

private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
