#include "jeu.h"
#include <QVBoxLayout>

Jeu::Jeu(QWidget *parent) : QWidget(parent) {
    // 1. On crée un layout pour organiser la page
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 2. On crée le label qui accueillera toutes les infos
    m_affichageInfos = new QLabel(this);
    m_affichageInfos->setAlignment(Qt::AlignCenter); // On centre le texte

    // On peut lui donner un style par défaut
    m_affichageInfos->setStyleSheet("font-size: 16px;");

    layout->addWidget(m_affichageInfos);
}

void Jeu::initialiserAffichage(int nb, QStringList var, QStringList pseudos) {
    // 3. On construit une chaîne de caractères plus riche
    QString texte = "<h2>Configuration de la Partie</h2><br>";

    texte += QString("<b>Nombre de joueurs :</b> %1<br><br>").arg(nb);

    texte += "<b>Liste des joueurs :</b><br>";
    texte += pseudos.join(" - ") + "<br><br>"; // "Joueur 1 - Joueur 2..."

    texte += "<b>Variantes choisies :</b><br>";
    if (var.isEmpty()) {
        texte += "Aucune";
    } else {
        texte += var.join(", ");
    }

    // 4. On applique le texte final (le QLabel comprend le HTML de base)
    m_affichageInfos->setText(texte);
}
