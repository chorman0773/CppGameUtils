#ifndef __NBT_hpp_2018_06_12_11_14
#define __NBT_hpp_2018_06_12_11_14

#include <CompilerTraits.hpp>

#include <type_traits>
#include <SFML/System/InputStream.hpp>
#include <cstdint>
#include <string>
#include <cstdio>
#include <array>
#include <utility>
#include <vector>
#include <map>
#include <UUID.hpp>
#include <tuple>


using std::string;
using std::vector;
using std::map;

#define TAG_END 0
#define TAG_BYTE 1
#define TAG_SHORT 2
#define TAG_INT 3
#define TAG_LONG 4
#define TAG_FLOAT 5
#define TAG_DOUBLE 6
#define TAG_BYTEARRAY 7
#define TAG_STRING 8
#define TAG_LIST 9
#define TAG_COMPOUND 10
#define TAG_INTARRAY 11
#define TAG_LONGARRAY 12
#define TAG_ANY_NUMERIC 99

namespace nbt{
    class NBTTag;//Forward Declare
    class NBTInputStream{
    private:
        bool isMemory;
        uint32_t size;
        const uint8_t* memory;
        FILE* f;
        bool isLEByteOrder;
    public:
        explicit NBTInputStream(const char*);
        explicit NBTInputStream(const uint8_t*);
        NBTInputStream(const NBTInputStream&);
        NBTInputStream(NBTInputStream&&);
        NBTInputStream& operator=(const NBTInputStream&);
        NBTInputStream& operator=(NBTInputStream&&);
        ~NBTInputStream();
        int8_t readByte();
        uint8_t readUnsignedByte();
        int16_t readShort();
        int32_t readInt();
        int64_t readLong();
        float readFloat();
        double readDouble();
        string readString();
        NBTInputStream& operator>>(int8_t&);
        NBTInputStream& operator>>(int16_t&);
        NBTInputStream& operator>>(int32_t&);
        NBTInputStream& operator>>(int64_t&);
        NBTInputStream& operator>>(float&);
        NBTInputStream& operator>>(double&);
        NBTInputStream& operator>>(string&);
        void setReadingByteOrder(bool);
    };

    struct use_memory_t{};
    const use_memory_t use_memory;

    class NBTOutputStream{
    private:
        FILE* file;
        uint8_t* memory;
        size_t pos;
        size_t size;
        void reserve(size_t);
    public:
        explicit NBTOutputStream(const char*);
        explicit NBTOutputStream(use_memory_t);
        NBTOutputStream(const NBTOutputStream&);
        NBTOutputStream(NBTOutputStream&&);
        NBTOutputStream& operator=(const NBTOutputStream&);
        NBTOutputStream& operator=(NBTOutputStream&&);
        ~NBTOutputStream();
        void writeByte(int8_t);
        void writeUnsignedByte(uint8_t);
        void writeShort(int16_t);
        void writeInt(int32_t);
        void writeLong(int64_t);
        void writeFloat(float);
        void writeDouble(double);
        void writeString(const string&);
        NBTOutputStream& operator<<(int8_t);
        NBTOutputStream& operator<<(int16_t);
        NBTOutputStream& operator<<(int32_t);
        NBTOutputStream& operator<<(int64_t);
        NBTOutputStream& operator<<(float);
        NBTOutputStream& operator<<(double);
        NBTOutputStream& operator<<(const string&);
    };
    class NBTTag{
    public:
        virtual ~NBTTag();
        virtual uint8_t getTagType()const=0;
        virtual void write(NBTOutputStream&)const=0;
        virtual void read(NBTInputStream&)=0;
    };
    class NBTPrimitive:public NBTTag{
    public:
        virtual ~NBTPrimitive();
        virtual int getInteger()const=0;
        virtual float getFloat()const;
        virtual double getDouble()const;
        virtual int64_t getLong()const;
        int8_t getByte()const;
        int16_t getShort()const;
        bool getBoolean()const;
        operator int()const;
        operator float()const;
        operator double()const;
        operator int64_t()const;
        operator int8_t()const;
        operator int16_t()const;
        explicit operator bool()const;
    };

    namespace detail{
        template<typename T,uint8_t tag> class NBTTagPrimitive:public NBTPrimitive{
        private:
            typedef T type;
            type val;
        public:
            NBTTagPrimitive(type val):val(val){}
            NBTTagPrimitive(){}
            template<typename U,typename=typename std::enable_if<std::is_scalar<U>::value&&(!std::is_void<U>::value)>::type> 
                NBTTagPrimitive(U u):val(u){}
            template<typename U,uint8_t tag> NBTTagPrimitive(const NBTTagPrimitive<U,tag>& o):val(o.val){}
            template<typename U,uint8_t tag> NBTTagPrimitive(NBTTagPrimitive<U,tag>&& o):val(o.val){}
            NBTTagPrimitive& operator=(type val){
                this->val = val;
                return *this;
            }
            template<typename U,typename=typename std::enable_if<std::is_scalar<U>::value&&(!std::is_void<U>::value)>::type>
             NBTTagPrimitive& operator=(U val){
                 this->val = val;
                 return *this;
            }
            template<typename U,uint8_t tag> NBTTagPrimitive& operator=(const NBTTagPrimitive<U,tag>& o){
                val = o.val;
                return *this;
            }
            template<typename U,uint8_t tag> NBTTagPrimitive& operator=(NBTTagPrimitive<U,tag>&& o){
                val = o.val;
                return *this;
            }
            int getInteger()const{
                return int(val);
            }
            float getFloat()const{
                return float(val);
            }
            double getDouble()const{
                return double(val);
            }
            int64_t getLong()const{
                return int64_t(val);
            }
            uint8_t getTagType()const{
                return tag;
            }
            void write(NBTOutputStream& o)const{
                o << val;
            }
            void read(NBTInputStream& i){
                i >> val;
            }
        };
        using std::declval;
        template<typename T,uint8_t tag> class NBTArray:public NBTTag{
        private:
            typedef T type;
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef T* iterator;
            typedef const T* const_iterator;
            typedef size_t size_type;
            size_t _size;
            T* underlying;
        public:
            NBTArray():underlying(nullptr),_size(0){}
            NBTArray(size_t size):_size(size),underlying(size==0?nullptr:new T[size]){}
            template<size_t N> NBTArray(const T(&arr)[N]):_size(N),underlying(new T[N]){
                for(size_t i =0;i<N;i++)
                    underlying[i] = arr[i];

            }
            template<size_t N> NBTArray(T(&&arr)[N]):_size(N),underlying(new T[N]){
                for(size_t i =0;i<N;i++)
                    underlying[i] = arr[i];
            }
            template<size_t N> NBTArray(const std::array<T,N>& arr):_size(N),underlying(new T[N]){
                for(size_t i=0;i<N;i++)
                    underlying[i] = arr[i];
            }
            template<size_t N> NBTArray(std::array<T,N>&& arr):_size(N),underlying(new T[N]){
                for(size_t i=0;i<N;i++)
                    underlying[i] = arr[i];
            }
            NBTArray(const NBTArray& arr):_size(arr._size),underlying(new T[arr.size]){
                for(size_t i=0;i<_size;i++)
                    underlying[i] = arr[i];
            }
            NBTArray(NBTArray&& arr):_size(arr._size),underlying(std::exchange(arr.underlying,nullptr)){}
            ~NBTArray(){
                if(arr!=nullptr)
                    delete[] arr;
            }

            NBTArray& operator=(const NBTArray& arr){
                if(&arr==this)
                    return *this;
                if(underlying!=nullptr)
                    delete[] underlying;
                _size = arr._size;
                underlying = new T[_size];
                for(size_t i=0;i<_size;i++)
                    underlying[i] = arr[i];
                return *this;
            }
            NBTArray& operator=(NBTArray&& arr){
                _size = arr._size;
                if(underlying!=nullptr)
                    delete[] underlying;
                underlying = std::exchange(arr.underlying,nullptr);
                return *this;
            }
            reference operator[](size_type o){
                return underlying[o];
            }
            const_reference operator[](size_type o)const{
                return underlying[o];
            }
            iterator begin(){
                return underlying;
            }
            const_iterator begin()const{
                return underlying;
            }
            iterator end(){
                return underlying+size;
            }
            const_iterator end()const{
                return underlying+size;
            }
            pointer data(){
                return underlying
            }
            const_pointer data()const{
                return underlying;
            }
            size_type size()const{
                return _size;
            }
            size_type length()const{
                return _size;
            }
            uint8_t getTagType()const{
                return tag;
            }
            void write(NBTOutputStream& o)const{
                o << size;
                for(type t:*this)
                    o << t;
            }
            void read(NBTInputStream& i){
                if(underlying!=nullptr)
                    delete[] underlying;
                i >> _size;
                underlying = new T[_size];
            }
        };
    };
    typedef detail::NBTTagPrimitive<int8_t,1> NBTTagByte;
    typedef detail::NBTTagPrimitive<int16_t,2> NBTTagShort;
    typedef detail::NBTTagPrimitive<int32_t,3> NBTTagInt;
    typedef detail::NBTTagPrimitive<int64_t,4> NBTTagLong;
    typedef detail::NBTTagPrimitive<float,5> NBTTagFloat;
    typedef detail::NBTTagPrimitive<double,6> NBTTagDouble;
    typedef detail::NBTArray<int8_t,7> NBTTagByteArray;
    typedef detail::NBTArray<int32_t,11> NBTTagIntArray;
    typedef detail::NBTArray<int64_t,12> NBTTagLongArray;
    class NBTTagEnd:public NBTTag{
    public:
        NBTTagEnd();
        void read(NBTInputStream&);
        void write(NBTOutputStream&)const;
        uint8_t getTagType()const;
    };
    class NBTTagString:public NBTTag{
    private:
        string str;
    public:
        NBTTagString();
        NBTTagString(const string&);
        NBTTagString(string&&);
        const string& getString()const;
    };

    namespace erasure{
        struct ErasedNBTTag{
        private:
            uint8_t tagType;
            NBTTag* tag;
        public:
            ErasedNBTTag();
            template<typename T,typename=typename std::enable_if<std::is_base_of<NBTTag,T>::value>::type> 
                ErasedNBTTag(std::in_place_type_t<T>,const NBTTag& tag){
                    tag = new T(dynamic_cast<const T&>(tag));
                }
            template<typename T,typename=typename std::enable_if<std::is_base_of<NBTTag,T>::value>::type> 
                ErasedNBTTag(std::in_place_type_t<T>,NBTTag&& tag){
                    tag = new T(std::move(dynamic_cast<T&>(tag)));
                }
            ErasedNBTTag(uint8_t,NBTInputStream&);

            template<typename T,typename=typename std::enable_if<std::is_base_of<NBTTag,T>::value>::type>
                ErasedNBTTag(const T& t){
                    tag = new T(t);
                    tagType = tag->getTagType();
                    shouldDelete = true;
                }
            template<typename T,typename=typename std::enable_if<std::is_base_of<NBTTag,T>::value>::type>
                ErasedNBTTag(T&& t){
                    tag = new T(std::move(t));
                    tagType = tag->getTagType();
                    shouldDelete = true;
                }
            ErasedNBTTag(const ErasedNBTTag&);
            ErasedNBTTag(ErasedNBTTag&&);
            ~ErasedNBTTag();
            operator NBTTag&();
            operator const NBTTag&()const;
            template<typename T> operator typename std::enable_if<std::is_base_of<NBTTag,T>::value,T>::type&(){
                return dynamic_cast<T&>(*tag);
            }
            template<typename T> operator typename std::enable_if<std::is_base_of<NBTTag,T>::value,const T>::type&()const{
                return dynamic_cast<const T&>(*tag);
            }
            void write(NBTOutputStream&)const;
            ErasedNBTTag& operator=(NBTTag&);
            uint8_t getTagType()const;
            template<typename T,typename=typename std::enable_if<std::is_base_of<NBTTag,T>::value>::type>
                ErasedNBTTag& operator=(const T& t){
                    if(shouldDelete)
                        delete tag;
                    tag = new T(t);
                    tagType = t.getTagType();
                    shouldDelete = true;
                }
            template<typename T,typename=typename std::enable_if<std::is_base_of<NBTTag,T>::value>::type>
                ErasedNBTTag& operator=(T&& t){
                    if(shouldDelete)
                        delete tag;
                    tag = new T(std::move(t));
                    tagType = t.getTagType();
                    shouldDelete = true;
                }
        };
    };
    class NBTTagList:public NBTTag{
    private:
        typedef erasure::ErasedNBTTag type;
        typedef type& reference;
        typedef const type& const_reference;
        typedef type* pointer;
        typedef const type* const_pointer;
        typedef vector<type>::iterator iterator;
        typedef vector<type>::const_iterator const_iterator;
        typedef NBTTag tag;
        typedef NBTTag& tag_reference;
        typedef const NBTTag& const_tag_reference;
        uint8_t tagType;
        vector<type> underlying;
    public:
        NBTTagList();
        bool add(tag_reference);
        bool set(size_t,tag_reference);
        void remove(size_t);
        const_reference operator[](size_t)const;
        size_t size()const;
        uint8_t getListTagtype()const;
        void clear();
        bool empty()const;
        uint8_t getTagType()const;
        void write(NBTOutputStream&)const;
        void read(NBTInputStream&);
    };
    class NBTTagCompound final:public NBTTag{
    private:
        map<string,erasure::ErasedNBTTag> underlying;
        typedef erasure::ErasedNBTTag erased_type;
        typedef erased_type& erased_reference;
        typedef const erased_type& const_erased_reference;
        typedef erased_type&& erased_rvalue;
        typedef string key_type;
        typedef NBTTag value_type;
        typedef const string& key_reference;
        typedef NBTTag& reference;
        typedef const NBTTag& const_reference;
        typedef map<string,erasure::ErasedNBTTag> map_type;
        typedef map_type::iterator iterator;
        typedef map_type::const_iterator const_iterator;
    public:
        NBTTagCompound();
        erased_reference operator[](const string&);
        const_erased_reference operator[](const string&)const;
        bool hasTag(const string&);
        bool hasTag(const string&,uint8_t);
        int8_t getByte(const string&)const;
        int16_t getShort(const string&)const;
        int32_t getInt(const string&)const;
        int64_t getLong(const string&)const;
        float getFloat(const string&)const;
        double getDouble(const string&)const;
        bool getBoolean(const string&)const;
        NBTTagList& getTagList(const string&);
        NBTTagCompound& getTagCompound(const string&);
        UUID getUUID(const string&)const;
        void setByte(const string&,int8_t);
        void setShort(const string&,int16_t);
        void setInt(const string&,int32_t);
        void setLong(const string&,int64_t);
        void setFloat(const string&,float);
        void setDouble(const string&,double);
        void setBoolean(const string&,bool);
        void setTagList(const string&,const NBTTagList&);
        void setTagCompound(const string&,const NBTTagCompound&);
        void setUUID(const string&,const UUID&);
        uint8_t getTagType()const;
        void read(NBTInputStream&);
        void write(NBTOutputStream&)const;
    };
};

#endif