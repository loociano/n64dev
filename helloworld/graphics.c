#include <nusys.h>
#include <PR/gs2dex.h>

#define GFX_LIST_NUM 4
#define GFX_LIST_SIZE 2048

NUContData contData[4];

typedef struct st_Display {
    u32	width;
    u32	high;
} ScreenSize;

ScreenSize screen[] = {
    {320, 240},
    {640, 480}
};

Gfx	gfxListBuf[GFX_LIST_NUM][GFX_LIST_SIZE];
u32	gfxListCnt = 0;
Gfx*	gfxListPtr;
Gfx*	gfxListStartPtr;

void gfxClearCfb(u32 size)
{
    ScreenSize* screenPtr;
    screenPtr = &screen[size];

    gSPSegment(gfxListPtr++, 0, 0x0);
    gDPSetCycleType(gfxListPtr++, G_CYC_FILL);
    
    // Clear Z-buffer
    gDPSetDepthImage(gfxListPtr++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetColorImage(gfxListPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		     screenPtr->width,
		     OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetFillColor(gfxListPtr++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 |
				  GPACK_ZDZ(G_MAXFBZ,0)));
    gDPFillRectangle(gfxListPtr++, 0, 0, screen->width-1, screenPtr->high-1);

    gDPPipeSync(gfxListPtr++);

    gDPSetColorImage(gfxListPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		     screenPtr->width,
		     OS_K0_TO_PHYSICAL(nuGfxCfb_ptr)); 
    gDPSetFillColor(gfxListPtr++, (GPACK_RGBA5551(0, 0, 0, 1) << 16
				     | GPACK_RGBA5551(0, 0, 0, 1)));
    gDPFillRectangle(gfxListPtr++, 0, 0,
		     screenPtr->width-1, screenPtr->high - 1);
    gDPPipeSync(gfxListPtr++);
}

void gfxListBufferChange()
{
    gfxListStartPtr = &gfxListBuf[gfxListCnt][0];
    gfxListPtr = gfxListStartPtr;
    gfxListCnt++;
    gfxListCnt %= GFX_LIST_NUM;
}
