#include <iostream>

using namespace std;

// 链表节点
struct ListNode
{
    ListNode(const int& v):val(v),next(nullptr){}
    int val;
    ListNode* next;
};

// 链表类
class LinkedList
{
public:
    LinkedList() = default;
    ~LinkedList() = default;

public:
    // 尾部插入
    void push_back(const int& v)
    {
        auto new_p = new ListNode(v);

        // 如果头节点为空，则更新头节点
        if(m_head == nullptr)
        {
            m_head = new_p;
        }
        else
        {
            // 如果头节点不为空，循环查找尾节点
            auto p = m_head; 
            while (p->next != nullptr)
            {
                p = p->next;
            }
            p->next = new_p;
        }
    }

    // 删除指定值的元素
    void remove_val(const int& v)
    {
        // 头节点为空，直接返回
        if(m_head == nullptr)
        {
            return;
        }

        // 头节点就是待删除的节点，直接删除头节点
        if(m_head->val == v)
        {
            auto temp = m_head;
            m_head = m_head->next;
            delete temp;
            return;
        }

        // 头节点不为空，并且待删除的节点不是头节点
        // 循环查找待删节点
        auto p = m_head;
        while (p->next != nullptr && p->next->val != v)
        {
            p = p->next;
        }

        // 待删节点是 p->next，删除待删节点
        if(p->next != nullptr)
        {
            auto temp = p->next;
            p->next = p->next->next;
            delete temp;
        }
    }

    bool find_val(const int& v)
    {
        if(m_head == nullptr)
        {
            return false;
        }

        auto p = m_head;
        while (p != nullptr)
        {
            if(p->val == v)
            {
                return true;
            }
            p = p->next;
        }

        return false;
    }

    // 打印链表
    void show()
    {
        cout << "-------------------------" << endl;

        if(m_head == nullptr)
        {
            cout << "{{ head ptr is nullptr }}" << endl;
            cout << "-------------------------" << endl;
            return;
        }

        auto p = m_head;
        while (p != nullptr)
        {
            cout << p->val << endl;
            p = p->next;
        }
        
        cout << "-------------------------" << endl;
    }

public:
    ListNode* m_head{nullptr};
};

int main()
{
    // LinkedList ll;
    // ll.push_back(1);
    // ll.push_back(2);
    // ll.push_back(3);
    // ll.show();
    // ll.remove_val(1);
    // ll.show();
    // ll.remove_val(3);
    // ll.show();

    // LinkedList l1;
    // l1.remove_val(1);
    // l1.show();

    // LinkedList l2;
    // l2.push_back(1);
    // l2.show();
    // l2.remove_val(1);
    // l2.show();

    LinkedList l3;
    l3.push_back(1);
    l3.push_back(2);
    l3.push_back(1);
    l3.show();
    l3.remove_val(1);
    l3.show();

    cout << "find_val(0):" << l3.find_val(0) << endl;
    cout << "find_val(1):" << l3.find_val(1) << endl;
    cout << "find_val(2):" << l3.find_val(2) << endl;

    return 0;
}