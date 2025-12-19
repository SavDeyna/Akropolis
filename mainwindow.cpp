#include "mainwindow.h"
#include "selecjoueurs.h"
#include "menu.h"
#include "Partie.h"
#include <QFile>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    // Crée les pages
    m_menuScreen = new Menu(this);
    m_selecJoueursScreen = new SelecJoueurs(this);
    m_jeuScreen = new Jeu(this);
    m_endScreen = new EndScreen(this);

    m_stackedWidget->addWidget(m_menuScreen);        // Index 0
    m_stackedWidget->addWidget(m_selecJoueursScreen); // Index 1
    m_stackedWidget->addWidget(m_jeuScreen);         // Index 2
    m_stackedWidget->addWidget(m_endScreen);         // Index 3

    connect(m_menuScreen, &Menu::playClicked, this, &MainWindow::showSelecJoueurs); // Jouer
    connect(m_menuScreen, &Menu::quitClicked, this, &MainWindow::onQuitClicked); // Quitter
    connect(m_selecJoueursScreen, &SelecJoueurs::backToMenu, this, &MainWindow::showMenu);
    connect(m_selecJoueursScreen, &SelecJoueurs::launchGame, this, &MainWindow::showJeu);
    connect(m_endScreen, &EndScreen::retourMenuClicked, this, &MainWindow::showMenu); // Ecran de fin
    connect(m_jeuScreen, &Jeu::gameFinished, this, &MainWindow::showEndScreen); // gameFinished => déclenche écran de fin

    m_stackedWidget->setCurrentIndex(MENU_PAGE);
    QFile styleFile(":/styleMenu.qss");

    styleFile.open(QFile::ReadOnly | QFile::Text);
    this->setStyleSheet(styleFile.readAll());
    styleFile.close();

    setWindowTitle("Akropolis - Menu");
}

void MainWindow::onQuitClicked() {
    qDebug("Bouton 'Quitter' cliqué ! Fermeture de l'application...");
    QApplication::quit();
}

void MainWindow::showMenu() {
    // On demande au gestionnaire de pile d'afficher la page du menu (Index 0)
    m_stackedWidget->setCurrentIndex(MENU_PAGE);

    qDebug() << "Retour au menu principal.";
}

void MainWindow::showSelecJoueurs() {
    int joueurs = m_menuScreen->getSelectedPlayerCount();
    QStringList variantes = m_menuScreen->getSelectedVariantes();
    m_selecJoueursScreen->updateSetup(joueurs, variantes);

    m_stackedWidget->setCurrentIndex(SETUP_PAGE);
    qDebug() << "Affichage de l'écran de confirmation pour " << joueurs << " joueurs.";
}

void MainWindow::showJeu() {
    int nb = m_menuScreen->getSelectedPlayerCount();
    QStringList pseudos = m_selecJoueursScreen->getPseudos();
    QStringList variantes = m_menuScreen->getSelectedVariantes();

    // Convert QStringList to std::vector<std::string>
    std::vector<std::string> pseudosVec;
    for (const QString& p : pseudos) {
        pseudosVec.push_back(p.toStdString());
    }
    std::vector<std::string> variantesVec;
    for (const QString& v : variantes) {
        variantesVec.push_back(v.toStdString());
    }

    // Initialize game (all logic delegated to Partie)
    Partie& partie = Partie::getInstance();
    partie.initializeNewGame(nb, pseudosVec, variantesVec);

    // Update UI
    m_jeuScreen->initialiserAffichage(nb, variantes, pseudos);
    m_jeuScreen->drawBoard(partie.getCurrentPlayer().getPlateau());
    m_jeuScreen->drawTiles();
    m_jeuScreen->setPiocheSize(partie.getPiocheSize());
    m_jeuScreen->updatePlayerInfo();

    // Display game screen
    m_stackedWidget->setCurrentWidget(m_jeuScreen);
}

void MainWindow::showEndScreen() {
    Partie& partie = Partie::getInstance();
    partie.calculerScoresFinDePartie();
    
    // Récupérer les scores des participants
    std::vector<PlayerScore> scores;
    for (const auto& p : partie.getParticipants()) {
        PlayerScore ps;
        ps.pseudo = QString::fromStdString(p.getParticipant().getPseudo());
        ps.score = p.getPoints();
        ps.pierres = p.getPierres();
        scores.push_back(ps);
    }
    
    // Afficher l'écran de fin avec les scores
    m_endScreen->afficherScores(scores);
    m_stackedWidget->setCurrentIndex(END_SCREEN_PAGE);
}
