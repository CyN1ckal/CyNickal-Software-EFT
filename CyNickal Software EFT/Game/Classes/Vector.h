#pragma once

struct Vector3
{
	float x{ 0.0f };
	float y{ 0.0f };
	float z{ 0.0f };

	Vector3 operator-(Vector3 rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}
};

struct Vector2
{
	float x{ 0.0f };
	float y{ 0.0f };

	Vector2 operator-(Vector2 rhs) const
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}
};
