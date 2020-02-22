#ifndef RBTREE_H_INCLUDED
#define RBTREE_H_INCLUDED

#include<cstdio>

#define bro(x) ( ((x)->par->lc==(x)) ? ((x)->par->rc) : ((x)->par->lc) )
#define islc(x) ((x)!=NULL&&(x)->par->lc==(x))
#define isrc(x) ((x)!=NULL&&(x)->par->rc==(x))

template<typename T>
class redblacktree{
protected:
        struct Node;
        Node* _root;
        Node* _hot;

        void init(T);
        void connect34(Node*,Node*,Node*,Node*,Node*,Node*,Node*);
        void SolveDoubleRed(Node*);
        void SolveDoubleBlack(Node*);
        Node* find(T,const int); ///允许重复的查找
        Node* rfind(T,const int);
	Node* findkth(int ,Node*);
	int find_rank(T,Node*);

public:
	struct iterator;
	redblacktree():_root(NULL),_hot(NULL){}
        int get_rank(T);
        iterator insert(T);
        bool remove(T);
        int size();
        bool empty();
        iterator kth(int);
        iterator lower_bound(T);
        iterator upper_bound(T);
};

template<typename T>
struct redblacktree<T>::Node{
        T val;
        bool RBc;///true 为red
        Node* par;
        Node* lc;
        Node* rc;
        int s;

        Node(T v=T(),bool RB=true,Node* f=NULL,Node*l=NULL,Node* r=NULL,int ss=1):
                val(v),RBc(RB),par(f),lc(l),rc(r),s(ss){}
        Node* succ(){
                Node* p=rc;
                for(;p->lc;p=p->lc)
                        p->s--;
		return p;
        }
        Node* leftNode(){///找中序遍历的前驱节点
                Node* p=this;
                if(!lc){///p是par的右儿子即找到了
                        for(;p->par&&islc(p);p=p->par);
                        p=p->par;
                }else{
			p=p->lc;
                        for(;p->rc;p=p->rc);
                }
                return p;
        }
	Node* rightNode(){///找中序遍历后驱节点
		Node* p=this;
                if(!rc){///p是par的左儿子即找到了
                        for(;p->par&&isrc(p);p=p->par);
                        p=p->par;
                }else{
			p=p->rc;
                        for(;p->lc;p=p->lc);
                }
                return p;
	}
	void maintain(){
		s=1;
		if(lc)s+=lc->s;
		if(rc)s+=rc->s;
	}

};

template<typename T>
struct redblacktree<T>::iterator{
private:
        Node* u;
public:

        iterator& operator++(){
        	u=u->rightNode();
                return *this;
        }
        iterator& operator--(){
        	u=u->leftNode();
                return *this;
        }
        T operator*(){
                return u->val;
        }

	iterator(Node* u=NULL):u(u){}
	iterator(T const& val):u(rfind(val,0)){}
        iterator(iterator const& it):u(it.u){}
};





#endif // RBTREE_H_INCLUDED
