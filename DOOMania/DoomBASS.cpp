#include "DoomBASS.h"

//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "Chocolate DOOM/i_sound.h"
#include "Chocolate DOOM/mus2mid.h"
#include "Chocolate DOOM/m_misc.h"
#include "Chocolate DOOM/deh_str.h"
#include "Chocolate DOOM/w_wad.h"
#include "Chocolate DOOM/z_zone.h"
#ifdef __cplusplus
}
#endif

#include "includes/bass/c/bass.h"
#include "includes/bassmidi/c/bassmidi.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <map>

namespace DoomBASS
{
    //namespace StreamMusic
    //{
    //    HSTREAM handle;
    //
    //    static int Load(std::filesystem::path filePath, boolean looping)
    //    {
    //        DWORD flags = BASS_ASYNCFILE | BASS_UNICODE;
    //
    //        // #TODO: read file tags for loop info, get code from XNFSMusicPlayer
    //        if (looping)
    //            flags |= BASS_SAMPLE_LOOP;
    //
    //        if (handle)
    //            BASS_ChannelFree(handle);
    //
    //        handle = BASS_StreamCreateFile(FALSE, filePath.wstring().c_str(), 0, 0, flags);
    //        if (!handle)
    //        {
    //            return BASS_ErrorGetCode();
    //        }
    //
    //        return 0;
    //    }
    //
    //    static int Play()
    //    {
    //        if (!handle)
    //            return -1;
    //
    //        if (!BASS_ChannelPlay(handle, TRUE))
    //            return BASS_ErrorGetCode();
    //
    //        return 0;
    //    }
    //}

    namespace MidiMusic
    {
        HSTREAM handle;
        HSOUNDFONT soundfont;
        BASS_MIDI_FONT fontstruct;

        void* midbuffer;
        size_t midbuffer_len;

        float midivol = 1.0f;

        static int LoadSoundFont(std::filesystem::path filePath)
        {
            DWORD flags = BASS_UNICODE;

            if (soundfont)
                BASS_MIDI_FontFree(soundfont);

            printf("Loading soundfont: %s\n", filePath.string().c_str());

            soundfont = BASS_MIDI_FontInit(filePath.wstring().c_str(), flags);
            if (!soundfont)
            {
                int result = BASS_ErrorGetCode();
                if (result)
                {
                    printf("Error loading soundfont: %d\n", result);
                }

                return result;
            }

            fontstruct.font = soundfont;
            fontstruct.bank = 0;
            fontstruct.preset = -1;

            return 0;
        }

        static void FreeSoundfont()
        {
            BASS_MIDI_FontFree(soundfont);
        }

        //static int WriteMemFile(std::filesystem::path filePath, void* buf, size_t len)
        //{
        //    std::ofstream ofile;
        //    ofile.open(filePath, std::ios::binary);
        //    if (!ofile.is_open())
        //        return -1;
        //
        //    ofile.write((const char*)buf, len);
        //
        //    ofile.flush();
        //    ofile.close();
        //
        //    return 0;
        //}

        // code from i_winmusic.c
        static boolean IsMid(byte* mem, int len)
        {
            return len > 4 && !memcmp(mem, "MThd", 4);
        }

        static void ReleaseMidBuffer()
        {
            if (midbuffer != NULL)
            {
                free(midbuffer);
                midbuffer = 0;
                midbuffer_len = 0;
            }
        }

        static void LoadMidBuffer(void* data, size_t len)
        {
            if (midbuffer != NULL)
            {
                ReleaseMidBuffer();
            }

            midbuffer_len = len;
            midbuffer = malloc(len);
            memcpy(midbuffer, data, len);
        }

        static int LoadFromBuffer(void* buf, size_t len)
        {
            DWORD flags = BASS_MIDI_NOCROP;

            if (handle)
                BASS_ChannelFree(handle);

            if (!soundfont)
                return -1;

            handle = BASS_MIDI_StreamCreateFile(TRUE, buf, 0, len, flags, 1);

            if (!handle)
                return BASS_ErrorGetCode();

            if (!BASS_MIDI_StreamSetFonts(handle, &fontstruct, 1))
            {
                return BASS_ErrorGetCode();
            }

            return 0;
        }

        static int Load(std::filesystem::path filePath)
        {
            DWORD flags = BASS_MIDI_NOCROP | BASS_UNICODE;

            if (handle)
                BASS_ChannelFree(handle);

            if (!soundfont)
                return -1;

            handle = BASS_MIDI_StreamCreateFile(FALSE, filePath.wstring().c_str(), 0, 0, flags, 1);

            if (!handle)
                return BASS_ErrorGetCode();

            if (!BASS_MIDI_StreamSetFonts(handle, &fontstruct, 1))
            {
                return BASS_ErrorGetCode();
            }

            return 0;
        }

        static int SetVolume(float volume)
        {
            if (!handle)
                return -1;

            if (!BASS_ChannelSetAttribute(handle, BASS_ATTRIB_VOL, volume))
            {
                return BASS_ErrorGetCode();
            }

            return 0;
        }

        static int Play(boolean looping)
        {
            if (!handle)
                return -1;

            if (looping)
            {
                if (BASS_ChannelFlags(handle, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP) == -1)
                    return BASS_ErrorGetCode();
            }
            else
            {
                int result = BASS_ChannelFlags(handle, 0, 0);
                if (result == -1)
                {
                    return BASS_ErrorGetCode();
                }

                if (result & BASS_SAMPLE_LOOP)
                {
                    result = BASS_ChannelFlags(handle, 0, BASS_SAMPLE_LOOP);
                    if (result == -1)
                    {
                        return BASS_ErrorGetCode();
                    }
                }
            }

            SetVolume(midivol);

            if (!BASS_ChannelPlay(handle, TRUE))
                return BASS_ErrorGetCode();


            return 0;
        }

        static int Pause()
        {
            if (!handle)
                return -1;

            if (!BASS_ChannelPause(handle))
                return BASS_ErrorGetCode();

            return 0;
        }

        static int Resume()
        {
            if (!handle)
                return -1;

            SetVolume(midivol);

            if (!BASS_ChannelPlay(handle, FALSE))
                return BASS_ErrorGetCode();

            return 0;
        }

        static int Stop()
        {
            if (!handle)
                return -1;

            if (!BASS_ChannelStop(handle))
                return BASS_ErrorGetCode();

            return 0;
        }

        static boolean IsPlaying(int* errorval)
        {
            DWORD activity = BASS_ChannelIsActive(handle);

            *errorval = BASS_ErrorGetCode();
            return activity == BASS_ACTIVE_PLAYING;
        }



        static boolean ConvertAndLoadMus(byte* musdata, int len/*, std::filesystem::path filePath*/)
        {
            MEMFILE* instream;
            MEMFILE* outstream;
            void* outbuf;
            size_t outbuf_len;
            int result;

            instream = mem_fopen_read(musdata, len);
            outstream = mem_fopen_write();

            result = mus2mid(instream, outstream);

            if (result == 0)
            {
                mem_get_buf(outstream, &outbuf, &outbuf_len);

                if (midbuffer != NULL)
                {
                    ReleaseMidBuffer();
                }

                LoadMidBuffer(outbuf, outbuf_len);
            }

            mem_fclose(instream);
            mem_fclose(outstream);

            return result == 0;
        }

    }
    
    namespace SoundEffects
    {
        constexpr DWORD maxSoundChans = 128;
        constexpr boolean use_sfx_prefix = true;
        
        struct DoomSFX
        {
            HSAMPLE handle;
            uint8_t* data;
            int samplerate;
            int length;
        };

        std::map<std::string, DoomSFX> DoomSFXInfos;

        static void GetSfxLumpName(sfxinfo_t* sfx, char* buf, size_t buf_len)
        {
            // Linked sfx lumps? Get the lump number for the sound linked to.

            if (sfx->link != NULL)
            {
                sfx = sfx->link;
            }

            // Doom adds a DS* prefix to sound lumps; Heretic and Hexen don't
            // do this.

            //if (use_sfx_prefix)
            {
                M_snprintf(buf, buf_len, "ds%s", DEH_String(sfx->name));
            }
            //else
            //{
            //    M_StringCopy(buf, DEH_String(sfx->name), buf_len);
            //}
        }

        static int GetSfxLumpNum(sfxinfo_t* sfx)
        {
            char namebuf[9];

            GetSfxLumpName(sfx, namebuf, sizeof(namebuf));

            return W_GetNumForName(namebuf);
        }

        static int ExpandSoundData(sfxinfo_t* sfxinfo,
            byte* data,
            int samplerate,
            int length)
        {
            DoomSFX sound = {0, nullptr, samplerate, length};

            DWORD flags = BASS_SAMPLE_OVER_POS | BASS_SAMPLE_MONO | BASS_SAMPLE_8BITS;

            HSAMPLE samp = BASS_SampleCreate(length, samplerate, 1, maxSoundChans, flags);
            if (!samp)
            {
                return BASS_ErrorGetCode();
            }

            uint8_t* newData = (uint8_t*)malloc(length);
            memcpy(newData, data, length);

            if (BASS_SampleSetData(samp, newData) == FALSE)
            {
                free(newData);
                return BASS_ErrorGetCode();
            }

            sound.data = newData;

            sound.handle = samp;

            std::string name = sfxinfo->name;

            DoomSFXInfos[name] = sound;

            return 0;
        }

        static void Cache(sfxinfo_t* sfxinfo)
        {
            int lumpnum;
            unsigned int lumplen;
            int samplerate;
            unsigned int length;
            byte* data;

            // need to load the sound

            lumpnum = sfxinfo->lumpnum;
            data = (byte*)W_CacheLumpNum(lumpnum, PU_STATIC);
            lumplen = W_LumpLength(lumpnum);

            // Check the header, and ensure this is a valid sound

            if (lumplen < 8
                || data[0] != 0x03 || data[1] != 0x00)
            {
                // Invalid sound

                return;
            }


            // 16 bit sample rate field, 32 bit length field

            samplerate = (data[3] << 8) | data[2];
            length = (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4];

            // If the header specifies that the length of the sound is greater than
            // the length of the lump itself, this is an invalid sound lump

            // We also discard sound lumps that are less than 49 samples long,
            // as this is how DMX behaves - although the actual cut-off length
            // seems to vary slightly depending on the sample rate.  This needs
            // further investigation to better understand the correct
            // behavior.

            if (length > lumplen - 8 || length <= 48)
            {
                return;
            }

            // The DMX sound library seems to skip the first 16 and last 16
            // bytes of the lump - reason unknown.

            data += 16;
            length -= 32;


            if (ExpandSoundData(sfxinfo, data + 8, samplerate, length) != 0)
            {
                return;
            }

            // don't need the original lump any more
            W_ReleaseLumpNum(lumpnum);

        }

        static void Precache(sfxinfo_t* sounds, int num_sounds)
        {
            char namebuf[9];

            printf("DoomBASS: precaching sound effects");

            for (int i = 0; i < num_sounds; i++)
            {
                GetSfxLumpName(&sounds[i], namebuf, sizeof(namebuf));
                sounds[i].lumpnum = W_CheckNumForName(namebuf);
                strncpy(sounds[i].name, namebuf, 9);

                if (sounds[i].lumpnum != -1)
                {
                    Cache(&sounds[i]);
                }
            }
        }

        static DoomSFX* GetSFXHandle(sfxinfo_t* sfxinfo)
        {
            std::string soundName = sfxinfo->name;

            auto it = DoomSFXInfos.find(soundName);
            if (it == DoomSFXInfos.end())
                return nullptr;

            return &it->second;
        }

        // #TODO: ADD BASS3D AUDIO
        static HCHANNEL Start(sfxinfo_t* sfxinfo, int vol, int sep, int pitch)
        {
            DoomSFX* hSound = GetSFXHandle(sfxinfo);

            if (hSound == nullptr)
                return -1;

            DWORD flags = 0;

            HCHANNEL chan = BASS_SampleGetChannel(hSound->handle, flags);

            if (!chan)
            {
                //return BASS_ErrorGetCode();
                return -1;
            }

            float newVol = (float)vol / 127.0f;
            float newSep = std::lerp(-1.0f, 1.0f, (float)sep / 256.0f);
            float newPitch = (float)pitch / (float)NORM_PITCH;

            BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, newVol);
            BASS_ChannelSetAttribute(chan, BASS_ATTRIB_PAN, newSep);
            BASS_ChannelSetAttribute(chan, BASS_ATTRIB_FREQ, hSound->samplerate * newPitch);

            BASS_ChannelPlay(chan, FALSE);

            return chan;
        }

        static void Stop(HCHANNEL chan)
        {
            BASS_ChannelStop(chan);
        }

        static bool SoundIsPlaying(HCHANNEL chan)
        {
            return BASS_ChannelIsActive(chan) == BASS_ACTIVE_PLAYING;
        }

        // #TODO: ADD BASS3D AUDIO
        static void UpdateSoundParams(HCHANNEL chan, int vol, int sep)
        {
            float newVol = (float)vol / 127.0f;
            float newSep = std::lerp(-1.0f, 1.0f, (float)sep / 256.0f);

            BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, newVol);
            BASS_ChannelSetAttribute(chan, BASS_ATTRIB_PAN, newSep);
        }

        static void Shutdown()
        {
            for (const auto& info : DoomSFXInfos)
            {
                BASS_SampleFree(info.second.handle);
                free(info.second.data);

            }

            DoomSFXInfos.clear();
        }

    }

}

//
// SFX START
//

static void DoomBASS_ShutdownSFX(void)
{
    DoomBASS::SoundEffects::Shutdown();
}

static int DoomBASS_GetSfxLumpNum(sfxinfo_t* sfx)
{
    return DoomBASS::SoundEffects::GetSfxLumpNum(sfx);
}

static void DoomBASS_UpdateSoundParams(int channel, int vol, int sep)
{
    return DoomBASS::SoundEffects::UpdateSoundParams(channel, vol, sep);
}

static int DoomBASS_StartSound(sfxinfo_t* sfxinfo, int channel, int vol, int sep, int pitch)
{
    return DoomBASS::SoundEffects::Start(sfxinfo, vol, sep, pitch);
}

static void DoomBASS_StopSound(int channel)
{
    return DoomBASS::SoundEffects::Stop(channel);
}

static boolean DoomBASS_SoundIsPlaying(int channel)
{
    return DoomBASS::SoundEffects::SoundIsPlaying(channel);
}

static void DoomBASS_CacheSounds(sfxinfo_t* sounds, int num_sounds)
{
    return DoomBASS::SoundEffects::Precache(sounds, num_sounds);
}

//
// SFX END
//


//
// MUSIC START
//

static boolean DoomBASS_InitMidi(void)
{
    return DoomBASS::MidiMusic::LoadSoundFont("DOOMSona.sf2"); // #TODO: configurable filename maybe?
}

static void DoomBASS_ShutdownMidi(void)
{
    DoomBASS::MidiMusic::FreeSoundfont();
}

static void DoomBASS_SetMusicVolume(int volume)
{
    DoomBASS::MidiMusic::midivol = (float)volume / 127.0f;
    DoomBASS::MidiMusic::SetVolume(DoomBASS::MidiMusic::midivol);
}

static void DoomBASS_PauseSong(void)
{
    DoomBASS::MidiMusic::Pause();
}

static void DoomBASS_ResumeSong(void)
{
    DoomBASS::MidiMusic::Resume();
}

static void* DoomBASS_RegisterSong(void* data, int len)
{
    if (DoomBASS::MidiMusic::IsMid((byte*)data, len))
    {
        DoomBASS::MidiMusic::LoadMidBuffer(data, len);
    }
    else
    {
        if (!DoomBASS::MidiMusic::ConvertAndLoadMus((byte*)data, len))
            return NULL;
    }

    DoomBASS::MidiMusic::LoadFromBuffer(DoomBASS::MidiMusic::midbuffer, DoomBASS::MidiMusic::midbuffer_len);

    return &DoomBASS::MidiMusic::handle;
}

static void DoomBASS_UnRegisterSong(void* handle)
{
    if (handle)
    {
        BASS_ChannelFree(*(HSTREAM*)handle);
        *(HSTREAM*)handle = 0;
    }

    DoomBASS::MidiMusic::ReleaseMidBuffer();
}

static void DoomBASS_PlaySong(void* handle, boolean looping)
{
    DoomBASS::MidiMusic::Play(looping);
}

static void DoomBASS_StopSong(void)
{
    DoomBASS::MidiMusic::Stop();
}


static boolean DoomBASS_MusicIsPlaying(void)
{
    int errval = 0;
    return DoomBASS::MidiMusic::IsPlaying(&errval);
}

//
// MUSIC END
//

static boolean bBassInited = 0;
boolean DoomBASS_GetBassInited()
{
    return bBassInited;
}

void DoomBASS_SetBassInited(boolean val)
{
    bBassInited = val;
}

int DoomBASS_Shutdown()
{
    bBassInited = false;

    DoomBASS_ShutdownMidi();

    if (!BASS_Free())
    {
        return BASS_ErrorGetCode();
    }

    return 0;
}

int DoomBASS_Init(int samplerate)
{
    if (!BASS_Init(-1, samplerate, 0, 0, NULL))
    {
        return BASS_ErrorGetCode();
    }

    DoomBASS_InitMidi();

    //int result = DoomBASS::MidiMusic::Load("level1.mid");
    //
    //result = DoomBASS::MidiMusic::Play(false);

    bBassInited = true;
    return 0;
}



static snddevice_t DoomBASS_devices[] =
{
    SNDDEVICE_PAS,
    SNDDEVICE_WAVEBLASTER,
    SNDDEVICE_SOUNDCANVAS,
    SNDDEVICE_GENMIDI,
    SNDDEVICE_AWE32,
};

//
// SFX START
//

const sound_module_t DoomBASS_SFX_module =
{
    DoomBASS_devices,
    arrlen(DoomBASS_devices),
    NULL,
    DoomBASS_ShutdownSFX,
    DoomBASS_GetSfxLumpNum,
    NULL,
    DoomBASS_UpdateSoundParams,
    DoomBASS_StartSound,
    DoomBASS_StopSound,
    DoomBASS_SoundIsPlaying,
    DoomBASS_CacheSounds,
};

//
// SFX END
//

//
// MUSIC START
//

const music_module_t DoomBASS_module =
{
    DoomBASS_devices,
    arrlen(DoomBASS_devices),
    DoomBASS_InitMidi,
    DoomBASS_ShutdownMidi,
    DoomBASS_SetMusicVolume,
    DoomBASS_PauseSong,
    DoomBASS_ResumeSong,
    DoomBASS_RegisterSong,
    DoomBASS_UnRegisterSong,
    DoomBASS_PlaySong,
    DoomBASS_StopSong,
    DoomBASS_MusicIsPlaying,
    NULL,  // Poll
};

//
// MUSIC END
//