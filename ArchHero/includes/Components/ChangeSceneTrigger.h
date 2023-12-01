#pragma once

#include "Component.h"
#include "Interfaces/IObserver.h"
#include <unordered_map>
#include <string>

using namespace Core;

class ChangeSceneTrigger : public Component, public IObserver<std::unordered_map<std::string, void*>>
{
public:
	ChangeSceneTrigger(Object* Owner);
	virtual ~ChangeSceneTrigger() = default;
	virtual void Start() override;
	virtual Component* Clone(Object* Owner);
	virtual void SetupClone(Component* Child);

	// Hérité via IObserver
	virtual void OnNotify(const std::unordered_map<std::string, void*>& Value) override;

};