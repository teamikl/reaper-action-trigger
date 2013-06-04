
#include <stdlib.h>
#include <windows.h>

int main(int argc, char **argv)
{
  long action = 0;
  HWND reaper = FindWindow("REAPERwnd", NULL);

  if (argc == 2) {
    action = atol(argv[1]);
  } else {
    return EXIT_FAILURE;
  }

  if (reaper != NULL) {
    SendMessage(reaper, WM_ACTIVATE, WA_ACTIVE, 0);

    if (action > 0) {
      SendMessage(reaper, WM_COMMAND, action, 0);
    }
  }

  return EXIT_SUCCESS;
}
