#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup> // Ajout de ce composant essentiel

class Menu : public QWidget // <<<<<< CORRECTION CRUCIALE : Doit hériter de QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);

    // Méthodes d'accès pour que MainWindow puisse lire les choix faits
    int getSelectedPlayerCount() const;
    QStringList getSelectedVariantes() const;

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
    // ... (Déclarez les QRadioButtons, QCheckboxes, et QLabels ici)

    // Exemples des widgets que vous devez déclarer pour les utiliser dans setupUI():
    // QPushButton *m_playButton;
    // QLabel *m_titleLabel;
};

#endif // MENU_H
