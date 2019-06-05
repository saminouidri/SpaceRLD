/*******************************************************************************
* Fonctions projet : FctAntirebond.ino
* Nom et classe    : Fonction anti-rebond utilisant une temporisation de boucle.
* Date de creation : Fred Sculati & Jean-Pierre Kolly, février 2016
*******************************************************************************/

#include "FctAntirebond.h"

byte Antirebond (int iEntree,byte bRead){
  boolean boInput = 0;
  
  if(bRead & ACTIF_A_1)        // Si l'entrée avec pulldown (actif à 1)
  {
    boInput = digitalRead(iEntree);
    bRead = bRead ^ ACTIF_A_1;  // Remet à 0 le bit indiquant "actif à 1"
  }
  else
  {
    boInput = !digitalRead(iEntree); // Entrée avec pulldown (active à 0)
  }
  
  if(boInput) // Si l'entrée est active
  {
    switch (bRead)
    {
      case ACTIF          : 
      case FLANC_ACTIF    : return ACTIF;
      
      case INACTIF        : return JUSTE_ACTIF;
      case JUSTE_ACTIF    : return FLANC_ACTIF;
    }
  }
  else       // Donc l'entrée est inactive
  {
    switch (bRead)
    {
      case INACTIF        :
      case FLANC_INACTIF  : return INACTIF;
      
      case ACTIF          : return JUSTE_INACTIF;
      case JUSTE_INACTIF  : return FLANC_INACTIF;
    }
  }
}
