// Copyright Mihai Preda.

#pragma once

typedef unsigned char byte;
typedef long long i64;
typedef unsigned long long u64;
typedef int      i32;
typedef unsigned u32;

static_assert(sizeof(u32) == 4,   "size u32");
static_assert(sizeof(u64) == 8,   "size u64");

#ifdef __GNUC__
void log(const char *fmt, ...) __attribute__ ((format(printf, 1, 2)));
#else
void log(const char *fmt, ...);
#endif

void initLog(const char *);

using namespace std; // std::string, std::pair, std::vector, std::unique_ptr;

#define BASE_VERSION "6.3"
// Reason for version change:
// 6.3 : new P-1 implem with multi-rounds and tests.
// 6.2 : added FFT-10 and FFT-6 middle step. Drop 3 & 5 middle steps.
// 6.1 : added P-1
// 6.0 : dropped PRP-1; investigate multi-GPU support.
// 5.0 : dropped TF; dropped old checkpoint load.
// 4.7 : parsing B1 & B2 bounds for PRP-1 in worktodo.txt
// 4.6 : merging LowGpu and OpenGpu back into Gpu.
// 4.5 : yet another kset schedule.
// 4.4 : new PRP,P-1 schedule.
// 4.3 : drop P-1 as standalone task and savefile. Background GCD.
// 4.2 : change PRP residue type to "type-4" (from "type-1").
// 4.1 : introduce PRP-1
// 4.0 : add P-1 first stage; require GMP.
// 3.9 : use save/load path vs. commit/rollback
// 3.8 : TF multi-bit in one pass. OWL TF savefile v2.
// 3.7 : integrated TF
// 3.6 : initial TF
// 3.5 : require OpenCL 2.x
// 3.4 : add factor-9 FFT step, and more FFT sizes.
// 3.3 : add new factor-5 FFT.
// 3.2 : always use fused tail. Drop un-fused fftH, square, multiply kernels.
// 3.1 : use computed trig (instead of tables) in transpose and tailFused / square. Reworked tailFused.
// 3.0 : add CUDA backend.

// The git revision should be passed through -D on the compiler command line (see Makefile).
#ifdef REV
#define VERSION BASE_VERSION "-" REV
#else
#define VERSION BASE_VERSION
#endif

#define PROGRAM "gpuowl"
