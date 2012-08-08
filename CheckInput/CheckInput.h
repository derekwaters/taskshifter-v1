////////////////////////////////////////////////////////////////

#define DLLIMPORT __declspec(dllimport)

#pragma comment ( lib, "tschinp.lib" )

DLLIMPORT BOOL CheckInputInit();
DLLIMPORT void CheckInputTerm();
DLLIMPORT DWORD CheckInputGetLastInputTime();
