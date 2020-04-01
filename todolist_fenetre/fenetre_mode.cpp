#include "fenetre_mode.h"
#include "lib_todolist.h"
#include <QBoxLayout>
#include <QFormLayout>


fenetreMode::fenetreMode() : QWidget(){

    this->setWindowTitle("Gestionnaire de tâches");
    this->setFixedSize(300,200);

    b_create = new boutonMode("Ajouter une nouvelle tâche.", this);
    b_modify = new boutonMode("Modifier une tâche.", this);
    b_list = new boutonMode("Lister des tâches.", this);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(b_create);
    layout->addWidget(b_modify);
    layout->addWidget(b_list);

    this->setLayout(layout);

    QObject::connect(b_create, SIGNAL(clicked()), this, SLOT(new_w_create()));
    QObject::connect(b_modify, SIGNAL(clicked()), this, SLOT(new_w_modify()));
    QObject::connect(b_list, SIGNAL(clicked()), this, SLOT(new_w_list()));
}

fenetreForm::fenetreForm(): QWidget(){


//Création des widgets utiles à la fenêtre

    title = new QLineEdit;
    deadline = new QLineEdit;
    priority = new QComboBox;
        priority->addItem("Facultative");
        priority->addItem("Normale");
        priority->addItem("Urgente");
        priority->setCurrentIndex(1);
    subtasks = new QLineEdit;
    description = new QLineEdit;
    commentary = new QLineEdit;
    status = new QComboBox;
        status->addItem("Prévue");
        status->addItem("Commencée");
        status->addItem("En progression");
        status->addItem("Terminée");
    prog_bar = new QSlider(Qt::Horizontal);
        prog_bar->setRange(0, 100);
    prog_lcd = new QLCDNumber;
        prog_lcd->setSegmentStyle(QLCDNumber::Flat);
    b_ok = new QPushButton("Ok");
        b_ok->setCursor(Qt::PointingHandCursor);
    b_annuler = new QPushButton("Annuler");
        b_annuler->setCursor(Qt::PointingHandCursor);


//Création du Layout formulaire avec ses widgets
    layout = new QFormLayout;
    layout->addRow("Titre", title);
    layout->addRow("Date Limite", deadline);
    layout->addRow("Priorité", priority);
    layout->addRow("Sous-tâches", subtasks);
    layout->addRow("Description", description);
    layout->addRow("Commentaires", commentary);
    layout->addRow("Statut", status);


//Layout boutons ok/annuler

    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(b_ok);
    hlayout->addWidget(b_annuler);

//Création du Layout grille de la fenêtre

    QVBoxLayout* layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(layout);
    layoutPrincipal->addLayout(hlayout);


    this->setLayout(layoutPrincipal);

    QObject::connect(prog_bar, SIGNAL(valueChanged(int)), prog_lcd, SLOT(display(int)));
    QObject::connect(b_annuler, SIGNAL(clicked()), this, SLOT(close()));
}

void fenetreCreate::create(){

    /*On range les réponses de l'utilisateur dans un vecteur*/
    answers.push_back((this->title->text()).toStdString());answers.push_back((this->deadline->text()).toStdString());answers.push_back((this->priority->currentText()).toStdString());
    answers.push_back((this->subtasks->text()).toStdString());answers.push_back((this->description->text()).toStdString());answers.push_back((this->commentary->text()).toStdString());
    answers.push_back((this->status->currentText()).toStdString());answers.push_back(nbToStr(this->prog_lcd->value()));

    int identifiant;
    /*On crée le fichier de sauvegarde s'il n'existe pas déjà et on attribue l'identifiant de la tâche à créer.*/
    if (exist("sauvegarde.txt")) {
        identifiant = assign_id();
    }
    else {
        ofstream ecrire_taches("sauvegarde.txt"); //On crée un nouveau fichier de sauvegarde
        identifiant = 2;
        ecrire_taches << identifiant <<endl;
    }
    string mess_affiche = saving(create_task(identifiant, answers));
    QMessageBox::information(this, "Sauvegarde", QString::fromStdString(mess_affiche));
    this->close();

}

fenetreChoixIdentifiant::fenetreChoixIdentifiant(): QWidget(){

    QPushButton* b_ok = new QPushButton("Valider");
        b_ok->setCursor(Qt::PointingHandCursor);
    QList<QString> labels; //Les titres des colonnes
        labels.push_back("Identifiant");
        labels.push_back("Titre");
    QLabel* label3 = new QLabel("Veuillez choisir l'identifiant de la tâche à modifier:");
    choixIdentifiant = new QLineEdit; //La réponse


    /*On crée la liste des tâches existantes*/

    QVector<QString>* id = list_carac(0);
    QVector<QString>* titres = list_carac(1);
    QTreeWidget* liste_id_titre = new QTreeWidget;
    liste_id_titre->setColumnCount(2);

    for(int i=0; i<id->count(); i++){
        QTreeWidgetItem* itm_id_titre = new QTreeWidgetItem(liste_id_titre);
        itm_id_titre->setText(0, id->value(i));
        itm_id_titre->setText(1, titres->value(i));
    }
    liste_id_titre->setHeaderLabels(labels);

    /*On range tout dans un layout vertical*/

    QVBoxLayout* Vlayout = new QVBoxLayout;
    Vlayout->addWidget(liste_id_titre);
    Vlayout->addWidget(label3);
    QHBoxLayout* Hlayout = new QHBoxLayout;
    Vlayout->addLayout(Hlayout);
    Hlayout->addWidget(choixIdentifiant, 5);
    Hlayout->addWidget(b_ok, 1);

    this->setLayout(Vlayout);

    QObject::connect(choixIdentifiant, SIGNAL(returnPressed()), this, SLOT(selectTask()));
    QObject::connect(b_ok, SIGNAL(clicked()), this, SLOT(selectTask()));
}

void fenetreChoixIdentifiant::selectTask(){
    w_a_modifier = new fenetreModify;

    int i = (choixIdentifiant->text()).toInt();

    //On récupère les informations de la tâche sélectionnée et on les remplit par défaut dans la fenêtre de changement
    task task_selected = select_task(i);
    w_a_modifier->title->setText(QString::fromStdString(task_selected.title));
    w_a_modifier->deadline->setText(QString::fromStdString(task_selected.deadline));
    w_a_modifier->priority->setCurrentIndex(task_selected.indic_priority);
    w_a_modifier->subtasks->setText(QString::fromStdString(vectToStr(task_selected.subtask)));
    w_a_modifier->description->setText(QString::fromStdString(task_selected.description));
    w_a_modifier->commentary->setText(QString::fromStdString(vectToStr(task_selected.commentaries)));
    w_a_modifier->status->setCurrentIndex(task_selected.indic_status);
    w_a_modifier->prog_lcd->display(task_selected.progression);
    w_a_modifier->identifiant_modif = i;
    w_a_modifier->date_closure = task_selected.str_closure;
    w_a_modifier->date_creation = task_selected.str_creation;

    w_a_modifier->show();
    this->close();
}

void fenetreModify::modify(){
    /*On range les réponses de l'utilisateur dans un vecteur*/
    answers.push_back((this->title->text()).toStdString());answers.push_back((this->deadline->text()).toStdString());answers.push_back((this->priority->currentText()).toStdString());
    answers.push_back((this->subtasks->text()).toStdString());answers.push_back((this->description->text()).toStdString());answers.push_back((this->commentary->text()).toStdString());
    answers.push_back((this->status->currentText()).toStdString());answers.push_back(nbToStr(this->prog_lcd->value()));

    task tache_modifiee = create_task(this->identifiant_modif, answers);
    if(this->date_closure!=" "){
        tache_modifiee.str_closure = this->date_closure;
    }
    tache_modifiee.str_creation = this->date_creation;

    string mess_affiche = saving(tache_modifiee,"modify");
    QMessageBox::information(this, "Sauvegarde", QString::fromStdString(mess_affiche));
    this->close();
}

void fenetreList::list(){
    /*On range les réponses de l'utilisateur dans un vecteur*/
    answers.push_back((this->title->text()).toStdString());answers.push_back((this->deadline->text()).toStdString());answers.push_back((this->description->text()).toStdString());
    answers.push_back((this->priority->currentText()).toStdString());answers.push_back((this->status->currentText()).toStdString());answers.push_back((this->subtasks->text()).toStdString());
    answers.push_back((this->commentary->text()).toStdString());answers.push_back((this->progression->text()).toStdString());

    /*On récupère les caractéristiques sélectionnantes et les indexes correspondant*/
    bool tout = true;
    for(unsigned int i=0; i<answers.size(); i++){
        if(answers[i]!=""){
            tout = false;
            carac_a_lister.push_back(answers[i]);
            indexes_carac.push_back(i+1); //correspond aux indexes des caractéristiques dans la sauvegarde
        }
    }

    if(tout){
        carac_a_lister.push_back("tout");
    }

    fenetre_resultat = new QWidget;

    QTreeWidget* liste = list_task(carac_a_lister, indexes_carac, fenetre_resultat);
    QVBoxLayout* Vlayout = new QVBoxLayout;
    Vlayout->addWidget(liste);

    fenetre_resultat->setLayout(Vlayout);
    fenetre_resultat->show();
    //QMessageBox::information(this, "Caractéristiques listées", QString::fromStdString(vectToStr(carac_a_lister)));
    this->close();
}
