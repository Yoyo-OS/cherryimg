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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
//#include <DTitlebar>
//#include <DSearchEdit>
#include <QLabel>
#include <QTabBar>
#include <QPushButton>
#include <QProgressBar>

#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QStackedWidget>
#include <QStatusBar>
#include <QButtonGroup>
#include <QJsonObject>
#include <QTimer>

const QString SETTINGS_GROUP = "MAINWINDOW";
const QString SETTINGS_WINSIZE_W_KEY = "WindowWidth";
const QString SETTINGS_WINSIZE_H_KEY = "WindowHeight";

class HomePageWidget;
class ImageViewer;
class QSettings;
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void setDMainWindow(QMainWindow *mainwidow);

    void setValue(const QString &group, const QString &key,
                  const QVariant &value);
    QVariant value(const QString &group, const QString &key,
                   const QVariant &defaultValue = QVariant());
    //解析快捷键预览
    QJsonObject createShorcutJson();

    //初始化大小
    void initSize();

private:

    void initUI();
protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;

public slots:
    void slotOpenImg();
    bool slotDrogImg(const QStringList &paths);
    void quitApp();

    //保存大小
    void slotSaveSize();

    //显示快捷键预览
    void showShortCut();
private:
    QStackedWidget   *m_centerWidget = nullptr;
    HomePageWidget   *m_homePageWidget = nullptr;
    ImageViewer      *m_imageViewer = nullptr;
    QMainWindow      *m_mainwidow = nullptr;
    QSettings        *m_settings = nullptr;
    QTimer           *m_saveSettingTimer = nullptr;
};

#endif // MAINWINDOW_H
