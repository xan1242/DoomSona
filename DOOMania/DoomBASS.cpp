#include "DoomBASS.h"

//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "Chocolate DOOM/i_sound.h"
#include "Chocolate DOOM/mus2mid.h"
#ifdef __cplusplus
}
#endif

#include "includes/bass/c/bass.h"
#include "includes/bassmidi/c/bassmidi.h"
#include <filesystem>
#include <fstream>

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
}

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
