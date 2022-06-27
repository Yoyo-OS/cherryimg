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
#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QRegExpValidator>


class RenameDialog : public QDialog
{
    Q_OBJECT
public:
    RenameDialog(const QString &filename, QWidget *parent = nullptr);
//    void onThemeChanged(ViewerThemeManager::AppTheme theme);
    QLineEdit *m_lineedt;
    QPushButton *okbtn;
    QPushButton *cancelbtn;
    QString GetFilePath();
    QString GetFileName();
    void InitDlg();

    void setCurrentTip();
    QString geteElidedText(QFont font, QString str, int MaxWidth);
private slots:
    void slotsFocusChanged(bool onFocus);
public:
    void slotsUpdate();
protected:
    void paintEvent(QPaintEvent *event);
private:
    QVBoxLayout *m_vlayout{nullptr};
    QVBoxLayout *m_labvlayout{nullptr};
    QHBoxLayout *m_hlayout{nullptr};
    QHBoxLayout *m_edtlayout{nullptr};
    QLabel *m_labformat{nullptr};
    QString m_filenamepath;
    QString m_filename;
    QString m_DirPath;
    QString m_basename;

};

#endif // RENAMEDIALOG_H
