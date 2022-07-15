// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include <vector>

/**
 * Represent a 2D array.
 * The 2D array is stored in a single array, to improve cache usage.
 */
template <typename T> class Array2D
{
public:
    /**
     * Height and width of the 2D array.
     */
    SIZE_T height;
    SIZE_T width;

    /**
     * The array containing the data of the 2D array.
     */
    TArray<T> data;

    /**
    * Array contains succussful wfc.
    */
    bool complete = false;

    /*
    Array2D()
        : data(TArray<T>()) {
    }
    */


    /**
     * Build a 2D array given its height and width.
     * All the array elements are initialized to default value.
     */
    Array2D(SIZE_T height, SIZE_T width) noexcept
        : height(height), width(width) {
        data.SetNum(width * height);
    }

    /**
     * Build a 2D array given its height and width.
     * All the array elements are initialized to value.
     */
    Array2D(SIZE_T height, SIZE_T width, T value) noexcept
        : height(height), width(width) {
        data.Init(value, width * height);
    }
     /**
     * Build a 2D array given its height, width, and data.
     */
    Array2D(SIZE_T height, SIZE_T width, TArray<T> data) noexcept
        : height(height), width(width), data(data) { }
    /**
     * Return a const reference to the element in the i-th line and j-th column.
     * i must be lower than height and j lower than width.
     */
    const T& get(SIZE_T i, SIZE_T j) const noexcept {
        //assert(i < height&& j < width);
        return data[j + i * width];
    }

    /**
     * Return a reference to the element in the i-th line and j-th column.
     * i must be lower than height and j lower than width.
     */
    T& get(SIZE_T i, SIZE_T j) noexcept {
        //assert(i < height&& j < width);
        return data[j + i * width];
    }

    /**
     * Return the current 2D array reflected along the x axis.
     */
    Array2D<T> reflected() const noexcept {
        Array2D<T> result = Array2D<T>(width, height);
        for (SIZE_T y = 0; y < height; y++) {
            for (SIZE_T x = 0; x < width; x++) {
                result.get(y, x) = get(y, width - 1 - x);
            }
        }
        return result;
    }

    /**
     * Return the current 2D array rotated 90° anticlockwise
     */
    Array2D<T> rotated() const noexcept {
        Array2D<T> result = Array2D<T>(width, height);
        for (SIZE_T y = 0; y < width; y++) {
            for (SIZE_T x = 0; x < height; x++) {
                result.get(y, x) = get(x, width - 1 - y);
            }
        }
        return result;
    }

    /**
     * Return the sub 2D array starting from (y,x) and with size (sub_width,
     * sub_height). The current 2D array is considered toric for this operation.
     */
    Array2D<T> get_sub_array(SIZE_T y, SIZE_T x, SIZE_T sub_width,
        SIZE_T sub_height) const noexcept {
        Array2D<T> sub_array_2d = Array2D<T>(sub_width, sub_height);
        for (SIZE_T ki = 0; ki < sub_height; ki++) {
            for (SIZE_T kj = 0; kj < sub_width; kj++) {
                sub_array_2d.get(ki, kj) = get((y + ki) % height, (x + kj) % width);
            }
        }
        return sub_array_2d;
    }

    /**
     * Check if two 2D arrays are equals.
     */
    bool operator==(const Array2D<T>& a) const noexcept {
        if (height != a.height || width != a.width) {
            return false;
        }

        for (SIZE_T i = 0; i < data.Num(); i++) {
            if (a.data[i] != data[i]) {
                return false;
            }
        }
        return true;
    }
	//~Array2D();
};

/*
template <typename ValueType, typename ArrayType>
struct TArray2DMapKeyFuncs :
    public BaseKeyFuncs<
    TPair<Array2D<ArrayType>, ValueType>,
    TArray<ArrayType>
    >
{
private:
    typedef BaseKeyFuncs<
        TPair<Array2D<ArrayType>, ValueType>,
        TArray<ArrayType>
    > Super;
public:
    typedef typename Super::ElementInitType ElementInitType;
    typedef typename Super::KeyInitType     KeyInitType;

    //using FTypeHashOnlyMap = TMap<Array2D<ArrayType>, ValueType, FDefaultSetAllocator,
    //    TArray2DMapKeyFuncs<ValueType, ArrayType>>;

    static KeyInitType GetSetKey(ElementInitType Element)
    {
        return Element.Key.data;
    }

    static bool Matches(KeyInitType A, KeyInitType B)
    {
        return A == B;
    }

    static uint32 GetKeyHash(KeyInitType Key)
    {
        int seed = Key.Num();
        for (const ArrayType& i : Key) {
            seed ^= std::hash<ArrayType>()(i) + (SIZE_T)0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
        //return FCrc::StrCrc32(*Key);
    }
};
*/

FORCEINLINE uint32 GetTypeHash(const Array2D<FLinearColor>& Thing)
{
    SIZE_T seed = Thing.data.Num();
    for (const FLinearColor& i : Thing.data) {
        seed ^= (sizeof FLinearColor) + (SIZE_T)0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

/**
 * Hash function.
 * 
 #if UE_BUILD_DEBUG
uint32 GetTypeHash(const Array2D<FLinearColor>& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const Array2D<FLinearColor>& Thing)
{
    uint32 Hash = FCrc::TypeCrc32(Thing.data);
    return Hash;
}
#endif

namespace std {
    template <typename T> class hash<Array2D<T>> {
    public:
        SIZE_T operator()(const Array2D<T>& a) const noexcept {
            SIZE_T seed = a.data.Num();
            for (const T& i : a.data) {
                seed ^= hash<T>()(i) + (SIZE_T)0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
} // namespace std
 */