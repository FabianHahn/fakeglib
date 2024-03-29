#include "fakeglib/GTime.h"

#include <chrono>
#include <thread>
#include <type_traits>

typedef std::conditional<
	std::chrono::high_resolution_clock::is_steady,
	std::chrono::high_resolution_clock,
	std::chrono::steady_clock>::type SteadyClock;

FAKEGLIB_API gint64 g_get_monotonic_time()
{
	std::chrono::time_point<SteadyClock> now = SteadyClock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
}

FAKEGLIB_API gint64 g_get_real_time()
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
}

FAKEGLIB_API void g_usleep(gulong microseconds)
{
	std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}
