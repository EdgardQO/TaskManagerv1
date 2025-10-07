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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Administrador
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuhola;
    QMenu *menuRendimiento;
    QMenu *menuHistorial_de_aplicaciones;
    QMenu *menuInicio;
    QMenu *menuUsuarios;
    QMenu *menuDetalles;
    QMenu *menuServicios;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Administrador)
    {
        if (Administrador->objectName().isEmpty())
            Administrador->setObjectName("Administrador");
        Administrador->resize(800, 600);
        centralwidget = new QWidget(Administrador);
        centralwidget->setObjectName("centralwidget");
        Administrador->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Administrador);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menuhola = new QMenu(menubar);
        menuhola->setObjectName("menuhola");
        menuRendimiento = new QMenu(menubar);
        menuRendimiento->setObjectName("menuRendimiento");
        menuHistorial_de_aplicaciones = new QMenu(menubar);
        menuHistorial_de_aplicaciones->setObjectName("menuHistorial_de_aplicaciones");
        menuInicio = new QMenu(menubar);
        menuInicio->setObjectName("menuInicio");
        menuUsuarios = new QMenu(menubar);
        menuUsuarios->setObjectName("menuUsuarios");
        menuDetalles = new QMenu(menubar);
        menuDetalles->setObjectName("menuDetalles");
        menuServicios = new QMenu(menubar);
        menuServicios->setObjectName("menuServicios");
        Administrador->setMenuBar(menubar);
        statusbar = new QStatusBar(Administrador);
        statusbar->setObjectName("statusbar");
        Administrador->setStatusBar(statusbar);

        menubar->addAction(menuhola->menuAction());
        menubar->addAction(menuRendimiento->menuAction());
        menubar->addAction(menuHistorial_de_aplicaciones->menuAction());
        menubar->addAction(menuInicio->menuAction());
        menubar->addAction(menuUsuarios->menuAction());
        menubar->addAction(menuDetalles->menuAction());
        menubar->addAction(menuServicios->menuAction());

        retranslateUi(Administrador);

        QMetaObject::connectSlotsByName(Administrador);
    } // setupUi

    void retranslateUi(QMainWindow *Administrador)
    {
        Administrador->setWindowTitle(QCoreApplication::translate("Administrador", "MainWindow", nullptr));
        menuhola->setTitle(QCoreApplication::translate("Administrador", "Procesos", nullptr));
        menuRendimiento->setTitle(QCoreApplication::translate("Administrador", "Rendimiento", nullptr));
        menuHistorial_de_aplicaciones->setTitle(QCoreApplication::translate("Administrador", "Historial de aplicaciones", nullptr));
        menuInicio->setTitle(QCoreApplication::translate("Administrador", "Inicio", nullptr));
        menuUsuarios->setTitle(QCoreApplication::translate("Administrador", "Usuarios", nullptr));
        menuDetalles->setTitle(QCoreApplication::translate("Administrador", "Detalles", nullptr));
        menuServicios->setTitle(QCoreApplication::translate("Administrador", "Servicios", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Administrador: public Ui_Administrador {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
