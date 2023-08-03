#pragma once

constexpr size_t JF_PayloadSize = 8192;
constexpr size_t JF_NumPackets = 128;

//Debug util
#ifdef ARVE_DEBUG
#  define DBG(x) x
#else
#  define DBG(x)
#endif // DEBUG
