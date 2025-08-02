#include "widget.h"
#include <osgtopng.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    // OsgToPng otp;
    // osg::Node* node = osgDB::readNodeFile("D:/baoli_OSGB_8CM/Production_3/Data/Tile_+000_+000/Tile_+000_+000_L19_00010t3.osgb");
    // otp.renderToPNG(node, "D:/isometric_view.png",2048,2048);
    w.show();
    return a.exec();
}

