#include "easyarchiver.h"
#include "bitfilecompressor.hpp"
#include "bitarchivewriter.hpp"


CEasyArchiver::CEasyArchiver(const std::string& s7zlib, const bit7z::BitInFormat& format)
    : m_7zlib(s7zlib)
    , m_bfeExtractor(m_7zlib, format)
{
    // TODO
}

CEasyArchiver::~CEasyArchiver()
{
    // TODO
}

bool CEasyArchiver::Extract(const std::string &sArchivePath, const std::string &sOutPath, const std::string &sPwd)
{
    try { // bit7z classes can throw BitException objects

        if (!sPwd.empty()) m_bfeExtractor.setPassword(sPwd);
        m_bfeExtractor.extract(sArchivePath, sOutPath);

    } catch ( const bit7z::BitException& ex ) { return false; }

    return true;
}

bool CEasyArchiver::Extract(const std::string &sArchivePath, const std::string &sFileName, const std::string &sOutPath, const QString &sPwd)
{
    return true;
}

bool CEasyArchiver::Extract(const std::string &sArchivePath, std::vector<bit7z::byte_t> &buffer, const std::string &sPwd)
{
    return true;
}

bool CEasyArchiver::Compress(const std::vector<std::string> files, const std::string &sArchivePath, bit7z::BitInFormat arFmt, const std::string &sPwd)
{
    return true;
}

bool CEasyArchiver::Compress(const std::map<std::string, std::string> &filesMap, const std::string &sArchivePath, bit7z::BitInFormat arFmt, const std::string &sPwd)
{
    return true;
}

bool CEasyArchiver::Compress(const std::string &sDirPath, const std::string &sArchivePath, bit7z::BitInFormat arFmt, const std::string &sPwd)
{
    return true;
}

bool CEasyArchiver::Compress(ModifyMode mode, const std::vector<std::string> files, const std::string &sArchivePath, bit7z::BitInFormat arFmt, const std::string &sPwd)
{
    return true;
}

bool CEasyArchiver::Compress(const std::string &file, std::vector<bit7z::byte_t> &buffer, bit7z::BitInFormat arFmt, const std::string &sPwd)
{
    return true;
}

bool CEasyArchiver::ReadArchive(const std::string &file, QList<QVariant> &qsMetaData)
{
    return true;
}




