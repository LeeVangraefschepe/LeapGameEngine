#pragma once

namespace leap
{
	class GameObject;
	class Transform;
	class Collider;

	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		GameObject* GetGameObject() const { return m_pOwner; }
		Transform* GetTransform() const;

		void Destroy();
		bool IsMarkedAsDead() const;

		void SetActive(bool isActive);
		bool IsActive() const;

	protected:
		virtual void Awake() {}
		virtual void Start() {}
		virtual void OnEnable() {}
		virtual void OnDisable() {}
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void LateUpdate() {}
		virtual void OnGUI() {}
		virtual void OnDestroy() {}

		/// <summary>
		/// The first parameter is the collider linked to this component, the second parameter is the incoming collider.
		/// </summary>
		virtual void OnCollisionEnter(Collider* /*pCollider*/, Collider* /*pOther*/) {}
		/// <summary>
		/// The first parameter is the collider linked to this component, the second parameter is the incoming collider.
		/// </summary>
		virtual void OnCollisionStay(Collider* /*pCollider*/, Collider* /*pOther*/) {}
		/// <summary>
		/// The first parameter is the collider linked to this component, the second parameter is the incoming collider.
		/// </summary>
		virtual void OnCollisionExit(Collider* /*pCollider*/, Collider* /*pOther*/) {}
		/// <summary>
		/// The first parameter is the collider linked to this component, the second parameter is the incoming collider.
		/// </summary>
		virtual void OnTriggerEnter(Collider* /*pCollider*/, Collider* /*pOther*/) {}
		/// <summary>
		/// The first parameter is the collider linked to this component, the second parameter is the incoming collider.
		/// </summary>
		virtual void OnTriggerStay(Collider* /*pCollider*/, Collider* /*pOther*/) {}
		/// <summary>
		/// The first parameter is the collider linked to this component, the second parameter is the incoming collider.
		/// </summary>
		virtual void OnTriggerExit(Collider* /*pCollider*/, Collider* /*pOther*/) {}

	private:
		friend GameObject;

		void SetOwner(GameObject* pOwner);

		void ChangeActiveState();
		void TryCallStart();
		void TryCallEnableAndDisable();

		/// <summary>
		/// These functions are internally used to 
		/// change and check values of the m_StateFlags bitmask
		/// </summary>
		bool IsActiveLocalNextFrame() const;
		bool IsActiveLocal() const;
		void SetActiveLocal(bool isActive);
		bool IsActiveWorld() const;
		void SetActiveWorld(bool isActive);
		bool WasActiveWorldPreviousFrame() const;
		void SetPreviousActiveWorld(bool isActive);
		bool HasStarted() const;
		bool IsInitialized() const;
		void Initialize();

		enum class StateFlags : char
		{
			IsActiveLocalNextFrame		= 1 << 0,
			IsActiveLocal				= 1 << 1,
			WasActiveWorldPreviousFrame = 1 << 2,
			IsActiveWorld				= 1 << 3,
			IsInitialized				= 1 << 4,
			HasStarted					= 1 << 5,
			IsMarkedAsDead				= 1 << 6
		};

		unsigned char m_StateFlags{ static_cast<unsigned char>(StateFlags::IsActiveLocalNextFrame) };

		GameObject* m_pOwner{};
	};
}