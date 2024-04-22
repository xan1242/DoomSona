#ifndef __RE_MAIN__
#define __RE_MAIN__

#ifdef __cplusplus
extern "C" {
#endif
#include "sounds.h"
	typedef struct doomerpal {
		char r, g, b;
	} doomerpal;
	__declspec(dllexport) void SetFramebufferEnabled(boolean val);
	__declspec(dllexport) boolean GetFramebufferEnabled();
	void RE_Framebuffer_Dump(char* image, doomerpal* pal);
	int RE_Control_ReadIn();
	int RE_Joy_ReadIn(char a);
	void RE_Control();
	void RE_Music_Play(musicinfo_t* song);
	void RE_Sound_Cache(sfxinfo_t* sounds, int count);
	void RE_Sound_Start(sfxinfo_t* sound, int vol, int sep);
	void RE_Sound_Update(sfxinfo_t* sound, int vol, int sep);
	boolean RE_Sound_IsPlaying(sfxinfo_t* sound);
#ifdef __cplusplus
}
#endif

#endif