#include "engine/engine.hpp"
#include "usersystem.hpp"

#include "components/portal.hpp"
#include "components/body.hpp"
#include "components/colorable.hpp"
#include "components/renderable.hpp"
#include "components/lerpable.hpp"
#include "components/holdable.hpp"
#include "components/teleportable.hpp"

#include "engine/utils.hpp"

using namespace GameComponent;

UserSystem::UserSystem()
{
	
}

void UserSystem::onInit()
{
    // TODO: this is really bad, find a better way to connect events to member functions
	eventDispatcher->sink<GameEvent::SpawnBall>().connect<static_cast<void (UserSystem::*)(const GameEvent::SpawnBall&)>(&UserSystem::receive)>(this);
	eventDispatcher->sink<GameEvent::SpawnPortal>().connect<static_cast<void (UserSystem::*)(const GameEvent::SpawnPortal&)>(&UserSystem::receive)>(this);
	eventDispatcher->sink<GameEvent::FreeArea>().connect<static_cast<void (UserSystem::*)(const GameEvent::FreeArea&)>(&UserSystem::receive)>(this);
	eventDispatcher->sink<GameEvent::EscapeFromArea>().connect<static_cast<void (UserSystem::*)(const GameEvent::EscapeFromArea&)>(&UserSystem::receive)>(this);
}


sf::Vector2f UserSystem::generateRandomDirection()
{
	sf::Vector2f dir((std::rand() % 200) - 100, (std::rand() % 200) - 100);

	return utils::normalize(dir);
}

sf::FloatRect UserSystem::generateRect(const sf::Vector2f& position)
{
	const float size = 200;
	sf::FloatRect rect(position, sf::Vector2f(size, size));
	rect.position.y -= size * 0.5f;
	rect.position.x -= size * 0.5f;

	return rect;
}

void UserSystem::receive(const GameEvent::SpawnBall &event)
{
	auto wsize = engine->getWindow().getSize();

	auto position = event.position;
	auto direction = event.direction;

	if (event.position == sf::Vector2f())
	{
		position = sf::Vector2f(std::rand() % wsize.x, std::rand() % wsize.y);
		direction = generateRandomDirection();
	}

	float size = Body::MIN_START_SIZE + static_cast<float>(std::rand() % 2);

	auto entity = registry->create();
	registry->emplace<Body>(entity, position, direction * static_cast<float>(Body::SPEED), size);
	registry->emplace<Colorable>(entity);
	registry->emplace<Teleportable>(entity);
	registry->emplace<Renderable>(entity, position);
}

void UserSystem::receive(const GameEvent::SpawnPortal &event)
{
	static int e_link = -1; // todo remove this dirty flag

	auto entity = registry->create();
	registry->emplace<Body>(entity, event.position, sf::Vector2f(), (float)Body::PORTAL_SIZE);
	registry->emplace<Portal>(entity);
	registry->emplace<Colorable>(entity);
	registry->emplace<Renderable>(entity, event.position);

	auto& c = registry->get<Colorable>(entity);
	c.color = sf::Color(20, 20, 40);
	c.decay = sf::seconds(65536);

// FIXME
#if 0
	if (e_link != -1)
	{
		registry->get<Portal>(entity).link = e_link;
		registry->get<Portal>(e_link).link = entity;

		e_link = -1;
	}
	else
		e_link = entity;
#endif	
}


void UserSystem::receive(const GameEvent::FreeArea &event)
{
	auto border = generateRect(event.position);
	
	registry->view<Body, Colorable>().each([&](auto entity, Body &body, Colorable& colorable)
	{ 
		if (!registry->any_of<Portal>(entity) && border.contains(body.position))
			if (event.collapse)
			{
				colorable.color = sf::Color(123, 212, 80);
				colorable.decay = sf::seconds(3);
				colorable.timer = sf::Time::Zero;

				registry->emplace<Lerpable>(entity, event.position);
			}
			else
				registry->destroy(entity);
	});
}

void UserSystem::receive(const GameEvent::EscapeFromArea &event)
{
	static bool ready = true; // todo again remove this dirty flag

	if (!event.hold)
	{
		auto view = registry->view<Body, Colorable, Holdable>();
		for (auto entity : view)
		{
			auto &body = view.get<Body>(entity);
			auto &colorable = view.get<Colorable>(entity);

			body.direction = utils::normalize(body.position - event.position) * static_cast<float>(Body::SPEED) * 2.f;

			colorable.decay = sf::seconds(3);
			colorable.timer = sf::Time::Zero;
		}

		auto holdableView = registry->view<Holdable>();
		for (auto entity : holdableView)
		{
			registry->remove<Holdable>(entity);
		}
		ready = true;
	}
	else if (ready == true)
	{
		auto border = generateRect(event.position);

		registry->view<Body, Colorable>().each([&](auto entity, Body &body, Colorable& colorable)
		{
			if (!registry->any_of<Portal>(entity) && border.contains(body.position))
			{
				colorable.color = sf::Color(194, 177, 128);
				colorable.decay = sf::seconds(2048);

				registry->emplace<Holdable>(entity);
			}

		});

		ready = false;
	}


}
