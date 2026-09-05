#ifndef EXTRACTDLG_H
#define EXTRACTDLG_H

#include <QDialog>

namespace Ui {
class ExtractDlg;
}

class ExtractDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ExtractDlg(QWidget *parent = nullptr);
    ~ExtractDlg();

private:
    Ui::ExtractDlg *ui;
};

#endif // EXTRACTDLG_H
