#include"RB_tree.h"
#include<iostream>
#include<vector>
 // namespace  std;

int main()
{   
    myrbtree::RB_Tree<int>* my_rb_tree = new myrbtree::RB_Tree<int>;
    
    int a[] = {16,3, 7, 11, 9, 26, 18, 14, 15};
	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		my_rb_tree->insert(a[i]);
	}
    //层序遍历，打印节点
    my_rb_tree->fls();
    std::cout << "Isbalance---->" <<my_rb_tree->Isbalance()<< std::endl;
    return 0;
}
