#pragma once
#include "tokens.h"
#include <stdlib.h>
#include <string.h>

size_t parse_line(const char* line, Token** tokens_out);
