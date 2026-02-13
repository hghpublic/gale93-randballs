#pragma once

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

class BaseSystem
{
protected:
	class Engine* engine;
	entt::registry* registry;
	entt::dispatcher* eventDispatcher;

	inline virtual void onInit() {}
public:
	BaseSystem();
	~BaseSystem();

	void init(Engine *e, entt::registry& r, entt::dispatcher& d);

	virtual void update(const float dt);
};

