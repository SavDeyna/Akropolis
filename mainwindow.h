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
#include "jeu.h"

enum Pages {
    MENU_PAGE = 0,
    SETUP_PAGE = 1,
    JEU_PAGE = 2
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
    void showJeu();

    // Slots pour gérer les actions reçues par signaux du Menu
    void onPlayClicked(); // Slot déclenché par le signal playClicked() du Menu
    void onChargerClicked();
    void onQuitClicked();

private:
    QStackedWidget *m_stackedWidget;
    Menu* m_menuScreen;
    SelecJoueurs* m_selecJoueursScreen;
    Jeu* m_jeuScreen;
};

#endif // MAINWINDOW_H
