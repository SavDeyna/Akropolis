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

    // Méthodes d'accès pour que MainWindow puisse lire les choix faits
    int getSelectedPlayerCount() const;
    QStringList getSelectedVariantes() const;

    // Une petite fonction utilitaire pour ton moteur
    bool estVarianteActive(const QString& nom) const {
        return m_variantesChoisies.contains(nom);
    }

signals:
    // Signaux émis pour notifier le contrôleur (MainWindow)
    void playClicked();
    void chargerClicked();
    void quitClicked();

private slots:
    // Seul le slot pour stocker le choix du groupe de boutons est nécessaire en interne
    void onNbJoueursChanged(int id);

private:
    void setupUI(); // Fonction interne pour construire l'interface (appellée par le constructeur)

    // Membres UI/Logique de cette page
    QButtonGroup *m_nbJoueursGroup;
    int m_selectedPlayers = 2;
    QStringList m_variantesChoisies;
    QCheckBox *m_varHabitations;
    QCheckBox *m_varMarches;
    QCheckBox *m_varCasernes;
    QCheckBox *m_varTemples;
    QCheckBox *m_varJardins;
    // ... (Déclarez les QRadioButtons, QCheckboxes, et QLabels ici)

    // Exemples des widgets que vous devez déclarer pour les utiliser dans setupUI():
    // QPushButton *m_playButton;
    // QLabel *m_titleLabel;
};

#endif // MENU_H
