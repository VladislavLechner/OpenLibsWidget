#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    memoryAllocation();
    setUpWidgets();

    connect(m_startScan, &QPushButton::clicked,
            this, &Widget::startScanPressed);

    connect(m_connectToTheLib, &QPushButton::clicked,
            this, &Widget::connectionToThelib);

    connect(m_disconnectFromTheLib, &QPushButton::clicked,
            this, &Widget::closeLib);

}

Widget::~Widget()
{
//    if (m_lib != nullptr)
//        qDebug() << dlclose(m_lib);
//    else
//        qDebug() << "FFFFFFFFFFF";
    if (m_widget != nullptr)
        releaseWidgetInstance(m_widget);
    qDebug() << "Widget destructor";
//    m_widget->setParent(this);
}

void Widget::connectionToThelib()
{
    m_disconnectFromTheLib->setEnabled(true);
    m_connectToTheLib->setEnabled(false);
//    void* lib;

    std::string pathLib = (m_inputPathForScan->text().toStdString() + boost::filesystem::path::separator + m_treeView->currentIndex().data().toString().toStdString());
    m_lib = dlopen(pathLib.c_str(), RTLD_LAZY);
    if (m_lib == nullptr)
    {
        throw std::runtime_error(dlerror());
        return;
    }
    dlerror();

//    qDebug() << "Here";
    getWidgetInstance();
//            dlclose(m_lib);
//    else
//        qDebug() << dlerror();

}


void Widget::closeLib()
{
    m_disconnectFromTheLib->setEnabled(false);
    m_connectToTheLib->setEnabled(true);

    releaseWidgetInstance(m_widget);
    m_widget = nullptr;

    qDebug() << dlclose(m_lib);
    m_lib = nullptr;

}


void Widget::startScanPressed()
{
    try {
        m_connectToTheLib->setEnabled(true);
        ScanDirectory scanDir;
        scanDir.checkPath(m_inputPathForScan->text().toStdString());
        m_treeView->setLibsNamesAndDescription(scanDir.libsPaths());
        setUpFileSystemModel(scanDir.libsPaths(), m_inputPathForScan->text());
    }  catch (...) {
        std::exception_ptr ex;
        ex = std::current_exception();
        handle_eptr(ex);
        std::cout << "EXCEPTION!!!" << std::endl;
    }
}

bool Widget::getWidgetInstance()
{
    typedef void *(*GetInputWidget)(std::string);
    GetInputWidget getInputWidget = nullptr;

    getInputWidget = reinterpret_cast<GetInputWidget>(dlsym(m_lib, "getWidgetInstance"));
    if (getInputWidget == nullptr)
    {
        throw std::runtime_error(dlerror());
//        qDebug() << "Cannot load create function: " << dlerror() << '\n';
        return false;
    }
    dlerror();

//    m_widget = reinterpret_cast<QWidget *>(m_widget);
    m_widget = reinterpret_cast<QWidget *>(getInputWidget(m_inputPathForScan->text().toStdString()));
    if (m_widget == nullptr)
    {
        throw std::runtime_error("Не удалось открыть библиотеку");
        return false;
    }
    qDebug() << m_widget;
//    qDebug() << "Here!";
    m_widget->show();
    return true;
}

void Widget::releaseWidgetInstance(QWidget *instance)
{
    typedef void (*ReleaseInputWidget)(QWidget* );
    ReleaseInputWidget releaseInputWidget = nullptr;

    releaseInputWidget = reinterpret_cast<ReleaseInputWidget>(dlsym(m_lib, "releaseWidgetInstance"));
    if (releaseInputWidget == nullptr)
    {
        throw std::runtime_error(dlerror());
        return;
    }
    dlerror();
    instance->close();
    releaseInputWidget(instance);
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
    m_treeView = new TreeView(this);
}

void Widget::setUpWidgets()
{
    m_layOut->addWidget(m_pathForScan, 0,0);
    m_layOut->addWidget(m_inputPathForScan, 0,1);
    m_layOut->addWidget(m_startScan, 1,0,1,2);
    m_layOut->addWidget(m_connectToTheLib, 3,0);
    m_layOut->addWidget(m_disconnectFromTheLib, 3,1);

    m_layOut->addWidget(m_treeView,2,0,1,2);

    m_disconnectFromTheLib->setEnabled(false);
    m_connectToTheLib->setEnabled(false);

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

void Widget::setUpFileSystemModel(std::pair< std::list<std::string>, std::list<std::string>> libsPaths, QString path)
{
    QStringList temp;
    for (auto x: libsPaths.first)
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

