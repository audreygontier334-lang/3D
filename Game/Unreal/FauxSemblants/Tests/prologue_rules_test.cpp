#include "../Source/FauxSemblants/Public/FSPrologueRules.h"
#include <cassert>
#include <iostream>

int main()
{
	using namespace FSPrologueRules;
	assert(IsWindowOpen(true, 0.0, 10.0));
	assert(IsWindowOpen(true, 9.999, 10.0));
	assert(!IsWindowOpen(true, 10.0, 10.0)); // Expiry before the next phase tick.
	assert(!IsWindowOpen(true, 10.001, 10.0));
	assert(!IsWindowOpen(false, 5.0, 10.0)); // Holding never adds another action.
	assert(!IsWindowOpen(true, -1.0, 10.0));
	assert(!IsWindowOpen(true, 0.0, 0.0));
	assert(IsWindowOpen(true, 20.0, 22.5)); // Explicit accessibility duration.
	assert(!IsWindowOpen(true, 22.5, 22.5));

	assert(!HasReachedTarget(false, 7, 7, 0.0, 0.0)); // Sent but not holding.
	assert(HasReachedTarget(true, 7, 7, 59.9, 99.9));
	assert(!HasReachedTarget(true, 8, 7, 0.0, 0.0)); // Recall/new send invalidates old request.
	assert(!HasReachedTarget(true, 7, 7, 60.0, 0.0));
	assert(!HasReachedTarget(true, 7, 7, 0.0, 100.0)); // Same XY on another level.
	assert(!HasReachedTarget(true, 7, 7, 600.0, 0.0)); // Blocked before target.
	std::cout << "15 prologue boundary checks passed\n";
}
