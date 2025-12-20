#include "endscreen.h"
#include <QHeaderView>
#include <QFont>
#include <algorithm>

EndScreen::EndScreen(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void EndScreen::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(30);

    // Style global
    setStyleSheet(
        "QWidget { background: #1e90b4; }"
        "QLabel { color: #f3f6f8; font-family: 'Segoe UI'; }"
        "QTableWidget {"
        "   background: #2c2f36;"
        "   color: #f3f6f8;"
        "   border: 2px solid #44505c;"
        "   border-radius: 10px;"
        "   font: 14pt 'Segoe UI';"
        "   gridline-color: #44505c;"
        "}"
        "QTableWidget::item {"
        "   padding: 10px;"
        "   border-bottom: 1px solid #44505c;"
        "}"
        "QTableWidget::item:selected {"
        "   background: #3d4756;"
        "}"
        "QHeaderView::section {"
        "   background: #44505c;"
        "   color: #f3f6f8;"
        "   padding: 12px;"
        "   border: none;"
        "   font-weight: bold;"
        "   font-size: 15pt;"
        "}"
        "QPushButton {"
        "   background: #2c2f36;"
        "   color: #f3f6f8;"
        "   border: 2px solid #44505c;"
        "   border-radius: 8px;"
        "   padding: 15px 30px;"
        "   font: 14pt 'Segoe UI';"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background: #3d4756;"
        "   border-color: #5a6575;"
        "}"
        "QPushButton:pressed {"
        "   background: #252830;"
        "}"
    );

    // Titre
    m_titleLabel = new QLabel("Fin de la partie");
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(32);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);

    // Table des scores
    m_scoresTable = new QTableWidget(this);
    m_scoresTable->setColumnCount(4);
    m_scoresTable->setHorizontalHeaderLabels({"Position", "Joueur", "Points", "Pierres"});
    m_scoresTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_scoresTable->verticalHeader()->setVisible(false);
    m_scoresTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_scoresTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_scoresTable->setMinimumHeight(300);
    mainLayout->addWidget(m_scoresTable, 1);

    // Bouton retour au menu
    m_retourMenuButton = new QPushButton("Retour au menu");
    m_retourMenuButton->setFixedSize(300, 70);
    mainLayout->addWidget(m_retourMenuButton, 0, Qt::AlignCenter);

    // Connexion du signal
    connect(m_retourMenuButton, &QPushButton::clicked, this, &EndScreen::retourMenuClicked);
}

void EndScreen::afficherScores(const std::vector<PlayerScore>& scores)
{
    // Trier les scores par ordre décroissant
    std::vector<PlayerScore> sortedScores = scores;
    std::sort(sortedScores.begin(), sortedScores.end(),
        [](const PlayerScore& a, const PlayerScore& b) {
            return a.score > b.score;
        });

    m_scoresTable->setRowCount(sortedScores.size());

    for (size_t i = 0; i < sortedScores.size(); ++i) {
        // Position
        QTableWidgetItem *posItem = new QTableWidgetItem(QString::number(i + 1));
        posItem->setTextAlignment(Qt::AlignCenter);
        posItem->setFont(QFont("Segoe UI", 14, QFont::Bold));
        
        // Pseudo
        QTableWidgetItem *pseudoItem = new QTableWidgetItem(sortedScores[i].pseudo);
        pseudoItem->setTextAlignment(Qt::AlignCenter);
        pseudoItem->setFont(QFont("Segoe UI", 14));
        
        // Score
        QTableWidgetItem *scoreItem = new QTableWidgetItem(QString::number(sortedScores[i].score));
        scoreItem->setTextAlignment(Qt::AlignCenter);
        scoreItem->setFont(QFont("Segoe UI", 14, QFont::Bold));
        
        // Pierres
        QTableWidgetItem *pierresItem = new QTableWidgetItem(QString::number(sortedScores[i].pierres));
        pierresItem->setTextAlignment(Qt::AlignCenter);
        pierresItem->setFont(QFont("Segoe UI", 14));

        // Mettre en évidence le gagnant (première ligne)
        if (i == 0) {
            posItem->setBackground(QBrush(QColor(255, 215, 0, 100))); // Or translucide
            pseudoItem->setBackground(QBrush(QColor(255, 215, 0, 100)));
            scoreItem->setBackground(QBrush(QColor(255, 215, 0, 100)));
            pierresItem->setBackground(QBrush(QColor(255, 215, 0, 100)));
        }

        m_scoresTable->setItem(i, 0, posItem);
        m_scoresTable->setItem(i, 1, pseudoItem);
        m_scoresTable->setItem(i, 2, scoreItem);
        m_scoresTable->setItem(i, 3, pierresItem);
    }

    // Ajuster la hauteur des lignes pour un meilleur espacement
    for (int i = 0; i < m_scoresTable->rowCount(); ++i) {
        m_scoresTable->setRowHeight(i, 60);
    }
}
