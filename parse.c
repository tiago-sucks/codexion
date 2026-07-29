#include "codexion.h"

// ◦ number_of_coders: The number of coders and also the number of dongles.

// ◦ time_to_burnout (in milliseconds): If a coder did not start compiling within
// time_to_burnout milliseconds since the beginning of their last compile or the
// beginning of the simulation, they burn out.

// ◦ time_to_compile (in milliseconds): The time it takes for a coder to compile.
//	 During that time, they must hold two dongles.

// ◦ time_to_debug (in milliseconds): The time a coder will spend debugging.

// ◦ time_to_refactor (in milliseconds): The time a coder will spend refactoring.
// After completing the refactoring phase, the coder will immediately attempt to
// acquire dongles and start compiling again.

// ◦ number_of_compiles_required: If all coders have compiled at least this
// many times, the simulation stops. Otherwise, it stops when a coder burns
// out.

// ◦ dongle_cooldown (in milliseconds): After being released, a dongle is unavail-
// able until its cooldown has passed.

// ◦ scheduler: The arbitration policy used by dongles to decide who gets them
// when multiple coders request them.
// The value must be exactly one of: fifo or edf.

// fifo means First In, First Out: the dongle is granted to the coder whose
// request arrived first.

// edf means Earliest Deadline First with deadline = last_compile_start +
// time_to_burnout