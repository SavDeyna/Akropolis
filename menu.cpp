#include "menu.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QDebug>
#include <QFont>
#include <QSize>


Menu::Menu(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QWidget *choixModesWidget = new QWidget(this);
    QHBoxLayout *choixModes = new QHBoxLayout(choixModesWidget);
    choixModes->setAlignment(Qt::AlignCenter);

    QVBoxLayout *choixJoueurs = new QVBoxLayout();
    choixJoueurs->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    QVBoxLayout *choixVariantes = new QVBoxLayout();
    choixVariantes->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    QLabel *titleLabel = new QLabel("Akropolis");
    titleLabel->setObjectName("gameTitleLabel");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    // Ajout du titre
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(50);

    QPushButton *playButton = new QPushButton("Jouer");
    QPushButton *chargerButton = new QPushButton("Charger une partie");
    QPushButton *quitButton = new QPushButton("Quitter");

    m_nbJoueursGroup = new QButtonGroup(this); // Parenté à Menu (this)
    // ... (Création des QRadioButtons, Lables, et ajout au m_nbJoueursGroup)

    QLabel *labelIA = new QLabel("Mode 1 Joueur");
    QLabel *labelNbJoueurs = new QLabel("Modes Classiques");
    QRadioButton *IllustreArchitecte = new QRadioButton("Illustre Architecte", this);
    QRadioButton *deuxJoueur = new QRadioButton("2 Joueuers", this);
    QRadioButton *troisJoueur = new QRadioButton("3 Joueuers", this);
    QRadioButton *quatreJoueur = new QRadioButton("4 Joueuers", this);

    m_nbJoueursGroup->addButton(IllustreArchitecte, 1);
    m_nbJoueursGroup->addButton(deuxJoueur, 2);
    m_nbJoueursGroup->addButton(troisJoueur, 3);
    m_nbJoueursGroup->addButton(quatreJoueur, 4);
    deuxJoueur->setChecked(true);
    m_selectedPlayers = 2; // Initialisation du membre logique

    // Variantes
    QLabel *labelVar = new QLabel("Variantes");
    // NOTE: Pour récupérer les choix des variantes, vous devrez les rendre
    // accessibles ou les stocker dans des QList<QCheckBox*> membres de la classe Menu.
    m_varHabitations = new QCheckBox("Habitations", this);
    m_varMarches = new QCheckBox("Marchés", this);
    m_varCasernes = new QCheckBox("Casernes", this);
    m_varTemples = new QCheckBox("Temples", this);
    m_varJardins = new QCheckBox("Jardins", this);

    // Définit une taille uniforme pour les boutons
    QSize buttonSize(300, 70);
    playButton->setFixedSize(buttonSize);
    chargerButton->setFixedSize(buttonSize);
    quitButton->setFixedSize(buttonSize);

    // Assemblage
    mainLayout->addSpacing(30);
    mainLayout->addWidget(playButton, 0, Qt::AlignCenter);
    mainLayout->addWidget(chargerButton, 0, Qt::AlignCenter);
    mainLayout->addWidget(choixModesWidget, 0, Qt::AlignCenter);
    mainLayout->addWidget(quitButton, 0, Qt::AlignCenter);

    choixModes->addLayout(choixJoueurs, 0);
    choixModes->addSpacing(50);
    choixModes->addLayout(choixVariantes, 1);

    choixJoueurs->addWidget(labelIA);
    choixJoueurs->addWidget(IllustreArchitecte);
    choixJoueurs->addWidget(labelNbJoueurs);
    choixJoueurs->addWidget(deuxJoueur);
    choixJoueurs->addWidget(troisJoueur);
    choixJoueurs->addWidget(quatreJoueur);
    choixJoueurs->addStretch();

    choixVariantes->addWidget(labelVar);
    choixVariantes->addWidget(m_varHabitations);
    choixVariantes->addWidget(m_varMarches);
    choixVariantes->addWidget(m_varCasernes);
    choixVariantes->addWidget(m_varTemples);
    choixVariantes->addWidget(m_varJardins);
    choixVariantes->addStretch();

    // Nous connectons le clic du bouton au signal que Menu ÉMET.
    connect(playButton, &QPushButton::clicked, this, &Menu::playClicked);
    connect(chargerButton, &QPushButton::clicked, this, &Menu::chargerClicked);
    connect(quitButton, &QPushButton::clicked, this, &Menu::quitClicked);

    // Connexion interne pour la logique du nombre de joueurs
    connect(m_nbJoueursGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, &Menu::onNbJoueursChanged);
}

void Menu::onNbJoueursChanged(int id)
{
    m_selectedPlayers = id;
    qDebug() << "Menu:: Nombre de joueurs sélectionné : " << m_selectedPlayers;
}

int Menu::getSelectedPlayerCount() const
{
    return m_selectedPlayers;
}

// Dans une fonction de ton Menu (ex: getSelectedVariants)
QStringList Menu::getSelectedVariantes() const {
    QStringList variantes;

    if (m_varHabitations->isChecked()) {
        variantes << "Habitations";
    }
    if (m_varMarches->isChecked()) {
        variantes << "Marchés";
    }
    if (m_varCasernes->isChecked()) {
        variantes << "Casernes";
    }
    if (m_varTemples->isChecked()) {
        variantes << "Temples";
    }
    if (m_varJardins->isChecked()) {
        variantes << "Jardins";
    }
    return variantes;
}
