#pragma once

extern std::wstring fileName[3];

void PlayMP3(const WCHAR* filename);

void PlayMP3Close();

void MonitorMediaEvent();

void PlaySecondMP3(const WCHAR* filename);

void CloseSecondMP3();

void MonitorSecondMediaEvent();

void GameBackgroundSound(int num);

void CloseGameBackgroundSound();