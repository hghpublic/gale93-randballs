#include "engine/engine.hpp"
#include "gamescene.hpp"

#include "systems/rendersystem.hpp"
#include "systems/usersystem.hpp"
#include "systems/movesystem.hpp"
#include "systems/colorsystem.hpp"
#include "systems/collisionsystem.hpp"
#include "systems/teleportsystem.hpp"

#include "events/spawnball.hpp"
#include "events/freearea.hpp"
#include "events/escapefromarea.hpp"
#include "events/spawnportal.hpp"

#include "engine/utils.hpp"


GameScene::GameScene() : Scene("game"), instructions(font)
{
	[[maybe_unused]] auto result = font.openFromFile("media/DejaVuSans.ttf");

	instructions.setFont(font);
	instructions.setScale(sf::Vector2f(0.5f, 0.5f));

	sf::String str = "[T] Toggle text\n";
	str += "[Left Mouse] Press and direct to spawn balls\n";
	str += "[A] Spawn 500 balls\n";
	str += "[Q] Hold and cast away all balls in a zone\n";
	str += "[Middle Mouse] Spawn a portal\n";
	str += "[Right Mouse] Collapse balls in a zone\n";
	str += "[Space] Delete balls in an zone\n";
	str += "~ Rules ~\n";
	str += "Each ball randomly changes its color.\nOn collision if they have the same color they merge, otherwise bounce";
	instructions.setString(str);
}


GameScene::~GameScene()
{
}

void GameScene::init()
{
	em.init(engine);

	em.addSystem(std::make_unique<UserSystem>());
	em.addSystem(std::make_unique<MoveSystem>());
	em.addSystem(std::make_unique<CollisionSystem>());
	em.addSystem(std::make_unique<ColorSystem>());
	em.addSystem(std::make_unique<Teleportsystem>());

	em.addRenderSystem(std::make_unique<RenderSystem>(&engine->getWindow()));
}

sf::Vector2f GameScene::getMousePosition()
{
	return static_cast<sf::Vector2f>(sf::Mouse::getPosition(engine->getWindow()));
}

void GameScene::update()
{
	while (auto event = window->pollEvent())
	{
		handleDefaultEvents(*event);

		if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button == sf::Mouse::Button::Left)
			{
				directional_spawn = true;
				click_position = getMousePosition();
			}
			else if (mouseButtonPressed->button == sf::Mouse::Button::Right)
				em.getEventDispatcher()->trigger(GameEvent::FreeArea(getMousePosition(), true));
			else if (mouseButtonPressed->button == sf::Mouse::Button::Middle)
				em.getEventDispatcher()->trigger<GameEvent::SpawnPortal>(getMousePosition());
		}
		else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseButtonReleased->button == sf::Mouse::Button::Left)
				directional_spawn = false;
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->code == sf::Keyboard::Key::A)
				for (int i = 0; i < 500; i++)
					em.getEventDispatcher()->trigger<GameEvent::SpawnBall>();
			else if (keyPressed->code == sf::Keyboard::Key::Space)
				em.getEventDispatcher()->trigger(GameEvent::FreeArea(getMousePosition(), false));
			else if (keyPressed->code == sf::Keyboard::Key::T)
				show_text = !show_text;
		}
		else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyEvent->code == sf::Keyboard::Key::Q)
				em.getEventDispatcher()->trigger(GameEvent::EscapeFromArea(getMousePosition(), true));
		}
		else if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>())
		{
			if (keyEvent->code == sf::Keyboard::Key::Q)
				em.getEventDispatcher()->trigger(GameEvent::EscapeFromArea(getMousePosition(), false));
		}
	}
}

void GameScene::fixedupdate(const float dt)
{
	directional_spawn_acc += dt;
	if (directional_spawn && directional_spawn_acc > 0.1f)
	{
		auto mouse = getMousePosition();
		if (mouse != click_position)
		{
			directional_spawn_acc = 0.f;
			em.getEventDispatcher()->trigger(GameEvent::SpawnBall(click_position, utils::normalize(click_position - mouse)));
		}
	}

	em.onUpdate(dt);
}

void GameScene::render(const float alpha_lerp)
{
	window->clear(sf::Color(83, 83, 83));

	em.onRender(alpha_lerp);
	if (show_text)
		window->draw(instructions);

	window->display();
}
