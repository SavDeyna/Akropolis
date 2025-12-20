#include <Partie.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <map>
#include <sstream>

using json = nlohmann::json;
using namespace std ;

//fonction locale de mélange de vecteur. 
//Méthode trouvée sur un forum : fisher_yates_shuffle (https://stackoverflow.com/questions/6127503/shuffle-array-in-c/6128209#6128209)
void Partie::melangePioche() {
    size_t n= pioche.size();
    for (size_t i = n-1; i>0 ; i--){
        unsigned int j = std::rand() % (i + 1);
        swap(pioche[i], pioche[j]); 
    } 
}

void Partie::ChargerTuiles(){
    ifstream file("data/tuiles.json");
    //lecture du fichier
    if (file.is_open()){
        json data;
        file >> data;
        file.close();

        std::map<string,TypeHexagone> dict ;
        dict["rouge"]  = TypeHexagone::Caserne;   // quartier rouge
        dict["gris"]   = TypeHexagone::Carriere;  // carrière
        dict["bleu"]   = TypeHexagone::Habitation; // quartier bleu
        dict["violet"] = TypeHexagone::Temple;    // quartier violet
        dict["vert"]   = TypeHexagone::Jardin;    // quartier vert
        dict["jaune"]  = TypeHexagone::Marche;    // quartier jaune

        cout << "Nombre de tuiles lues : " << data["tuiles"].size() << endl;
        for (unsigned int i=0 ; i<61; i++){   
            vector<Hexagone> v ; 

            // Lecture des 3 hexagones avec leur type et si c'est une place (nbEtoiles > 0)
            int etoiles_a = data["tuiles"][i][0][1];
            int etoiles_b = data["tuiles"][i][1][1];
            int etoiles_c = data["tuiles"][i][2][1];

            Hexagone a(0, 1, dict[data["tuiles"][i][0][0]], etoiles_a > 0);
            Hexagone b(0, 0, dict[data["tuiles"][i][1][0]], etoiles_b > 0);
            Hexagone c(1, 0, dict[data["tuiles"][i][2][0]], etoiles_c > 0);
            v.push_back(a);
            v.push_back(b);
            v.push_back(c);

            Tuile t( i,v); 
            pioche.push_back(t);
        }
        melangePioche();
    }
    else {
        std::cout << "Erreur dans l'ouverture du fichier tuiles.json";
    }
}


void Partie::choixMDJ() {
    ifstream file("data/mdj.json");
    //lecture du fichier
    if (file.is_open()){
        json data;
        file >> data;
        file.close();
        unsigned int i = 0 ;
        unsigned int nbMdj = data.size();
        string a ;
        std::cout<<"Choississez le mode de jeu :\nTapez 1 : Selectionner\nAutre : Suivant";
        do {
            i++;
            i%=nbMdj;
            a="";
            std::cout<<"\n    Nom : " <<data[i]["nom"]<<"\n    Nombre de Joueur : "<<data[i]["nbrJoueur"]<<"\n    Nombre de IA : "<< data[i]["nbrIA"]<<"\n    Description : "<<data[i]["description"]<<"\n";

            std::cin >> a;
        }while(a !="1");
        ModeDeJeu m(data[i]["nom"],data[i]["nbrJoueur"],data[i]["nbrIA"],data[i]["description"]);
        this->SetMdj(m);
        SetNbParticipants();
    }
    else {
        std::cout << "Erreur dans l'ouverture du fichier mdj.json";
    }
}

void Partie::GenererTuilesAleatoires(unsigned int n) {
    static std::vector<TypeHexagone> types = {
        TypeHexagone::Caserne,
        TypeHexagone::Carriere,
        TypeHexagone::Habitation,
        TypeHexagone::Temple,
        TypeHexagone::Jardin,
        TypeHexagone::Marche
    };

    for (unsigned int k = 0; k < n; k++) {
        // types aléatoires des 3 hexagones de la tuile
        TypeHexagone t1 = types[rand() % types.size()];
        TypeHexagone t2 = types[rand() % types.size()];
        TypeHexagone t3 = types[rand() % types.size()];

        // forme (triangle)
        std::vector<Hexagone> disp;
        disp.emplace_back(0, 1, t1);
        disp.emplace_back(0, 0, t2);
        disp.emplace_back(1, 0, t3);

        // id basé sur la taille actuelle (je suis pas sûr de ca)
        int id = pioche.size();

        Tuile tuile(id, disp);
        pioche.push_back(tuile);
    }
}
Partie::Partie(unsigned int tour, vector<Participation>&& participants , ModeDeJeu mdj, vector<Tuile>&& pioche):
    mdj(mdj),participants(participants),tour(tour),pioche(pioche){}


void Partie::calculerScoresFinDePartie() {
    for (auto& part : participants) {
        // Appel du calcul des points depuis le Plateau
        unsigned int pts = part.getPlateau().calculerPoints(mdj, part.getPierres());
        
        // Stocker le résultat dans la Participation
        part.setNbPoints(pts);  // Il faut ajouter un setter pour nbPoints dans Participation
    }
}

void Partie::initializeNewGame(int nbJoueurs, const std::vector<std::string>& pseudos, const std::vector<std::string>& variantes) {
    // Create mode de jeu
    ModeDeJeu newMdj("Custom", nbJoueurs, 0, "Custom game");
    
    // Activate variants
    for (const auto& var : variantes) {
        if (var == "Habitations") newMdj.activerVariante(Variante::Habitations);
        else if (var == "Marches") newMdj.activerVariante(Variante::Marches);
        else if (var == "Casernes") newMdj.activerVariante(Variante::Casernes);
        else if (var == "Temples") newMdj.activerVariante(Variante::Temples);
        else if (var == "Jardins") newMdj.activerVariante(Variante::Jardins);
    }
    
    SetMdj(newMdj);
    SetNbParticipants();
    
    // Add participants with proper pseudos
    for (int i = 0; i < nbJoueurs; ++i) {
        std::string pseudo = (i < static_cast<int>(pseudos.size()) && !pseudos[i].empty()) 
            ? pseudos[i] 
            : "Player " + std::to_string(i + 1);
        addParticipation(pseudo);
    }
    
    // Initialize game
    randomizePlayerOrder();
    ChargerTuiles();
    initializePlayerStones();
    debutTour();
}

string Partie::getGagnant() {
    unsigned int max = 0 ;
    for (unsigned int i = 1 ; i<nbParticipants ; i++){
        if (participants[max].getPoints()<participants[i].getPoints()){
            max=i;
        }
    }
    return this->getParticipants()[max].getParticipant().getPseudo();
}

void Partie::addParticipation(string pseudo) {
    if (participants.size() == 4) {
        throw std::runtime_error("Nombre maximal de participants atteint");
    }
    joueurs.push_back(std::make_unique<Joueur>(pseudo));
    
    unsigned int ordre = participants.size() +1;
    participants.emplace_back(*joueurs.back(), ordre);
}

void Partie::initializePlayerStones() {
    // Give initial stones based on turn order (ordre 1 → 1 stone, ordre 2 → 2 stones, etc.)
    for (auto& p : participants) {
        unsigned int ordre = p.getOrdrePassage();
        p.setPierres(ordre);
    }
}

void Partie::randomizePlayerOrder() {
    // Create a vector of indices to shuffle
    std::vector<unsigned int> indices;
    for (unsigned int i = 1; i <= participants.size(); ++i) {
        indices.push_back(i);
    }
    
    // Shuffle the indices
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);
    
    // Assign shuffled indices as new turn order
    for (size_t i = 0; i < participants.size(); ++i) {
        participants[i].setOrdrePassage(indices[i]);
    }
}

void Partie::SetNbParticipants(){
    //Une fois le mode de jeu choisi, permet de mettre à jour le nb de participant
    nbParticipants= mdj.getNbJoueur()+mdj.getNbIA();
};

void Partie::refillJeu() {
    bool wasRefilled = false;
    
    // Only add a new card when there's exactly 1 card left
    if (jeu.size() == 1 && !pioche.empty()) {
        jeu.push_back(std::move(pioche.back()));
        pioche.pop_back();
        wasRefilled = true;
    }
    // Continue filling to target size (getNbParticipants() + 2)
    while (jeu.size() < getNbParticipants() + 2 && !pioche.empty()) {
        jeu.push_back(std::move(pioche.back()));
        pioche.pop_back();
    }
    
    // When pioche is refilled, rotate the architecte
    if (wasRefilled) {
        rotateArchitecte();
    }
}

void Partie::debutTour(){
    // La tuile de départ est déjà placée par le constructeur de Plateau
    // On remplit simplement le jeu depuis la pioche
    refillJeu();
}

Participation& Partie::getCurrentPlayer() {
    // Find the player with ordre 1 (current turn)
    for (auto& p : participants) {
        if (p.getOrdrePassage() == 1) {
            return p;
        }
    }
    // Fallback to first player
    return participants[0];
}

void Partie::rotateArchitecte() {
    // Rotate the turn order: current architecte becomes last
    for (auto& p : participants) {
        p.prochainOrdrePassage(this->getNbParticipants());
    }
}

void Partie::finTour(){
    //Va permettre de vider la pioche, changer l'ordre des participations, tour++, et de mettre à jour le nombre de points

    //On vide le jeu
    while(!jeu.empty()){
        jeu.pop_back();
    }

    //Mise à jour des ordres de passage + mise à jour du nombre de points
    for (unsigned int i =0 ; i<this->getNbParticipants();i++){
        participants[i].prochainOrdrePassage(this->getNbParticipants());
        unsigned int points =participants[i].getPlateau().calculerPoints(mdj,participants[i].getPierres());
        participants[i].setNbPoints(points);
    }

    //prochain tour
    tour++;
}

void Partie::chargerDepuisSauvegarde(unsigned int t,std::vector<Participation>&& p,const ModeDeJeu& m,std::vector<Tuile>&& pi, std::vector<unique_ptr<Participant>>&& j) {
    tour = t;

    joueurs = std::move(j);
    participants = std::move(p);

    mdj = m;
    pioche = std::move(pi);
    
    SetNbParticipants();
}


// Pour la sauvegarde
std::string ModeDeJeu::ToStringVariente() const {
    std::ostringstream f;
    bool paspremiertour = false;

    static const std::map<Variante, std::string> Conversion = {
        {Variante::Casernes,    "Caserne"},
        {Variante::Jardins,     "Jardin"},
        {Variante::Temples,     "Temple"},
        {Variante::Marches,     "Marche"},
        {Variante::Habitations, "Habitation"}
    };

    for (const auto& v : variantes) {
        auto it = Conversion.find(v);
        if (it == Conversion.end()) {
            throw std::runtime_error("Variante inconnue dans ToStringVariente()");
        }

        if (paspremiertour) {
            f << ",";
        }
        paspremiertour = true;
        f << it->second;
    }

    return f.str();
}


