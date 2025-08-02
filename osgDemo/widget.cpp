#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //osgWidget = new osgQOpenGLWidget(this);          // 指定osg窗口显示位置
   // osgWidget->setGeometry(this->geometry());        // 指定osg窗口显示大小
    //m_fileName = "D:/baoli_OSGB_8CM/Production_3/Data/Tile_+000_+000/Tile_+000_+000_L19_00010t3.osgb";  // 显示模型路径


//    for (int i = 0;i < 7;i++) {
//        for (int j = 0;j < 9;j++) {
//            QString TileNum = "Tile_+00" + QString::number(i) + "_+00" + QString::number(j);
//            QString temp = "D:/baoli_OSGB_8CM/Production_3/Data/"+TileNum+"/"+TileNum+".osgb";
//            tilePaths.push_back(temp);
//        }
//    }
    //connect(osgWidget, SIGNAL(initialized()), this, SLOT(initOSG()));

}

Widget::~Widget()
{
    delete ui;
    if(osgWidget != NULL)
    {
        delete osgWidget;
    }
}

void Widget::initOSG()
{
    osg::Node* node = osgDB::readNodeFile(m_fileName.toStdString());

    // 1. 创建相机
    osg::ref_ptr<osg::Camera> camera = createIsometricCamera(2048, 2048);

    // 2. 创建图像对象
    osg::ref_ptr<osg::Image> image = new osg::Image;
    image->allocateImage(2048, 2048, 1, GL_RGBA, GL_UNSIGNED_BYTE);

    // 3. 将图像附加到相机
    camera->attach(osg::Camera::COLOR_BUFFER, image.get());

    osgViewer::Viewer* pViewer = osgWidget->getOsgViewer();
    pViewer->setCameraManipulator(new osgGA::TrackballManipulator);


    setupIsometricView(camera, node);

    // 4. 创建场景根节点
    osg::ref_ptr<osg::Group> sceneRoot = new osg::Group;
    sceneRoot->addChild(node);
    sceneRoot->addChild(camera);
    // 5. 配置视图


    pViewer->setSceneData(sceneRoot);
    //pViewer->setCamera(camera);

    osgDB::writeImageFile(*image, "D:/isometric_view.png");
    std::cout << "Saved isometric view to: " << "D:/isometric_view.png" << std::endl;
//    sceneRoot->addChild(mergeTilesets());





    //pViewer->setSceneData(node);
}

void Widget::on_pushButton_clicked()
{
    m_fileName = "D:/baoli_OSGB_8CM/Production_3/Data/Tile_+000_+006/Tile_+000_+006_L17_000.osgb";
    initOSG();
}

osg::ref_ptr<osg::Node> Widget::loadTileset(const std::string &path)
{
    // 加载OSGB格式倾斜摄影数据
        osg::ref_ptr<osg::Node> tile = osgDB::readNodeFile(path);
    // debug
        if (!tile) {
            osg::notify(osg::WARN) << "Failed to load tileset at " << path << std::endl;
            return nullptr;
        }
        return tile;
}

osg::ref_ptr<osg::Group> Widget::mergeTilesets()
{
    //合并子结点
    osg::ref_ptr<osg::Group> root = new osg::Group;

        for (const auto& path : tilePaths) {
            if (osg::ref_ptr<osg::Node> tile = loadTileset(path.toStdString())) {
                root->addChild(tile);
            }
        }
        return root;
}



osg::Camera* Widget::createIsometricCamera(int width, int height)
{
    // 1. 创建离屏渲染相机
    osg::Camera* camera = new osg::Camera;

    // 2. 设置相机属性
    camera->setClearColor(osg::Vec4(0.9f, 0.9f, 1.0f, 1.0f)); // 天蓝色背景
    camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->setViewport(0, 0, width, height);
    camera->setRenderOrder(osg::Camera::PRE_RENDER);
    camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);

    // 3. 使用帧缓冲对象(FBO)进行离屏渲染
    camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

    return camera;
}

void Widget::setupIsometricView(osg::Camera* camera, osg::Node* scene)
{
    // 1. 获取场景包围球
    osg::BoundingSphere bs = scene->getBound();
    if (!bs.valid()) return;

    // 2. 计算2.5D视角参数
    const osg::Vec3f center = bs.center();
    const float radius = bs.radius();

    // 3. 设置等轴测投影参数
    const float distance = radius * 3.0f;  // 相机距离
    const float angle = osg::PI / 4.0f;    // 45度俯角 (俯视角度)
    const float azimuth = osg::PI / 4.0f;  // 45度方位角 (东北方向)

    // 4. 计算相机位置 (45度俯角，45度方位角)
    osg::Vec3f eye(
        center.x() + distance * cos(angle) * cos(azimuth),
        center.y() + distance * cos(angle) * sin(azimuth),
        center.z() + distance * sin(angle)
    );

    // 5. 设置视图矩阵 (看向场景中心)
    camera->setViewMatrixAsLookAt(eye, center, osg::Vec3(0, 0, 1));

    // 6. 设置正交投影 (等轴测投影)
    const float viewSize = radius * 2.5f;
    camera->setProjectionMatrixAsOrtho(
        -viewSize, viewSize,  // left, right
        -viewSize, viewSize,  // bottom, top
        -radius * 10.0f,      // near (足够远)
        radius * 10.0f        // far (足够远)
    );
}









