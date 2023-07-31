#include "globals.h"

// El estado del juego es útil para todos los objetos, así que lo hacemos global.
// ¡Los globales son malignos, vaya que sí...!
Globals::States Globals::GAME_STATE = Globals::States::MENU; // Inicializa el estado del juego con el estado "MENU"
