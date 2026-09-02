#include "mainwindow.h"
#include "../thrid_party/bit7z/include/bitarchivereader.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    try { // bit7z classes can throw BitException objects
        using namespace bit7z;

        Bit7zLibrary lib{ "7z.dll" };

        // Opening the archive
        BitArchiveReader archive{ lib, "path/to/archive.gz", BitFormat::GZip };

        // Testing the archive
        archive.test();

        // Extracting the archive
        archive.extractTo( "out/dir/" );
    } catch ( const bit7z::BitException& ex ) { /* Do something with ex.what()...*/ }

    return QApplication::exec();
}
