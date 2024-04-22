#pragma once
#include <string.h>
#include <iostream>
#include <vector>
//#include <array>
#include <algorithm>
#include <cstdint>

#ifdef PATTERNS_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#endif

#ifndef PATTERN_DETECTOR_HPP
#define PATTERN_DETECTOR_HPP

namespace Pattern
{
    struct Section
    {
        uintptr_t addr;
        size_t size;
#ifdef PATTERNS_USE_NAMES
        std::string name;
#endif // PATTERNS_USE_NAMES

        auto operator<=>(const Section& other) const {
            return addr <=> other.addr;
        }
    };

#ifdef PATTERNS_WIN32
    namespace Win32
    {
        static int GetSections(HMODULE hModule, std::vector<Section>* outSections)
        {
            // Get DOS header
            IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)hModule;

            // Check for the DOS signature
            if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
            {
                return -1;
            }

            // Get NT headers
            IMAGE_NT_HEADERS32* ntHeaders = (IMAGE_NT_HEADERS32*)((BYTE*)hModule + dosHeader->e_lfanew);

            // Check for the PE signature
            if (ntHeaders->Signature != IMAGE_NT_SIGNATURE)
            {
                return -1;
            }

            outSections->clear();

            // Get the section headers
            IMAGE_SECTION_HEADER* sectionHeader = IMAGE_FIRST_SECTION(ntHeaders);

            // Iterate through the sections
            for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; ++i)
            {
                if ((sectionHeader[i].Characteristics & IMAGE_SCN_MEM_READ) &&
                    (sectionHeader[i].Characteristics & IMAGE_SCN_MEM_EXECUTE))
                {
#ifdef PATTERNS_USE_NAMES
                    Section s = { reinterpret_cast<uintptr_t>(sectionHeader[i].VirtualAddress + (LPBYTE)hModule), sectionHeader[i].SizeOfRawData, (char*)sectionHeader[i].Name };
#else
                    Section s = { reinterpret_cast<uintptr_t>(sectionHeader[i].VirtualAddress + (LPBYTE)hModule), sectionHeader[i].SizeOfRawData };
#endif
                    outSections->push_back(s);
                }
            }

            if (!outSections->empty())
                std::sort(outSections->begin(), outSections->end());

            return 0;
        }
    }
#endif

#ifdef PATTERNS_CONSTEXPR

// Helper macro to concatenate tokens
#define CONCATENATE_DETAIL(x, y) x ## y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)

// Generate a unique variable name based on __COUNTER__
#define UNIQUE_VARIABLE_NAME(prefix) CONCATENATE(prefix, __COUNTER__)

    constexpr size_t countTokens(const char* str) {
        int count = 0;
        bool inToken = false;
        while (*str) {
            if (*str == ' ') {
                inToken = false;
            }
            else if (!inToken) {
                count++;
                inToken = true;
            }
            str++;
        }
        return count;
    }

    constexpr size_t countWildcardTokens(const char* str) {
        int count = 0;
        bool inToken = false;
        while (*str) {
            if (*str == ' ') {
                inToken = false;
            }
            else if (!inToken) {
                if (*str == '?') {
                    count++;
                    inToken = true;
                }
            }
            str++;
        }

        return count;
    }

    template<size_t N, size_t Nt = 1>
    class constPatternClass
    {
    public:
        size_t size = N;
        size_t sizeWcTokens = 0;
        static constexpr size_t sizeWcTokenArray = Nt == 0 ? 1 : Nt;
        uint8_t bytes[N] = { 0 };
        uint8_t wildcards[sizeWcTokenArray] = { 0 };

        constexpr uint8_t hexCharToUint8(char c)
        {
            if (c >= '0' && c <= '9') return static_cast<uint8_t>(c - '0');
            else if (c >= 'a' && c <= 'f') return static_cast<uint8_t>(c - 'a' + 10);
            else if (c >= 'A' && c <= 'F') return static_cast<uint8_t>(c - 'A' + 10);
            return 0;
        }

        constexpr void hexStringToArray(const char* hexString)
        {
            size_t result_index = 0;
            size_t wc_index = 0;
            for (size_t i = 0; hexString[i] != '\0' && result_index < N; i++) 
            {
                char c1 = hexString[i];
                char c2 = hexString[i + 1];

                if (c1 == '\0') break; // Check for end of string

                if (c2 == '?' || c2 == '*') 
                { 
                    // Wildcard represented as a single token
                    bytes[result_index] = 0;
                    wildcards[wc_index] = result_index;
                    //wildcards[result_index] = 1;
                    result_index++;
                    wc_index++;
                    i++; // Move to the next token
                }
                else if (c1 != ' ' && c2 != ' ') 
                { 
                    // Normal hex byte representation
                    uint8_t byte = (hexCharToUint8(c1) << 4) | hexCharToUint8(c2);
                    bytes[result_index] = byte;
                    //wildcards[result_index] = 0;
                    result_index++;
                    i++; // Move to the next token
                }
            }
        }


        constexpr constPatternClass(const char* string)
        {
            if (Nt <= 0)
                wildcards[0] = 0xFF;
            else
                sizeWcTokens = Nt;

            hexStringToArray(string);
        }
    };
#define ConstPattern(name, pattern) Pattern::constPatternClass<Pattern::countTokens(pattern), Pattern::countWildcardTokens(pattern)>name(pattern)

#else
    std::vector<std::pair<uint8_t, bool>> hexStringToVector(const std::string& hexString)
    {
        std::vector<std::pair<uint8_t, bool>> result;
        std::istringstream iss(hexString);

        std::string token;
        while (iss >> token)
        {
            if (token == "?" || token == "*")
            {
                result.push_back(std::make_pair(0, true));
            }
            else if (token.size() == 2 || token.size() == 4)
            {
                uint8_t byte = 0;
                bool wildcard = false;
                for (size_t i = 0; i < token.size(); ++i)
                {
                    char c = token[i];
                    if (c == '?' || c == '*')
                    {
                        wildcard = true;
                    }
                    else
                    {
                        byte <<= 4;
                        if (c >= '0' && c <= '9')
                            byte |= (c - '0');
                        else if (c >= 'a' && c <= 'f')
                            byte |= (c - 'a' + 10);
                        else if (c >= 'A' && c <= 'F')
                            byte |= (c - 'A' + 10);
                        else {
                            //std::cerr << "Invalid hex character: " << c << std::endl;
                            return {};
                        }
                    }
                }
                result.push_back(std::make_pair(byte, wildcard));
            }
            else
            {
                //std::cerr << "Invalid token size: " << token.size() << std::endl;
                return {};
            }
        }
        return result;
    }
#endif

    class Detector
    {
    private:
        std::vector<Section> sections;
#ifdef PATTERNS_CONSTEXPR

        // check wildcard by reference
        bool bIsWildcard(int index, const uint8_t* wildcards, size_t inWildcardsSize)
        {
            for (int i = 0; i < inWildcardsSize; i++)
            {
                if (wildcards[i] == index)
                    return true;
            }
            return false;
        }

        uintptr_t searchMemory(const uint8_t* pattern, const uint8_t* wildcards, size_t inPatternSize, size_t inWildcardsSize, const uint8_t* memory, size_t memorySize)
        {
            size_t patternSize = inPatternSize;
            for (size_t i = 0; i <= memorySize - patternSize; ++i)
            {
                bool match = true;
                for (size_t j = 0; j < patternSize; ++j)
                {
                    if (pattern[j] == 0)
                    {
                        if (!bIsWildcard(j, wildcards, inWildcardsSize))
                        {
                            // If not a wildcard, compare byte values
                            if (pattern[j] != memory[i + j])
                            {
                                match = false;
                                break;
                            }
                        }
                        // If it's a wildcard, continue to the next byte
                    }
                    else if (pattern[j] != memory[i + j])
                    {
                        match = false;
                        break;
                    }
                }
                if (match)
                {
                    return reinterpret_cast<uintptr_t>(&memory[i]);
                }
            }
            return 0;
        }
#else
		uintptr_t searchMemory(const std::vector<std::pair<uint8_t, bool>>& pattern, const uint8_t* memory, size_t memorySize) 
        {
			size_t patternSize = pattern.size();
			for (size_t i = 0; i <= memorySize - patternSize; ++i) 
            {
				bool match = true;
				for (size_t j = 0; j < patternSize; ++j) 
                {
					if (!pattern[j].second) 
                    { 
                        // If not a wildcard, compare byte values
						if (pattern[j].first != memory[i + j]) 
                        {
							match = false;
							break;
						}
					}
					// If it's a wildcard, continue to the next byte
				}
				if (match) 
                {
					return reinterpret_cast<uintptr_t>(&memory[i]);
				}
			}
			return 0;
		}
#endif // PATTERNS_CONSTEXPR
        uintptr_t searchMemory(const uint8_t* pattern, size_t inPatternSize, const uint8_t* memory, size_t memorySize)
        {
            size_t patternSize = inPatternSize;
            for (size_t i = 0; i <= memorySize - patternSize; ++i)
            {
                bool match = true;
                for (size_t j = 0; j < patternSize; ++j)
                {
                    if (pattern[j] != memory[i + j])
                    {
                        match = false;
                        break;
                    }
                }
                if (match)
                {
                    return reinterpret_cast<uintptr_t>(&memory[i]);
                }
            }
            return 0;
        }

        Section* findSection(uintptr_t addr)
        {
            for (int i = 0; i < sections.size(); i++)
            {
                uintptr_t sectionEnd = sections[i].addr + sections[i].size;

                if ((addr >= sections[i].addr) && (addr <= sectionEnd))
                    return &sections[i];
            }

            return nullptr;
        }

    public:
#ifdef PATTERNS_WIN32
        Detector()
#else
        Detector(uintptr_t base_addr, size_t size)
#endif // PATTERNS_WIN32
        {
#ifdef PATTERNS_WIN32
            Win32::GetSections(GetModuleHandle(NULL), &sections);
#else
            Section sec;
            sec.addr = base_addr;
            sec.size = size;
            sections.push_back(sec);
#endif // PATTERNS_WIN32

        }
        ~Detector()
        {

        }
#ifdef PATTERNS_CONSTEXPR
        bool GetMultiple(const uint8_t* pattern, const uint8_t* wildcards, size_t inPatternSize, size_t inWildcardsSize, size_t max_count, std::vector<uintptr_t>* out)
        {
            size_t patternSize = inPatternSize;
#else
        bool GetMultiple(const char* pattern, size_t max_count, std::vector<uintptr_t>* out)
        {
            auto pat = hexStringToVector(pattern);

            if (pat.empty())
                return;
            size_t patternSize = pat.size();
#endif // PATTERNS_CONSTEXPR
            int foundCount = 0;

            for (Section s : sections)
            {
                uintptr_t startAddr = s.addr;
                uintptr_t endAddr = s.addr + s.size;

                while (startAddr < endAddr)
                {
                    size_t size = endAddr - startAddr;

                    if (size < patternSize)
                        break;

#ifdef PATTERNS_CONSTEXPR
                    uintptr_t search = searchMemory(pattern, wildcards, patternSize, inWildcardsSize, reinterpret_cast<uint8_t*>(startAddr), size);
#else
                    uintptr_t search = searchMemory(pat, reinterpret_cast<uint8_t*>(startAddr), size);
#endif // PATTERNS_CONSTEXPR
                    
                    if (search)
                    {
                        out->push_back(search);
                        foundCount++;

                        if (foundCount >= max_count)
                            return foundCount != 0;
                        startAddr = search + 1;
                    }

                    if (foundCount == 0)
                        break;

                }
            }

            return foundCount != 0;
        }

#ifdef PATTERNS_CONSTEXPR
        uintptr_t Get(unsigned int index, const uint8_t* pattern, const uint8_t* wildcards, size_t inPatternSize, size_t inWildcardsSize, intptr_t offset = 0)
        {
            size_t patternSize = inPatternSize;
#else
        uintptr_t Get(unsigned int index, const char* pattern, intptr_t offset = 0)
        {
            auto pat = hexStringToVector(pattern);
            if (pat.empty())
                return 0;
            size_t patternSize = pat.size();
#endif // PATTERNS_CONSTEXPR

            int foundCount = 0;

            for (Section s : sections)
            {
                uintptr_t startAddr = s.addr;
                uintptr_t endAddr = s.addr + s.size;

                while (startAddr < endAddr)
                {
                    size_t size = endAddr - startAddr;
                    if (size < patternSize)
                        break;
#ifdef PATTERNS_CONSTEXPR
                    uintptr_t search = searchMemory(pattern, wildcards, patternSize, inWildcardsSize, reinterpret_cast<uint8_t*>(startAddr), size);
#else
                    uintptr_t search = searchMemory(pat, reinterpret_cast<uint8_t*>(startAddr), size);
#endif // PATTERNS_CONSTEXPR
                    if (search)
                    {
                        if (index == foundCount)
                            return search + offset;

                        foundCount++;

                        startAddr = search + 1;
                    }

                    if (foundCount == 0)
                        break;
                }
            }

#ifdef PATTERNS_DEBUG
            DebugBreak();
#endif

            return 0;
        }

#ifdef PATTERNS_CONSTEXPR
        inline uintptr_t GetFirst(const uint8_t* pattern, const uint8_t* wildcards, size_t inPatternSize, size_t inWildcardsSize, intptr_t offset = 0)
        {
#else
        inline uintptr_t GetFirst(const char* pattern, intptr_t offset = 0)
        {
#endif // PATTERNS_CONSTEXPR
            return Get(0, pattern, wildcards, inPatternSize, inWildcardsSize, offset);
        }

        // direct buffer searchers
        bool GetMultiple(const uint8_t* pattern, size_t patternSize, size_t max_count, std::vector<uintptr_t>*out)
        {
            int foundCount = 0;

            for (Section s : sections)
            {
                uintptr_t startAddr = s.addr;
                uintptr_t endAddr = s.addr + s.size;

                while (startAddr < endAddr)
                {
                    size_t size = endAddr - startAddr;

                    if (size < patternSize)
                        break;

                    uintptr_t search = searchMemory(pattern, patternSize, reinterpret_cast<uint8_t*>(startAddr), size);
                    if (search)
                    {
                        out->push_back(search);
                        foundCount++;

                        if (foundCount >= max_count)
                            return foundCount != 0;
                        startAddr = search + 1;
                    }

                    if (foundCount == 0)
                        break;

                }
            }

#ifdef PATTERNS_DEBUG
            if (foundCount == 0)
                DebugBreak();
#endif

            return foundCount != 0;
        }


        uintptr_t Get(int index, const uint8_t* pattern, size_t patternSize, intptr_t offset = 0)
        {
            int foundCount = 0;

            for (Section s : sections)
            {
                uintptr_t startAddr = s.addr;
                uintptr_t endAddr = s.addr + s.size;

                while (startAddr < endAddr)
                {
                    size_t size = endAddr - startAddr;
                    if (size < patternSize)
                        break;

                    uintptr_t search = searchMemory(pattern, patternSize, reinterpret_cast<uint8_t*>(startAddr), size);
                    if (search)
                    {
                        if (index == foundCount)
                            return search + offset;

                        foundCount++;

                        startAddr = search + 1;
                    }

                    if (foundCount == 0)
                        break;
                }
            }

            return 0;
        }

        inline uintptr_t GetFirst(const uint8_t* pattern, size_t patternSize, intptr_t offset = 0)
        {
            return Get(0, pattern, patternSize, offset);
        }
    };
}

#ifdef PATTERNS_CONSTEXPR

// 
// I have no idea how or why this works, but it does, and I shall not touch it.
// The dark arts of compiler macros shalln't be disobeyed!
//

#define _ConstPatternInlineGetFirst(instance, pattern, offset, name)\
    [](Pattern::Detector* pi) -> uintptr_t {\
        constexpr static ConstPattern(name, pattern);\
        return pi->GetFirst(name.bytes, name.wildcards, name.size, name.sizeWcTokens, offset);\
    }(instance)

#define _ConstPatternInlineGetMultiple(instance, pattern, max_count, out_pvector, name)\
    [](Pattern::Detector* pi, std::vector<uintptr_t>* outVec) -> bool {\
        constexpr static ConstPattern(name, pattern);\
        return pi->GetMultiple(name.bytes, name.wildcards, name.size, name.sizeWcTokens, max_count, outVec);\
    }(instance, out_pvector)

#define _ConstPatternInlineGet(instance, index, pattern, offset, name)\
    [](Pattern::Detector* pi) -> uintptr_t {\
        constexpr static ConstPattern(name, pattern);\
        return pi->Get(index, name.bytes, name.wildcards, name.size, name.sizeWcTokens, offset);\
    }(instance)

#define ConstPatternInlineGetFirst(instance, pattern, offset) _ConstPatternInlineGetFirst(instance, pattern, offset, UNIQUE_VARIABLE_NAME(constpat))
#define ConstPatternInlineGetMultiple(instance, pattern, max_count, out_pvector) _ConstPatternInlineGetMultiple(instance, pattern, max_count, out_pvector, UNIQUE_VARIABLE_NAME(constpat))
#define ConstPatternInlineGet(instance, index, pattern, offset) _ConstPatternInlineGet(instance, index, pattern, offset, UNIQUE_VARIABLE_NAME(constpat))

#define ConstPatternGetFirst(instance, constpattern, offset) instance->GetFirst(constpattern.bytes, constpattern.wildcards, constpattern.size, offset);
#define ConstPatternGetMultiple(instance, constpattern, max_count, out_pvector) instance->GetMultiple(constpattern.bytes, constpattern.wildcards, constpattern.size, max_count, out_pvector);
#define ConstPatternGet(instance, index, constpattern, offset) instance->Get(index, constpattern.bytes, constpattern.wildcards, constpattern.size, offset);

#endif // PATTERNS_CONSTEXPR

#endif // PATTERN_DETECTOR_HPP