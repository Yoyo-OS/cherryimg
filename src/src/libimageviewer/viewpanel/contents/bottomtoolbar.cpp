/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     ZhangYong <zhangyong@uniontech.com>
 *
 * Maintainer: ZhangYong <ZhangYong@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "bottomtoolbar.h"

#include <QTimer>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QDebug>
#include <QPainterPath>
#include <QAbstractItemModel>
#include <QtMath>

#include <QLabel>
#include <QToolButton>

#include "imgviewlistview.h"
#include "imgviewwidget.h"
#include "accessibility/ac-desktop-define.h"
#include "unionimage/baseutils.h"
#include "unionimage/imageutils.h"
#include "unionimage/unionimage.h"
#include "service/commonservice.h"
#include "imageengine.h"
#include "image-viewer_global.h"

namespace {
/////////

/////////////
const int LEFT_MARGIN = 10;
const QSize ICON_SIZE = QSize(50, 50);
const int ICON_SPACING = 9;
const int FILENAME_MAX_LENGTH = 600;
const int RIGHT_TITLEBAR_WIDTH = 100;
const QString LOCMAP_SELECTED_DARK = ":/dark/images/58 drak.svg";
const QString LOCMAP_NOT_SELECTED_DARK = ":/dark/images/imagewithbg-dark.svg";
const QString LOCMAP_SELECTED_LIGHT = ":/light/images/58.svg";
const QString LOCMAP_NOT_SELECTED_LIGHT = ":/light/images/imagewithbg.svg";

const int TOOLBAR_MINIMUN_WIDTH = 782;
const int TOOLBAR_JUSTONE_WIDTH_ALBUM = 532;
const int TOOLBAR_JUSTONE_WIDTH_LOCAL = 350;
const int RT_SPACING = 20;
const int TOOLBAR_HEIGHT = 60;

const int TOOLBAR_DVALUE = 114 + 8;

const int THUMBNAIL_WIDTH = 32;
const int THUMBNAIL_ADD_WIDTH = 32;
const int THUMBNAIL_LIST_ADJUST = 9;
const int THUMBNAIL_VIEW_DVALUE = 668;
const int MAINWINDOW_MIN_WIDTH = 630;
const int LOAD_LEFT_RIGHT = 25;     //前后加载图片数（动态）

}  // namespace

LibBottomToolbar::LibBottomToolbar(QWidget *parent) : QWidget(parent)
{
//    this->setBlurBackgroundEnabled(true);
//    this->blurBackground()->setRadius(30);
//    this->blurBackground()->setBlurEnabled(true);
//    this->blurBackground()->setMode(DBlurEffectWidget::GaussianBlur);
//    QColor backMaskColor(255, 255, 255, 140);
//    this->blurBackground()->setMaskColor(backMaskColor);
    initUI();
    initConnection();
}

LibBottomToolbar::~LibBottomToolbar()
{

}

int LibBottomToolbar::getAllFileCount()
{
    if (m_imgListWidget) {
        return m_imgListWidget->getImgCount();
    } else {
        return -1;
    }
}

int LibBottomToolbar::getToolbarWidth()
{
    //默认值，下面会重新计算
    int width = 300;
    if (LibCommonService::instance()->getImgViewerType() == imageViewerSpace::ImgViewerType::ImgViewerTypeLocal
            || LibCommonService::instance()->getImgViewerType() == imageViewerSpace::ImgViewerType::ImgViewerTypeNull) {
        width = 0;
        setButtonVisible(imageViewerSpace::ButtonTypeBack, false);
        setButtonVisible(imageViewerSpace::ButtonTypeCollection, false);
    } else if (LibCommonService::instance()->getImgViewerType() == imageViewerSpace::ImgViewerType::ImgViewerTypeAlbum) {
        //相册
        width = 0;
        setButtonVisible(imageViewerSpace::ButtonTypeBack, true);
        setButtonVisible(imageViewerSpace::ButtonTypeCollection, true);
    }
    //如果相册的按钮存在，增加宽度
    if (m_backButton->isVisible()) {
        width += m_backButton->width() + ICON_SPACING;
    }
    if (m_clBT->isVisible()) {
        width += m_clBT->width() + ICON_SPACING;
    }
    //看图，本地图片
    width += LEFT_RIGHT_MARGIN * 2;//左右边距
    if (m_preButton->isVisible()) {
        width += m_preButton->width() + ICON_SPACING;//上一张宽度加边距
        width += m_nextButton->width() + ICON_SPACING;//上一张宽度加边距
        width += m_spaceWidget->width();//特殊控件宽度
    }
    //增加可控制设置图片宽度
    if (m_adaptImageBtn->isVisible()) {
        width += m_adaptImageBtn->width() + ICON_SPACING;//适应图片
    }
    if (m_adaptScreenBtn->isVisible()) {
        width += m_adaptScreenBtn->width() + ICON_SPACING;//适应屏幕
    }
    if (m_rotateLBtn->isVisible()) {
        width += m_rotateLBtn->width() + ICON_SPACING;//左旋
    }
    if (m_ocrBtn->isVisible()) {
        width += m_ocrBtn->width() + ICON_SPACING;//OCR
    }
    if (m_rotateRBtn->isVisible()) {
        width += m_rotateRBtn->width() + ICON_SPACING;//右旋
    }
    if (m_trashBtn->isVisible()) {
        width += m_trashBtn->width();//右旋
    }
    if (m_imgListWidget->getImgCount() <= 1) {
        width += 0;
    } else {
        //ITEM_CURRENT_WH存在着数字是60,但实际大小绘制的时候减小为30所以使用ImgViewListView::ITEM_CURRENT_WH / 2
        width += LibImgViewListView::ITEM_CURRENT_WH;
        width += (m_imgListWidget->getImgCount() /*- 1*/) * (LibImgViewListView::ITEM_CURRENT_WH / 2 + LibImgViewListView::ITEM_SPACING);
        width += m_spaceWidget_thumbnailLeft->width();
        width += m_spaceWidget_thumbnailRight->width();
    }

    return width;
}

imageViewerSpace::ItemInfo LibBottomToolbar::getCurrentItemInfo()
{
    return m_imgListWidget->getCurrentImgInfo();
}

void LibBottomToolbar::setCurrentPath(const QString &path)
{
    m_imgListWidget->setCurrentPath(path);
}

QStringList LibBottomToolbar::getAllPath()
{
    return m_imgListWidget->getAllPath();
}

void LibBottomToolbar::setRotateBtnClicked(const bool &bRet)
{
    if (m_rotateLBtn) {
        m_rotateLBtn->setEnabled(bRet);
    }
    if (m_rotateRBtn) {
        m_rotateRBtn->setEnabled(bRet);
    }
}

void LibBottomToolbar::setPictureDoBtnClicked(const bool &bRet)
{
    if (m_ocrBtn) {
        m_ocrBtn->setEnabled(bRet);
    }
    if (m_adaptImageBtn) {
        m_adaptImageBtn->setEnabled(bRet);
    }
    if (m_adaptScreenBtn) {
        m_adaptScreenBtn->setEnabled(bRet);
    }
    if (m_rotateLBtn) {
        m_rotateLBtn->setEnabled(bRet);
    }
    if (m_rotateRBtn) {
        m_rotateRBtn->setEnabled(bRet);
    }
}

QToolButton *LibBottomToolbar::getBottomtoolbarButton(imageViewerSpace::ButtonType type)
{
    QToolButton *button = nullptr;
    switch (type) {
    default:
        break;
    case imageViewerSpace::ButtonTypeBack:
        button = m_backButton;
        break;
    case imageViewerSpace::ButtonTypeNext:
        button = m_nextButton;
        break;
    case imageViewerSpace::ButtonTypePre:
        button = m_preButton;
        break;
    case imageViewerSpace::ButtonTypeAdaptImage:
        button = m_adaptImageBtn;
        break;
    case imageViewerSpace::ButtonTypeAdaptScreen:
        button = m_adaptScreenBtn;
        break;
    case imageViewerSpace::ButtonTypeCollection:
        button = m_clBT;
        break;
    case imageViewerSpace::ButtonTypeOcr:
        button = m_ocrBtn;
        break;
    case imageViewerSpace::ButtonTypeRotateLeft:
        button = m_rotateLBtn;
        break;
    case imageViewerSpace::ButtonTypeRotateRight:
        button = m_rotateRBtn;
        break;
    case imageViewerSpace::ButtonTypeTrash:
        button = m_trashBtn;
        break;
    }
    return button;
}

void LibBottomToolbar::disCheckAdaptImageBtn()
{
    m_adaptImageBtn->setChecked(false);
    badaptImageBtnChecked = false;
}
void LibBottomToolbar::disCheckAdaptScreenBtn()
{
    m_adaptScreenBtn->setChecked(false);
    badaptScreenBtnChecked = false;
}

void LibBottomToolbar::checkAdaptImageBtn()
{
    m_adaptImageBtn->setChecked(true);
    badaptImageBtnChecked = true;
}
void LibBottomToolbar::checkAdaptScreenBtn()
{
    m_adaptScreenBtn->setChecked(true);
    badaptScreenBtnChecked = true;
}

void LibBottomToolbar::deleteImage()
{
    if (m_imgListWidget->getImgCount() == 0)
        return;
    //移除正在展示照片
    if (m_imgListWidget) {
        QString path = m_imgListWidget->getCurrentPath();

        QFile file(path);
        if (!file.exists()) {
            return;
        }
        //文件是否被删除的判断bool值
        bool iRetRemove = false;
        if (LibCommonService::instance()->getImgViewerType() == imageViewerSpace::ImgViewerTypeLocal) {
            //先删除文件，需要判断文件是否删除，如果删除了，再决定看图软件的显示
            //不再采用默认删除,使用utils里面的删除
            Libutils::base::trashFile(path);
            QFile fileRemove(path);
            if (!fileRemove.exists()) {
                iRetRemove = true;
            }
        } else if (LibCommonService::instance()->getImgViewerType() == imageViewerSpace::ImgViewerTypeAlbum) {
            iRetRemove = true;
        }

        if (iRetRemove) {
            m_imgListWidget->removeCurrent();
            if (m_imgListWidget->getImgCount() == 1) {
                if (m_preButton) {
                    setButtonVisible(imageViewerSpace::ButtonTypePre, false);
                }
                if (m_nextButton) {
                    setButtonVisible(imageViewerSpace::ButtonTypeNext, false);
                }
                if (m_spaceWidget) {
                    m_spaceWidget->setVisible(false);
                }
                if (m_spaceWidget_thumbnailLeft) {
                    m_spaceWidget_thumbnailLeft->setVisible(false);
                }
                if (m_spaceWidget_thumbnailRight) {
                    m_spaceWidget_thumbnailRight->setVisible(false);
                }
                //BUG#93072 图片删到最后要清理一下控件聚焦
                m_trashBtn->clearFocus();
                //当图片不存在的时候,回到初始界面
                if (!QFileInfo(m_imgListWidget->getCurrentImgInfo().path).isFile()) {
                    emit ImageEngine::instance()->sigPicCountIsNull();
                };
            } else if (m_imgListWidget->getImgCount() == 0) {
                emit ImageEngine::instance()->sigPicCountIsNull();
            }
            if (m_imgListWidget->getCurrentCount() >= m_imgListWidget->getImgCount() - 1) {
                m_nextButton->setEnabled(false);
            }
            if (m_imgListWidget->getCurrentCount() == 0) {
                m_preButton->setEnabled(false);
            }
//        qDebug() << m_imgListWidget->getCurrentPath();
//        qDebug() << m_imgListWidget->getCurrentCount();
            emit removed();     //删除数据库图片
//        setIsConnectDel(true);
            m_imgListWidget->moveCenterWidget();
        }
    }

}

void LibBottomToolbar::onBackButtonClicked()
{
    //2020/6/9 DJH 优化退出全屏，不再闪出退出全屏的间隙 31331
    this->setVisible(false);
    this->setVisible(true);
}

void LibBottomToolbar::onAdaptImageBtnClicked()
{
    emit resetTransform(false);
    m_adaptImageBtn->setChecked(true);
    if (!badaptImageBtnChecked) {
        badaptImageBtnChecked = true;
    }
}

void LibBottomToolbar::onAdaptScreenBtnClicked()
{
    emit resetTransform(true);
    m_adaptScreenBtn->setChecked(true);
    if (!badaptScreenBtnChecked) {
        badaptScreenBtnChecked = true;
    }
}

void LibBottomToolbar::onclBTClicked()
{
    if (true == m_bClBTChecked) {
//        DBManager::instance()->removeFromAlbum(COMMON_STR_FAVORITES, QStringList(m_currentpath), AlbumDBType::Favourite);
    } else {
//        DBManager::instance()->insertIntoAlbum(COMMON_STR_FAVORITES, QStringList(m_currentpath), AlbumDBType::Favourite);
//        emit dApp->signalM->insertedIntoAlbum(COMMON_STR_FAVORITES, QStringList(m_currentpath));
    }
}

void LibBottomToolbar::onRotateLBtnClicked()
{
    emit rotateClockwise();
}

void LibBottomToolbar::onRotateRBtnClicked()
{
    emit rotateCounterClockwise();
}

void LibBottomToolbar::onTrashBtnClicked()
{
    //更换删除顺序,相册需要现在显示删除,再删除本体
    QString path;
    if (m_imgListWidget) {
        path = m_imgListWidget->getCurrentPath();
    }
    if (path.isEmpty() && m_currentpath.isEmpty()) {
        path = m_currentpath;
    }
    deleteImage();
    emit ImageEngine::instance()->sigDel(path);
    //    emit dApp->signalM->deleteByMenu();
}

void LibBottomToolbar::slotThemeChanged(int type)
{
    QString rStr;
    if (type == 1) {
        QColor maskColor(247, 247, 247);
        maskColor.setAlphaF(0.15);
//        m_forwardWidget->setMaskColor(maskColor);

        QPalette pa;
        pa = m_preButton->palette();
        pa.setColor(QPalette::Light, QColor("#FFFFFF"));
        pa.setColor(QPalette::Dark, QColor("#FFFFFF"));
        // 单个按钮边框
        QColor btnframecolor("#000000");
        btnframecolor.setAlphaF(0.00);
        pa.setColor(QPalette::Window, btnframecolor);
        // 取消阴影
        pa.setColor(QPalette::Shadow, btnframecolor);
        m_backButton->setPalette(pa);
        m_preButton->setPalette(pa);
        m_nextButton->setPalette(pa);
        m_adaptImageBtn->setPalette(pa);
        m_adaptScreenBtn->setPalette(pa);
        m_clBT->setPalette(pa);
        m_ocrBtn->setPalette(pa);
        m_rotateLBtn->setPalette(pa);
        m_rotateRBtn->setPalette(pa);
        m_trashBtn->setPalette(pa);

    } else {
        QColor maskColor("#202020");
        maskColor.setAlphaF(0.50);
//        m_forwardWidget->setMaskColor(maskColor);

        QPalette pa;
        pa = m_preButton->palette();
        pa.setColor(QPalette::Light, QColor("#303030"));
        pa.setColor(QPalette::Dark, QColor("#303030"));
        // 单个按钮边框
        QColor btnframecolor("#000000");
        btnframecolor.setAlphaF(0.30);
        pa.setColor(QPalette::Window, btnframecolor);
        // 取消阴影
        pa.setColor(QPalette::Shadow, btnframecolor);
        m_backButton->setPalette(pa);
        m_preButton->setPalette(pa);
        m_nextButton->setPalette(pa);
        m_adaptImageBtn->setPalette(pa);
        m_adaptScreenBtn->setPalette(pa);
        m_clBT->setPalette(pa);
        m_ocrBtn->setPalette(pa);
        m_rotateLBtn->setPalette(pa);
        m_rotateRBtn->setPalette(pa);
        m_trashBtn->setPalette(pa);
    }
}

void LibBottomToolbar::slotOpenImage(int index, QString path)
{
    if (index == 0) {
        m_preButton->setEnabled(false);
    } else {
        m_preButton->setEnabled(true);
    }
    if (index >= m_imgListWidget->getImgCount() - 1) {
        m_nextButton->setEnabled(false);
    } else {
        m_nextButton->setEnabled(true);
    }
//    qDebug() << index << m_imgListWidget->getImgCount();

    //BUG#93143
    QFileInfo info(path);
    //左转右转的控制不在这里
    if (!info.isFile() || !info.exists()) {
        m_adaptImageBtn->setEnabled(false);
        m_adaptImageBtn->setChecked(false);
        m_adaptScreenBtn->setEnabled(false);
        m_trashBtn->setEnabled(false);
        m_ocrBtn->setEnabled(false);
    } else {
        m_adaptImageBtn->setEnabled(true);
        m_adaptScreenBtn->setEnabled(true);
        m_trashBtn->setEnabled(true);
        m_ocrBtn->setEnabled(true);
    }
}

void LibBottomToolbar::setIsConnectDel(bool bFlags)
{
    if (bFlags) {
        connect(m_trashBtn, &QToolButton::clicked, this, &LibBottomToolbar::onTrashBtnClicked, Qt::UniqueConnection);
    } else {
        m_trashBtn->disconnect();
    }
}

void LibBottomToolbar::thumbnailMoveCenterWidget()
{
    m_imgListWidget->moveCenterWidget();
}

void LibBottomToolbar::onNextButton()
{
    emit sigRotateSave();
    if (m_rotateLBtn) {
        m_rotateLBtn->setEnabled(false);
    }
    if (m_rotateRBtn) {
        m_rotateRBtn->setEnabled(false);
    }
    if (m_ocrBtn) {
        m_ocrBtn->setEnabled(false);
    }
    if (m_imgListWidget) {
        m_imgListWidget->openNext();
    }
}

void LibBottomToolbar::onPreButton()
{
    emit sigRotateSave();
    if (m_rotateLBtn) {
        m_rotateLBtn->setEnabled(false);
    }
    if (m_rotateRBtn) {
        m_rotateRBtn->setEnabled(false);
    }
    if (m_ocrBtn) {
        m_ocrBtn->setEnabled(false);
    }
    if (m_imgListWidget) {
        m_imgListWidget->openPre();
    }
}

void LibBottomToolbar::onThumbnailChanged(QPixmap pix, const QSize &originalSize)
{
    m_imgListWidget->flushCurrentImg(pix, originalSize);
}

void LibBottomToolbar::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    if (LibCommonService::instance()->getImgViewerType() == imageViewerSpace::ImgViewerType::ImgViewerTypeAlbum
            && m_imgListWidget->isVisible()) {
        if ((topLevelWidget()->width() - MAINWINDOW_MIN_WIDTH) < (ICON_SIZE.width() * 2)) {
            m_rotateRBtn->setVisible(false);
            m_rotateLBtn->setVisible(false);
        } else {
            m_rotateRBtn->setVisible(true);
            m_rotateLBtn->setVisible(true);
        }
    }

    emit sigResizeBottom();
    m_imgListWidget->moveCenterWidget();
}

void LibBottomToolbar::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    if (LibCommonService::instance()->getImgViewerType() == imageViewerSpace::ImgViewerType::ImgViewerTypeAlbum
            && m_imgListWidget->isVisible()) {
        if ((topLevelWidget()->width() - MAINWINDOW_MIN_WIDTH) < (ICON_SIZE.width() * 2)) {
            m_rotateRBtn->setVisible(false);
            m_rotateLBtn->setVisible(false);
        } else {
            m_rotateRBtn->setVisible(true);
            m_rotateLBtn->setVisible(true);
        }
    }

    m_imgListWidget->moveCenterWidget();
}

void LibBottomToolbar::leaveEvent(QEvent *e)
{
    emit sigLeaveBottom();
    return QWidget::leaveEvent(e);
}

void LibBottomToolbar::setAllFile(QString path, QStringList paths)
{
    //每次打开清空一下缩略图
    m_imgListWidget->clearListView();
    if (paths.size() <= 1) {
        setButtonVisible(imageViewerSpace::ButtonTypePre, false);
        setButtonVisible(imageViewerSpace::ButtonTypeNext, false);
        m_spaceWidget->setVisible(false);
        m_spaceWidget_thumbnailLeft->setVisible(false);
        m_spaceWidget_thumbnailRight->setVisible(false);
    } else {
        setButtonVisible(imageViewerSpace::ButtonTypePre, true);
        setButtonVisible(imageViewerSpace::ButtonTypeNext, true);
        m_spaceWidget->setVisible(true);
        m_spaceWidget_thumbnailLeft->setVisible(true);
        m_spaceWidget_thumbnailRight->setVisible(true);
    }

    QList<imageViewerSpace::ItemInfo> itemInfos;
    for (int i = 0; i < paths.size(); i++) {
        imageViewerSpace::ItemInfo info;
        info.path = paths.at(i);
        itemInfos << info;
    }

    m_imgListWidget->setAllFile(itemInfos, path);
}

void LibBottomToolbar::updateCollectButton()
{
    if (m_currentpath.isEmpty()) {
        return;
    }
//    if (DBManager::instance()->isImgExistInAlbum(COMMON_STR_FAVORITES, m_currentpath, AlbumDBType::Favourite)) {
//        m_clBT->setToolTip(tr("Unfavorite"));
//        m_clBT->setIcon(QIcon::fromTheme("dcc_ccollection"));
//        m_clBT->setIconSize(QSize(36, 36));
//        m_bClBTChecked = true;
//    } else {
//        m_clBT->setToolTip(tr("Favorite"));
//        DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
//        Q_UNUSED(themeType);
//        m_clBT->setIcon(QIcon::fromTheme("dcc_collection_normal"));
//        m_clBT->setIconSize(QSize(36, 36));
//        m_bClBTChecked = false;
//    }
}

void LibBottomToolbar::initUI()
{
    auto backLayout = new QVBoxLayout(this);
    backLayout->setSpacing(0);
    backLayout->setContentsMargins(0, 0, 0, 0);

    m_forwardWidget = new QWidget(this);
//    m_forwardWidget->setBlurRectXRadius(18);
//    m_forwardWidget->setBlurRectYRadius(18);
//    m_forwardWidget->setRadius(30);
//    m_forwardWidget->setBlurEnabled(true);
//    m_forwardWidget->setMode(DBlurEffectWidget::GaussianBlur);
    QColor maskColor(255, 255, 255, 76);
//    m_forwardWidget->setMaskColor(maskColor);
    backLayout->addWidget(m_forwardWidget);

    QHBoxLayout *hb = new QHBoxLayout(m_forwardWidget);
    this->setLayout(hb);
    hb->setContentsMargins(LEFT_RIGHT_MARGIN, 0, LEFT_RIGHT_MARGIN, 0);
    hb->setSpacing(ICON_SPACING);

    //初始化按钮显示状态
    m_btnDisplaySwitch.set();

    //返回，相册使用
    m_backButton = new QToolButton(this);
    m_backButton->setFixedSize(ICON_SIZE);
//    AC_SET_OBJECT_NAME(m_backButton, BottomToolbar_Back_Button);
//    AC_SET_ACCESSIBLE_NAME(m_backButton, BottomToolbar_Back_Button);
    m_backButton->setIcon(QIcon(":/icons/deepin/builtin/texts/dcc_back_36px.svg"));
    m_backButton->setIconSize(QSize(36, 36));
    m_backButton->setToolTip(QObject::tr("Back"));
    setButtonVisible(imageViewerSpace::ButtonTypeBack, false);
    hb->addWidget(m_backButton);

    m_spaceWidget = new QWidget(this);
    m_spaceWidget->setFixedSize(ICON_SPACING, ICON_SPACING);
    if (LibCommonService::instance()->getImgViewerType() == imageViewerSpace::ImgViewerType::ImgViewerTypeAlbum) {
        hb->addWidget(m_spaceWidget);
        setButtonVisible(imageViewerSpace::ButtonTypeBack, true);
    }

    //上一张
    m_preButton = new QToolButton(this);
//    AC_SET_OBJECT_NAME(m_preButton, BottomToolbar_Pre_Button);
//    AC_SET_ACCESSIBLE_NAME(m_preButton, BottomToolbar_Pre_Button);
    m_preButton->setFixedSize(ICON_SIZE);
    m_preButton->setIcon(QIcon(":/icons/deepin/builtin/texts/dcc_previous_36px.svg"));
    m_preButton->setIconSize(QSize(36, 36));
    m_preButton->setToolTip(QObject::tr("Previous"));
    m_preButton->hide();
    hb->addWidget(m_preButton);

    //下一张
    m_nextButton = new QToolButton(this);
//    AC_SET_OBJECT_NAME(m_nextButton, BottomToolbar_Next_Button);
//    AC_SET_ACCESSIBLE_NAME(m_nextButton, BottomToolbar_Next_Button);
    m_nextButton->setFixedSize(ICON_SIZE);
    m_nextButton->setIcon(QIcon(":/icons/deepin/builtin/texts/dcc_next_36px.svg"));
    m_nextButton->setIconSize(QSize(36, 36));
    m_nextButton->setToolTip(QObject::tr("Next"));
    m_nextButton->hide();
    hb->addWidget(m_nextButton);
    if (LibCommonService::instance()->getImgViewerType() == imageViewerSpace::ImgViewerType::ImgViewerTypeLocal) {
        hb->addWidget(m_spaceWidget);
    }

    //适应图片
    m_adaptImageBtn = new QToolButton(this);
//    AC_SET_OBJECT_NAME(m_adaptImageBtn, BottomToolbar_AdaptImg_Button);
//    AC_SET_ACCESSIBLE_NAME(m_adaptImageBtn, BottomToolbar_AdaptImg_Button);
    m_adaptImageBtn->setFixedSize(ICON_SIZE);
    m_adaptImageBtn->setIcon(QIcon(":/icons/deepin/builtin/texts/dcc_11_36px.svg"));
    m_adaptImageBtn->setIconSize(QSize(36, 36));
    m_adaptImageBtn->setToolTip(QObject::tr("1:1 Size"));
    m_adaptImageBtn->setCheckable(true);
    hb->addWidget(m_adaptImageBtn);

    //适应屏幕
    m_adaptScreenBtn = new QToolButton(this);
    m_adaptScreenBtn->setFixedSize(ICON_SIZE);
//    AC_SET_OBJECT_NAME(m_adaptScreenBtn, BottomToolbar_AdaptScreen_Button);
//    AC_SET_ACCESSIBLE_NAME(m_adaptScreenBtn, BottomToolbar_AdaptScreen_Button);
    m_adaptScreenBtn->setIcon(QIcon(":/icons/deepin/builtin/texts/dcc_fit_36px.svg"));
    m_adaptScreenBtn->setIconSize(QSize(36, 36));
    m_adaptScreenBtn->setToolTip(QObject::tr("Fit to window"));
//    m_adaptScreenBtn->setCheckable(true);
    hb->addWidget(m_adaptScreenBtn);

    //收藏，相册使用
    m_clBT = new QToolButton(this);
    m_clBT->setFixedSize(ICON_SIZE);
//    AC_SET_OBJECT_NAME(m_clBT, BottomToolbar_Collect_Button);
//    AC_SET_ACCESSIBLE_NAME(m_clBT, BottomToolbar_Collect_Button);
    hb->addWidget(m_clBT);

    //ocr
    m_ocrBtn = new QToolButton(this);
    m_ocrBtn->setFixedSize(ICON_SIZE);
    m_ocrBtn->setIcon(QIcon(":/icons/deepin/builtin/texts/dcc_ocr_36px.svg"));
    m_ocrBtn->setIconSize(QSize(36, 36));
    m_ocrBtn->setToolTip(QObject::tr("Extract text"));
    hb->addWidget(m_ocrBtn);

    //向左旋转
    m_rotateLBtn = new QToolButton(this);
//    AC_SET_OBJECT_NAME(m_rotateLBtn, BottomToolbar_Rotate_Left_Button);
//    AC_SET_ACCESSIBLE_NAME(m_rotateLBtn, BottomToolbar_Rotate_Left_Button);
    m_rotateLBtn->setFixedSize(ICON_SIZE);
    m_rotateLBtn->setIcon(QIcon(":/icons/deepin/builtin/texts/dcc_left_36px.svg"));
    m_rotateLBtn->setIconSize(QSize(36, 36));
    m_rotateLBtn->setToolTip(QObject::tr("Rotate counterclockwise"));
    hb->addWidget(m_rotateLBtn);

    //向右旋转
    m_rotateRBtn = new QToolButton(this);
//    AC_SET_OBJECT_NAME(m_rotateRBtn, BottomToolbar_Rotate_Right_Button);
//    AC_SET_ACCESSIBLE_NAME(m_rotateRBtn, BottomToolbar_Rotate_Right_Button);
    m_rotateRBtn->setFixedSize(ICON_SIZE);
    m_rotateRBtn->setIcon(QIcon(":/icons/deepin/builtin/texts/dcc_right_36px.svg"));
    m_rotateRBtn->setIconSize(QSize(36, 36));
    m_rotateRBtn->setToolTip(QObject::tr("Rotate clockwise"));
    hb->addWidget(m_rotateRBtn);

    //增加缩略图左侧空隙
    m_spaceWidget_thumbnailLeft = new QWidget(this);
    m_spaceWidget_thumbnailLeft->setFixedWidth(0);
    hb->addWidget(m_spaceWidget_thumbnailLeft);

    //缩略图列表
    m_imgListWidget = new MyImageListWidget(this);
    hb->addWidget(m_imgListWidget);

    //增加缩略图右侧空隙
    m_spaceWidget_thumbnailRight = new QWidget(this);
    m_spaceWidget_thumbnailRight->setFixedWidth(5);
    hb->addWidget(m_spaceWidget_thumbnailRight);

    //删除
    m_trashBtn = new QToolButton(this);
    m_trashBtn->setFixedSize(ICON_SIZE);
//    AC_SET_OBJECT_NAME(m_trashBtn, BottomToolbar_Trash_Button);
//    AC_SET_ACCESSIBLE_NAME(m_trashBtn, BottomToolbar_Trash_Button);
    m_trashBtn->setIcon(QIcon(":/icons/deepin/builtin/icons/dcc_delete_36px.svg"));
    m_trashBtn->setIconSize(QSize(36, 36));
    m_trashBtn->setToolTip(QObject::tr("Delete"));
    hb->addWidget(m_trashBtn);
}

void LibBottomToolbar::initConnection()
{
    //返回按钮，相册使用
    connect(m_backButton, &QToolButton::clicked, this, &LibBottomToolbar::onBackButtonClicked);
    //前一张
    connect(m_preButton, &QToolButton::clicked, this, &LibBottomToolbar::onPreButton);
    //下一张
    connect(m_nextButton, &QToolButton::clicked, this, &LibBottomToolbar::onNextButton);
    //适应图片
    connect(m_adaptImageBtn, &QToolButton::clicked, this, &LibBottomToolbar::onAdaptImageBtnClicked);
    //适应屏幕
    connect(m_adaptScreenBtn, &QToolButton::clicked, this, &LibBottomToolbar::onAdaptScreenBtnClicked);
    //收藏，相册使用
    connect(m_clBT, &QToolButton::clicked, this, &LibBottomToolbar::onclBTClicked);
    //向左旋转
    connect(m_rotateLBtn, &QToolButton::clicked, this, &LibBottomToolbar::onRotateLBtnClicked);
    //向右旋转
    connect(m_rotateRBtn, &QToolButton::clicked, this, &LibBottomToolbar::onRotateRBtnClicked);
    //缩略图列表，单机打开图片
    connect(m_imgListWidget, &MyImageListWidget::openImg, this, &LibBottomToolbar::openImg, Qt::QueuedConnection);
    connect(m_imgListWidget, &MyImageListWidget::openImg, this, &LibBottomToolbar::slotOpenImage);
    //删除
    connect(m_trashBtn, &QToolButton::clicked, this, &LibBottomToolbar::onTrashBtnClicked);
    //ocr
    connect(m_ocrBtn, &QToolButton::clicked, this, &LibBottomToolbar::sigOcr);
}

void LibBottomToolbar::setButtonAlawysNotVisible(imageViewerSpace::ButtonType id, bool notVisible)
{
    m_btnDisplaySwitch.set(id, !notVisible);

    if (notVisible) {
        auto btn = getBottomtoolbarButton(id);
        if (btn != nullptr) {
            btn->setVisible(false);
        }
    }
}

void LibBottomToolbar::setButtonVisible(imageViewerSpace::ButtonType id, bool visible)
{
    auto btn = getBottomtoolbarButton(id);
    if (btn != nullptr) {
        if (m_btnDisplaySwitch.test(id)) {
            btn->setVisible(visible);
        } else {
            btn->setVisible(false);
        }
    }
}
