/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
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
	float DistanceTo(const Vector3& other) const
	{
		float dx = x - other.x;
		float dy = y - other.y;
		float dz = z - other.z;
		return sqrtf(dx * dx + dy * dy + dz * dz);
	}
	Vector3 Normalize() const {
		float length = sqrtf(x * x + y * y + z * z);
		return Vector3(x / length, y / length, z / length);
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
	float DistanceTo(const Vector2& other) const
	{
		float dx = x - other.x;
		float dy = y - other.y;
		return sqrtf(dx * dx + dy * dy);
	}
	float DistanceTo(const ImVec2& other) const
	{
		float dx = x - other.x;
		float dy = y - other.y;
		return sqrtf(dx * dx + dy * dy);
	}
};

struct Vector4
{
	float x{ 0.0f };
	float y{ 0.0f };
	float z{ 0.0f };
	float w{ 0.0f };
};

struct Matrix44
{
	float M[4][4]{};
};