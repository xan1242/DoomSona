//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2008 David Flater
// Copyright(C) 2020 TheStoneBanana
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	System interface for sound. (Loading sounds from WAD directly into Retro Engine memory, sloppily adapted from 'i_sdlsound.c')
//

#include "../pch.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

extern "C" {
#include "deh_str.h"
#include "i_sound.h"
#include "i_system.h"
#include "i_swap.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"
#include "z_zone.h"

#include "doomtype.h"
}
#include "re_main.h"

typedef struct allocated_sound_s allocated_sound_t;

struct allocated_sound_s
{
    sfxinfo_t* sfxinfo;
    struct Mix_Chunk{
        int allocated;
        uint8_t* abuf;
        uint32_t alen;
        uint8_t volume;     /* Per-sample volume, 0-128 */
    } chunk;
    int use_count;
    int pitch;
    allocated_sound_t* prev, * next;
};

static boolean sound_initialized = false;

static int mixer_freq = 44100;
static boolean use_sfx_prefix = true;

// Doubly-linked list of allocated sounds.
// When a sound is played, it is moved to the head, so that the oldest
// sounds not used recently are at the tail.

static allocated_sound_t* allocated_sounds_head = NULL;
static allocated_sound_t* allocated_sounds_tail = NULL;
static int allocated_sounds_size = 0;

// Hook a sound into the linked list at the head.

static void AllocatedSoundLink(allocated_sound_t* snd)
{
    snd->prev = NULL;

    snd->next = allocated_sounds_head;
    allocated_sounds_head = snd;

    if (allocated_sounds_tail == NULL)
    {
        allocated_sounds_tail = snd;
    }
    else
    {
        snd->next->prev = snd;
    }
}

// Unlink a sound from the linked list.

static void AllocatedSoundUnlink(allocated_sound_t* snd)
{
    if (snd->prev == NULL)
    {
        allocated_sounds_head = snd->next;
    }
    else
    {
        snd->prev->next = snd->next;
    }

    if (snd->next == NULL)
    {
        allocated_sounds_tail = snd->prev;
    }
    else
    {
        snd->next->prev = snd->prev;
    }
}

static void FreeAllocatedSound(allocated_sound_t* snd)
{
    // Unlink from linked list.

    AllocatedSoundUnlink(snd);

    // Keep track of the amount of allocated sound data:

    allocated_sounds_size -= snd->chunk.alen;

    free(snd);
}

// Search from the tail backwards along the allocated sounds list, find
// and free a sound that is not in use, to free up memory.  Return true
// for success.

static boolean FindAndFreeSound(void)
{
    allocated_sound_t* snd;

    snd = allocated_sounds_tail;

    while (snd != NULL)
    {
        if (snd->use_count == 0)
        {
            FreeAllocatedSound(snd);
            return true;
        }

        snd = snd->prev;
    }

    // No available sounds to free...

    return false;
}

// Enforce SFX cache size limit.  We are just about to allocate "len"
// bytes on the heap for a new sound effect, so free up some space
// so that we keep allocated_sounds_size < snd_cachesize

static void ReserveCacheSpace(size_t len)
{
    if (snd_cachesize <= 0)
    {
        return;
    }

    // Keep freeing sound effects that aren't currently being played,
    // until there is enough space for the new sound.

    while (allocated_sounds_size + len > snd_cachesize)
    {
        // Free a sound.  If there is nothing more to free, stop.

        if (!FindAndFreeSound())
        {
            break;
        }
    }
}

// Allocate a block for a new sound effect.

static allocated_sound_t* AllocateSound(sfxinfo_t* sfxinfo, size_t len)
{
    allocated_sound_t* snd;

    // Keep allocated sounds within the cache size.

    ReserveCacheSpace(len);

    // Allocate the sound structure and data.  The data will immediately
    // follow the structure, which acts as a header.

    do
    {
        snd = (allocated_sound_t*)malloc(sizeof(allocated_sound_t) + len);

        // Out of memory?  Try to free an old sound, then loop round
        // and try again.

        if (snd == NULL && !FindAndFreeSound())
        {
            return NULL;
        }

    } while (snd == NULL);

    // Skip past the chunk structure for the audio buffer

    snd->chunk.abuf = (byte*)(snd + 1);
    snd->chunk.alen = len;
    snd->chunk.allocated = 1;
    snd->chunk.volume = 128;
    snd->pitch = NORM_PITCH;

    snd->sfxinfo = sfxinfo;
    snd->use_count = 0;

    // Keep track of how much memory all these cached sounds are using...

    allocated_sounds_size += len;

    AllocatedSoundLink(snd);

    return snd;
}

// Lock a sound, to indicate that it may not be freed.

static void LockAllocatedSound(allocated_sound_t* snd)
{
    // Increase use count, to stop the sound being freed.

    ++snd->use_count;

    //printf("++ %s: Use count=%i\n", snd->sfxinfo->name, snd->use_count);

    // When we use a sound, re-link it into the list at the head, so
    // that the oldest sounds fall to the end of the list for freeing.

    AllocatedSoundUnlink(snd);
    AllocatedSoundLink(snd);
}

// Search through the list of allocated sounds and return the one that matches
// the supplied sfxinfo entry and pitch level.

static allocated_sound_t* GetAllocatedSoundBySfxInfoAndPitch(sfxinfo_t* sfxinfo, int pitch)
{
    allocated_sound_t* p = allocated_sounds_head;

    while (p != NULL)
    {
        if (p->sfxinfo == sfxinfo && p->pitch == pitch)
        {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

static boolean ConvertibleRatio(int freq1, int freq2)
{
    int ratio;

    if (freq1 > freq2)
    {
        return ConvertibleRatio(freq2, freq1);
    }
    else if ((freq2 % freq1) != 0)
    {
        // Not in a direct ratio

        return false;
    }
    else
    {
        // Check the ratio is a power of 2

        ratio = freq2 / freq1;

        while ((ratio & 1) == 0)
        {
            ratio = ratio >> 1;
        }

        return ratio == 1;
    }
}

// Generic sound expansion function for any sample rate.
// Returns number of clipped samples (always 0).

static boolean ExpandSoundData(sfxinfo_t* sfxinfo,
    byte* data,
    int samplerate,
    int length)
{
    allocated_sound_t* snd;
    allocated_sound_s::Mix_Chunk* chunk;
    uint32_t expanded_length;

    // Calculate the length of the expanded version of the sample.

    expanded_length = (uint32_t)((((uint64_t)length) * mixer_freq) / samplerate);

    // Double up twice: 8 -> 16 bit and mono -> stereo

    //expanded_length *= 2;

    // Allocate a chunk in which to expand the sound

    snd = AllocateSound(sfxinfo, expanded_length);

    if (snd == NULL)
    {
        return false;
    }

    chunk = &snd->chunk;

    int8_t* expanded = (int8_t*)chunk->abuf;
    //int expanded_length;
    int expand_ratio;
    int i;

    // Generic expansion if conversion does not work:
    //
    // SDL's audio conversion only works for rate conversions that are
    // powers of 2; if the two formats are not in a direct power of 2
    // ratio, do this naive conversion instead.

    // number of samples in the converted sound

    expanded_length = ((uint64_t)length * mixer_freq) / samplerate;
    expand_ratio = (length << 8) / expanded_length;

    for (i = 0; i < expanded_length; ++i)
    {
        int8_t sample;
        int src;

        src = (i * expand_ratio) >> 8;

        sample = data[src];
        //sample -= 32768;

        // expand 8->16 bits, mono->stereo

        expanded[i] = sample;
    }

    return true;
}

static void GetSfxLumpName(sfxinfo_t* sfx, char* buf, size_t buf_len)
{
    // Linked sfx lumps? Get the lump number for the sound linked to.

    if (sfx->link != NULL)
    {
        sfx = sfx->link;
    }

    // Doom adds a DS* prefix to sound lumps; Heretic and Hexen don't
    // do this.

    if (use_sfx_prefix)
    {
        M_snprintf(buf, buf_len, "ds%s", DEH_String(sfx->name));
    }
    else
    {
        M_StringCopy(buf, DEH_String(sfx->name), buf_len);
    }
}

// ----------------------------------------------------------------------------
// Caching the DOOM sounds into Retro Engine's internal format
// ----------------------------------------------------------------------------
// typedef struct {
//     char hash[0x10];									// md5 hash of the sound filename (128 bits)
//     float* dataPtr;
//     int dataSize;
//     int null;
//     unsigned char maxConcurrentPlays;
//     SonicMania::Scope scope;
// } soundSlot;
// 
// FastcallFunctionPointer(int, Engine_GenerateHash, (char* Hash, int Length), 0x1CB620);
// FunctionPointer(char*, sub_5CE810, (unsigned int a1, int a2, unsigned int a3, int a4), 0x1CE810);
// 
// DataArray(char, Engine_HashBuffer, 0x43CDA8, 0x400);
// DataArray(soundSlot, SFXScopes, 0xA5AC98, 256);
// 
// int dumbA1 = 0, dumbA2 = 0;
// const int loc_5CE810 = baseAddress + 0x1CE810;
// __declspec(naked) static void Call_5CE810(void) {
//     __asm {
//         push    edx
//         push    ecx
// 
//         mov     edx,[dumbA1]
//         mov     ecx,[dumbA2]
//         push    0
//         push    2
//         call	loc_5CE810
// 
//         pop     ecx
//         pop     edx
// 
//         add     esp, 8
// 
//         retn
//     }
// }

void RE_Sound_Cache(sfxinfo_t* sounds, int count) {
    // SonicMania::Engine_ClearSoundFXData();                                      // for safety, clear SFX data ourselves (prevents a dev menu crash)
    // char lumpName[9];
    // 
    // printf("RE_Sound_Cache: ");
    // 
    // // For each of the sounds...
    // for (int i = 0; i < count; i++) {
    //     GetSfxLumpName(&sounds[i], lumpName, sizeof(lumpName));                 // get the lump name of the sound we're looking for
    //     sounds[i].lumpnum = W_CheckNumForName(lumpName);                        // does the sound exist in the supplied WAD?
    //     // If the sound exists...
    //     if (sounds[i].lumpnum != -1) {
    //         // Read the contents of the sound lump into a buffer
    //         byte* lumpDat = (byte*)W_CacheLumpNum(sounds[i].lumpnum, PU_STATIC);
    //         unsigned int lumpLen = W_LumpLength(sounds[i].lumpnum);
    // 
    //         // If the lump's header indicates that this sound is invalid...
    //         if (lumpLen < 8 || lumpDat[0] != 0x03 || lumpDat[1] != 0x00) {
    //             W_ReleaseLumpNum(sounds[i].lumpnum);                            // free the lump buf
    //             continue;                                                       // keep checking sounds
    //         }
    // 
    //         // Read the sample rate and the sound length from the lump
    //         unsigned int sndRate = (lumpDat[3] << 8) | lumpDat[2];
    //         unsigned int sndLen = (lumpDat[7] << 24) | (lumpDat[6] << 16) | (lumpDat[5] << 8) | lumpDat[4];
    //         // If the length indicates an invalid sound...
    //         if (sndLen > lumpLen - 8 || sndLen <= 48) {
    //             W_ReleaseLumpNum(sounds[i].lumpnum);                            // free the lump buf
    //             continue;                                                       // keep checking sounds
    //         }
    // 
    //         // We know we have a valid sound now, so try to find a free slot to load it into
    //         short freeSlot = -1;
    //         // For each of the existing sound slots...
    //         for (int slot = 0; slot < 256; slot++) {
    //             // If a sound isn't loaded into this slot...
    //             if (!*(int*)SFXScopes[slot].hash) {
    //                 freeSlot = slot;										    // we found our free slot
    //                 break;													    // get out
    //             }
    //         }
    // 
    //         // If a free slot was found...
    //         if (freeSlot != -1) {
    //             lumpDat += 16;
    //             sndLen -= 32;
    //             // Try to expand the data to a higher rate. If successful...
    //             if (ExpandSoundData(&sounds[i], lumpDat+ 8, sndRate, sndLen)) {
    //                 allocated_sound_t* snd = GetAllocatedSoundBySfxInfoAndPitch(&sounds[i], NORM_PITCH);
    // 
    //                 // Generate a hash for the lump's name
    //                 char HASH[16];
    //                 memset(Engine_HashBuffer, 0, 0x400);						// init the hash buffer
    //                 memcpy_s(Engine_HashBuffer, 0x400, lumpName, sizeof(lumpName));
    //                 Engine_GenerateHash(HASH, strlen((const char*)lumpName));
    //                 memcpy_s(SFXScopes[freeSlot].hash, 16, HASH, 16);
    // 
    //                 dumbA1 = snd->chunk.alen * sizeof(float);
    //                 dumbA2 = (int)&SFXScopes[freeSlot].dataPtr;
    //                 Call_5CE810();
    //                 SFXScopes[freeSlot].dataSize = snd->chunk.alen;
    // 
    //                 SFXScopes[freeSlot].scope = SonicMania::Scope_Stage;        // keep the sound within the scope of the stage (DOOM)
    //                 SFXScopes[freeSlot].maxConcurrentPlays = 1;                 // only play one of the same sound at a time
    // 
    //                 for (int x = 0; x < snd->chunk.alen; x++) {
    //                     SFXScopes[freeSlot].dataPtr[x] = (float)(snd->chunk.abuf[x] - 128) / 128.0f;
    //                 }
    // 
    //                 FreeAllocatedSound(snd);                                    // free the expanded sound data
    //             }
    //         }
    //         W_ReleaseLumpNum(sounds[i].lumpnum);                                // release the current lump buffer
    //         printf(".");
    //     }
    // }
    // 
    // printf("\n");
}

// ----------------------------------------------------------------------------
// Playing DOOM sounds
// ----------------------------------------------------------------------------
void RE_Sound_Start(sfxinfo_t* sound, int vol, int sep) {
    // char lumpName[9];
    // GetSfxLumpName(sound, lumpName, sizeof(lumpName));             // get the lump name of the sound we're looking for
    // int slot = SonicMania::PlaySoundFXS(lumpName);
    // // if the sound was able to actually be played...
    // if (slot != -1) {
    // 	SonicMania::ChangeMusicSpeed(slot, (vol / 127.0f), (float)(sep - 128) / 128.0f, 1.0f);
    // }
}

void RE_Sound_Update(sfxinfo_t* sound, int vol, int sep) {
    // char lumpName[9];
    // GetSfxLumpName(sound, lumpName, sizeof(lumpName));             // get the lump name of the sound we're looking for
    // int slot = SonicMania::GetSoundFXID(lumpName);
    // // if the sound is playing...
    // if (slot != -1) {
    //     SonicMania::ChangeMusicSpeed(slot, (vol / 127.0f), (float)(sep - 128) / 128.0f, 1.0f);
    // }
}

boolean RE_Sound_IsPlaying(sfxinfo_t* sound) {
    //char lumpName[9];
    //GetSfxLumpName(sound, lumpName, sizeof(lumpName));             // get the lump name of the sound we're looking for
    //int slot = SonicMania::GetSoundFXID(lumpName);
    //// if the sound is playing...
    //return (slot != -1 ? true : false);
    return false;
}