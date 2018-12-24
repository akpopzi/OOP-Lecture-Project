#include "ImageProcess.h"
#include <opencv2/opencv.hpp>
#include "Filter.h"

int Layer::ID_dispatcher = 0;

void TraceStack::push(MAT changedMat, unsigned layerID)
{
	Trace t{ changedMat, layerID };
	traces.push_back(t);
	if (traces.size() > MAX_TRACES)
		traces.erase(traces.begin());
}

Trace TraceStack::top()
{
	return traces.back();
}


cv::Mat& ImageProcess::parseMAT(MAT Mat)
{
	return *Mat.mat;
}

MAT ImageProcess::packMAT(cv::Mat mat)
{
	return { std::make_shared<cv::Mat>(mat) };
}

void ImageProcess::revertChange()
{
	auto t = Traces.top();
	std::for_each(Layers.begin(), Layers.end(),
		[t](Layer &layer) { if (t.traceLayerID == layer.getID()) layer.setMat(t.traceValue); });
	//TODO ʵ�ָֻ���ɾ����ͼ��
}


void ImageProcess::loadImageAsTopLayer(MAT Mat)
{
	Layer newLayer{ Mat };
	Layers.push_back(newLayer);
}

void ImageProcess::loadImageAsBottomLayer(MAT Mat)
{
	Layer newLayer{ Mat };
	Layers.push_front(newLayer);
}

void ImageProcess::deleteLayer(Layer& layer)
{
	deleteLayer(layer.getID());
}

void ImageProcess::deleteLayer(unsigned layerID)
{
	auto it = std::find_if(Layers.begin(), Layers.end(),
		[layerID](Layer &L) {return L.getID() == layerID; });
	if (it == Layers.end())
		throw std::runtime_error("Delete layer failed. Layer ID =" + std::to_string(layerID));
	Traces.push(it->getMat(), it->getID());
	Layers.erase(it);
}


void ImageProcess::moveLayerUp(Layer& layer)
{
	moveLayerUp(layer.getID());
}

void ImageProcess::moveLayerUp(unsigned layerID)
{
	auto it = std::find_if(Layers.begin(), Layers.end(),
		[layerID](Layer &L) {return L.getID() == layerID; });
	if (it == Layers.end()-1)
		return;
	auto itafter = it + 1;
	std::swap(*it, *itafter);
}

void ImageProcess::moveLayerDown(Layer& layer)
{
	moveLayerDown(layer.getID());
}

void ImageProcess::moveLayerDown(unsigned layerID)
{
	auto it = std::find_if(Layers.begin(), Layers.end(),
		[layerID](Layer &L) {return L.getID() == layerID; });
	if (it == Layers.begin())
		return;
	auto itafter = it - 1;
	std::swap(*it, *itafter);
}



void ImageProcess::GaussianBlur(ImageProcess &process, Layer &layer, double strength)
{
	auto mat = parseMAT(layer.getMat());

	//�޸�ǰ�洢��ͼ���ԭͼ��,��Ϊ���������ı���
	process.Traces.push(layer.getMat(), layer.getID());

	cv::Mat dst;
	//��strength�����Բ�ֵ
	int size = 3 + strength * (std::min(mat.rows, mat.cols) / 10.0 - 3);
	//kernel�����������ߴ�
	if (size % 2 == 0) ++size;
	cv::Size kernel_size(size, size);

	//�Լ���д�Ĵ������Ĳ����б�Ҫ����cv���к�����д��(����GaussianBlur)
	cv::GaussianBlur(mat, dst, kernel_size, 0);

	MAT DST = packMAT(dst);
	layer.setMat(DST);
}

void ImageProcess::Sculpture(ImageProcess& process, Layer &layer)
{
	process.Traces.push(layer.getMat(), layer.getID());
	auto dst = SculptureFilter(parseMAT(layer.getMat()));
	auto DST = packMAT(dst);
	layer.setMat(DST);
}
