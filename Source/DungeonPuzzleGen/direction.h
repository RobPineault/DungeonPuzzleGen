// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"




class DUNGEONPUZZLEGEN_API direction
{
public:
	/**
 * A direction is represented by an unsigned integer in the range [0; 3].
 * The x and y values of the direction can be retrieved in these tables.
 */
	const int directions4_x[4] = { 0, 1, 0, -1 };
	const int directions4_y[4] = { -1, 0, 1, 0 };
	const int directions_x[8] = { 0,1,1,1,0,-1,-1,-1 };
	const int directions_y[8] = { -1,-1,0,1,1,1,0,-1 };


	/**
	 * Return the opposite direction of direction.
	 */
	//constexpr unsigned get_opposite_direction(unsigned direction) noexcept {
	//	return 3 - direction;
	//}
	//constexpr unsigned get_opposite_direction(unsigned direction) noexcept {
	//	return 3 - direction;
	//}

	direction();
	~direction();
};
 
