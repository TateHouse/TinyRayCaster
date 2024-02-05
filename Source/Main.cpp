#include <cstddef>

unsigned int packColor(const std::byte red,
                       const std::byte green,
                       const std::byte blue,
                       const std::byte alpha) {
	return (static_cast<unsigned int>(red) << 0) +
	       (static_cast<unsigned int>(green) << 8) +
	       (static_cast<unsigned int>(blue) << 16) +
	       (static_cast<unsigned int>(alpha) << 24);
}

void unpackColor(const unsigned int color,
                 std::byte& red,
                 std::byte& green,
                 std::byte& blue,
                 std::byte& alpha) {
	red = static_cast<std::byte>((color >> 0) & 255);
	green = static_cast<std::byte>((color >> 8) & 255);
	blue = static_cast<std::byte>((color >> 16) & 255);
	alpha = static_cast<std::byte>((color >> 24) & 255);
}

int main(int argc, char* argv[]) {
	return 0;
}