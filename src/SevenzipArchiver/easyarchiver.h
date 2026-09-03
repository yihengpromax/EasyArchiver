#ifndef EASYARCHIVER_H
#define EASYARCHIVER_H

// bit7z header
#include "bitfileextractor.hpp"

#include <QList>


class CEasyArchiver
{
public:
    enum ModifyMode{Append, Other};

public:
    explicit CEasyArchiver(const std::string& s7zlib, const bit7z::BitInFormat& format); // ExtractFmt
    ~CEasyArchiver();

public:
    ///////////////////////////// 1.Extracting Files from an Archive
    // Extracting a simple archive
    bool Extract(const std::string& sArchivePath, const std::string& sOutPath, const std::string& sPwd = "");

     // Extracting a specific file inside an archive
    bool Extract(const std::string& sArchivePath, const std::string& sFileName, const std::string& sOutPath, const QString& sPwd = "");

    // Extracting the first file of an archive to a buffer
    bool Extract(const std::string& sArchivePath,  std::vector< bit7z::byte_t >& buffer, const std::string& sPwd = "");


    ///////////////////////////// 2.Compressing Files into an Archive
    // Creating a simple zip archive
    bool Compress(const std::vector< std::string > files, const std::string& sArchivePath, bit7z::BitInFormat arFmt, const std::string& sPwd = "");

    // Creating a zip archive with a custom directory structure
    bool Compress(const std::map< std::string, std::string >& filesMap, const std::string& sArchivePath, bit7z::BitInFormat arFmt, const std::string& sPwd = "");

    // Compressing a directory
    bool Compress(const std::string& sDirPath, const std::string& sArchivePath, bit7z::BitInFormat arFmt, const std::string& sPwd = "");

    // Updating an existing zip archive
    bool Compress(ModifyMode mode , const std::vector< std::string > files, const std::string& sArchivePath, bit7z::BitInFormat arFmt, const std::string& sPwd = "");

    // Compressing a single file into a buffer
     bool Compress(const std::string& file, std::vector< bit7z::byte_t >& buffer, bit7z::BitInFormat arFmt, const std::string& sPwd = "");


    ///////////////////////////// 3.Reading Archive Metadata
    bool ReadArchive(const std::string& file, QList<QVariant>& qsMetaData);

private:
    bit7z::Bit7zLibrary m_7zlib;
    bit7z::BitFileExtractor m_bfeExtractor;
};
#endif // EASYARCHIVER_H
