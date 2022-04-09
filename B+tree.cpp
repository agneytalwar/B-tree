#include<bits/stdc++.h>
using namespace std;
class key;
class node;
class key{
    public:
    key(int n,node *rightchild){
        data=n;
        rc=rightchild;
    }
    int data;
    node *rc;
};
class node{
    public:
    node(bool isin){
        isindex=isin;
        occupancy=0;
        parent=NULL;
        lc=NULL;
    }
    node* parent;
    bool isindex;
    int occupancy;
    vector<key*> elements;
    //vector<key> elements;
    node* lc;
};
class bptree{
    int d,t,num_indexnodes,num_datanodes;
    node *root;
    bool insertkey(key* sendupkey,node* leftchild,node* parentnode);
    public:
    bptree(int D,int T){
        d=D;
        t=T;
        num_datanodes=0;
        num_indexnodes=0;
        root=NULL;
        //num_datanodes=1;
        //root=new node(false);
    }
    bool insert(int n);
    void display();
};
void rearrange(node* currptr){
    int i=currptr->elements.size()-1;
    while(i>0){
        if((currptr->elements[i-1]->data)>(currptr->elements[i]->data)){
            auto t=(currptr->elements[i-1]);
            currptr->elements[i-1]=(currptr->elements[i]);
            currptr->elements[i]=t;
        }else{
            break;
        }
        i--;
    }
    //currptr->occupancy++;
}
bool bptree::insertkey(key* sendupkey,node* leftchild,node* parentnode){
    if(parentnode==NULL){
        node *newroot=new node(true);
        newroot->elements.push_back(sendupkey);
        newroot->lc=leftchild;
        newroot->occupancy++;
        num_indexnodes++;
        root=newroot;
        return true;
    }    
    //if parent node is not full we can add key to correct location and return
    if((parentnode->elements.size())<(2*t+1)){
        parentnode->elements.push_back(sendupkey);
        rearrange(parentnode);
        parentnode->occupancy++;
        return true;    
    }
    //if not we need to recursively call same procedure with middle key
    //Finding the parent of this parentnode by searching for first value of parentnode
        int searchval=parentnode->elements[0]->data;
        node* currptr=root;
        node* newparentnode=NULL;
        while((currptr->elements[0]->data)!=searchval){
            newparentnode=currptr;
            //Linear search
            int i;
            for(i=0;i<currptr->elements.size();i++){
                if(currptr->elements[i]->data>searchval){
                    i--;
                    break;
                }
            }
            if(i==(currptr->elements.size())){
                i--;
            }
            if(i!=-1){
                currptr=currptr->elements[i]->rc;
            }else{
                currptr=currptr->lc;
            }
    }
    //rest of work
    node * splitnode=new node(true);
    num_indexnodes++;
    //Adding one more key than capacity
    parentnode->elements.push_back(sendupkey);
    rearrange(parentnode);
    key* newupkey=parentnode->elements[t];
    // cout<<"push up value : "<<newupkey->data<<endl;
    node* newlc=parentnode;
    splitnode->lc=newupkey->rc;
    for(int i=t+1;i<(2*t+2);i++){
        splitnode->elements.push_back(parentnode->elements[i]);
        splitnode->occupancy++;
    }
    for(int i=t;i<(2*t+2);i++){
        parentnode->elements.pop_back();
        parentnode->occupancy--;
    }
    newupkey->rc=splitnode;
    //Recursive call
    return insertkey(newupkey,parentnode,newparentnode);
}
void bptree::display(){
    cout<<num_indexnodes<<" "<<num_datanodes<<" ";
    for(auto x: root->elements){
        cout<<x->data<<" ";
    }
    cout<<endl;
}
bool bptree::insert(int n){
    if(root==NULL){
        root= new node(false);
        num_datanodes++;
        key * newkey=new key(n,NULL);
        root->elements.push_back(newkey);
        root->occupancy++;
        return true;
    }
    //May need to add special case here for root occupancy full 
    node * currptr=root;
    node *parentnode=NULL;
    while((currptr->isindex)){
        parentnode=currptr;
        //Linear search
        int i;
        for(i=0;i<currptr->elements.size();i++){
            if((currptr->elements[i]->data)>n){
                // cout<<i<<endl;
                // cout<<(currptr->elements[i]->data)<<" is greater than "<<n<<" so we have to go back!!"<<endl;
                i--;
                // if(i!=-1){
                //     cout<<"new value of i: "<<i<<endl;
                //     cout<<"value in i is "<<(currptr->elements[i]->data)<<"and right child of i contains: ";
                //     node* xyz=currptr->elements[i]->rc;
                //     for(int i=0;i<(xyz->elements.size());i++){
                //         cout<<(xyz->elements[i]->data)<<" ";
                //     }
                //     cout<<endl;
                // }
                break;
            }
        }
        if(i==(currptr->elements.size())){
            i--;
        }
        if(i!=-1){
            currptr=currptr->elements[i]->rc;
            // cout<<"We will traverse "<<n<<" in node containing: ";
            // //Printing currptre
            // cout<<"currptr:"<<endl;
            // for(int i=0;i<currptr->elements.size();i++){
            //     cout<<(currptr->elements[i]->data)<<" ";
            // }
            // cout<<endl;
        }else{
            currptr=currptr->lc;
        }
    }
    //Now currptr contains node where new key must be inserted
    //if location has space
    if((currptr->elements.size())!=(2*d)){
        key* newkey=new key(n,NULL);
        currptr->elements.push_back(newkey);
        // rearrange(currptr);
        int i=currptr->elements.size()-1;
        while(i>0){
            if((currptr->elements[i-1]->data)>(currptr->elements[i]->data)){
                auto t=(currptr->elements[i-1]);
                currptr->elements[i-1]=(currptr->elements[i]);
                currptr->elements[i]=t;
            }
            else{
                break;
            }
            i--;
        }
        currptr->occupancy++;
        return true;
    }
    //if location does not have space 
    node* splitnode=new node(false);
    num_datanodes++;
    key* newkey=new key(n,NULL);
    //Adding one more key than capacity
    currptr->elements.push_back(newkey);
    rearrange(currptr);
    // for(int i=0;i<=(2*d);i++){
    //     cout<<(currptr->elements[i]->data)<<" ";
    // }
    // cout<<endl;
    for(int i=d;i<=(2*d);i++){
        //cout<<"pushing into splitnode : "<<currptr->elements[i]->data<<endl;
        splitnode->elements.push_back(currptr->elements[i]);
        splitnode->occupancy++;
    }
    for(int i=d;i<=(2*d);i++){
        //cout<<"poping from currptr : "<<currptr->elements[3*d-i]->data<<endl;
        currptr->elements.pop_back();
        currptr->occupancy--;
    }
    // //Printing currptre
    // cout<<"currptr:"<<endl;
    // for(int i=0;i<currptr->elements.size();i++){
    //     cout<<(currptr->elements[i]->data)<<" ";
    // }
    // cout<<endl;
    // //Printing splitnode
    // cout<<"splitnode:"<<endl;
    // for(int i=0;i<splitnode->elements.size();i++){
    //     cout<<(splitnode->elements[i]->data)<<" ";
    // }
    // cout<<endl;
    
    // splitnode->elements.push_back(newkey);
    // rearrange(splitnode);
    // splitnode->occupancy++;
    key* sendupkey=new key(splitnode->elements[0]->data,splitnode);
    // cout<<"send up value : "<<sendupkey->data<<endl;
    // cout<<"the send up key has rc as :";
    // node* xyz=sendupkey->rc;
    // for(int i=0;i<(xyz->elements.size());i++){
    //     cout<<(xyz->elements[i]->data)<<" ";
    // }
    // cout<<endl;
    return insertkey(sendupkey,currptr,parentnode);
}
int main(){
    int T,D;
    cin>>D>>T;
    bptree b1(D,T);
    int ch;
    cin>>ch;
    while(ch!=3){
        switch(ch){
            case 1:{
                int n;
                cin>>n;
                b1.insert(n);
                break;
            }
            case 2:{
                b1.display();
            }
        }
        cin>>ch;
    }
}