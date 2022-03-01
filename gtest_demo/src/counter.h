#pragma once
#include "common.h"
class Counter
{
public:
    void Init();
    void Uninit();
    int  AddOnce();
    int  SubOnce();

private:
    int GetCurNum();

    int m_count_number = 0;
};