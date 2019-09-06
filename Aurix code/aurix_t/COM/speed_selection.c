/**
 * @file localization.c
 * @author Guillaume RIPOLL
 * @brief Regroupe les fonctions du process Speed_selection de l'unité COM
*/
#include "com.h"
#include "speed_selection.h"

void com_speed_selection (Speed gui, Speed rec, Speed safety, Speed arp, Speed *speed_sp) {
	*speed_sp = gui;
}
