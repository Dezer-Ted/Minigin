﻿#include "Command.h"
#include "../../SceneObjects/GameObject.h"

dae::GameObject* dae::GameObjectCommand::GetGameObject() const
{
	return m_Owner;
}

dae::GameObjectCommand::GameObjectCommand(GameObject* gameObject) :
	m_Owner{gameObject}
{

}

void dae::Die::Execute()
{
}

dae::Die::Die(GameObject* pOwner) : GameObjectCommand(pOwner)
{
}

void dae::GainPoints::Execute()
{
}

dae::GainPoints::GainPoints(GameObject* pOwner) : GameObjectCommand(pOwner)
{
}
