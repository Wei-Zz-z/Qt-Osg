#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
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

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    osg::ref_ptr<osg::Node> loadTileset(const std::string& path);
    osg::ref_ptr<osg::Group> mergeTilesets();

    osg::Camera* createIsometricCamera(int width, int height);
    void setupIsometricView(osg::Camera* camera, osg::Node* scene);

protected slots:
    void initOSG();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;

    osgQOpenGLWidget* osgWidget;
    QString m_fileName;
    std::vector<QString> tilePaths = {};

};

#endif // WIDGET_H
