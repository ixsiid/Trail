#include "../include/gui.h"

#include "../include/dft.h"
#include "../include/waveview.h"

namespace Steinberg {
namespace HelloWorld {

GUI::GUI(void* controller) : VSTGUIEditor(controller) {
	ViewRect viewRect(0, 0, 1200, 600);
	setRect(viewRect);

	wave = nullptr;
}

bool PLUGIN_API GUI::open(void* parent, const PlatformType& platformType) {
	// GUIウィンドウが開かれたときに、UIを作成する

	// まずはフレーム(配置領域)がすでに作成・設定されているか確認。
	// すでに作成・設定されている場合(frameがNULLでない場合)は終了
	// frameは継承元クラスで定義されている。
	if (frame) return false;

	// 作成するフレームのサイズを設定
	CRect size(0, 0, 1200, 600);

	// フレームを作成。作成に失敗したら(NULLなら)終了。
	// 引数には、フレームサイズ、自作GUIクラスのポインタを指定する
	frame = new CFrame(size, this);
	if (frame == nullptr) return false;

	// 作成したフレームに背景画像を設定
	CBitmap* cbmp = new CBitmap("background.png");  // リソースから背景画像を読み込む
	frame->setBackground(cbmp);				   // フレームに背景画像を設定
	cbmp->forget();						   // フレームに設定後は背景画像はforgetで解放しておく

	CRect waveSize(40, 10, 40 + 512, 10 + 512);
	wave = new WaveView(waveSize);
	frame->addView(wave);

	CRect projectionSize(40 + 512 + 20, 10, 40 + 512 + 20 + 512, 10 + 512);
	projection = new Projection(projectionSize);
	frame->addView(projection);

	// 作成したフレームを開く
	frame->open(parent);

	// GUIウィンドウのオープンに成功した場合はtrueを返す
	return true;
}

CMessageResult GUI::notify(CBaseObject* sender, const char* message) {
	if (message == CVSTGUITimer::kMsgTimer) {
		if (wave != nullptr) {
			wave->update();
			
		}
	}

	return VSTGUIEditor::notify(sender, message);
}

void GUI::valueChanged(CControl* pControl) {
}

void PLUGIN_API GUI::close() {
	frame->forget();
	//frame->close();
}

}  // namespace HelloWorld
}  // namespace Steinberg