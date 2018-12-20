# design assembleur


## preprocesseur

#define <expression>

* expansion des symboles
* validité comme symbole
* stockage dans la table des symboles

#undef 

* expansion des symboles
* recherche et suppression de la table des symboles

#if <expression> = <expression>

* +1 au niveau #if
* expansion des symboles de chaque côté du =
* comparaison des deux valeurs (string)
* si pas = :
* saut au prochain #else ou #endif de même niveau (sauter par dessus les #if/#ifdef/#else/#endif imbriqués)

#ifdef <expression>

* +1 au niveau de #if
* expansion des symboles
* vérification existence du symbole résultant
* si pas trouvé :
* saut au prochain #else ou #endif de même niveau (sauter par dessus les #if/#ifdef/#else/#endif imbriqués)

#else

* erreur si pas de niveau #if
* on n'arrive pas dessus en cas de #if réussi donc saut jusqu'au #endif de même niveau (sauter par dessus les #if/#ifdef/#else/#endif imbriqués)

#endif

* erreur si pas de niveau #if
* -1 au niveau de #if

#include <expression>

* expansion des symboles
* calcul de l'expression
* si fichier inexistant, ERROR
* --> inclusion : 
  * empiler le path et la position actuelles
  * fermer
  * traiter le fichier inclus (appel récursif)
  * rouvrir le path et la position actuelle
  * sauter le #include effectué
  
#inserthex <expression>

* expansion des macros
* calcul de l'expression
* si fichier inexistant, ERROR
* ensuite voir ou réutiliser la lecture des hexfiles par les memorymodule

Fonctions:

- map des symboles->value
- expansion des symboles dans une chaine
- calcul d'expression string
