# LogicSimulator – Simulateur de circuits logiques

## Description
LogicSimulator est un simulateur graphique pour les circuits logiques élémentaires (AND, OR, XOR, NOT).  
Il permet de saisir une équation booléenne, de visualiser l’arbre logique avec des portes PNG, et de générer les chronogrammes des signaux.

## Fonctionnalités
- Saisie d’équations booléennes via l’interface
- Tokenization et parsing de l’expression
- Chargement des vecteurs d’entrée depuis un fichier texte
- Affichage de l’arbre logique avec portes PNG
- Génération de chronogrammes des signaux

## Installation
1. Ouvrir le projet dans Visual Studio
2. Compiler et exécuter
3. Saisir une équation logique et charger un fichier de signaux
4. Cliquer sur **Simuler** pour voir l’arbre logique et les chronogrammes

## Technologies utilisées
- C++ avec MFC pour l’interface graphique
- GDI+ pour le dessin des portes et chronogrammes

## Exemple
Saisie : `(X AND Y) OR Z`  
Affichage : Arbre logique et chronogrammes pour X, Y, Z et la sortie S.


<img width="958" height="871" alt="image" src="https://github.com/user-attachments/assets/eab52993-7f1d-4fa0-8f10-a8b70f5e88b8" />


## License
Ce projet est sous licence MIT.
