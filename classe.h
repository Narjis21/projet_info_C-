#ifndef CLASSE_H
#define CLASSE_H


#include <iostream> /*input/output stream*/
#include <fstream> /*filestream : permet d'intéragir avec les fichiers*/
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include "lib_todolist.h"
using namespace std;


/* Ordre des caractéristiques dans le fichier sauvegarde:
Identifiant;Titre;Deadline;Description;Index Priorités;Index Status;sous tâches sous forme "sstache1-sstache2"; commentaires "com1-com2"; Date de création*/


class task {

    private:
    const time_t timestamp_creation = time(NULL); /*Date de création de l'objet task*/
    tm* struct_creation = localtime(&timestamp_creation);
    vector<string> priorities;
    vector<string> status;

    public:
    int id;
    int nb_subtask, indic_priority = 1, indic_status = 0;
    float progression = 0;
    const char* time_with_null_caracter = asctime(struct_creation);
    string str_creation;
    string title;
    string deadline;
    tm* struct_closure;
    string str_closure = " ";
    string description;
    vector<string> commentaries;

    vector<string> subtask;

    task() {}

    task(int identifiant, string titre, string dateLimite, int i_priorite, vector<string> sous_taches, string date_creation = "0") :
    id(identifiant), title(titre), deadline(dateLimite), indic_priority(i_priorite), subtask(sous_taches) {
        priorities.push_back("Facultative"); priorities.push_back("Normale"); priorities.push_back("Urgente");
        status.push_back("Prévue"); status.push_back("Commencée"); status.push_back("En progression"); status.push_back("Terminée");
        if (date_creation == "0"){
            /*Si la date de création est initialisée à 0, c'est qu'elle vient d'être créee et on veut garder
            la chaîne de caractères sans le pointeur nul à la fin pour éviter des problèmes lors des conversions
            task/string pour la sauvegarde et inversement.*/
            string pop(time_with_null_caracter);
            pop.pop_back();
            str_creation = pop;
        }
        else {
            str_creation = date_creation;
        }
        if (dateLimite=="."){
            deadline = "Aucune";
        }
    }

    ~task () {
    }


    void change_title(string titre) {
        this->title = titre;
    }
    void change_deadline(string dateLimite) {
        this->deadline = dateLimite;
    }
    void change_priority(string priority) {
        int index(0);
        while (this->priorities[index]!=priority)
        {
            index++;
        }
        this->indic_priority=index;
    }
    void change_commentary(vector<string> commentaire, string type_modif) {
        if (type_modif=="supprimer"){
            commentaries.pop_back();
        }
        else if (type_modif=="remplacer"){
            this->commentaries = commentaire;
        }
        if(type_modif=="ajouter"){
            for(unsigned int j = 0; j<commentaire.size(); j++){
                this->commentaries.push_back(commentaire[j]);
            }
        }
    }
    void change_description(string descr) {
        this->description = descr;
    }
    void change_subtask(vector<string> sous_tache, string type_modif) {
        if (type_modif=="supprimer"){
            subtask.pop_back();
        }
        else if (type_modif=="remplacer"){
            this->subtask = sous_tache;
        }
        if(type_modif=="ajouter"){
            for(unsigned int j = 0; j<sous_tache.size(); j++){
                this->subtask.push_back(sous_tache[j]);
            }
        }
    }
    void change_status(string stat) {
        int index(0);
        while (this->status[index]!=stat)
        {
            index++;
        }
        this->indic_status=index;
        if(stat=="Terminée"){this->close_task();} //clôture automatique de la tâche si le statut est "Terminée"
        else{}
    }

    void close_task() {
        if (str_closure==" "){
        const time_t timestamp_closure = time(NULL);
        this->struct_closure = localtime(&timestamp_closure);
        const char* tps_fin = asctime(struct_closure);
        str_closure = tps_fin;
        str_closure.pop_back();
        if (this->indic_status!= 3){change_status("Terminée");}
        else if(this->progression!=100){change_progression(100);}
        }
        else{}
    }

    void change_progression(float prog) {
        this->progression = prog;
        if (this->progression==100){ //si on indique qu'on a fini la tâche, clôture automatique
            this->close_task();
        }
        else{}
    }
};


#endif // CLASSE_H
