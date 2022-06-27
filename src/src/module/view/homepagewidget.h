#ifndef HOMEPAGEWIDGET_H
#define HOMEPAGEWIDGET_H

#include <QWidget>
#include <QLabel>
//#include <DGuiApplicationHelper>
#include <QPushButton>
//#include <DFontSizeManager>

#include <QVBoxLayout>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QObject>
#include <QMimeData>
#include <QFileInfo>
#include <QMimeDatabase>

class QGestureEvent;
class QPinchGesture;
class QSwipeGesture;
class QPanGesture;


class HomePageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HomePageWidget(QWidget *parent = nullptr);
    ~HomePageWidget() override;

    bool checkMimeData(const QMimeData *mimeData);
    bool checkMinePaths(const QStringList &pathlist);
signals:
    void sigOpenImage();
    void sigDrogImage(const QStringList &);
public slots:

//    void ThemeChange(DGuiApplicationHelper::ColorType type);

    void openImageInDialog();

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
private:
    bool m_isDefaultThumbnail = false;
    QLabel *m_thumbnailLabel;
    QPixmap m_logo;

    QPixmap m_defaultImage;
    QColor m_inBorderColor;
    QString m_picString;
    bool m_theme = false;
    bool m_usb = false;
    int m_startx = 0;
    int m_maxTouchPoints = 0;
    QLabel *m_tips = nullptr;
    bool m_deepMode = false;
};

#endif // HOMEPAGEWIDGET_H
