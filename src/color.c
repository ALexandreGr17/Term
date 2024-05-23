#include "color.h"

void uctoa(unsigned char val, char* out){
	for(int i = 2; i >= 0; i--){
		out[i] = (val % 10) + '0';
		val /= 10;
	}
}

void create_color(unsigned char rgb[3], char* out){
	char* def = "\x1b[38;2;";
	int i = 0;
	for(; def[i]; i++){
		out[i] = def[i];
	}
	
	for(int j = 0; j < 3; j++){
		char buf[3] = {0};
		uctoa(rgb[j], buf);
		for(char k = 0; k < 3; k++){
			out[i] = buf[k];
			i++;
		}
		out[i] = ';';
		i++;
	}
	out[i-1] = 'm';
	out[i] = 0;
}

