void Init()
{
    
}

void Uninit()
{
    
}

int  Counter::AddOnce()
{
    return ++m_count_number;
}

int  Counter::SubOnce()
{
    if(m_count_number == 0)
    {
        return 0;
    }
    return --m_count_number;
}

int GetCurNum()
{
    return m_count_number;
}