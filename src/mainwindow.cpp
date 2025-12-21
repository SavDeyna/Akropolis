#include "mainwindow.h"
#include "selecjoueurs.h"
#include "menu.h"
#include "Sauvegarde.h"
#include "Partie.h"
#include <QFile>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget); 

    // creation des pages
    m_menuScreen = new Menu(this);
    m_selecJoueursScreen = new SelecJoueurs(this);
    m_jeuScreen = new Jeu(this);
    m_selecSaveScreen = new SelecSave(this);
    m_endScreen = new EndScreen(this);

    // ajout des pages au QStackedWidget
    m_stackedWidget->addWidget(m_menuScreen);  
    m_stackedWidget->addWidget(m_selecJoueursScreen); 

    m_stackedWidget->addWidget(m_jeuScreen);
    m_stackedWidget->addWidget(m_selecSaveScreen);

    m_stackedWidget->addWidget(m_endScreen); 
    m_stackedWidget->addWidget(m_endScreen);  

    connect(m_menuScreen, &Menu::playClicked, this, &MainWindow::showSelecJoueurs);

    connect(m_menuScreen, &Menu::quitClicked, this, &MainWindow::onQuitClicked);

    connect(m_menuScreen, &Menu::chargerClicked, this, &MainWindow::showSelecSave);

    connect(m_selecJoueursScreen, &SelecJoueurs::backToMenu, this, &MainWindow::showMenu);

    connect(m_selecJoueursScreen, &SelecJoueurs::launchGame, this, &MainWindow::showJeu);

    connect(m_jeuScreen, &Jeu::gameFinished, this, &MainWindow::showEndScreen);
    connect(m_endScreen, &EndScreen::retourMenuClicked, this, &MainWindow::showMenu);

    // afficher la page initiale
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
    QApplication::quit();
}

void MainWindow::showMenu() {
    m_stackedWidget->setCurrentIndex(MENU_PAGE);

    qDebug() << "Retour au menu principal.";
}

void MainWindow::showSelecJoueurs() {
    int joueurs = m_menuScreen->getSelectedPlayerCount();
    int tuiles = m_menuScreen->getTuilesSelected();
    QStringList variantes = m_menuScreen->getSelectedVariantes();
    m_selecJoueursScreen->updateSetup(joueurs, tuiles, variantes);

    m_stackedWidget->setCurrentIndex(SETUP_PAGE);
    qDebug() << "Affichage de l'écran de confirmation pour " << joueurs << " joueurs.";
}

void MainWindow::showJeu() {
    int nb = m_menuScreen->getSelectedPlayerCount();
    int tuiles = m_menuScreen->getTuilesSelected();
    QStringList p = m_selecJoueursScreen->getPseudos();
    QStringList v = m_menuScreen->getSelectedVariantes();

    m_jeuScreen->initialiserAffichage(nb, tuiles, v, p);

    m_stackedWidget->setCurrentWidget(m_jeuScreen);
}

void MainWindow::showSelecSave() {
    if (!m_selecSaveScreen) {
        qDebug() << "Erreur : m_selecSaveScreen est NULL";
        return;
    }
    SauvegardeManager saveManager;
    std::vector<SauvegardeInfo> mesSaves = saveManager.getListeSauvegardes();
    m_selecSaveScreen->initialiserAffichage(mesSaves);
    m_stackedWidget->setCurrentWidget(m_selecSaveScreen);
}

void MainWindow::showEndScreen() {
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

    m_endScreen->afficherScores(scores);
    
    m_stackedWidget->setCurrentIndex(END_SCREEN_PAGE);
    qDebug() << "Affichage de l'écran de fin.";
}
