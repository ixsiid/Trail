#include "../include/dft.h"

namespace Steinberg {
namespace HelloWorld {

bool DFT::initialized = false;
float* DFT::re		  = nullptr;
float* DFT::im		  = nullptr;

int* DFT::index  = nullptr;
float* DFT::w_re = nullptr;
float* DFT::w_im = nullptr;

float* DFT::spectrum = nullptr;
float* DFT::fpeak	 = nullptr;

double DFT::f0 = 0.0;
float DFT::lowFreqWeight = 3.0f;

void DFT::initialize() {
	if (initialized) return;
	initialized = true;
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

	re = new float[num];
	im = new float[num];

	spectrum = new float[256];
	fpeak    = new float[256];
	for (int i = 0; i < 256; i++) {
		spectrum[i] = 0.0f;
		fpeak[i]	  = -4.0f;
	}

	int p = 0;
	for (int i = 0; i < sizeof(int) * 8; i++) {
		if ((num & (1 << i)) > 0) {
			p += i + 1;
		}
	}

	index = new int[num];
	for (int i = 0; i < num; i++) {
		index[i] = 0;
		int t    = i;
		for (int j = 1; j < p; j++) {
			index[i] <<= 1;
			if ((t & 1) == 1) index[i] |= 1;
			t >>= 1;
		}
	}

	double r = 6.283185307179586476925286766559 / num;

	int mask1 = (num - 1) >> 1;
	int mask2 = (num - 1) >> 2;
	int mask0 = (num - 1) ^ mask1;

	w_re = new float[num / 2];
	w_im = new float[num / 2];
	for (int i = 0; i < num / 2; i++) {
		if ((i & mask1) == 0) {
			w_re[i] = (i & mask0) > 0 ? -1.0f : 1.0f;
			w_im[i] = 0.0f;
		} else if ((i & mask2) == 0) {
			w_re[i] = 0.0f;
			w_im[i] = (i & mask0) > 0 ? -1.0f : 1.0f;
		} else {
			double phase = i * r;

			w_re[i] = cos(phase);
			w_im[i] = sin(phase);
		}
	}
}

void DFT::destroy() {
	delete[] re;
	delete[] im;
	delete[] spectrum;

	delete[] index;
	delete[] w_re;
	delete[] w_im;

	initialized = false;
}

void DFT::process(float* source) {
	memcpy(re, source, sizeof(float) * num);
	memset(im, 0, sizeof(float) * num);

	for (int ns = num / 2; ns > 0; ns /= 2) {
		int arg = 0;
		for (int j = 0; j < num; j += 2 * ns) {
			float* _w_re = w_re + arg;
			float* _w_im = w_im + arg;

			for (int i = j; i < j + ns; i++) {
				int i1 = i + ns;

				float _r = re[i1] * (*_w_re) - im[i1] * (*_w_im);
				float _i = re[i1] * (*_w_im) + im[i1] * (*_w_re);

				re[i1] = re[i] - _r;
				im[i1] = im[i] - _i;

				re[i] += _r;
				im[i] += _i;
			}

			int k = num / 4;
			while (k <= arg && k > 0) {
				arg -= k;
				k /= 2;
			}
			arg += k;
		}
	}

	// 1 - 256 で 0(DC) - 1389.312Hz (44100), 1512.183Hz (44800)
	for (int i = 1; i <= 256; i++) {
		int k = index[i];
		float p = re[k] * re[k] + im[k] * im[k];

		spectrum[i - 1] = logf(p * i);
		fpeak[i - 1] = spectrum[i - 1] / pow(i, lowFreqWeight);
	}

	f0 = 0;
	float pv = fpeak[0];
	for(int i=1; i<256; i++) {
		if (pv < fpeak[i]) {
			f0 = i;
			pv = fpeak[i];
		}
	}

	/*
	// keyword: εフィルタ, 平均値フィルタ
	// 移動平均してみる
	const int n = 7;
	const int lf = 256 - 7;
	float fil[lf];
	for (int i=0; i<lf; i++) {
		fil[i] = 0;
		for(int j=0; j<n; j++) fil[i] += spectrum[i + j];
	}

	const int l1 = lf - 1;
	float p1[l1];
	for(int i=0; i<l1; i++) p1[i] = fil[i + 1] - fil[i]; 
	const int l2 = l1 - 1;
	float p2[l2];
	for(int i=0; i<l2; i++) p2[i] = p1[i+1] - p1[i];
	
	for(int i=0; i<l2 -1; i++) {
		fpeak[i + 4] = (p2[i] > 0 && p2[i +1] < 0) ? (spectrum[i + 4] > -4.0f ? spectrum[i + 4] : -16) : -16;
	}
	*/
}

}  // namespace HelloWorld
}  // namespace Steinberg