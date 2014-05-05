#include <nds.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>

void initVideo()
{
    /*  Set the video mode on the main screen. */
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);

	//map vram a to start of main background graphics memory
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);

    /*  Set up affine background 3 on main as a 16-bit color background. */
    REG_BG3CNT = BG_BMP16_256x256 |
                 BG_BMP_BASE(0) | // The starting place in memory
                 BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG3PA = 1 << 8;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 1 << 8;

    /*  Place main screen background 3 at the origin (upper left of the
     *  screen).
     */
    REG_BG3X = 0;
    REG_BG3Y = 0;

    /*  Set up affine background 3 on main as a 16-bit color background. */
    REG_BG3CNT_SUB = BG_BMP16_256x256 |
                 BG_BMP_BASE(0) | // The starting place in memory
                 BG_PRIORITY(3); // A low priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG3PA_SUB = 1 << 8;
    REG_BG3PB_SUB = 0;
    REG_BG3PC_SUB = 0;
    REG_BG3PD_SUB = 1 << 8;

    /*  Place main screen background 3 at the origin (upper left of the
     *  screen).
     */
    REG_BG3X_SUB = 0;
    REG_BG3Y_SUB = 0;
}

int main(void)
{
    touchPosition touch;
    srand(time(NULL));
    powerOn(POWER_ALL_2D);

    /*  Configure the VRAM and background control registers. */
    lcdMainOnTop(); // Place the main screen on the bottom physical screen
    initVideo();
	u16* video_buffer_main = (u16*)BG_BMP_RAM(0);
	u16* video_buffer_sub =  (u16*)BG_BMP_RAM_SUB(0);

    while(1)
    {
        swiWaitForVBlank();
        touchRead(&touch);
        video_buffer_sub[256 * touch.py + touch.px] = RGB15(rand() % 32, rand() % 32, rand() % 32) | BIT(15);
        video_buffer_main[256 * touch.py + (256 - touch.px)] = RGB15(rand() % 32, rand() % 32, rand() % 32) | BIT(15);
    }

    return 0;
}
