#include "disk.h"


//SuperBlock
//空闲的bitmap表
//Block,inode的大小
//基本信息,例如最近一次写入资料时间,修改时间,创建时间
//block,inode总量

//资料存放区:inode table,data block
//metadata:bitmap,superblock

void bm(){
    int t=1024;
    bitset<4096> inodeMap;
    inodeMap.reset();
}

int main(){
    
        bm();
    int j=0x65171734;
    bitset<512> inodeMap(j);
    //fstream f("./a.dat",ios::out|ios::binary);
    //f.write((char*)&inodeMap,2);
    fstream f("./a.dat",ios::in|ios::binary);
    int al;
    f.read((char*)&al,2);
    cout<<al;
    
    //SuperBlock s;
    //s.blockSize=1024;
    //s.unusedBlockCount=100;
    //s.unusedInodeCount=100;
    //s.totalBlockCount=100;
    //s.blockGroupCount=35;
    //SuperBlock t;
    //cout<<f.tellg()<<" "<<f.tellp()<<endl;
    //f.read((char*) &t,sizeof(SuperBlock));
    //cout<<f.tellg()<<" "<<f.tellp()<<endl;
    //cout<<t.unusedBlockCount<<" "<<t.blockSize;
    //f.close();
}

