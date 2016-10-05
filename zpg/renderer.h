#pragma once

class Renderer
{
public:
	void render(Scene& scene, Camera& camera, Shader& shader, cv::Mat& buffer);
};