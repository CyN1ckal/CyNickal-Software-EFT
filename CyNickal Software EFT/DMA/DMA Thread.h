/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

#include "DMA/CDMAConnection/CDMAConnection.h"

template <typename T, typename F>
class CTimer
{
public:
	CTimer(T _Interval, F _Function) : m_Interval(_Interval), m_Function(_Function) {}

	void Tick(std::chrono::steady_clock::time_point CurrentTime)
	{
		if (std::chrono::duration_cast<T>(CurrentTime - m_LastExecutionTime) >= m_Interval)
		{
			m_Function();
			m_LastExecutionTime = CurrentTime;
		}
	}
private:
	T m_Interval{};
	F m_Function{};
	std::chrono::high_resolution_clock::time_point m_LastExecutionTime{};
};

void DMA_Thread_Main();