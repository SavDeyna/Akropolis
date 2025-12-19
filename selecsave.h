#ifndef SELECSAVE_H
#define SELECSAVE_H

#include "Sauvegarde.h"
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class SelecSave : public QWidget
{
    Q_OBJECT
public:
    explicit SelecSave(QWidget *parent = nullptr);
    void initialiserAffichage(std::vector<SauvegardeInfo> listeSaves);

signals:
    void saveSelected(int idSave); // Signal pour dire à MainWindow quelle save charger

private:
    void setupUI();
    QListWidget *m_listSaves;
    QPushButton *m_btnCharger;
    QLabel *m_titre;
};

#endif
