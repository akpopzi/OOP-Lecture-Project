#ifndef _IMAGE_PROCESS_H
#define _IMAGE_PROCESS_H
#include <opencv2/core/mat.hpp>
#include <deque>
#include "ImageConverter.h"



class Layer
{
private:
	MAT value;
	unsigned ID;
	//�������ϵ�����µ���ͼ��Ĵ�С.ͼ���СֻӰ�����ʱ�Ĵ�С,��Ӱ���ͼ���value�ľ���ߴ�
	std::pair<int, int> topLeftPoint, bottomRightPoint;

	static int ID_dispatcher;
public:
	Layer(const MAT value) :value(value), ID(++ID_dispatcher), topLeftPoint({ 0,0 }), bottomRightPoint({ 0,0 }) { }
	Layer(const MAT value, int leftUpX, int leftUpY, int rightDownX, int rightDownY)
		:value(value), ID(++ID_dispatcher), topLeftPoint({ leftUpX,leftUpY }), bottomRightPoint({ rightDownX,rightDownY }) {}
	unsigned getID() const { return ID; }
	MAT getMat() { return value; }
	void setMat(const MAT value) { this->value = value; }
	std::pair<int, int> getLeftUpPoint() { return topLeftPoint; }
	std::pair<int, int> getRightDownPoint() { return bottomRightPoint; }
	void setTopLeftPoint(int x, int y) { topLeftPoint.first = x; topLeftPoint.second = y; }
	void setBottomRightPoint(int x, int y) { bottomRightPoint.first = x; bottomRightPoint.second = y; }

};

struct Trace
{
	MAT traceValue;
	unsigned traceLayerID;
	Trace(const MAT value, const unsigned layerID) :traceValue(value), traceLayerID(layerID) {}
};

class TraceStack
{
private:
	enum { MAX_TRACES = 5 };
	std::vector<Trace> traces;
public:
	TraceStack() = default;
	void push(MAT changedMat, unsigned layerID);
	void pop()
	{
		traces.pop_back();
	}
	Trace top();
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


	//����MAX_TRACES��mat,���ڳ���
	TraceStack Traces;
public:

	//TODO:֧�ֶ��ѡ������
	std::vector<ImagePartial> SelectedParts;

	//TODO:ͼ��
	std::deque<Layer> Layers;

	//������һ�ε��޸�
	void revertChange();

	//��һ��MAT����Ϊ��process�Ķ���ͼ��,��ͼ�㽫������ͼ����Ϸ�
	void loadImageAsTopLayer(MAT mat);
	//��һ��MAT����Ϊ��process�ĵײ�ͼ��,��ͼ�㽫������ͼ����·�
	void loadImageAsBottomLayer(MAT mat);

	void deleteLayer(Layer &layer);
	void deleteLayer(unsigned layerID);
	void moveLayerUp(Layer &layer);
	void moveLayerUp(unsigned layerID);
	void moveLayerDown(Layer &layer);
	void moveLayerDown(unsigned layerID);

	static void GaussianBlur(ImageProcess &process, Layer &layer, double strength);
	static void Sculpture(ImageProcess &process, Layer &layer);
};

#endif