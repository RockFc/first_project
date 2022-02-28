#pragma once

class Counter
{
public:
    void Init();
    int AddOnce();
    int SubOnce();
    void Uninit();

private:
    int GetCurNum();

    int m_count_number = 0;
};