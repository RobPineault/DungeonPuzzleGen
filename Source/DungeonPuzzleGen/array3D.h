// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


//typedef FPlatformTypes::SIZE_T SIZE_T;

 /**
  * Represent a 3D array.
  * The 3D array is stored in a single array, to improve cache usage.
  */
template <typename T> class Array3D
{

public:
    /**
     * The dimensions of the 3D array.
     */
    SIZE_T height;
    SIZE_T width;
    SIZE_T depth;

    /**
     * The array containing the data of the 3D array.
     */
    TArray<T> data;

    /**
     * Build a 2D array given its height, width and depth.
     * All the arrays elements are initialized to default value.
     */
    Array3D(SIZE_T height, SIZE_T width, SIZE_T depth) noexcept
        : height(height), width(width), depth(depth)
         {
        data.SetNum(width * height * depth);
    }

    /**
     * Build a 2D array given its height, width and depth.
     * All the arrays elements are initialized to value
     */
    Array3D(SIZE_T height, SIZE_T width, SIZE_T depth,
        T value) noexcept
        : height(height), width(width), depth(depth)
         {
        data.Init(value, width * height * depth);
    }

    /**
     * Return a const reference to the element in the i-th line, j-th column, and
     * k-th depth. i must be lower than height, j lower than width, and k lower
     * than depth.
     */
    const T& get(SIZE_T i, SIZE_T j, SIZE_T k) const noexcept {
        //assert(i < height&& j < width&& k < depth);
        return data[i * width * depth + j * depth + k];
    }

    /**
     * Return a reference to the element in the i-th line, j-th column, and k-th
     * depth. i must be lower than height, j lower than width, and k lower than
     * depth.
     */
    T& get(SIZE_T i, SIZE_T j, SIZE_T k) noexcept {
        return data[i * width * depth + j * depth + k];
    }

    /**
     * Check if two 3D arrays are equals.
     */
    bool operator==(const Array3D& a) const noexcept {
        if (height != a.height || width != a.width || depth != a.depth) {
            return false;
        }

        for (SIZE_T i = 0; i < data.Num(); i++) {
            if (a.data[i] != data[i]) {
                return false;
            }
        }
        return true;
    }
	//~Array3D();
};
