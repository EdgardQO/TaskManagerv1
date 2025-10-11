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
    QAction *actionHistorial_de_aplicaciones;
    QAction *actionInicio;
    QAction *actionUsuario;
    QAction *actionDetalles;
    QAction *actionServicios;
    QWidget *centralwidget;
    QStackedWidget *stackedWidgetPrincipal;
    QWidget *page;
    QWidget *widget;
    QWidget *page_2;
    QLabel *label;
    QWidget *page_3;
    QWidget *widget_2;
    QWidget *page_4;
    QWidget *widget_3;
    QWidget *page_5;
    QWidget *widget_4;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1182, 557);
        actionRendimiento = new QAction(MainWindow);
        actionRendimiento->setObjectName("actionRendimiento");
        actionRendimiento->setMenuRole(QAction::MenuRole::NoRole);
        actionProcesos = new QAction(MainWindow);
        actionProcesos->setObjectName("actionProcesos");
        actionProcesos->setMenuRole(QAction::MenuRole::NoRole);
        actionHistorial_de_aplicaciones = new QAction(MainWindow);
        actionHistorial_de_aplicaciones->setObjectName("actionHistorial_de_aplicaciones");
        actionHistorial_de_aplicaciones->setMenuRole(QAction::MenuRole::NoRole);
        actionInicio = new QAction(MainWindow);
        actionInicio->setObjectName("actionInicio");
        actionInicio->setMenuRole(QAction::MenuRole::NoRole);
        actionUsuario = new QAction(MainWindow);
        actionUsuario->setObjectName("actionUsuario");
        actionUsuario->setMenuRole(QAction::MenuRole::NoRole);
        actionDetalles = new QAction(MainWindow);
        actionDetalles->setObjectName("actionDetalles");
        actionDetalles->setMenuRole(QAction::MenuRole::NoRole);
        actionServicios = new QAction(MainWindow);
        actionServicios->setObjectName("actionServicios");
        actionServicios->setMenuRole(QAction::MenuRole::NoRole);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidgetPrincipal = new QStackedWidget(centralwidget);
        stackedWidgetPrincipal->setObjectName("stackedWidgetPrincipal");
        stackedWidgetPrincipal->setGeometry(QRect(30, 20, 1141, 471));
        page = new QWidget();
        page->setObjectName("page");
        widget = new QWidget(page);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(-10, 0, 1151, 481));
        stackedWidgetPrincipal->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        label = new QLabel(page_2);
        label->setObjectName("label");
        label->setGeometry(QRect(410, 60, 66, 18));
        stackedWidgetPrincipal->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        widget_2 = new QWidget(page_3);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(-11, -1, 1141, 461));
        stackedWidgetPrincipal->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        widget_3 = new QWidget(page_4);
        widget_3->setObjectName("widget_3");
        widget_3->setGeometry(QRect(-11, -11, 1141, 471));
        stackedWidgetPrincipal->addWidget(page_4);
        page_5 = new QWidget();
        page_5->setObjectName("page_5");
        widget_4 = new QWidget(page_5);
        widget_4->setObjectName("widget_4");
        widget_4->setGeometry(QRect(9, -1, 1121, 461));
        stackedWidgetPrincipal->addWidget(page_5);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1182, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        toolBar->addAction(actionProcesos);
        toolBar->addAction(actionRendimiento);
        toolBar->addAction(actionHistorial_de_aplicaciones);
        toolBar->addAction(actionInicio);
        toolBar->addAction(actionUsuario);
        toolBar->addAction(actionDetalles);
        toolBar->addAction(actionServicios);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionRendimiento->setText(QCoreApplication::translate("MainWindow", "Rendimiento", nullptr));
        actionProcesos->setText(QCoreApplication::translate("MainWindow", "Procesos", nullptr));
        actionHistorial_de_aplicaciones->setText(QCoreApplication::translate("MainWindow", "Historial de aplicaciones", nullptr));
        actionInicio->setText(QCoreApplication::translate("MainWindow", "Inicio", nullptr));
        actionUsuario->setText(QCoreApplication::translate("MainWindow", "Usuario", nullptr));
        actionDetalles->setText(QCoreApplication::translate("MainWindow", "Detalles", nullptr));
        actionServicios->setText(QCoreApplication::translate("MainWindow", "Servicios", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "vista rendimiento", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
