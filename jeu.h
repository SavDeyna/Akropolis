#ifndef JEU_H
#define JEU_H

#include <QWidget>
#include <QLabel>

class Jeu : public QWidget
{
    Q_OBJECT
public:
    explicit Jeu(QWidget *parent = nullptr);
    void initialiserAffichage(int nb, QStringList var, QStringList pseudos); // La fonction de mise à jour
private:
    QLabel *m_affichageInfos;
    void setupUI();
};

#endif // JEU_H
