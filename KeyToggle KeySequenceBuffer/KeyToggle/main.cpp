#include "Key_Operator.h"
#include <iostream>
#include <vector>
enum class AppState { RECORD, INACTIVE, EXIT };
AppState appState = AppState::RECORD;
Key_Operator CONTROL(0x11), R(0x52), I(0x49);
std::vector<int> keySequenceBuffer;
void EvaluateSequence(std::vector<int>keySequenceBuffer) {
	if (keySequenceBuffer[0] = 0x11 && keySequenceBuffer[1] == 0x52 && keySequenceBuffer[2] == 0x49) {
		std::cout << "CTRL + R + I has been Pressed\n";
		keySequenceBuffer.clear();
	}
}
bool shift = false;
int main(void) {
	while (appState == AppState::RECORD)
	{
		if (CONTROL) {
			std::cout << "CTRL has been Pressed\n" ;
			std::cout << keySequenceBuffer.size() << "\n";
			keySequenceBuffer.push_back(0x11);
			if (keySequenceBuffer.size() == 3) {
				EvaluateSequence(keySequenceBuffer);
				keySequenceBuffer.clear();
			}
		}
		if (R)
		{
			std::cout << "R has been Pressed\n";
			std::cout << keySequenceBuffer.size() << "\n";
			keySequenceBuffer.push_back(0x52);
			if (keySequenceBuffer.size() == 3) {
				EvaluateSequence(keySequenceBuffer);
				keySequenceBuffer.clear();
			}
		}
		if (I)
		{
			std::cout << "I has been Pressed\n";
			std::cout << keySequenceBuffer.size() << "\n";
			keySequenceBuffer.push_back(0x49);
			if (keySequenceBuffer.size() == 3) {
				EvaluateSequence(keySequenceBuffer);
				keySequenceBuffer.clear();
			}
		}
		if (GetAsyncKeyState(192) < 0 && shift == false)
		{
			std::cout << "pressed\n";
			shift = true;
		}
		if (GetAsyncKeyState(192) == 0 && shift == true)
		{
			std::cout << "released\n";
			shift = false;
		}
	}
}