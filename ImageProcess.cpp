#include "ImageProcess.h"
#include <opencv2/opencv.hpp>
#include "Filter.h"

void TraceStack::push(MAT Mat)
{
	traces.push_back(Mat);
	if (traces.size() > MAX_TRACES)
		traces.erase(traces.begin());
}

MAT TraceStack::top()
{
		return *(--traces.end());
}


cv::Mat& ImageProcess::parseMAT(MAT Mat)
{
	return *Mat.mat;
}

MAT ImageProcess::packMAT(cv::Mat mat)
{
	return { std::make_shared<cv::Mat>(mat) };
}


void ImageProcess::loadImage(MAT Mat)
{
	Traces.push(Mat);
	//Layers.push_back(Mat);
}


//strength [0,1]
MAT ImageProcess::GaussianBlur(ImageProcess &process, MAT Mat, double strength)
{
	auto mat = parseMAT(Mat);
	cv::Mat dst;
	//��strength�����Բ�ֵ
	int size = 3 + strength * (std::min(mat.rows, mat.cols) / 10.0 - 3);
	//kernel�����������ߴ�
	if (size % 2 == 0) ++size;
	cv::Size kernel_size(size, size);

	//�Լ���д�Ĵ������Ĳ����б�Ҫ����cv���к�����д��(����GaussianBlur)
	cv::GaussianBlur(mat, dst, kernel_size, 0);
	
	MAT DST = packMAT(dst);
	//����ɴ����mat����traces��,���ڳ�����һ���޸ĵȹ���
	process.Traces.push(DST);
	return DST;
}

MAT ImageProcess::Sculpture(ImageProcess& process, MAT Mat)
{
	auto dst = SculptureFilter(parseMAT(Mat));
	auto DST = packMAT(dst);
	process.Traces.push(DST);
	return DST;
}
