#include "Kernel.h"

int DrawPixel(UINT64 *VideoHandle, UINT32 Color, int X, int Y);

UINT64 main(BOOT_CONFIG *BootConfig) {
    UINT64 PassBack = 0;
    UINT32 Blue = 0xFF00B7FF;
    UINT64 *VideoHandle = (UINT64 *)(BootConfig->VideoConfig.FrameBufferBase);

    for (int i = 0; i < 1080; i++)
    {
        for (int j = 0; j < 1920; j++)
        {
            DrawPixel(VideoHandle, Blue, j, i);
        }
        
    }
    PassBack = BootConfig->VideoConfig.FrameBufferBase;
    return PassBack;
}

int DrawPixel(UINT64 *VideoHandle, UINT32 Color, int X, int Y) {
    UINT32 *Pixel = VideoHandle;
    Pixel += Y * 1920 + X;
    *Pixel = Color;
    return 0;
}