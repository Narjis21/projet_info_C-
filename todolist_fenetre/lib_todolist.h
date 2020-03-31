#ifndef LIB_TODOLIST_H
#define LIB_TODOLIST_H


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <QFile>
#include <QListWidget>
#include "classe.h"
using namespace std;

/* Ordre des caractéristiques dans le fichier sauvegarde:
Identifiant;Titre;Deadline;Description;Index Priorités;Index Status;sous tâches sous forme "sstache1-sstache2"; commentaires "com1-com2"; Date de création*/




/*Fonctions annexes de conversions:
- conversion vector<string>/string
- conversion int/string*/

void split(const string &chaine, char delimiteur, vector<string> &elements){
    stringstream ss(chaine);
    string sousChaine;
    while (getline(ss, sousChaine, delimiteur))
    {
    elements.push_back(sousChaine);
    }
}

vector<string> split_vect(const string &chaine, char delimiteur) {
    vector<string> elements;
    split(chaine, delimiteur, elements);
    return elements;
}

string vectToStr(vector<string> sous_taches) {
    ostringstream ligne;
    for (unsigned int i = 0; i != sous_taches.size();++i) {
        ligne<<sous_taches[i]<<'-';
    }
    return ligne.str();
}



task strToTask(const string chaine) {
    vector<string> elements = split_vect(chaine, ';');
    int id;
    stringstream identifiant(elements[0]);
    identifiant >> id;
    vector<string> soustaches = split_vect(elements[6], '-');
    int ipriorite;
    stringstream prio(elements[4]);
    prio >> ipriorite;
    task tache_correspondante(id, elements[1], elements[2], ipriorite, soustaches, elements[9].c_str());
    stringstream stat(elements[5]);
    stat >> tache_correspondante.indic_status;
    tache_correspondante.commentaries= split_vect(elements[7], '-');
    tache_correspondante.description=elements[3];
    tache_correspondante.progression=stof(elements[8]);
    tache_correspondante.str_closure=elements[10];
    return tache_correspondante;
}

string taskToStr(const task tache) {
    ostringstream ligne;
    ligne<<tache.id<<';'<<tache.title<<';'<<tache.deadline<<';'<<tache.description<<';'
    <<tache.indic_priority<<';'<<tache.indic_status<<';'<<vectToStr(tache.subtask)<<';'
    <<vectToStr(tache.commentaries)<<';'<<tache.progression<<';'<<tache.str_creation<<';'
    <<tache.str_closure<<';';
    return ligne.str();
}

string nbToStr(int nombre){
    ostringstream a;
    a << nombre;
    return a.str();
}

/*int strToNb(string ligne){
    stringstream a(ligne);
    int nombre(0);
    a >> nombre;
    return nombre;
}*/ //stoi

/*Fonction annexe donnant le temps restant avant la deadline d'une tâche*/
string time_left(task tache) {
        string message;
        if(tache.deadline == "Aucune") {
            return "Pas de date limite.\n";
        }
            time_t timestamp_actuel = time(NULL);
            vector<string> jour_mois_an = split_vect(tache.deadline, '.');
            tm struct_deadline;
            struct_deadline.tm_mday=stoi(jour_mois_an[0]);struct_deadline.tm_mon = stoi(jour_mois_an[1])-1;
            struct_deadline.tm_year = stoi(jour_mois_an[2])-1900;struct_deadline.tm_sec = 0;struct_deadline.tm_min = 0;
            struct_deadline.tm_hour = 0;
            time_t timestamp_deadline = mktime(&struct_deadline);
            int s = difftime(timestamp_deadline, timestamp_actuel);
            message = "Il vous reste "+nbToStr((s/604800))+" semaine(s), "+nbToStr(((s%604800)/86400)+1)+" jour(s) et "+nbToStr(((s%604800)%86400)/3600)+" heure(s).\n";
            return message;
    }


/*Fonctions annexes pour la sauvegarde sous format .txt*/

/*Remplacer_ligne ne remplit plus totalement sa fonction, les fonctions remove et rename ne marchent pas à l'intérieur alors j'ai décidé de les sortir et de les ajouter à la suite de l'appel de remplacer_ligne dans saving. Pour une certaine raison, ça marche. */

void remplacer_ligne(string fichier, int nb_ligne, string nouvelle_ligne) {
    ifstream lecture(fichier.c_str());
    ofstream ecriture("fichier_bis.txt");
    string ligne;
    int i = 1;
    while(getline(lecture, ligne) and i!=nb_ligne) {
        ecriture << ligne << endl;
        i++;
    }
    ecriture << nouvelle_ligne <<endl;
    while(getline(lecture, ligne)) {
        ecriture << ligne <<endl;
    }
}

string saving(task tache, string mode = "create") {
    string ligne = taskToStr(tache);
    if(mode=="create"){
        ofstream ecriture("sauvegarde.txt", ios::app); //On n'écrase pas le fichier, on rajoute les informations à la fin
        ecriture<<ligne<<endl;
        }
    else if(mode=="modify"){
        remplacer_ligne("sauvegarde.txt", tache.id, ligne);
        QFile::remove("sauvegarde.txt");
        QFile::rename("fichier_bis.txt", "sauvegarde.txt");
    }

    string message;
    message = "Sauvegarde:\n\n"+ligne+"\n\n";
    if (tache.str_closure==" "){
        message = message + time_left(tache);
    }
    else{
        message = message + "La tâche a été effectuée et/ou clôturée le "+tache.str_closure+".\n";
    }
    return message;
}

/*Fonctions annexes pour récupérer le dernier id et pour assigner les id aux nouvelles tâches créées*/

int read_last_id() {
    int identifiant;
    ifstream lire_identifiant("sauvegarde.txt");
    lire_identifiant.seekg(0, ios::beg); /*Le dernier identifiant stocké en première ligne du fichier sauvegarde*/
    lire_identifiant>>identifiant;
    return identifiant;
}

int assign_id() {
    int identifiant = read_last_id();
    identifiant++; /*L'identifiant de chaque tâche sera égal au nombre de fois où
        on a executé le programme pour créer une tâche */
    remplacer_ligne("sauvegarde.txt", 1, nbToStr(identifiant)); /*On met à jour le dernier identifiant*/
    QFile::remove("sauvegarde.txt");
    QFile::rename("fichier_bis.txt", "sauvegarde.txt");
    return identifiant;
}


/*Fonctions donnant les indexes de la liste correspondant aux différents niveaux de priorité/status*/
int index_priorite(string priorite) {
    vector<string> priorities;
    priorities.push_back("Facultative"); priorities.push_back("Normale"); priorities.push_back("Urgente");
    int index(0);
        while (priorities[index]!=priorite)
        {
            index++;
        }
    return index;
}

int index_status(string status) {
    vector<string> statuts;
    statuts.push_back("Prévue"); statuts.push_back("Commencée"); statuts.push_back("En progression"); statuts.push_back("Terminée");
    int index(0);
        while (statuts[index]!=status)
        {
            index++;
        }
    return index;
}

/*Fonction annexes booléennes pour indiquer si les éléments d'une liste correspondent à ceux qu'on veut et pour indiquer si le fichier sauvegarde existe déjà*/

bool correspondance(vector<string> ligne_decoupee, vector<int> indexes, vector<string> criteres){
    bool rep = true;
    for(unsigned int i=0; i<criteres.size(); i++){
        if(ligne_decoupee[indexes[i]]!=criteres[i]){
            rep = false;
        }
    }
    return rep;
}

bool exist(string nom_du_fichier) {
    ifstream lecture(nom_du_fichier);
    return !lecture.fail();
}


/*Fonctions ajoutées/modifiées pour l'interface graphique*/

task create_task(int identifiant, vector<string> consignes) {
        string date;
        if (consignes[1]==""){
            date = "Aucune";
        }
        else {
            date = consignes[1];
        }

        vector<string> sous_taches;
        sous_taches = split_vect(consignes[5], ',');
        /*if (consignes[3]==""){
            sous_taches = split_vect(" ", ' ');

        }
        else {

        }*/


        task tache_cree(identifiant, consignes[0], date, index_priorite(consignes[2]), sous_taches);


        tache_cree.description = consignes[4];

        vector<string> commentaires = split_vect(consignes[5], ',');
        tache_cree.commentaries = commentaires;
        tache_cree.change_status(consignes[6]);
        tache_cree.change_progression(stof(consignes[7]));

        return tache_cree;
}

QVector<QString>* list_carac(int i){
    QVector<QString>* liste_caracteristiques = new QVector<QString>;

    string ligne;
    ifstream lire_taches("sauvegarde.txt");
    std::getline(lire_taches, ligne); //On se place à la deuxième ligne pour commencer à lire les titres des différentes tâches
    while (std::getline(lire_taches,ligne)){ //Afficher la carac séléctionnée par i des tâches existantes
        vector<string> decoupe_ligne = split_vect(ligne, ';');
        liste_caracteristiques->push_back(QString::fromStdString(decoupe_ligne[i]));
    }
    return liste_caracteristiques;

}

task select_task(int identifiant){
    string ligne;
    int i = 2;
    ifstream lecture("sauvegarde.txt");
    getline(lecture, ligne); //On se place en deuxième ligne pour commencer à lire
    while(getline(lecture, ligne) and i!=identifiant) {
        i++;
    }
    task res = strToTask(ligne);
    return res;
}



#endif // LIB_TODOLIST_H
