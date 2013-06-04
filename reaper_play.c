
#include <windows.h>

#define REAPER_ACTION_PLAY 1007

int main(void)
{
  HWND reaper = FindWindow("REAPERwnd", NULL);

  if (reaper != NULL) {
    SendMessage(reaper, WM_ACTIVATE, WA_ACTIVE, 0);
    SendMessage(reaper, WM_COMMAND, REAPER_ACTION_PLAY, 0);
  }

  return 0;
}
