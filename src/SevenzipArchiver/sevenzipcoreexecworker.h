#ifndef SEVENZIPCOREEXECWORKER_H
#define SEVENZIPCOREEXECWORKER_H

#include "easyarchiver.h"

#include <QObject>


class SevenZipCoreExecWorker : public QObject
{
public:
    SevenZipCoreExecWorker(CEasyArchiver::ArchiverFunctionType type, const QString& in, const QString& out);
    void DoWorking();

signals:
    void ProgressChanged(int precent);
    void CurrentDealFileChanged(const QString& sFileName);

private:
    CEasyArchiver::ArchiverFunctionType m_iType;

};


#endif // SEVENZIPCOREEXECWORKER_H
