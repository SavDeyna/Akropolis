#ifndef SELECJOUEURS_H
#define SELECJOUEURS_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>

class SelecJoueurs : public QWidget
{
    Q_OBJECT
public:
    explicit SelecJoueurs(QWidget *parent = nullptr);
    void updateSetup(int playerCount, int tuiles, QStringList variantes);
    QStringList getPseudos() const;

signals:
    void launchGame();
    void backToMenu();

private:
    QLabel *m_infoLabel;
    QVBoxLayout *choixVariantes;
    QVBoxLayout *m_layoutPseudos;
    QList<QLineEdit*> m_champsPseudos; 
};

#endif // SELECJOUEURS_H
