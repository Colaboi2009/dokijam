#pragma once

#include <string>
#include <SDL3/SDL.h>
#include "structs.hpp"

namespace loader::ase {
void freeAse(Asefile &f);
// make sure to call freeAse after using
void aseprite(const std::string filepath, Asefile &f);
}
