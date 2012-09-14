#ifndef POW_H
#define POW_H

template<int B, int N> struct   Pow
{
    enum { value = B*Pow<B, N-1>::value };
};

template<int B> struct          Pow<B, 0>
{
    enum { value = 1 };
};

int bitset[Pow<2, 13>::value];


#endif // POW_H
