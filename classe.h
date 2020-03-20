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
Identifiant;Titre;Deadline;Description;Priorités;Status;Identifiants des sous-tâches
*/

class task {

    private:
    const time_t timestamp_creation = time(NULL);
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
    string description;
    vector<string> commentaries;
    //int temps_restant;

    vector<string> subtask;

    task() {}

    task(int identifiant, string titre, string dateLimite, int i_priorite, vector<string> sous_taches, string date_creation = "0") : 
    id(identifiant), title(titre), deadline(dateLimite), indic_priority(i_priorite), subtask(sous_taches) {
        priorities.push_back("Facultatif"); priorities.push_back("Normale"); priorities.push_back("Urgente");
        status.push_back("Prévue"); status.push_back("Commencée"); status.push_back("En progression"); status.push_back("Terminée");
        if (date_creation == "0"){
            string pop(time_with_null_caracter);
            pop.pop_back();
            str_creation = pop;
        }
        else {
            str_creation = date_creation;
        }
        if (dateLimite=="."){
            deadline = "01.01.3000"; //C'est complètement arbitraire...
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
    void add_commentary(string commentaire) {
        this->commentaries.push_back(commentaire);
    }
    void add_description(string descr) {
        this->description = descr;
    }
    void add_subtask(string sous_tache) {
            this->subtask.push_back(sous_tache);
    }
    void change_status(string stat) {
        int index(0);
        while (this->status[index]!=stat)
        {
            index++;
        }
        this->indic_status=index;
    }    
    void change_progression(float prog) {
        this->progression = prog;
    }
    void close_task() {
        const time_t timestamp_closure = time(NULL);
        this->struct_closure = localtime(&timestamp_closure);
}
    /* void time_left() {

    }
    Si j'ai le temps et la foi de le faire*/
};

#endif