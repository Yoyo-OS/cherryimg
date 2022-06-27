#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QCoreApplication>

#define dApp (static_cast<Application *>(QCoreApplication::instance()))
class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int &argc, char **argv);
    ~Application();
signals:
    void sigQuit();
};

#endif // APPLICATION_H
