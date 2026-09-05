#include "extractdlg.h"
#include "ui_extractdlg.h"

ExtractDlg::ExtractDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExtractDlg)
{
    ui->setupUi(this);
}

ExtractDlg::~ExtractDlg()
{
    delete ui;
}
