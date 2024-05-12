#include <Windows.h>
#include <string>
#include <gdiplus.h>
#include <objidl.h>
#include <dshow.h>
#include <thread>
#include "sound.h"

#pragma comment(lib, "strmiids.lib")

using namespace std;
using namespace Gdiplus;


// DirectShow 인터페이스
IGraphBuilder* pGraph = NULL;
IMediaControl* pControl = NULL;
IMediaEvent* pEvent = NULL;
IBasicAudio* pBasicAudio = NULL;

// 두 번째 오디오 파일을 위한 DirectShow 인터페이스
IGraphBuilder* pGraph2 = NULL;
IMediaControl* pControl2 = NULL;
IMediaEvent* pEvent2 = NULL;
IBasicAudio* pBasicAudio2 = NULL;

void PlayMP3(const WCHAR* filename) {
    CoInitialize(NULL);
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
    pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
    pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
    pGraph->QueryInterface(IID_IBasicAudio, (void**)&pBasicAudio);

    BSTR bstrFilename = SysAllocString(filename);
    pControl->RenderFile(bstrFilename);
    SysFreeString(bstrFilename);
    pControl->Run();

    if (pBasicAudio != NULL) {
        pBasicAudio->put_Volume(-2500); // 볼륨 설정
    }

    // 별도의 스레드나 타이머를 사용하여 이벤트 모니터링
    std::thread eventMonitor(MonitorMediaEvent);
    eventMonitor.detach(); // 백그라운드에서 실행
}

void PlayMP3Close() {
    if (pControl) pControl->Release();
    if (pGraph) pGraph->Release();
    if (pEvent) pEvent->Release();
    if (pBasicAudio) pBasicAudio->Release();
    CoUninitialize();
}

void MonitorMediaEvent() {
    long evCode;
    LONG_PTR param1, param2;
    while (pEvent->GetEvent(&evCode, &param1, &param2, 0) == S_OK) {
        pEvent->FreeEventParams(evCode, param1, param2);
        if (evCode == EC_COMPLETE) {
            // 미디어 재생 완료 이벤트 처리
            PlayMP3Close(); // 리소스 해제 함수 호출
            break;
        }
    }
}

void PlaySecondMP3(const WCHAR* filename) {
    CoInitialize(NULL);
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph2);
    pGraph2->QueryInterface(IID_IMediaControl, (void**)&pControl2);
    pGraph2->QueryInterface(IID_IMediaEvent, (void**)&pEvent2);
    pGraph2->QueryInterface(IID_IBasicAudio, (void**)&pBasicAudio2);

    BSTR bstrFilename = SysAllocString(filename);
    pControl2->RenderFile(bstrFilename);
    SysFreeString(bstrFilename);
    pControl2->Run();

    if (pBasicAudio2 != NULL) {
        pBasicAudio2->put_Volume(0); // 볼륨 설정
    }

    // 별도의 스레드에서 이벤트 모니터링
    std::thread eventMonitor(MonitorSecondMediaEvent);
    eventMonitor.detach(); // 백그라운드에서 실행
}

void CloseSecondMP3() {
    if (pControl2) pControl2->Release();
    if (pGraph2) pGraph2->Release();
    if (pEvent2) pEvent2->Release();
    if (pBasicAudio2) pBasicAudio2->Release();
    CoUninitialize();
}

void MonitorSecondMediaEvent() {
    long evCode;
    LONG_PTR param1, param2;
    while (pEvent2->GetEvent(&evCode, &param1, &param2, 0) == S_OK) {
        pEvent2->FreeEventParams(evCode, param1, param2);
        if (evCode == EC_COMPLETE) {
            // 미디어 재생 완료 이벤트 처리
            CloseSecondMP3(); // 리소스 해제 함수 호출
            break;
        }
    }
}