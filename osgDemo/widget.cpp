#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    osgWidget = new osgQOpenGLWidget(this);          // 指定osg窗口显示位置
    osgWidget->setGeometry(this->geometry());        // 指定osg窗口显示大小
    //m_fileName = "D:/baoli_OSGB_8CM/Production_3/Data/Tile_+000_+000/Tile_+000_+000_L19_00010t3.osgb";  // 显示模型路径

//遍历读取文件名集合
   for (int i = 0;i < 7;i++) {
       for (int j = 0;j < 9;j++) {
           QString TileNum = "Tile_+00" + QString::number(i) + "_+00" + QString::number(j);
           QString temp = "D:/baoli_OSGB_8CM/Production_3/Data/"+TileNum+"/"+TileNum+".osgb";
           tilePaths.push_back(temp);
       }
   }
    connect(osgWidget, SIGNAL(initialized()), this, SLOT(initOSG()));

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

    osgViewer::Viewer* pViewer = osgWidget->getOsgViewer();
    pViewer->setCameraManipulator(new osgGA::TrackballManipulator);

    //  创建场景根节点
    osg::ref_ptr<osg::Group> sceneRoot = new osg::Group;

    // 加载文件集合到根节点
    sceneRoot->addChild(mergeTilesets());

    pViewer->setSceneData(node);
}

//暂时无用
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













