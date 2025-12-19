#include "selecsave.h"
#include <QVBoxLayout>

SelecSave::SelecSave(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void SelecSave::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_titre = new QLabel("<h2>Choisir une sauvegarde</h2>");
    m_titre->setAlignment(Qt::AlignCenter);

    // La liste des sauvegardes
    m_listSaves = new QListWidget(this);
    m_listSaves->setStyleSheet("QListWidget::item { height: 50px; }");

    // Bouton de validation
    m_btnCharger = new QPushButton("Charger la partie", this);
    m_btnCharger->setEnabled(false); // Désactivé tant qu'on n'a rien choisi

    layout->addWidget(m_titre);
    layout->addWidget(m_listSaves);
    layout->addWidget(m_btnCharger);

    // Activer le bouton seulement si une ligne est cliquée
    connect(m_listSaves, &QListWidget::itemClicked, [this]() {
        m_btnCharger->setEnabled(true);
    });
}


void SelecSave::initialiserAffichage(std::vector<SauvegardeInfo> listeSaves) {
    m_listSaves->clear(); // Vide la liste avant de la remplir

    for (const auto& info : listeSaves) {
        // On construit une chaîne avec tes champs : Nom, Mode et Tour
        QString ligne = QString("%1 | Mode : %2 | Tour : %3").arg(QString::fromStdString(info.nomSauvegarde)).arg(QString::fromStdString(info.nomMDJ)).arg(info.tour);

        QListWidgetItem *item = new QListWidgetItem(ligne, m_listSaves);

        // On stocke l'index JSON dans l'item (donnée invisible pour l'utilisateur)
        // Cela nous permettra de savoir quel élément charger plus tard.
        item->setData(Qt::UserRole, info.index);

        // Un peu de style pour le texte
        item->setFont(QFont("Arial", 10));
    }
}
