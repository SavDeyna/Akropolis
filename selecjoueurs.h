#ifndef SELECJOUEURS_H
#define SELECJOUEURS_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class SelecJoueurs : public QWidget
{
    Q_OBJECT
public:
    explicit SelecJoueurs(QWidget *parent = nullptr);
    void updateSetup(int playerCount, QStringList variantes);

signals:
    void launchGame();
    void backToMenu();

private:
    QLabel *m_infoLabel;
};

#endif // SELECJOUEURS_H
