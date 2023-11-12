#include "PhysXMaterial.h"

#include "PhysXEngine.h"
#include <PxPhysics.h>
#include <PxMaterial.h>

leap::physics::PhysXMaterial::PhysXMaterial(PhysXEngine* pEngine)
	: m_pMaterial{ pEngine->GetPhysics()->createMaterial(0.6f, 0.6f, 0.0f) }
{
}

leap::physics::PhysXMaterial::~PhysXMaterial()
{
	m_pMaterial->release();
}

void leap::physics::PhysXMaterial::SetDynamicFriction(float friction)
{
	m_pMaterial->setDynamicFriction(friction);
}

void leap::physics::PhysXMaterial::SetStaticFriction(float friction)
{
	m_pMaterial->setStaticFriction(friction);
}

void leap::physics::PhysXMaterial::SetBounciness(float bounciness)
{
	m_pMaterial->setRestitution(bounciness);
}

float leap::physics::PhysXMaterial::GetDynamicFriction()
{
	return m_pMaterial->getDynamicFriction();
}

float leap::physics::PhysXMaterial::GetStaticFriction()
{
	return m_pMaterial->getStaticFriction();
}

float leap::physics::PhysXMaterial::GetBounciness()
{
	return m_pMaterial->getRestitution();
}
