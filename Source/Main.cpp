unsigned int packColor(const unsigned char red,
                       const unsigned char green,
                       const unsigned char blue,
                       const unsigned char alpha = 255) {
	return (alpha << 24) + (blue << 16) + (green << 8) + red;
}

void unpackColor(const unsigned int color,
                 unsigned char& red,
                 unsigned char& green,
                 unsigned char& blue,
                 unsigned char& alpha) {
	red = (color >> 0) & 255;
	green = (color >> 8) & 255;
	blue = (color >> 16) & 255;
	alpha = (color >> 24) & 255;
}

int main(int argc, char* argv[]) {
	return 0;
}