#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    memoryAllocation();
    setUpWidgets();

    connect(m_startScan, &QPushButton::clicked,
            this, &Widget::startScanPressed);

//    connect(m_startScan, &QPushButton::clicked,
//            this, &Widget::startScanPressed);
}

Widget::~Widget()
{
}

bool Widget::connectionToThelib(/*QString path*/)
{
    m_lib = dlopen("/home/semen/qtProjects/test/libSumWidget.so", RTLD_NOW);
    if (m_lib == nullptr)
    {
        qDebug() << "Cannot load library: " << dlerror() << '\n';
        return false;
    }
    dlerror();

    typedef std::string *(*GetInfo)();
    GetInfo info = nullptr;

    info = reinterpret_cast<GetInfo>(dlsym(m_lib, "getInfo")); // приводим к указателю на фукцнию
    if (info == nullptr)
    {
        qDebug() << "Cannot load create function: " << dlerror() << '\n';
        return false;
    }
    dlerror();

    std::string t = *info();

    dlclose(m_lib);

    return true;
}


bool Widget::closeLib()
{
    return dlclose(m_lib);
}

void Widget::showInputWidget()
{
    if (getSumWidgetInstance())
        m_sumWidget->show();
}

void Widget::startScanPressed()
{
    try {
        ScanDirectory scanDir;
        scanDir.checkPath(m_inputPathForScan->text().toStdString());
        setUpFileSystemModel(scanDir.libsPaths(), m_inputPathForScan->text());
    }  catch (...) {
        std::exception_ptr ex;
        ex = std::current_exception();
        handle_eptr(ex);
        std::cout << "EXCEPTION!!!" << std::endl;
    }
}

bool Widget::getSumWidgetInstance()
{
    typedef void *(*GetInputWidget)();
    GetInputWidget getInputWidget = nullptr;

    getInputWidget = reinterpret_cast<GetInputWidget>(dlsym(m_lib, "getInstance")); // приводим к указателю на фукцнию
    if (getInputWidget == nullptr)
    {
        qDebug() << "Cannot load create function: " << dlerror() << '\n';
        return false;
    }
    dlerror();

    m_sumWidget = reinterpret_cast<InputSumWidget *>(getInputWidget()); // получаем экземпляр класса
    if (m_sumWidget == nullptr)
    {
        qDebug() << "Cannot create data base" << '\n';
        return false;
    }
    return true;
}

bool Widget::releaseSumWidgetInstance()
{
    typedef void *(*ReleaseInputWidget)(InputSumWidget* );
    ReleaseInputWidget releaseInputWidget = nullptr;

    releaseInputWidget = reinterpret_cast<ReleaseInputWidget>(dlsym(m_lib, "releaseInstance")); // приводим к указателю на фукцнию
    if (releaseInputWidget == nullptr)
    {
        qDebug() << "Cannot load create function: " << dlerror() << '\n';
        return false;
    }
    dlerror();

    qDebug() << m_sumWidget;

    releaseInputWidget(m_sumWidget);

    qDebug() << m_sumWidget;

    return true;
}

void Widget::memoryAllocation()
{
    m_connectToTheLib = new QPushButton("Connect to the lib", this);
    m_disconnectFromTheLib = new QPushButton("Disconnect from the lib", this);
    m_startScan = new QPushButton("Start scan", this);
    m_inputPathForScan = new QLineEdit("/home/semen/qtProjects/test", this);
    m_pathForScan = new QLabel("Enter the path", this);
    m_layOut = new QGridLayout(this);

    m_fileModel = new QFileSystemModel(this);
    m_treeView = new QTreeView(this);
}

void Widget::setUpWidgets()
{
    m_layOut->addWidget(m_pathForScan, 0,0);
    m_layOut->addWidget(m_inputPathForScan, 0,1);
    m_layOut->addWidget(m_startScan, 1,0,1,2);
    m_layOut->addWidget(m_connectToTheLib, 3,0);
    m_layOut->addWidget(m_disconnectFromTheLib, 3,1);

    m_layOut->addWidget(m_treeView,2,0,1,2);
}

void Widget::handle_eptr(std::exception_ptr eptr)
{
    try {
        if (eptr) {
            std::rethrow_exception(eptr);
        }
    } catch(const std::exception& e) {
        QMessageBox::warning(this, "Error", QString::fromStdString(e.what()));
    }
}

void Widget::setUpFileSystemModel(std::list<std::string> libsPaths, QString path)
{
    QStringList temp;
    for (auto x: libsPaths)
    {
        if (x.find("/") < x.size())
        {
            int pos = x.rfind("/");
            temp.push_back(QString::fromStdString(x.erase(0,pos + 1)));
        }
        else
             temp.push_back(QString::fromStdString(x));
    }
    if (!boost::filesystem::is_directory(path.toStdString()))
    {
        int pos = path.toStdString().rfind("/");
        path.toStdString().erase(pos, path.toStdString().size());
    }
    m_fileModel->setRootPath(path);
    m_treeView->setModel(m_fileModel);
    m_treeView->setRootIndex(m_fileModel->index((path)));
    m_fileModel->setNameFilters(temp);

}

