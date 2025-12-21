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


// IllustreArchitecte

void Partie::donnerPierresArchitecte(unsigned int nb) {
    if (!estModeSoloArchitecte())
        return;

    architecte.addPierres(nb);
}


void Partie::jouerTourArchitecte() {
    if (!mdj.estSoloArchitecte())
        return;

    if (jeu.empty())
        return;
    donnerPierresArchitecte(participants[0].getPierrePourArchi());
    unsigned int indiceChoisi = 0;
    bool trouvePlace = false;

    // chercher la tuile avec Place la moins chère
    for (unsigned int i = 0; i < jeu.size(); ++i) {
        if (jeu[i].possedePlace()) {
            if (architecte.getPierres() >= i) {
                indiceChoisi = i;
                trouvePlace = true;
                break;
            }
        }
    }

    // sinon prendre la première tuile
    if (!trouvePlace) {
        indiceChoisi = 0;
    }

    unsigned int coutTuile = indiceChoisi;

    // l'architecte paye (si possible)
    if (architecte.getPierres() >= coutTuile) {
        architecte.depenserPierres(coutTuile);
    } else {
        coutTuile = 0; // il prend gratuitement
    }

    // l'architecte récupère la tuile pour le score
    architecte.prendreTuile(std::move(jeu[indiceChoisi]));

    // retirer la tuile du chantier
    jeu.erase(jeu.begin() + indiceChoisi);

    std::cout << "[Architecte] prend la tuile " << indiceChoisi
              << " (coût " << coutTuile << " pierres)\n"
              << "Nombre de Points : "<< architecte.calculerPoints()<<"\n";
}

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

            /* j'ai supprimé le champ "étoiles", j'ai donc légèrement modifié l'ancienne signature : 

            Hexagone a(0,1,data["tuiles"][i][0][1],dict[data["tuiles"][i][0][0]]) ; 
            Hexagone b(0,0,data["tuiles"][i][1][1],dict[data["tuiles"][i][1][0]]) ;
            Hexagone c(1,0,data["tuiles"][i][2][1],dict[data["tuiles"][i][2][0]]) ;
            */

            Hexagone a(0,1,dict[data["tuiles"][i][0][0]]) ; 
            Hexagone b(0,0,dict[data["tuiles"][i][1][0]]) ;
            Hexagone c(1,0,dict[data["tuiles"][i][2][0]]) ;
            v.push_back(a);
            v.push_back(b);
            v.push_back(c);

            Tuile t( i,v); 
            pioche.push_back(t);
        }
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
        
        // activer le mode solo architecte
        if (data[i]["nom"] == "Solo Architecte") {
            m.activerSoloArchitecte();
            std::string choix ;
            std::cout << "Tapez 0 pour facile\nTapez 1 pour moyen\nTapez autre pour Difficile\n";
            std::cout <<"Choississez la difficulté :";
            std::cin >> choix;
            if (choix == "0"){
                architecte.SetDifficulte(Difficulte::Facile);
            }
            else if (choix == "1"){
                architecte.SetDifficulte(Difficulte::Moyen);
            }
            else architecte.SetDifficulte(Difficulte::Difficile);
        }
        
        this->SetMdj(m);
        SetNbParticipants();
    }
    else {
        std::cout << "Erreur dans l'ouverture du fichier tuiles.json";
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

void Partie::SetNbParticipants(){
    //Une fois le mode de jeu choisi, permet de mettre à jour le nb de participant
    nbParticipants= mdj.getNbJoueur()+mdj.getNbIA();
};

void Partie::debutTour(){
    //Va permettre de charger la pioche

    //Nombre de pièce dans le jeu (nbJoueur + 2 (+ 1 si mode Architecte illustre car l'illustre n'est pas un joueur)
    for (unsigned int i = 0; i < this->getNbParticipants()+2+ this->estModeSoloArchitecte();i++){
        jeu.push_back(std::move(pioche.back()));
        pioche.pop_back();
    }
    
}

void Partie::finTour(){
    //Va permettre de vider le jeu, changer l'ordre des participations, tour++, et de mettre à jour le nombre de points

    //On vide le jeu en le remettant dans la pioche
    //On fait cela pour la sauvegarde qui doit récupérer les tuiles
    while(!jeu.empty()){
        pioche.push_back(std::move(jeu.back()));
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

void Partie::chargerDepuisSauvegarde(unsigned int t,std::vector<Participation>&& p,const ModeDeJeu& m,std::vector<Tuile>&& pi, std::vector<unique_ptr<Participant>>&& j, IllustreArchitecte archi) {
    tour = t;

    joueurs = std::move(j);
    participants = std::move(p);

    mdj = m;
    pioche = std::move(pi);

    architecte = std::move(archi);
    
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


