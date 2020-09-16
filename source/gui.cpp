#include "../include/gui.h"

namespace Steinberg {
namespace HelloWorld {

GUI::GUI(void* controller) : VSTGUIEditor(controller) {
	ViewRect viewRect(0, 0, 200, 200);
	setRect(viewRect);

//	wave = nullptr;
}

bool PLUGIN_API GUI::open(void* parent, const PlatformType& platformType)
{
	// GUIウィンドウが開かれたときに、UIを作成する
 
	// まずはフレーム(配置領域)がすでに作成・設定されているか確認。
	// すでに作成・設定されている場合(frameがNULLでない場合)は終了
	// frameは継承元クラスで定義されている。
	if (frame) return false;
 
	// 作成するフレームのサイズを設定
	CRect size(0, 0, 200, 200);
 
	// フレームを作成。作成に失敗したら(NULLなら)終了。
	// 引数には、フレームサイズ、自作GUIクラスのポインタを指定する
	frame = new CFrame(size, this);
	if (frame == nullptr) return false;
 
	// 作成したフレームに背景画像を設定
	CBitmap* cbmp = new CBitmap("background.png"); // リソースから背景画像を読み込む
	frame->setBackground(cbmp); // フレームに背景画像を設定
	cbmp->forget();	// フレームに設定後は背景画像はforgetで解放しておく

	// CRect waveSize(10, 10, 120, 120);
	// wave = new WaveView(waveSize, 2048);
	// frame->addView(wave);
 
	// 作成したフレームを開く
	frame->open(parent);
 
	// GUIウィンドウのオープンに成功した場合はtrueを返す
	return true;
}

/*
CMessageResult GUI::notify(CBaseObject *sender, const char *message) {
	if (message == CVSTGUITimer::kMsgTimer) {
		if (wave != nullptr) {
			// wave->update();
		}
	}

	return VSTGUIEditor::notify(sender, message);
}*/

void GUI::valueChanged(CControl*pControl) {
	
}

void PLUGIN_API GUI::close() {
	if (frame) {
		frame->forget();
		frame = nullptr;
	}
}

}  // namespace Vst
}  // namespace Steinberg