#pragma once

#include "Component.h"
#include "Interfaces/IObserver.h"
#include <unordered_map>
#include <string>

using namespace Core;

class GateComponent : public Component, public IObserver<std::unordered_map<std::string, void*>>
{
public:
	GateComponent(Object* Owner);
	virtual ~GateComponent() = default;
	virtual Component* Clone(Object* Owner);
	virtual void SetupClone(Component* Child);
	virtual void Start() override;
	virtual void OnNotify(const std::unordered_map<std::string, void*>& Value) override;

private:
	int mEnemiesCount = 0;
	size_t mSoundId = 0;

	void OnOpenAnimationEnd();

public:
	inline void SetEnemiesCount(int Count) { mEnemiesCount = Count; }

	void SetAudioFile(const std::string& File);
	
};