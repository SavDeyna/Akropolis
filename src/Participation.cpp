#include "Participation.h"
#include "Plateau.h"
#include <queue>
#include <set>
#include <map>

using namespace std;

void Participation::prochainOrdrePassage(unsigned int nbParticipants){
    //Mettre à jour l'ordre de passage
    if (ordrePassage !=1) ordrePassage-- ;
    else ordrePassage= nbParticipants;
}

void Participation::Jouer(vector<Tuile>& jeu){
    cout <<"Tour de "<<participant->getPseudo()<<"\n>>";
    cout <<"Nombre de points : "<<nbPoints<<"\n";
    cout <<"Nombre de pierres :" << pierres<<"\n";
    for (unsigned int i = 0 ; i< jeu.size(); i++){
        jeu[i].afficherTuile();
        cout<<"Prix : "<<i<<"\n";
    }
    plateau.dessinerPlateau(7);
    
    unsigned int tuileIndice = 0;
    bool saisieValide = false;

    while (!saisieValide) {

        cout << "Choisissez votre tuile à placer (1 à " << jeu.size() << ") : ";
        if (cin >> tuileIndice) {
            if (tuileIndice >= 1 && tuileIndice <= jeu.size()) {
                tuileIndice--;   
                if (pierres<tuileIndice){
                    cout<<"Nombre de pierres insuffisant :"<< pierres<<" < " <<tuileIndice<<"\n";
                }
                else saisieValide = true;
            }
            else {
                cout << "Indice invalide. Réessayez.\n";
            }
        }
        else {
            cin.clear();               
            cin.ignore(52729, '\n');   
            cout << "Veuillez entrer un nombre valide.\n";
        }
    }


    saisieValide = false;
    while(!saisieValide){
        int q ,r,s;
        cout<<"Choisissez les coordonnées ou placer la tuile :\n";
        cout<<"Coordonnée 1 [0:9]: ";
        if (cin >> q) {
            if (q >= 0 && q<=9) {
                cout<<"Coordonnée 2 [0:9]: ";
                if (cin >> r) {
                    if (r >= 0 && r<=9) {
                        cout<<"Coordonnée 3 [0:9]: ";
                        if (cin >> s) {
                            if (s >= 0 && s<=9) {
                                //les 3 coordonnées sont valides, on continue...
                                unsigned int zone ;
                                cout << "      \\ 1 /\n"
                                "   6   \\ /   2\n"
                                "------- + -------\n"
                                "   5   / \\   3\n"
                                "      / 4 \\\n";
                                cout << "Veillez choisir la zone de la case ou placer la tuile : ";
                                if (cin >>zone){
                                    if (zone>=1 && zone<=6){
                                        //Attribution du signe en fonction de la zone
                                        switch (zone) {
                                            case 1 :
                                                r= (-r);
                                                break;
                                            case 2 :
                                                r= (-r);
                                                s= (-s);
                                                break;
                                            case 3 :
                                                s= (-s);
                                                break;
                                            case 4 :
                                                q= (-q);
                                                s= (-s);
                                                break;
                                            case 5 :
                                                q = (-q);
                                                break;
                                            case 6 :
                                                q = (-q);
                                                r = (-r);
                                                break;
                                        }
                                        HexagoneCoord a;
                                        a.q=q;
                                        a.r=r;
                                        a.s=s;

                                        //Choix de l'orientation de la tuile
                                        cout <<"Choisissez l'orientation de la tuile :\n1 : droite + bas-droite\n2 : droite + haut-droite\n";
                                        unsigned int orientation ;
                                        if (cin >>orientation){
                                            if (orientation ==1 || orientation == 2){
                                                //changement si nécessaire
                                                if (orientation != jeu[tuileIndice].getOrientation()){
                                                    jeu[tuileIndice].changeorientation();
                                                }
                                                
                                                if (!plateau.placerTuile(jeu[tuileIndice],a, pierres)){
                                                    cout<<"Placement non valide\n";
                                                }
                                                else {
                                                    saisieValide=true;
                                                    unsigned int coutTuile = tuileIndice;
                                                    // IllustreArchitecte
                                                    // le joueur paye 
                                                    pierres -= coutTuile;

                                                    // la Partie décide quoi faire des pierres (solo ou non)
                                                    Partie::getInstance().donnerPierresArchitecte(coutTuile);
                                                    //On enlève la tuile du jeu
                                                    jeu.erase(jeu.begin() +tuileIndice);
                                                }
                                            }
                                            else cout << "Il faut choisir entre 1 et 2\n";
                                        }
                                        else cout << "Nombre invalide\n";
                                        
                                    }
                                    else cout <<"Zone non définie\n";
                                }
                                else cout <<"Nombre invalide\n";
                            }
                            else cout <<"Coordonnée invalide\n";
                        }
                        else cout <<"Nombre invalide\n";
                    }
                    else cout<<"Coordonnée invalide\n";
                }
                else cout<<"Nombre Invalide\n";      
            }
            else {
                cout << "Coordonnée invalide\n";
            }
        }
        else cout << "Nombre invalide\n";
    }
    plateau.dessinerPlateau(7);
}