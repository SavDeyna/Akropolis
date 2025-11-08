#include <Partie.h>
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
        dict["rouge"]= TypeHexagone::Caserne;
        dict["gris"] = TypeHexagone::Carriere ;
        dict["bleu"] = TypeHexagone::Quartier ;
        dict["violet"] = TypeHexagone::Temple ;
        dict["vert"] = TypeHexagone::Jardin ;
        dict["jaune"] = TypeHexagone::Marche ;


        for (unsigned int i=0 ; i<61; i++){
            vector<Hexagone> v ;

            Hexagone a(0,1,data["tuiles"][i][0][1],dict[data["tuiles"][i][0][0]]) ; 
            Hexagone b(0,0,data["tuiles"][i][1][1],dict[data["tuiles"][i][1][0]]) ;
            Hexagone c(1,0,data["tuiles"][i][2][1],dict[data["tuiles"][i][2][0]]) ;
            v.push_back(a);
            v.push_back(b);
            v.push_back(c);

Partie::Partie(std::initializer_list<const Joueur*> js, Variante v)
    : variante(v)
{
    if (js.size() == 0 || js.size() > 4)
        throw std::invalid_argument("Une partie doit avoir entre 1 et 4 joueurs.");

    // Remplit le tableau et compte
    nbJoueurs = 0;
    for (const Joueur* p : js) {
        joueurs[nbJoueurs++] = p;  // on stocke tel quel (non-owner)
    }
    // Pad le reste avec nullptr
    for (std::size_t i = nbJoueurs; i < joueurs.size(); ++i) {
        joueurs[i] = nullptr;
    }
}
            Tuile t( i,v);
            pioche.push_back(t);
        }
    }
    else{
        std::cout<<"Erreur dans l'ouverture du fichier tuiles.json";
    }
}