// Engine-independent prologue rules, shared by runtime and boundary tests.
#pragma once

namespace FSPrologueRules
{
	inline bool IsWindowOpen(bool bWindowPhase, double Elapsed, double Duration)
	{
		return bWindowPhase && Duration > 0.0 && Elapsed >= 0.0 && Elapsed < Duration;
	}

	inline bool HasReachedTarget(bool bHolding, unsigned int CurrentRequest, unsigned int ExpectedRequest,
		double Distance2D, double HeightDifference)
	{
		return bHolding && CurrentRequest == ExpectedRequest
			&& Distance2D >= 0.0 && Distance2D < 60.0
			&& HeightDifference >= 0.0 && HeightDifference < 100.0;
	}
}
