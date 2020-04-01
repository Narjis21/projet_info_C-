#ifndef FENETRE_MODE_H
#define FENETRE_MODE_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QSlider>
#include <QLCDNumber>
#include <QLabel>
#include <QListWidget>
#include <QTreeWidget>
#include <QFormLayout>
#include <vector>
#include <string>
#include "classe.h"


class boutonMode : public QPushButton {

    Q_OBJECT

    public:
    boutonMode(QString label, QWidget* fenetre): QPushButton(label, fenetre) {
        this->setFixedSize(fenetre->width()-25,50);
        this->setCursor(Qt::PointingHandCursor);
    }

};

class fenetreForm : public QWidget {

    Q_OBJECT

    public:
        QFormLayout* layout;
        QLineEdit* title;
        QLineEdit* deadline;
        QComboBox* priority;
        QLineEdit* subtasks;
        QLineEdit* description;
        QLineEdit* commentary;
        QComboBox* status;
        QSlider* prog_bar;
        QLCDNumber* prog_lcd;
        QPushButton* b_ok;
        QPushButton* b_annuler;

        vector<string> answers;
        fenetreForm();
        virtual ~fenetreForm(){};
};

class fenetreCreate: public fenetreForm {

    Q_OBJECT

    public slots:
        void create();
    public:
        fenetreCreate(): fenetreForm(){
            layout->addRow("Progression", prog_bar);
            layout->addRow("", prog_lcd);
            QObject::connect(b_ok, SIGNAL(clicked()), this, SLOT(create()));
        };
        ~fenetreCreate(){};
};

class fenetreModify: public fenetreForm {

    Q_OBJECT

    public slots:
        void modify();
    public:
        int identifiant_modif;
        string date_closure;
        string date_creation;
        fenetreModify(): fenetreForm(){
            layout->addRow("Progression", prog_bar);
            layout->addRow("", prog_lcd);
            QObject::connect(b_ok, SIGNAL(clicked()), this, SLOT(modify()));
        };
        ~fenetreModify(){};
};

class fenetreChoixIdentifiant: public QWidget {

    Q_OBJECT

    private:
        QLineEdit* choixIdentifiant;
    public slots:
        void selectTask();
    public:
        QVector<QString>* caracteristiques_initiales;
        fenetreModify* w_a_modifier;
        fenetreChoixIdentifiant();
        virtual ~fenetreChoixIdentifiant(){};

};

class fenetreList : public fenetreForm {

    Q_OBJECT

    public slots:
        void list();

    public:
        QLineEdit* progression;
        QWidget* fenetre_resultat;
        vector<string> carac_a_lister;
        vector<int> indexes_carac;
        fenetreList(): fenetreForm(){
            //Rajouter de quoi initialiser les Combobox à "" et transformer la progression en QLineEdit pour simplifier
            priority->addItem("");
            priority->setCurrentIndex(3);
            status->addItem("");
            status->setCurrentIndex(4);
            progression = new QLineEdit;
            layout->addRow("Progression", progression);
            QLabel* lab = new QLabel("Entrez les caractéristiques des tâches à lister:");
            layout->insertRow(0, lab);


            QObject::connect(b_ok, SIGNAL(clicked()), this, SLOT(list()));
        };
        virtual ~fenetreList(){};
};

class fenetreMode : public QWidget {

    Q_OBJECT

    private:
        boutonMode* b_create;
        boutonMode* b_modify;
        boutonMode* b_list;
        fenetreCreate* w_create;
        fenetreChoixIdentifiant* w_modify;
        fenetreList* w_list;

    public slots:
        void new_w_create(){
            w_create = new fenetreCreate;
            w_create->show();
        }

        void new_w_modify(){
            w_modify = new fenetreChoixIdentifiant;
            w_modify->show();
        }

        void new_w_list(){
            w_list = new fenetreList;
            w_list->show();
        }
    public:
        fenetreMode();
        virtual ~fenetreMode(){};
};

#endif // FENETRE_MODE_H
