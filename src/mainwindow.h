#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileIconProvider>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


namespace Sth
{
// QFileSystemModel 列序：Name / Size / Type / Date Modified
constexpr int kNameColumn = 0;
constexpr int kTypeColumn = 2;
constexpr int kNameColumnWidth = 180;
constexpr int kTypeColumnWidth = 150;

// 作下拉列表用时去掉左侧分支连接线
constexpr auto kComboViewStyle = R"(
QTreeView::branch {
    border-image: none;
    image: none;
})";
}



class QFileSystemModel;
class QStandardItemModel;
class QStandardItem;
class QTreeView;
class CEasyArchiver;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void OnBtnAddClicked();
    void OnBtnExtractClicked();
    void OnCbCurrentTextChanged(const QString& sText);
    void OnLoadDirectoryFiles(const QString& sDirPath);
    void OnTreeActivated(const QModelIndex& index);
    void OnComboActivated(int row);
    void OnTreeContextMenu(const QPoint& pos);

private:
    void InitWidgets();
    void InitConnections();
    bool CheckSelectFileIsArchiver(const QModelIndex& index);
    void InitFileTreeView();
    void InitPathComboBox();
    QStandardItem* CreateComputerItem() const;
    QFileIconProvider m_iconProvider;
    QIcon PathIcon(const QString& path) const;

    void SetCurrentPath(const QString& path);   // 唯一入口
    void RebuildComboPath(const QString& path);
    void SelectComboItem(QStandardItem* item);

    // menu
    void BuildArchiveMenu(QMenu* menu, const QString& path);
    void BuildFileMenu   (QMenu* menu, const QString& path);
    void BuildDirMenu    (QMenu* menu, const QString& path);
    void BuildBlankMenu  (QMenu* menu);
    void AppendCommonFileActions(QMenu* menu, const QString& path);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_pFileModel;
    QStandardItemModel *m_pStandardModel;
    QString m_qsDesktopPath;
    CEasyArchiver* m_pEasyArchiver;
    QTreeView*     m_pCbView;  // 下拉用的树视图
    QStandardItem* m_pPathChainRoot;  // 路径链在第 0 行的根节点
    QString        m_strCurrentPath;
    bool           m_bSyncing;
};
#endif // MAINWINDOW_H
