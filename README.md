# Projet OCR
## Introduction
![]<img src="https://cdn.sick.com/media/ZOOM/2/82/782/IM0077782.png" alt="OCR" height="400">

Le projet OCR a pour but de détecter une grille de sudoku, de la scanner et de la résoudre. Une interface graphique supportera le scan et permettra à l'utilisateur de résoudre tous ses sudokus !

### Fonctionnalités de première soutenance

1. Implémentation traitement d'image

- [ ] Rotation manuelle de l’image
- [ ] Détection de la grille et de la position des cases ;
- [ ] Découpage de l’image (sauvegarde de chaque case sous la forme d’une image) 


2. Implémentation du [réseau de neurones](https://metalblog.ctif.com/wp-content/uploads/sites/3/2021/04/Reseau-de-neurones-pour-faire-du-Machine-Learning.jpg)

- [ ] Implémentation de l’algorithme de résolution d’un sudoku. Vous devrez implémenter cet algorithme
      dans le programme en ligne de commande solver (cf. III.2.5 Résolution d’une grille de sudoku)
- [ ]  Une preuve de concept de votre réseau de neurones. Pour cette preuve, vous réaliserez un mini réseau
       capable d’apprendre la fonction OU EXCLUSIF.
       
3. Elements devant être commencés pour la première soutenance
- [ ] Sauvegarde et chargement des poids du réseau de neurones
- [ ] Jeu d’images pour l’apprentissage
- [ ] Manipulation de fichiers pour la sauvegarde des résultats 


### Répartition des tâches

Tâche | Responsable
------------ | -------------
Traitement d'image | Clément & Vincent
Réseau de neurones | Hugo & Nicolas
