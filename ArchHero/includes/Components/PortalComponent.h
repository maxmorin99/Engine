#pragma once

#include "Components/Component.h"
#include "Interfaces/IObserver.h"

class PortalComponent : public Core::Component, public Core::IObserver<std::unordered_map<std::string, void*>>
{
public:
	PortalComponent(Core::Object* Owner);
	virtual ~PortalComponent() = default;
	virtual void Start() override;

	virtual void OnNotify(const std::unordered_map<std::string, void*>& Value) override;

};