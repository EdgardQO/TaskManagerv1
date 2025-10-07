/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionRendimiento;
    QAction *actionProcesos;
    QWidget *centralwidget;
    QStackedWidget *stackedWidgetPrincipal;
    QWidget *page;
    QLabel *label;
    QWidget *page_2;
    QLabel *label_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionRendimiento = new QAction(MainWindow);
        actionRendimiento->setObjectName("actionRendimiento");
        actionRendimiento->setMenuRole(QAction::MenuRole::NoRole);
        actionProcesos = new QAction(MainWindow);
        actionProcesos->setObjectName("actionProcesos");
        actionProcesos->setMenuRole(QAction::MenuRole::NoRole);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidgetPrincipal = new QStackedWidget(centralwidget);
        stackedWidgetPrincipal->setObjectName("stackedWidgetPrincipal");
        stackedWidgetPrincipal->setGeometry(QRect(10, 90, 781, 471));
        page = new QWidget();
        page->setObjectName("page");
        label = new QLabel(page);
        label->setObjectName("label");
        label->setGeometry(QRect(250, 150, 301, 151));
        stackedWidgetPrincipal->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        label_2 = new QLabel(page_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(320, 150, 431, 221));
        stackedWidgetPrincipal->addWidget(page_2);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        toolBar->addAction(actionProcesos);
        toolBar->addAction(actionRendimiento);

        retranslateUi(MainWindow);

        stackedWidgetPrincipal->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionRendimiento->setText(QCoreApplication::translate("MainWindow", "Rendimiento", nullptr));
        actionProcesos->setText(QCoreApplication::translate("MainWindow", "Procesos", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Vista procesos", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Vista rendimiento", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
