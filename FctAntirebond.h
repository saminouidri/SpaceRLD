/*******************************************************************************
*  FctAntirebond.h
*  Fonction anti-rebond utilisant une temporisation de boucle.
*  Fred Sculati & Jean-Pierre Kolly, f�vrier 2016
*  Date de creation   :
*******************************************************************************/

#ifndef _Fct
#define _Fct

// Prototypes des fonctions
  // d�finition des valeurs pass�es et re�u par la fontion "AntiRebond"
  #define INACTIF         0 // 0: L'entr�e est non actionn�e
  #define FLANC_INACTIF   1 // 1: L'entr�e passe en position de repos
  #define ACTIF           2 // 2: L'entr�e est active
  #define FLANC_ACTIF     3 // 3: L'entr�e passe � l'�tat actif
  #define JUSTE_ACTIF     6 // 6: L'entr�e active avant antirebond (ne pas utiliser)
  #define JUSTE_INACTIF   9 // 9: L'entr�e inactive avant antirebond (ne pas utiliser)
  #define ACTIF_A_1 0b10000000 // A utiliser en faisant un "|" avec le byte en param�tre
                               // permet d'avoir une entr�e active � 1 (pulldown)

  byte Antirebond(int,byte);  // Le param�tre "int" est l'entr�e � traiter
                              // Voir les "#define" pour le byte pass�
                              // La valeur re�u correspond � l'�tat avant l'appel 
#endif                        // de la fonction
