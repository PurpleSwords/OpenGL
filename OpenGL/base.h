#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//顶点着色器
extern const char *vertexShaderSource;

//片段着色器
extern const char *fragmentShaderSource;

void framebuffer_size_callback(GLFWwindow* window, int width, int heigh);

void processInput(GLFWwindow* window);