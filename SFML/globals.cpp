#include "globals.h"

// El estado del juego es �til para todos los objetos, as� que lo hacemos global.
// �Los globales son malignos, vaya que s�...!
Globals::States Globals::GAME_STATE = Globals::States::MENU; // Inicializa el estado del juego con el estado "MENU"
