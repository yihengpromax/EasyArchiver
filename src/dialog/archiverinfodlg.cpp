#include "archiverinfodlg.h"
#include "ui_archiverinfodlg.h"

ArchiverInfoDlg::ArchiverInfoDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ArchiverInfoDlg)
{
    ui->setupUi(this);
}

ArchiverInfoDlg::~ArchiverInfoDlg()
{
    delete ui;
}
