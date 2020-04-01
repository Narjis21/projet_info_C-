#ifndef LIB_TODOLIST_H
#define LIB_TODOLIST_H


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
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
void time_left(task tache) {
        if(tache.deadline == "Aucune") {
            cout<<"Pas de date limite.\n";
        }
        else {
            time_t timestamp_actuel = time(NULL);
            vector<string> jour_mois_an = split_vect(tache.deadline, '.');
            tm struct_deadline;
            struct_deadline.tm_mday=stoi(jour_mois_an[0]);struct_deadline.tm_mon = stoi(jour_mois_an[1])-1;
            struct_deadline.tm_year = stoi(jour_mois_an[2])-1900;struct_deadline.tm_sec = 0;struct_deadline.tm_min = 0;
            struct_deadline.tm_hour = 0;
            time_t timestamp_deadline = mktime(&struct_deadline);
            int s = difftime(timestamp_deadline, timestamp_actuel);
            cout<<"Il vous reste "<<((s/604800))<<" semaine(s), "<<((s%604800)/86400)<<" jour(s) et "<<(((s%604800)%86400)/3600)<<" heure(s).\n";
        }
    }


/*Fonctions annexes pour la sauvegarde sous format .txt*/

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
    remove(fichier.c_str());
    rename("fichier_bis.txt", fichier.c_str());
}

void saving(task tache, string mode = "create") {
    string ligne = taskToStr(tache);
    cout<<"\nSauvegarde:\n\n"<<ligne<<endl<<endl;
    if (tache.str_closure==" "){
        time_left(tache);
    }
    else{
        cout<<"La tâche a été effectuée et/ou clôturée le "+tache.str_closure+"."<<endl;
    }

    if(mode=="create"){
        ofstream ecriture("sauvegarde.txt", ios::app); //On n'écrase pas le fichier, on rajoute les informations à la fin
        ecriture<<ligne;
        }
    else if(mode=="modify"){
        remplacer_ligne("sauvegarde.txt", tache.id, ligne);
    }
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


#endif // LIB_TODOLIST_H
