#include "AES.hpp"
#include <cstring>
unsigned char multiply(unsigned char a,unsigned char b){
    unsigned char ret(a);
    if(b==1)
        return a;
    else while(b>0){
        if(b==1){
            b--;
            ret^=a;
        }else{
            b-=2;
            ret^=(a<<1^((a&0x80!=0)?0x1b:0));
        }
    }
    return a;
}
namespace aes{
    aes_word::aes_word(int val):val(val){}
    aes_word::aes_word(unsigned char(&input)[4]){
        val = input[0]|int(input[1])<<8|int(input[2])<<16|int(input[3])<<24;
    }
    aes_word::aes_word(){}
    aes_word& aes_word::operator=(int val){
        this->val = val;
        return *this;
    }

    unsigned char& aes_word::operator [](size_t i){
        return reinterpret_cast<unsigned char(&)[4]>(val)[i];
    }    
    aes_word::operator int(){
        return val;
    }
    aes_word aes_word::operator%(unsigned char(&table)[256]){
        union{
            unsigned char bytes[4];
            int val;
        };
        val = this->val;
        for(unsigned char& b:bytes)
            b = table[b];
        return val;
    }
    aes_word& aes_word::operator%=(unsigned char(&table)[256]){
        return (*this = (this->operator%(table)));
    }
    aes_word aes_word::operator*(aes_matrix& mtx){
        union{
            int val;
            unsigned char bytes[4];
        };
        unsigned char (&matrix)[4][4] = mtx.entries;
        val = this->val;
        bytes[0] = multiply(bytes[0],matrix[0][0])^multiply(bytes[1],matrix[0][1])^multiply(bytes[2],matrix[0][2])^multiply(bytes[3],matrix[0][3]);
        bytes[1] = multiply(bytes[0],matrix[1][0])^multiply(bytes[1],matrix[1][1])^multiply(bytes[2],matrix[1][2])^multiply(bytes[3],matrix[1][3]);
        bytes[2] = multiply(bytes[0],matrix[2][0])^multiply(bytes[1],matrix[2][1])^multiply(bytes[2],matrix[2][2])^multiply(bytes[3],matrix[2][3]);
        bytes[3] = multiply(bytes[0],matrix[3][0])^multiply(bytes[1],matrix[3][1])^multiply(bytes[2],matrix[3][2])^multiply(bytes[3],matrix[3][3]);
    }
    aes_word& aes_word::operator*=(aes_matrix& mtx){
        return (*this = (this->operator*(mtx)));
    }
    aes_word aes_word::operator<<(int i){
        unsigned char bytes[4];
        bytes[0] =(*this)[4];
        bytes[1] = (*this)[0];
        bytes[2] = (*this)[1];
        bytes[3] = (*this)[2];
        return bytes;
    }
    aes_word& aes_word::operator<<=(int i){
        return (*this = (this->operator<<(i)));
    }
    aes_word aes_word::operator^(const aes_word& e){
        return val^e.val;
    }
    aes_word& aes_word::operator^=(const aes_word& e){
        val ^= e.val;
        return *this;
    }
    aes_block::aes_block(){}
    aes_block::aes_block(unsigned char(&arr)[4][4]){
        memcpy(this->bytes,arr,16);
    }
    aes_block aes_block::operator<<(int i){
        unsigned char block[4][4];
        block[0][0] = bytes[0][0];
        block[1][0] = bytes[1][0];
        block[2][0] = bytes[2][0];
        block[3][0] = bytes[3][0];
        block[0][1] = bytes[3][1];
        block[1][1] = bytes[0][1];
        block[2][1] = bytes[1][1];
        block[3][1] = bytes[2][1];
        block[0][2] = bytes[2][2];
        block[1][2] = bytes[3][2];
        block[2][2] = bytes[0][2];
        block[3][2] = bytes[1][2];
        block[0][3] = bytes[3][3];
        block[1][3] = bytes[0][3];
        block[2][3] = bytes[1][3];
        block[3][3] = bytes[2][3];
        return block;
    }
    aes_block aes_block::operator>>(int i){
        unsigned char block[4][4];
        block[0][0] = bytes[0][0];
        block[1][0] = bytes[1][0];
        block[2][0] = bytes[2][0];
        block[3][0] = bytes[3][0];
        block[0][1] = bytes[1][1];
        block[1][1] = bytes[2][1];
        block[2][1] = bytes[3][1];
        block[3][1] = bytes[0][1];
        block[0][2] = bytes[2][2];
        block[1][2] = bytes[1][2];
        block[2][2] = bytes[0][2];
        block[3][2] = bytes[2][2];
        block[0][3] = bytes[3][3];
        block[1][3] = bytes[0][3];
        block[2][3] = bytes[1][3];
        block[3][3] = bytes[2][3];
        return block;
    }
    aes_block& aes_block::operator<<=(int i){
        return (*this = (this->operator<<(i)));
    }
    aes_word& aes_block::operator[](int i){
        return reinterpret_cast<aes_word(&)[4]>(bytes)[i];
    }
    aes_block aes_block::operator%(unsigned char(&sbox)[256]){
        unsigned char block[4][4];
        for(size_t x = 0;x<4;x++)
            for(size_t y = 0;y<4;y++)
                block[x][y] = sbox[bytes[x][y]];
        return block;
    }
    aes_block& aes_block::operator%=(unsigned char(&sbox)[256]){
        for(size_t x = 0;x<4;x++)
            for(size_t y = 0;y<4;y++)
                bytes[x][y] = sbox[bytes[x][y]];
        return *this;
    }
    aes_block aes_block::operator^(round_key& key){
        unsigned char block[4][4];
        for(size_t x = 0;x<4;x++)
            for(size_t y = 0;y<4;y++)
                block[x][y] = bytes[x][y]^key.keys[x][y];
        return block;
    }
    aes_block& aes_block::operator^=(round_key& key){
        for(size_t x = 0;x<4;x++)
            for(size_t y = 0;y<4;y++)
                bytes[x][y] ^= key.keys[x][y];
        return *this;
    }
    aes_block& aes_block::operator<<=(int i){
        return (*this = (this->operator<<(i)));
    }
    aes_block& aes_block::operator>>=(int i){
        return (*this = (this->operator>>(i)));
    }
    namespace schedule{
        void core(aes_word& word,int round){
            word %= sbox;
            word[3] ^= rcon[round]; 
        }
    };

    void getKey(aes_key& key,unsigned char(&inKey)[16]){
        key.keySize = 16;
        key.rounds = 10;
        memcpy(key.roundKeys,inKey,16);
        for(size_t n = 1;n<10;n++){
            key.roundKeys[n].keys[0] = key.roundKeys[n-1].keys[3];
            schedule::core(key.roundKeys[n].keys[0],n);
            key.roundKeys[n].keys[0] ^= key.roundKeys[n-1].keys[0];
            for(size_t j = 1;j<4;j++){
                key.roundKeys[n].keys[j] = key.roundKeys[n].keys[j-1];
                key.roundKeys[n].keys[j] <<= 1;
                key.roundKeys[n].keys[j] ^= key.roundKeys[n-1].keys[j];
            }
        }
    }
    
};