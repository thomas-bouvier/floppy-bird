# reports

## Compte-rendu réunion n°5 (05/04/2017)

Une fuite mémoire n'a toujours pas été corrigée.

Il reste encore quelques warning dans le doxyfile.

Le makefile est a modifié pour qu'il s'adapte au systême d'exploitation.

Les 100% de couverture du code par les tests unitaires pour la version 2 sont aussi à atteindre pour la prochaine fois.


## Compte-rendu réunion n°4 (22/03/2017)

La version 1 du jeu ne compile pas automatiquement. Des ajustements sont a effectuer dans le projet code::blocks et dans le Makefile :
- Tous les LDFLAGS pour la SDL devront être remplacés par une lige générique
- Un fichier de configuration devra être crée afin de permettre, via des chemins relatifs, de retrouver automatiquement les chemins des différents fichiers (fichier de sauvegarde du niveau, sprites...)

Un fichier doxyfile permettant de générer automatiquement la documentation du projet sera ajouté.

Les tests unitaires pour la partie concernant le jeu devront être fini pour la prochiane réunion. Ils doivent recouvrir 100% du code et seront inclus dans la version du projet présente sur la branche master.


## Compte-rendu réunion n°3 (07/03/2017)

Actuellement le modèle MVC n'est pas respecté dans le jeu. Les structures de données doivent être indépendantes de la SDL ; elles sont donc à refaire, en séparant davantage les fichiers (Bird, Pipe, Camera...).

Toutes les structures de données et prototypes sont à déterminer avant de commencer à coder, afin de pouvoir mettre en place rapidement les tests unitaires.

En mode version, tous les membres d'un groupe sont normalement occupés à préparer la prochaine version. Nous fonctionnerons différemment, puisque le réseau de neurone et le traitement d'image peuvent être développés en parallèle.

Toute trace de travail, de recherche doit figurer dans le git. Dès qu'une source est consultée, une synthèse peut être effectuée et déposée sur le git.

Les documents du robots, à savoir le matériel, sa fabrication, ses fichiers mécaniques, seront également déposés sur git.

Un fichier sera ajouté, décrivant le rôle de chacun dans le projet, puisque ceux-ci ne figurent pas dans le Gantt.

Avant la prochaine réunion, le fichier .cpb de Codeblocks sera déposé sur git, afin de compiler directement le jeu, puisque la V1 sera normalement finie.

Prochaine réunion le mercredi 22/03/2017 à 16h30.


## Compte-rendu réunion n°2 (16/02/2017)

Les compte-rendus seront réalisés en format Markdown et stockés sur le gitlab.

Nous séparerons le git en deux répertoires, pour le projet tournant sur Windows et le projet tournant sur la raspberry. Ces deux projets représentent les deux grandes parties de notre cahier des charges.

Il pourra être intéressant de comparer les deux IA.

Le contenu des versions du projet doit être plus spécifique. Une version équivaut à un projet fonctionnel. Il faudra éviter de travailler sur une version avancée avant d’avoir terminé la version courante.

Il faudra revoir le cahier des charges et ajouter notamment, plus d’informations sur la première IA, un Gantt/versioning plus détaillé, les spécifications de l’écran externe, de l’automate externe.

Prochaine réunion le mardi 07/03/2017 à 15h30.


## Compte rendu réunion n°1 (10/02/2017)

Un diagramme de Gantt précis sera à réaliser et il contiendra notamment les phases de test.

Une tâche à effectuer dès le début est la définition des structures de données, de cette façon tout le monde pourra travailler en parallèle.

La branche IHM n’a pas à dépendre de la branche modèle au début.

Une seule personne pourra effectuer les push sur la branche master.

Les soutenances auront lieu les 30 et 31 mai, il serait préférable d’avoir fini le développement dès début mai.

Un fichier README détaillé sera aussi à réaliser pour permettre l’installation de notre projet.

Tâche à réaliser pour la prochaine réunion : bien définir les spécifications du projet de telle façon que l’on soit sûrs que tout le monde ait la même chose en tête, définir les différentes versions du projet ainsi que les rôles de chacun.

Prochaine réunion le jeudi 16/02/2017 à 13h30.
