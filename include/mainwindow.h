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
#include "selecsave.h"
#include "endscreen.h"

enum Pages {
    MENU_PAGE = 0,
    SETUP_PAGE = 1,
    SAVE_SELECT_PAGE = 2,
    JEU_PAGE = 3,
    END_SCREEN_PAGE = 4
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
    void showSelecSave();
    void showEndScreen();
    void onQuitClicked();

private:
    QStackedWidget *m_stackedWidget;
    Menu* m_menuScreen;
    SelecJoueurs* m_selecJoueursScreen;
    Jeu* m_jeuScreen;
    SelecSave* m_selecSaveScreen;
    EndScreen* m_endScreen;
};

