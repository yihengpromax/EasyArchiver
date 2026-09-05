#ifndef ARCHIVERINFODLG_H
#define ARCHIVERINFODLG_H

#include <QDialog>

namespace Ui {
class ArchiverInfoDlg;
}

class ArchiverInfoDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ArchiverInfoDlg(QWidget *parent = nullptr);
    ~ArchiverInfoDlg();

private:
    Ui::ArchiverInfoDlg *ui;
};

#endif // ARCHIVERINFODLG_H
