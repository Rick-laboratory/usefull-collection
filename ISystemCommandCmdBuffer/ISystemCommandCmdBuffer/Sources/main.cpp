#include"../Headers/Interfaces/ISystemCommandOutputBuffer.h"
#include"../Headers/SystemCommandOutputBuffer.h"
void test(ISystemCommandOutputBuffer* Obj) {
	Obj->CreateTempCommandOutputFile("dir", "testfile.txt");
	Obj->BufferCommandOutput();
	//Obj->DeleteTempCommandOutputFile();
}
void testSystem() {
	SystemCommandOutputBuffer objekt;
	test(&objekt);
}
int main() {
	testSystem();
	return 0;
}