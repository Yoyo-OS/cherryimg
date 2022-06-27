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
#ifndef BOTTOMTOOBAR_H
#define BOTTOMTOOBAR_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QAbstractItemModel>
#include <QStandardItem>
#include <QPropertyAnimation>
#include <QHBoxLayout>

#include <QListWidget>
//#include <QSpinner>
#include <QListView>
//#include <QAnchors>
#include <QWidget>
#include <QToolButton>
//#include <QThumbnailProvider>
//#include <QIconButton>
//#include <QBlurEffectWidget>
//#include <QGuiApplicationHelper>
#include <QLabel>

#include <bitset>

#include "image-viewer_global.h"

class ElidedLabel;
class QAbstractItemModel;
//class DImageButton;
class ImageButton;
class LibImgViewListView;
class ImageItem;
class MyImageListWidget;

class LibBottomToolbar : public QWidget
{
    Q_OBJECT
public:

    const int LEFT_RIGHT_MARGIN = 10;
    const int ICON_SPACING_NOMAL = 10;//普通间距
    const int ICON_SPACING_Max = 40;//相册返回，看图下一张后，特殊间隔

    struct TTBContentData {
        int index;
//        ImageDataSt data;
    };
//    explicit TTBContent(bool inDB, DBImgInfoList m_infos, QWidget *parent = 0);
    explicit LibBottomToolbar(QWidget *parent = nullptr);
    ~LibBottomToolbar() override;

    void initUI();
    void initConnection();
//    void updateScreenNoAnimation();
    //设置需要浏览的图片信息
    void setAllFile(QString path, QStringList paths);
    //获取所有图片数量
    int getAllFileCount();
    //获取不同数量情况下工具栏的宽度
    int getToolbarWidth();
    //获取当前展示图片信息
    imageViewerSpace::ItemInfo getCurrentItemInfo();

    void setCurrentPath(const QString &path);

    //获取所有路径
    QStringList getAllPath();

    //设置旋转按钮是否可点击
    void setRotateBtnClicked(const bool &bRet);

    //设置图片操作按钮是否可点击(1:1,适应图片，ocr，旋转)
    void setPictureDoBtnClicked(const bool &bRet);

    //获得工具栏按钮
    QToolButton *getBottomtoolbarButton(imageViewerSpace::ButtonType type);

    //设置下方工具栏按钮的显示和隐藏，false为永久隐藏，true为跟随原有策略显示或隐藏
    void setButtonAlawysNotVisible(imageViewerSpace::ButtonType id, bool notVisible);

    //设置底部按钮显隐，用此API设置将会受到setBottomToolBarButtonVisiable的设置结果影响，使用getBottomtoolbarButton操作原始按钮不受此影响
    //限于目前的代码结构，这个设置仅能在设置隐藏的时候立即生效
    void setButtonVisible(imageViewerSpace::ButtonType id, bool visible);

signals:
    void resetTransform(bool fitWindow);
    void rotateClockwise();
    void rotateCounterClockwise();

    void removed();
    void imageEmpty(bool v);
    void contentWidthChanged(int width);
    void showPrevious();
    void openImg(int index, QString path);
    //平板需求，退出时重置ttb显隐
    void resetShoworHide();
    //由于添加或者删除导致长度变化,参数默认为true
    void sigWidthChanged(bool visible);

    //ocr信号
    void sigOcr();

    //保存旋转的信号
    void sigRotateSave();

    //移出窗口信号
    void sigLeaveBottom();

    void sigResizeBottom();


public slots:
    void updateCollectButton();

    void disCheckAdaptImageBtn();
    void disCheckAdaptScreenBtn();
    void checkAdaptImageBtn();
    void checkAdaptScreenBtn();
    void deleteImage();
    void onNextButton();
    void onPreButton();
    void onBackButtonClicked();
    void onAdaptImageBtnClicked();
    void onAdaptScreenBtnClicked();
    void onclBTClicked();
    void onRotateLBtnClicked();
    void onRotateRBtnClicked();
    void onTrashBtnClicked();
    void onThumbnailChanged(QPixmap pix, const QSize &originalSize);
    //主题变化
    void slotThemeChanged(int type);

    //打开一张图，需要判断工具栏的按钮的状态
    void slotOpenImage(int index, QString path);

    //设置删除键的信号槽函数链接
    void setIsConnectDel(bool bFlags);

    //缩略图移动居中
    void thumbnailMoveCenterWidget();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void leaveEvent(QEvent *e) override;
public:
    QString m_imageType;

private:
    bool m_bClBTChecked;
    QWidget *m_forwardWidget = nullptr;

    QToolButton *m_backButton = nullptr;

    QToolButton *m_preButton = nullptr;
    QToolButton *m_nextButton = nullptr;
    QWidget *m_spaceWidget = nullptr;
    QWidget *m_spaceWidget_thumbnailLeft = nullptr;
    QWidget *m_spaceWidget_thumbnailRight = nullptr;

    QToolButton *m_adaptImageBtn = nullptr;
    QToolButton *m_adaptScreenBtn = nullptr;
    QToolButton *m_clBT = nullptr;
    QToolButton *m_ocrBtn = nullptr;
    QToolButton *m_rotateLBtn = nullptr;
    QToolButton *m_rotateRBtn = nullptr;
    QToolButton *m_trashBtn = nullptr;
    std::bitset<imageViewerSpace::ButtonTypeCount> m_btnDisplaySwitch;
    MyImageListWidget *m_imgListWidget = nullptr;
    int m_windowWidth;
    int m_contentWidth;
    bool badaptImageBtnChecked = false;
    bool badaptScreenBtnChecked = false;
    QString m_currentpath = "";
};

#endif // BOTTOMTOOBAR_H
