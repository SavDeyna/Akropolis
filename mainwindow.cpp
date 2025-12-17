#include "mainwindow.h"
#include "selecjoueurs.h"
#include "menu.h"
#include <QFile>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget); // Ceci remplace l'ancien setCentralWidget de setupMenuUI

    // Crée les pages (elles construisent leur UI elles-mêmes dans leur constructeur)
    m_menuScreen = new Menu(this);
    m_selecJoueursScreen = new SelecJoueurs(this);

    // Ajout des pages au Stacked Widget
    m_stackedWidget->addWidget(m_menuScreen);        // Index 0
    m_stackedWidget->addWidget(m_selecJoueursScreen); // Index 1

    // Connexion Jouer (déclenche la fonction qui gère la transmission des données et la transition)
    connect(m_menuScreen, &Menu::playClicked, this, &MainWindow::showSelecJoueurs);

    // Connexion Quitter
    connect(m_menuScreen, &Menu::quitClicked, this, &MainWindow::onQuitClicked);

    // [Ajouter la connexion pour Charger si la classe Menu l'émet]
    // connect(m_menuScreen, &Menu::chargerClicked, this, &MainWindow::onChargerClicked);

    connect(m_selecJoueursScreen, &SelecJoueurs::backToMenu, this, &MainWindow::showMenu);

    // Afficher la page initiale
    m_stackedWidget->setCurrentIndex(MENU_PAGE);

    QFile styleFile(":/styleMenu.qss");

    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QString style = styleFile.readAll();
        this->setStyleSheet(style);
        styleFile.close();
        qDebug("Feuille de style chargée avec succès.");
    } else {
        qDebug("Erreur: Impossible de charger la feuille de style.");
    }

    setWindowTitle("Akropolis - Menu");
}
// Implémentation des slots d'action

void MainWindow::onPlayClicked() {
    qDebug("Bouton 'Jouer' cliqué ! Démarrage du jeu...");
    // Ici, vous ajouterez la logique pour passer à l'écran de jeu.
}

void MainWindow::onChargerClicked() {
    qDebug("Bouton 'Charger' cliqué ! Affichage des options...");
    // Ici, vous ajouterez la logique pour ouvrir la fenêtre des options.
}

void MainWindow::onQuitClicked() {
    qDebug("Bouton 'Quitter' cliqué ! Fermeture de l'application...");
    // Demande à l'application Qt de se terminer
    QApplication::quit();
}

void MainWindow::showSelecJoueurs() {
    int joueurs = m_menuScreen->getSelectedPlayerCount(); // <--- NOUVEAU
    QStringList variantes = m_menuScreen->getSelectedVariantes();
    // Assurez-vous que la méthode de SelecJoueurs prend bien un int et un QStringList
    // (Ajustez le nom de la méthode si nécessaire, exemple : updateSetupInfo)
    m_selecJoueursScreen->updateSetup(joueurs, variantes);

    m_stackedWidget->setCurrentIndex(SETUP_PAGE);
    qDebug() << "Affichage de l'écran de confirmation pour " << joueurs << " joueurs.";
}

void MainWindow::showMenu()
{
    // On demande au gestionnaire de pile d'afficher la page du menu (Index 0)
    m_stackedWidget->setCurrentIndex(MENU_PAGE);

    qDebug() << "Retour au menu principal.";
}
