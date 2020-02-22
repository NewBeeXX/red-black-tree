#include<iostream>
#include<cassert>
#include"rbtree.h"
using namespace std;


redblacktree<int> tree;

int main(){
    int x,q,opt;
    cin>>q;
    while(q--){
        cin>>opt>>x;
        if(opt==1){
		tree.insert(x);
        }else if(opt==2){
		tree.remove(x);
        }else if(opt==3){
                cout<<tree.get_rank(x)<<endl;
        }else if(opt==4){
                cout<<(*(tree.kth(x)))<<endl;
        }else if(opt==5){
		cout<<(*(tree.lower_bound(x)))<<endl;
        }else if(opt==6){
		cout<<(*(tree.upper_bound(x)))<<endl;
        }
    }

    return 0;
}




/** 插入
  *直接作为红节点插入 不会使根到叶子路径上black数变化,但可能出现双红
  *
  */
template<typename T>
typename
redblacktree<T>::iterator redblacktree<T>::insert(T v){
        Node* p=find(v,1);
        if(_hot==NULL){
		init(v);
                return iterator(_root);
        }
        p=new Node(v,true,_hot,NULL,NULL,1);
        _hot->val<=v? _hot->rc=p : _hot->lc=p;
        SolveDoubleRed(p);
        return iterator(p);
}


/** @brief (one liner)
  *可以找到==v的节点
  * (documentation goes here)
  */
  template<typename T>
typename
redblacktree<T>::Node* redblacktree<T>::rfind(T v, const int op){
        Node* p=_root;
        _hot=NULL;
        while(p&&p->val!=v){
                _hot=p;
                p->s+=op;
		p=v<p->val?p->lc:p->rc;
        }
        return p;

}

/** @brief (one liner)
  *最后一个非空节点记录在_hot, 整棵树一致地采用v<节点 往左, v>=节点 往右
  * (documentation goes here)
  */
template<typename T>
typename
redblacktree<T>::Node* redblacktree<T>::find(T v, const int op){
        Node* p=_root;
        _hot=NULL;
        while(p){
                _hot=p;
                p->s+=op;
                p=v<p->val?p->lc:p->rc;
        }
        return p;
}

/** 插入第一个节点
  *
  * (documentation goes here)
  */
template<typename T>
void redblacktree<T>::init(T v){
        _root=new Node(v,false,NULL,NULL,NULL,1);
}




/** @brief (one liner)
  *
  * (documentation goes here)
  */
template<typename T>
typename
redblacktree<T>::iterator redblacktree<T>::upper_bound(T v){
        Node* p=_root;
        while(p){
                _hot=p;
                p=v<p->val?p->lc:p->rc;
        }
        p=_hot->val>v?_hot:_hot->rightNode();
        return iterator(p);
}

/** @brief (one liner)
  * 注意这里找的是<v的最大数
  * (documentation goes here)
  */
template<typename T>
typename
redblacktree<T>::iterator redblacktree<T>::lower_bound(T v){
        Node* p=_root;
        ///首先记住平衡树不管怎么旋转,中序遍历序列总是不递减的
        ///在p->val<v时,直接往右边走。因为上面这个性质，所以可以直接当作是对有序数组二分.
        while(p){
		_hot=p;
		p=p->val<v?p->rc:p->lc;
        }
        ///有时候会碰到_hot==v的情况
        p=_hot->val<v?_hot:_hot->leftNode();
        return iterator(p);
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
template<typename T>
typename
redblacktree<T>::iterator redblacktree<T>::kth(int rank){
        return iterator(findkth(rank,_root));
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
template<typename T>
typename
redblacktree<T>::Node* redblacktree<T>::findkth(int k, Node* u){
        ///尝试自己写
        int sz=(u->lc?u->lc->s:0)+1;
        if(k==sz)return u;
        else if(k<sz)return findkth(k,u->lc);
        else return findkth(k-sz,u->rc);
}

/** @brief (one liner)
  *取得v的排名 (比v小的数+1)
  * (documentation goes here)
  */
template<typename T>
int redblacktree<T>::get_rank(T v){
        return find_rank(v,_root);
}

/** @brief (one liner)
  *其实就是找到中序第一个val的节点 注意取左儿子时判断非空
  * (documentation goes here)
  */
template<typename T>
int redblacktree<T>::find_rank(T v, Node* u){
        if(!u)return 1;
        if(u->val<v)return find_rank(v,u->rc)+(1+(u->lc?u->lc->s:0));
        else return find_rank(v,u->lc);
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
template<typename T>
bool redblacktree<T>::empty(){
        return !_root;
}

/** @brief (one liner)
  *
  * (documentation goes here)
  */
template<typename T>
int redblacktree<T>::size(){
        return _root->s;
}


//        Node* succ(){
//                Node* p=rc;
//                for(;p->lc;p=p->lc)
//                        p->s--;
//		return p;
//        }

/** @brief (one liner)
  * 这里面用到了rfind ,注意rfind仅用于随便查找一个==的节点,插入什么的还是用find找位置
  * (documentation goes here)
  */
template<typename T>
bool redblacktree<T>::remove(T v){
        Node* p=rfind(v,-1);
        if(!p)return false;
        Node* suc;
        while(p->lc||p->rc){///不断替换变成删除叶节点,替换不会改变颜色,仅仅是val的替换
                if(p->lc==NULL)suc=p->rc;
                else if(p->rc==NULL)suc=p->lc;
                else suc=p->succ();///找到中序遍历p的后继
                p->s--;
                p->val=suc->val;
                p=suc;
        }
        if(p->RBc==false){
                p->s--;
                SolveDoubleBlack(p);
        }
        if(p==_root){
		_root=NULL;
		delete p;
		return true;
        }
        if(islc(p))p->par->lc=NULL;
        else p->par->rc=NULL;

        delete p;
        return true;
}









/** @brief (one liner)
  *
  * (documentation goes here)
  */
template<typename T>
void redblacktree<T>::SolveDoubleBlack(Node* u){
        while(u!=_root){
                Node* pp=u->par;
                Node* bb=bro(u);
                if(bb->RBc){ ///兄弟红
                        bb->RBc=false;
                        pp->RBc=true;
                        if(_root==pp)_root=bb;
                        if(pp->par){
                                if(islc(pp))pp->par->lc=bb;
                                else pp->par->rc=bb;
                        }
                        bb->par=pp->par;
                        if(islc(u))connect34(bb,pp,bb->rc,u,bb->lc,bb->rc->lc,bb->rc->rc);
			else connect34(bb,bb->lc,pp,bb->lc->lc,bb->lc->rc,bb->rc,u);
			bb=bro(u);
                        pp=u->par;
                }
                ///其实如果原本兄弟红的 转化后都变成了兄弟黑 BB-3
                if(bb->lc&&bb->lc->RBc){///兄弟黑且有红左儿子  兄弟黑则不可能有黑儿子
                        bool oldrbc=pp->RBc;
                        pp->RBc=false;
                        if(pp->lc==u){
                                if(pp->par){
                                        if(islc(pp))pp->par->lc=bb->lc;
                                        else pp->par->rc=bb->lc;
                                }
                                bb->lc->par=pp->par;
                                if(_root==pp)_root=bb->lc;
                                connect34(bb->lc,pp,bb,pp->lc,bb->lc->lc,bb->lc->rc,bb->rc);
                        }else{
                                bb->lc->RBc=false;
                                if(pp->par){
                                        if(islc(pp))pp->par->lc=bb;
                                        else pp->par->rc=bb;
                                }
                                bb->par=pp->par;
                                if(_root==pp)_root=bb;
                                connect34(bb,bb->lc,pp,bb->lc->lc,bb->lc->rc,bb->rc,pp->rc);
                        }
                        pp->par->RBc=oldrbc;
                        return;
                }else if(bb->rc&&bb->rc->RBc){ /// 兄弟黑且有右红儿子
                        bool oldrbc=pp->RBc;
                        pp->RBc=false;
                        if(pp->lc==u){
                                bb->rc->RBc=false;
                                if(pp->par){
                                        if(islc(pp))pp->par->lc=bb;
                                        else pp->par->rc=bb;
                                }
                                bb->par=pp->par;
                                if(_root==pp)_root=bb;
                                connect34(bb,pp,bb->rc,pp->lc,bb->lc,bb->rc->lc,bb->rc->rc);
                        }else{
				if(pp->par){
					if(islc(pp))pp->par->lc=bb->rc;
					else pp->par->rc=bb->rc;
				}
				bb->rc->par=pp->par;
                                if(_root==pp)_root=bb->rc;
                                connect34(bb->rc,bb,pp,bb->lc,bb->rc->lc,bb->rc->rc,pp->rc);
                        }
                        pp->par->RBc=oldrbc;
                        return;
                }
                if(pp->RBc){ ///父亲红(意思是兄弟黑但没儿子 但兄弟不会不存在的否则原本就不黑平衡)
                        //BB-2R
			pp->RBc=false;
			bb->RBc=true;
                        return;
                }else{ //就是BB-2B
                        bb->RBc=true;
                        u=pp;
                }
        }
#ifdef __REDBLACK_DEBUG
    --blackheight;
#endif

}

/** @brief (one liner)
  * 双红
  * (documentation goes here)
  */
template<typename T>
void redblacktree<T>::SolveDoubleRed(Node* u){
        while(u->par==NULL||u->par->RBc){
                if(u==_root){
			_root->RBc=false;
                        return;
                }
                Node* pp=u->par;
                assert(pp->RBc==true);
                Node* unc=bro(pp);
                Node* gp=pp->par;
                assert(gp!=NULL);///可以保证既然par为红则绝对有祖父节点为黑
                if(unc&&unc->RBc){ ///uncle为红 递归
                        gp->RBc=true;
                        unc->RBc=pp->RBc=false;
                        u=gp;
                }else{ ///不需要递归的情况
                        if(islc(pp)){
                                if(islc(u)){///同向
                                        pp->par=gp->par;
                                        if(gp==_root)_root=pp;
                                        else if(islc(gp))gp->par->lc=pp;
                                        else gp->par->rc=pp;
                                        connect34(pp,u,gp,u->lc,u->rc,pp->rc,unc);
                                        pp->RBc=false;
                                        gp->RBc=true;
                                }else{///不同向
                                        u->par=gp->par;
					if(gp==_root)_root=u;
                                        else if(islc(gp))gp->par->lc=u;
                                        else gp->par->rc=u;
					connect34(u,pp,gp,pp->lc,u->lc,u->rc,unc);
                                        u->RBc=false;
                                        gp->RBc=true;
                                }
                        }else{///镜像
                                if(isrc(u)){
                                        pp->par=gp->par;
					if(gp==_root)_root=pp;
                                        else if(islc(gp))gp->par->lc=pp;
                                        else gp->par->rc=pp;
                                        connect34(pp,gp,u,unc,pp->lc,u->lc,u->rc);
                                        pp->RBc=false;
                                        gp->RBc=true;
                                }else{
                                        u->par=gp->par;
					if(gp==_root)_root=u;
                                        else if(islc(gp))gp->par->lc=u;
                                        else gp->par->rc=u;
                                        connect34(u,gp,pp,unc,u->lc,u->rc,pp->rc);
                                        u->RBc=false;
                                        gp->RBc=true;
                                }
                        }
                        return;
                }
        }
}

/** @brief (one liner)
  * 实现旋转
  * (documentation goes here)
  */
template<typename T>
void redblacktree<T>::connect34(Node* rt, Node* le, Node*ri, Node* u1, Node* u2, Node* u3, Node* u4){
        rt->lc=le,le->par=rt;
        rt->rc=ri,ri->par=rt;
        le->lc=u1,le->rc=u2,ri->lc=u3,ri->rc=u4;
        if(u1)u1->par=le;
        if(u2)u2->par=le;
        if(u3)u3->par=ri;
        if(u4)u4->par=ri;
        le->maintain(),ri->maintain(),rt->maintain();
}



