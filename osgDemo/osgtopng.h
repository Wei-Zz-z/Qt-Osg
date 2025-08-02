#ifndef OSGTOPNG_H
#define OSGTOPNG_H

#include <QDebug>
#include <QString>
#include <osgQOpenGL/osgQOpenGLWidget>
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Camera>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>

class OsgToPng
{
public:
    OsgToPng();

    osg::Camera* createIsometricCamera(int width, int height);
    void setupIsometricView(osg::Camera* camera, osg::Node* scene);
    void renderToPNG(osg::Node* scene, const std::string& filename, int width, int height);

private:

};

#endif // OSGTOPNG_H
