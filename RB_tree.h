////第一版，简单的T类型比较，适用于int，double
#include<initializer_list>
#include<deque>
#include<vector>
#include<iostream>
namespace myrbtree{

//节点颜色设计 constexpr
typedef bool rb_tree_node_color_type;
static const rb_tree_node_color_type rb_tree_black = true;
static const rb_tree_node_color_type rb_tree_red = false;
//节点设计
template<class T>
struct rb_tree_node_base
{
    typedef rb_tree_node_color_type color_type;
    typedef rb_tree_node_base<T>* base_ptr;
    
    //定义父节点，子节点
    base_ptr parent;
    base_ptr left;
    base_ptr right;
    color_type color;

    //定义节点值
    T value;

    base_ptr get_node_ptr(){
        return &*this;
    }
    rb_tree_node_base(T value):value(value),parent(nullptr),left(nullptr),right(nullptr),color(rb_tree_red){};

};

//基本操作
//设为红
template<class NodePtr> 
bool set_red(NodePtr node){
    node->color = rb_tree_red;
}
//设为黑
template<class NodePtr> 
bool set_black(NodePtr node){
    node->color = rb_tree_black;
}
//判断节点为红
template<class NodePtr>
bool rb_tree_node_is_red(NodePtr node){
    return node->color==rb_tree_red;
}
//判断节点为黑
template<class NodePtr>
bool rb_tree_node_is_black(NodePtr node){
    return node->color==rb_tree_black;
}
//节点最小
template<class NodePtr>
NodePtr rb_tree_min(NodePtr node){
    while(node->left){
        node = node->left;
    }
    return node;
}
//节点最大
template<class NodePtr>
NodePtr rb_tree_max(NodePtr node){
    while (node->right)
    {
       node = node->right;
    }
    return node;
    
}
//是左节点
template<class NodePtr>
bool rb_tree_node_is_lchild(NodePtr node)
{
    return node == node->parent->left;
}
//是右节点
template<class NodePtr>
bool rb_tree_node_is_rchild(NodePtr node)
{
    return node == node->parent->right;
}
//寻找后继节点
template<class NodePtr>
NodePtr re_tree_next(NodePtr node){
    if(node->right){
        return rb_tree_min(node->next);
    }
    //没有右子树，向上找，只要节点是父节点的右节点，那么该节点就是了
    while(!rb_tree_node_is_lchild(node)){
        node = node->parent;
    }
    return node->parent;
}
//左旋
template<class NodePtr>
void rb_tree_left_route(NodePtr node,NodePtr& root)
{   
    auto node_lchild = node->right;
    //处理新中心原来的左节点
    node->right = node_lchild->left;
    if(node_lchild->left)
        node_lchild->left->parent = node;
    //新中心节点与其父节点
    node_lchild->parent = node->parent;
    if(node==root)
    {   
        root = node_lchild;
        
    }
    
    else if(rb_tree_node_is_lchild(node))
    {
        node->parent->left = node_lchild;
    }
    else
    {
        node->parent->right = node_lchild;
    }
    //新中心与原中心
    node_lchild->left = node;
    node->parent = node_lchild;
}
//右旋
template<class NodePtr>
void rb_tree_right_route(NodePtr node,NodePtr& root)
{
    NodePtr new_center = node->left;
    //处理新中心被挤下去的节点
    node->left = new_center->right;
    if(new_center->right)
    {
        new_center->right->parent = node;
    }
    //处理新中心与父亲节点
    new_center->parent = node->parent;
    if(root==node){
        root = new_center;
    }
    else if(rb_tree_node_is_lchild(node))
    {
        node->parent->left = new_center;
    }
    else{
        node->parent->right = new_center;
    }
    //新中心与旧中心
    new_center->right = node;
    node->parent = new_center;
}
//插入重平衡
template<class NodePtr>
void rb_tree_insert_fix(NodePtr new_node,NodePtr& root)
{   
    set_red(new_node);
    //插入new_node 假设需要调整
    //需要调整时，必然有祖父节点
    while(new_node!=root&&rb_tree_node_is_red(new_node->parent)){
    NodePtr parent_node = new_node->parent;
    NodePtr uncle_node = nullptr;
    if(rb_tree_node_is_lchild(parent_node)){
       uncle_node = parent_node->parent->right;
        //判断u节点
    //情况一：u为黑或者空。将红色旋转过去即可
        if(uncle_node==nullptr||rb_tree_node_is_black(uncle_node))
        {
            if(rb_tree_node_is_rchild(new_node))
            {   //std::cout<<"171"<<std::endl;
                new_node = new_node->parent;
                rb_tree_left_route(new_node,root);
            }
            new_node->parent->color = rb_tree_black;
            new_node->parent->parent->color = rb_tree_red;
            rb_tree_right_route(new_node->parent->parent,root);
            

        }
        else{
            //u为红，找组父，祖父变红，pu变黑
            parent_node->color = rb_tree_black;
            uncle_node->color = rb_tree_black;
            parent_node->parent->color = rb_tree_red;
            new_node = parent_node->parent;
        }   

    }
    else{
        uncle_node = parent_node->parent->left;
        //判断u节点
    //情况一：u为黑或者空。将红色旋转过去即可
         if(uncle_node==nullptr||rb_tree_node_is_black(uncle_node))
        {   
            if(rb_tree_node_is_lchild(new_node)){
                new_node = new_node->parent;
            rb_tree_right_route(new_node,root);
        }
        new_node->parent->color = rb_tree_black;
        new_node->parent->parent->color = rb_tree_red;
        rb_tree_left_route(new_node->parent->parent,root);
       
        }
        else{
            //u为红，找组父，祖父变红，pu变黑
            parent_node->color = rb_tree_black;
            uncle_node->color = rb_tree_black;
            parent_node->parent->color = rb_tree_red;
            new_node = parent_node->parent;
        }
    }
    
    }
    root->color = rb_tree_black;
     


}
//删除重平衡
template<class NodePtr>
NodePtr rb_tree_delete_fix(NodePtr old_node,NodePtr& root){
    
}

template <class T>
class RB_Tree{

//嵌套定义类型
typedef rb_tree_node_base<T>* base_ptr;
typedef rb_tree_node_base<T> base_node;

private:
//元数据：header，节点数量
int node_count;
base_ptr _header;

private:
//取得根节点，最大，最小节点
base_ptr& root() const {return _header->parent;};
base_ptr& leftmost() const{return _header->left;};
base_ptr& rightmost() const{return _header->right;};

public:
// 容量相关操作:空，size
bool empty() {return node_count==0;}
int size() {return node_count;}

public:
//复制，构造，析构函数
RB_Tree(){rb_tree_init();};
RB_Tree(const RB_Tree& rhs);
RB_Tree(RB_Tree&& rhs);
~RB_Tree();

//=
RB_Tree& operator=(const RB_Tree& rhs);
RB_Tree& operator=(RB_Tree&& rhs);

public:
//插入操作，删除操作，查找操作

//插入
//emplace
base_ptr insert_unique_iter(base_ptr iter);
base_ptr insert_unique_val(T value);
base_ptr insert(T value){
    return insert_unique_val(value);
}
base_ptr _insert(base_ptr cur_node,base_ptr parent,T valie);

//insert

//删除
void clear();
base_ptr erase_unique(const T keys);
base_ptr rease(base_ptr iter);


//查找

//遍历：
void fls();
bool Isbalance();
bool _Isbalance(base_ptr root, int count, int num);
private:

//初始化节点
void rb_tree_init();

};

//实现
template<class T>
void RB_Tree<T>::rb_tree_init (){
    //创建一个节点
    _header = new base_node(-1);
    _header->color = rb_tree_black;
    root()=nullptr;
    leftmost()=_header;
    rightmost()=_header;
    node_count=0;
}

template<class T>
typename RB_Tree<T>::base_ptr
RB_Tree<T>::insert_unique_val(T insert_value)
{
    //插入一个值，不允许重复
    base_ptr cur_node = root();
    base_ptr parent = _header;
    // if(!cur_node){
    //     //插入为根节点
    //     _header->parent = new base_node(insert_value);
    //     leftmost()=_header->parent;
    //     rightmost() = _header->parent;
    //     _header->parent->parent = _header;
    //     _header->parent->color = rb_tree_black;
    //     return _header->parent;
    // }
    while(cur_node)
    {
        //插入到叶子节点
        parent = cur_node;
        cur_node = (insert_value>cur_node->value)?cur_node->right:cur_node->left;

    }
    _insert(cur_node,parent,insert_value);
}
template<class T>
typename RB_Tree<T>::base_ptr
RB_Tree<T>::_insert(base_ptr cur_node,base_ptr parent,T value){
    base_ptr new_node=nullptr;
    if(parent==_header||value<parent->value){
         new_node = new base_node(value);
        parent->left = new_node;
        if(parent==_header){
            root()=new_node;
            rightmost()=new_node;
        }
        else if(parent==leftmost()){
            leftmost()=new_node;
        }
    }
    else{
        new_node = new base_node(value);
        parent->right = new_node;
        
        if(parent==rightmost()){
            rightmost()=new_node;
        }
    }
    new_node->parent = parent;
    new_node->left = nullptr;
    new_node->right = nullptr;
    ++node_count;
    rb_tree_insert_fix(new_node,root());

}

// template<class T>
// typename RB_Tree<T>::base_ptr
// RB_Tree<T>::insert_unique_val(T insert_value){
//     //插入一个值，不允许重复
//     base_ptr cur_node = root();
//     if(!cur_node){
//         //插入为根节点
//         _header->parent = new base_node(insert_value);
//         leftmost()=_header->parent;
//         rightmost() = _header->parent;
//         _header->parent->parent = _header;
//         _header->parent->color = rb_tree_black;
//         return _header->parent;
//     }
//     while(cur_node){
//         //插入到叶子节点
//         if(insert_value>cur_node->value){
//             //向右找
//             //右节点不为空，继续找
//             if(cur_node->right){
//                 cur_node = cur_node->right;
//             }
//             //右节点空，插入，调整
//             else{
//                 cur_node->right = new base_node(insert_value);
//                 cur_node->right->parent = cur_node;
//                 if(rb_tree_node_is_red(cur_node)){
//                     rb_tree_insert_fix(cur_node->right,root());
//                 }
//                 break;
//             }

//         }
//         else{
//             //向左找
//             //左不为空，继续找
//             if(cur_node->left){
//                 cur_node = cur_node->left;
//             }
//             //空，插入，调整
//             else{
//                 cur_node->left = new base_node(insert_value);
//                 cur_node->left->parent = cur_node;
//                 if(rb_tree_node_is_red(cur_node)){
//                     rb_tree_delete_fix(cur_node->left,root());
//                 }
//                 break;
//             }
//         }
//     }

// }

template<class T>
void RB_Tree<T>::fls(){
    std::deque<base_ptr>que;
    std::vector<std::vector<int> >ans;
    std::vector<std::vector<bool> >ans1;
    if(_header->parent)
    

    que.push_back(_header->parent);

    while(!que.empty()){
        int size = que.size();
        ans.push_back(std::vector<int>());
        ans1.push_back(std::vector<bool>());
        for(int i=0;i<size;++i){
            ans.back().push_back(que.front()->value);
            bool co = que.front()->color;//?1:0;
            ans1.back().push_back(co);
            
            if(que.front()->left) que.push_back(que.front()->left);
            if(que.front()->right) que.push_back(que.front()->right);
            que.pop_front();
        }
    }
    
    for(int i=0;i<ans.size();++i){
        for(int j=0;j<ans[i].size();++j){
            std::cout<<ans[i][j]<<" ";
        std::cout<<ans1[i][j]<<" ";
        }
       std::cout<<std::endl;

   }
    

}
template<class T>
bool RB_Tree<T>::Isbalance()
	{
		//1.空树认为是RBTree
		if (root() == NULL)
			return true;
		//2.根结点非黑不是RBTree
		if (root()->color == rb_tree_red)
			return false;
		int count = 0;
		//以左子树上的黑色结点作为一个基准，如果一样就证明是，不一样就证明不是RBTree
		base_ptr cur = root();
		while (cur != NULL)
		{
			if (cur->color == rb_tree_black)
				count++;
			cur = cur->left;
		}
		//num用来标记每条路径上的黑色结点的个数
	   //count用来标记左子树上的黑色结点的个数
		int num = 0;
		return _Isbalance(root(), count, num);
	}
template<class T>
bool RB_Tree<T>::_Isbalance(base_ptr root, int count, int num)
	{
		if (root == NULL)
			return true;
		//3.连续红结点不是RBTree,root结点一定有父结点
		if (root->color == rb_tree_red&&root->parent->color == rb_tree_red)
		{
			std::cout << root->value << " 有连续的红结点" << std::endl;
			return false;
		}
		if (root->color == rb_tree_black)
			num++;
		if (root->left == NULL&&root->right == NULL)
		{
			if (num != count)
			{
				std::cout << root->value << " 黑色结点个数不一样" << std::endl;
				return false;
			}
		}
	return _Isbalance(root->left, count, num) && _Isbalance(root->right, count, num);
	}
}