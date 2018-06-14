#pragma once
#include <CompilerTraits.hpp>
#include <NBT.hpp>
#include <SHA256.hpp>
#include <AES.hpp>
#include <Version.hpp>

#define SHADENBT_ENABLE_CRYPTOSHADE

#define SHADE_MAGIC 0xAD4E4254
#define CRYPTO_MAGIC 0xEC4E4254
namespace shade{
    namespace versions{
        class current{
#       ifdef CPP_HAS_CONSTEXPR
            constexpr static const uint32_t major = 1;
            constexpr static const uint32_t minor = 1;
#       else
            const static uint32_t major;
            const static uint32_t minor;
#       endif
        };
#       ifndef CPP_HAS_CONSTEXPR
           const uint32_t current::major = 1;
           const uint32_t current::minor = 1;
#       endif  
    };
    const VersionConstants<versions::current> Current;
    class ShadeSaveFile{
    private:
        uint32_t magic;
        uint8_t flags;
        Version version;
        nbt::NBTTagCompound compound;
    public:
        ShadeSaveFile(const nbt::NBTTagCompound&);
        ShadeSaveFile(FILE*);
        ShadeSaveFile();
        Version getVersion()const;
        nbt::NBTTagCompound& getCompound();
        void setCompound(nbt::NBTTagCompound&);
        const nbt::NBTTagCompound& getCompound()const;
        bool isMalformed()const;
        static Version getCurrent();
        void write(FILE*)const;
    };
    class CryptoShadeSaveFile{
    private:
        uint32_t magic;
        uint8_t flags;
        Version ver;
        string password;
        nbt::NBTTagCompound compound;
    public:
        CryptoShadeSaveFile(const nbt::NBTTagCompound&,const string&);
        CryptoShadeSaveFile(FILE*,const string&);
        Version getVersion()const;
        nbt::NBTTagCompound& getCompound();
        void setCompound(nbt::NBTTagCompound&);
        const nbt::NBTTagCompound& getCompound()const;
        void setPassword(const string&);
        void write(FILE*);
    };
};