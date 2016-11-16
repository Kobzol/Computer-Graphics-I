#include "stdafx.h"

Tracer::Tracer(int samplesPerPixel): samplesPerPixel(samplesPerPixel)
{

}

Tracer::~Tracer()
{

}

int Tracer::getSamplesPerPixel() const
{
	return this->samplesPerPixel;
}
