#ifndef GENERALPURPOSEREGISTER_HPP
#define GENERALPURPOSEREGISTER_HPP



#include <cstdint>


#define R64_TO_32 0xFFFFFFFF
#define R32_TO_16 0xFFFF
#define R16_TO_8 0xFF


__declspec( align( 16 ) )struct _XMM_REGISTER
{
    uintmax_t Low; ///< Lower 64-bits of the XMM register
    intmax_t High; ///< Higher 64-bits of the XMM register
};

__declspec( align( 32 ) )struct _YMM_REGISTER
{
    _XMM_REGISTER  Low;  ///< Lower 128-bits of the YMM register
    _XMM_REGISTER  High; ///< Higher 128-bits of the YMM register
};

/**
 * @brief Represents a general-purpose register with access to different sizes (64-bit, 32-bit, 16-bit, and 8-bit).
 */
struct _GP_REGISTER
{
private:

    uintmax_t value; ///< Holds the 64-bit value of the register

public:
    /**
     * @brief Gets the full 64-bit value of the register.
     * @return The 64-bit value.
     */
    __forceinline auto GetR64() const
    {
        return this->value;
    }

    /**
     * @brief Gets the lower 32-bit value of the register.
     * @return The 32-bit value.
     */
    __forceinline auto GetR32() const
    {
        return static_cast< uint32_t >( this->value & R64_TO_32 );
    }

    /**
     * @brief Gets the lower 16-bit value of the register.
     * @return The 16-bit value.
     */
    __forceinline auto GetR16() const
    {
        return static_cast< uint16_t >( this->value & R32_TO_16 );
    }

    /**
     * @brief Gets the lower 8-bit value of the register.
     * @return The 8-bit value.
     */
    __forceinline auto GetR8() const
    {
        return static_cast< uint8_t >( this->value & R16_TO_8 );
    }

    /**
     * @brief Assigns a new value to the register.
     * @param val The value to assign.
     */
    __forceinline void operator=( __in const uintmax_t& val )
    {
        this->value = val;
    }

    /**
     * @brief Checks if the register value is equal to a given value.
     * @param val The value to compare with.
     * @return True if equal, false otherwise.
     */
    __forceinline bool operator==( __in const uintmax_t& val ) const
    {
        return this->value == val;
    }

    /**
     * @brief Checks if the register value is not equal to a given value.
     * @param val The value to compare with.
     * @return True if not equal, false otherwise.
     */
    __forceinline bool operator!=( __in const uintmax_t& val ) const
    {
        return this->value != val;
    }

    /**
     * @brief Increments the register value by 1.
     */
    __forceinline void operator++()
    {
        this->value++;
    }

    /**
     * @brief Decrements the register value by 1.
     */
    __forceinline void operator--()
    {
        this->value--;
    }

    /**
     * @brief Adds a given value to the register value.
     * @param val The value to add.
     */
    __forceinline void operator+=( __in const uintmax_t& val )
    {
        this->value += val;
    }

    /**
     * @brief Subtracts a given value from the register value.
     * @param val The value to subtract.
     */
    __forceinline void operator-=( __in const uintmax_t& val )
    {
        this->value -= val;
    }

    /**
     * @brief Checks if the register value is less than a given value.
     * @param val The value to compare with.
     * @return True if less, false otherwise.
     */
    __forceinline bool operator<( __in const uintmax_t& val ) const
    {
        return this->value < val;
    }

    /**
     * @brief Checks if the register value is greater than a given value.
     * @param val The value to compare with.
     * @return True if greater, false otherwise.
     */
    __forceinline bool operator>( __in const uintmax_t& val ) const
    {
        return this->value > val;
    }

    /**
     * @brief Performs a bitwise AND operation between the register value and a given value.
     * @param val The value to AND with.
     * @return The result of the AND operation.
     */
    __forceinline auto operator&( __in const uintmax_t& val ) const
    {
        return( this->value & val );
    }

    /**
     * @brief Performs a bitwise OR operation between the register value and a given value.
     * @param val The value to OR with.
     * @return The result of the OR operation.
     */
    __forceinline auto operator|( __in const uintmax_t& val ) const
    {
        return( this->value | val );
    }

    /**
     * @brief Performs a bitwise XOR operation between the register value and a given value.
     * @param val The value to XOR with.
     * @return The result of the XOR operation.
     */
    __forceinline auto operator^( __in const uintmax_t& val ) const
    {
        return( this->value ^ val );
    }

    /**
     * @brief Performs a bitwise NOT operation on the register value.
     * @return The result of the NOT operation.
     */
    __forceinline auto operator~() const
    {
        return ~this->value;
    }

    /**
     * @brief Performs a left shift operation on the register value by a given number of bits.
     * @param val The number of bits to shift.
     * @return The result of the left shift operation.
     */
    __forceinline auto operator<<( __in const uintmax_t& val ) const
    {
        return( this->value << val );
    }

    /**
     * @brief Performs a right shift operation on the register value by a given number of bits.
     * @param val The number of bits to shift.
     * @return The result of the right shift operation.
     */
    __forceinline auto operator>>( __in const uintmax_t& val ) const
    {
        return( this->value >> val );
    }

    /**
     * @brief Performs a left shift assignment on the register value by a given number of bits.
     * @param val The number of bits to shift.
     */
    __forceinline void operator<<=( __in const uintmax_t& val )
    {
        this->value <<= val;
    }

    /**
     * @brief Performs a right shift assignment on the register value by a given number of bits.
     * @param val The number of bits to shift.
     */
    __forceinline void operator>>=( __in const uintmax_t& val )
    {
        this->value >>= val;
    }
};



#endif // !GENERALPURPOSEREGISTER_HPP
