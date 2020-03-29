# projet_info_C++
Ordre des caractéristiques dans le fichier sauvegarde: 
Identifiant;Titre;Deadline;Description;Priorité;Status;sous-tâches; commentaires; progression;date de création;date de clôture;


    Ordre des arguments à rentrer à la création: "title" "deadline" "priority" "subtask1,subtask2"
"description" "commentary" "status" "progression"(seul le premier est obligatoire) ex:
./tdln.exe create "Finir le projet d'info" "31.03.2020" "Urgente" "Implémenter les fonctions annexes dans lib_todolist.h, Finir la fonction create_task"

    Ordre des arguments à rentrer à la modification: "caractéristique à modifier(en français)" "ajouter/supprimer/remplacer" "modification" ex: 
./tdln.exe modify "commentaires" "ajouter " "Projet assez long: commencer tôt., La partie create_task est terminée mais looping infini avec les cin donc changer de méthode."
Le type de modification n'est pas obligatoire pour les changements évidents (on va forcément remplacer une deadline par exemple) et pour ceux qui ne le sont pas, c'est "ajouter" par défaut, ex:
./tdln.exe modify "titre" "Finir le projet de C++ et maths info" va remplacer le titre
./tdln.exe modify "commentaires" "La partie modify est plus subtile que prévu." va ajouter un commentaire
./tdln.exe modify "sous-tâches" "supprimer" " " va supprimer la dernière sous-tâche

    Ordre des arguments à rentrer pour lister: "caractéristique concernée" "valeur" ex: 
./tdln.exe list "deadline" "31.03.2020" "priorité" "Urgente"
Il faut les rentrer avant de choisir l'identifiant de la tâche à modifier
Si on veut tout lister: ./tdln.exe list

    Lors de l'appel du main en mode creation, si une propriété est facultative mais que vous voulez en initialiser une 
autre après, entrez '.' à la place, ex rentrer la priorité mais pas la date butoire:
./tdln.exe create "Finir le projet d'info" "." "Urgente"
