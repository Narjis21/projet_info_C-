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
    for (int i = 0; i != sous_taches.size();++i) {
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
    return tache_correspondante;
}

string taskToStr(const task tache) {
    ostringstream ligne;
    ligne<<tache.id<<';'<<tache.title<<';'<<tache.deadline<<';'<<tache.description<<';'
    <<tache.indic_priority<<';'<<tache.indic_status<<';'<<vectToStr(tache.subtask)<<';'
    <<vectToStr(tache.commentaries)<<';'<<tache.progression<<';'<<tache.str_creation;
    return ligne.str();
}

string nbToStr(int nombre)
{
    ostringstream a;
    a << nombre;
    return a.str();
}

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

int read_last_id() {
    int identifiant;
    ifstream lire_identifiant("sauvegarde.txt");
    lire_identifiant.seekg(0, ios::beg);
    lire_identifiant>>identifiant;
    return identifiant;
}

int assign_id() {
    int identifiant = read_last_id();
    identifiant++; /*L'identifiant de chaque tâche sera égal au nombre de fois où 
        on a executé le programme pour créer une tâche */
    remplacer_ligne("sauvegarde.txt", 1, nbToStr(identifiant));
    return identifiant;
}

void saving(task tache, string mode = "create") {
    string ligne = taskToStr(tache);
    cout<<"\nSauvegarde:\n\n"<<ligne<<endl<<endl;
    if(mode=="create"){    
        ofstream ecriture("sauvegarde.txt", ios::app); //On n'écrase pas le fichier puisqu'il existe déjà
        ecriture<<ligne;
        }
    else if(mode=="modify"){
        remplacer_ligne("sauvegarde.txt", tache.id, ligne);
    }

}

int index_priorite(string priorite) {
    vector<string> priorities;
    priorities.push_back("Facultatif"); priorities.push_back("Normale"); priorities.push_back("Urgente");
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

bool correspondance(vector<string> ligne_decoupee, vector<int> indexes, vector<string> criteres){
    bool rep = true;
    for(int i=0; i<criteres.size(); i++){
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

#endif

