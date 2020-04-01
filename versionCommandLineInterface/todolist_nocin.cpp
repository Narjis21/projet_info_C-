#include <iostream> /*input/output stream*/
#include <fstream> /*filestream : permet d'intéragir avec les fichiers*/
#include <string>
#include <sstream>
#include <ctime>
#include "lib_todolist.h"
using namespace std;
/* Ordre des caractéristiques dans le fichier sauvegarde: 
Identifiant;Titre;Deadline;Description;Priorité;Status;sous-tâches; commentaires; progression;date de création;date de clôture;
*/

/*Ordre des arguments à rentrer à la création: "title" "deadline" "priority" "subtask1,subtask2"
"description" "commentary" "status" "progression"(seul le premier est obligatoire) ex:
./tdln.exe create "Finir le projet d'info" "31.03.2020" "Urgente" "Implémenter les fonctions annexes dans lib_todolist.h, Finir la fonction create_task"*/

/*Ordre des arguments à rentrer à la modification: "caractéristique à modifier(en français)" "ajouter/supprimer/remplacer" "modification" ex: 
./tdln.exe modify "commentaires" "ajouter " "Projet assez long: commencer tôt., La partie create_task est terminée mais looping infini avec les cin donc changer de méthode."
Le type de modification n'est pas obligatoire pour les changements évidents (on va forcément remplacer une deadline par exemple) et pour ceux qui ne le sont pas, c'est "ajouter" par défaut, ex:
./tdln.exe modify "titre" "Finir le projet de C++ et maths info" va remplacer le titre
./tdln.exe modify "commentaires" "La partie modify est plus subtile que prévu." va ajouter un commentaire
./tdln.exe modify "sous-tâches" "supprimer" " " va supprimer la dernière sous-tâche*/

/*Ordre des arguments à rentrer pour lister: "caractéristique concernée" "valeur" ex: 
./tdln.exe list "deadline" "31.03.2020" "priorité" "Urgente"
Il faut les rentrer avant de choisir l'identifiant de la tâche à modifier
Si on veut tout lister: ./tdln.exe list*/

/*Lors de l'appel du main en mode creation, si une propriété est facultative mais que vous voulez en initialiser une 
autre après, entrez '.' à la place, ex rentrer la priorité mais pas la date butoire:
./tdln.exe create "Finir le projet d'info" "." "Urgente"*/

void create_task(int identifiant, char** consignes, int taille) {
        string titre = consignes[2];


        string date;
        if (taille<=4){
            date = "Aucune"; 
        }
        else if (consignes[3]=="."){
            date = "Aucune";
        }
        else {
            date = consignes[3];
        }


        int ind_priorite;
        if (taille>=5){
            string priorite = consignes[4];
            if (priorite=="."){
                priorite="Normale";
            }
            ind_priorite = index_priorite(priorite);
        }
        else {
            ind_priorite = 1; //Normale
        }


        vector<string> sous_taches;
        if (taille<=6) {
            vector<string> sous_taches = split_vect(" ", ' '); //Initialiser le vecteur avec un espace seulement
        }
        else if (consignes[5]=="."){
            vector<string> sous_taches = split_vect(" ", ' ');
           
        }
        else {
            vector<string> sous_taches = split_vect(consignes[5], ',');
        }


        task tache_cree(identifiant, titre, date, ind_priorite, sous_taches);


        if (taille>=7) {
            tache_cree.description = consignes[6];
        }
        if (taille>=8) {
            vector<string> commentaires = split_vect(consignes[7], ',');
            tache_cree.commentaries = commentaires;
        }
        if (taille>=9) {
            tache_cree.change_status(consignes[8]);
        }
        if (taille>=10) {
            tache_cree.change_progression(stof(consignes[9]));
        }
        saving(tache_cree);
}

void modify_task(int identifiant, char** consignes, int taille) {
    string type_modif;
    string modification;
    if (taille==4){
        type_modif = "ajouter";
        modification = consignes[3];
    }
    else {
        type_modif = consignes[3];
        modification = consignes[4];
    }
    
    ifstream lecture("sauvegarde.txt");
    string ligne_tache;
    for (int i=1; i<=identifiant; i++) {
        std::getline(lecture, ligne_tache);
    }
    task tache_modif = strToTask(ligne_tache);
    vector<string> liste_carac;
    liste_carac.push_back("titre");liste_carac.push_back("deadline");liste_carac.push_back("description");
    liste_carac.push_back("priorité");liste_carac.push_back("statut");liste_carac.push_back("sous-tâches");
    liste_carac.push_back("commentaires");liste_carac.push_back("progression");
    int i = 0;
    while(consignes[2]!=liste_carac[i]){
        i++;
    }
    switch(i) {
        case 0: //titre
            tache_modif.change_title(modification);
            break;
        case 1: //deadline
            tache_modif.change_deadline(modification);
            break;
        case 2: //description
            tache_modif.change_description(modification);        
            break;
        case 3: //priorité
            tache_modif.change_priority(modification);
            break;
        case 4: //statut
            tache_modif.change_status(modification);
            break;
        case 5:
            { //sous-tâches
                vector<string> sstaches = split_vect(modification, ',');
                tache_modif.change_subtask(sstaches, type_modif);
            }
            break;
        case 6:
            { //commentaires
                vector<string> comm = split_vect(modification, ',');
                tache_modif.change_commentary(comm, type_modif);
                
            }
            break;
        case 7: //progression
            tache_modif.change_progression(stoi(modification));
            break;
    }
    saving(tache_modif, "modify");
}

void list_task(char** consignes, int taille){
    /*On redéfinit les listes de priorités et de statuts pour ne pas lister les indices seulement.*/
    vector<string> priorities;
    priorities.push_back("Facultative"); priorities.push_back("Normale"); priorities.push_back("Urgente");
    vector<string> statuts;
    statuts.push_back("Prévue"); statuts.push_back("Commencée"); statuts.push_back("En progression"); statuts.push_back("Terminée");


    if(taille==2){ //Cas où on veut tout lister
        string ligne;
        ifstream lire_taches("sauvegarde.txt");
        std::getline(lire_taches, ligne);
        while (std::getline(lire_taches,ligne)){
            vector<string> decoupe_ligne = split_vect(ligne, ';');  
            decoupe_ligne[4]=priorities[stoi(decoupe_ligne[4])]; //Pour afficher les priorités et statuts plutôt que les indexes
            decoupe_ligne[5]=statuts[stoi(decoupe_ligne[5])];
            for(int i=0; i<decoupe_ligne.size(); i++){
                std::cout<<decoupe_ligne[i]<<';';
            }
            if (decoupe_ligne[10]==" "){
                time_left(strToTask(ligne));
            }
            else{
                cout<<"Tâche clôturée.\n\n";
            }
            std::cout<<endl;
        }
    }

    else{
        int k = taille/2; //argc est toujours pair quand on list (critère + valeur), k = nb criteres+1
        vector<string> liste_carac;
        liste_carac.push_back("titre");liste_carac.push_back("deadline");liste_carac.push_back("description");
        liste_carac.push_back("priorité");liste_carac.push_back("statut");liste_carac.push_back("sous-tâches");
        liste_carac.push_back("commentaires");liste_carac.push_back("progression");
        vector<int> indexes;
        vector<string> criteres_a_lister;
        for (int i=1; i<k; i++) {
            int j = 0;
            while(consignes[2*i]!=liste_carac[j]){
                j++;
            }
            indexes.push_back(j+1); //On a les entiers qui nous permettent de regarder dans la sauvegarde les paramètres pertinents (pour l'exemple précédent, on obtient juste "2" pour "deadline")
            if (j+1 == 4){
                criteres_a_lister.push_back(nbToStr(index_priorite(consignes[2*i+1])));
            }
            else if (j+1 == 5){
                criteres_a_lister.push_back(nbToStr(index_status(consignes[2*i+1])));
            }
            else{
                criteres_a_lister.push_back(consignes[2*i+1]); //On garde dans une liste les criteres selectionnant les tâches à lister (ici "31.03.2020")
            }
        }
        string ligne;
        ifstream lire_taches("sauvegarde.txt");
        std::getline(lire_taches, ligne);

        bool resultat= false;
        while (std::getline(lire_taches,ligne)){ //Afficher toutes les informations des tâches vérifiant les critères
            vector<string> decoupe_ligne = split_vect(ligne, ';');
            if(correspondance(decoupe_ligne, indexes, criteres_a_lister)){
                resultat=true;
                decoupe_ligne[4]=priorities[stoi(decoupe_ligne[4])]; 
                decoupe_ligne[5]=statuts[stoi(decoupe_ligne[5])];
                for(int i=0; i<decoupe_ligne.size(); i++){
                    std::cout<<decoupe_ligne[i]<<';';
                }
                if (decoupe_ligne[10]==" "){
                    time_left(strToTask(ligne));
                }
                else{
                    cout<<"Tâche clôturée.\n";
                }
            }
        }
        if(resultat==false){
            cout<<"Aucune tâche ne correspond à votre demande."<<endl;
        }
    }
}

int main(int argc, char** argv) {
    string mode = argv[1]; //create, modify ou list
    string ligne;
    int identifiant;
        switch(mode[0]) {
            case 'c':{
                /*On crée le fichier de sauvegarde s'il n'existe pas déjà et on attribue l'identifiant de la tâche à créer.*/
                if (exist("sauvegarde.txt")) {
                    identifiant = assign_id();
                }
                else {
                    ofstream ecrire_taches("sauvegarde.txt"); //On crée un nouveau fichier de sauvegarde
                    identifiant = 2;
                    ecrire_taches << identifiant <<endl;
                }
                create_task(identifiant, argv, argc);
                break;
            }
                
            case 'm':{
                ifstream lire_taches("sauvegarde.txt");
                std::cout<<"Sélectionnez l'identifiant de la tâche à modifier: "<<endl;
                std::getline(lire_taches, ligne); //On se place à la deuxième ligne pour commencer à lire les titres des différentes tâches
                while (std::getline(lire_taches,ligne)){ //Afficher tous les titres et les identifiants des tâches existantes
                    vector<string> decoupe_ligne = split_vect(ligne, ';');
                    std::cout<<decoupe_ligne[0]<<' '<<decoupe_ligne[1]<<endl;  
                }
                string str_id;
                std::cin >> str_id; 
                identifiant = stoi(str_id); 
                modify_task(identifiant, argv , argc);
                break;
            }
            case 'l':{
                list_task(argv, argc);            
                break;
            }
        }
}
