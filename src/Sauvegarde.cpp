#include "Sauvegarde.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;
using namespace std ;

HexagoneCoord HexaCfromString(const std::string& str);
HexState HexaSfromString(const std::string& str);
Tuile TuileFromString(const std::string& str);
Variante VarianteFromString(const std::string& str);

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

Sauvegarde::Sauvegarde(const Partie& p, const string& nomsauvegarde) : architecte(p.getArchitecte()),nom(nomsauvegarde),mdj(p.mdj),nbParticipants(p.nbParticipants),tour(p.getTour()),pioche(p.getPioche()){
    
    participants.reserve(p.participants.size());
    for (const auto& part : p.participants) {
        participants.emplace_back(part.getParticipant(),part.getOrdrePassage(),part.getPoints(),part.getPierres(),part.getPlateau());
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
        {"nom", s.mdj.getnomMDJ()},
        {"EstSoloArchitecte", s.mdj.estSoloArchitecte()}
    };
    if (s.mdj.estSoloArchitecte()){
        json architecte;
        architecte["pierres"] = s.architecte.getPierres();
        const std::map<TypeHexagone, std::string> Conversion = {
            {TypeHexagone::Carriere,   "Carriere"},
            {TypeHexagone::Caserne,    "Caserne"},
            {TypeHexagone::Jardin,     "Jardin"},
            {TypeHexagone::Temple,     "Temple"},
            {TypeHexagone::Marche,     "Marche"},
            {TypeHexagone::Habitation, "Habitation"},
        };

        json infos ;

        for (const auto& [type, info] : s.architecte.getInfos()) {
            auto it = Conversion.find(type);
            if (it != Conversion.end()) {
                infos[it->second] = {
                    {"quartiers", info.quartiers},
                    {"places", info.places}
                };
            }
        }

        architecte["Quartier"] = infos;
        newSave["Architecte"]=architecte;
    }
    
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

    IllustreArchitecte archi ;
    //On active le mode Solo Architecte au besoin
    if (data["mdj"]["nom"]=="Solo Architecte"){
        mdj.activerSoloArchitecte();

        //On reconstruit l'architecte
        
        archi.SetPierres(data["Architecte"]["pierres"]);

        const std::map<std::string, TypeHexagone> Conversion = {
            {"Carriere",   TypeHexagone::Carriere},
            {"Caserne",    TypeHexagone::Caserne},
            {"Jardin",     TypeHexagone::Jardin},
            {"Temple",     TypeHexagone::Temple},
            {"Marche",     TypeHexagone::Marche},
            {"Habitation", TypeHexagone::Habitation}
        };

        for (const auto& [str, nb] : data["Architecte"]["Quartier"].items()) {
            InfosQuartier info;

            auto it = Conversion.find(str);
            if (it != Conversion.end()) {
                info.quartiers = nb["quartiers"];
                info.places = nb["places"];
                archi.SetInfo(it->second,info);
            }
        }

    }

    

    for (const auto& v : data["mdj"]["variantes"]){
        mdj.activerVariante(VarianteFromString(v.get<std::string>()));
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

        participants.emplace_back(*joueur,data["participants"][i]["ordrePassage"],data["participants"][i]["nbPoints"],data["participants"][i]["pierre"],plateau);
        joueurs.push_back(std::move(joueur));
    }
   

    vector<Tuile> pioche ;
    for (const auto& tuile : data["pioche"]){
        pioche.push_back(TuileFromString(tuile));
    }
    
    Partie& p = Partie::getInstance();
    p.chargerDepuisSauvegarde(data["tour"],std::move(participants),mdj,std::move(pioche),std::move(joueurs),std::move(archi));
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

    char c1, c2;

    //On vérifie la structure et on lit les valeurs
    if (!std::getline(f,typetemp,',')) {
        throw std::runtime_error("TypeHexagone invalide: " + str);
    }

    auto it = Conversion.find(typetemp);
    if (it == Conversion.end()) {
        throw std::runtime_error("TypeHexagone inconnu: " + typetemp);
    }
    type = it->second;

    if (!(f >> hauteur)) {
        throw std::runtime_error("r invalide: " + str);
    }

    //virgule
    if (!(f >> c1) || c1 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> id_tuile)) {
        throw std::runtime_error("s invalide: " + str);
    }
    //virgule
    if (!(f >> c2) || c2 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> place)) {
        throw std::runtime_error("s invalide: " + str);
    }

    return HexState{type,hauteur,id_tuile,place};
}

Tuile TuileFromString(const std::string& str) {
    const std::map<std::string, TypeHexagone> Conversion = {
        {"Carriere",   TypeHexagone::Carriere},
        {"Caserne",    TypeHexagone::Caserne},
        {"Jardin",     TypeHexagone::Jardin},
        {"Temple",     TypeHexagone::Temple},
        {"Marche",     TypeHexagone::Marche},
        {"Habitation", TypeHexagone::Habitation}
    };

    std::istringstream f(str);
    std::vector<Hexagone> vecteurHexa;

    for (unsigned int i = 0; i < 3; i++) {
        std::string tempQ, tempR, tempType, tempPlace;

        if (!std::getline(f, tempQ, ',')) throw std::runtime_error("q invalide: " + str);
        if (!std::getline(f, tempR, ',')) throw std::runtime_error("r invalide: " + str);
        if (!std::getline(f, tempType, ',')) throw std::runtime_error("typeHexagone invalide: " + str);
        if (!std::getline(f, tempPlace, ',')) throw std::runtime_error("place invalide: " + str);
        
        //Conversion
        int q = std::stoi(tempQ);
        int r = std::stoi(tempR);
        bool place = (tempPlace == "1" || tempPlace == "true");

        auto it = Conversion.find(tempType);
        if (it == Conversion.end()) throw std::runtime_error("TypeHexagone inconnu: " + tempType);
        TypeHexagone type = it->second;

        vecteurHexa.push_back(Hexagone(q, r, type, place));
    }

    std::string tempId;
    if (!std::getline(f, tempId)) throw std::runtime_error("id invalide: " + str);
    unsigned int id = std::stoi(tempId);

    return Tuile(id, vecteurHexa);
}


Variante VarianteFromString(const std::string& str) {
    const std::map<std::string, Variante> Conversion = {
        {"Caserne",    Variante::Casernes},
        {"Jardin",     Variante::Jardins},
        {"Temple",     Variante::Temples},
        {"Marche",     Variante::Marches},
        {"Habitation", Variante::Habitations}
    };

    std::istringstream f(str);
    std::string temp;

    if (!(f >> temp)) {
        throw std::runtime_error("Variante invalide\n");
    }

    auto it = Conversion.find(temp);
    if (it == Conversion.end()) {
        throw std::runtime_error("Variante inconnue\n");
    }

    return it->second;
}

void SauvegardeManager::supprimerSauvegarde(unsigned int id){
    json data;

    //Lecture des sauvegardes
    ifstream file("data/sauvegarde.json");
    if (!file.is_open()){
        throw std::runtime_error("Erreur dans l'ouverture du fichier\n");
    }
    file >> data;
    file.close();
    
    

    if (id>= data.size()){
        throw std::runtime_error("id invalide, > nombre de sauvegarde\n");
    }


    json datanew = json::array();
    unsigned int i = 0;

    while (i<data.size()){
        if (i!=id){
            datanew.push_back(data[i]);
        }
        i++;
    }
    
    //Ecriture des sauvegardes
    ofstream file2("data/sauvegarde.json");
    file2 << datanew.dump(4);
    file2.close();
}