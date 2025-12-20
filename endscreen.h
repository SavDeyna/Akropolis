#ifndef ENDSCREEN_H
#define ENDSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <vector>
#include <QString>

struct PlayerScore {
    QString pseudo;
    unsigned int score;
    unsigned int pierres;
};

class EndScreen : public QWidget
{
    Q_OBJECT

public:
    explicit EndScreen(QWidget *parent = nullptr);
    void afficherScores(const std::vector<PlayerScore>& scores);

signals:
    void retourMenuClicked();

private:
    void setupUI();
    
    QLabel *m_titleLabel;
    QTableWidget *m_scoresTable;
    QPushButton *m_retourMenuButton;
};

#endif
