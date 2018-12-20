## Evaluation expression

    (1) vérification syntaxe pour éliminer erreurs critiques

        1a) appariement parenthèses
        1b) opérateurs 2 arguments
        1c) copie de travail 

    (2) evaluation sur la copie de travail
      2a) réduction de toutes les parenthèses (4) à partir [start] = [0]
      2b) calcul expression 0 à fin (5) 
      2c) libérer copie de travail et renvoyer le résultat. 

## Réduction des parenthèses

Réduit tous les niveaux de parenthèses à partir d'un indice. 
Ne doit pas commencer à l'intérieur d'un niveau de parenthèses.
Modifie le tableau des tokens à partir d'un indice de départ.
Ce qui précède l'indice de départ dans le tableau n'est pas modifié.

Entrées :

* tableau
* indice de départ

Sorties :

* tableau modifié
* renvoie la valeur située à l'indice de départ

(4) réduction parenthèses à partir de [i]:

    4a) recherche PAROPEN à partir de [i]:
    4b) si plus de PAROPEN, terminé (retour)
    4c) PAROPEN en [i]: recherche PARCLOSE à partir de [i+1]
    
      4c-1) si PAROPEN en [j]: appel récursif (2a) à partir de j
      4c-2) si pas PARCLOSE, continuer recherche (boucle 2c)
      4c-3) sinon PARCLOSE en [j]:
        
      4c-4) calcul expression de [i+1] à [j-1] (5)
      4c-5) suppression PAROPEN [i+1] à PARCLOSE [j]
      4c-6) placement du résultat en [i] à la place du PAROPEN
      4c-7) continuer recherche PAROPEN en 4a à partir de [i+1]

## Calcul d'expression 

Calcule une epxression à partir d'un indice.
Suppose qu'il n'y a pas de parenthèses. 
Modifie le tableau des tokens entre un indice de départ et un indice de fin.
Ce qui précède l'indice de départ dans le tableau n'est pas modifié.

Utilisé par le réducteur de parenthèses et par l'évaluateur global.

Entrées :

* tableau
* indice de départ
* indice de fin

Sorties :

* tableau modifié
* renvoi résultat

(5) Calcul expression de [i] à [j] (suppose absence de parenthèse)
    
    5a) trouver opérateur de priorité la plus élevée en [i]
      5a-1) si opérateur unaire (not, minus)
        5a-1a) calculer op[i] avec arg[i+1]
        5a-1b) suppression arg[i+1]
        5a-1c) remplacer op[i] par résultat
      5a-2) sinon (op binaire) 
        5a-2a) calculer op[i] avec arg[i-1] et arg[i+1]
        5a-2b) supprimer op[i] et arg[i+1] 
        5a-2c) remplacer arg[i-1] par résultat
      5a-3) boucler en 5a
          
    fin lorsqu'il n'y a plus d'opérateurs, le token [0] est alors le résultat à renvoyer
        