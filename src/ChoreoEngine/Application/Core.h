#pragma once

#ifdef CE_ENABLE_ASSERTS 
    #include <signal.h>
 	#define CE_ASSERT(x, ...) { if(!(x)) { CE_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } } 
 	#define CE_CORE_ASSERT(x, ...) { if(!(x)) { CE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } } 
 #else 
 	#define CE_ASSERT(x, ...) 
 	#define CE_CORE_ASSERT(x, ...) 
 #endif 

// this just shits the bit by x places. 
// so if you have bit 1 you can offset it. 
// This way we can create a bitfield with multiple 
// events set and then just mask them out.
#define BIT(x) (1 << x)

#define CE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
