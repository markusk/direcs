/********************************************************************************
** Form generated from reading ui file 'gui.ui'
**
** Created: Thu Oct 2 13:38:13 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Gui
{
public:
    QGroupBox *groupBoxControl;
    QPushButton *btnBackward;
    QPushButton *btnLeft;
    QPushButton *btnRight;
    QPushButton *btnStartStop;
    QPushButton *btnForward;
    QSpinBox *spinBoxSpeed1;
    QSpinBox *spinBoxSpeed2;
    QGroupBox *groupBoxNetwork;
    QLabel *label_4;
    QLineEdit *lineEditPortValue;
    QPushButton *btnTest;
    QGroupBox *groupBoxLog;
    QTextEdit *textEdit1;
    QPushButton *btnClear;
    QPushButton *btnExit;
    QLabel *lblLink;
    QGroupBox *groupBoxWeb;
    QWidget *widgetWeb;

    void setupUi(QWidget *Gui)
    {
    if (Gui->objectName().isEmpty())
        Gui->setObjectName(QString::fromUtf8("Gui"));
    Gui->resize(1151, 751);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(Gui->sizePolicy().hasHeightForWidth());
    Gui->setSizePolicy(sizePolicy);
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/images/images/underFootOne.png")), QIcon::Normal, QIcon::Off);
    Gui->setWindowIcon(icon);
    Gui->setLayoutDirection(Qt::LeftToRight);
    groupBoxControl = new QGroupBox(Gui);
    groupBoxControl->setObjectName(QString::fromUtf8("groupBoxControl"));
    groupBoxControl->setGeometry(QRect(10, 10, 411, 411));
    btnBackward = new QPushButton(groupBoxControl);
    btnBackward->setObjectName(QString::fromUtf8("btnBackward"));
    btnBackward->setGeometry(QRect(150, 280, 111, 111));
    QIcon icon1;
    icon1.addPixmap(QPixmap(QString::fromUtf8(":/images/images/down.png")), QIcon::Normal, QIcon::Off);
    btnBackward->setIcon(icon1);
    btnBackward->setIconSize(QSize(82, 82));
    btnLeft = new QPushButton(groupBoxControl);
    btnLeft->setObjectName(QString::fromUtf8("btnLeft"));
    btnLeft->setGeometry(QRect(20, 150, 111, 111));
    QIcon icon2;
    icon2.addPixmap(QPixmap(QString::fromUtf8(":/images/images/back.png")), QIcon::Normal, QIcon::Off);
    btnLeft->setIcon(icon2);
    btnLeft->setIconSize(QSize(82, 82));
    btnRight = new QPushButton(groupBoxControl);
    btnRight->setObjectName(QString::fromUtf8("btnRight"));
    btnRight->setGeometry(QRect(280, 150, 111, 111));
    QIcon icon3;
    icon3.addPixmap(QPixmap(QString::fromUtf8(":/images/images/forward.png")), QIcon::Normal, QIcon::Off);
    btnRight->setIcon(icon3);
    btnRight->setIconSize(QSize(62, 62));
    btnStartStop = new QPushButton(groupBoxControl);
    btnStartStop->setObjectName(QString::fromUtf8("btnStartStop"));
    btnStartStop->setGeometry(QRect(150, 150, 111, 110));
    btnStartStop->setFocusPolicy(Qt::ClickFocus);
    btnStartStop->setIcon(icon);
    btnStartStop->setIconSize(QSize(82, 82));
    btnStartStop->setAutoDefault(true);
    btnStartStop->setDefault(true);
    btnForward = new QPushButton(groupBoxControl);
    btnForward->setObjectName(QString::fromUtf8("btnForward"));
    btnForward->setGeometry(QRect(150, 20, 111, 111));
    QIcon icon4;
    icon4.addPixmap(QPixmap(QString::fromUtf8(":/images/images/up.png")), QIcon::Normal, QIcon::Off);
    btnForward->setIcon(icon4);
    btnForward->setIconSize(QSize(82, 82));
    spinBoxSpeed1 = new QSpinBox(groupBoxControl);
    spinBoxSpeed1->setObjectName(QString::fromUtf8("spinBoxSpeed1"));
    spinBoxSpeed1->setGeometry(QRect(20, 20, 111, 111));
    QFont font;
    font.setPointSize(34);
    spinBoxSpeed1->setFont(font);
    spinBoxSpeed1->setAlignment(Qt::AlignCenter);
    spinBoxSpeed1->setMinimum(1);
    spinBoxSpeed1->setMaximum(255);
    spinBoxSpeed1->setValue(23);
    spinBoxSpeed2 = new QSpinBox(groupBoxControl);
    spinBoxSpeed2->setObjectName(QString::fromUtf8("spinBoxSpeed2"));
    spinBoxSpeed2->setGeometry(QRect(280, 20, 111, 111));
    spinBoxSpeed2->setFont(font);
    spinBoxSpeed2->setAlignment(Qt::AlignCenter);
    spinBoxSpeed2->setMinimum(1);
    spinBoxSpeed2->setMaximum(255);
    spinBoxSpeed2->setValue(23);
    groupBoxNetwork = new QGroupBox(Gui);
    groupBoxNetwork->setObjectName(QString::fromUtf8("groupBoxNetwork"));
    groupBoxNetwork->setGeometry(QRect(10, 430, 411, 111));
    label_4 = new QLabel(groupBoxNetwork);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(10, 20, 41, 16));
    lineEditPortValue = new QLineEdit(groupBoxNetwork);
    lineEditPortValue->setObjectName(QString::fromUtf8("lineEditPortValue"));
    lineEditPortValue->setGeometry(QRect(10, 40, 171, 24));
    lineEditPortValue->setMaxLength(6);
    btnTest = new QPushButton(groupBoxNetwork);
    btnTest->setObjectName(QString::fromUtf8("btnTest"));
    btnTest->setGeometry(QRect(10, 70, 81, 21));
    groupBoxLog = new QGroupBox(Gui);
    groupBoxLog->setObjectName(QString::fromUtf8("groupBoxLog"));
    groupBoxLog->setGeometry(QRect(10, 550, 411, 171));
    textEdit1 = new QTextEdit(groupBoxLog);
    textEdit1->setObjectName(QString::fromUtf8("textEdit1"));
    textEdit1->setGeometry(QRect(10, 20, 371, 111));
    btnClear = new QPushButton(groupBoxLog);
    btnClear->setObjectName(QString::fromUtf8("btnClear"));
    btnClear->setGeometry(QRect(10, 140, 91, 21));
    btnExit = new QPushButton(Gui);
    btnExit->setObjectName(QString::fromUtf8("btnExit"));
    btnExit->setGeometry(QRect(1060, 710, 81, 26));
    QIcon icon5;
    icon5.addPixmap(QPixmap(QString::fromUtf8(":/images/images/exit.png")), QIcon::Normal, QIcon::Off);
    btnExit->setIcon(icon5);
    lblLink = new QLabel(Gui);
    lblLink->setObjectName(QString::fromUtf8("lblLink"));
    lblLink->setGeometry(QRect(10, 730, 83, 16));
    lblLink->setCursor(QCursor(Qt::PointingHandCursor));
    lblLink->setTextFormat(Qt::RichText);
    lblLink->setOpenExternalLinks(true);
    lblLink->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);
    groupBoxWeb = new QGroupBox(Gui);
    groupBoxWeb->setObjectName(QString::fromUtf8("groupBoxWeb"));
    groupBoxWeb->setGeometry(QRect(440, 10, 701, 531));
    widgetWeb = new QWidget(groupBoxWeb);
    widgetWeb->setObjectName(QString::fromUtf8("widgetWeb"));
    widgetWeb->setGeometry(QRect(10, 20, 681, 501));

    retranslateUi(Gui);

    QMetaObject::connectSlotsByName(Gui);
    } // setupUi

    void retranslateUi(QWidget *Gui)
    {
    Gui->setWindowTitle(QApplication::translate("Gui", "mrs remote", 0, QApplication::UnicodeUTF8));
    groupBoxControl->setTitle(QApplication::translate("Gui", "Control", 0, QApplication::UnicodeUTF8));
    btnBackward->setShortcut(QApplication::translate("Gui", "2", 0, QApplication::UnicodeUTF8));
    btnLeft->setShortcut(QApplication::translate("Gui", "4", 0, QApplication::UnicodeUTF8));
    btnRight->setShortcut(QApplication::translate("Gui", "6", 0, QApplication::UnicodeUTF8));
    btnStartStop->setText(QString());
    btnStartStop->setShortcut(QApplication::translate("Gui", "5", 0, QApplication::UnicodeUTF8));
    btnForward->setShortcut(QApplication::translate("Gui", "8", 0, QApplication::UnicodeUTF8));
    groupBoxNetwork->setTitle(QApplication::translate("Gui", "Network", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("Gui", "Port:", 0, QApplication::UnicodeUTF8));
    lineEditPortValue->setText(QApplication::translate("Gui", "45454", 0, QApplication::UnicodeUTF8));
    btnTest->setText(QApplication::translate("Gui", "&Test", 0, QApplication::UnicodeUTF8));
    btnTest->setShortcut(QApplication::translate("Gui", "Alt+T", 0, QApplication::UnicodeUTF8));
    groupBoxLog->setTitle(QApplication::translate("Gui", "Log", 0, QApplication::UnicodeUTF8));
    btnClear->setText(QApplication::translate("Gui", "&Clear", 0, QApplication::UnicodeUTF8));
    btnClear->setShortcut(QApplication::translate("Gui", "Alt+C", 0, QApplication::UnicodeUTF8));
    btnExit->setText(QApplication::translate("Gui", "E&xit", 0, QApplication::UnicodeUTF8));
    btnExit->setShortcut(QApplication::translate("Gui", "Alt+X", 0, QApplication::UnicodeUTF8));
    lblLink->setText(QApplication::translate("Gui", "www.direcs.de", 0, QApplication::UnicodeUTF8));
    groupBoxWeb->setTitle(QApplication::translate("Gui", "Web", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Gui);
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
