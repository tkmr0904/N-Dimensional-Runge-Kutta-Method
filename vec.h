#include <iostream>

template<int n>
class Vec
{
    public:
    double a[n];
    constexpr Vec(){}
    
    template<typename... Args>
    constexpr Vec(Args ...args): a{args...}
    {}

    constexpr double length() const
    {   
        double sum = 0;
        for(int i=0; i<n; ++i)
            sum += a[i]*a[i];
        return sqrt(sum);
    }
    constexpr Vec normalize();

    double& operator[](int const& i)
    {
        return a[i];
    }
};

template<int n>
std::ostream& operator<<(std::ostream& l, const Vec<n>& r) 
{
    std::cout << '(';
    for(int i=0; i<n-1; i++)
        std::cout << r.a[i] << ", ";
    std::cout << r.a[n-1];
    std::cout << ')';
    return l;
}

template<int n>
constexpr Vec<n> operator / (Vec<n> const& l, double const& r)
{
    Vec<n> ret;
    for(int i=0; i<n; i++)
        ret.a[i] = l.a[i] / r;
    return ret;
}

template<int n>
constexpr void operator /=(Vec<n>& l, double const& r)
{   l = l/r;}

template<int n>
constexpr Vec<n> Vec<n>::normalize()
{   *this /= this->length();  return *this;}

template<int n>
constexpr Vec<n> operator + (Vec<n> const& l, Vec<n> const& r)
{   
    Vec<n> ret;
    for(int i=0; i<n; i++)
        ret.a[i] = l.a[i] + r.a[i];
    return ret;
}

template<int n>
constexpr void operator += (Vec<n>& l, Vec<n> const& r)
{   
    l = l + r;
}

template<int n>
constexpr Vec<n> operator - (Vec<n> const& l, Vec<n> const& r)
{   
    Vec<n> ret;
    for(int i=0; i<n; i++)
        ret.a[i] = l.a[i] - r.a[i];
    return ret;
}

template<int n>
constexpr Vec<n> operator * (Vec<n> const& l, double const& r)
{
    Vec<n> ret;
    for(int i=0; i<n; i++)
        ret.a[i] = l.a[i] * r;
    return ret;
}

template<int n>
constexpr Vec<n> operator * (double const& l, Vec<n> const& r)
{
    return r*l;
}


template<int n>
constexpr void operator *=(Vec<n>& l, double const& r)
{   l = l*r;}

template<int n>
constexpr double dot(Vec<n> const& l, Vec<n> const& r)
{   
    double ret = 0;
    for(int i=0; i<n; i++)
        ret += l.a[i] * r.a[i];
    return ret;
}

template<int n>
std::istream& operator>>(std::istream& l, Vec<n>& r)
{
    for(int i=0; i<n; i++)
        std::cin >> r.a[i]; 
    return l;
}

