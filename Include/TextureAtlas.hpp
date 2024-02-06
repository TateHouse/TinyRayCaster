#pragma once

#include <filesystem>
#include <vector>

namespace TinyRayCaster {
class TextureAtlas final {
public:
	explicit TextureAtlas(const std::filesystem::path& filePath);
	TextureAtlas(const TextureAtlas& other) = default;
	TextureAtlas(TextureAtlas&& other) noexcept = default;
	~TextureAtlas() = default;

public:
	TextureAtlas& operator=(const TextureAtlas& other) = default;
	TextureAtlas& operator=(TextureAtlas&& other) noexcept = default;

public:
	[[nodiscard]] unsigned int getPixel(const unsigned int textureCoordinateX,
	                                    const unsigned int textureCoordinateY,
	                                    const unsigned int textureIndex) const;
	[[nodiscard]] std::vector<unsigned int> getPixelColumn(const unsigned int textureId,
	                                                       const unsigned int textureCoordinateX,
	                                                       const unsigned int columnHeight) const;
	[[nodiscard]] unsigned int getTextureCount() const;
	[[nodiscard]] unsigned int getTextureSize() const;

private:
	int width {};
	int height {};
	unsigned int textureCount {};
	unsigned int textureSize {};
	std::vector<unsigned int> atlas;
};
}