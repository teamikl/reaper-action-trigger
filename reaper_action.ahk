argc = %0%
action = %1%

IfWinExist ahk_class REAPERwnd
{
  WinActivate
  if (argc == 1) {
    SendMessage 0x111, action, ,
  }
}