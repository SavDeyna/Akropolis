#include "mainwindow.h"
#include "selecjoueurs.h"
#include "menu.h"
#include "Partie.h"
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
    m_jeuScreen = new Jeu(this);
    m_endScreen = new EndScreen(this);

    // Ajout des pages au Stacked Widget
    m_stackedWidget->addWidget(m_menuScreen);        // Index 0
    m_stackedWidget->addWidget(m_selecJoueursScreen); // Index 1
    m_stackedWidget->addWidget(m_jeuScreen);          // Index 2
    m_stackedWidget->addWidget(m_endScreen);          // Index 3

    // Connexion Jouer (déclenche la fonction qui gère la transmission des données et la transition)
    connect(m_menuScreen, &Menu::playClicked, this, &MainWindow::showSelecJoueurs);

    // Connexion Quitter
    connect(m_menuScreen, &Menu::quitClicked, this, &MainWindow::onQuitClicked);

    // [Ajouter la connexion pour Charger si la classe Menu l'émet]
    // connect(m_menuScreen, &Menu::chargerClicked, this, &MainWindow::onChargerClicked);

    connect(m_selecJoueursScreen, &SelecJoueurs::backToMenu, this, &MainWindow::showMenu);

    connect(m_selecJoueursScreen, &SelecJoueurs::launchGame, this, &MainWindow::showJeu);

    // Connexions pour la sélection/suppression de sauvegardes
    connect(m_selecSaveScreen, &SelecSave::saveSelected, this, &MainWindow::onSaveSelected);
    connect(m_selecSaveScreen, &SelecSave::saveDeleted, this, &MainWindow::onSaveDeleted);

    // Connexion pour la fin de partie et retour au menu
    connect(m_jeuScreen, &Jeu::gameFinished, this, &MainWindow::showEndScreen);
    connect(m_endScreen, &EndScreen::retourMenuClicked, this, &MainWindow::showMenu);

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

void MainWindow::onQuitClicked() {
    qDebug("Bouton 'Quitter' cliqué ! Fermeture de l'application...");
    // Demande à l'application Qt de se terminer
    QApplication::quit();
}

void MainWindow::showMenu() {
    // On demande au gestionnaire de pile d'afficher la page du menu (Index 0)
    m_stackedWidget->setCurrentIndex(MENU_PAGE);

    qDebug() << "Retour au menu principal.";
}

void MainWindow::showSelecJoueurs() {
    int joueurs = m_menuScreen->getSelectedPlayerCount();
    int tuiles = m_menuScreen->getTuilesSelected();
    QStringList variantes = m_menuScreen->getSelectedVariantes();
    // Assurez-vous que la méthode de SelecJoueurs prend bien un int et un QStringList
    // (Ajustez le nom de la méthode si nécessaire, exemple : updateSetupInfo)
    m_selecJoueursScreen->updateSetup(joueurs, tuiles, variantes);

    m_stackedWidget->setCurrentIndex(SETUP_PAGE);
    qDebug() << "Affichage de l'écran de confirmation pour " << joueurs << " joueurs.";
}

void MainWindow::showJeu() {
    int nb = m_menuScreen->getSelectedPlayerCount();
    int tuiles = m_menuScreen->getTuilesSelected();
    QStringList p = m_selecJoueursScreen->getPseudos();
    QStringList v = m_menuScreen->getSelectedVariantes();

    // On les donne à l'écran de jeu
    m_jeuScreen->initialiserAffichage(nb, tuiles, v, p);

    // On affiche l'écran
    m_stackedWidget->setCurrentWidget(m_jeuScreen);
}

void MainWindow::showEndScreen() {
    // Récupérer les scores de tous les joueurs
    Partie& partie = Partie::getInstance();
    auto& participants = partie.getParticipants();
    
    std::vector<PlayerScore> scores;
    for (auto& p : participants) {
        PlayerScore ps;
        ps.pseudo = QString::fromStdString(p.getParticipant().getPseudo());
        ps.score = p.getPoints();
        ps.pierres = p.getPierres();
        scores.push_back(ps);
    }
    
    // Afficher les scores
    m_endScreen->afficherScores(scores);
    
    m_stackedWidget->setCurrentIndex(END_SCREEN_PAGE);
    qDebug() << "Affichage de l'écran de fin.";
}

void MainWindow::onSaveSelected(int idSave) {
    try {
        SauvegardeManager saveManager;
        saveManager.chargerSauvegarde(idSave);
        
        // Initialiser l'affichage du jeu depuis la sauvegarde
        m_jeuScreen->chargerDepuisSauvegarde();
        
        // Afficher l'écran de jeu
        m_stackedWidget->setCurrentWidget(m_jeuScreen);
        qDebug() << "Sauvegarde chargée avec succès, ID:" << idSave;
    } catch (const std::exception& e) {
        qDebug() << "Erreur lors du chargement de la sauvegarde:" << e.what();
    }
}

void MainWindow::onSaveDeleted(int idSave) {
    try {
        SauvegardeManager saveManager;
        saveManager.supprimerSauvegarde(idSave);
        
        // Rafraîchir la liste des sauvegardes
        std::vector<SauvegardeInfo> mesSaves = saveManager.getListeSauvegardes();
        m_selecSaveScreen->initialiserAffichage(mesSaves);
        
        qDebug() << "Sauvegarde supprimée avec succès, ID:" << idSave;
    } catch (const std::exception& e) {
        qDebug() << "Erreur lors de la suppression de la sauvegarde:" << e.what();
    }
}
