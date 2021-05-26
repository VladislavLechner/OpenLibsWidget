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
//    if (m_sumWidget != nullptr)
//        delete m_sumWidget;
//    else if (m_fileWidget != nullptr)
//        delete m_fileWidget;
}

void Widget::connectionToThelib()
{
    m_disconnectFromTheLib->setEnabled(true);
    m_connectToTheLib->setEnabled(false);

    std::string pathLib = (m_inputPathForScan->text().toStdString() + boost::filesystem::path::separator + m_treeView->currentIndex().data().toString().toStdString());
    m_lib = dlopen(pathLib.c_str(), RTLD_NOW);
    if (m_lib == nullptr)
    {
        throw std::runtime_error(dlerror());
//        qDebug() << "Cannot load library: " << dlerror() << '\n';
        return;
    }
    dlerror();

    getWidgetInstance();

//    typedef void *(*GetInputWidget)();
//    GetInputWidget getInputWidget = nullptr;

//    getInputWidget = reinterpret_cast<GetInputWidget>(dlsym(m_lib, "getWidgetInstance")); // приводим к указателю на фукцнию
//    if (getInputWidget == nullptr)
//    {
//        throw std::runtime_error(dlerror());
////        qDebug() << "Cannot load create function: " << dlerror() << '\n';
//        return;
//    }
//    dlerror();


//    if (m_treeView->currentIndex().data().toString().toStdString() == "libSumWidget.so")
//    {
//        m_sumWidget = reinterpret_cast<InputSumWidget *>(getInputWidget()); // получаем экземпляр класса
//        if (m_sumWidget == nullptr)
//        {
//            throw std::runtime_error("Не удалось создать экземпляр класса SumWidget");
////            qDebug() << "Cannot create data base" << '\n';
//            return;
//        }
//        m_sumWidget->show();
//    }
//    else if (m_treeView->currentIndex().data().toString().toStdString() == "libFileWidget.so")
//    {
//        m_fileWidget = reinterpret_cast<InputFileWidget *>(getInputWidget());
//        if (m_fileWidget == nullptr)
//        {
//            throw std::runtime_error("Не удалось создать экземпляр класса FileWidget");
////            qDebug() << "Cannot create data base" << '\n';
//            return;
//        }
//        m_fileWidget->show();
//    }

//    typedef std::string *(*GetInfo)();
//    GetInfo info = nullptr;

//    info = reinterpret_cast<GetInfo>(dlsym(m_lib, "getInfo")); // приводим к указателю на фукцнию
//    if (info == nullptr)
//    {
//        qDebug() << "Cannot load create function: " << dlerror() << '\n';
//        return false;
//    }
//    dlerror();

//    std::string* t = info();

//    delete t;



//    dlclose(m_lib);

}


void Widget::closeLib()
{
    m_disconnectFromTheLib->setEnabled(false);
    m_connectToTheLib->setEnabled(true);

    if (m_fileWidget != nullptr)
    {
        m_fileWidget->close();
//        delete m_fileWidget;
        m_fileWidget = nullptr;
    }
    else if (m_sumWidget != nullptr)
    {
        m_sumWidget->close();
//        typedef void (*ReleaseInputWidget)(InputSumWidget* );
//        ReleaseInputWidget getInputWidget = nullptr;

//        getInputWidget = reinterpret_cast<ReleaseInputWidget>(dlsym(m_lib, "releaseWidgetInstance")); // приводим к указателю на фукцнию
//        if (getInputWidget == nullptr)
//        {
//            throw std::runtime_error(dlerror());
//    //        qDebug() << "Cannot load create function: " << dlerror() << '\n';
//            return;
//        }
//        dlerror();
//        ReleaseInputWidget(m_sumWidget);

//        delete m_sumWidget;
        m_sumWidget = nullptr;
    }
    dlclose(m_lib);
}

//void Widget::showInputWidget()
//{
//    if (getSumWidgetInstance())
//        m_sumWidget->show();
//}

void Widget::startScanPressed()
{
    try {
        m_connectToTheLib->setEnabled(true);
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

bool Widget::getWidgetInstance()
{
//    if (m_sumWidget != nullptr)
//        delete m_sumWidget;
//    else if (m_fileWidget != nullptr)
//        delete m_fileWidget;

    typedef void *(*GetInputWidget)();
    GetInputWidget getInputWidget = nullptr;

    getInputWidget = reinterpret_cast<GetInputWidget>(dlsym(m_lib, "getWidgetInstance")); // приводим к указателю на фукцнию
    if (getInputWidget == nullptr)
    {
        throw std::runtime_error(dlerror());
//        qDebug() << "Cannot load create function: " << dlerror() << '\n';
        return false;
    }
    dlerror();


    if (m_treeView->currentIndex().data().toString().toStdString() == "libSumWidget.so")
    {
        m_sumWidget = reinterpret_cast<InputSumWidget *>(getInputWidget()); // получаем экземпляр класса
        if (m_sumWidget == nullptr)
        {
            throw std::runtime_error("Не удалось создать экземпляр класса SumWidget");
//            qDebug() << "Cannot create data base" << '\n';
            return false;
        }
        m_sumWidget->show();
    }
    else if (m_treeView->currentIndex().data().toString().toStdString() == "libFileWidget.so")
    {
        m_fileWidget = reinterpret_cast<InputFileWidget *>(getInputWidget());
        if (m_fileWidget == nullptr)
        {
            throw std::runtime_error("Не удалось создать экземпляр класса FileWidget");
//            qDebug() << "Cannot create data base" << '\n';
            return false;
        }
        m_fileWidget->show();
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

