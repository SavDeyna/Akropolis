#include "jeu.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

Jeu::Jeu(QWidget *parent) : QWidget(parent), m_joueurActuel(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_affichageInfos = new QLabel(this);
    m_affichageInfos->setAlignment(Qt::AlignCenter);

    m_joueurActuelLabel = new QLabel(this);
    m_joueurActuelLabel->setAlignment(Qt::AlignCenter);
    m_joueurActuelLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

    m_pierresLabel = new QLabel("Pierres : 0", this);
    m_pierresLabel->setAlignment(Qt::AlignCenter);

    m_suivantButton = new QPushButton("Joueur suivant");

    layout->addWidget(m_affichageInfos);
    layout->addSpacing(20);
    layout->addWidget(m_joueurActuelLabel);
    layout->addWidget(m_pierresLabel);
    layout->addWidget(m_suivantButton);

    connect(m_suivantButton, &QPushButton::clicked,
            this, &Jeu::joueurSuivant);
}

void Jeu::initialiserAffichage(int nb, QStringList var, QStringList pseudos)
{
    Q_UNUSED(nb);
    Q_UNUSED(var);

    m_pseudos = pseudos;
    m_joueurActuel = 0;

    // Initialise le vecteur des pierres à 0 pour chaque joueur
    m_pierres.fill(0, m_pseudos.size());

    // Met à jour les labels
    if (!m_pseudos.isEmpty()) {
        m_joueurActuelLabel->setText("Joueur actuel : " + m_pseudos[m_joueurActuel]);
        m_pierresLabel->setText("Pierres : 0");
    }
}

void Jeu::piocher()
{
    if (m_joueurActuel < m_pierres.size()) {
        m_pierres[m_joueurActuel]++;
        m_pierresLabel->setText("Pierres : " + QString::number(m_pierres[m_joueurActuel]));
    }
}

void Jeu::joueurSuivant()
{
    if (m_pseudos.isEmpty())
        return;

    m_joueurActuel = (m_joueurActuel + 1) % m_pseudos.size();
    m_joueurActuelLabel->setText("Joueur actuel : " + m_pseudos[m_joueurActuel]);
    m_pierresLabel->setText("Pierres : " + QString::number(m_pierres[m_joueurActuel]));
}

void Jeu::mettreAJourPierres(int nbPierres)
{
    if (m_joueurActuel < m_pierres.size()) {
        m_pierres[m_joueurActuel] = nbPierres;
        m_pierresLabel->setText("Pierres : " + QString::number(nbPierres));
    }
}
