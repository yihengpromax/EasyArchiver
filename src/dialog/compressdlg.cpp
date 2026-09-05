#include "compressdlg.h"
#include "ui_compressdlg.h"

CompressDlg::CompressDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CompressDlg)
{
    ui->setupUi(this);
}

CompressDlg::~CompressDlg()
{
    delete ui;
}
