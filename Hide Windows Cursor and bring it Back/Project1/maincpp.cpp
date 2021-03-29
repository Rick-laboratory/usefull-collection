#include<Windows.h>
HCURSOR createBlankCursor()
{
    int cw = GetSystemMetrics(SM_CXCURSOR);
    int ch = GetSystemMetrics(SM_CYCURSOR);
    UINT8* cursorAND = new UINT8[ch * ((cw + 31) >> 2)];
    UINT8* cursorXOR = new UINT8[ch * ((cw + 31) >> 2)];
    memset(cursorAND, 0xff, ch * ((cw + 31) >> 2));
    memset(cursorXOR, 0x00, ch * ((cw + 31) >> 2));
    HCURSOR c = CreateCursor(GetModuleHandle(NULL),
        0, 0, cw, ch, cursorAND, cursorXOR);
    delete[] cursorXOR;
    delete[] cursorAND;
    return c;
}
int main() {
    SetSystemCursor(createBlankCursor(), 32512);
    DestroyCursor(createBlankCursor());
    Sleep(15000);
    SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
	return EXIT_SUCCESS;
}