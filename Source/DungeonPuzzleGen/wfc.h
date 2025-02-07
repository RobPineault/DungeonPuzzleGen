// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
//#include "Core.h"

//#include <random>

#include "array2D.h"
#include "propagator.h"
//#include "wave.h"
//#include "Containers/Array.h"

//class Wave;
//typedef FPlatformTypes::SIZE_T SIZE_T;
/**
 * Class containing the generic WFC algorithm.
 */
class DUNGEONPUZZLEGEN_API WFC
{
private:
    /**
     * The random number generator.
     */
    FRandomStream gen;

    /**
     * The distribution of the patterns as given in input.
     */
    const TArray<double> patterns_frequencies;

    /**
     * The wave, indicating which patterns can be put in which cell.
     */
    Wave wave;

    /**
     * The number of distinct patterns.
     */
    const SIZE_T nb_patterns;

    /**
     * The propagator, used to propagate the information in the wave.
     */
    Propagator propagator;

    /**
     * Transform the wave to a valid output (a 2d array of patterns that aren't in
     * contradiction). This function should be used only when all cell of the wave
     * are defined.
     */
    Array2D<unsigned> wave_to_output() const noexcept;

public:
    /**
     * Basic constructor initializing the algorithm.
     */
    WFC(bool periodic_output, int seed, TArray<double> patterns_frequencies,
        Propagator::PropagatorState propagator, unsigned wave_height,
        unsigned wave_width)
        noexcept;

    /**
     * Run the algorithm, and return a result if it succeeded.
     */
    Array2D<unsigned> run() noexcept;

    /**
     * Return value of observe.
     */
    enum ObserveStatus {
        success,    // WFC has finished and has succeeded.
        failure,    // WFC has finished and failed.
        to_continue // WFC isn't finished.
    };

    /**
     * Define the value of the cell with lowest entropy.
     */
    ObserveStatus observe() noexcept;

    /**
     * Propagate the information of the wave.
     */
    void propagate() noexcept { propagator.propagate(wave); }

    /**
     * Remove pattern from cell (i,j).
     */
    void remove_wave_pattern(unsigned i, unsigned j, unsigned pattern) noexcept {
        if (wave.get(i, j, pattern)) {
            wave.set(i, j, pattern, false);
            propagator.add_to_propagator(i, j, pattern);
        }
    }
	//~WFC();
};



