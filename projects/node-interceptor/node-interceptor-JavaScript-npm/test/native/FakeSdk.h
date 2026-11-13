#pragma once
#include <cstdint>
void FakePushKey(unsigned device,unsigned code,unsigned flags=0);
void FakePushMouse(unsigned device,int x,int y,unsigned buttons=0,short wheel=0,unsigned flags=0);
void FakeReset();
unsigned FakeSentKeys();unsigned FakeSentMice();
bool FakeAwaitSent(unsigned keys,unsigned mice);
void FakeRemove(unsigned id);
