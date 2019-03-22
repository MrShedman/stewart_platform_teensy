#include "filter.h"

namespace
{
    const float TWO_PI = 6.28318530718;
}

float nullFilterApply(void *filter, float input)
{
	return input;
}

void pt1FilterInit(pt1Filter_t *filter, float f_cut, float dT)
{
	filter->RC = 1.0f / (TWO_PI * f_cut);
	filter->dT = dT;
	filter->k = filter->dT / (filter->RC + filter->dT);
}

float pt1FilterApply(pt1Filter_t *filter, float input)
{
	filter->state = filter->state + filter->k * (input - filter->state);
	return filter->state;
}

float pt1FilterApply4(pt1Filter_t *filter, float input, float f_cut, float dT)
{
	if (!filter->RC) 
    {
		filter->RC = 1.0f / (TWO_PI * f_cut);
		filter->dT = dT;
		filter->k = filter->dT / (filter->RC + filter->dT);
	}

	filter->state = filter->state + filter->k * (input - filter->state);

	return filter->state;
}