#pragma once

#include "../Component.h"

#include <Data/CustomMesh.h>

#include <vec3.hpp>
#include <vec4.hpp>
#include <trigonometric.hpp>

#include <vector>

namespace leap
{
	namespace graphics
	{
		class IMeshRenderer;
	}
	class Texture;

	class ParticleRenderer final : public Component
	{
	public:
		struct EmitterSettings
		{
			unsigned int maxParticles{ 10 };
			float particlesPerSecond{ 2.0f };
			float minSize{ 0.1f };
			float maxSize{ 0.2f };
			float minScalePerSecond{ 0.5f };
			float maxScalePerSecond{ 1.0f };
			float minLifetime{ 1.0f };
			float maxLifetime{ 1.0f };
			float minSpawnRadius{ 0.0f };
			float maxSpawnRadius{ 0.5f };
			glm::vec3 minStartVelocity{};
			glm::vec3 maxStartVelocity{};
			glm::vec3 minEndVelocity{};
			glm::vec3 maxEndVelocity{};
			glm::vec4 startColor{ 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec4 endColor{ 1.0f, 1.0f, 1.0f, 0.0f };
			float minRotation{};
			float maxRotation{};
			float minRotationPerSecond{};
			float maxRotationPerSecond{};

			// Sets min and max size to the given value
			void SetSize(float size)
			{
				minSize = size;
				maxSize = size;
			}
			// Sets min and max size to the given value
			void SetScalePerSecond(float scalePerSecond)
			{
				minScalePerSecond = scalePerSecond;
				maxScalePerSecond = scalePerSecond;
			}
			// Sets min and max lifetime to the given value
			void SetLifetime(float lifetime)
			{
				minLifetime = lifetime;
				maxLifetime = lifetime;
			}
			// Sets min and max spawn radius to the given value
			void SetSpawnRadius(float spawnRadius)
			{
				minSpawnRadius = spawnRadius;
				maxSpawnRadius = spawnRadius;
			}
			// Sets (min and max) start and end velocities to the given value
			void SetVelocity(const glm::vec3& velocity)
			{
				SetStartVelocity(velocity);
				SetEndVelocity(velocity);
			}
			// Sets (min and max) start and end velocities to the given value
			void SetVelocity(float x, float y, float z)
			{
				SetStartVelocity(x,y,z);
				SetEndVelocity(x,y,z);
			}
			// Sets min and max start velocities to the given value
			void SetStartVelocity(const glm::vec3& velocity)
			{
				minStartVelocity = velocity;
				maxStartVelocity = velocity;
			}
			// Sets min and max start velocities to the given value
			void SetStartVelocity(float x, float y, float z)
			{
				minStartVelocity.x = x;
				minStartVelocity.y = y;
				minStartVelocity.z = z;
				maxStartVelocity.x = x;
				maxStartVelocity.y = y;
				maxStartVelocity.z = z;
			}
			// Sets min and max end velocities to the given value
			void SetEndVelocity(const glm::vec3& velocity)
			{
				minEndVelocity = velocity;
				maxEndVelocity = velocity;
			}
			// Sets min and max end velocities to the given value
			void SetEndVelocity(float x, float y, float z)
			{
				minEndVelocity.x = x;
				minEndVelocity.y = y;
				minEndVelocity.z = z;
				maxEndVelocity.x = x;
				maxEndVelocity.y = y;
				maxEndVelocity.z = z;
			}
			// Sets start and end color to the given value
			void SetColor(const glm::vec4& color)
			{
				startColor = color;
				endColor = color;
			}
			// Sets start and end color to the given value
			void SetColor(float r, float g, float b, float a = 1.0f)
			{
				startColor.r = r;
				startColor.g = g;
				startColor.b = b;
				startColor.a = a;
				endColor.r = r;
				endColor.g = g;
				endColor.b = b;
				endColor.a = a;
			}
			// Sets start and end rotation to the given value
			void SetRotation(float rotation, bool degrees = true)
			{
				const float radians{ degrees ? glm::radians(rotation) : rotation };
				minRotation = radians;
				maxRotation = radians;
			}
			void SetMinRotation(float rotation, bool degrees = true)
			{
				const float radians{ degrees ? glm::radians(rotation) : rotation };
				minRotation = radians;
			}
			void SetMaxRotation(float rotation, bool degrees = true)
			{
				const float radians{ degrees ? glm::radians(rotation) : rotation };
				maxRotation = radians;
			}
			// Sets start and end rotation per second to the given value
			void SetRotationPerSecond(float rotationPerSecond, bool degrees = true)
			{
				const float radians{ degrees ? glm::radians(rotationPerSecond) : rotationPerSecond };
				minRotationPerSecond = radians;
				maxRotationPerSecond = radians;
			}
			void SetMinRotationPerSecond(float rotationPerSecond, bool degrees = true)
			{
				const float radians{ degrees ? glm::radians(rotationPerSecond) : rotationPerSecond };
				minRotationPerSecond = radians;
			}
			void SetMaxRotationPerSecond(float rotationPerSecond, bool degrees = true)
			{
				const float radians{ degrees ? glm::radians(rotationPerSecond) : rotationPerSecond };
				maxRotationPerSecond = radians;
			}
		};

		ParticleRenderer();
		virtual ~ParticleRenderer() = default;

		ParticleRenderer(const ParticleRenderer& other) = delete;
		ParticleRenderer(ParticleRenderer&& other) = delete;
		ParticleRenderer& operator=(const ParticleRenderer& other) = delete;
		ParticleRenderer& operator=(ParticleRenderer&& other) = delete;

		EmitterSettings& GetEmitterSettings() { return m_Settings; }
		void SetTexture(const Texture& texture) const;

	private:
		struct ParticleVertex
		{
			glm::vec3 position{};
			glm::vec4 color{};
			float size{};
			float rotation{};
		};

		struct Particle
		{
			bool active{};
			
			float curLifetime{};
			float lifetime{};

			glm::vec3 startVelocity{};
			glm::vec3 endVelocity{};
			float scalePerSecond{};
			float rotationPerSecond{};

			glm::vec3 position{};
			glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
			float size{ 1.0f };
			float rotation{};
		};

		virtual void Update() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

		void EvaluateParticleAmount();
		void SpawnParticles();
		void SpawnParticle();
		void UpdateParticles();
		void UpdateParticleMesh();

		EmitterSettings m_Settings{};
		std::vector<Particle> m_Particles{};
		unsigned int m_NrActiveParticles{};

		float m_TimeSinceParticle{};

		graphics::IMeshRenderer* m_pRenderer{};
		graphics::CustomMesh m_Mesh{};
	};
}