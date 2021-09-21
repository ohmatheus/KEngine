#pragma once

class ISteering
{
public:

	static glm::vec2	CrossProduct(const glm::vec2 & v)
	{
		return glm::vec2(v.y, -v.x);
	}

	// returns the lenght of the (imaginary) z vector, is the same as CrossProduct(vec3, vec3).z
	static float		CrossProduct(const glm::vec2 &v1, const glm::vec2 &v2)
	{
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

	static float		Angle(const glm::vec2 &v1, const glm::vec2 &v2)
	{
		const float			dot = glm::dot(v1, v2);
		const float			cross = ISteering::CrossProduct(glm::vec2(v2), glm::vec2(v1));
		const float			acosDot = glm::acos(dot);
		const float			angleDif = glm::degrees(acosDot);

		return glm::sign(cross) * angleDif;
	}


};