#version 330 core
out vec4 FragColor;

//in vec3 ourColor;
in vec3 ourPosition;

void main()
{
	FragColor = vec4(ourPosition, 1.0f);
}
/*
    ��ɫΪ�ڣ���ɫ��СΪ0.0f����ɫ��������Ϊ��ʱĬ��Ϊ0�����Ϊ��
    ����ɫ���Ϸ������ʱ�������-y��
*/