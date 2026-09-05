#include "easyarchiver.h"
#include "bitfilecompressor.hpp"
#include "bitarchivereader.hpp"

#include <QDebug>

static CEasyArchiver* g_pEasyArchiver = nullptr;
CEasyArchiver::CEasyArchiver(const std::string& s7zlib, const bit7z::BitInFormat& format)
    : m_7zlib{s7zlib}
    , m_bfeExtractor{m_7zlib, format}
{
    // TODO
}

CEasyArchiver::~CEasyArchiver()
{
    // TODO
}

CEasyArchiver *CEasyArchiver::GetInstance(const std::string &s7zlib, const bit7z::BitInFormat &format)
{
    if (!g_pEasyArchiver)
    {
        g_pEasyArchiver = new CEasyArchiver(s7zlib, format);
    }

    return g_pEasyArchiver;
}

bool CEasyArchiver::Extract(const std::string &sArchivePath, const std::string &sOutPath, const std::string &sPwd)
{
    try { // bit7z classes can throw BitException objects

        if (!sPwd.empty()) m_bfeExtractor.setPassword(sPwd);
        m_bfeExtractor.extract(sArchivePath, sOutPath);

    } catch ( const bit7z::BitException& ex ){
        qDebug() << "Extract archive Failed, Code: "<< ex.code().value() <<"Info: " << ex.what();
        return false;
    }

    return true;
}

bool CEasyArchiver::Extract(const std::string &sArchivePath, const std::string &sFileName, const std::string &sOutPath, const std::string &sPwd)
{
    try {

        if (!sPwd.empty()) m_bfeExtractor.setPassword(sPwd);
        m_bfeExtractor.extractMatching(sArchivePath, sFileName, sOutPath);

    } catch (const bit7z::BitException& ex){
        qDebug() << "Extract archive Failed, Code: "<< ex.code().value() <<"Info: " << ex.what();
        return false;
    }

    return true;
}

bool CEasyArchiver::Extract(const std::string &sArchivePath, std::vector<bit7z::byte_t> &buffer, const std::string &sPwd)
{
    try {

        if (!sPwd.empty()) m_bfeExtractor.setPassword(sPwd);
        m_bfeExtractor.extract(sArchivePath, buffer);

    } catch (const bit7z::BitException& ex){
        qDebug() << "Extract archive Failed, Code: "<< ex.code().value() <<"Info: " << ex.what();
        return false;
    }

    return true;
}

bool CEasyArchiver::Compress(const std::vector<std::string> files, const std::string &sArchivePath, const bit7z::BitInOutFormat& format, const std::string &sPwd)
{
    try {

        bit7z::BitFileCompressor compressor{ m_7zlib, format };
        if (!sPwd.empty()) compressor.setPassword(sPwd);
        compressor.compress(files, sArchivePath);

    } catch (const bit7z::BitException& ex){
        qDebug() << "Compress files Failed, Code: "<< ex.code().value() <<"Info: " << ex.what();
        return false;
    }

    return true;
}

bool CEasyArchiver::Compress(const std::map<std::string, std::string> &filesMap, const std::string &sArchivePath, const bit7z::BitInOutFormat& format, const std::string &sPwd)
{
    try {

        bit7z::BitFileCompressor compressor{ m_7zlib, format };
        if (!sPwd.empty()) compressor.setPassword(sPwd);
        compressor.compress(filesMap, sArchivePath);

    } catch (const bit7z::BitException& ex){
        qDebug() << "Compress files Failed, Code: "<< ex.code().value() <<"Info: " << ex.what();
        return false;
    }

    return true;
}

bool CEasyArchiver::Compress(const std::string &sDirPath, const std::string &sArchivePath, const bit7z::BitInOutFormat& format, const std::string &sPwd)
{
    try {

        bit7z::BitFileCompressor compressor{ m_7zlib, format };
        if (!sPwd.empty()) compressor.setPassword(sPwd);
        compressor.compressDirectory(sDirPath, sArchivePath);

    } catch (const bit7z::BitException& ex){
        qDebug() << "Compress dirctory Failed, Code: "<< ex.code().value() <<"Info: " << ex.what();
        return false;
    }

    return true;
}

bool CEasyArchiver::Compress(ModifyMode mode, const std::vector<std::string> files, const std::string &sArchivePath, const bit7z::BitInOutFormat& format, const std::string &sPwd)
{
    try {

        bit7z::BitFileCompressor compressor{ m_7zlib, format };
        if (!sPwd.empty()) compressor.setPassword(sPwd);
        switch (mode) {
        case ModifyMode::Append:
            compressor.setUpdateMode(bit7z::UpdateMode::Append);
            break;
        default:
            break;
        }

        compressor.compressFiles(files, sArchivePath);

    } catch (const bit7z::BitException& ex){
        qDebug() << "Compress files Failed, Code: "<< ex.code().value() <<"Info: " << ex.what();
        return false;
    }

    return true;
}

bool CEasyArchiver::Compress(const std::string &file, std::vector<bit7z::byte_t> &buffer, const bit7z::BitInOutFormat& format, const std::string &sPwd)
{
    try {

        bit7z::BitFileCompressor compressor{ m_7zlib, format };
        if (!sPwd.empty()) compressor.setPassword(sPwd);
        compressor.compressFile(file, buffer);

    } catch (const bit7z::BitException& ex){
        qDebug() << "Compress file Failed, Code: "<< ex.code().value() <<"Info: " << ex.what();
        return false;
    }

    return true;
}

bool CEasyArchiver::ReadArchive(const std::string &sArchivePath, std::string &qsMetaData, std::vector<std::string>& childinfos, const std::string &sPwd)
{
    try {

        bit7z::BitArchiveReader arc{ m_7zlib, sArchivePath, bit7z::BitFormat::Auto };
        if (!sPwd.empty()) arc.setPassword(sPwd);

        // Archive metadata
        qsMetaData.append("Archive properties\n");
        qsMetaData.append("  Items count: ").append(std::to_string(arc.itemsCount())).append("\n");
        qsMetaData.append("  Folders count: ").append(std::to_string(arc.foldersCount())).append("\n");
        qsMetaData.append("  Files count: ").append(std::to_string(arc.filesCount())).append("\n");
        qsMetaData.append("  Size: ").append(std::to_string(arc.size())).append("\n");
        qsMetaData.append("  Packed size: ").append(std::to_string(arc.packSize())).append("\n");

        // The metadata of the archived items
        for (const auto& item : arc)
        {
            std::string sInfo;
            sInfo.append("  Item index: ").append(std::to_string(item.index())).append("\n");
            sInfo.append("    Name: ").append(item.name()).append("\n");
            sInfo.append("    Extension: ").append(item.extension()).append("\n");
            sInfo.append("    Path: ").append(item.path()).append("\n");
            sInfo.append("    IsDir: ").append(item.isDir() ? "true" : "false").append("\n");
            sInfo.append("    Size: ").append(std::to_string(item.size())).append("\n");
            sInfo.append("    Packed size: ").append(std::to_string(item.packSize())).append("\n");
            sInfo.append("    CRC: ").append(std::to_string(item.crc())).append("\n");
            childinfos.emplace_back(sInfo);
        }

    } catch (const bit7z::BitException& ex){
        qDebug() << "Read archive Failed, Code: "<< ex.code().value() <<"Info: " << ex.what();
        return false;
    }

    return true;
}




