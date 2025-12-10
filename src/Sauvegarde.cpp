#include "Sauvegarde.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std ;

Sauvegarde::Sauvegarde(const Partie p, const string& nomsauvegarde){
    nom= nomsauvegarde;
    tour = p.getTour();
    nbParticipants = p.getNbParticipants();
    mdj = p.mdj;

    participants=p.participants;

    pioche =p.pioche;
    
}
void SauvegardeManager::enregistrerSauvegarde(const Sauvegarde& s) {
    json newSave;

    newSave["nomSauvegarde"] = s.nom;
    newSave["tour"] = s.tour;
    newSave["nbParticipants"] = s.nbParticipants;

    newSave["mdj"] = {
        {"nbrJoueur", s.mdj.getNbJoueur()},
        {"nbrIA", s.mdj.getNbrIA()},
        {"description", s.mdj.getDescription()},
        {"nom", s.mdj.getnomMDJ()}
    };

    for (unsigned int i = 0; i < s.nbParticipants; i++) {
        json temp;
        temp["nom"] = s.participants[i].getParticipant()->getPseudo();
        temp["pierre"] = s.participants[i].getPierres();
        newSave["participants"].push_back(temp);
    }

    
    json sauvegardes;

    ifstream file("data/sauvegarde.json");
    if (file.is_open()) {
        try {
            file >> sauvegardes;
            file.close();
        } catch (...) {
            sauvegardes = json::array();
            cout << "Erreur dans l'ouverture du fichier json, supression des sauvegardes.\n";
        }
    }


    if (!sauvegardes.is_array())
        sauvegardes = json::array();


    sauvegardes.push_back(newSave);


    ofstream file2("data/sauvegarde.json");
    file2 << sauvegardes.dump(4);
    file2.close();

    cout << "Sauvegarde ajoutée avec succès.\n";
}


vector<SauvegardeInfo> SauvegardeManager::getListeSauvegardes(){
    vector<SauvegardeInfo> liste ;
    json data;

    ifstream file("data/sauvegarde.json");
    if (!file.is_open()){
        throw "Erreur dans l'ouverture du fichier";
    }
    file >> data;
    file.close();

    if (!data.is_array()) return liste;

    for (unsigned int i = 0; i< data.size(); i++){
        SauvegardeInfo infos ;
        infos.index=i;
        infos.nomMDJ=data[i]["mdj"]["nom"];
        infos.nomSauvegarde=data[i]["nomSauvegarde"];
        infos.tour=data[i]["tour"];
        liste.push_back(infos);
    }
    return liste;
}
Partie& SauvegardeManager::chargerSauvegarde(unsigned int id){
    json data;

    ifstream file("data/sauvegarde.json");
    if (!file.is_open()){
        throw "Erreur dans l'ouverture du fichier";
    }
    file >> data;
    file.close();

    if (!data.is_array() || id >= data.size())
        throw runtime_error("ID de sauvegarde invalide");

    //A initier
    vector<Participation> participants;
    ModeDeJeu mdj ;
    vector<Tuile> pioche ;
    Partie& p = Partie::getInstance();
    p.tour = data[id]["tour"];
    p.participants = participants;
    p.mdj = mdj;
    p.pioche = pioche;
    p.nbParticipants = participants.size();
    return p;
}
