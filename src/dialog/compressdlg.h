#ifndef COMPRESSDLG_H
#define COMPRESSDLG_H

#include <QDialog>

namespace Ui {
class CompressDlg;
}

class CompressDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CompressDlg(QWidget *parent = nullptr);
    ~CompressDlg();

private:
    Ui::CompressDlg *ui;
};

#endif // COMPRESSDLG_H
