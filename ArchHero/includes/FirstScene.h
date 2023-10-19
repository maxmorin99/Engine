#pragma once

#include "Services/Scene.h"

using namespace Core;

class FirstScene : public Scene
{
public:
	FirstScene(const char* name);
	virtual void Load() override;
};