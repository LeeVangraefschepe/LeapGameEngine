#include "ParticleRenderer.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "../../Graphics/Material.h"
#include "../../GameContext/GameContext.h"
#include "../../GameContext/Timer.h"
#include "../Transform/Transform.h"
#include "../../Graphics/Texture.h"

#include <Interfaces/IRenderer.h>
#include <Interfaces/IMeshRenderer.h>
#include <Interfaces/IMesh.h>
#include <Shaders/Billboard.h>
#include <Camera.h>

#include <random>

leap::ParticleRenderer::ParticleRenderer()
{
	// Create a meshrenderer and a mesh
	m_pRenderer = ServiceLocator::GetRenderer().CreateMeshRenderer();
	m_pRenderer->SetMesh(ServiceLocator::GetRenderer().CreateMesh());
	m_pRenderer->SetAsPointRenderer();

	// Create a billboard material and apply it to the meshrenderer
	const Material billboardMaterial{ Material::Create<graphics::shaders::Billboard>("ParticleSystem", true) };
	m_pRenderer->SetMaterial(billboardMaterial.GetInternal());
}

void leap::ParticleRenderer::SetTexture(const Texture& texture)
{
	m_pRenderer->GetMaterial()->SetTexture("gTexture", texture.GetInternal());
}

void leap::ParticleRenderer::Update()
{
	EvaluateParticleAmount();
	SpawnParticles();	
	UpdateParticles();
	UpdateParticleMesh();

	// Apply the inverse view matrix of the camera
	m_pRenderer->GetMaterial()->SetMat4x4("gViewInverse", ServiceLocator::GetRenderer().GetCamera()->GetInverseViewMatrix());
}

void leap::ParticleRenderer::OnDisable()
{
	// Clear any mesh data in the meshrenderer when the particle system goes inactive
	m_Mesh.Clear();
	m_pRenderer->GetMesh()->ReloadMesh(m_Mesh);
}

void leap::ParticleRenderer::OnDestroy()
{
	ServiceLocator::GetRenderer().RemoveMeshRenderer(m_pRenderer);
}

void leap::ParticleRenderer::EvaluateParticleAmount()
{
	// Update the amount of particles if the maxParticles variable has been changed
	if (m_Particles.size() == m_Settings.maxParticles) return;

	m_Particles.resize(m_Settings.maxParticles);
}

void leap::ParticleRenderer::SpawnParticles()
{
	// Update the time since a particle was spawned
	m_TimeSinceParticle += GameContext::GetInstance().GetTimer()->GetDeltaTime();

	// Only spawn a particle if the time since a particle is above the time between particles
	float timeBetweenParticles{ 1.0f / m_Settings.particlesPerSecond };
	if (m_TimeSinceParticle < timeBetweenParticles) return;

	// Reset the timer and spawn a new particle
	m_TimeSinceParticle -= timeBetweenParticles;
	SpawnParticle();
}

void leap::ParticleRenderer::SpawnParticle()
{
	// There cannot be more active particles as there are particles in memory
	if (m_NrActiveParticles == m_Particles.size()) return;

	// Create a random number creator
	std::random_device rd{};
	std::mt19937 gen{ rd() };
	std::uniform_real_distribution<float> rnd{ 0.0f, 1.0f };

	// Get an inactive particle
	Particle& curParticle{ m_Particles[m_Particles.size() - 1] };

	// Enable the particle
	curParticle.active = true;

	// Calculate the particle spawn position
	curParticle.position = GetTransform()->GetWorldPosition();
	const float latitude{ acosf(2.0f * rnd(gen) - 1.0f) - glm::pi<float>() / 2.0f };
	const float longitude{ 2.0f * glm::pi<float>() * rnd(gen) };
	const float radiusAlpha{ sqrtf(rnd(gen)) };
	const float radius{ (1.0f - radiusAlpha) * m_Settings.minSpawnRadius + radiusAlpha * m_Settings.maxSpawnRadius };
	curParticle.position.x += cosf(latitude) * cosf(longitude) * radius;
	curParticle.position.y += cosf(latitude) * sinf(longitude) * radius;
	curParticle.position.z += sinf(latitude) * radius;

	// Calculate a lifetime for the particle and reset the previous lifetime
	const float lifetimeAlpha{ rnd(gen) };
	curParticle.lifetime = (1.0f - lifetimeAlpha) * m_Settings.minLifetime + lifetimeAlpha * m_Settings.maxLifetime;
	curParticle.curLifetime = 0.0f;

	// Calculate a start and end velocity
	const float startVelocityAlpha{ rnd(gen) };
	const float endVelocityAlpha{ rnd(gen) };
	curParticle.startVelocity = (1.0f - startVelocityAlpha) * m_Settings.minStartVelocity + startVelocityAlpha * m_Settings.maxStartVelocity;
	curParticle.endVelocity = (1.0f - endVelocityAlpha) * m_Settings.minEndVelocity + endVelocityAlpha * m_Settings.maxEndVelocity;

	// Calculate scale and scale over time
	const float scaleAlpha{ rnd(gen) };
	const float scaleOverTimeAlpha{ rnd(gen) };
	curParticle.size = (1.0f - scaleAlpha) * m_Settings.minSize + scaleAlpha * m_Settings.maxSize;
	curParticle.scalePerSecond = (1.0f - scaleOverTimeAlpha) * m_Settings.minScalePerSecond + scaleOverTimeAlpha * m_Settings.maxScalePerSecond;

	// Calculate rotation and rotation per second
	const float rotationAlpha{ rnd(gen) };
	const float rotationPerSecondAlpha{ rnd(gen) };
	curParticle.rotation = (1.0f - rotationAlpha) * m_Settings.minRotation + rotationAlpha * m_Settings.maxRotation;
	curParticle.rotationPerSecond = (1.0f - rotationPerSecondAlpha) * m_Settings.minRotationPerSecond + rotationPerSecondAlpha * m_Settings.maxRotationPerSecond;

	// Move this particle to the end of the list of active particles
	//  and update the number of active particles
	std::swap(curParticle, m_Particles[m_NrActiveParticles++]);
}

void leap::ParticleRenderer::UpdateParticles()
{
	for (Particle& particle : m_Particles)
	{
		// The moment the loop hits an inactive particle, 
		//	all the other particles are also inactive so we can stop the loop
		if (!particle.active) break;

		const float deltaTime{ GameContext::GetInstance().GetTimer()->GetDeltaTime() };

		// Update the lifetime of the particle and disable it if it exceeds the max lifetime
		particle.curLifetime += deltaTime;
		if (particle.curLifetime > particle.lifetime)
		{
			particle.active = false;
			// Swap the current particle with the last active particle
			std::swap(particle, m_Particles[--m_NrActiveParticles]);
			continue;
		}

		// Calculate the percentage of the particle's lifespan
		const float lifePercentage{ particle.curLifetime / particle.lifetime };

		// Update velocity and position
		const glm::vec3 velocity{ (1.0f - lifePercentage) * particle.startVelocity + lifePercentage * particle.endVelocity };
		particle.position += velocity * deltaTime;

		// Update color
		particle.color = (1.0f - lifePercentage) * m_Settings.startColor + lifePercentage * m_Settings.endColor;

		// Update size
		particle.size += particle.scalePerSecond * deltaTime;

		// Update rotation
		particle.rotation += particle.rotationPerSecond * deltaTime;
	}
}

void leap::ParticleRenderer::UpdateParticleMesh()
{
	// Clear any previous mesh data
	m_Mesh.Clear();

	for (int i{}; i < m_Particles.size(); ++i)
	{
		// Get the current particle
		const Particle& particle{ m_Particles[i] };

		// The moment the loop hits an inactive particle, 
		//	all the other particles are also inactive so we can stop the loop
		if (!particle.active) break;

		// Add the particle to the mesh
		ParticleVertex v{ particle.position, particle.color, particle.size, particle.rotation };
		m_Mesh.AddVertex(v);
		m_Mesh.AddIndex(i);
	}

	// Reload the mesh data in the graphics engine
	m_pRenderer->GetMesh()->ReloadMesh(m_Mesh);
}
