#include "jeu.h"
#include <QHBoxLayout>
#include <QGraphicsPolygonItem>
#include <cmath>
#include <QWheelEvent>
#include <QMessageBox>
#include <QScrollBar>
#include <QPainterPath>
#include <algorithm>
#include <unordered_map>
#include "Partie.h"

// Fonction utilitaire pour convertir TypeHexagone en QColor
static QColor getColorForType(TypeHexagone type) {
    switch (type) {
        case TypeHexagone::Carriere: return Qt::gray;
        case TypeHexagone::Caserne: return Qt::red;
        case TypeHexagone::Jardin: return Qt::green;
        case TypeHexagone::Temple: return Qt::magenta;
        case TypeHexagone::Marche: return Qt::yellow;
        case TypeHexagone::Habitation: return Qt::blue;
        default: return Qt::white;
    }
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = mapToScene(event->pos());
    double size = 30.0;
    double x = scenePos.x();
    double y = scenePos.y();
    double q = (2.0 * x) / (3.0 * size);
    double r = (y / (size * sqrt(3.0))) - q / 2.0;
    int iq = round(q);
    int ir = round(r);
    int is = -(iq + ir);
    if (event->button() == Qt::LeftButton) {
        emit hexClicked(iq, ir);
    } else if (event->button() == Qt::RightButton) {
        emit hexRightClicked(iq, ir);
    }
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    QPointF scenePos = mapToScene(event->pos());
    emit mousePosUpdated(scenePos);
    
    // Edge scrolling detection
    int dx = 0, dy = 0;
    if (event->pos().x() < 50) dx = -1;
    else if (event->pos().x() > width() - 50) dx = 1;
    if (event->pos().y() < 50) dy = -1;
    else if (event->pos().y() > height() - 50) dy = 1;
    
    // Always emit to update state (even if 0,0 to stop scrolling)
    emit edgeScroll(dx, dy);
    
    double size = 30.0;
    double x = scenePos.x();
    double y = scenePos.y();
    double q = (2.0 * x) / (3.0 * size);
    double r = (y / (size * sqrt(3.0))) - q / 2.0;
    int iq = round(q);
    int ir = round(r);
    emit hexHovered(iq, ir);
}

void MyGraphicsView::wheelEvent(QWheelEvent *event) {
    QPoint mousePos = mapFromGlobal(QCursor::pos());
    QPointF scenePos = mapToScene(mousePos);
    double size = 30.0;
    double x = scenePos.x();
    double y = scenePos.y();
    double q = (2.0 * x) / (3.0 * size);
    double r = (y / (size * sqrt(3.0))) - q / 2.0;
    int iq = round(q);
    int ir = round(r);
    emit hexHovered(iq, ir);
    QGraphicsView::wheelEvent(event);
}

void MyGraphicsView::leaveEvent(QEvent *event) {
    emit mouseLeft();
    emit edgeScroll(0, 0);  // Stop scrolling when mouse leaves
    QGraphicsView::leaveEvent(event);
}

Jeu::Jeu(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void Jeu::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    // Global style
    setStyleSheet(
        "QWidget { background: #1e90b4; color: #f3f6f8; font: 12pt 'Segoe UI'; }"
        "QLabel { color: #f3f6f8; }"
        "QFrame#piocheFrame { background: #2c2f36; border-radius: 10px; padding: 12px; }"
        "QGraphicsView { background: #2c2f36; border: 1px solid #44505c; border-radius: 10px; }"
    );

    // 2. On crée le label qui accueillera toutes les infos
    m_affichageInfos = new QLabel(this);
    m_affichageInfos->setAlignment(Qt::AlignCenter);
    m_affichageInfos->setStyleSheet("font-size: 16px;");
    m_affichageInfos->hide(); // Hide the info display

    // Graphics view for board
    scene = new QGraphicsScene(this);
    view = new MyGraphicsView(this);
    view->setScene(scene);
    view->setMinimumSize(600, 400);
    view->setMouseTracking(true);

    currentTilesLabel = new QLabel("Current tuiles: none");
    currentTilesLabel->hide();

    piocheLabel = new QLabel("Pioche: ");
    playerInfoLabel = new QLabel("Player info");

    // Top info bar
    QHBoxLayout *infoBar = new QHBoxLayout();
    infoBar->setSpacing(16);
    infoBar->addWidget(piocheLabel);
    infoBar->addStretch();
    infoBar->addWidget(playerInfoLabel);

    // Pioche container
    piocheFrame = new QFrame(this);
    piocheFrame->setObjectName("piocheFrame");
    QVBoxLayout *piocheLayout = new QVBoxLayout(piocheFrame);
    piocheLayout->setContentsMargins(8, 8, 8, 8);
    piocheLayout->setSpacing(8);
    piocheHeaderLabel = new QLabel("Chantier", piocheFrame);
    piocheHeaderLabel->setStyleSheet("font-weight: 600; letter-spacing: 0.5px; background: transparent;");
    tilesLayout = new QHBoxLayout();
    tilesLayout->setSpacing(12);
    tilesLayout->setContentsMargins(0, 0, 0, 0);
    piocheLayout->addWidget(piocheHeaderLabel);
    piocheLayout->addLayout(tilesLayout);

    layout->addWidget(m_affichageInfos);
    layout->addWidget(currentTilesLabel);
    layout->addLayout(infoBar);
    layout->addWidget(piocheFrame, 0);
    layout->addWidget(view, 1);

    // Setup scroll timer for smooth, controlled scrolling
    scrollTimer = new QTimer(this);
    scrollTimer->setInterval(100);  // Scroll every 100ms
    connect(scrollTimer, &QTimer::timeout, this, &Jeu::performScroll);

    connect(view, &MyGraphicsView::hexClicked, this, &Jeu::onHexClicked);
    connect(view, &MyGraphicsView::hexRightClicked, this, &Jeu::onHexRightClicked);
    connect(view, &MyGraphicsView::hexHovered, this, &Jeu::onMouseOverHex);
    connect(view, &MyGraphicsView::mousePosUpdated, this, &Jeu::onMousePosUpdated);
    connect(view, &MyGraphicsView::mouseLeft, this, &Jeu::clearPreviewSlot);
    connect(view, &MyGraphicsView::edgeScroll, this, &Jeu::onEdgeScroll);
}

void Jeu::initialiserAffichage(int nb, int tuiles, QStringList var, QStringList pseudos) {
    // 3. On construit une chaîne de caractères plus riche
    QString texte = "<h2>Configuration de la Partie</h2><br>";

    texte += QString("<b>Nombre de joueurs :</b> %1<br><br>").arg(nb);

    texte += "<b>Liste des joueurs :</b><br>";
    texte += pseudos.join(" - ") + "<br><br>"; // "Joueur 1 - Joueur 2..."

    texte += "<b>Variantes choisies :</b><br>";
    if (var.isEmpty()) {
        texte += "Aucune";
    } else {
        texte += var.join(", ");
    }

    texte += "<br><br><b>Tuiles: </b>";
        if (tuiles == 0) {
        texte += "défaut <br>";
    } else {
        texte += "aléatoire <br>";
    }

    // 4. On applique le texte final (le QLabel comprend le HTML de base)
    m_affichageInfos->setText(texte);
}

void Jeu::drawBoard(const Plateau& plateau) {
    scene->clear();
    double size = 30.0;
    double pi = acos(-1.0);
    auto grille = plateau.getGrille();
    
    // Grouper les hexagones par tuile
    std::map<int, std::vector<HexagoneCoord>> tuileHexagones;
    for (const auto& [coord, state] : grille) {
        tuileHexagones[state.id_tuile].push_back(coord);
    }
    
    // D'abord dessiner tous les hexagones remplis avec bordure fine
    for (const auto& [coord, state] : grille) {
        double x = size * (1.5 * coord.q);
        double y = size * (sqrt(3)/2 * coord.q + sqrt(3) * coord.r);
        
        QPolygonF hex;
        for (int i = 0; i < 6; ++i) {
            double angle = pi / 3 * i;
            double px = x + size * cos(angle);
            double py = y + size * sin(angle);
            hex << QPointF(px, py);
        }
        QGraphicsPolygonItem *item = new QGraphicsPolygonItem(hex);
        QColor color = getColorForType(state.type);
        item->setBrush(color);
        // Bordure fine grise pour tous les hexagones
        item->setPen(QPen(QColor(60, 60, 60), 1));
        scene->addItem(item);
    }
    
    // Dessiner le contour épais de chaque tuile
    for (const auto& [tileId, hexCoords] : tuileHexagones) {
        // Créer un set pour vérifier rapidement si un hex appartient à cette tuile
        std::set<std::pair<int,int>> tileHexSet;
        for (const auto& c : hexCoords) {
            tileHexSet.insert({c.q, c.r});
        }
        
        // Directions des 6 voisins en coordonnées axiales
        // L'ordre correspond aux côtés de l'hexagone graphique (flat-top, angle 0 = droite)
        static const int NEIGHBOR_DIRS[6][2] = {
            {+1,  0}, // côté 0 (droite) → voisin E
            {+1, -1}, // côté 1 (haut-droite) → voisin NE
            { 0, -1}, // côté 2 (haut-gauche) → voisin NW
            {-1,  0}, // côté 3 (gauche) → voisin W
            {-1, +1}, // côté 4 (bas-gauche) → voisin SW
            { 0, +1}  // côté 5 (bas-droite) → voisin SE
        };
        
        // Pour chaque hexagone de cette tuile
        for (const auto& coord : hexCoords) {
            double x = size * (1.5 * coord.q);
            double y = size * (sqrt(3)/2 * coord.q + sqrt(3) * coord.r);
            
            // Pour chaque côté
            for (int side = 0; side < 6; ++side) {
                int nq = coord.q + NEIGHBOR_DIRS[side][0];
                int nr = coord.r + NEIGHBOR_DIRS[side][1];
                
                // Si le voisin n'est pas dans la même tuile → bordure externe
                if (tileHexSet.find({nq, nr}) == tileHexSet.end()) {
                    // Calculer les deux sommets de ce côté
                    double angle1 = pi / 3 * side;
                    double angle2 = pi / 3 * ((side + 1) % 6);
                    QPointF p1(x + size * cos(angle1), y + size * sin(angle1));
                    QPointF p2(x + size * cos(angle2), y + size * sin(angle2));
                    
                    QPen pen(Qt::black, 3);
                    pen.setCapStyle(Qt::FlatCap);
                    QGraphicsLineItem* line = new QGraphicsLineItem(p1.x(), p1.y(), p2.x(), p2.y());
                    line->setPen(pen);
                    scene->addItem(line);
                }
            }
        }
    }
    
    // Enfin afficher les étoiles (au-dessus des bordures)
    for (const auto& [coord, state] : grille) {
        if (state.place) {
            double x = size * (1.5 * coord.q);
            double y = size * (sqrt(3)/2 * coord.q + sqrt(3) * coord.r);
            
            QGraphicsTextItem* star = new QGraphicsTextItem("★");
            star->setDefaultTextColor(Qt::white);
            QFont font = star->font();
            font.setPointSize(14);
            font.setBold(true);
            star->setFont(font);
            QRectF bounds = star->boundingRect();
            star->setPos(x - bounds.width() / 2, y - bounds.height() / 2);
            scene->addItem(star);
        }
    }
}

void Jeu::setCurrentTiles(const QString& text) {
    currentTilesLabel->setText(text);
}

void Jeu::setPiocheSize(int size) {
    piocheLabel->setText(QString("Pioche: %1 tuiles").arg(size));
}

void Jeu::updatePlayerInfo() {
    Partie& partie = Partie::getInstance();
    auto& participants = partie.getParticipants();
    if (!participants.empty()) {
        Participation& p = partie.getCurrentPlayer();
        QString pseudo = QString::fromStdString(p.getParticipant().getPseudo());
        
        // Add "Architecte en chef" title for player with ordre 1
        if (p.getOrdrePassage() == 1) {
            pseudo += " (Architecte en chef)";
        }
        
        QString info = QString("%1, Points: %2, Pierres: %3").arg(pseudo, QString::number(p.getPoints()), QString::number(p.getPierres()));
        playerInfoLabel->setText(info);
    }
}

void Jeu::drawTiles() {
    // clear previous
    for(auto view : tileViews) {
        tilesLayout->removeWidget(view);
        delete view->scene();
        delete view;
    }
    for(auto frame : tileFrames) {
        tilesLayout->removeWidget(frame);
        delete frame;
    }
    tileViews.clear();
    tileFrames.clear();

    Partie& partie = Partie::getInstance();
    auto& jeu = partie.getJeu();
    for(size_t i = 0; i < jeu.size(); ++i) {
        const Tuile& tile = jeu[i];
        
        // Create frame container
        QFrame* tileFrame = new QFrame(this);
        tileFrame->setFixedSize(118, 128);
        tileFrame->setStyleSheet("QFrame { background: #2c2f36; border: 4px solid #2c2f36; }");
        QVBoxLayout* frameLayout = new QVBoxLayout(tileFrame);
        frameLayout->setContentsMargins(0, 0, 0, 0);
        frameLayout->setSpacing(0);
        
        QGraphicsScene *tileScene = new QGraphicsScene(this);
        TileView *tileView = new TileView(i, this);
        tileView->setScene(tileScene);
        tileView->setFixedSize(110, 110);
        tileView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tileView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        
        // draw the tile's hexagons
        double size = 15.0;
        double pi_val = acos(-1.0);
        for(const Hexagone& hex : tile.getDisposition()) {
            int q = hex.getQ(), r = hex.getR();
            double x = size * (1.5 * q);
            double y = size * (sqrt(3)/2 * q + sqrt(3) * r);
            QPolygonF hexPoly;
            for (int j = 0; j < 6; ++j) {
                double angle = pi_val / 3 * j;
                double px = x + size * cos(angle);
                double py = y + size * sin(angle);
                hexPoly << QPointF(px, py);
            }
            QGraphicsPolygonItem *item = new QGraphicsPolygonItem(hexPoly);
            QColor color = getColorForType(hex.getTypeHexagone());
            item->setBrush(color);
            item->setPen(QPen(Qt::black));
            tileScene->addItem(item);
            
            // Afficher une étoile sur les places
            if (hex.isPlace()) {
                QGraphicsTextItem* star = new QGraphicsTextItem("★");
                star->setDefaultTextColor(Qt::white);
                QFont starFont = star->font();
                starFont.setPointSize(8);
                starFont.setBold(true);
                star->setFont(starFont);
                QRectF bounds = star->boundingRect();
                star->setPos(x - bounds.width() / 2, y - bounds.height() / 2);
                tileScene->addItem(star);
            }
        }
        // Add cost label
        QGraphicsTextItem* costLabel = new QGraphicsTextItem(QString::number(i));
        costLabel->setDefaultTextColor(Qt::white);
        QFont font = costLabel->font();
        font.setBold(true);
        font.setPointSize(11);
        costLabel->setFont(font);
        costLabel->setPos(65, 70); // bottom-right corner
        tileScene->addItem(costLabel);
        
        frameLayout->addWidget(tileView);
        tilesLayout->addWidget(tileFrame);
        tileViews.push_back(tileView);
        tileFrames.push_back(tileFrame);
        connect(tileView, &TileView::tileSelected, this, &Jeu::selectTile);
    }
}

void Jeu::selectTile(int index) {
    Partie& partie = Partie::getInstance();
    auto& participants = partie.getParticipants();
    if (participants.empty()) return;
    Participation& participation = partie.getCurrentPlayer();
    if (participation.getPierres() < (unsigned int)index) {
        return;
    }
    // clear previous highlights
    for (auto frame : tileFrames) {
        frame->setStyleSheet("QFrame { background: #2c2f36; border: 4px solid #2c2f36; }");
    }
    if (selectedTile == index) {
        // deselect
        selectedTile = -1;
        clearPreview();
    } else {
        selectedTile = index;
        if (index >= 0 && index < (int)tileFrames.size()) {
            tileFrames[index]->setStyleSheet("QFrame { background: #2c2f36; border: 4px solid white; }");
        }
    }
}

void Jeu::onHexClicked(int q, int r) {
    clearPreview();
    if(selectedTile == -1) return;
    Partie& partie = Partie::getInstance();
    auto& jeu = partie.getJeu();
    if(selectedTile >= jeu.size()) return;
    Tuile& tile = jeu[selectedTile];
    auto& participants = partie.getParticipants();
    if (participants.empty()) return;
    
    // Get current player (ordre 1 = Architecte en chef)
    Participation& participation = partie.getCurrentPlayer();
    Plateau& plateau = participation.getPlateau();
    unsigned int cost = selectedTile;  // Cost is the index
    
    // Check if player has enough stones
    if (participation.getPierres() < cost) {
        return;  // Not enough stones
    }
    
    unsigned int nbPierres = 0;  // Will be incremented by placerTuile for each carriere covered
    HexagoneCoord origin{q, r, -(q + r)};
    if (plateau.placerTuile(tile, origin, nbPierres)) {
        // Pay the cost (index = cost) and add stones from covered carrieres
        participation.setPierres(participation.getPierres() - cost + nbPierres);
        
        // Remove the tile from jeu - this automatically updates costs for remaining tiles
        jeu.erase(jeu.begin() + selectedTile);
        
        // Check for end of game (only 1 tile total left)
        if (partie.getPiocheSize() + jeu.size() == 1) {
            // End of game - emit signal to show end screen
            emit gameFinished();
            return;
        }
        
        // Check if we need to refill the pioche
        if (jeu.size() == 1) {
            partie.refillJeu();
            // rotateArchitecte() is called inside refillJeu()
        } else {
            // Move to next player: ALL players rotate their order
            for (auto& p : participants) {
                p.prochainOrdrePassage(partie.getNbParticipants());
            }
        }
        
        selectedTile = -1;
        Participation& currentPlayer = partie.getCurrentPlayer();
        drawBoard(currentPlayer.getPlateau());
        
        // Reset camera to center
        view->centerOn(0, 0);
        
        drawTiles();
        setPiocheSize(partie.getPiocheSize());
        updatePlayerInfo();
        QString tilesText = "Current tiles: ";
        for (const auto& t : jeu) {
            tilesText += QString::number(t.getId()) + " ";
        }
        setCurrentTiles(tilesText);
        if (jeu.empty()) {
            partie.finTour();
            partie.debutTour();
            drawBoard(plateau);
            drawTiles();
            setPiocheSize(partie.getPiocheSize());
            updatePlayerInfo();
            auto& newJeu = partie.getJeu();
            QString newTilesText = "Current tiles: ";
            for (const auto& t : newJeu) {
                newTilesText += QString::number(t.getId()) + " ";
            }
            setCurrentTiles(newTilesText);
        }
    } else {
        // Invalid placement, do nothing
    }
}

void Jeu::onMouseOverHex(int q, int r) {
    lastHoverQ = q;
    lastHoverR = r;
    if (selectedTile != -1) {
        drawPreview(q, r);
    } else {
        clearPreview();
    }
}

void Jeu::drawPreview(int q, int r) {
    clearPreview();
    Partie& partie = Partie::getInstance();
    auto& jeu = partie.getJeu();
    if (selectedTile >= jeu.size()) return;
    const Tuile& tile = jeu[selectedTile];
    double size = 30.0;
    double pi_val = acos(-1.0);
    for (const Hexagone& hex : tile.getDisposition()) {
        int hq = q + hex.getQ();
        int hr = r + hex.getR();
        double x = size * (1.5 * hq);
        double y = size * (sqrt(3)/2 * hq + sqrt(3) * hr);
        QPolygonF hexPoly;
        for (int j = 0; j < 6; ++j) {
            double angle = pi_val / 3 * j;
            double px = x + size * cos(angle);
            double py = y + size * sin(angle);
            hexPoly << QPointF(px, py);
        }
        QGraphicsPolygonItem *item = new QGraphicsPolygonItem(hexPoly);
        QColor color = getColorForType(hex.getTypeHexagone());
        // Use a diagonal pattern to show this is a preview while still showing the tile
        item->setBrush(QBrush(color, Qt::BDiagPattern));
        item->setPen(QPen(Qt::red, 2));
        scene->addItem(item);
        previewItems << item;
        
        // Afficher une étoile sur les places (en preview aussi)
        if (hex.isPlace()) {
            QGraphicsTextItem* star = new QGraphicsTextItem("★");
            star->setDefaultTextColor(Qt::white);
            QFont starFont = star->font();
            starFont.setPointSize(14);
            starFont.setBold(true);
            star->setFont(starFont);
            QRectF bounds = star->boundingRect();
            star->setPos(x - bounds.width() / 2, y - bounds.height() / 2);
            scene->addItem(star);
            previewItems << star;
        }
    }
}

void Jeu::clearPreview() {
    for (auto item : previewItems) {
        scene->removeItem(item);
        delete item;
    }
    previewItems.clear();
}

void Jeu::clearPreviewSlot() {
    clearPreview();
}

void Jeu::onHexRightClicked(int q, int r) {
    if (selectedTile == -1) return;
    
    Partie& partie = Partie::getInstance();
    auto& jeu = partie.getJeu();
    if (selectedTile >= jeu.size()) return;
    
    Tuile& tile = jeu[selectedTile];
    tile.tournerDroite();
    currentTileRotation++;
    
    // Clear preview first
    clearPreview();
    
    // Redraw using the current hover position (lastHoverQ, lastHoverR)
    // This ensures the tile stays centered exactly under the mouse
    drawPreview(lastHoverQ, lastHoverR);
}

void Jeu::onEdgeScroll(int dx, int dy) {
    pendingScrollDx = dx;
    pendingScrollDy = dy;
    
    if (!scrollTimer->isActive()) {
        scrollTimer->start();
    }
}

void Jeu::performScroll() {
    if (pendingScrollDx == 0 && pendingScrollDy == 0) {
        scrollTimer->stop();
        return;
    }
    
    QScrollBar* hScrollBar = view->horizontalScrollBar();
    QScrollBar* vScrollBar = view->verticalScrollBar();
    
    if (hScrollBar) {
        hScrollBar->setValue(hScrollBar->value() + pendingScrollDx * 15);
    }
    if (vScrollBar) {
        vScrollBar->setValue(vScrollBar->value() + pendingScrollDy * 15);
    }
}

void Jeu::onMousePosUpdated(QPointF scenePos) {
    lastMouseScenePos = scenePos;
}
