#ifndef JEU_H
#define JEU_H

#include <QWidget>
#include <QLabel>

class Jeu : public QWidget
{
    Q_OBJECT
public:
    explicit Jeu(QWidget *parent = nullptr);
    void initialiserAffichage(int nb, int tuiles, QStringList var, QStringList pseudos);
private:
    QLabel *m_affichageInfos;
};

#endif // JEU_H
