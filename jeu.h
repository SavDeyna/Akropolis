#ifndef JEU_H
#define JEU_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QVector>

class Jeu : public QWidget
{
    Q_OBJECT

public:
    explicit Jeu(QWidget *parent = nullptr);
    void initialiserAffichage(int nb, QStringList var, QStringList pseudos);

public slots:
    void mettreAJourPierres(int nbPierres);
    void joueurSuivant();
    void piocher();  // Ajout de la déclaration ici

private:
    QLabel *m_affichageInfos;
    QLabel *m_joueurActuelLabel;
    QLabel *m_pierresLabel;

    QPushButton *m_suivantButton;

    QStringList m_pseudos;
    int m_joueurActuel;

    QVector<int> m_pierres;  // Pour stocker les pierres de chaque joueur
};

#endif // JEU_H
