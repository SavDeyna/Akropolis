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
#include "selecsave.h"

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
    void showSelecJoueurs();
    void showMenu();
    void showJeu();
    void showSelecSave();
    void onQuitClicked();

private:
    QStackedWidget *m_stackedWidget;
    Menu* m_menuScreen;
    SelecJoueurs* m_selecJoueursScreen;
    Jeu* m_jeuScreen;
    SelecSave* m_selecSaveScreen;
};

#endif // MAINWINDOW_H
