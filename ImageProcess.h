#ifndef _IMAGE_PROCESS_H
#define _IMAGE_PROCESS_H
#include <deque>
#include <vector>
#include "MAT.h"



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

class LayerStorage
{
private:
	std::deque<Layer> layers;
	int layerLevel = 0;
public:
	LayerStorage() = default;
	void addLayerAsTop(MAT Mat);
	void addLayerAsTop(Layer &layer);
	void addLayerAsBottom(MAT Mat);
	void addLayerAsBottom(Layer &layer);
	void addLayerAfter(MAT Mat, int index);
	void addLayerAfter(MAT Mat, Layer &layer);
	void deleteLayer(Layer &layer);
	void deleteLayer(unsigned layerID);
	void moveLayerUp(Layer &layer);
	void moveLayerUp(int index);
	void moveLayerUpByID(unsigned layerID);
	void moveLayerDown(Layer &layer);
	void moveLayerDown(int index);
	void moveLayerDownByID(unsigned layerID);

	Layer& front() { return layers.front(); }
	Layer& back() { return layers.back(); }

	auto begin()->std::deque<Layer>::iterator { return layers.begin(); }
	auto end()->std::deque<Layer>::iterator { return layers.end(); }
	auto rbegin()->std::deque<Layer>::reverse_iterator { return layers.rbegin(); }
	auto rend()->std::deque<Layer>::reverse_iterator { return layers.rend(); }

	Layer& operator[](int i);
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
	static cv::Mat& parseMAT(MAT Mat);
	static MAT packMAT(cv::Mat mat);


	//����MAX_TRACES��mat,���ڳ���
	TraceStack Traces;
public:

	//TODO:֧�ֶ��ѡ������
	//std::vector<ImagePartial> SelectedParts;

	LayerStorage Layers;

	//������һ�ε��޸�
	void revertChange();



	static void GaussianBlur(ImageProcess &process, Layer &layer, double strength);
	static void Sculpture(ImageProcess &process, Layer &layer);
};

#endif