#include "selecjoueurs.h"
#include <QStringList>
#include <QLineEdit>


SelecJoueurs::SelecJoueurs(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_infoLabel = new QLabel("Configuration actuelle : 2 joueurs.");
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

void SelecJoueurs::updateSetup(int playerCount, QStringList variantes) {
    m_infoLabel->setText(QString("Prêt à lancer une partie à %1 joueurs.").arg(playerCount));


    // 1. On vide le layout actuel pour ne pas accumuler les anciens labels
    QLayoutItem *child;
    while ((child = choixVariantes->takeAt(0)) != nullptr) {
        delete child->widget(); // Supprime le widget (QLabel)
        delete child;           // Supprime l'item du layout
    }

    QLayoutItem *item;
    while ((item = m_layoutPseudos->takeAt(0)) != nullptr) {
        // Si l'item contient un widget (QLabel ou QLineEdit), on le supprime
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    m_champsPseudos.clear();

    // 2. On ajoute un titre unique si la liste n'est pas vide
    if (!variantes.isEmpty()) {
        choixVariantes->addWidget(new QLabel("<b>Variantes activées :</b>"));

        // 3. On boucle pour afficher chaque variante
        for (const QString &variante : variantes) {
            qDebug() << "Affichage de la variante :" << variante;

            // On crée un label pour chaque nom dans la liste
            QLabel *label = new QLabel("- " + variante);
            choixVariantes->addWidget(label);
        }
    } else {
        choixVariantes->addWidget(new QLabel("<i>Aucune variante sélectionnée</i>"));
    }

    for (int i = 0; i < playerCount; ++i) {
        // Création du champ avec un texte par défaut "Joueur 1", "Joueur 2", etc.
        QLineEdit *edit = new QLineEdit(QString("Joueur %1").arg(i + 1));
        edit->setFixedWidth(200);

        // On l'ajoute à l'affichage
        m_layoutPseudos->addWidget(new QLabel(QString("Pseudo Joueur %1 :").arg(i + 1)));
        m_layoutPseudos->addWidget(edit);

        // On le stocke dans notre liste pour plus tard !
        m_champsPseudos.append(edit);
    }
}

QStringList SelecJoueurs::getPseudos() const {
    QStringList listePseudos;

    // On parcourt la liste de pointeurs que nous avons remplie dans updateSetup
    for (QLineEdit *champ : m_champsPseudos) {
        if (champ) { // Sécurité : on vérifie que le pointeur n'est pas nul
            QString pseudo = champ->text().trimmed(); // .trimmed() enlève les espaces inutiles au début/fin

            // Si le joueur a laissé le champ vide, on peut mettre un nom par défaut
            if (pseudo.isEmpty()) {
                listePseudos << "Joueur Anonyme";
            } else {
                listePseudos << pseudo;
            }
        }
    }

    return listePseudos;
}
