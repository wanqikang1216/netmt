#include "hashchaining.h"

int main()
{
    hash_table _hash_table;
    Node *_node = _hash_table.hash_search(3);   //��ʱ��Ϊ�գ�����search
    Node *nullptr = NULL; 
	
	if(_node == nullptr)
        cout<<"Now _node is a nullptr!"<<endl;

    string name = "asdfg";

    for(int i=0; i<5; i++)  //����ڵ�
    {
        name[i]++;
        _hash_table.hash_insert(i,name);
    }

    _node = _hash_table.hash_search(3);     //����search(int _key)�ӿ�
    cout<<_node->name<<endl;

    _hash_table.hash_delete(3,"btegg");     //����delete(int _key, string _name)�ӿ�
    _node = _hash_table.hash_search(3);
    if(_node == nullptr)
        cout<<"Now _node is a nullptr!"<<endl;
    else
        cout<<_node->name<<endl;

    _hash_table.hash_insert(2,"b");     //����insert(int _key, string _name)�ӿ�

    _node = _hash_table.hash_search(2,"btefg");     //����search(int _key, string _name)�ӿ�
        cout<<_node->name<<endl;

    _node = _hash_table.hash_search(2,"b"); 
        cout<<_node->name<<endl;

    _hash_table.hash_delete(2,"btefg");
    _node = _hash_table.hash_search(2);
    if(_node == nullptr)
        cout<<"Now _node is a nullptr!"<<endl;
    else
        cout<<_node->name<<endl;
    return 0;
}
