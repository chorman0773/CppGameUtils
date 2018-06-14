#ifndef __PkmComAPL_hpp_2018_06_01_11_27
#define __PkmComAPL_hpp_2018_06_01_11_27

#define PKMCOM_CONSTEXPR constexpr

#ifdef _MSC_VER
#if _MSC_VER < 1800
#define PKMCOM_CONSTEXPR inline
#define PKMCOM_CONSTEXPR_DISABLED
#endif
#endif


#include <Version.hpp>
#include <UUID.hpp>
#include <JTime.hpp>

#include <text/TextComponent.hpp>

#include <type_traits>

#include <json/json.h>
#include <string>
#include <stdexcept>

template<typename T,typename U> PKMCOM_CONSTEXPR T constexpr_reinterpret(U&& val){
    union{
        U val;
        T target;
    }u = {val};
    return u.target;
}

namespace pkmcom{
  typedef bool pkm_boolean;
  typedef unsigned char pkm_byte;
  typedef char pkm_sbyte;
  typedef short pkm_short;
  typedef unsigned short pkm_ushort;
  typedef int pkm_int;
  typedef unsigned int pkm_uint;
  typedef long long pkm_long;
  typedef float pkm_float;
  typedef double pkm_double;
  typedef std::string pkm_string;
  typedef Json::Value pkm_json;
  typedef Instant pkm_instant;
  typedef Duration pkm_duration;
  typedef UUID pkm_uuid;
  typedef Version pkm_version;


  class ProtocolError{
  public:
    virtual pkm_json getErrorTextComponent()=0;
    virtual ~ProtocolError()=default;
  };
  class BadEnumValue final:public ProtocolError{
  public:
    ~BadEnumValue()=default;
    pkm_json getErrorTextComponent();
  };

  class InvalidPacket final:public ProtocolError{
  public:
    ~InvalidPacket()=default;
    pkm_json getErrorTextComponent();
  };
  class MalformedPacket final:public ProtocolError{
  private:
    pkm_json msg;
  public:
    ~MalformedPacket()=default;
    MalformedPacket(const pkm_json&);
    MalformedPacket(const std::string&);
    MalformedPacket(const text::TextComponent&);
    pkm_json getErrorTextComponent();
  };

  template<typename T> struct pkm_traits{

  };

  template<> struct pkm_traits<pkm_boolean>{
      typedef pkm_boolean& read_type;
      typedef pkm_boolean write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(pkm_boolean b){
          return b?1337:1331;
      }
      PKMCOM_CONSTEXPR static pkm_uint size(pkm_boolean b){
          return 1;
      }
  };

  template<> struct pkm_traits<pkm_byte>{
      typedef pkm_byte& read_type;
      typedef pkm_byte write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(pkm_byte b){
          return int(b)&0xff;
      }
      PKMCOM_CONSTEXPR static pkm_uint size(pkm_byte b){
          return 1;
      }

  };

  template<> struct pkm_traits<pkm_sbyte>{
      typedef pkm_sbyte& read_type;
      typedef pkm_sbyte write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(pkm_sbyte b){
          return int(b)&0xff|((b&0x80==0)?0:0xffffff00);
      }
      PKMCOM_CONSTEXPR static pkm_uint size(pkm_sbyte b){
          return 1;
      }

  };

  template<> struct pkm_traits<pkm_short>{
      typedef pkm_short& read_type;
      typedef pkm_short write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(pkm_short b){
          return int(b)&0xffff|((b&0x8000==0)?0:0xffff0000);
      }
      PKMCOM_CONSTEXPR static pkm_uint size(pkm_short b){
          return 2;
      }

  };

  template<> struct pkm_traits<pkm_ushort>{
      typedef pkm_ushort& read_type;
      typedef pkm_ushort write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(pkm_ushort b){
          return int(b)&0xffff;
      }
      PKMCOM_CONSTEXPR static pkm_uint size(pkm_ushort b){
          return 2;
      }

  };

  template<> struct pkm_traits<pkm_int>{
      typedef pkm_int& read_type;
      typedef pkm_int write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(pkm_int i){
          return i;
      }
      PKMCOM_CONSTEXPR static pkm_uint size(pkm_int b){
          return 4;
      }

  };

  template<> struct pkm_traits<pkm_uint>{
      typedef pkm_int& read_type;
      typedef pkm_int write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(pkm_uint i){
          return int(i);
      }
      PKMCOM_CONSTEXPR static pkm_uint size(pkm_uint b){
          return 4;
      }
  };

  template<> struct pkm_traits<pkm_long>{
      typedef pkm_long& read_type;
      typedef pkm_long write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(pkm_long l){
          return (l>>32LL)^int(l);
      }
      PKMCOM_CONSTEXPR static pkm_uint size(pkm_long l){
          return 8;
      }

  };

  template<> struct pkm_traits<pkm_string>{
      typedef pkm_string& read_type;
      typedef const pkm_string& write_type;
      inline static pkm_int hashcode(const pkm_string& str){
          int32_t hash = 0;
          const int32_t prime = 31;
          for(char c:str){
              hash *= prime;
              hash += pkm_traits<pkm_sbyte>::hashcode(c);
          }
          return hash;
      }
      inline static pkm_uint size(const pkm_string& str){
          return 2+str.size();
      }
  };

  template<> struct pkm_traits<pkm_json>{
      typedef pkm_json& read_type;
      typedef const pkm_json& write_type;
      inline static pkm_int hashcode(const pkm_json& val){
          pkm_string str = val.asString();
          return pkm_traits<pkm_string>::hashcode(str);
      }
      inline static pkm_uint size(const pkm_json& val){
          std::string str = val.asString();
          return 2+str.length();
      }
  };

  template<> struct pkm_traits<pkm_instant>{
      typedef pkm_instant& read_type;
      typedef const pkm_instant& write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(const pkm_instant& i){
          pkm_long seconds = i.toEpochSecond();
          pkm_int nanos = i.getNanos();
          return pkm_traits<pkm_long>::hashcode(seconds)*31+pkm_traits<pkm_int>::hashcode(nanos);
      }
      PKMCOM_CONSTEXPR static pkm_uint size(const pkm_instant& i){
          return 12;
      }
  };
  template<> struct pkm_traits<pkm_duration>{
      typedef pkm_duration& read_type;
      typedef const pkm_duration& write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(const pkm_duration& i){
          pkm_long seconds = i.getSeconds();
          pkm_int nanos = i.getNanos();
          return pkm_traits<pkm_long>::hashcode(seconds)*31+pkm_traits<pkm_int>::hashcode(nanos);
      }
      PKMCOM_CONSTEXPR static pkm_uint size(const pkm_duration& i){
          return 12;
      }
  };

  template<typename E,typename=typename std::enable_if<std::is_enum<E>::value>::type> 
  struct pkm_traits<E>{
      typedef typename std::underlying_type<E>::type underlying_type;
      typedef pkm_traits<underlying_type> underlying_traits;
      typedef E& read_type;
      typedef E write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(E e){
          union{
              E e1;
              underlying_type underlying;
          } u = {e};
          return underlying_traits::hashcode(u.underlying);
      }
      PKMCOM_CONSTEXPR static pkm_uint size(E e){
          union{
              E e1;
              underlying_type underlying;
          }u = {e};
          return underlying_traits::size(u.underlying);
      } 
  };

  template<> struct pkm_traits<pkm_uuid>{
      typedef pkm_uuid& read_type;
      typedef const pkm_uuid& write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(const pkm_uuid& u){
          return pkm_traits<pkm_long>::hashcode(u.getHigh())*31+pkm_traits<pkm_long>::hashcode(u.getLow());
      }
      PKMCOM_CONSTEXPR static pkm_uint size(const pkm_uuid& i){
          return 16;
      }
  };

  template<> struct pkm_traits<pkm_version>{
      typedef pkm_version& read_type;
      typedef pkm_version write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(pkm_version v){
          return v.getMajor()*31+v.getMinor();
      }
      PKMCOM_CONSTEXPR static pkm_uint size(const pkm_version v){
          return 2;
      }
  };

  template<> struct pkm_traits<pkm_float>{
      typedef pkm_float& read_type;
      typedef pkm_float write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(pkm_float f){
          union{
              pkm_float f1 = f1;
              pkm_int bits;
          };
          
          return bits;
      }
      PKMCOM_CONSTEXPR static pkm_uint size(const pkm_duration& i){
          return 4;
      }
  };

  template<> struct pkm_traits<pkm_double>{
      typedef pkm_double& read_type;
      typedef pkm_double write_type;
      PKMCOM_CONSTEXPR static pkm_int hashcode(pkm_double f){
          union{
              pkm_double f1;
              pkm_long bits;
          }u = {f};
          return pkm_traits<pkm_long>::hashcode(u.bits);
      }
      PKMCOM_CONSTEXPR static pkm_uint size(pkm_double d){
          return 8;
      }
  };

  enum class pkm_status : pkm_byte{
          None=0, Poison=1, Toxic=2,Paralysis=3,
          Burn=4, Sleep=5, Freeze=6
  };
  struct pkm_longstring{
  public:
      std::string underlying;
      pkm_longstring(const std::string&);
      pkm_longstring(std::string&&);
      pkm_longstring()=default;
  }; 

  template<> struct pkm_traits<pkm_longstring>{
      typedef pkm_longstring& read_type;
      typedef const pkm_longstring& write_type;
      inline static pkm_int hashcode(const pkm_longstring& str){
          pkm_int hash = 0;
          const pkm_int prime = 31;
          for(char c:str.underlying){
              hash *= prime;
              hash += pkm_traits<pkm_sbyte>::hashcode(c);
          }
          return hash;
      }
      inline static pkm_uint size(const pkm_longstring& str){
          return 4+str.underlying.length();
      }
  };

  struct pkm_longjson{
  public:
      pkm_json underlying;
      pkm_longjson(const pkm_json&);
      pkm_longjson(pkm_json&&);
      pkm_longjson()=default;
  };


  template<> struct pkm_traits<pkm_longjson>{
      typedef pkm_longjson& read_type;
      typedef const pkm_longjson& write_type;
      inline static pkm_int hashcode(const pkm_longjson& val){
          pkm_string str = val.underlying.asString();
          return pkm_traits<pkm_string>::hashcode(str);
      }

      inline static pkm_uint size(const pkm_longjson& val){
          pkm_string str = val.underlying.asString();
          return 4+str.size();
      }

  };

  struct PokemonData{
      pkm_byte id;
      pkm_string species;
      pkm_byte pallateInfo;
      pkm_ushort form;
      pkm_float hp;
      pkm_status status;
      pkm_json name;
      pkm_byte cryVolume;
      pkm_byte cryPitch;
      pkm_uint speciesBytes;
      pkm_byte level;
  };

  template<> struct pkm_traits<PokemonData>{
      typedef PokemonData& read_type;
      typedef const PokemonData& write_type;
      inline const pkm_int hashcode(const PokemonData& pkmdata){
          pkm_int hash = 0;
          const pkm_int prime = 31;
          hash += pkm_traits<pkm_byte>::hashcode(pkmdata.id);
          hash *= 31;
          hash += pkm_traits<pkm_string>::hashcode(pkmdata.species);
          hash *= 31;
          hash += pkm_traits<pkm_byte>::hashcode(pkmdata.pallateInfo);
          hash *= 31;
          hash += pkm_traits<pkm_ushort>::hashcode(pkmdata.form);
          hash *= 31;
          hash += pkm_traits<pkm_float>::hashcode(pkmdata.hp);
          hash *= 31;
          hash += pkm_traits<pkm_status>::hashcode(pkmdata.status);
          hash *= 31;
          hash += pkm_traits<pkm_json>::hashcode(pkmdata.name);
          hash *= 31;
          hash += pkm_traits<pkm_byte>::hashcode(pkmdata.cryVolume);
          hash *= 31;
          hash += pkm_traits<pkm_byte>::hashcode(pkmdata.cryPitch);
          hash *= 31;
          hash += pkm_traits<pkm_uint>::hashcode(pkmdata.speciesBytes);
          hash *= 31;
          hash += pkm_traits<pkm_byte>::hashcode(pkmdata.level);
          return hash;
      }
      inline const pkm_uint size(const PokemonData& pkmdata){
          return 14+pkm_traits<pkm_string>::size(pkmdata.species)+pkm_traits<pkm_json>::size(pkmdata.name);
      }
  };

  class PacketBuffer{
  private:
      pkm_byte* bytes;
      size_t pos;
      size_t capacity;
      void resize(size_t);
  public:
      PacketBuffer();
      PacketBuffer(pkm_byte*,size_t);
      PacketBuffer& operator<<(pkm_traits<pkm_boolean>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_byte>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_sbyte>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_short>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_ushort>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_int>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_uint>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_long>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_string>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_json>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_uuid>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_version>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_instant>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_duration>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_longstring>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_longjson>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_float>::write_type);
      PacketBuffer& operator<<(pkm_traits<pkm_double>::write_type);
      PacketBuffer& operator<<(pkm_traits<PokemonData>::write_type);
      PacketBuffer& operator<<(const PacketBuffer&);
      template<typename E,typename UType=typename std::underlying_type<E>::type> PacketBuffer& operator<<(pkm_traits<E>::write_type e){
          return *this << static_cast<UType>(e);
      }
      PacketBuffer& operator>>(pkm_traits<pkm_boolean>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_byte>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_sbyte>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_short>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_ushort>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_int>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_uint>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_long>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_string>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_json>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_uuid>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_instant>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_duration>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_longstring>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_longjson>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_float>::read_type);
      PacketBuffer& operator>>(pkm_traits<pkm_double>::read_type);
      PacketBuffer& operator>>(pkm_traits<PokemonData>::read_type);
      template<typename E,typename UType=typename std::underlying_type<E>::type> PacketBuffer& operator>>(pkm_traits<E>::read_type e){
          return *this >> reinterpret_cast<UType&>(e);
      }
  };

  template<typename T,typename void_type=void> struct is_packet:std::false_type{};
  template<typename T,typename void_type=typename std::enable_if<std::is_same<decltype(T::id),pkm_byte>::value>::type,
  typename=decltype(std::declval<T>().read(declval<PacketBuffer&>())),
  typename=decltype(std::declval<const T>().write(std::declval<PacketBuffer&>())),
  typename=decltype(std::declval<const T>().hashcode()),typename=decltype(std::declval<const T>().size()>> 
      struct is_packet<T,void_type>:true_type{};

  template<typename T,typename=typename std::enable_if<is_packet<T>::value>::type> struct pkm_traits{
        typedef const T& write_type;
        typedef T& read_type;
        inline static pkm_int hashcode(const T& packet){
          return packet.hashcode();
        }
        inline static pkm_uint size(const T& packet){
          return packet.size();
        }
  };

  template<typename T,typename=typename std::enable_if<is_packet<T>::value>::type> PacketBuffer& operator>>(PacketBuffer& buff,pkm_traits<T>::read_type packet){
          packet.read(buff);
          return buff;
      }
 template<typename T,typename void_type=typename std::enable_if<is_packet<T>::value>::type>
      PacketBuffer& operator<<(PacketBuffer& buff,pkm_traits<T>::write_type packet){
          packet.write(buff);
          return buff;
      }

  class PacketDescriptor{
  private:
    pkm_byte id;
    pkm_int hash;
    pkm_uint sz;
    PacketBuffer buff;
  public:
    PacketDescriptor();
    PacketDescriptor(PacketBuffer&);
    template<class T,typename=typename std::enable_if<is_packet<T>::value>::type> PacketDescriptor(const T& t){
      id = T::id;
      hash = t.hashcode();
      sz = t.size();
      buf << t;
    }
    pkm_int hashcode()const;
    pkm_uint size()const;
    pkm_byte getId()const;
    const PacketBuffer& getBuffer()const;
    template<class T,typename=typename std::enable_if<is_packet<T>::value>::type> T& validate(T& out){
      if(id!=T::id)
        throw "Invalid packet id";
      else{
        out.read(buff);
        if(out.hashcode()!=hash)
          throw MalformedPacket(text::translate("net.protocolerror.hashmissmatch"));
        return out;
      }
    }
    template<class T> operator T&&()const{
        T val;
        return std::move(validate(val));
    }
  };
  template<> struct pkm_traits<PacketDescriptor>{
    typedef const PacketDescriptor& write_type;
    typedef PacketDescriptor& read_type;
    inline static pkm_int hashcode(const PacketDescriptor& desc){
     return desc.hashcode(); 
    }
    inline static pkm_uint size(const PacketDescriptor& desc){
     return desc.size();
    }
  };
  PacketBuffer& operator<<(PacketBuffer& buff,const PacketDescriptor& desc){
    return buff << desc.getId() << desc.hashcode() << desc.size() << desc.getBuffer();
  }
  PacketBuffer& operator>>(PacketBuffer& buff,PacketDescriptor& desc){
    desc = PacketDescriptor(buff);
    return buff;
  }


  class CPacketConnectServer{
  private:
      pkm_json account;
      pkm_version ver;
      pkm_json player;
  public:
      PKMCOM_CONSTEXPR static pkm_byte id = 0;
      pkm_int hashcode()const;
      pkm_uint size()const;
      void read(PacketBuffer&);
      void write(PacketBuffer&)const;
      CPacketConnectServer(pkm_json,pkm_version,pkm_json);
      CPacketConnectServer();
      pkm_version getClientVersion()const;
      const pkm_json& getPlayerObject()const;
      const pkm_json& getSentryAccount()const;
  };

  class SPacketAcceptConnection{
  private:
      pkm_string base_channel;
      pkm_int numChannels;
      pkm_string* channels;
      pkm_long connectionId;
      pkm_long permissions;
  public:
      PKMCOM_CONSTEXPR static pkm_byte id = 1;
      pkm_int hashcode()const;
      pkm_uint size()const;
      void read(PacketBuffer&);
      void write(PacketBuffer&)const;
      SPacketAcceptConnection(const pkm_string&,pkm_int,pkm_string*,pkm_long,pkm_long);
      SPacketAcceptConnection();
      const pkm_string& getChannel(size_t)const;
      const pkm_string& getBaseChannel()const;
      pkm_int getNumChannels()const;
      pkm_long getConnectionId()const;
      pkm_long getPermissions()const;
  };

  class SPacketDenyConnection{
  private:
      pkm_json reason;
  public:
      PKMCOM_CONSTEXPR static pkm_byte id = 2;
      pkm_int hashcode()const;
      pkm_uint size()const;
      void read(PacketBuffer&);
      void write(PacketBuffer&)const;
      SPacketDenyConnection(const pkm_json&);
      const pkm_json& getReason()const;
  };

  enum class pkm_action : pkm_byte{
      move = 0, switch_pkm = 1,
      err_item = 2, err_run = 3
  };

  class CPacketAction{
  private:
      pkm_action action;
      union{
          struct{
              pkm_byte move;
              pkm_int numTargets;
              pkm_byte targets[6];
              pkm_boolean mega;
          }combat;
          pkm_byte switch_target;
      };
      pkm_byte source;
  public:
      PKMCOM_CONSTEXPR static pkm_byte id = 3;
      CPacketAction();
      CPacketAction(pkm_byte,pkm_int,pkm_byte[],pkm_boolean);
      CPacketAction(pkm_byte);
      pkm_int hashcode()const;
      pkm_uint size()const;
      void read(PacketBuffer&);
      void write(PacketBuffer&)const;
      pkm_byte getMove()const;
      pkm_int getNumTarget()const;
      const pkm_byte* getTargets()const;
      pkm_boolean isMegaTrigger()const;
      pkm_byte getSwitchTarget()const;
      pkm_action getAction()const;
      pkm_byte getSource()const;
  };
                                                        
};
#endif