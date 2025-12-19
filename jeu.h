#pragma once
#include <QWidget>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QFrame>
#include "Plateau.h"

class TileView : public QGraphicsView {
    Q_OBJECT
public:
    TileView(int idx, QWidget *parent = nullptr) : QGraphicsView(parent), index(idx), isHighlighted(false) {
        updateStyle();
    }
    
    void setHighlighted(bool h) {
        isHighlighted = h;
        updateStyle();
    }
    
private:
    void updateStyle() {
        if (isHighlighted) {
            setStyleSheet("TileView { background: #2c2f36; border: 4px solid white; border-radius: 8px; }");
        } else {
            setStyleSheet("TileView { background: #2c2f36; border: 2px solid #2c2f36; border-radius: 8px; }");
        }
    }
    
signals:
    void tileSelected(int idx);
    
protected:
    void mousePressEvent(QMouseEvent *event) override {
        emit tileSelected(index);
        QGraphicsView::mousePressEvent(event);
    }
    
private:
    int index;
    bool isHighlighted;
};

class MyGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent = nullptr) : QGraphicsView(parent) {}
signals:
    void hexClicked(int q, int r);
    void hexRightClicked(int q, int r);
    void hexHovered(int q, int r);
    void mousePosUpdated(QPointF scenePos);
    void mouseLeft();
    void edgeScroll(int dx, int dy);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

class Jeu : public QWidget
{
    Q_OBJECT
public:
    explicit Jeu(QWidget *parent = nullptr);
    void initialiserAffichage(int nb, int tuiles, QStringList var, QStringList pseudos);
signals:
    void gameFinished();
private slots:
    void onHexClicked(int q, int r);
    void onHexRightClicked(int q, int r);
    void selectTile(int index = -1);
    void onMouseOverHex(int q, int r);
    void onMousePosUpdated(QPointF scenePos);
    void onEdgeScroll(int dx, int dy);
    void performScroll();
    void clearPreviewSlot();
private:
    QLabel *m_affichageInfos;
    void drawBoard(const Plateau& plateau);
    void setCurrentTiles(const QString& text);
    void drawTiles();
    void setPiocheSize(int size);
    void updatePlayerInfo();
    void drawPreview(int q, int r);
    void clearPreview();
    QLabel *currentTilesLabel;
    QLabel *piocheLabel;
    QLabel *playerInfoLabel;
    QLabel *piocheHeaderLabel;
    QFrame *piocheFrame;
    QGraphicsScene *scene;
    MyGraphicsView *view;
    QHBoxLayout *tilesLayout;
    std::vector<TileView*> tileViews;
    std::vector<QFrame*> tileFrames;
    QList<QGraphicsItem*> previewItems;
    int selectedTile{-1};
    int currentTileRotation{0};
    int lastHoverQ{0}, lastHoverR{0};
    QPointF lastMouseScenePos;
    QTimer* scrollTimer;
    int pendingScrollDx{0}, pendingScrollDy{0};
    void setupUI();
};

