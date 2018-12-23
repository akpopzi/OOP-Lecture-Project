#ifndef _IMAGE_PROCESS_H
#define _IMAGE_PROCESS_H
#include <opencv2/core/mat.hpp>
#include <deque>
#include "ImageConverter.h"


/*
 * ���ʹ��ImageProcess��ImageConverter��
 * ImageProcess.h������cv::Mat��ʵ�֣����ǰ�˲���Ҫ׼��OpenCV�������ɰ���ͷ�ļ�
 * ǰ�˵���ʱ���������н�ImageProcess.cpp�е�ʵ���޸�Ϊ����ҪOpenCV��
 * ����QImageToMatת����������QImage���κ�ת����ֻ�Ƿ���QImage��ָ�룻MatToQImageҲֻ�ǽ���ָ��
 * ��ǰ��˺ϲ�ʱ���ٰ�ImageProcess.cpp��Ϊ��ǰ�������õ�ʵ�֡�
 * ����ImageProcess.cpp������Qtͷ�ļ�����˺�˲���Ҫӵ��Qt������
 *
 * ImageConvertor.hͬ��������cv::Mat��ʵ��
 * ����ǰ�˵���ʱ����OpenCV����ʹ�á�
 * ���û��ʹ�����������ͷ�ļ���ֱ��ʹ����Qt�ࡣ
 */


class TraceStack
{
private:
	enum { MAX_TRACES = 5 };
	std::vector<MAT> traces;
public:
	TraceStack() = default;
	void push(MAT Mat);
	void pop()
	{
		traces.pop_back();
	}
	MAT top() const;
};

class ImageProcess
{
	struct ImagePartial
	{
		MAT src;
		cv::Rect part;
	};
	static cv::Mat& parseMAT(MAT Mat);
	static MAT packMAT(cv::Mat mat);
public:

	//TODO:֧�ֶ��ѡ������
	std::vector<ImagePartial> SelectedParts;
	//TODO:ͼ��
	std::deque<MAT> Layers;
	//����MAX_TRACES��mat,���ڳ���
	TraceStack Traces;


	static MAT GaussianBlur(ImageProcess &process, MAT Mat, double strength);
};

#endif