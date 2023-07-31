// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

#include "dsi.h"

// Size of the inner heap (adjust as necessary).
#define INNER_HEAP_SIZE 0x80000

#ifdef __cplusplus
extern "C"
{
#endif

    // Sysmodules should not use applet*.
    u32 __nx_applet_type = AppletType_None;

    // Sysmodules will normally only want to use one FS session.
    u32 __nx_fs_num_sessions = 1;

    // Newlib heap configuration function (makes malloc/free work).
    void __libnx_initheap(void)
    {
        static u8 inner_heap[INNER_HEAP_SIZE];
        extern void *fake_heap_start;
        extern void *fake_heap_end;

        // Configure the newlib heap.
        fake_heap_start = inner_heap;
        fake_heap_end = inner_heap + sizeof(inner_heap);
    }

    // Service initialization.
    void __appInit(void)
    {
        Result rc;

        // Retrieve the current version of Horizon OS.
        rc = setsysInitialize();
        if (R_SUCCEEDED(rc))
        {
            SetSysFirmwareVersion fw;
            rc = setsysGetFirmwareVersion(&fw);
            if (R_SUCCEEDED(rc))
                hosversionSet(MAKEHOSVERSION(fw.major, fw.minor, fw.micro));
            setsysExit();
        }
    }

    // Service deinitialization.
    void __appExit(void)
    {
    }

#ifdef __cplusplus
}
#endif

static int64_t GetTick()
{
    int64_t tick;
    __asm__ __volatile__("mrs %[tick], cntpct_el0"
                         : [tick] "=&r"(tick)::"memory");
    return tick;
}

static int64_t ConvertToTick(int64_t ns)
{
    return ns * 192L / 10000L;
}

static uintptr_t dsiBase = 0;
static uintptr_t dsiSize = 0;

static void dsi_wait_trigger()
{
    auto timeout = GetTick() + ConvertToTick(250000000L);
    auto dsi = reinterpret_cast<volatile uint32_t *>(dsiBase);

    for (;;)
    {
        if (GetTick() > timeout)
            // fatalThrow(MAKERESULT(Module_Libnx, 2139));
            break;

        if (dsi[DSI_TRIGGER] == 0)
            break;
    }

    // svcSleepThread(5000000L);
}

static void dsi_send_cmd(uint8_t cmd, uint32_t param, uint32_t wait)
{
    auto dsi = reinterpret_cast<volatile uint32_t *>(dsiBase);

    dsi[DSI_VIDEO_MODE_CONTROL] = DSI_CMD_PKT_VID_ENABLE | DSI_DSI_LINE_TYPE(4);

    uint32_t host_control = dsi[DSI_HOST_CONTROL];
    dsi[DSI_HOST_CONTROL] = (host_control & ~(DSI_HOST_CONTROL_TX_TRIG_MASK)) | DSI_HOST_CONTROL_TX_TRIG_HOST;

    dsi[DSI_WR_DATA] = (param << 8) | cmd;
    dsi[DSI_TRIGGER] = DSI_TRIGGER_HOST;

    dsi_wait_trigger();

    dsi[DSI_HOST_CONTROL] = host_control;
    dsi[DSI_VIDEO_MODE_CONTROL] = 0;
}

// Main program entrypoint
int main(int argc, char *argv[])
{
    Result rc = svcQueryIoMapping(&dsiBase, &dsiSize, DSI_BASE, DSI_SIZE);
    if (R_FAILED(rc))
    {
        fatalThrow(rc);
        return 1;
    }

    const uint32_t flags[4] = {
        DCS_ADDRESS_MODE_BGR_COLOR,
        0,
        DCS_ADDRESS_MODE_H_FLIP | DCS_ADDRESS_MODE_V_FLIP,
        DCS_ADDRESS_MODE_H_FLIP,
    };
    uint32_t ctr = 0;

    while (true)
    {
        dsi_send_cmd(MIPI_DSI_DCS_SHORT_WRITE_PARAM,
                     MIPI_DCS_SET_ADDRESS_MODE | ((flags[ctr++ % 4]) << 8),
                     20000);

        svcSleepThread(1000000000L);
    }
    return 0;
}
