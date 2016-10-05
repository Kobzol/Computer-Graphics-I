#pragma once

class Light
{
public:
	Light(Vector3 position);

	Vector3 getPosition();

private:
	Vector3 position;
};