#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "track_settings.h"

int find_center_line(uint16_t *pixels)
{
    static int previousCenter = 64;

    uint16_t smoothingFilter[128];
    smoothingFilter[0]   = pixels[0];
    smoothingFilter[127] = pixels[127];

    // --- SMOOTHING (3-point) ---
    for (int i = 1; i < 127; i++)
    {
        smoothingFilter[i] = (pixels[i - 1] + pixels[i] + pixels[i + 1]) / 3;
    }

    int leftEdge  = -1;
    int rightEdge = -1;
    int threshold = 275;

    // --- LEFT EDGE (dark -> light) ---
    for (int i = 12; i < 118; i++)
    {
        int diff = (int)smoothingFilter[i] - (int)smoothingFilter[i - 1];
        if (diff > threshold)
        {
            leftEdge = i;
            break;
        }
    }

    // --- RIGHT EDGE (light -> dark) ---
    // Only search to the right of leftEdge to avoid grabbing the same line twice
    int searchStart = (leftEdge != -1) ? leftEdge + 15 : 10;
    for (int i = 116; i > searchStart; i--)
    {
        int diff = (int)smoothingFilter[i] - (int)smoothingFilter[i + 1];
        if (diff > threshold)
        {
            rightEdge = i;
            break;
        }
    }

    // --- CENTER CALCULATION ---
    if (leftEdge != -1 && rightEdge != -1 && rightEdge > leftEdge)
    {
        previousCenter = (leftEdge + rightEdge) / 2;
    }
    else if (leftEdge != -1 && rightEdge == -1)
    {
        int estimatedCenter = leftEdge + 20;
        previousCenter = (previousCenter * 3 + estimatedCenter) / 4;
    }
    else if (rightEdge != -1 && leftEdge == -1)
    {
        int estimatedCenter = rightEdge - 20;
        previousCenter = (previousCenter * 3 + estimatedCenter) / 4;
    }

    // Clamp
    if (previousCenter < 0)   previousCenter = 0;
    if (previousCenter > 127) previousCenter = 127;

    return previousCenter;
}

int is_carpet(uint16_t *pixels)
{
    uint32_t sum = 0;
    for (int i = 0; i < 128; i++)
    {
        sum += pixels[i];
    }
    if ((sum / 128) < 2000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}