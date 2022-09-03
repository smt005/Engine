#pragma once

namespace Engine
{

enum class VirtualKey
{
	VK_0 = 48,
	VK_1 = 49,
	VK_2 = 50,
	VK_3 = 51,
	VK_4 = 52,
	VK_5 = 53,
	VK_6 = 54,
	VK_7 = 55,
	VK_8 = 56,
	VK_9 = 57,
	A = 65,
	B =	66,
	C = 67,
	D =	68,
	E =	69,
	F =	70,
	G =	71,
	H =	72,
	I =	73,
	J =	74,
	K =	75,
	L =	76,
	M =	77,
	N =	78,
	O =	79,
	P =	80,
	Q =	81,
	R =	82,
	S =	83,
	T =	84,
	U =	85,
	V =	86,
	W =	87,
	X =	88,
	Y =	89,
	Z =	90,

	TILDE =	96,

	SPACE	= 32,
	ESCAPE	= 256,
	RETURN	= 259,

	RIGHT	= 262,
	LEFT	= 263,
	DOWN	= 264,
	UP		= 265,

	SHIFT	= 340,
	CONTROL	= 341
};


enum class VirtualTap
{
	LEFT = 0,
	RIGHT = 1,
	MIDDLE = 2,
	SCROLL_UP = 3,
	SCROLL_BOTTOM = 4
};

};	// Engine