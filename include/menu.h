#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QCheckBox>

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    int getSelectedPlayerCount() const;
    QStringList getSelectedVariantes() const;
    int getTuilesSelected() const; // 0->defaut et 1->aleatoire
    bool estVarianteActive(const QString& nom) const {
        return m_variantesChoisies.contains(nom);
    }

signals:
    void playClicked();
    void chargerClicked();
    void quitClicked();

private slots:
    void onNbJoueursChanged(int id);
    void onTuilesChanged(int id);

private:
    void setupUI();
    QButtonGroup *m_nbJoueursGroup;
    QButtonGroup *m_tuilesChoixGroup;
    int m_selectedPlayers = 2;
    bool m_choixTuiles = 0;
    QStringList m_variantesChoisies;
    QCheckBox *m_varHabitations;
    QCheckBox *m_varMarches;
    QCheckBox *m_varCasernes;
    QCheckBox *m_varTemples;
    QCheckBox *m_varJardins;
};

#endif // MENU_H
