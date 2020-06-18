#version 330 core
out vec4 FragColor;

//in vec3 ourColor;
in vec3 ourPosition;

void main()
{
	FragColor = vec4(ourPosition, 1.0f);
}
/*
    颜色为黑：颜色最小为0.0f（黑色），坐标为负时默认为0，因此为黑
    （黑色在上方是输出时输入的是-y）
*/