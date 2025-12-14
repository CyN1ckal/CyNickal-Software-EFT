#pragma once

class Radar
{
public:
	static void Render();
	static void RenderSettings();

public:
	static inline bool bSettings{ true };
	static inline bool bMasterToggle{ true };
	static inline bool bLocalViewRay{ true };
	static inline bool bOtherPlayerViewRays{ true };
	static inline float fScale{ 0.5f };
	static inline float fLocalViewRayLength{ 100.0f };
	static inline float fOtherViewRayLength{ 33.0f };
	static inline float fEntityRadius{ 4.0f };
};