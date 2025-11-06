#include <Partie.h>

#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

//Bien faire les setter / getter 

int main(){
    
    std::cout<<"Lancement du jeu\n";
    
    std::cout<<"Création des objets :\n";
    Partie partie;
    std::cout<<"    Mode de jeu\n";
    
    // Besoin d'une fonction qui va me créer des objets modedejeu pour chaque mode de jeu. Créer les méthodes associées nécesssaires
    //GameMode mode = ;

    //Boucle pour choisir le mode de jeu, détruire les autres mdj non utilisés

    std::cout<<"    Participant(s)\n";

    //Construire les objets participants en fonction du nbr de joueurs dans objet mode de jeu 
    //partie.initParticipants();

    //Mettre leur plateau associé, initié avec 1 case neutre à définir, 3 cases cailloux

    std::cout<<"    Partie\n";
    //Partie partie(mode);

    //Méthode pour choisir au hasard l'ordre des participants: les joueurs réels commencent toujours, donner l'ordre des participants actuels, changer de 1 l'ordre des participants(le premier devient le dernier)
    //partie.computeTurnOrder();
    //créer méthode tour suivant
    //méthode initialisation : 
        //donne un ordre de passage
        //donne le nbr de cailloux en fonction des ordres de passages

    std::cout<<"    Plateau";

        //priorité à créer le stockage du plateau

        //Méthode pour voir si une tuile(3 hexagones) peut être placée sur plateau à certaines coordonnées, passées en paramètre
        //Méthode pour placer, qui utilise la méthode de vérif au préalable
        //méthode calcul de points

    std::cout<<"    Tuile";
        //Méthode de rotation de tuile
        //méthode de chargement des 61 tuiles
        //méthode de tirage de x tuiles parmis les 61, sans remise (faire par exemple un tableau ou on retire des éléments petits à petit)
        //méthode de chargement des 61 pièces stockées

        //non prioritaire : création aléatoire des 61 tuiles (proba à déterminer)

}
