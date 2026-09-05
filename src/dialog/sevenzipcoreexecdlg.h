#ifndef SEVENZIPCOREEXECDLG_H
#define SEVENZIPCOREEXECDLG_H

#include <QDialog>

namespace Ui {
class SevenZipCoreExecDlg;
}

class SevenZipCoreExecDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SevenZipCoreExecDlg(QWidget *parent = nullptr);
    ~SevenZipCoreExecDlg();

    bool ShowSevenZipCoreExecDlg(QWidget *parent, const QString& sFilePath, bool bExtract = true);
    void InitWidgets();
    void InitConnect();

private slots:
    void OnBtnHideClicked();
    void OnBtnCancelClicked();

private:
    void UpdateLabel();
    void UpdateProgress();

private:
    Ui::SevenZipCoreExecDlg *ui;
    QString m_qsTitle;

};

#endif // SEVENZIPCOREEXECDLG_H
