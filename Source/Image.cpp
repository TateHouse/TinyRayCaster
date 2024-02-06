#include "Image.hpp"

#include "Color.hpp"

namespace TinyRayCaster {
Image::Image(const std::filesystem::path& filePath, unsigned int width, unsigned int height) :
		width {width}, height {height} {
	if (width == 0 || height == 0) {
		throw std::invalid_argument {"Width and height must be greater than 0."};
	}
	
	if (!std::filesystem::exists(filePath.parent_path())) {
		throw std::filesystem::filesystem_error {"Parent path does not exist.", std::error_code {}};
	}
	
	if (!filePath.has_filename()) {
		throw std::filesystem::filesystem_error {"No filename provided.", std::error_code {}};
	}
	
	if (!filePath.has_extension() || filePath.extension() != ".ppm") {
		throw std::filesystem::filesystem_error {"A .ppm extension is required.", std::error_code {}};
	}
	
	fileStream.open(filePath, std::ios::out | std::ios::binary);
	
	if (!fileStream.is_open()) {
		throw std::filesystem::filesystem_error {"Could not open file.", std::error_code {}};
	}
	
	fileStream << "P6\n" << width << " " << height << "\n255\n";
}

void Image::write(const std::vector<unsigned int>& pixels) {
	if (pixels.size() != width * height) {
		throw std::invalid_argument {"Pixel count does not match width and height."};
	}
	
	for (std::size_t pixelIndex {0}; pixelIndex < pixels.size(); ++pixelIndex) {
		const TinyRayCaster::Color color {pixels[pixelIndex]};
		const auto red {static_cast<char>(color.getRed())};
		const auto green {static_cast<char>(color.getGreen())};
		const auto blue {static_cast<char>(color.getBlue())};
		fileStream << red << green << blue;
	}
	
	fileStream.close();
}
}