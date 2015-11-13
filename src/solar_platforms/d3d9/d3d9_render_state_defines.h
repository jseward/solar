#pragma once

//taken from https://github.com/bkaradzic/bgfx

#include <stdint.h>

///
#define D3D9_STATE_RGB_WRITE             UINT64_C(0x0000000000000001) //!< Enable RGB write.
#define D3D9_STATE_ALPHA_WRITE           UINT64_C(0x0000000000000002) //!< Enable alpha write.
#define D3D9_STATE_DEPTH_WRITE           UINT64_C(0x0000000000000004) //!< Enable depth write.

#define D3D9_STATE_DEPTH_TEST_LESS       UINT64_C(0x0000000000000010) //!< Enable depth test, less.
#define D3D9_STATE_DEPTH_TEST_LEQUAL     UINT64_C(0x0000000000000020) //!< Enable depth test, less equal.
#define D3D9_STATE_DEPTH_TEST_EQUAL      UINT64_C(0x0000000000000030) //!< Enable depth test, equal.
#define D3D9_STATE_DEPTH_TEST_GEQUAL     UINT64_C(0x0000000000000040) //!< Enable depth test, greater equal.
#define D3D9_STATE_DEPTH_TEST_GREATER    UINT64_C(0x0000000000000050) //!< Enable depth test, greater.
#define D3D9_STATE_DEPTH_TEST_NOTEQUAL   UINT64_C(0x0000000000000060) //!< Enable depth test, not equal.
#define D3D9_STATE_DEPTH_TEST_NEVER      UINT64_C(0x0000000000000070) //!< Enable depth test, never.
#define D3D9_STATE_DEPTH_TEST_ALWAYS     UINT64_C(0x0000000000000080) //!< Enable depth test, always.
#define D3D9_STATE_DEPTH_TEST_SHIFT      4
#define D3D9_STATE_DEPTH_TEST_MASK       UINT64_C(0x00000000000000f0) //!< Depth test state bit mask.

#define D3D9_STATE_BLEND_ZERO            UINT64_C(0x0000000000001000) //!<
#define D3D9_STATE_BLEND_ONE             UINT64_C(0x0000000000002000) //!<
#define D3D9_STATE_BLEND_SRC_COLOR       UINT64_C(0x0000000000003000) //!<
#define D3D9_STATE_BLEND_INV_SRC_COLOR   UINT64_C(0x0000000000004000) //!<
#define D3D9_STATE_BLEND_SRC_ALPHA       UINT64_C(0x0000000000005000) //!<
#define D3D9_STATE_BLEND_INV_SRC_ALPHA   UINT64_C(0x0000000000006000) //!<
#define D3D9_STATE_BLEND_DST_ALPHA       UINT64_C(0x0000000000007000) //!<
#define D3D9_STATE_BLEND_INV_DST_ALPHA   UINT64_C(0x0000000000008000) //!<
#define D3D9_STATE_BLEND_DST_COLOR       UINT64_C(0x0000000000009000) //!<
#define D3D9_STATE_BLEND_INV_DST_COLOR   UINT64_C(0x000000000000a000) //!<
#define D3D9_STATE_BLEND_SRC_ALPHA_SAT   UINT64_C(0x000000000000b000) //!<
#define D3D9_STATE_BLEND_FACTOR          UINT64_C(0x000000000000c000) //!<
#define D3D9_STATE_BLEND_INV_FACTOR      UINT64_C(0x000000000000d000) //!<
#define D3D9_STATE_BLEND_SHIFT           12                           //!< Blend state bit shift.
#define D3D9_STATE_BLEND_MASK            UINT64_C(0x000000000ffff000) //!< Blend state bit mask.

#define D3D9_STATE_BLEND_EQUATION_ADD    UINT64_C(0x0000000000000000) //!<
#define D3D9_STATE_BLEND_EQUATION_SUB    UINT64_C(0x0000000010000000) //!<
#define D3D9_STATE_BLEND_EQUATION_REVSUB UINT64_C(0x0000000020000000) //!<
#define D3D9_STATE_BLEND_EQUATION_MIN    UINT64_C(0x0000000030000000) //!<
#define D3D9_STATE_BLEND_EQUATION_MAX    UINT64_C(0x0000000040000000) //!<
#define D3D9_STATE_BLEND_EQUATION_SHIFT  28                           //!< Blend equation bit shift.
#define D3D9_STATE_BLEND_EQUATION_MASK   UINT64_C(0x00000003f0000000) //!< Blend equation bit mask.

#define D3D9_STATE_BLEND_INDEPENDENT     UINT64_C(0x0000000400000000) //!< Enable blend independent.

#define D3D9_STATE_CULL_CW               UINT64_C(0x0000001000000000) //!< Cull clockwise triangles.
#define D3D9_STATE_CULL_CCW              UINT64_C(0x0000002000000000) //!< Cull counter-clockwise triangles.
#define D3D9_STATE_CULL_SHIFT            36                           //!< Culling mode bit shift.
#define D3D9_STATE_CULL_MASK             UINT64_C(0x0000003000000000) //!< Culling mode bit mask.

/// See D3D9_STATE_ALPHA_REF(_ref) helper macro.
#define D3D9_STATE_ALPHA_REF_SHIFT       40                           //!< Alpha reference bit shift.
#define D3D9_STATE_ALPHA_REF_MASK        UINT64_C(0x0000ff0000000000) //!< Alpha reference bit mask.

#define D3D9_STATE_PT_TRISTRIP           UINT64_C(0x0001000000000000) //!< Tristrip.
#define D3D9_STATE_PT_LINES              UINT64_C(0x0002000000000000) //!< Lines.
#define D3D9_STATE_PT_LINESTRIP          UINT64_C(0x0003000000000000) //!< Line strip.
#define D3D9_STATE_PT_POINTS             UINT64_C(0x0004000000000000) //!< Points.
#define D3D9_STATE_PT_SHIFT              48                           //!< Primitive type bit shift.
#define D3D9_STATE_PT_MASK               UINT64_C(0x0007000000000000) //!< Primitive type bit mask.

#define D3D9_STATE_POINT_SIZE_SHIFT      52                           //!< Point size bit shift.
#define D3D9_STATE_POINT_SIZE_MASK       UINT64_C(0x0ff0000000000000) //!< Point size bit mask.

/// Enable MSAA write when writing into MSAA frame buffer. This flag is ignored when not writing into
/// MSAA frame buffer.
#define D3D9_STATE_MSAA                  UINT64_C(0x1000000000000000) //!< Enable MSAA rasterization.

/// Do not use!
#define D3D9_STATE_RESERVED_SHIFT        61                           //!< Internal bits shift.
#define D3D9_STATE_RESERVED_MASK         UINT64_C(0xe000000000000000) //!< Internal bits mask.

/// See D3D9_STATE_POINT_SIZE(_size) helper macro.
#define D3D9_STATE_NONE                  UINT64_C(0x0000000000000000) //!< No state.
#define D3D9_STATE_MASK                  UINT64_C(0xffffffffffffffff) //!< State mask.

/// Default state is write to RGB, alpha, and depth with depth test less enabled, with clockwise
/// culling and MSAA (when writing into MSAA frame buffer, otherwise this flag is ignored).
#define D3D9_STATE_DEFAULT (0 \
					| D3D9_STATE_RGB_WRITE \
					| D3D9_STATE_ALPHA_WRITE \
					| D3D9_STATE_DEPTH_TEST_LESS \
					| D3D9_STATE_DEPTH_WRITE \
					| D3D9_STATE_CULL_CW \
					| D3D9_STATE_MSAA \
					)

#define D3D9_STATE_ALPHA_REF(_ref)   ( ( (uint64_t)(_ref )<<D3D9_STATE_ALPHA_REF_SHIFT )&D3D9_STATE_ALPHA_REF_MASK)
#define D3D9_STATE_POINT_SIZE(_size) ( ( (uint64_t)(_size)<<D3D9_STATE_POINT_SIZE_SHIFT)&D3D9_STATE_POINT_SIZE_MASK)

///
#define D3D9_STATE_BLEND_FUNC_SEPARATE(_srcRGB, _dstRGB, _srcA, _dstA) (UINT64_C(0) \
					| ( ( (uint64_t)(_srcRGB)|( (uint64_t)(_dstRGB)<<4) )   ) \
					| ( ( (uint64_t)(_srcA  )|( (uint64_t)(_dstA  )<<4) )<<8) \
					)

#define D3D9_STATE_BLEND_EQUATION_SEPARATE(_rgb, _a) ( (uint64_t)(_rgb)|( (uint64_t)(_a)<<3) )

///
#define D3D9_STATE_BLEND_FUNC(_src, _dst)    D3D9_STATE_BLEND_FUNC_SEPARATE(_src, _dst, _src, _dst)
#define D3D9_STATE_BLEND_EQUATION(_equation) D3D9_STATE_BLEND_EQUATION_SEPARATE(_equation, _equation)

#define D3D9_STATE_BLEND_ADD         (D3D9_STATE_BLEND_FUNC(D3D9_STATE_BLEND_ONE,       D3D9_STATE_BLEND_ONE          ) )
#define D3D9_STATE_BLEND_ALPHA       (D3D9_STATE_BLEND_FUNC(D3D9_STATE_BLEND_SRC_ALPHA, D3D9_STATE_BLEND_INV_SRC_ALPHA) )
#define D3D9_STATE_BLEND_DARKEN      (D3D9_STATE_BLEND_FUNC(D3D9_STATE_BLEND_ONE,       D3D9_STATE_BLEND_ONE          ) | D3D9_STATE_BLEND_EQUATION(D3D9_STATE_BLEND_EQUATION_MIN) )
#define D3D9_STATE_BLEND_LIGHTEN     (D3D9_STATE_BLEND_FUNC(D3D9_STATE_BLEND_ONE,       D3D9_STATE_BLEND_ONE          ) | D3D9_STATE_BLEND_EQUATION(D3D9_STATE_BLEND_EQUATION_MAX) )
#define D3D9_STATE_BLEND_MULTIPLY    (D3D9_STATE_BLEND_FUNC(D3D9_STATE_BLEND_DST_COLOR, D3D9_STATE_BLEND_ZERO         ) )
#define D3D9_STATE_BLEND_NORMAL      (D3D9_STATE_BLEND_FUNC(D3D9_STATE_BLEND_ONE,       D3D9_STATE_BLEND_INV_SRC_ALPHA) )
#define D3D9_STATE_BLEND_SCREEN      (D3D9_STATE_BLEND_FUNC(D3D9_STATE_BLEND_ONE,       D3D9_STATE_BLEND_INV_SRC_COLOR) )
#define D3D9_STATE_BLEND_LINEAR_BURN (D3D9_STATE_BLEND_FUNC(D3D9_STATE_BLEND_DST_COLOR, D3D9_STATE_BLEND_INV_DST_COLOR) | D3D9_STATE_BLEND_EQUATION(D3D9_STATE_BLEND_EQUATION_SUB) )

///
#define D3D9_STATE_BLEND_FUNC_RT_x(_src, _dst) (0 \
					| ( uint32_t( (_src)>>D3D9_STATE_BLEND_SHIFT) \
					| ( uint32_t( (_dst)>>D3D9_STATE_BLEND_SHIFT)<<4) ) \
					)

#define D3D9_STATE_BLEND_FUNC_RT_xE(_src, _dst, _equation) (0 \
					| D3D9_STATE_BLEND_FUNC_RT_x(_src, _dst) \
					| ( uint32_t( (_equation)>>D3D9_STATE_BLEND_EQUATION_SHIFT)<<8) \
					)

#define D3D9_STATE_BLEND_FUNC_RT_1(_src, _dst)  (D3D9_STATE_BLEND_FUNC_RT_x(_src, _dst)<< 0)
#define D3D9_STATE_BLEND_FUNC_RT_2(_src, _dst)  (D3D9_STATE_BLEND_FUNC_RT_x(_src, _dst)<<11)
#define D3D9_STATE_BLEND_FUNC_RT_3(_src, _dst)  (D3D9_STATE_BLEND_FUNC_RT_x(_src, _dst)<<22)

#define D3D9_STATE_BLEND_FUNC_RT_1E(_src, _dst, _equation) (D3D9_STATE_BLEND_FUNC_RT_xE(_src, _dst, _equation)<< 0)
#define D3D9_STATE_BLEND_FUNC_RT_2E(_src, _dst, _equation) (D3D9_STATE_BLEND_FUNC_RT_xE(_src, _dst, _equation)<<11)
#define D3D9_STATE_BLEND_FUNC_RT_3E(_src, _dst, _equation) (D3D9_STATE_BLEND_FUNC_RT_xE(_src, _dst, _equation)<<22)

