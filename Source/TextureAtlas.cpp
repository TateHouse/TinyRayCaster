#include "TextureAtlas.hpp"

#include "Color.hpp"
#include "STBImage.hpp"

namespace TinyRayCaster {
TextureAtlas::TextureAtlas(const std::filesystem::path& filePath) {
	if (!std::filesystem::exists(filePath)) {
		throw std::filesystem::filesystem_error {"TextureAtlas file does not exist.", std::error_code {}};
	}
	
	if (!filePath.has_filename()) {
		throw std::filesystem::filesystem_error {"No filename provided.", std::error_code {}};
	}
	
	if (!filePath.has_extension() || filePath.extension() != ".png") {
		throw std::filesystem::filesystem_error {"A .png extension is required.", std::error_code {}};
	}
	
	auto channelCount {-1};
	auto* data {stbi_load(filePath.string().c_str(), &width, &height, &channelCount, 0)};
	
	if (!data) {
		throw std::runtime_error {"Failed to load texture."};
	}
	
	constexpr auto expectedChannelCount {4};
	if (channelCount != expectedChannelCount) {
		stbi_image_free(data);
		throw std::runtime_error {"TextureAtlas must have 4 channels."};
	}
	
	textureCount = width / height;
	textureSize = width / textureCount;
	if (width != height * textureCount) {
		stbi_image_free(data);
		throw std::runtime_error {"TextureAtlas file must contain N square textures packed horizontally."};
	}
	
	atlas = std::vector<unsigned int>(width * height);
	
	for (std::size_t rowIndex {0}; rowIndex < height; ++rowIndex) {
		for (std::size_t columnIndex {0}; columnIndex < width; ++columnIndex) {
			const auto textureIndex {rowIndex * width + columnIndex};
			const auto red {static_cast<std::byte>(data[textureIndex * expectedChannelCount + 0])};
			const auto green {static_cast<std::byte>(data[textureIndex * expectedChannelCount + 1])};
			const auto blue {static_cast<std::byte>(data[textureIndex * expectedChannelCount + 2])};
			const auto alpha {static_cast<std::byte>(data[textureIndex * expectedChannelCount + 3])};
			const TinyRayCaster::Color color {red, green, blue, alpha};
			atlas[textureIndex] = color.getColor();
		}
	}
	
	stbi_image_free(data);
}

unsigned int TextureAtlas::getPixel(const unsigned int textureCoordinateX,
                                    const unsigned int textureCoordinateY,
                                    const unsigned int textureIndex) const {
	if (textureIndex >= textureCount) {
		throw std::out_of_range {"TextureAtlas index out of range."};
	}
	
	const auto atlasIndex {textureCoordinateX + textureIndex * textureSize + textureCoordinateY * width};
	return atlas[atlasIndex];
}

std::vector<unsigned int> TextureAtlas::getPixelColumn(const unsigned int textureId,
                                                       const unsigned int textureCoordinateX,
                                                       const unsigned int columnHeight) const {
	if (textureId >= textureCount) {
		throw std::out_of_range {"TextureAtlas index out of range."};
	}
	
	if (textureCoordinateX > textureSize) {
		throw std::out_of_range {"TextureAtlas coordinates out of range."};
	}
	
	std::vector<unsigned int> column(columnHeight);
	for (std::size_t columnY {0}; columnY < columnHeight; ++columnY) {
		column[columnY] = getPixel(textureCoordinateX, (columnY * textureSize) / columnHeight, textureId);
	}
	
	return column;
}

[[nodiscard]] unsigned int TextureAtlas::getTextureCount() const {
	return textureCount;
}

[[nodiscard]] unsigned int TextureAtlas::getTextureSize() const {
	return textureSize;
}
}