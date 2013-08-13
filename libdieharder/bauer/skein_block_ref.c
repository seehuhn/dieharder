/***********************************************************************
**
** Implementation of the Skein block functions.
**
** Source code author: Doug Whiting, 2008.
**
** This algorithm and source code is released to the public domain.
**
** Compile-time switches:
**
**  SKEIN_USE_ASM             -- set bits (256/512/1024) to select which
**                               versions use ASM code for block processing
**                               [default: use C for all block sizes]
**
************************************************************************/

#include <string.h>
#include "skein.h"
#include <stdio.h>

/* 64-bit rotate left --  defined in skein_port.h as macro
u64b_t RotL_64(u64b_t x,uint_t N)
    {
    return (x << (N & 63)) | (x >> ((64-N) & 63));
    }
*/

#define BLK_BITS    (WCNT*64)

/* macro to perform a key injection (same for all block sizes) */
#define InjectKey(r)                                                \
    for (i=0;i < WCNT;i++)                                          \
         X[i] += ks[((r)+i) % (WCNT+1)];                            \
    X[WCNT-3] += ts[((r)+0) % 3];                                   \
    X[WCNT-2] += ts[((r)+1) % 3];                                   \
    X[WCNT-1] += (r);                    /* avoid slide attacks */  \



void Threefish_512_Process_Blocks(Threefish_512_Ctxt_t *ctx, const u08b_t *input,
		void *output, size_t blkCnt) {
	enum { WCNT = SKEIN_512_STATE_WORDS };

	size_t  i,r;
	u64b_t  ts[3];                            /* key schedule: tweak */
	u64b_t  ks[WCNT+1];                       /* key schedule: chaining vars */
	u64b_t  *X = (void *) output ;            /* local copy of vars */
	u64b_t  w [WCNT];                         /* local copy of input block */

	Skein_assert(blkCnt != 0);                /* never call with blkCnt == 0! */
	/* precompute the key schedule for this block */
	ks[WCNT] = SKEIN_KS_PARITY;
	for (i=0;i < WCNT; i++) {
		ks[i]     = ctx->Key[i];
		ks[WCNT] ^= ctx->Key[i];            /* compute overall parity */
	}
	ts[0] = ctx->T[0];	/* Tweak words */
	ts[1] = ctx->T[1];
	ts[2] = ts[0] ^ ts[1];

	do  {
		Skein_Get64_LSB_First(w,input,WCNT); /* get input block in little-endian format */
		for (i=0;i < WCNT; i++) {             /* do the first full key injection */
			X[i]  = w[i] + ks[i];
		}
		X[WCNT-3] += ts[0];
		X[WCNT-2] += ts[1];


		for (r=1;r <= SKEIN_512_ROUNDS_TOTAL/8; r++) { /* unroll 8 rounds */
			X[0] += X[1]; X[1] = RotL_64(X[1],R_512_0_0); X[1] ^= X[0];
			X[2] += X[3]; X[3] = RotL_64(X[3],R_512_0_1); X[3] ^= X[2];
			X[4] += X[5]; X[5] = RotL_64(X[5],R_512_0_2); X[5] ^= X[4];
			X[6] += X[7]; X[7] = RotL_64(X[7],R_512_0_3); X[7] ^= X[6];

			X[2] += X[1]; X[1] = RotL_64(X[1],R_512_1_0); X[1] ^= X[2];
			X[4] += X[7]; X[7] = RotL_64(X[7],R_512_1_1); X[7] ^= X[4];
			X[6] += X[5]; X[5] = RotL_64(X[5],R_512_1_2); X[5] ^= X[6];
			X[0] += X[3]; X[3] = RotL_64(X[3],R_512_1_3); X[3] ^= X[0];

			X[4] += X[1]; X[1] = RotL_64(X[1],R_512_2_0); X[1] ^= X[4];
			X[6] += X[3]; X[3] = RotL_64(X[3],R_512_2_1); X[3] ^= X[6];
			X[0] += X[5]; X[5] = RotL_64(X[5],R_512_2_2); X[5] ^= X[0];
			X[2] += X[7]; X[7] = RotL_64(X[7],R_512_2_3); X[7] ^= X[2];

			X[6] += X[1]; X[1] = RotL_64(X[1],R_512_3_0); X[1] ^= X[6];
			X[0] += X[7]; X[7] = RotL_64(X[7],R_512_3_1); X[7] ^= X[0];
			X[2] += X[5]; X[5] = RotL_64(X[5],R_512_3_2); X[5] ^= X[2];
			X[4] += X[3]; X[3] = RotL_64(X[3],R_512_3_3); X[3] ^= X[4];
			InjectKey(2*r-1);

			X[0] += X[1]; X[1] = RotL_64(X[1],R_512_4_0); X[1] ^= X[0];
			X[2] += X[3]; X[3] = RotL_64(X[3],R_512_4_1); X[3] ^= X[2];
			X[4] += X[5]; X[5] = RotL_64(X[5],R_512_4_2); X[5] ^= X[4];
			X[6] += X[7]; X[7] = RotL_64(X[7],R_512_4_3); X[7] ^= X[6];

			X[2] += X[1]; X[1] = RotL_64(X[1],R_512_5_0); X[1] ^= X[2];
			X[4] += X[7]; X[7] = RotL_64(X[7],R_512_5_1); X[7] ^= X[4];
			X[6] += X[5]; X[5] = RotL_64(X[5],R_512_5_2); X[5] ^= X[6];
			X[0] += X[3]; X[3] = RotL_64(X[3],R_512_5_3); X[3] ^= X[0];

			X[4] += X[1]; X[1] = RotL_64(X[1],R_512_6_0); X[1] ^= X[4];
			X[6] += X[3]; X[3] = RotL_64(X[3],R_512_6_1); X[3] ^= X[6];
			X[0] += X[5]; X[5] = RotL_64(X[5],R_512_6_2); X[5] ^= X[0];
			X[2] += X[7]; X[7] = RotL_64(X[7],R_512_6_3); X[7] ^= X[2];

			X[6] += X[1]; X[1] = RotL_64(X[1],R_512_7_0); X[1] ^= X[6];
			X[0] += X[7]; X[7] = RotL_64(X[7],R_512_7_1); X[7] ^= X[0];
			X[2] += X[5]; X[5] = RotL_64(X[5],R_512_7_2); X[5] ^= X[2];
			X[4] += X[3]; X[3] = RotL_64(X[3],R_512_7_3); X[3] ^= X[4];
			InjectKey(2*r);
		}
		/* DON'T do the final "feedforward" xor, update context chaining vars */
		//		for (i=0;i < WCNT;i++)
		//			ctx->X[i] = X[i] ^ w[i];

		//		Skein_Clear_First_Flag(ctx->h);		/* clear the start bit */
		input += SKEIN_512_BLOCK_BYTES;
		X += WCNT;
	}
	while (--blkCnt);
}

typedef unsigned long long uint64;

#if 1
uint64 timingAdjust = 200;

uint64 time() {
	volatile uint64 temp = 4;
	__asm__ __volatile__ (
			"cpuid\n\t"
			"rdtsc\n\t"
			"leaq %0, %%rcx\n\t"
			"movl %%eax, (%%rcx)\n\t"
			"movl %%edx, 4(%%rcx)\n\t"
			: : "m" (temp) : "%eax", "%ebx", "%rcx", "%edx");
	return temp - timingAdjust;
}

#endif

int main() {
	Threefish_512_Ctxt_t ctx;
	unsigned char input[64];
	unsigned char output[64];
	unsigned char testKey[64];
	unsigned char testTweak[16];
	unsigned int i;
	unsigned char *bigTest;
	uint64 startTime, endTime;

	memset(&ctx, 0, sizeof(ctx));
	memset(input, 0, sizeof(input));
	memset(output, 0, sizeof(output));

	Threefish_512_Process_Blocks(&ctx, input, output, 1);

	for (i = 0; i < 64; i++) {
		if (i % 32 == 0) printf("\n");
		printf("%02x", output[i]);
	}
	printf("\n");
//	408be942494492eab19daa3e96ad19aedfc41f4e55f8a2626c1e46d54547a713
//	d43b21f0de1a10881ed5c4adefdad1c4172cd768c8fc28d0dde9df018042fe3e

	for (i = 0; i < 16; i++) testTweak[i] = i;
	for (i = 0; i < 64; i++) {
		testKey[i] = i + 16;
		input[i] = 255-i;
	}

	memcpy(ctx.T, testTweak, 16);
	memcpy(ctx.Key, testKey, 64);

	Threefish_512_Process_Blocks(&ctx, input, output, 1);

	for (i = 0; i < 64; i++) {
		if (i % 32 == 0) printf("\n");
		printf("%02x", output[i]);
	}
	printf("\n");
//	869ae12210e51d3b0736399f2acb400de230600b13e62f1df7596a146232d281
//	dfbf127a65571b9a798906c719678394c50d995138fd83f2bfa54a3bc350d2f0

	bigTest = (unsigned char *) malloc(sizeof(*bigTest) * 64 * 1048576);
	startTime = time();
	Threefish_512_Process_Blocks(&ctx, bigTest, bigTest, 1048576);
	endTime = time();

	printf("Timing: %f cycles/byte\n", (double) (endTime - startTime) / (64 * 1048576));

	return 0;
}

