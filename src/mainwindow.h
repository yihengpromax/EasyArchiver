#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void InitWidgets();
    void InitConnect();

private slots:
    void OnBtnAddClicked();
    void OnBtnExtractClicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
