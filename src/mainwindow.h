#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QFileSystemModel;
class QStandardItemModel;
class CEasyArchiver;
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
    void OnCbCurrentTextChanged(const QString& sText);
    void OnLoadDirectoryFiles(const QString& sDirPath);

private:
    bool CheckSelectFileIsArchiver(const QModelIndex& index);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_pFileModel;
    QStandardItemModel *m_pStandardModel;
    QString m_qsDesktopPath;
    CEasyArchiver* m_pEasyArchiver;
};
#endif // MAINWINDOW_H
