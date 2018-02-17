#ifdef USE_SDL
#include "dx_linux.h"
#else
#include <windows.h>
#include <windowsx.h>
#include <dsound.h>
#endif

extern	IDirectSoundBuffer8 *MakeSoundBuffer(IDirectSound8 *ds, char* lpSampleName);
extern	void *GetWAVRes(HMODULE hModule, char* lpResName);
extern	BOOL WriteWAVData( IDirectSoundBuffer8 *pDSB, BYTE *pbWaveData, DWORD cbWaveSize );
extern	BOOL UnpackWAVChunk( void *pRIFFBytes, LPWAVEFORMATEX *lpwfmx, BYTE **ppbWaveData, DWORD *pcbWaveSize );

