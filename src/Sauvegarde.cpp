#include "Sauvegarde.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std ;

Sauvegarde::Sauvegarde(const Partie& p, const string& nomsauvegarde) : nom(nomsauvegarde),mdj(p.mdj),nbParticipants(p.nbParticipants),tour(p.getTour()){
    
    participants.reserve(p.participants.size());
    for (const auto& part : p.participants) {
        participants.emplace_back(part.getParticipant(),part.getOrdrePassage());
    }
}
void SauvegardeManager::enregistrerSauvegarde(const Sauvegarde& s) {
    json newSave;

    //Préparation de la sauvegarde
    newSave["nomSauvegarde"] = s.nom;
    newSave["tour"] = s.tour;
    newSave["nbParticipants"] = s.nbParticipants;

    newSave["mdj"] = {
        {"nbrJoueur", s.mdj.getNbJoueur()},
        {"nbrIA", s.mdj.getNbIA()},
        {"description", s.mdj.getDescription()},
        {"nom", s.mdj.getnomMDJ()}
    };
    vector<json> participantsTab;
    for (unsigned int i = 0; i < s.nbParticipants; i++) {
        json temp;
        temp["nom"] = s.participants[i].getParticipant().getPseudo();
        temp["pierre"] = s.participants[i].getPierres();
        temp["ordrePassage"] = s.participants[i].getOrdrePassage();
        temp["nbPoints"] = s.participants[i].getPoints();
        std::map<HexagoneCoord, HexState> grille = s.participants[i].getPlateau().getGrille();
        json Plateau ;
        for (const auto& [cle, valeur] : grille) {
            //Conversion des coordonnées
            std::string cleS = cle.toString();
            std::string valeurS = valeur.toString();
            Plateau[cleS] = valeurS;
        }
        temp["plateau"]= Plateau ;
        //Rajout du participant
        participantsTab.push_back(temp);
    }
    //Ajout des participants
    newSave["participants"].push_back(participantsTab);

    //Lecture des sauvegardes existantes
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

    //Ajout de la sauvegarde
    sauvegardes.push_back(newSave);


    //Ecriture des sauvegardes
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
    p.chargerDepuisSauvegarde(data[id]["tour"],participants,mdj,pioche);
    return p;
}
