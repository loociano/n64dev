#include <nusys.h>

// defined in graphics.c
extern Gfx*	gfxListPtr;
extern Gfx*	gfxListStartPtr;
extern void	gfxListBufferChange();
extern void	gfxClearCfb(u32 size);
extern NUContData contData[4]; // Data Controller data obtained by the Controller Manager

void displayFrame()
{
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTBLUE); // Changes the color of text displayed in the console window
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 13, 13); // Sets the position where text will be displayed inside console window
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "Hello World!"); // Outputs the character string to console window
}

void displayConsole(u32 gfxTaskNum)
{
    gfxListBufferChange();
    gfxClearCfb(0);
    gDPFullSync(gfxListPtr++); // Signals that the RDP has completed its last process
    gSPEndDisplayList(gfxListPtr++); // Ends a display list
    // Start the graphics task
    nuGfxTaskStart(gfxListStartPtr, // pointer to the display list buffer
                   gfxListPtr - gfxListStartPtr, // display list size
                   NU_GFX_UCODE_S2DEX, // microcode to be used
                   NU_SC_NOSWAPBUFFER); // specifies flags for task manager and flags to be handed over to tasks
                                        // Flags specified for the task manager:
                                        // NU_SC_NOSWAPBUFFER = No switching of the frame buffer
    nuDebConDisp(NU_SC_SWAPBUFFER); // Displays the console window. NU_SC_SWAPBUFFER = Swap frame buffer
    nuDebConClear(NU_DEB_CON_WINDOW0); // Clears the console window. NU_DEB_CON_WINDOW0 = Window 0
    displayFrame();
}

void scanInputs()
{
}

void menu()
{
    nuDebConScroll(NU_DEB_CON_WINDOW0, NU_DEB_CON_SCROLL_OFF); // Turns console window scroll feature on/off
    nuDebConWindowShow(NU_DEB_CON_WINDOW1, NU_DEB_CON_WINDOW_OFF); // Shows/hides display of console window
    nuDebConWindowShow(NU_DEB_CON_WINDOW2, NU_DEB_CON_WINDOW_OFF); // Shows/hides display of console window
    nuDebConWindowShow(NU_DEB_CON_WINDOW3, NU_DEB_CON_WINDOW_OFF); // Shows/hides display of console window
    nuGfxDisplayOn(); // Display the screen upon switching to next frame buffer
    nuGfxFuncSet(displayConsole); // Registers the retrace call back function in the graphics thread

    while(1){
        nuGfxRetraceWait(1); // Wait for 1 retrace
        nuContDataGetExAll(contData); // Gets the Controller data
        scanInputs();
    }
}

void mainproc(void* arg)
{
    nuGfxInit(); // Initializes and starts the graphics thread and the graphics task manager
    nuContInit();
    nuContGBPakMgrInit();
    while(1){
        menu();
    }
}



