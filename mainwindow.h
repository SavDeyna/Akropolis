#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QApplication>
#include <QDebug>
#include <QWidget>
#include <QCheckBox>
#include <QRadioButton>
#include <QStackedWidget>
#include "selecjoueurs.h"
#include "menu.h"

enum Pages {
    MENU_PAGE = 0,
    SETUP_PAGE = 1
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    // Nouveaux slots pour gérer les actions et les transitions
    void showSelecJoueurs(); // Transition vers la page de configuration
    void showMenu(); // Revenir au menu

    // Slots pour gérer les actions reçues par signaux du Menu
    void onPlayClicked(); // Slot déclenché par le signal playClicked() du Menu
    void onChargerClicked();
    void onQuitClicked();

private:
    QStackedWidget *m_stackedWidget;
    SelecJoueurs* m_selecJoueursScreen;
    Menu* m_menuScreen; // Le nouveau membre représentant la page du menu
};

#endif // MAINWINDOW_H
