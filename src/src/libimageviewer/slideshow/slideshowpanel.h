/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     LiuMingHang <liuminghang@uniontech.com>
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
#ifndef SLIDESHOWPANEL_H
#define SLIDESHOWPANEL_H

#include "imageanimation.h"
#include "accessibility/ac-desktop-define.h"

#include <QMenu>
#include <QToolButton>
#include <QLabel>
#include <QWidget>

#include <QHBoxLayout>
#include <QShortcut>
#include <QObject>
#include <QGuiApplication>
#include <QScreen>
#include <QStyleFactory>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>

// For view images
struct ViewInfo {
    QWidget *lastPanel {nullptr};  // For back to the last panel
    int viewMainWindowID = 0;
    bool fullScreen = false;
    bool slideShow = false;
    QString album = QString();
    QString path;                       // Specific current open one
    QStringList paths = QStringList();  // Limit the view range
};

class SlideShowBottomBar : public QWidget
{
    Q_OBJECT
public:
    explicit SlideShowBottomBar(QWidget *parent = nullptr);
public:
    QToolButton *m_preButton;
    QToolButton *m_nextButton;
    QToolButton *m_playpauseButton;
    QToolButton *m_cancelButton;
    bool isStop = false;

public slots:
    void onPreButtonClicked();
    void onPlaypauseButtonClicked();
    void onUpdatePauseButton();
    void onInitSlideShowButton();
    void onNextButtonClicked();
    void onCancelButtonClicked();

signals:
    void showPrevious();
    void showPause();
    void showContinue();
    void showNext();
    void showCancel();
};

class LibSlideShowPanel : public QWidget
{
    Q_OBJECT
public:
    enum MenuItemId {
        IdStopslideshow,
        IdPlayOrPause,
        IdPlay,
        IdPause
    };
    explicit LibSlideShowPanel(QWidget *parent = nullptr);
    void initConnections();
    void initMenu();
    void appendAction(int id, const QString &text, const QString &shortcut);
    void backToLastPanel();
    void showNormal();
    void showFullScreen();

    SlideShowBottomBar *slideshowbottombar;
signals:
    void hideSlidePanel(const QString &path = "");

public slots:
    void startSlideShow(const ViewInfo &vinfo);
    void onMenuItemClicked(QAction *action);
//    void onThemeChanged(ViewerThemeManager::AppTheme dark);
    void onSingleAnimationEnd();
    void onESCKeyStopSlide();
    void onShowPause();
    void onShowContinue();
    void onShowPrevious();
    void onShowNext();
    void onCustomContextMenuRequested();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
private:
    LibImageAnimation *m_animation;
    QMenu *m_menu;
    QShortcut *m_sEsc;
    ViewInfo m_vinfo;
    QColor m_bgColor;
    bool m_isMaximized = false;
    int m_hideCursorTid;
    bool m_isf5move = false;
};

#endif // SLIDESHOWPANEL_H
