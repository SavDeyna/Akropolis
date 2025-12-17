#include "selecjoueurs.h"
#include <QStringList>


SelecJoueurs::SelecJoueurs(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_infoLabel = new QLabel("Configuration actuelle : 2 joueurs.");
    m_infoLabel->setAlignment(Qt::AlignCenter);

    QPushButton *launchButton = new QPushButton("Lancer le Jeu");
    QPushButton *backButton = new QPushButton("Retour au Menu");

    layout->addWidget(m_infoLabel);
    layout->addWidget(launchButton);
    layout->addWidget(backButton);
    layout->setAlignment(Qt::AlignCenter);

    connect(launchButton, &QPushButton::clicked, this, &SelecJoueurs::launchGame);
    connect(backButton, &QPushButton::clicked, this, &SelecJoueurs::backToMenu);
}

void SelecJoueurs::updateSetup(int playerCount, QStringList variantes) {
    m_infoLabel->setText(QString("Prêt à lancer une partie à %1 joueurs.").arg(playerCount));
}
