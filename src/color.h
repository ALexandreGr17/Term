#pragma once

#include <stdio.h>
#include <stdlib.h>

#define COLOR_RESET "\x1b[0m"

void create_color(unsigned char rgb[3], char* out);
