#pragma once

#include <string>
#include <SDL3/SDL.h>
#include "structs.hpp"

namespace loader::ase {
void aseprite(const std::string filepath, Asefile &f);
}
