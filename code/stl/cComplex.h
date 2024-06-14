#pragma once
#include "c_utility.h"
#include "c_limits.h"
#include "cNumbers.h"

#include <cmath>


CUSTOM_BEGIN

// Definitions
template<class Type>
class Complex;

template<class Type>
struct ComplexTraits;

template<class Type>
Type abs(const Complex<Type>& val); // return magnitude of complex

template<class Type>
constexpr Type norm(const Complex<Type>& val);  // return squared magnitude

template<class Type>
Type arg(const Complex<Type>& val); // return phase angle of complex as real

template<class Type>
constexpr Complex<Type> conj(const Complex<Type>& val); // return complex conjugate

template<class Type>
Complex<Type> proj(const Complex<Type>& val); // return complex projection

template<class Type>
Complex<Type> polar(const Type& rho, const Type& theta = Type());

template<class Type>
Complex<Type> exp(const Complex<Type>& val);

template<class Type>
Complex<Type> log(const Complex<Type>& val);

template<class Type>
Complex<Type> log10(const Complex<Type>& val);

template<class Type>
Complex<Type> sqrt(const Complex<Type>& val);

template<class Type>
Complex<Type> pow(const Complex<Type>& base, const Type& ex);

template<class Type>
Complex<Type> pow(const Type& base, const Complex<Type>& ex);

template<class Type>
Complex<Type> pow(const Complex<Type>& base, const Complex<Type>& ex);

template<class Type>
Complex<Type> sin(const Complex<Type>& val);

template<class Type>
Complex<Type> asin(const Complex<Type>& val);

template<class Type>
Complex<Type> sinh(const Complex<Type>& val);

template<class Type>
Complex<Type> asinh(const Complex<Type>& val);

template<class Type>
Complex<Type> cos(const Complex<Type>& val);

template<class Type>
Complex<Type> acos(const Complex<Type>& val);

template<class Type>
Complex<Type> cosh(const Complex<Type>& val);

template<class Type>
Complex<Type> acosh(const Complex<Type>& val);

template<class Type>
Complex<Type> tan(const Complex<Type>& val);

template<class Type>
Complex<Type> atan(const Complex<Type>& val);

template<class Type>
Complex<Type> tanh(const Complex<Type>& val);

template<class Type>
Complex<Type> atanh(const Complex<Type>& val);


CUSTOM_DETAIL_BEGIN

// complex traits implementation
template<class Type>
struct _ComplexTraitsBase
{
    static_assert(is_floating_point_v<Type>, "Complex traits accepts only floating point data type");

    using FloatType = Type;

    static constexpr FloatType epsilon_v() {
        return numeric_limits<FloatType>::epsilon();
    }

    static constexpr FloatType max_v() {
        return (numeric_limits<FloatType>::max)();
    }

    static constexpr FloatType norm_min_v() {
        return (numeric_limits<FloatType>::min)() > 0 ? (numeric_limits<FloatType>::min)() : 0;
    }

    static constexpr FloatType nan_v() {
        return numeric_limits<FloatType>::quiet_NaN();
    }

    static constexpr FloatType inf_v() {
        return numeric_limits<FloatType>::infinity();
    }

    static constexpr bool is_nan(FloatType val) {
        return val == nan_v();
    }

    static bool is_inf(FloatType val) {
        return ::abs(val) == inf_v();
    }

    static bool signbit(FloatType val) { // has all floating point overloads
        return std::signbit(val);
    }

    static FloatType abs_v(FloatType val) { // has all floating point overloads
        return ::abs(val);
    }

};  // END _ComplexTraitsBase

struct _ComplexTraitsFloat : _ComplexTraitsBase<float>
{
    using FloatType = typename _ComplexTraitsBase<float>::FloatType; // float

    static FloatType copysign(FloatType magnitude, FloatType sign) {
        return ::copysignf(magnitude, sign);
    }

    static FloatType sin(FloatType val) {
        return ::sinf(val);
    }

    static FloatType sinh(FloatType val) {
        return ::sinhf(val);
    }

    static FloatType asinh(FloatType val) {
        return ::asinhf(val);
    }

    static FloatType cos(FloatType val) {
        return ::cosf(val);
    }

    static FloatType cosh(FloatType val) {
        return ::coshf(val);
    }

    static FloatType tan(FloatType val) {
        return ::tanf(val);
    }

    static FloatType tanh(FloatType val) {
        return ::tanhf(val);
    }

    static FloatType atan(FloatType val) {
        return ::atanf(val);
    }

    static FloatType atanh(FloatType val) {
        return ::atanhf(val);
    }

    static FloatType atan2(FloatType yVal, FloatType xVal) {
        return ::atan2f(yVal, xVal);
    }

    static FloatType hypot(FloatType left, FloatType right) { // return sqrt(left ^ 2 + right ^ 2)
        return ::hypotf(left, right);
    }

    static FloatType exp(FloatType val) { // return e ^ val
        return ::expf(val);
    }

    static FloatType ldexp(FloatType val, int exponent) { // return val * 2 ^ exponent
        return ::ldexpf(val, exponent);
    }

    static FloatType log(FloatType val) { // return log_e (val)
        return ::logf(val);
    }

    static FloatType log1p(FloatType val) { // return log_e (1 + val)
        return ::log1pf(val);
    }

    static FloatType pow(FloatType base, FloatType exponent) {
        return ::powf(base, exponent);
    }

    static FloatType sqrt(FloatType val) {
        return ::sqrtf(val);
    }
};  // END _ComplexTraitsFloat

struct _ComplexTraitsDouble : _ComplexTraitsBase<double>
{
    using FloatType = typename _ComplexTraitsBase<double>::FloatType; // double

    static FloatType copysign(FloatType magnitude, FloatType sign) {
        return ::copysign(magnitude, sign);
    }

    static FloatType sin(FloatType val) {
        return ::sin(val);
    }

    static FloatType sinh(FloatType val) {
        return ::sinh(val);
    }

    static FloatType asinh(FloatType val) {
        return ::asinh(val);
    }

    static FloatType cos(FloatType val) {
        return ::cos(val);
    }

    static FloatType cosh(FloatType val) {
        return ::cosh(val);
    }

    static FloatType tan(FloatType val) {
        return ::tan(val);
    }

    static FloatType tanh(FloatType val) {
        return ::tanh(val);
    }

    static FloatType atan(FloatType val) {
        return ::atan(val);
    }

    static FloatType atanh(FloatType val) {
        return ::atanh(val);
    }

    static FloatType atan2(FloatType yVal, FloatType xVal) {
        return ::atan2(yVal, xVal);
    }

    static FloatType hypot(FloatType left, FloatType right) {
        return ::hypot(left, right);
    }

    static FloatType exp(FloatType val) {
        return ::exp(val);
    }

    static FloatType ldexp(FloatType val, int exponent) {
        return ::ldexp(val, exponent);
    }

    static FloatType log(FloatType val) {
        return ::log(val);
    }

    static FloatType log1p(FloatType val) {
        return ::log1p(val);
    }

    static FloatType pow(FloatType base, FloatType exponent) {
        return ::pow(base, exponent);
    }

    static FloatType sqrt(FloatType val) {
        return ::sqrt(val);
    }
};  // END _ComplexTraitsDouble

struct _ComplexTraitsLongDouble : _ComplexTraitsBase<long double>
{
    using FloatType = typename _ComplexTraitsBase<long double>::FloatType; // long double

    static FloatType copysign(FloatType magnitude, FloatType sign) {
        return ::copysignl(magnitude, sign);
    }

    static FloatType sin(FloatType val) {
        return ::sinl(val);
    }

    static FloatType sinh(FloatType val) {
        return ::sinhl(val);
    }

    static FloatType asinh(FloatType val) {
        return ::asinhl(val);
    }

    static FloatType cos(FloatType val) {
        return ::cosl(val);
    }

    static FloatType cosh(FloatType val) {
        return ::coshl(val);
    }

    static FloatType tan(FloatType val) {
        return ::tanl(val);
    }

    static FloatType tanh(FloatType val) {
        return ::tanhl(val);
    }

    static FloatType atan(FloatType val) {
        return ::atanl(val);
    }

    static FloatType atanh(FloatType val) {
        return ::atanhl(val);
    }

    static FloatType atan2(FloatType yVal, FloatType xVal) {
        return ::atan2l(yVal, xVal);
    }

    static FloatType hypot(FloatType left, FloatType right) {
        return ::hypotl(left, right);
    }

    static FloatType exp(FloatType val) {
        return ::expl(val);
    }

    static FloatType ldexp(FloatType val, int exponent) {
        return ::ldexpl(val, exponent);
    }

    static FloatType log(FloatType val) {
        return ::logl(val);
    }

    static FloatType log1p(FloatType val) {
        return ::log1pl(val);
    }

    static FloatType pow(FloatType base, FloatType exponent) {
        return ::powl(base, exponent);
    }

    static FloatType sqrt(FloatType val) {
        return ::sqrtl(val);
    }
};  // END _ComplexTraitsLongDouble


template<class Type>    // helper for polar
Complex<Type> _polar_positive_nan_inf_zero_rho(const Type& rho, const Type& theta) { // rho is +NaN/+Inf/+0
    if (ComplexTraits<Type>::is_nan(theta) || ComplexTraits<Type>::is_inf(theta))
        if (ComplexTraits<Type>::is_inf(rho))
            return Complex<Type>(rho, ComplexTraits<Type>::sin(theta)); // (Inf, NaN/Inf)
        else
            return Complex<Type>(rho, ComplexTraits<Type>::copysign(rho, theta)); // (NaN/0, NaN/Inf)
    else if (theta == Type())
        return Complex<Type>(rho, theta); // (NaN/Inf/0, 0)
    else // theta is finite non-zero
        return Complex<Type>(rho * ComplexTraits<Type>::cos(theta),
            rho * ComplexTraits<Type>::sin(theta)); // (NaN/Inf/0, finite non-zero)
}

CUSTOM_DETAIL_END


// complex traits specialization
template<class Type>
struct ComplexTraits : detail::_ComplexTraitsBase<Type> {};  // cannot use other than specializations below

template<>
struct ComplexTraits<float> : detail::_ComplexTraitsFloat {};

template<>
struct ComplexTraits<double> : detail::_ComplexTraitsDouble {};

template<>
struct ComplexTraits<long double> : detail::_ComplexTraitsLongDouble {};


template<class Type>
class Complex
{
public:
    static_assert(is_floating_point_v<Type>, "Complex class accepts only floating point data type");

    using TraitsType    = ComplexTraits<Type>;
    using ValueType     = Type;

private:
    enum RepParts
    {
        Real = 0,
        Imag = 1
    };

    ValueType _rep[2];  // values stored in array where 0-real / 1-imag

public:
    // Constructors

    constexpr Complex(  const ValueType& realVal = ValueType(),
                        const ValueType& imagVal = ValueType())
        : _rep{realVal, imagVal} { /*Empty*/ }

    template<class _Type>
    constexpr Complex(const Complex<_Type>& other)
        : _rep{ static_cast<ValueType>(other.real()), static_cast<ValueType>(other.imag()) } { /*Empty*/ }

public:
    // Operators

    // from this type
    constexpr Complex& operator=(const ValueType& val) {
        _rep[Real] = val;
        _rep[Imag] = 0;
        return *this;
    }

    constexpr Complex& operator+=(const ValueType& val) {
        _rep[Real] += val;
        return *this;
    }

    constexpr Complex& operator-=(const ValueType& val) {
        _rep[Real] -= val;
        return *this;
    }

    constexpr Complex& operator*=(const ValueType& val) {
        _rep[Real] *= val;
        _rep[Imag] *= val;
        return *this;
    }

    constexpr Complex& operator/=(const ValueType& val) {
        _rep[Real] /= val;
        _rep[Imag] /= val;
        return *this;
    }
    // END from this type

    // from this complex
    constexpr Complex& operator+=(const Complex& other) {
        _add(other);
        return *this;
    }

    constexpr Complex& operator-=(const Complex& other) {
        _sub(other);
        return *this;
    }

    constexpr Complex& operator*=(const Complex& other) {
        _mul(other);
        return *this;
    }

    constexpr Complex& operator/=(const Complex& other) {
        _div(other);
        return *this;
    }
    // END from this complex

    // from other complex type
    template<class _Type>
    constexpr Complex& operator=(const Complex<_Type>& other) {
        _rep[Real] = static_cast<ValueType>(other.real());
        _rep[Imag] = static_cast<ValueType>(other.imag());
        return *this;
    }

    template<class _Type>
    constexpr Complex& operator+=(const Complex<_Type>& other) {
        _add(other);
        return *this;
    }

    template<class _Type>
    constexpr Complex& operator-=(const Complex<_Type>& other) {
        _sub(other);
        return *this;
    }

    template<class _Type>
    constexpr Complex& operator*=(const Complex<_Type>& other) {
        _mul(other);
        return *this;
    }

    template<class _Type>
    constexpr Complex& operator/=(const Complex<_Type>& other) {
        _div(other);
        return *this;
    }
    // END from other comnplex type

public:
    // Main functions

    constexpr void real(const ValueType& val) { // set real component
        _rep[Real] = val;
    }

    constexpr void imag(const ValueType& val) { // set imaginary component
        _rep[Imag] = val;
    }

    constexpr ValueType real() const { // return real component
        return _rep[Real];
    }

    constexpr ValueType imag() const { // return imaginary component
        return _rep[Imag];
    }

private:
    // Helpers

    template<class _Type>
    constexpr void _add(const Complex<_Type>& other) {
        _rep[Real] += static_cast<ValueType>(other.real());
        _rep[Imag] += static_cast<ValueType>(other.imag());
    }

    template<class _Type>
    constexpr void _sub(const Complex<_Type>& other) {
        _rep[Real] -= static_cast<ValueType>(other.real());
        _rep[Imag] -= static_cast<ValueType>(other.imag());
    }

    template<class _Type>
    constexpr void _mul(const Complex<_Type>& other) {
        ValueType otherReal = static_cast<ValueType>(other.real());
        ValueType otherImag = static_cast<ValueType>(other.imag());

        ValueType temp      = _rep[Real] * otherReal - _rep[Imag] * otherImag;
        _rep[Imag]          = _rep[Real] * otherImag + _rep[Imag] * otherReal;
        _rep[Real]          = temp;
    }

    template<class _Type>
    constexpr void _div(const Complex<_Type>& other) {
        ValueType otherReal = static_cast<ValueType>(other.real());
        ValueType otherImag = static_cast<ValueType>(other.imag());

        if (TraitsType::is_nan(otherReal) || TraitsType::is_nan(otherImag))
        {
            // set NaN result
            _rep[Real] = TraitsType::nan_v();
            _rep[Imag] = _rep[Real];
        }
        else if (TraitsType::abs_v(otherImag) < TraitsType::abs_v(otherReal))
        {
            ValueType wr = otherImag / otherReal;
            ValueType wd = otherReal + wr * otherImag;

            if (TraitsType::is_nan(wd) || wd == 0)
            {
                // set NaN result
                _rep[Real] = TraitsType::nan_v();
                _rep[Imag] = _rep[Real];
            }
            else
            {
                // compute representable result
                ValueType temp  = (_rep[Real] + _rep[Imag] * wr) / wd;
                _rep[Imag]      = (_rep[Imag] - _rep[Real] * wr) / wd;
                _rep[Real]      = temp;
            }
        }
        else if (otherImag == 0) // other._rep[Real] == 0 && other._rep[Imag] == 0
        {
            _rep[Real] /= otherReal;
            _rep[Imag] /= otherReal;
        }
        else // 0 < |other._rep[Real]| <= |other._rep[Imag]|
        {
            ValueType wr = otherReal / otherImag;
            ValueType wd = otherImag + wr * otherReal;

            if (TraitsType::is_nan(wd) || wd == 0)
            {
                // set NaN result
                _rep[Real] = TraitsType::nan_v();
                _rep[Imag] = _rep[Real];
            }
            else
            {
                // compute representable result
                ValueType temp  = (_rep[Real] * wr + _rep[Imag]) / wd;
                _rep[Imag]      = (_rep[Imag] * wr - _rep[Real]) / wd;
                _rep[Real]      = temp;
            }
        }
    }
};  // END Complex common


// Complex unary operators

template<class Type>
constexpr Complex<Type> operator+(const Complex<Type>& right) {
    return right;
}

template<class Type>
constexpr Complex<Type> operator-(const Complex<Type>& right) {
    return Complex<Type>(-right.real(), -right.imag());
}

// Complex binary operators

// ostream
template<class Type>
std::ostream& operator<<(std::ostream& os, const Complex<Type>& complex) {
	os << '(' << complex.real() << ',' << complex.imag() << ')';
	return os;
}

// add
template<class Type>
constexpr Complex<Type> operator+(const Complex<Type>& left, const Complex<Type>& right) {
    Complex<Type> temp(left);
    temp += right;
    return temp;
}

template<class Type>
constexpr Complex<Type> operator+(const Complex<Type>& left, const Type& val) {
    Complex<Type> temp(left);
    temp += val;
    return temp;
}

template<class Type>
constexpr Complex<Type> operator+(const Type& val, const Complex<Type>& right) {
    Complex<Type> temp(right);
    temp += val;
    return temp;
}

// sub
template<class Type>
constexpr Complex<Type> operator-(const Complex<Type>& left, const Complex<Type>& right) {
    Complex<Type> temp(left);
    temp -= right;
    return temp;
}

template<class Type>
constexpr Complex<Type> operator-(const Complex<Type>& left, const Type& val) {
    Complex<Type> temp(left);
    temp -= val;
    return temp;
}

template<class Type>
constexpr Complex<Type> operator-(const Type& val, const Complex<Type>& right) {
    Complex<Type> temp(val);
    temp -= right;
    return temp;
}

// mul
template<class Type>
constexpr Complex<Type> operator*(const Complex<Type>& left, const Complex<Type>& right) {
    Complex<Type> temp(left);
    temp *= right;
    return temp;
}

template<class Type>
constexpr Complex<Type> operator*(const Complex<Type>& left, const Type& val) {
    Complex<Type> temp(left);
    temp *= val;
    return temp;
}

template<class Type>
constexpr Complex<Type> operator*(const Type& val, const Complex<Type>& right) {
    Complex<Type> temp(right);
    temp *= val;
    return temp;
}

// div
template<class Type>
constexpr Complex<Type> operator/(const Complex<Type>& left, const Complex<Type>& right) {
    Complex<Type> temp(left);
    temp /= right;
    return temp;
}

template<class Type>
constexpr Complex<Type> operator/(const Complex<Type>& left, const Type& val) {
    Complex<Type> temp(left);
    temp /= val;
    return temp;
}

template<class Type>
constexpr Complex<Type> operator/(const Type& val, const Complex<Type>& right) {
    Complex<Type> temp(val);
    temp /= right;
    return temp;
}

// equal
template<class Type>
constexpr bool operator==(const Complex<Type>& left, const Complex<Type>& right) {
    return left.real() == right.real() && left.imag() == right.imag();
}

template<class Type>
constexpr bool operator!=(const Complex<Type>& left, const Complex<Type>& right) {
    return !(left == right);
}

template<class Type>
constexpr bool operator==(const Complex<Type>& left, const Type& val) {
    return left.real() == val && left.imag() == 0;
}

template<class Type>
constexpr bool operator!=(const Complex<Type>& left, const Type& val) {
    return !(left == val);
}

template<class Type>
constexpr bool operator==(const Type& val, const Complex<Type>& right) {
    return val == right.real() && 0 == right.imag();
}

template<class Type>
constexpr bool operator!=(const Type& val, const Complex<Type>& right) {
    return !(val == right);
}

// Other Complex operations

template<class Type>
Type abs(const Complex<Type>& val) { // return magnitude of complex
    return ComplexTraits<Type>::hypot(val.real(), val.imag());
}

template<class Type>
constexpr Type norm(const Complex<Type>& val) { // return squared magnitude
    return val.real() * val.real() + val.imag() * val.imag();
}

template<class Type>
Type arg(const Complex<Type>& val) { // return phase angle of complex as real
    return ComplexTraits<Type>::atan2(val.imag(), val.real());
}

template<class Type>
constexpr Complex<Type> conj(const Complex<Type>& val) { // return complex conjugate
    return Complex<Type>(val.real(), -val.imag());
}

template<class Type>
Complex<Type> proj(const Complex<Type>& val) { // return complex projection
    if (ComplexTraits<Type>::is_inf(val.real()) || ComplexTraits<Type>::is_inf(val.imag()))
        return Complex<Type>(   ComplexTraits<Type>::inf_v(),
                                ComplexTraits<Type>::copysign(Type(), val.imag()));

    return val;
}

template<class Type>
Complex<Type> polar(const Type& rho, const Type& theta) {
    if (ComplexTraits<Type>::signbit(rho))
        return -detail::_polar_positive_nan_inf_zero_rho(-rho, theta);

    return detail::_polar_positive_nan_inf_zero_rho(rho, theta);
}

template<class Type>
Complex<Type> exp(const Complex<Type>& val) {
    return custom::polar(ComplexTraits<Type>::exp(val.real()), val.imag());
}

template<class Type>
Complex<Type> log(const Complex<Type>& val) {
    return Complex<Type>(ComplexTraits<Type>::log(custom::abs(val)), custom::arg(val));
}

template<class Type>
Complex<Type> log10(const Complex<Type>& val) {
    return custom::log(val) / ComplexTraits<Type>::log(Type(10.0));
}

template<class Type>
Complex<Type> sqrt(const Complex<Type>& val) {
    const Type realVal  = val.real();
    const Type imagVal  = val.imag();
    const Type zeroVal  = Type();

    if (realVal == zeroVal)
    {
        Type temp = ComplexTraits<Type>::sqrt(ComplexTraits<Type>::abs_v(imagVal) / Type(2.0));
        return Complex<Type>(temp, imagVal < zeroVal ? -temp : temp);
    }
    else
    {
        Type temp       = ComplexTraits<Type>::sqrt(Type(2.0) * (custom::abs(val) + ComplexTraits<Type>::abs_v(realVal)));
        Type tempBy2    = temp / Type(2.0);
        
        return realVal > zeroVal ?  Complex<Type>(tempBy2, imagVal / temp) :
                                    Complex<Type>(  ComplexTraits<Type>::abs_v(imagVal) / temp,
                                                    imagVal < zeroVal ? -tempBy2 : tempBy2);
    }
}

template<class Type>
Complex<Type> pow(const Complex<Type>& base, const Type& ex) {
    if (base.imag() == Type() && base.real() > Type())
        return ComplexTraits<Type>::pow(base.real(), ex);

    Complex<Type> temp = custom::log(base);
    return custom::polar<Type>(ComplexTraits<Type>::exp(ex * temp.real()), ex * temp.imag());
}

template<class Type>
Complex<Type> pow(const Type& base, const Complex<Type>& ex) {
    if (base > Type())
        return custom::polar<Type>( ComplexTraits<Type>::pow(base, ex.real()),
                                    ex.imag() * ComplexTraits<Type>::log(base));

    return custom::pow(Complex<Type>(base), ex);
}

template<class Type>
Complex<Type> pow(const Complex<Type>& base, const Complex<Type>& ex) {
    return base == Type() ? Type() : custom::exp(ex * custom::log(base));
}

template<class Type>
Complex<Type> sin(const Complex<Type>& val) {
    return Complex<Type>(   ComplexTraits<Type>::cosh(val.imag()) * ComplexTraits<Type>::sin(val.real()),
                            ComplexTraits<Type>::sinh(val.imag()) * ComplexTraits<Type>::cos(val.real()));
}

template<class Type>
Complex<Type> asin(const Complex<Type>& val) {
    Complex<Type> asinhTemp = custom::asinh(Complex<Type>(-val.imag(), val.real()));
    return Complex<Type>(asinhTemp.imag(), -asinhTemp.real());
}

template<class Type>
Complex<Type> sinh(const Complex<Type>& val) {
    return Complex<Type>(   ComplexTraits<Type>::sinh(val.real()) * ComplexTraits<Type>::cos(val.imag()),
                            ComplexTraits<Type>::cosh(val.real()) * ComplexTraits<Type>::sin(val.imag()));
}

template<class Type>
Complex<Type> asinh(const Complex<Type>& val) {
    // Kahan's formula -> asinh(z) = ln(z + sqrt(z^2 + 1))
    Complex<Type> temp((val.real() - val.imag()) * (val.real() + val.imag()) + Type(1.0),
                        Type(2.0) * val.real() * val.imag());

    return custom::log(custom::sqrt(temp) + val);
}

template<class Type>
Complex<Type> cos(const Complex<Type>& val) {
    return Complex<Type>(   ComplexTraits<Type>::cosh(val.imag()) * ComplexTraits<Type>::cos(val.real()),
                            -ComplexTraits<Type>::sinh(val.imag()) * ComplexTraits<Type>::sin(val.real()));
}

template<class Type>
Complex<Type> acos(const Complex<Type>& val) {
    const Complex<Type> asinTemp = custom::asin(val);
    return Complex<Type>(custom::numbers::pi_by_2_v<Type> - asinTemp.real(), -asinTemp.imag());
}

template<class Type>
Complex<Type> cosh(const Complex<Type>& val) {
    return Complex<Type>(   ComplexTraits<Type>::cosh(val.real()) * ComplexTraits<Type>::cos(val.imag()),
                            ComplexTraits<Type>::sinh(val.real()) * ComplexTraits<Type>::sin(val.imag()));
}

template<class Type>
Complex<Type> acosh(const Complex<Type>& val) {
    // Kahan's formula -> acosh(z) = ln(z + sqrt(z^2 - 1))
    return Type(2.0) * custom::log( custom::sqrt(Type(0.5) * (val + Type(1.0))) +
                                    custom::sqrt(Type(0.5) * (val - Type(1.0))));
}

template<class Type>
Complex<Type> tan(const Complex<Type>& val) {
    return custom::sin(val) / custom::cos(val);
}

template<class Type>
Complex<Type> atan(const Complex<Type>& val) {
    Complex<Type> atanhTemp = custom::atanh(Complex<Type>(-val.imag(), val.real()));
    return Complex<Type>(atanhTemp.imag(), -atanhTemp.real());
}

template<class Type>
Complex<Type> tanh(const Complex<Type>& val) {
    return custom::sinh(val) / custom::cosh(val);
}

template<class Type>
Complex<Type> atanh(const Complex<Type>& val) {
    const Type imag2    = val.imag() * val.imag();
    const Type temp     = Type(1.0) - imag2 - val.real() * val.real();

    Type num = Type(1.0) + val.real();
    Type den = Type(1.0) - val.real();

    num = imag2 + num * num;
    den = imag2 + den * den;

    return Complex<Type>(   Type(0.25) * (ComplexTraits<Type>::log(num) - ComplexTraits<Type>::log(den)),
			                Type(0.5) * ComplexTraits<Type>::atan2(Type(2.0) * val.imag(), temp));
}

CUSTOM_END