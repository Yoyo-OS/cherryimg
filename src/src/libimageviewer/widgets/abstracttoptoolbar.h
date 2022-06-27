#ifndef ABSTRACTTOPTOOLBAR_H
#define ABSTRACTTOPTOOLBAR_H

#include <QWidget>

class AbstractTopToolbar : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractTopToolbar(QWidget *parent = nullptr);

    virtual void setMiddleContent(const QString &path) = 0;
signals :
    //leaveTitle
    void sigLeaveTitle();
public slots:
    virtual void setTitleBarTransparent(bool a) = 0;
};

#endif // ABSTRACTTOPTOOLBAR_H
