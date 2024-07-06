#ifndef GENERALPURPOSEREGISTER_HPP
#define GENERALPURPOSEREGISTER_HPP



#include <cstdint>


#define R64_TO_32 0xFFFFFFFF
#define R32_TO_16 0xFFFF
#define R16_TO_8 0xFF


__declspec(align(16))struct _XMM_REGISTER 
{
    uintmax_t Low;
    intmax_t High;
};


struct _GP_REGISTER
{
private:

    uintmax_t value;

public:
    __forceinline auto GetR64() const
    {
        return this->value;
    }

    __forceinline auto GetR32() const
    {
        return static_cast<uint32_t>(this->value & R64_TO_32);
    }

    __forceinline auto GetR16() const
    {
        return static_cast<uint16_t>(this->value & R32_TO_16);
    }

    __forceinline auto GetR8() const
    {
        return static_cast<uint8_t>(this->value & R16_TO_8);
    }

    __forceinline void operator=(__in const uintmax_t& val)
    {
        this->value = val;
    }

    __forceinline bool operator==(__in const uintmax_t& val) const
    {
        return this->value == val;
    }

    __forceinline bool operator!=(__in const uintmax_t& val) const
    {
        return this->value != val;
    }

    __forceinline void operator++()
    {
        this->value++;
    }

    __forceinline void operator--()
    {
        this->value--;
    }

    __forceinline void operator+=(__in const uintmax_t& val)
    {
        this->value += val;
    }

    __forceinline void operator-=(__in const uintmax_t& val)
    {
        this->value -= val;
    }

    __forceinline bool operator<(__in const uintmax_t& val) const
    {
        return this->value < val;
    }

    __forceinline bool operator>(__in const uintmax_t& val) const
    {
        return this->value > val;
    }

    __forceinline auto operator&(__in const uintmax_t& val) const
    {
        return(this->value & val);
    }

    __forceinline auto operator|(__in const uintmax_t& val) const
    {
        return(this->value | val);
    }

    __forceinline auto operator^(__in const uintmax_t& val) const
    {
        return(this->value ^ val);
    }

    __forceinline auto operator~() const
    {
        return ~this->value;
    }

    __forceinline auto operator<<(__in const uintmax_t& val) const
    {
        return(this->value << val);
    }

    __forceinline auto operator>>(__in const uintmax_t& val) const
    {
        return(this->value >> val);
    }

    __forceinline void operator<<=(__in const uintmax_t& val)
    {
        this->value <<= val;
    }

    __forceinline void operator>>=(__in const uintmax_t& val)
    {
        this->value >>= val;
    }
};



#endif // !GENERALPURPOSEREGISTER_HPP


