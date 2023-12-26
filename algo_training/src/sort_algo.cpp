#include <iostream>
#include <vector>

using namespace std;
auto show_vec = [](const std::string& name, const auto& v){
    cout << name + " [ ";
    for(const auto& e : v)
    {
        cout << e  <<  ", ";
    }
    cout << "]";
    cout << endl << endl;
};


void select_sort(std::vector<int>& v)
{
    size_t len = v.size();
    // 从第0个元素开始循环，认为第i个元素之前的元素均已经排序
    for(size_t i = 0; i < len-1; i++)
    {
        // 从第1个元素开始往后，查找最小元素，下标记为min
        size_t min = i;
        for(size_t j=i+1; j<len; j++)
        {
            if(v[j] < v[min])
            {
                min = j;
            }
        }
        //将最小元素移动到本次循环
        swap(v[min], v[i]);
    }
}

// 冒泡排序
void bubble_sort(std::vector<int>& v)
{
    size_t len = v.size();
    // 从第0个元素开始循环，认为第i个元素之前的元素已经排序
    for(size_t i = 0; i < len-1; i++)
    {
        // 从第1个元素开始往后，查找最小元素，下标记为min
        for(size_t j=i+1; j<len; j++)
        {
            if(v[j] < v[i])
            {
                swap(v[j], v[i]);
            }
        }
    }
}


int main()
{
    std::vector<int> v1{1, 3 ,5 ,7 ,4};
    show_vec("before sort, v1", v1);
    select_sort(v1);
    show_vec("after sort, v1", v1);

    std::vector<int> v2{1, 3 ,5 ,7 ,4, 2, 8, 6, 9, 10};
    show_vec("before sort, v2", v2);
    bubble_sort(v2);
    show_vec("after sort, v2", v2);

    return 0;
}