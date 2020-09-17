#include "../include/dft.h"
#include "../include/static.h"

namespace Steinberg {
namespace HelloWorld {

DFT::DFT() {
	// 110 ~ 1760 Hzの間を対象とする

	// 周波数分解能からアプローチ
	// 110Hz(A2) の半音上 A#2 (116.54) の更に半分の3.27Hzを周波数分解能にしたい 
	// Δf = fs / N https://www.onosokki.co.jp/HP-WK/c_support/faq/fft_common/fft_analys_4.htm
	// サンプリング周波数 44100 Hzの場合 N = 13486
	// サンプリング周波数 48000 Hzの場合 N = 14678
	// よって N = 16384 (2^14) ディレイは、372msec

	// ディレイからアプローチする
	// 音は2m進む間に約5msec遅れる。ざっくりディレイ 20msec以内にしたい
	// Δt = N / fs
	// N = 1024, 分解能は 43Hz(44100), 46.8(48000)

	// バランスとると4096かなぁ
	this->_p = new float[Static::num];
	this->re = new float[Static::num];
	this->im = new float[Static::num];
	this->spectrum = new float[Static::num];

	for(int i=0; i<Static::num; i++) {
		_p[i] = re[i] = im[i] = spectrum[i] = 0.0f;
	}
}

DFT::~DFT() {
	delete[] _p;
	delete[] re;
	delete[] im;
	delete[] spectrum;
}

void DFT::process(float* source) {
	for (int i = 0; i < Static::num; i++) spectrum[i] = source[i];
}

}  // namespace HelloWorld
}  // namespace Steinberg