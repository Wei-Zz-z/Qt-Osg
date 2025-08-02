#include "osgtopng.h"

OsgToPng::OsgToPng()
{

}
osg::Camera* OsgToPng::createIsometricCamera(int width, int height)
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
    camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER);

    return camera;
}

void OsgToPng::setupIsometricView(osg::Camera* camera, osg::Node* scene)
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

void OsgToPng::renderToPNG(osg::Node* scene, const std::string& filename, int width, int height)
{

    // 1. 创建相机
    osg::ref_ptr<osg::Camera> camera = createIsometricCamera(width, height);

    // 2. 创建图像对象
    osg::ref_ptr<osg::Image> image = new osg::Image;
    image->allocateImage(width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE);

    // 3. 将图像附加到相机
    camera->attach(osg::Camera::COLOR_BUFFER, image.get());

    // 4. 创建场景根节点
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(scene);
    //root->addChild(camera);

    // 5. 配置视图
    setupIsometricView(camera, scene);

    // 6. 创建查看器并渲染
    osgViewer::Viewer viewer;
    viewer.setSceneData(root);
    viewer.setCamera(camera);
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded); // 关键!

        // 7. 手动初始化（替代realize）
    viewer.realize();

        // 8. 渲染并保存
    viewer.frame();
    osgDB::writeImageFile(*image, filename);
    std::cout << "Saved isometric view to: " << filename << std::endl;

}
