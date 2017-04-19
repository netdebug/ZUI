#if (defined HAVE_JS) && (HAVE_JS == 1)
/*
 *  Misc util stuff
 */

#include "duk_internal.h"

/*
 *  Lowercase digits for radix values 2 to 36.  Also doubles as lowercase
 *  hex nybble table.
 */

DUK_INTERNAL const duk_uint8_t duk_lc_digits[36] = {
	DUK_ASC_0, DUK_ASC_1, DUK_ASC_2, DUK_ASC_3,
	DUK_ASC_4, DUK_ASC_5, DUK_ASC_6, DUK_ASC_7,
	DUK_ASC_8, DUK_ASC_9, DUK_ASC_LC_A, DUK_ASC_LC_B,
	DUK_ASC_LC_C, DUK_ASC_LC_D, DUK_ASC_LC_E, DUK_ASC_LC_F,
	DUK_ASC_LC_G, DUK_ASC_LC_H, DUK_ASC_LC_I, DUK_ASC_LC_J,
	DUK_ASC_LC_K, DUK_ASC_LC_L, DUK_ASC_LC_M, DUK_ASC_LC_N,
	DUK_ASC_LC_O, DUK_ASC_LC_P, DUK_ASC_LC_Q, DUK_ASC_LC_R,
	DUK_ASC_LC_S, DUK_ASC_LC_T, DUK_ASC_LC_U, DUK_ASC_LC_V,
	DUK_ASC_LC_W, DUK_ASC_LC_X, DUK_ASC_LC_Y, DUK_ASC_LC_Z
};

DUK_INTERNAL const duk_uint8_t duk_uc_nybbles[16] = {
	DUK_ASC_0, DUK_ASC_1, DUK_ASC_2, DUK_ASC_3,
	DUK_ASC_4, DUK_ASC_5, DUK_ASC_6, DUK_ASC_7,
	DUK_ASC_8, DUK_ASC_9, DUK_ASC_UC_A, DUK_ASC_UC_B,
	DUK_ASC_UC_C, DUK_ASC_UC_D, DUK_ASC_UC_E, DUK_ASC_UC_F
};

/*
 *  Table for hex decoding ASCII hex digits
 */

DUK_INTERNAL const duk_int8_t duk_hex_dectab[256] = {
	/* -1 if invalid */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x00-0x0f */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x10-0x1f */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x20-0x2f */
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,  /* 0x30-0x3f */
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x40-0x4f */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x50-0x5f */
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x60-0x6f */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x70-0x7f */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x80-0x8f */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x90-0x9f */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0xa0-0xaf */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0xb0-0xbf */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0xc0-0xcf */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0xd0-0xdf */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0xe0-0xef */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1   /* 0xf0-0xff */
};

#if defined(DUK_USE_HEX_FASTPATH)
/* Preshifted << 4.  Must use 16-bit entry to allow negative value signaling. */
DUK_INTERNAL const duk_int16_t duk_hex_dectab_shift4[256] = {
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0x00-0x0f */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0x10-0x1f */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0x20-0x2f */
	0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0x30-0x3f */
	  -1, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0x40-0x4f */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0x50-0x5f */
	  -1, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0x60-0x6f */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0x70-0x7f */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0x80-0x8f */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0x90-0x9f */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0xa0-0xaf */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0xb0-0xbf */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0xc0-0xcf */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0xd0-0xdf */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  /* 0xe0-0xef */
	  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1   /* 0xf0-0xff */
};
#endif

/*
 *  Table for hex encoding bytes
 */

#if defined(DUK_USE_HEX_FASTPATH)
/* Lookup to encode one byte directly into 2 characters:
 *
 *   def genhextab(bswap):
 *       for i in xrange(256):
 *           t = chr(i).encode('hex')
 *           if bswap:
 *               t = t[1] + t[0]
 *           print('0x' + t.encode('hex') + 'U')
 *   print('big endian'); genhextab(False)
 *   print('little endian'); genhextab(True)
*/
DUK_INTERNAL const duk_uint16_t duk_hex_enctab[256] = {
#if defined(DUK_USE_INTEGER_BE)
	0x3030U, 0x3031U, 0x3032U, 0x3033U, 0x3034U, 0x3035U, 0x3036U, 0x3037U,
	0x3038U, 0x3039U, 0x3061U, 0x3062U, 0x3063U, 0x3064U, 0x3065U, 0x3066U,
	0x3130U, 0x3131U, 0x3132U, 0x3133U, 0x3134U, 0x3135U, 0x3136U, 0x3137U,
	0x3138U, 0x3139U, 0x3161U, 0x3162U, 0x3163U, 0x3164U, 0x3165U, 0x3166U,
	0x3230U, 0x3231U, 0x3232U, 0x3233U, 0x3234U, 0x3235U, 0x3236U, 0x3237U,
	0x3238U, 0x3239U, 0x3261U, 0x3262U, 0x3263U, 0x3264U, 0x3265U, 0x3266U,
	0x3330U, 0x3331U, 0x3332U, 0x3333U, 0x3334U, 0x3335U, 0x3336U, 0x3337U,
	0x3338U, 0x3339U, 0x3361U, 0x3362U, 0x3363U, 0x3364U, 0x3365U, 0x3366U,
	0x3430U, 0x3431U, 0x3432U, 0x3433U, 0x3434U, 0x3435U, 0x3436U, 0x3437U,
	0x3438U, 0x3439U, 0x3461U, 0x3462U, 0x3463U, 0x3464U, 0x3465U, 0x3466U,
	0x3530U, 0x3531U, 0x3532U, 0x3533U, 0x3534U, 0x3535U, 0x3536U, 0x3537U,
	0x3538U, 0x3539U, 0x3561U, 0x3562U, 0x3563U, 0x3564U, 0x3565U, 0x3566U,
	0x3630U, 0x3631U, 0x3632U, 0x3633U, 0x3634U, 0x3635U, 0x3636U, 0x3637U,
	0x3638U, 0x3639U, 0x3661U, 0x3662U, 0x3663U, 0x3664U, 0x3665U, 0x3666U,
	0x3730U, 0x3731U, 0x3732U, 0x3733U, 0x3734U, 0x3735U, 0x3736U, 0x3737U,
	0x3738U, 0x3739U, 0x3761U, 0x3762U, 0x3763U, 0x3764U, 0x3765U, 0x3766U,
	0x3830U, 0x3831U, 0x3832U, 0x3833U, 0x3834U, 0x3835U, 0x3836U, 0x3837U,
	0x3838U, 0x3839U, 0x3861U, 0x3862U, 0x3863U, 0x3864U, 0x3865U, 0x3866U,
	0x3930U, 0x3931U, 0x3932U, 0x3933U, 0x3934U, 0x3935U, 0x3936U, 0x3937U,
	0x3938U, 0x3939U, 0x3961U, 0x3962U, 0x3963U, 0x3964U, 0x3965U, 0x3966U,
	0x6130U, 0x6131U, 0x6132U, 0x6133U, 0x6134U, 0x6135U, 0x6136U, 0x6137U,
	0x6138U, 0x6139U, 0x6161U, 0x6162U, 0x6163U, 0x6164U, 0x6165U, 0x6166U,
	0x6230U, 0x6231U, 0x6232U, 0x6233U, 0x6234U, 0x6235U, 0x6236U, 0x6237U,
	0x6238U, 0x6239U, 0x6261U, 0x6262U, 0x6263U, 0x6264U, 0x6265U, 0x6266U,
	0x6330U, 0x6331U, 0x6332U, 0x6333U, 0x6334U, 0x6335U, 0x6336U, 0x6337U,
	0x6338U, 0x6339U, 0x6361U, 0x6362U, 0x6363U, 0x6364U, 0x6365U, 0x6366U,
	0x6430U, 0x6431U, 0x6432U, 0x6433U, 0x6434U, 0x6435U, 0x6436U, 0x6437U,
	0x6438U, 0x6439U, 0x6461U, 0x6462U, 0x6463U, 0x6464U, 0x6465U, 0x6466U,
	0x6530U, 0x6531U, 0x6532U, 0x6533U, 0x6534U, 0x6535U, 0x6536U, 0x6537U,
	0x6538U, 0x6539U, 0x6561U, 0x6562U, 0x6563U, 0x6564U, 0x6565U, 0x6566U,
	0x6630U, 0x6631U, 0x6632U, 0x6633U, 0x6634U, 0x6635U, 0x6636U, 0x6637U,
	0x6638U, 0x6639U, 0x6661U, 0x6662U, 0x6663U, 0x6664U, 0x6665U, 0x6666U
#else  /* DUK_USE_INTEGER_BE */
	0x3030U, 0x3130U, 0x3230U, 0x3330U, 0x3430U, 0x3530U, 0x3630U, 0x3730U,
	0x3830U, 0x3930U, 0x6130U, 0x6230U, 0x6330U, 0x6430U, 0x6530U, 0x6630U,
	0x3031U, 0x3131U, 0x3231U, 0x3331U, 0x3431U, 0x3531U, 0x3631U, 0x3731U,
	0x3831U, 0x3931U, 0x6131U, 0x6231U, 0x6331U, 0x6431U, 0x6531U, 0x6631U,
	0x3032U, 0x3132U, 0x3232U, 0x3332U, 0x3432U, 0x3532U, 0x3632U, 0x3732U,
	0x3832U, 0x3932U, 0x6132U, 0x6232U, 0x6332U, 0x6432U, 0x6532U, 0x6632U,
	0x3033U, 0x3133U, 0x3233U, 0x3333U, 0x3433U, 0x3533U, 0x3633U, 0x3733U,
	0x3833U, 0x3933U, 0x6133U, 0x6233U, 0x6333U, 0x6433U, 0x6533U, 0x6633U,
	0x3034U, 0x3134U, 0x3234U, 0x3334U, 0x3434U, 0x3534U, 0x3634U, 0x3734U,
	0x3834U, 0x3934U, 0x6134U, 0x6234U, 0x6334U, 0x6434U, 0x6534U, 0x6634U,
	0x3035U, 0x3135U, 0x3235U, 0x3335U, 0x3435U, 0x3535U, 0x3635U, 0x3735U,
	0x3835U, 0x3935U, 0x6135U, 0x6235U, 0x6335U, 0x6435U, 0x6535U, 0x6635U,
	0x3036U, 0x3136U, 0x3236U, 0x3336U, 0x3436U, 0x3536U, 0x3636U, 0x3736U,
	0x3836U, 0x3936U, 0x6136U, 0x6236U, 0x6336U, 0x6436U, 0x6536U, 0x6636U,
	0x3037U, 0x3137U, 0x3237U, 0x3337U, 0x3437U, 0x3537U, 0x3637U, 0x3737U,
	0x3837U, 0x3937U, 0x6137U, 0x6237U, 0x6337U, 0x6437U, 0x6537U, 0x6637U,
	0x3038U, 0x3138U, 0x3238U, 0x3338U, 0x3438U, 0x3538U, 0x3638U, 0x3738U,
	0x3838U, 0x3938U, 0x6138U, 0x6238U, 0x6338U, 0x6438U, 0x6538U, 0x6638U,
	0x3039U, 0x3139U, 0x3239U, 0x3339U, 0x3439U, 0x3539U, 0x3639U, 0x3739U,
	0x3839U, 0x3939U, 0x6139U, 0x6239U, 0x6339U, 0x6439U, 0x6539U, 0x6639U,
	0x3061U, 0x3161U, 0x3261U, 0x3361U, 0x3461U, 0x3561U, 0x3661U, 0x3761U,
	0x3861U, 0x3961U, 0x6161U, 0x6261U, 0x6361U, 0x6461U, 0x6561U, 0x6661U,
	0x3062U, 0x3162U, 0x3262U, 0x3362U, 0x3462U, 0x3562U, 0x3662U, 0x3762U,
	0x3862U, 0x3962U, 0x6162U, 0x6262U, 0x6362U, 0x6462U, 0x6562U, 0x6662U,
	0x3063U, 0x3163U, 0x3263U, 0x3363U, 0x3463U, 0x3563U, 0x3663U, 0x3763U,
	0x3863U, 0x3963U, 0x6163U, 0x6263U, 0x6363U, 0x6463U, 0x6563U, 0x6663U,
	0x3064U, 0x3164U, 0x3264U, 0x3364U, 0x3464U, 0x3564U, 0x3664U, 0x3764U,
	0x3864U, 0x3964U, 0x6164U, 0x6264U, 0x6364U, 0x6464U, 0x6564U, 0x6664U,
	0x3065U, 0x3165U, 0x3265U, 0x3365U, 0x3465U, 0x3565U, 0x3665U, 0x3765U,
	0x3865U, 0x3965U, 0x6165U, 0x6265U, 0x6365U, 0x6465U, 0x6565U, 0x6665U,
	0x3066U, 0x3166U, 0x3266U, 0x3366U, 0x3466U, 0x3566U, 0x3666U, 0x3766U,
	0x3866U, 0x3966U, 0x6166U, 0x6266U, 0x6366U, 0x6466U, 0x6566U, 0x6666U
#endif  /* DUK_USE_INTEGER_BE */
};
#endif  /* DUK_USE_HEX_FASTPATH */

/*
 *  Table for base-64 encoding
 */

#if defined(DUK_USE_BASE64_FASTPATH)
DUK_INTERNAL const duk_uint8_t duk_base64_enctab[64] = {
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,  /* A...P */
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,  /* Q...f */
	0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,  /* g...v */
	0x77, 0x78, 0x79, 0x7a, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2b, 0x2f   /* w.../ */
};
#endif  /* DUK_USE_BASE64_FASTPATH */

/*
 *  Table for base-64 decoding
 */

#if defined(DUK_USE_BASE64_FASTPATH)
DUK_INTERNAL const duk_int8_t duk_base64_dectab[256] = {
	/* -1 = error, -2 = allowed whitespace, -3 = padding ('='), 0...63 decoded bytes */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -1, -1, -2, -1, -1,  /* 0x00...0x0f */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x10...0x1f */
	-2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,  /* 0x20...0x2f */
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -3, -1, -1,  /* 0x30...0x3f */
	-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,  /* 0x40...0x4f */
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,  /* 0x50...0x5f */
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,  /* 0x60...0x6f */
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,  /* 0x70...0x7f */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x80...0x8f */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0x90...0x9f */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0xa0...0xaf */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0xb0...0xbf */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0xc0...0xcf */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0xd0...0xdf */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 0xe0...0xef */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1   /* 0xf0...0xff */
};
#endif  /* DUK_USE_BASE64_FASTPATH */

/*
 *  Arbitrary byteswap for potentially unaligned values
 *
 *  Used to byteswap pointers e.g. in debugger code.
 */

#if defined(DUK_USE_DEBUGGER_SUPPORT)  /* For now only needed by the debugger. */
DUK_INTERNAL void duk_byteswap_bytes(duk_uint8_t *p, duk_small_uint_t len) {
	duk_uint8_t tmp;
	duk_uint8_t *q = p + len - 1;

	while (p - q < 0) {
		tmp = *p;
		*p = *q;
		*q = tmp;
		p++;
		q--;
	}
}
#endif

/*
 *  Miscellaneous coercion / clamping helpers.
 */

/* Check whether a duk_double_t is a whole number in the 32-bit range (reject
 * negative zero), and if so, return a duk_int32_t.
 * For compiler use: don't allow negative zero as it will cause trouble with
 * LDINT+LDINTX, positive zero is OK.
 */
DUK_INTERNAL duk_bool_t duk_is_whole_get_int32_nonegzero(duk_double_t x, duk_int32_t *ival) {
	duk_int32_t t;

	t = (duk_int32_t) x;
	if (!((duk_double_t) t == x)) {
		return 0;
	}
	if (t == 0) {
		duk_double_union du;
		du.d = x;
		if (DUK_DBLUNION_HAS_SIGNBIT(&du)) {
			return 0;
		}
	}
	*ival = t;
	return 1;
}

/* Check whether a duk_double_t is a whole number in the 32-bit range, and if
 * so, return a duk_int32_t.
 */
DUK_INTERNAL duk_bool_t duk_is_whole_get_int32(duk_double_t x, duk_int32_t *ival) {
	duk_int32_t t;

	t = (duk_int32_t) x;
	if (!((duk_double_t) t == x)) {
		return 0;
	}
	*ival = t;
	return 1;
}

/*
 *  IEEE double checks
 */

DUK_INTERNAL duk_bool_t duk_double_is_anyinf(duk_double_t x) {
	duk_double_union du;
	du.d = x;
	return DUK_DBLUNION_IS_ANYINF(&du);
}

DUK_INTERNAL duk_bool_t duk_double_is_posinf(duk_double_t x) {
	duk_double_union du;
	du.d = x;
	return DUK_DBLUNION_IS_POSINF(&du);
}

DUK_INTERNAL duk_bool_t duk_double_is_neginf(duk_double_t x) {
	duk_double_union du;
	du.d = x;
	return DUK_DBLUNION_IS_NEGINF(&du);
}

DUK_INTERNAL duk_bool_t duk_double_is_nan(duk_double_t x) {
	duk_double_union du;
	du.d = x;
	/* Assumes we're dealing with a Duktape internal NaN which is
	 * NaN normalized if duk_tval requires it.
	 */
	DUK_ASSERT(DUK_DBLUNION_IS_NORMALIZED(&du));
	return DUK_DBLUNION_IS_NAN(&du);
}

DUK_INTERNAL duk_bool_t duk_double_is_nan_or_zero(duk_double_t x) {
	duk_double_union du;
	du.d = x;
	/* Assumes we're dealing with a Duktape internal NaN which is
	 * NaN normalized if duk_tval requires it.
	 */
	DUK_ASSERT(DUK_DBLUNION_IS_NORMALIZED(&du));
	return DUK_DBLUNION_IS_NAN(&du) || DUK_DBLUNION_IS_ANYZERO(&du);
}

DUK_INTERNAL duk_bool_t duk_double_is_nan_or_inf(duk_double_t x) {
	duk_double_union du;
	du.d = x;
	/* If exponent is 0x7FF the argument is either a NaN or an
	 * infinity.  We don't need to check any other fields.
	 */
#if defined(DUK_USE_64BIT_OPS)
#if defined(DUK_USE_DOUBLE_ME)
	return (du.ull[DUK_DBL_IDX_ULL0] & 0x000000007ff00000ULL) == 0x000000007ff00000ULL;
#else
	return (du.ull[DUK_DBL_IDX_ULL0] & 0x7ff0000000000000ULL) == 0x7ff0000000000000ULL;
#endif
#else
	return (du.ui[DUK_DBL_IDX_UI0] & 0x7ff00000UL) == 0x7ff00000UL;
#endif
}

DUK_INTERNAL duk_bool_t duk_double_is_nan_zero_inf(duk_double_t x) {
	duk_double_union du;
#if defined(DUK_USE_64BIT_OPS)
	duk_uint64_t t;
#else
	duk_uint32_t t;
#endif
	du.d = x;
#if defined(DUK_USE_64BIT_OPS)
#if defined(DUK_USE_DOUBLE_ME)
	t = du.ull[DUK_DBL_IDX_ULL0] & 0x000000007ff00000ULL;
	if (t == 0x0000000000000000ULL) {
		t = du.ull[DUK_DBL_IDX_ULL0] & 0x0000000080000000ULL;
		return t == 0;
	}
	if (t == 0x000000007ff00000UL) {
		return 1;
	}
#else
	t = du.ull[DUK_DBL_IDX_ULL0] & 0x7ff0000000000000ULL;
	if (t == 0x0000000000000000ULL) {
		t = du.ull[DUK_DBL_IDX_ULL0] & 0x8000000000000000ULL;
		return t == 0;
	}
	if (t == 0x7ff0000000000000ULL) {
		return 1;
	}
#endif
#else
	t = du.ui[DUK_DBL_IDX_UI0] & 0x7ff00000UL;
	if (t == 0x00000000UL) {
		return DUK_DBLUNION_IS_ANYZERO(&du);
	}
	if (t == 0x7ff00000UL) {
		return 1;
	}
#endif
	return 0;
}

DUK_INTERNAL duk_small_uint_t duk_double_signbit(duk_double_t x) {
	duk_double_union du;
	du.d = x;
	return (duk_small_uint_t) DUK_DBLUNION_GET_SIGNBIT(&du);
}

DUK_INTERNAL duk_double_t duk_double_trunc_towards_zero(duk_double_t x) {
	/* XXX: optimize */
	duk_small_int_t s = duk_double_signbit(x);
	x = DUK_FLOOR(DUK_FABS(x));  /* truncate towards zero */
	if (s) {
		x = -x;
	}
	return x;
}

DUK_INTERNAL duk_bool_t duk_double_same_sign(duk_double_t x, duk_double_t y) {
	duk_double_union du1;
	duk_double_union du2;
	du1.d = x;
	du2.d = y;

	return (((du1.ui[DUK_DBL_IDX_UI0] ^ du2.ui[DUK_DBL_IDX_UI0]) & 0x80000000UL) == 0);
}

DUK_INTERNAL duk_double_t duk_double_fmin(duk_double_t x, duk_double_t y) {
	/* Doesn't replicate fmin() behavior exactly: for fmin() if one
	 * argument is a NaN, the other argument should be returned.
	 * Duktape doesn't rely on this behavior so the replacement can
	 * be simplified.
	 */
	return (x < y ? x : y);
}

DUK_INTERNAL duk_double_t duk_double_fmax(duk_double_t x, duk_double_t y) {
	/* Doesn't replicate fmax() behavior exactly: for fmax() if one
	 * argument is a NaN, the other argument should be returned.
	 * Duktape doesn't rely on this behavior so the replacement can
	 * be simplified.
	 */
	return (x > y ? x : y);
}
#endif
