#pragma once
#if (defined(_M_AMD64) || defined(_M_X64) || defined(__amd64)) &&              \
    !defined(__x86_64__)
#define __x86_64__ 1
#endif

#ifndef SSE_INSTR_SET
#if defined(__AVX2__)
#define SSE_INSTR_SET 8
#elif defined(__AVX__)
#define SSE_INSTR_SET 7
#elif defined(__SSE4_2__)
#define SSE_INSTR_SET 6
#elif defined(__SSE4_1__)
#define SSE_INSTR_SET 5
#elif defined(__SSSE3__)
#define SSE_INSTR_SET 4
#elif defined(__SSE3__)
#define SSE_INSTR_SET 3
#elif defined(__SSE2__) || defined(__x86_64__)
#define SSE_INSTR_SET 2
#elif defined(__SSE__)
#define SSE_INSTR_SET 1
#elif defined(_M_IX86_FP)
#define SSE_INSTR_SET _M_IX86_FP
#else
#define SSE_INSTR_SET 0
#endif
#endif

#if SSE_INSTR_SET > 7
#ifdef __GNUC__
#include <x86intrin.h>
#else
#include <immintrin.h>
#endif
#elif SSE_INSTR_SET == 7
#include <immintrin.h>
#elif SSE_INSTR_SET == 6
#include <nmmintrin.h>
#elif SSE_INSTR_SET == 5
#include <smmintrin.h>
#elif SSE_INSTR_SET == 4
#include <tmmintrin.h>
#elif SSE_INSTR_SET == 3
#include <pmmintrin.h>
#elif SSE_INSTR_SET == 2
#include <emmintrin.h>
#elif SSE_INSTR_SET == 1
#include <xmmintrin.h>
#endif
