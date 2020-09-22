#include "../include/gui.h"
#include "../include/plugids.h"

namespace Steinberg {
namespace HelloWorld {

GUI::GUI(void* controller, DFT * dft, Projection * proj) : VSTGUIEditor(controller) {
	ViewRect viewRect(0, 0, 700, 860);
	setRect(viewRect);

	this->dft = dft;
	this->proj = proj;

	this->wave = nullptr;
	this->projection = nullptr;
}

bool PLUGIN_API GUI::open(void* parent, const PlatformType& platformType) {
	// GUIウィンドウが開かれたときに、UIを作成する

	// まずはフレーム(配置領域)がすでに作成・設定されているか確認。
	// すでに作成・設定されている場合(frameがNULLでない場合)は終了
	// frameは継承元クラスで定義されている。
	if (frame) return false;

	// 作成するフレームのサイズを設定
	CRect size(0, 0, 700, 860);

	// フレームを作成。作成に失敗したら(NULLなら)終了。
	// 引数には、フレームサイズ、自作GUIクラスのポインタを指定する
	frame = new CFrame(size, this);
	if (frame == nullptr) return false;

	// 作成したフレームに背景画像を設定
	CBitmap* cbmp = new CBitmap("background.png");  // リソースから背景画像を読み込む
	frame->setBackground(cbmp);				   // フレームに背景画像を設定
	cbmp->forget();						   // フレームに設定後は背景画像はforgetで解放しておく

	CRect waveSize(40, 10, 40 + 512, 10 + 256);
	wave = new WaveView(waveSize, dft);
	frame->addView(wave);

	CRect projectionSize(40, 10 + 256 + 30, 40 + 512, 10 + 256 + 30 + 512);
	projection = new ProjectionView(projectionSize, proj);
	frame->addView(projection);



	cbmp = new CBitmap("up_button.png");
	CRect ksize(0, 0, cbmp->getWidth(), cbmp->getHeight() / 2);
	ksize.offset(550, 280);
	CKickButton* kick = new CKickButton(ksize, this, kCCUpId, cbmp);
	Vst::ParamValue value = controller->getParamNormalized(kCCUpId);
	kick->setValueNormalized(value);
	cbmp->forget();
	frame->addView(kick);

	// 作成したフレームを開く
	frame->open(parent);

	// GUIウィンドウのオープンに成功した場合はtrueを返す
	return true;
}

CMessageResult GUI::notify(CBaseObject* sender, const char* message) {
	if (message == CVSTGUITimer::kMsgTimer) {
		if (wave ) wave->update();
		if (projection) projection->update();
	}

	return VSTGUIEditor::notify(sender, message);
}

void GUI::valueChanged(CControl* pControl) {
	int32 tag = pControl->getTag();
	float value = pControl->getValueNormalized();
	controller->setParamNormalized(tag, value);
	controller->performEdit(tag, value);
}

void PLUGIN_API GUI::close() {
	frame->forget();
}

}  // namespace HelloWorld
}  // namespace Steinberg