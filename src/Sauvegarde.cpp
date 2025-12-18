#include "Sauvegarde.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;
using namespace std ;

HexagoneCoord HexaCfromString(const std::string& str);
HexState HexaSfromString(const std::string& str);
Tuile TuileFromString(const std::string& str);

std::string VarianteToString(Variante v) {
    static const std::map<Variante, std::string> Conversion = {
        {Variante::Casernes,    "Caserne"},
        {Variante::Jardins,     "Jardin"},
        {Variante::Temples,     "Temple"},
        {Variante::Marches,     "Marche"},
        {Variante::Habitations, "Habitation"}
    };
    return Conversion.at(v);
}

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
        {"nbJoueur", s.mdj.getNbJoueur()},
        {"nbIA", s.mdj.getNbIA()},
        {"description", s.mdj.getDescription()},
        {"nom", s.mdj.getnomMDJ()}
    };
    
    newSave["mdj"]["variantes"] = nlohmann::json::array();
    for (const auto& v : s.mdj.getVariantes()) {
        newSave["mdj"]["variantes"].push_back(VarianteToString(v));
    }

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
    newSave["participants"] = participantsTab;

    json temp2= json::array();
    for (const auto& t : s.pioche){
        temp2.push_back(t.ToString());
    }
    newSave["pioche"] = temp2;

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
        throw std::runtime_error("Erreur dans l'ouverture du fichier");
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
Partie& SauvegardeManager::chargerSauvegarde(unsigned int index){
    json datatemp;

    ifstream file("data/sauvegarde.json");
    if (!file.is_open()){
        throw "Erreur dans l'ouverture du fichier";
    }
    file >> datatemp;
    file.close();

    if (!datatemp.is_array() || index >= datatemp.size())
        throw runtime_error("ID de sauvegarde invalide");

    json data = datatemp[index] ;

    //Création mdj
    ModeDeJeu mdj(data["mdj"]["nom"],data["mdj"]["nbJoueur"],data["mdj"]["nbIA"],data["mdj"]["description"]);
    for (const auto& v : data["mdj"]["variantes"]){
        mdj.activerVariante(v);
    }
    
    //Création participants
    vector<Participation> participants;
    vector<std::unique_ptr<Participant>> joueurs;
    for (unsigned int i = 0 ; i<data["nbParticipants"] ; i++){
        std::unique_ptr<Participant> joueur;
        joueur = std::make_unique<Joueur>(data["participants"][i]["nom"]);

        Plateau plateau;

        for (const auto& [cle, valeur] : data["participants"][i]["plateau"].items()) {
            std::string cleS = cle;
            std::string valeurS = valeur.get<std::string>();

            HexagoneCoord hexC = HexaCfromString(cleS);
            HexState hexS = HexaSfromString(valeurS);
            plateau.placerTuileSauvegarde(hexC,hexS);
        }

        participants.emplace_back(*joueur,data["participants"][i]["ordre"],data["participants"][i]["nbPoints"],data["participants"][i]["pierre"],plateau);
        joueurs.push_back(std::move(joueur));
    }

   

    vector<Tuile> pioche ;
    for (const auto& tuile : data["pioche"]){
        pioche.push_back(TuileFromString(tuile));
    }

    Partie& p = Partie::getInstance();
    p.chargerDepuisSauvegarde(data["tour"],std::move(participants),mdj,std::move(pioche),std::move(joueurs));
    return p;
}

HexagoneCoord HexaCfromString(const std::string& str) {
    std::istringstream f(str);

    int q, r, s;
    char c1, c2;

    //On vérifie la structure et on lit les valeurs
    if (!(f >> q)) {
        throw std::runtime_error("q invalide: " + str);
    }

    // virgule
    if (!(f >> c1) || c1 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> r)) {
        throw std::runtime_error("r invalide: " + str);
    }

    //virgule
    if (!(f >> c2) || c2 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> s)) {
        throw std::runtime_error("s invalide: " + str);
    }

    return HexagoneCoord{q, r, s};
}

HexState HexaSfromString(const std::string& str){
    const std::map<std::string, TypeHexagone> Conversion = {
    {"Carriere",   TypeHexagone::Carriere},
    {"Caserne",    TypeHexagone::Caserne},
    {"Jardin",     TypeHexagone::Jardin},
    {"Temple",     TypeHexagone::Temple},
    {"Marche",     TypeHexagone::Marche},
    {"Habitation", TypeHexagone::Habitation}
    };

    std::istringstream f(str);

    int hauteur, id_tuile;
    std::string typetemp ;
    TypeHexagone type;
    bool place;

    char c1, c2,c3;

    //On vérifie la structure et on lit les valeurs
    if (!(f>> typetemp)) {
        throw std::runtime_error("q invalide: " + str);
    }
    type=Conversion.at(typetemp);

    // virgule
    if (!(f >> c1) || c1 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> hauteur)) {
        throw std::runtime_error("r invalide: " + str);
    }

    //virgule
    if (!(f >> c2) || c2 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> id_tuile)) {
        throw std::runtime_error("s invalide: " + str);
    }
    //virgule
    if (!(f >> c3) || c3 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> place)) {
        throw std::runtime_error("s invalide: " + str);
    }

    return HexState{type,hauteur,id_tuile,place};
}

Tuile TuileFromString(const std::string& str){
    const std::map<std::string, TypeHexagone> Conversion = {
    {"Carriere",   TypeHexagone::Carriere},
    {"Caserne",    TypeHexagone::Caserne},
    {"Jardin",     TypeHexagone::Jardin},
    {"Temple",     TypeHexagone::Temple},
    {"Marche",     TypeHexagone::Marche},
    {"Habitation", TypeHexagone::Habitation}
    };

    std::istringstream f(str);

    
    

    vector<Hexagone> vecteurHexa;

    for (unsigned int i = 0;i<3 ; i++){
        //On vérifie la structure et on lit les valeurs
        char c1, c2, c3, c4;
        int q, r ;
        std::string temp;
        TypeHexagone type;
        bool place;
        if (!(f>> q)) {
            throw std::runtime_error("q invalide: " + str);   
        // virgule
        if (!(f >> c1) || c1 != ',') {
            throw std::runtime_error("virgule manquante: " + str);
        }

        if (!(f >> r)) {
            throw std::runtime_error("r invalide: " + str);
        }

        //virgule
        if (!(f >> c2) || c2 != ',') {
            throw std::runtime_error("virgule manquante: " + str);
        }

        if (!(f >> temp)) {
            throw std::runtime_error("typeHexagone invalide: " + str);
        }
        type = Conversion.at(temp);

        // virgule
        if (!(f >> c3) || c3 != ',') {
            throw std::runtime_error("virgule manquante: " + str);
        }

        if (!(f >> place)) {
            throw std::runtime_error("place invalide: " + str);
        }

        // virgule
        if (!(f >> c4) || c4 != ',') {
            throw std::runtime_error("virgule manquante: " + str);
        }
        vecteurHexa.push_back(Hexagone(q,r,type,place));
        }
    }
    unsigned int id;

    if (!(f >> id)) {
        throw std::runtime_error("id invalide: " + str);
    }


    
    Tuile t(id,vecteurHexa);
    return t;
}