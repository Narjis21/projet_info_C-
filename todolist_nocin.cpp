#include <iostream> /*input/output stream*/
#include <fstream> /*filestream : permet d'intéragir avec les fichiers*/
#include <string>
#include <sstream>
#include <ctime>
#include "lib_todolist.h"
using namespace std;
/* Ordre des caractéristiques dans le fichier sauvegarde: 
Identifiant;Titre;Deadline;Description;Priorité;Status;sous-tâches; commentaires; date de création; progression;
*/

/*Ordre des arguments à rentrer à la création: "title" "deadline" "priority" "subtask1,subtask2"
"description" "commentary" "status" (les quatresn derniers sont facultatifs) ex:
./tdln.exe create "Finir le projet d'info" "31.03.2020" "Urgente" "Implémenter les fonctions annexes dans lib_todolist.h, Finir la fonction create_task"*/

/*Ordre des arguments à rentrer à la modification: "caractéristique à modifier" "changement" ex: 
./tdln.exe modify "commentaires" "Projet assez long: commencer tôt., La partie create_task est terminée mais looping avec les cin, changer de méthode."*/

/*Ordre des arguments à rentrer pour lister: "caractéristique concernée" "valeur" ex: 
./tdln.exe list "deadline" "31.03.2020" "priorité" "Urgente"*/

void create_task(int identifiant, char** consignes, int taille) {
        string titre = consignes[2];


        string date;
        if (taille<=4){
            date = "01.01.3000"; //C'est complètement arbitraire...
        }
        else {
            date = consignes[3];
        }


        int ind_priorite;
        if (taille>=5){
            string priorite = consignes[4];
            ind_priorite = index_priorite(priorite);
        }
        else {
            ind_priorite = 1;
        }


        vector<string> sous_taches;
        if (taille<=6) {
            vector<string> sous_taches = split_vect(" ", ' ');
        }
        else if (consignes[5]=="."){
            vector<string> sous_taches = split_vect(" ", ' ');
           
        }
        else {
            vector<string> sous_taches = split_vect(consignes[5], ',');
        }


        task tache_cree(identifiant, titre, date, ind_priorite, sous_taches);


        if (taille>=7) {
            tache_cree.add_description(consignes[6]);
        }
        if (taille>=8) {
            vector<string> commentaires = split_vect(consignes[7], ',');
            tache_cree.commentaries = commentaires;
        }
        if (taille>=9) {
            tache_cree.change_status(consignes[8]);
        }
        saving(tache_cree);
}

void modify_task(int identifiant, char** consignes, int taille) {
    ifstream lecture("sauvegarde.txt");
    string ligne_tache;
    for (int i=1; i<=identifiant; i++) {
        getline(lecture, ligne_tache);
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
        tache_modif.change_title(consignes[3]);
        break;
        case 1: //deadline
        tache_modif.change_deadline(consignes[3]);
        break;
        case 2: //description
        tache_modif.add_description(consignes[3]);        
        break;
        case 3: //priorité
        tache_modif.change_priority(consignes[3]);
        break;
        case 4: //statut
        tache_modif.change_status(consignes[3]);
        break;
        case 5:{ //sous-tâches
            vector<string> sstaches = split_vect(consignes[3], ',');
            for(int j = 0; j<sstaches.size(); j++){
            tache_modif.add_subtask(sstaches[j]);
            }
        }
        break;
        case 6:{ //commentaires
            vector<string> comm = split_vect(consignes[3], ',');
            for(int j = 0; j<comm.size(); j++){
            tache_modif.add_commentary(comm[j]);
            }
        }
        break;
        case 7: //progression
        tache_modif.change_progression(stof(consignes[3]));
        break;
    }
    saving(tache_modif, "modify");
}

void list_task(char** consignes, int taille){
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
    getline(lire_taches, ligne);
    while (getline(lire_taches,ligne)){ //Afficher tous les titres et les identifiants des tâches existantes
        vector<string> decoupe_ligne = split_vect(ligne, ';');
        if(correspondance(decoupe_ligne, indexes, criteres_a_lister)){
            std::cout<<decoupe_ligne[0]<<' '<<decoupe_ligne[1]<<endl;
        }
    }
}

int main(int argc, char** argv) {
    string mode = argv[1];
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
                getline(lire_taches, ligne); //On se place à la deuxième ligne pour commencer à lire les titres des différentes tâches
                while (getline(lire_taches,ligne)){ //Afficher tous les titres et les identifiants des tâches existantes
                    vector<string> decoupe_ligne = split_vect(ligne, ';');
                    std::cout<<decoupe_ligne[0]<<' '<<decoupe_ligne[1]<<endl;  
                }
                string str_id;
                cin >> str_id; 
                identifiant = stoi(str_id); 
                modify_task(identifiant, argv , argc);
                break;
            }
            case 'l':{
                list_task(argv, argc);            
                break;
            }
        }
        /*std::cout<<"Voulez-vous continuer? [Y/n]"<<endl;
        std::cin>>rep;
        if (rep == 'Y') {
            std::cout<<"Entrez la ligne de commande selon le mode choisi (create, modify, list)"<<endl; //On entre ce qu'on aurait mis dans argv en suivant l'ordre précisé au début du code.
            std::cin>>argv;
        }
    }*/
}