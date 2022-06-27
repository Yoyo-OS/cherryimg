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
#ifndef MOREPICFLOATWIDGET_H
#define MOREPICFLOATWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGuiApplication>

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QWidget>

class MorePicFloatWidget : public QWidget
{
public:
    explicit MorePicFloatWidget(QWidget *parent = nullptr);
    ~MorePicFloatWidget();
    void initUI();

    QToolButton *getButtonUp();

    QToolButton *getButtonDown();

    void setLabelText(const QString &num);
private:
//ui
    QVBoxLayout *m_pLayout{nullptr};
    QLabel *m_labelNum{nullptr};
    QWidget *m_labelUp{nullptr};
    QWidget *m_labelDown{nullptr};
    QToolButton *m_buttonUp{nullptr};
    QToolButton *m_buttonDown{nullptr};
};

#endif // MOREPICFLOATWIDGET_H
