#pragma once

#include <string>

namespace leap
{
	class GameObject;
	class Mesh;

	class GameObjectUtility final
	{
	public:
		static void CreateFromFBX(GameObject* pParent, const std::string& filePath);

	private:
		static void CreateFromFBXRecursive(GameObject* pParent, const Mesh& mesh);
	};
}