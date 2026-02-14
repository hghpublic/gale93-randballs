#include "texturemanager.hpp"

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

void TextureManager::load(const std::string & name)
{
	textures[name] = sf::Texture();
	[[maybe_unused]] bool result = textures[name].loadFromFile(name);

	textures[name].setSmooth(true);
}

sf::Texture& TextureManager::get(const std::string & name)
{
	auto res = textures.find(name);
	if (res != textures.end())
		return res->second;

	load(name);
	return textures[name];
}
