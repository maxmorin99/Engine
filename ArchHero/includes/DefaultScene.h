#pragma once

#include "Services/Scene.h"

using namespace Core;

class DefaultScene : public Scene
{
public:
	DefaultScene(const char* name);
	virtual void Load() override;
};