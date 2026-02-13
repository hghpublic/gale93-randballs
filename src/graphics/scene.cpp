#include "engine/engine.hpp"
#include "scene.hpp"

Scene::Scene(const std::string& name) : name(name)
{
}

Scene::~Scene()
{
}

void Scene::setEngine(Engine *engine)
{
	this->engine = engine;

	window = &engine->getWindow();
	sceneMng = &engine->getSceneManager();
	textureMng = &engine->getTextureManager();
}

void Scene::handleDefaultEvents(const sf::Event& event)
{
	if (event.is<sf::Event::Closed>())
		engine->stop();
	else if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->code == sf::Keyboard::Key::Escape)
			engine->stop();
	}
	else if (const auto* resized = event.getIf<sf::Event::Resized>())
		engine->getWindow().setView(sf::View(sf::FloatRect({0, 0}, sf::Vector2f(resized->size))));

	// todo handle here resizing window and black border bars for resolution
}