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
    void updateSetup(int playerCount, QStringList variantes);
    QStringList getPseudos() const;

signals:
    void launchGame();
    void backToMenu();

private:
    QLabel *m_infoLabel;
    QVBoxLayout *choixVariantes;
    QVBoxLayout *m_layoutPseudos; // Le layout où on va afficher les QLineEdit
    QList<QLineEdit*> m_champsPseudos; // La liste pour stocker les objets
};

#endif // SELECJOUEURS_H
