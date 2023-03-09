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
template<class T>
void swap(T& lhs,T& rhs){
    T tmp = lhs;
    lhs = rhs;
    rhs = tmp;
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
NodePtr rb_tree_next(NodePtr node){
    if(node->right){
        return rb_tree_min(node->right);
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
            {   
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
NodePtr rb_tree_delete_fix(NodePtr old_node,NodePtr& root,NodePtr& leftmost,NodePtr& rightmost){
    //找到替换节点
    //old node 左右节点空，不需要替换，不空，使用后继节点替换
    NodePtr replaced_deleted_ptr = (!old_node->left||!old_node->right)?old_node:rb_tree_next(old_node);
    //被替换节点的子节点.
    //被删除节点只有一个左子节点，指向左；只有一个右节点，指向右；无：指向nullptr
    NodePtr replaced_child_ptr = replaced_deleted_ptr->left?replaced_deleted_ptr->left:replaced_deleted_ptr->right;
    
    NodePtr location = nullptr;
    //将replaced_deleted_ptr 替换掉 old_node，replaced_child_ptr替换掉 将replaced_deleted_ptr
    //结果是，old_node 还是那个节点，但是已经不在树中了。后面删除即可。
    //
    //old_node最多一个子节点情况
    if(replaced_deleted_ptr==old_node)
    {   
        //直接让子节点替换掉old位置和颜色
        location = replaced_deleted_ptr->parent;
        
        //子节点与原父节点连接
        if(replaced_child_ptr)
        {
            replaced_child_ptr->parent = location;
        }
        if(root==old_node)
            root = replaced_child_ptr;
        else if(rb_tree_node_is_lchild(old_node))
            old_node->parent->left = replaced_child_ptr;
        else 
            old_node->parent->right = replaced_child_ptr;

        //修改leftmost 和right most
        if(leftmost==old_node)
            leftmost = replaced_child_ptr?rb_tree_min(replaced_child_ptr):location;
        if(rightmost==old_node)
            rightmost = replaced_child_ptr?rb_tree_max(replaced_child_ptr):location;

    }
    else
    {
        //有两个子节点的情况，需要替换节点
        //old_node的左节点问题
        old_node->left->parent = replaced_deleted_ptr;//->left;
        replaced_deleted_ptr->left = old_node->left;
        //replaced_deleted_ptr是old的右节点:
        if(replaced_deleted_ptr==old_node->right)
        {
            location = replaced_deleted_ptr;
        }
        //不是右节点
        else
        {
            location = replaced_deleted_ptr->parent;

            //child 替换replaced_delete
            if(replaced_child_ptr)
                replaced_child_ptr->parent = location;
            location->left = replaced_child_ptr;

            //replaced_deleted_ptr替换oldptr：这也是为什么分是不是右节点的原因
            replaced_deleted_ptr->right = old_node->right;
            old_node->right->parent = replaced_deleted_ptr;
            
        }
        //处理replaced_deleted_ptr与old的父节点关系
        if(root==old_node)
            root=replaced_deleted_ptr;
        else if(rb_tree_node_is_lchild(old_node))
            old_node->parent->left = replaced_deleted_ptr;
        else 
            old_node->parent->right = replaced_deleted_ptr;
        
        replaced_deleted_ptr->parent = old_node->parent;
        myrbtree::swap(replaced_deleted_ptr->color,old_node->color);
        replaced_deleted_ptr = old_node;


        
    }
    //进行平衡操作
    //现在被删除节点的颜色存在replaced_deleted_ptr中，调整完毕后返回replaced_deleted_ptr
    //被删除节点为replaced_deleted_ptr
    //被删除为黑色才进行调整
    if(!rb_tree_node_is_red(replaced_deleted_ptr)){
        //被删除为黑，且子节点为红，直接设为黑即可
        //子节点不为红，进入while调整
        //也就是说，被删除节点为：黑色叶子节点进入循环。黑色叶子节点的限定：无子节点，或者有子节点但是为黑：不可能有黑子节点。
        while (replaced_child_ptr!=root &&(!replaced_child_ptr||!rb_tree_node_is_red(replaced_child_ptr)))
        {
            //如果是原被删除节点只有左节点或者有两个节点，replaced_child_ptr在父节点的左
            //如果仅有右节点，在右
            if(replaced_child_ptr==location->left)
            {   
                //与下面对称
                //看兄弟节点
                auto brother = location->right;
                //本质上是
                //兄弟为红，转黑
                if(rb_tree_node_is_red(brother)){
                    set_black(brother);
                    set_red(location);
                    rb_tree_left_route(location,root);
                    brother = location->right;
                }
                //兄弟为黑
                //兄弟有红子节点,就是通过旋转将一个红色旋转到被删除的那一边，并且设置为黑
                if((brother->left&&rb_tree_node_is_red(brother->left))||(brother->right&&rb_tree_node_is_red(brother->right)))
                {   //红色不是在右边，双旋
                    if(!brother->right || !rb_tree_node_is_red(brother->right) )
                    {
                       if(brother->left)
                            set_black(brother->left);
                        set_red(brother);
                        rb_tree_right_route(brother,root);
                        brother = location->right;
                    }
                    //红色you，单旋
                    brother->color = location->color;
                    set_black(location);
                    if(brother->right)
                        set_black(brother->right);
                    rb_tree_left_route(location,root);
                    break;

                }
                //兄弟节点为黑，无红色子节点
                //父节点下溢，替换删除的节点，修改兄弟颜色为红
                //父节点为红，结束。父节点为黑，相当于删除了父节点的父节点，另child指向父，location指向父节点的父节点
                //综合上述两种情况，父节点下移，location指向父。：如果父红（child为红，跳出循环），否则，继续循环
                else
                {
                    set_red(brother);
                    replaced_child_ptr = location;
                    location = location->parent;
                }
                
            }
            // if(replaced_child_ptr==location->right)
            else
            {   
                //看兄弟节点
                auto brother = location->left;
                //本质上是
                //兄弟为红，转黑
                if(rb_tree_node_is_red(brother)){
                    set_black(brother);
                    set_red(location);
                    rb_tree_right_route(location,root);
                    brother = location->left;
                }
                //兄弟为黑
                //兄弟有红子节点,就是通过旋转将一个红色旋转到被删除的那一边，并且设置为黑
                if((brother->left&&rb_tree_node_is_red(brother->left))||(brother->right&&rb_tree_node_is_red(brother->right)))
                {   //红色不是在左边，双旋
                    if(!brother->left || !rb_tree_node_is_red(brother->left) )
                    {
                       if(brother->right)
                            set_black(brother->right);
                        set_red(brother);
                        rb_tree_left_route(brother,root);
                        brother = location->left;
                    }
                    //红色左，单旋
                    brother->color = location->color;
                    set_black(location);
                    if(brother->left)
                        set_black(brother->left);
                    rb_tree_right_route(location,root);
                    break;

                }
                //兄弟节点为黑，无红色子节点
                //父节点下溢，替换删除的节点，修改兄弟颜色为红
                //父节点为红，结束。父节点为黑，相当于删除了父节点的父节点，另child指向父，location指向父节点的父节点
                //综合上述两种情况，父节点下移，location指向父。：如果父红（child为红，跳出循环），否则，继续循环
                else
                {
                    set_red(brother);
                    replaced_child_ptr = location;
                    location = location->parent;
                }
            }
            
        }
        
        if(replaced_child_ptr)
            set_black(replaced_child_ptr);
        
    }
    
    return replaced_deleted_ptr;

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
int erase_unique_key(const T keys);
base_ptr erase(base_ptr iter);



//查找

base_ptr find(const T key);

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


template<class T>
int RB_Tree<T>::erase_unique_key(const T keys){
    auto it = find(keys);
    
    if(it){
       erase(it);
       return 1; 
    }
    return 0;
}

template<class T>
typename RB_Tree<T>::base_ptr
RB_Tree<T>::erase(base_ptr iter){
    --node_count;
    rb_tree_delete_fix(iter,root(),leftmost(),rightmost());
    delete iter;

}


//查找元素k，返回指针
template<class T>
typename RB_Tree<T>::base_ptr
RB_Tree<T>::find(const T key){
    auto target_ptr = _header;
    auto cur_ptr = root();
    while(cur_ptr){
        if(key<cur_ptr->value){
            target_ptr = cur_ptr;
            cur_ptr = cur_ptr->left;
        }
        else if(key>cur_ptr->value){
            cur_ptr = cur_ptr->right;
        }
        else return cur_ptr;
    }
    return nullptr;

}


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
		if (root == nullptr)
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