// Fill out your copyright notice in the Description page of Project Settings.


#include "propagator.h"
//#include "wave.h"
/*
Propagator::~Propagator()
{
}
*/


void Propagator::init_compatible() noexcept {
    TArray<unsigned> value;
    value.SetNum(4);
    // We compute the number of pattern compatible in all directions.
    for (unsigned y = 0; y < wave_height; y++) {
        for (unsigned x = 0; x < wave_width; x++) {
            for (unsigned pattern = 0; pattern < patterns_size; pattern++) {
                for (int direction = 0; direction < 4; direction++) {
                    value[direction] =
                        static_cast<unsigned>(propagator_state[pattern][get_opposite_direction(direction)]
                            .Num());
                }
                compatible.get(y, x, pattern) = value;
            }
        }
    }
}

void Propagator::propagate(Wave &wave) noexcept {

    // We propagate every element while there is element to propagate.
    while (propagating.Num() != 0) {


        unsigned y1, x1, pattern;
        //tie(y1, x1, pattern) = propagating.Last();

        TTuple<unsigned, unsigned, unsigned> t;

        // The cell and pattern that has been set to false.
        t = propagating.Last();
        y1 = t.Get<0>();
        x1 = t.Get<1>();
        pattern = t.Get<2>();

        propagating.RemoveAt(propagating.Num() - 1);

        // We propagate the information in all 4 directions.
        for (unsigned direction = 0; direction < 4; direction++) {

            // We get the next cell in the direction direction.
            int dx = directions_x[direction];
            int dy = directions_y[direction];
            int x2, y2;
            if (periodic_output) {
                x2 = ((int)x1 + dx + (int)wave.width) % wave.width;
                y2 = ((int)y1 + dy + (int)wave.height) % wave.height;
            }
            else {
                x2 = x1 + dx;
                y2 = y1 + dy;
                if (x2 < 0 || x2 >= (int)wave.width) {
                    continue;
                }
                if (y2 < 0 || y2 >= (int)wave.height) {
                    continue;
                }
            }

            // The index of the second cell, and the patterns compatible
            unsigned i2 = x2 + y2 * wave.width;
            const TArray<unsigned>& patterns =
                propagator_state[pattern][direction];

            // For every pattern that could be placed in that cell without being in
            // contradiction with pattern1
            for (auto it = patterns.CreateConstIterator(); it.GetIndex() < patterns.Num(); ++it) {
                
                //.GetIndex() != patterns.Num()
                unsigned el = patterns[it.GetIndex()];
                // We decrease the number of compatible patterns in the opposite
                // direction If the pattern was discarded from the wave, the element
                // is still negative, which is not a problem
                TArray<unsigned>& value = compatible.get(y2, x2, el);
                if(value.Num() < 4)
                    value.SetNum(4);
                value[direction]--;

                // If the element was set to 0 with this operation, we need to remove
                // the pattern from the wave, and propagate the information
                if (value[direction] == 0) {
                    add_to_propagator(y2, x2, el);
                    wave.set(i2, *it, false);
                }
            }
        }
    }
}