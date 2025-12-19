#include "Plateau.h"
#include "Partie.h"   // pour ModeDeJeu / Variante

#include <queue>
#include <set>
#include <algorithm>
#include <iostream>
#include <climits>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;


Plateau::Plateau() {
    Tuile depart(0, true);
    HexagoneCoord origin{0, 0, 0};

    for (const auto& h : depart.getDisposition()) {
        HexagoneCoord pos {
            origin.q + h.getQ(),
            origin.r + h.getR(),
            origin.s + h.getS()
        };

        HexState st;
        st.type = h.getTypeHexagone();
        st.hauteur = 1;
        st.id_tuile = depart.getId();
        st.place = h.isPlace();

        grille[pos] = st;
    }
}

const HexState* Plateau::getHex(const HexagoneCoord& c) const {
    auto it = grille.find(c);
    return (it == grille.end()) ? nullptr : &it->second;
}

bool Plateau::estOccupe(const HexagoneCoord& c) const {
    return grille.find(c) != grille.end();
}

vector<HexagoneCoord> Plateau::getVoisins(const HexagoneCoord& c) const {
    static const int d[6][3] = {
        {+1,-1,0}, {+1,0,-1}, {0,+1,-1},
        {-1,+1,0}, {-1,0,+1}, {0,-1,+1}
    };
    vector<HexagoneCoord> v;
    for (auto& x : d) {
        HexagoneCoord nc{c.q + x[0], c.r + x[1], c.s + x[2]};
        if (grille.find(nc) != grille.end())
            v.push_back(nc);
    }
    return v;
}

bool Plateau::placerTuile(Tuile& t, const HexagoneCoord& origin, unsigned int& nbPierres, bool interactive) {
    std::vector<HexagoneCoord> coords;
    coords.reserve(3); // pour éviter une surallocation de mémoire


    for (const auto& h: t.getDisposition()) {
        // h.getQ() et getR() sont relatifs → on les additionne à origin
        HexagoneCoord pos {
            origin.q + h.getQ(),
            origin.r + h.getR(),
            origin.s + h.getS()
        };
        coords.push_back(pos);
    }

    // vérifions s'il y a déjà des hexa sur le plateau aux coords indiquées
    const HexState* hex1 = getHex(coords[0]);
    const HexState* hex2 = getHex(coords[1]);
    const HexState* hex3 = getHex(coords[2]);

    bool allEmpty = (!hex1 && !hex2 && !hex3);
    bool allPresent = (hex1 && hex2 && hex3);

    if (!allEmpty && !allPresent) {
        return false;
    }
    int h = 1;

    if (allPresent){
        if (!(hex1->hauteur == hex2->hauteur && hex2->hauteur == hex3->hauteur)) {
            return false;  // les hexa n'ont pas la même hauteur, la tuile ne peut pas être placée
        } 
        if (hex1->id_tuile == hex2->id_tuile && hex2->id_tuile == hex3->id_tuile) {
            return false; // la nouvelle tuile au niveau supérieur n'est pas à cheval sur au moins 2 tuiles différentes
        } 
        h = hex1->hauteur + 1;
        t.setHauteur(h);

        for (const auto* hex : {hex1, hex2, hex3}) {
            if (hex->type == TypeHexagone::Carriere)
                nbPierres++;
        }
    } else if (allEmpty) {
        if (h == 1){
            int i{0};
            vector<HexagoneCoord> voisins;
            while (i < 3) {
                voisins = getVoisins(coords[i]);
                if (!voisins.empty())
                    break; 
                i++;
            } 
            if (i == 3) {
                return false; // la nouvelle tuile n'est pas adjacente à une autre tuile de la Cité
            } 
        }
    } 
    
    for (size_t i = 0; i < coords.size(); ++i) {
        const Hexagone& hex10 = t.getDisposition()[i];
        const HexagoneCoord& position = coords[i];
        HexState st;

        st.type = hex10.getTypeHexagone();
        st.hauteur = h;
        st.id_tuile = t.getId();
        st.place = hex10.isPlace();
        grille[position] = st;
    }
    //=========
    return true;
}

void Plateau::afficherPlateau() const {
    std::cout << "=== Grille du plateau ===\n";
    if (grille.empty()) {
        std::cout << "(vide)\n";
        return;
    }
    for (const auto& [coord, st] : grille) {
        std::cout << "Hex (" << coord.q << "," << coord.r << "," << coord.s << ")"
                  << " type=" << static_cast<int>(st.type)
                  << " hauteur=" << st.hauteur
                  << " id_tuile=\"" << st.id_tuile << "\"\n";
    }
}

void Plateau::dessinerPlateau(const int radius) const{
    // on utilise 2 lignes de texte pour représenter une rangée d'hexagones
    vector<stringstream> lignes(2);
    for(int r = -radius; r <= radius; ++r){
        // Reset des stringstreams
        for(size_t i = 0; i < lignes.size(); ++i) { 
            lignes[i].str("");
            lignes[i].clear();
        }
        // calcul padding
        int abs_r = abs(r);
        string padding(abs_r * 4, ' '); 
        lignes[0] << padding;
        lignes[1] << padding;
        for(int q = -radius; q <= radius; ++q){
            int s = -q - r;
            // on ne dessine que si les coordonnées sont valides
            if (s >= -radius && s <= radius) {
                // récupération de l'hex
                auto it = grille.find(HexagoneCoord{q, r, s});
                const HexState * hexS = (it == grille.end()) ? nullptr : &it->second;
                string desc = " "; 
                string coords = " ";
                string typeString = "?";
                if (hexS != nullptr) {
                    // Si l'hexagone existe, on peut accéder à ses membres en toute sécurité
                    switch(hexS->type){
                        case TypeHexagone::Carriere: typeString = "C"; break;
                        case TypeHexagone::Caserne: typeString = "S"; break;
                        case TypeHexagone::Jardin: typeString = "J"; break;
                        case TypeHexagone::Temple: typeString = "T"; break;
                        case TypeHexagone::Marche: typeString = "M"; break;
                        case TypeHexagone::Habitation: typeString = "H"; break;
                        default: typeString = "?"; break;
                    }
                    desc = to_string(hexS->place) + typeString + to_string(hexS->hauteur);
                }

                coords = to_string(abs(q)) + to_string(abs(r)) + to_string(abs(s));
                
                // on s'assure que le desc fait 3 caractères pour l'alignement
                if (desc.length() < 3) desc.insert(0, 3 - desc.length(), ' ');
                if (desc.length() > 3) desc = desc.substr(0, 3);
                // coords
                if (coords.length() < 3) coords.insert(0, 3 - coords.length(), ' ');
                if (coords.length() > 3) coords = coords.substr(0, 3);

                lignes[0] << " / " << desc << " \\";
                lignes[1] << " \\ " << coords << " /";
            }
        }
        //affichage de la rangée complète
        for(const auto& line : lignes){
            cout << line.str() << "\n";
        }
    }
    cout<<"\nLégende : \n";
    cout<<"Première ligne : Etoilé , Type Quartier , Hauteur\n";
    cout<<"Deuxième ligne : Coordonnées";
    cout<<"Type Quartier :\nCarriere : C ; Caserne : S ; Jardin : J ; Temple : T ; Marche : M Habitation : H\n";
}

//CHATGPT
bool Plateau::peutPoserTuile(const Tuile& t, const HexagoneCoord& origin) const {
    const auto& hexs = t.getDisposition();

    for (const Hexagone& h : hexs) {

        // Conversion coordonnées relatives → absolues
        HexagoneCoord abs{
            origin.q + h.getQ(),
            origin.r + h.getR(),
            origin.s + h.getS()
        };

        // 1) Vérifier si la case est déjà occupée
        if (estOccupe(abs)) {
            return false; // un hexagone de la tuile overlap → interdit
        }
    }

    // 2) Vérifier qu'au moins un hexagone touche une tuile existante si le plateau n'est pas vide
    if (!estVide()) {
        bool touche = false;

        for (const Hexagone& h : hexs) {
            HexagoneCoord abs{
                origin.q + h.getQ(),
                origin.r + h.getR(),
                origin.s + h.getS()
            };

            // on regarde les voisins existants
            for (const auto& v : getVoisins(abs)) {
                if (estOccupe(v)) {
                    touche = true;
                    break;
                }
            }
            if (touche) break;
        }

        if (!touche) return false;  // la tuile doit être connectée
    }

    return true;
}



// SECTION CALCUL DES POINTS
// helper : retourne les 6 coordonnées voisines (indépendamment de l'occupation)
static std::vector<HexagoneCoord> voisinsCoord(const HexagoneCoord& c) {
    static const int d[6][3] = {
        {+1,-1,0}, {+1,0,-1}, {0,+1,-1},
        {-1,+1,0}, {-1,0,+1}, {0,-1,+1}
    };
    std::vector<HexagoneCoord> v;
    v.reserve(6);
    for (auto &x : d) {
        HexagoneCoord nc{ c.q + x[0], c.r + x[1], c.s + x[2] };
        v.push_back(nc);
    }
    return v;
}

// Renvoie le plus grand cluster d'habitations adjacentes
int Plateau::calculValeurHabitations() const {
    std::set<HexagoneCoord> visited;
    int meilleureValeur = 0;

    for (const auto& kv : grille) {
        const HexagoneCoord& coord = kv.first;
        const HexState& hs = kv.second;
        if (hs.type != TypeHexagone::Habitation) continue;
        if (visited.count(coord)) continue;

        std::queue<HexagoneCoord> q;
        q.push(coord);
        visited.insert(coord);
        int valeurGroupe = 0;

        while (!q.empty()) {
            HexagoneCoord cur = q.front(); q.pop();
            const HexState& hcur = grille.at(cur);
            valeurGroupe += hcur.hauteur;

            // getVoisins renvoie les voisins occupés
            for (const auto& n : getVoisins(cur)) {
                if (visited.count(n)) continue;
                auto it = grille.find(n);
                if (it != grille.end() && it->second.type == TypeHexagone::Habitation) {
                    visited.insert(n);
                    q.push(n);
                }
            }
        }

        meilleureValeur = std::max(meilleureValeur, valeurGroupe);
    }

    return meilleureValeur;
}

// Renvoie le nombre de marchés isolés (hauteur prise en compte pour le score)
int Plateau::calculValeurMarches() const {
    int total = 0;
    for (const auto& kv : grille) {
        const HexagoneCoord& coord = kv.first;
        const HexState& hs = kv.second;
        if (hs.type != TypeHexagone::Marche) continue;

        bool isole = true;
        // getVoisins renvoie voisins occupés ; si l'un d'eux est Marche => non isolé
        for (const auto& n : getVoisins(coord)) {
            const auto& neigh = grille.at(n);
            if (neigh.type == TypeHexagone::Marche) { isole = false; break; }
        }
        if (isole) total += hs.hauteur;
    }
    return total;
}

// Renvoie le nb de casernes avc au moins un voisin vide, hauteur prise en compte
int Plateau::calculValeurCasernes() const {
    int total = 0;
    for (const auto& kv : grille) {
        const HexagoneCoord& coord = kv.first;
        const HexState& hs = kv.second;
        if (hs.type != TypeHexagone::Caserne) continue;

        bool peripherie = false;
        // on doit tester les 6 positions voisines indépendamment de l'occupation
        for (const auto& n : voisinsCoord(coord)) {
            if (!estOccupe(n)) { peripherie = true; break; }
        }
        if (peripherie) total += hs.hauteur;
    }
    return total;
}

// Renvoie le nb de temples ayant bien 6 voisins, hauteur prise en compte
int Plateau::calculValeurTemples() const {
    int total = 0;
    for (const auto& kv : grille) {
        const HexagoneCoord& coord = kv.first;
        const HexState& hs = kv.second;
        if (hs.type != TypeHexagone::Temple) continue;

        bool entoure = true;
        for (const auto& n : voisinsCoord(coord)) {
            if (!estOccupe(n)) { entoure = false; break; }
        }
        if (entoure) total += hs.hauteur;
    }
    return total;
}

// Jardins, pas de contraintes
int Plateau::calculValeurJardins() const {
    int total = 0;
    for (const auto& kv : grille) {
        const HexState& hs = kv.second;
        if (hs.type == TypeHexagone::Jardin) total += hs.hauteur;
    }
    return total;
}


//Compter les étoiles (places) : Habitation:1, Marche/Temple/Caserne:2, Jardin:3
std::map<TypeHexagone,int> Plateau::compterEtoiles() const {
    std::map<TypeHexagone,int> res;
    res[TypeHexagone::Habitation] = 0;
    res[TypeHexagone::Marche]     = 0;
    res[TypeHexagone::Caserne]    = 0;
    res[TypeHexagone::Temple]     = 0;
    res[TypeHexagone::Jardin]     = 0;

    for (const auto& kv : grille) {
        const HexState& hs = kv.second;
        if (!hs.place) continue;
        switch (hs.type) { // si on a bien une place :
            case TypeHexagone::Habitation: res[hs.type] += 1; break;
            case TypeHexagone::Marche:     res[hs.type] += 2; break;
            case TypeHexagone::Caserne:    res[hs.type] += 2; break;
            case TypeHexagone::Temple:     res[hs.type] += 2; break;
            case TypeHexagone::Jardin:     res[hs.type] += 3; break;
            default: break;
        }
    }

    return res;
}

/* 
Appliquer les variantes (double les multiplicateurs quand condition remplie)
on reçoit "valeurs" (déjà calculées par les fonctions ci-dessus)
on modifie "multiplicateurs" (float) en conséquence des variantes
REGLES (https://cdn.1j1ju.com/medias/58/eb/a4-akropolis-regle.pdf): 
    Les Habitations : Si votre groupe d’Habitations a une valeur de 10 ou plus, ses points sont doublés.
    Les Marchés : Si vos Quartiers Marchands sont adjacents à une place Marché, leurs points sont doublés.
    Les Casernes : Si vos Casernes ont 3 ou 4 espaces vides adjacents, leurs points sont doublés.
    Les Temples : Si vos Temples sont placés sur un niveau supérieur, leurs points sont doublés.
    Les Jardins : Si vos Jardins sont adjacents à un lac (espace vide complètement entouré),leurs points sont doublés
*/
void Plateau::appliquerVariantes(const ModeDeJeu& mdj,
                                 std::map<TypeHexagone,int>& valeurs,
                                 std::map<TypeHexagone,float>& multiplicateurs) const
{
    // Habitations : si valeur >=10 => double
    if (mdj.varianteActive(Variante::Habitations)) {
        if (valeurs[TypeHexagone::Habitation] >= 10)
            multiplicateurs[TypeHexagone::Habitation] *= 2.f;
    }

    // Marchés : si un Marché est adjacent à une place Marche => double
    if (mdj.varianteActive(Variante::Marches)) {
        bool ok = false;
        for (const auto& kv : grille) {
            if (kv.second.type != TypeHexagone::Marche) continue;
            // getVoisins retourne voisins occupés ; on cherche une place de type Marche parmi eux
            for (const auto& n : getVoisins(kv.first)) {
                const HexState& nh = grille.at(n);
                if (nh.place && nh.type == TypeHexagone::Marche) { ok = true; break; }
            }
            if (ok) break;
        }
        if (ok) multiplicateurs[TypeHexagone::Marche] *= 2.f;
    }

    // Casernes : si une caserne a 3 ou 4 espaces vides adjacents => double
    if (mdj.varianteActive(Variante::Casernes)) {
        bool ok = false;
        for (const auto& kv : grille) {
            if (kv.second.type != TypeHexagone::Caserne) continue;
            int vides = 0;
            for (const auto& n : voisinsCoord(kv.first)) {
                if (!estOccupe(n)) ++vides;
            }
            if (vides == 3 || vides == 4) { ok = true; break; }
        }
        if (ok) multiplicateurs[TypeHexagone::Caserne] *= 2.f;
    }

    // Temples : si un temple (valide) est sur niveau >=2 => double
    if (mdj.varianteActive(Variante::Temples)) {
        bool ok = false;
        for (const auto& kv : grille) {
            if (kv.second.type == TypeHexagone::Temple && kv.second.hauteur >= 2) { ok = true; break; }
        }
        if (ok) multiplicateurs[TypeHexagone::Temple] *= 2.f;
    }

    // Jardins : si un Jardin est adjacent à un lac (case vide entourée) => double
    if (mdj.varianteActive(Variante::Jardins)) {
        bool ok = false;
        for (const auto& kv : grille) {
            if (kv.second.type != TypeHexagone::Jardin) continue;
            for (const auto& n : voisinsCoord(kv.first)) {
                if (estOccupe(n)) continue; // n doit être vide pour être un candidat lac
                // vérifier que n est entouré (tous ses 6 voisins occupés)
                bool entoure = true;
                for (const auto& m : voisinsCoord(n)) {
                    if (!estOccupe(m)) { entoure = false; break; }
                }
                if (entoure) { ok = true; break; }
            }
            if (ok) break;
        }
        if (ok) multiplicateurs[TypeHexagone::Jardin] *= 2.f;
    }
}


// Calcul final des points (public) : especte la règle "au moins une Place de la couleur est nécessaire" + ajoute les pierres
unsigned int Plateau::calculerPoints(const ModeDeJeu& mdj, unsigned int pierres) const {
    // valeurs de base
    std::map<TypeHexagone,int> valeurs;
    valeurs[TypeHexagone::Habitation] = calculValeurHabitations();
    valeurs[TypeHexagone::Marche]     = calculValeurMarches();
    valeurs[TypeHexagone::Caserne]    = calculValeurCasernes();
    valeurs[TypeHexagone::Temple]     = calculValeurTemples();
    valeurs[TypeHexagone::Jardin]     = calculValeurJardins();

    // étoiles
    std::map<TypeHexagone,int> etoiles = compterEtoiles();

    // multiplicateurs de variantes
    std::map<TypeHexagone,float> mult;
    mult[TypeHexagone::Habitation] = 1.f;
    mult[TypeHexagone::Marche]     = 1.f;
    mult[TypeHexagone::Caserne]    = 1.f;
    mult[TypeHexagone::Temple]     = 1.f;
    mult[TypeHexagone::Jardin]     = 1.f;

    appliquerVariantes(mdj, valeurs, mult); // modifie les multiplicateurs comme prévu

    // calcul par type
    unsigned int total = 0u;
    for (const auto& kv : valeurs) {
        TypeHexagone type = kv.first;
        int valeur = kv.second;
        if (valeur <= 0) continue; // pas de quartiers valides pour ce type

        int nbEtoiles = 0;
        auto it = etoiles.find(type);
        if (it != etoiles.end()) nbEtoiles = it->second;
        if (nbEtoiles <= 0) continue; // il faut au moins une Place de la couleur

        float facteur = mult[type];
        unsigned int pts = static_cast<unsigned int>( std::round( static_cast<float>(valeur) * static_cast<float>(nbEtoiles) * facteur ) );
        total += pts;
    }

    // pierres
    total += pierres;

    return total;
}

