#include "framework.h"
#include <filesystem>
#include <string>


namespace ModPath
{
    template<class T>
    inline T GetModulePath(HMODULE hModule)
    {
        static constexpr auto INITIAL_BUFFER_SIZE = MAX_PATH;
        static constexpr auto MAX_ITERATIONS = 7;

        if constexpr (std::is_same_v<T, std::filesystem::path>)
        {
            std::u16string ret;
            std::filesystem::path pathret;
            auto bufferSize = INITIAL_BUFFER_SIZE;
            for (size_t iterations = 0; iterations < MAX_ITERATIONS; ++iterations)
            {
                ret.resize(bufferSize);
                size_t charsReturned = 0;
                charsReturned = GetModuleFileNameW(hModule, (LPWSTR)&ret[0], bufferSize);
                if (charsReturned < ret.length())
                {
                    ret.resize(charsReturned);
                    pathret = ret;
                    return pathret;
                }
                else
                {
                    bufferSize *= 2;
                }
            }
        }
        else
        {
            T ret;
            auto bufferSize = INITIAL_BUFFER_SIZE;
            for (size_t iterations = 0; iterations < MAX_ITERATIONS; ++iterations)
            {
                ret.resize(bufferSize);
                size_t charsReturned = 0;
                if constexpr (std::is_same_v<T, std::string>)
                    charsReturned = GetModuleFileNameA(hModule, &ret[0], bufferSize);
                else
                    charsReturned = GetModuleFileNameW(hModule, &ret[0], bufferSize);
                if (charsReturned < ret.length())
                {
                    ret.resize(charsReturned);
                    return ret;
                }
                else
                {
                    bufferSize *= 2;
                }
            }
        }
        return T();
    }

    template<class T>
    inline T GetThisModulePath()
    {
        HMODULE hm = NULL;
        GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&DllMain, &hm);
        T r = GetModulePath<T>(hm);
        if constexpr (std::is_same_v<T, std::filesystem::path>)
            return r;
        else if constexpr (std::is_same_v<T, std::string>)
            r = r.substr(0, r.find_last_of("/\\") + 1);
        else
            r = r.substr(0, r.find_last_of(L"/\\") + 1);
        return r;
    }

    template<class T>
    inline T GetThisModuleName()
    {
        HMODULE hm = NULL;
        GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&DllMain, &hm);
        const T moduleFileName = GetModulePath<T>(hm);

        if constexpr (std::is_same_v<T, std::filesystem::path>)
            return moduleFileName.filename();
        else if constexpr (std::is_same_v<T, std::string>)
            return moduleFileName.substr(moduleFileName.find_last_of("/\\") + 1);
        else
            return moduleFileName.substr(moduleFileName.find_last_of(L"/\\") + 1);
    }
}
