#pragma once

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
#include "endscreen.h"

enum Pages {
    MENU_PAGE = 0,
    SETUP_PAGE = 1,
    JEU_PAGE = 2,
    END_SCREEN_PAGE = 3
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void showSelecJoueurs(); 
    void showMenu();
    void showJeu();
    void showEndScreen();
    void onQuitClicked();
    void onSaveSelected(int idSave);  // Slot pour charger une sauvegarde sélectionnée
    void onSaveDeleted(int idSave);   // Slot pour supprimer une sauvegarde

private:
    QStackedWidget *m_stackedWidget;
    Menu* m_menuScreen;
    SelecJoueurs* m_selecJoueursScreen;
    Jeu* m_jeuScreen;
    EndScreen* m_endScreen;
};

