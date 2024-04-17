#pragma once

#define N_COLUMNS	3
#define N_ROWS		5

enum ALPHA_INDEX {
	S = 0,
	T,
	A,
	R,

	CNT_S = 11,
	CNT_T = 7,
	CNT_A = 10,
	CNT_R = 10
};

float fAlphabet[4][5][3]{
	{
		// S
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	},
	{
		// T
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	},
	{
		// A
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
	},
	{
		// R
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
	}
};