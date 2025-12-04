#include <Partie.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <map>

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
        dict["blanc"]  = TypeHexagone::Place;     // place

        cout << "Nombre de tuiles lues : " << data["tuiles"].size() << endl;
        for (unsigned int i=0 ; i<61; i++){   
            cout<<"Chargement tuile numéro "<<i<<"\n";  
            vector<Hexagone> v ; 

            Hexagone a(0,1,data["tuiles"][i][0][1],dict[data["tuiles"][i][0][0]]) ; 
            Hexagone b(0,0,data["tuiles"][i][1][1],dict[data["tuiles"][i][1][0]]) ;
            Hexagone c(1,0,data["tuiles"][i][2][1],dict[data["tuiles"][i][2][0]]) ;
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
Participant Partie::getParticipant(std::size_t i) const {
            if (i >= nbParticipants) throw std::out_of_range("Index de participant");
            return participants[i];
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
    }
    else {
        std::cout << "Erreur dans l'ouverture du fichier tuiles.json";
    }
}

void Partie::initialiserParticipations() {
    participations.clear();

    for (std::size_t i = 0; i < participants.size(); ++i) {
        participations.emplace_back(&participants[i], i + 1);  // ordre de passage
    }
}

void Partie::calculerScoresFinDePartie() {
    for (auto& part : participations) {
        part.calculerPoints();
    }
}

Participation& Partie::getGagnant() {
    return *std::max_element(
        participations.begin(),
        participations.end(),
        [](const Participation& a, const Participation& b) {

            // Critère 1 : points
            if (a.getPoints() != b.getPoints())
                return a.getPoints() < b.getPoints();

            // Critère 2 : pierres en cas d'égalité
            return a.getPierres() < b.getPierres();
        }
    );
}