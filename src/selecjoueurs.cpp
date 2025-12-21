#include "selecjoueurs.h"
#include <QStringList>
#include <QLineEdit>
#include <string>


SelecJoueurs::SelecJoueurs(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_infoLabel = new QLabel("Configuration actuelle : 2 joueurs | tuiles par défaut");
    m_infoLabel->setAlignment(Qt::AlignCenter);
    choixVariantes = new QVBoxLayout();

    QPushButton *launchButton = new QPushButton("Lancer le Jeu");
    QPushButton *backButton = new QPushButton("Retour au Menu");

    m_layoutPseudos = new QVBoxLayout();

    layout->addWidget(m_infoLabel);
    layout->addLayout(choixVariantes);
    layout->addLayout(m_layoutPseudos);
    layout->addWidget(launchButton);
    layout->addWidget(backButton);
    layout->setAlignment(Qt::AlignCenter);

    connect(launchButton, &QPushButton::clicked, this, &SelecJoueurs::launchGame);
    connect(backButton, &QPushButton::clicked, this, &SelecJoueurs::backToMenu);
}

void SelecJoueurs::updateSetup(int playerCount, int tuiles, QStringList variantes) {
    QString stringTuiles = "";

    if (tuiles == 0) {
        stringTuiles = "tuiles par défaut";
    } else {
        stringTuiles = "tuiles aléatoires";
    }

    m_infoLabel->setText(QString("Prêt à lancer une partie à %1 joueurs | %2").arg(playerCount).arg(stringTuiles));


    // on vide le layout pour pas accumuler les widgets
    QLayoutItem *child;
    while ((child = choixVariantes->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;      
    }

    QLayoutItem *item;
    while ((item = m_layoutPseudos->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    m_champsPseudos.clear();

    if (!variantes.isEmpty()) {
        choixVariantes->addWidget(new QLabel("<b>Variantes activées :</b>"));
        for (const QString &variante : variantes) {
            qDebug() << "Affichage de la variante :" << variante;
            QLabel *label = new QLabel("- " + variante);
            choixVariantes->addWidget(label);
        }
    } else {
        choixVariantes->addWidget(new QLabel("<i>Aucune variante sélectionnée</i>"));
    }

    for (int i = 0; i < playerCount; ++i) {
        QLineEdit *edit = new QLineEdit(QString("Joueur %1").arg(i + 1));
        edit->setFixedWidth(200);
        m_layoutPseudos->addWidget(new QLabel(QString("Pseudo Joueur %1 :").arg(i + 1)));
        m_layoutPseudos->addWidget(edit);
        m_champsPseudos.append(edit);
    }
}

QStringList SelecJoueurs::getPseudos() const {
    QStringList listePseudos;

    for (QLineEdit *champ : m_champsPseudos) {
        if (champ) { 
            QString pseudo = champ->text().trimmed(); // .trimmed() enlève les espaces inutiles 
            if (pseudo.isEmpty()) {
                listePseudos << "Joueur Anonyme";
            } else {
                listePseudos << pseudo;
            }
        }
    }

    return listePseudos;
}
