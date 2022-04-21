#ifndef TEXTURE_H
#define TEXTURE_H

class texture
{
public:
	virtual vec3 value(double u, double v)const = 0;
};


class image_texture :public texture
{
public:
	unsigned char* data;//��main��ͨ��stbi_load�������ص�ͼ������
	int nx, ny;

public:
	image_texture() {}
	image_texture(unsigned char* pixels, int w, int h) :data(pixels), nx(w), ny(h) {}//���캯���и�ֵdata
	~image_texture()
	{
		delete data;
	}

	//ÿ�����ص�uv���꣬�������ͼ��Ӧλ������[0,1] vec3ֵ
	virtual vec3 value(double u, double v)const
	{
		if (data == nullptr)
			return vec3(1, 1, 0);

		int i = static_cast<int>((u)* nx); // ����һ�е�ĳ����
		int j = static_cast<int>((1 - v) * ny - 0.001);//main�д��ϵ��£�v��uv�����1��ʼ��0����j��0��ʼ���㣬����data���ݴ�һ����������Ͽ�ʼ

		//clampһ��
		if (i < 0)i = 0;
		if (j < 0)j = 0;
		if (i > nx - 1)i = nx - 1;
		if (j > ny - 1)j = ny - 1;

		auto rr = data[3 * i + 3 * nx * j + 0];
		auto rri = static_cast<int>(data[3 * i + 3 * nx * j + 0]);
		double r = static_cast<int>(data[3 * i + 3 * nx * j + 0]) / 255.0;//3*i��ʾһ��������3��ռλ��3*nx*j��ʾ����һ�У�ÿ��3��nx
		double g = static_cast<int>(data[3 * i + 3 * nx * j + 1]) / 255.0;//����255��ʾrgbΪ[0,1]
		double b = static_cast<int>(data[3 * i + 3 * nx * j + 2]) / 255.0;

		return vec3(r, g, b);
	}
};

#endif // !TEXTURE_H

