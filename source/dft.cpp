#include "../include/dft.h"

namespace Steinberg {
namespace HelloWorld {

DFT::DFT(int num) {
	this->num = num;

	f0		  = 0.0;
	noise_level = 0.0f;

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

DFT::~DFT() {
	delete[] re;
	delete[] im;
	delete[] spectrum;

	delete[] index;
	delete[] w_re;
	delete[] w_im;
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
		int k   = index[i];
		float p = re[k] * re[k] + im[k] * im[k];

		spectrum[i - 1] = logf(p * i);

		fpeak[i - 1] = noise_level;
	}

	// F0推定
	// 包括線を書く
	int vertex[32];
	float vertexValue[32];
	vertex[0]		= 256;
	vertexValue[0] = noise_level;
	int vIndex	= 1;
	float left	= noise_level;

	bool loop = true;
	while (loop) {
		loop			   = false;
		float len_max	   = 0.0f;
		int len_max_index = 0;

		float a = (vertexValue[vIndex - 1] - left) / vertex[vIndex - 1];
		for (int i = 1; i < vertex[vIndex - 1] - 1; i++) {
			float y	= left + a * i;
			float len = spectrum[i] - y;
			if (len > len_max) {
				len_max	    = len;
				len_max_index = i;
				loop		    = true;
			}
		}
		if (loop) {
			vertexValue[vIndex] = spectrum[len_max_index];
			vertex[vIndex]		= len_max_index;
			vIndex++;
		}
	}

	int leftIndex = 0;
	left		    = noise_level;
	f0		    = vertex[vIndex - 1];
	while (vIndex-- > 0) {
		int rightIndex = vertex[vIndex];
		float right	= vertexValue[vIndex];
		for (int i = leftIndex; i < rightIndex; i++) {
			float a  = (float)(i - leftIndex) / (rightIndex - leftIndex);
			fpeak[i] = a * (right - left) + left;
		}
		leftIndex = rightIndex;
		left		= right;
	}
}

}  // namespace HelloWorld
}  // namespace Steinberg