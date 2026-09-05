#include "sevenzipcoreexecdlg.h"
#include "ui_sevenzipcoreexecdlg.h"

SevenZipCoreExecDlg::SevenZipCoreExecDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SevenZipCoreExecDlg)
{
    ui->setupUi(this);
    // movetothread
}

SevenZipCoreExecDlg::~SevenZipCoreExecDlg()
{
    delete ui;
}

bool SevenZipCoreExecDlg::ShowSevenZipCoreExecDlg(QWidget *parent, const QString &sFilePath, bool bExtract)
{
    SevenZipCoreExecDlg* dlg = new SevenZipCoreExecDlg(parent);
    if (dlg)
    {
        dlg->m_qsTitle = tr("正在压缩");
        if (bExtract)
        {
            dlg->m_qsTitle = tr("正在解压");
        }
        dlg->m_qsTitle += sFilePath;
        dlg->InitWidgets();
        dlg->InitConnect();
        if (dlg->exec() == QDialog::Rejected)
        {

        }

        delete dlg;
        dlg = nullptr;
    }


    return true;
}

void SevenZipCoreExecDlg::InitWidgets()
{

}

void SevenZipCoreExecDlg::InitConnect()
{

}

void SevenZipCoreExecDlg::UpdateLabel()
{

}

void SevenZipCoreExecDlg::UpdateProgress()
{

}

void SevenZipCoreExecDlg::OnBtnHideClicked()
{

}

void SevenZipCoreExecDlg::OnBtnCancelClicked()
{

}
