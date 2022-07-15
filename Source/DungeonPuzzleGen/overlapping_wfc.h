// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 

class DUNGEONPUZZLEGEN_API overlapping_wfc
{
public:
	overlapping_wfc();
	~overlapping_wfc();
};
 */

//#include "array2D.h"
#include "wfc.h"
//#include <Runtime/CoreUObject/Public/UObject/ObjectMacros.h>
#include "Containers/Map.h"
#include "overlapping_wfc.generated.h"


USTRUCT(BlueprintType)
/**
 * Options needed to use the overlapping wfc.
 */
struct FOverlappingWFCOptions {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int out_height;  // The height of the output in pixels.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int out_width;   // The width of the output in pixels.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool periodic_input;  // True if the input is toric.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool periodic_output; // True if the output is toric.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int symmetry; // The number of symmetries (the order is defined in wfc).
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool ground;       // True if the ground needs to be set (see init_ground).
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int pattern_size; // The width and height in pixel of the patterns.

    /**
     * Get the wave height given these options.
     */
    int get_wave_height() const noexcept {
        return periodic_output ? out_height : out_height - pattern_size + 1;
    }

    /**
     * Get the wave width given these options.
     */
    int get_wave_width() const noexcept {
        return periodic_output ? out_width : out_width - pattern_size + 1;
    }
};

/**
 * Class generating a new image with the overlapping WFC algorithm.
 */
template <typename T> class OverlappingWFC {

protected:
    /**
     * The input image. T is usually a color.
     */
    Array2D<T> input;

    /**
     * Options needed by the algorithm.
     */
    FOverlappingWFCOptions options;

    /**
     * The array of the different patterns extracted from the input.
     */
    TArray<Array2D<T>> patterns;

    /**
     * The underlying generic WFC algorithm.
     */
    WFC wfc;

    /**
     * Constructor initializing the wfc.
     * This constructor is called by the other constructors.
     * This is necessary in order to initialize wfc only once.
     */
    OverlappingWFC(
        const Array2D<T>& input, const FOverlappingWFCOptions& options,
        const int& seed,
        const TTuple<TArray<Array2D<T>>, TArray<double>>& patterns,
        const TArray<TArray<TArray<unsigned>>>
        & propagator) noexcept
        : input(input), options(options), patterns(patterns.Get<0>()),
        wfc(options.periodic_output, seed, patterns.Get<1>(), propagator,
            options.get_wave_height(), options.get_wave_width()) {
        // If necessary, the ground is set.
        if (options.ground) {
            init_ground(wfc, input, patterns.Get<0>(), options);
        }
    }

    /**
     * Constructor used only to call the other constructor with more computed
     * parameters.
     */
    OverlappingWFC(const Array2D<T>& input, const FOverlappingWFCOptions& options,
        const int& seed,
        const TTuple<TArray<Array2D<T>>, TArray<double>>
        & patterns) noexcept
        : OverlappingWFC(input, options, seed, patterns,
            generate_compatible(patterns.Get<0>())) {}

    /**
     * Init the ground of the output image.
     * The lowest middle pattern is used as a floor (and ceiling when the input is
     * toric) and is placed at the lowest possible pattern position in the output
     * image, on all its width. The pattern cannot be used at any other place in
     * the output image.
     */
    void init_ground(WFC& wfcl, const Array2D<T>& inputl,
        const TArray<Array2D<T>>& patternsl,
        const FOverlappingWFCOptions& optionsl) noexcept {
        unsigned ground_pattern_id =
            get_ground_pattern_id(inputl, patternsl, optionsl);

        // Place the pattern in the ground.
        for (int j = 0; j < optionsl.get_wave_width(); j++) {
            set_pattern(ground_pattern_id, optionsl.get_wave_height() - 1, j);
        }

        // Remove the pattern from the other positions.
        for (int i = 0; i < optionsl.get_wave_height() - 1; i++) {
            for (int j = 0; j < optionsl.get_wave_width(); j++) {
                wfcl.remove_wave_pattern(i, j, ground_pattern_id);
            }
        }

        // Propagate the information with wfc.
        wfcl.propagate();
    }

    /**
     * Return the id of the lowest middle pattern.
     */
    static unsigned
        get_ground_pattern_id(const Array2D<T>& input,
            const TArray<Array2D<T>>& patterns,
            const FOverlappingWFCOptions& options) noexcept {
        // Get the pattern.
        Array2D<T> ground_pattern =
            input.get_sub_array(input.height - 1, input.width / 2,
                options.pattern_size, options.pattern_size);

        // Retrieve the id of the pattern.
        for (int i = 0; i < patterns.Num(); i++) {
            if (ground_pattern == patterns[i]) {
                return i;
            }
        }

        // The pattern exists.
        //assert(false);
        return 0;
    }

    /**
     * Return the list of patterns, as well as their probabilities of apparition.
     */
    static TTuple<TArray<Array2D<T>>, TArray<double>>
        get_patterns(const Array2D<T>& input,
            const FOverlappingWFCOptions& options) noexcept {
        TMap<Array2D<T>, unsigned> patterns_id;
        //, FDefaultSetAllocator,TArray2DMapKeyFuncs<unsigned, T>
        TArray<Array2D<T>> patterns;

        // The number of time a pattern is seen in the input image.
        TArray<double> patterns_weight;

        //patterns_weight.SetNum();

        TArray<Array2D<T>> symmetries;
        //TArray<TArray<T>> symmetries;
        //TMap<int, Array2D<T>> symmetries;
        //symmetries.SetNum(8);

        symmetries.Init(Array2D<T>(options.pattern_size, options.pattern_size), 8);
        //for (int i = 0; i < 8; i++) {
            //symmetries.Emplace(Array2D<T>(options.pattern_size, options.pattern_size));
        //}
        
        //UE_LOG(LogTemp, Warning, TEXT("symmetries #: %d"), symmetries.Num());
        unsigned max_i = options.periodic_input
            ? input.height
            : input.height - options.pattern_size + 1;
        unsigned max_j = options.periodic_input
            ? input.width
            : input.width - options.pattern_size + 1;

        for (unsigned i = 0; i < max_i; i++) {
            for (unsigned j = 0; j < max_j; j++) {
                // Compute the symmetries of every pattern in the image.
                /*
                symmetries[0] = input.get_sub_array(i, j, options.pattern_size, options.pattern_size);
                symmetries[1] = symmetries[0].reflected();
                symmetries[2] = symmetries[0].rotated();
                symmetries[3] = symmetries[2].reflected();
                symmetries[4] = symmetries[2].rotated();
                symmetries[5] = symmetries[4].reflected();
                symmetries[6] = symmetries[4].rotated();
                symmetries[7] = symmetries[6].reflected();
                
                
                symmetries[0] = Array2D<T>(options.pattern_size, options.pattern_size, input
                    .get_sub_array(i, j, options.pattern_size, options.pattern_size).data);
                symmetries[1] = Array2D<T>(options.pattern_size, options.pattern_size, symmetries[0].reflected().data);
                symmetries[2] = Array2D<T>(options.pattern_size, options.pattern_size, symmetries[0].rotated().data);
                symmetries[3] = Array2D<T>(options.pattern_size, options.pattern_size, symmetries[2].reflected().data);
                symmetries[4] = Array2D<T>(options.pattern_size, options.pattern_size, symmetries[2].rotated().data);
                symmetries[5] = Array2D<T>(options.pattern_size, options.pattern_size, symmetries[4].reflected().data);
                symmetries[6] = Array2D<T>(options.pattern_size, options.pattern_size, symmetries[4].rotated().data);
                symmetries[7] = Array2D<T>(options.pattern_size, options.pattern_size, symmetries[6].reflected().data);

                */

                symmetries[0].data = input.get_sub_array(i, j, options.pattern_size, options.pattern_size).data;
                symmetries[1].data = symmetries[0].reflected().data;
                symmetries[2].data = symmetries[0].rotated().data;
                symmetries[3].data = symmetries[2].reflected().data;
                symmetries[4].data = symmetries[2].rotated().data;
                symmetries[5].data = symmetries[4].reflected().data;
                symmetries[6].data = symmetries[4].rotated().data;
                symmetries[7].data = symmetries[6].reflected().data;
                

                // The number of symmetries in the option class define which symetries
                // will be used.
                for (int k = 0; k < options.symmetry; k++) {
                    //auto res = 
                    unsigned* pid = nullptr;
                    if (patterns_id.Num() > 0) {
                        pid = patterns_id.Find(symmetries[k]); //patterns_id.Contains(symmetries[k]);
                        //unsigned& Ref7 = patterns_id.FindOrAdd(symmetries[k]);
                        // If the pattern already exist, we just have to increase its number
                        // of appearance.
                    }
                    if (pid != nullptr) {
                        if(patterns_weight.Num() > (int)*pid)
                            patterns_weight[*pid] += 1;
                    }
                    else {
                        patterns_id.Add(symmetries[k], patterns.Num());

                        patterns.Add(symmetries[k]);
                        patterns_weight.Add(1);
                    }
                }
            }
        }

        return MakeTuple(patterns, patterns_weight);
    }

    /**
     * Return true if the pattern1 is compatible with pattern2
     * when pattern2 is at a distance (dy,dx) from pattern1.
     */
    static bool agrees(const Array2D<T>& pattern1, const Array2D<T>& pattern2,
        int dy, int dx) noexcept {
        unsigned xmin = dx < 0 ? 0 : dx;
        unsigned xmax = dx < 0 ? dx + pattern2.width : pattern1.width;
        unsigned ymin = dy < 0 ? 0 : dy;
        unsigned ymax = dy < 0 ? dy + pattern2.height : pattern1.width;

        // Iterate on every pixel contained in the intersection of the two pattern.
        for (unsigned y = ymin; y < ymax; y++) {
            for (unsigned x = xmin; x < xmax; x++) {
                // Check if the color is the same in the two patterns in that pixel.
                if (pattern1.get(y, x) != pattern2.get(y - dy, x - dx)) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Precompute the function agrees(pattern1, pattern2, dy, dx).
     * If agrees(pattern1, pattern2, dy, dx), then compatible[pattern1][direction]
     * contains pattern2, where direction is the direction defined by (dy, dx)
     * (see direction.hpp).
     */
    static TArray<TArray<TArray<unsigned>>>
        generate_compatible(const TArray<Array2D<T>>& patterns) noexcept {
        TArray<TArray<TArray<unsigned>>> compatible;
        TArray<TArray<unsigned>> inner;
        inner.SetNum(4);
        compatible.Init(inner, patterns.Num());

        //TArray<TArray<TArray<unsigned>>>(patterns.size());

        // Iterate on every dy, dx, pattern1 and pattern2
        for (int pattern1 = 0; pattern1 < patterns.Num(); pattern1++) {
            for (unsigned direction = 0; direction < 4; direction++) {
                for (int pattern2 = 0; pattern2 < patterns.Num(); pattern2++) {
                    if (agrees(patterns[pattern1], patterns[pattern2],
                        directions_y[direction], directions_x[direction])) {
                        compatible[pattern1][direction].Add(pattern2);
                    }
                }
            }
        }

        return compatible;
    }

    /**
     * Transform a 2D array containing the patterns id to a 2D array containing
     * the pixels.
     */
    Array2D<T> to_image(const Array2D<unsigned>& output_patterns) const noexcept {
        Array2D<T> output = Array2D<T>(options.out_height, options.out_width);

        if (options.periodic_output) {
            for (int y = 0; y < options.get_wave_height(); y++) {
                for (int x = 0; x < options.get_wave_width(); x++) {
                    output.get(y, x) = patterns[output_patterns.get(y, x)].get(0, 0);
                }
            }
        }
        else {
            for (int y = 0; y < options.get_wave_height(); y++) {
                for (int x = 0; x < options.get_wave_width(); x++) {
                    output.get(y, x) = patterns[output_patterns.get(y, x)].get(0, 0);
                }
            }
            for (int y = 0; y < options.get_wave_height(); y++) {
                const Array2D<T>& pattern =
                    patterns[output_patterns.get(y, options.get_wave_width() - 1)];
                for (int dx = 1; dx < options.pattern_size; dx++) {
                    output.get(y, options.get_wave_width() - 1 + dx) = pattern.get(0, dx);
                }
            }
            for (int x = 0; x < options.get_wave_width(); x++) {
                const Array2D<T>& pattern =
                    patterns[output_patterns.get(options.get_wave_height() - 1, x)];
                for (int dy = 1; dy < options.pattern_size; dy++) {
                    output.get(options.get_wave_height() - 1 + dy, x) =
                        pattern.get(dy, 0);
                }
            }
            const Array2D<T>& pattern = patterns[output_patterns.get(
                options.get_wave_height() - 1, options.get_wave_width() - 1)];
            for (int dy = 1; dy < options.pattern_size; dy++) {
                for (int dx = 1; dx < options.pattern_size; dx++) {
                    output.get(options.get_wave_height() - 1 + dy,
                        options.get_wave_width() - 1 + dx) = pattern.get(dy, dx);
                }
            }
        }
        output.complete = true;
        return output;
    }

    TOptional<unsigned> get_pattern_id(const Array2D<T>& pattern) {
        unsigned* pattern_id = std::find(patterns.begin(), patterns.end(), pattern);

        if (pattern_id != patterns.end()) {
            return *pattern_id;
        }

        return NULL;
    }

    /**
     * Set the pattern at a specific position, given its pattern id
     * pattern_id needs to be a valid pattern id, and i and j needs to be in the wave range
     */
    void set_pattern(unsigned pattern_id, unsigned i, unsigned j) noexcept {
        for (int p = 0; p < patterns.Num(); p++) {
            if (pattern_id != p) {
                wfc.remove_wave_pattern(i, j, p);
            }
        }
    }

public:
    /**
     * The constructor used by the user.
     */
    OverlappingWFC(const Array2D<T>& input, const FOverlappingWFCOptions& options,
        int seed) noexcept
        : OverlappingWFC(input, options, seed, get_patterns(input, options)) {}

    /**
     * Set the pattern at a specific position.
     * Returns false if the given pattern does not exist, or if the
     * coordinates are not in the wave
     */
    bool set_pattern(const Array2D<T>& pattern, unsigned i, unsigned j) noexcept {
        auto pattern_id = get_pattern_id(pattern);

        if (pattern_id == NULL || i >= options.get_wave_height() || j >= options.get_wave_width()) {
            return false;
        }

        set_pattern(pattern_id, i, j);
        return true;
    }

    /**
     * Run the WFC algorithm, and return the result if the algorithm succeeded.
     */
    Array2D<T> run() noexcept {
        Array2D<unsigned> result = wfc.run();
        if (result.complete) {
            return to_image(result);
        }
        Array2D<T> failed(1, 1);
        return failed;
    }
};
