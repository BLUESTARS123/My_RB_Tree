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
    my_rb_tree->fls();
    std::cout<<"----------"<<std::endl;
    int b[] = {15};
	for (size_t i = 0; i < sizeof(b) / sizeof(b[0]); i++)
	{   
		std::cout<<my_rb_tree->erase_unique_key(b[i])<<std::endl;
        
	}
    //层序遍历，打印节点
    my_rb_tree->fls();
    if(my_rb_tree->Isbalance())
        std::cout << "Isbalance---->" <<"YES"<< std::endl;
    else
         std::cout << "Isbalance---->" <<"NO"<< std::endl;
    return 0;
}
