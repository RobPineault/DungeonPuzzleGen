// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
//#include "direction.h"
#include "array3D.h"
//#include <vector>
//#include <array>
#include "wave.h"

//class Wave;

constexpr int directions_x[4] = { 0, -1, 1, 0 };
constexpr int directions_y[4] = { -1, 0, 0, 1 };

/**
 * Return the opposite direction of direction.
 */
constexpr unsigned get_opposite_direction(unsigned direction) noexcept {
    return 3 - direction;
}
/**
 * Propagate information about patterns in the wave.
 */
class DUNGEONPUZZLEGEN_API Propagator
{
public:
    using PropagatorState = TArray<TArray<TArray<unsigned>>>;

private:
    /**
     * The size of the patterns.
     */
    const SIZE_T patterns_size;

    /**
     * propagator[pattern1][direction] contains all the patterns that can
     * be placed in next to pattern1 in the direction direction.
     */
    PropagatorState propagator_state;

    /**
     * The wave width and height.
     */
    const unsigned wave_width;
    const unsigned wave_height;

    /**
     * True if the wave and the output is toric.
     */
    const bool periodic_output;

    /**
     * All the tuples (y, x, pattern) that should be propagated.
     * The tuple should be propagated when wave.get(y, x, pattern) is set to
     * false.
     */
    TArray<TTuple<unsigned, unsigned, unsigned>> propagating;

    /**
     * compatible.get(y, x, pattern)[direction] contains the number of patterns
     * present in the wave that can be placed in the cell next to (y,x) in the
     * opposite direction of direction without being in contradiction with pattern
     * placed in (y,x). If wave.get(y, x, pattern) is set to false, then
     * compatible.get(y, x, pattern) has every element negative or null
     */
    Array3D<TArray<unsigned>> compatible;

    /**
     * Initialize compatible.
     */
    void init_compatible() noexcept;
public:
    /**
     * Constructor building the propagator and initializing compatible.
     */
    Propagator(unsigned wave_height, unsigned wave_width, bool periodic_output,
        PropagatorState propagator_state) noexcept
        : patterns_size(propagator_state.Num()),
        propagator_state(propagator_state), wave_width(wave_width),
        wave_height(wave_height), periodic_output(periodic_output),
        compatible(wave_height, wave_width, patterns_size) {
        init_compatible();
    }

    /**
     * Add an element to the propagator.
     * This function is called when wave.get(y, x, pattern) is set to false.
     */
    void add_to_propagator(unsigned y, unsigned x, unsigned pattern) noexcept {
        // All the direction are set to 0, since the pattern cannot be set in (y,x).
        TArray<unsigned> temp = {};
        compatible.get(y, x, pattern) = temp;
        propagating.Emplace(y, x, pattern);
    }

    /**
     * Propagate the information given with add_to_propagator.
     */
    void propagate(Wave& wave) noexcept;
};
