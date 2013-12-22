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
    fstream *f;
    f=new fstream;
    f->open("./a.dat",ios::in|ios::out|ios::binary);
    FileSystem fs(f);
    fs.initDisk(1024);
    fs.sbPrint();
    //fstream f("./a.dat",ios::out|ios::binary);
    //f.write((char*)&inodeMap,2);
    
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

bool FileSystem::initDisk(int _blockSize){
        if(!fs)
            return false;
//==============superblock=================== 
        superBlock = new SuperBlock;
        bs=_blockSize;
        superBlock->blockSize = _blockSize;
        // get the size of the whole disk
        fs->clear();
        fs->seekg(0,ios::end);
        diskSize = fs->tellg();
        // count the number of block when the size and block size defined;
        superBlock->totalBlockCount = diskSize/_blockSize;
        superBlock->unusedBlockCount = superBlock->totalBlockCount;
        //block组数目 = 总disk大小/每个group大小
        int bgSize=bs*8*bs;
        int bgCount=0;
        superBlock->blockGroupSize=bgSize;
        if(diskSize%bgSize!=0){
           bgCount = diskSize/bgSize + 1; 
        }
        superBlock->blockGroupCount = bgCount;
        //inode数量
        superBlock->totalInodeCount = bs*8;
        superBlock->unusedInodeCount = bs*8;
        //每组block数目=inode的数目=8*blocksize
        //因此每组大小为num*bs = bs^2*8
        //1024byte的bs大小,bitmap为8192个,一组为8m
        //2048byte,bitmap=16k,一组32m
        //4096byte,bitmap=32k,一组128m
        superBlock->blockPerGroupNum = bs*8;
        superBlock->inodePerGroupNum = bs*8;

//============superblock end=================

//============GDT start=====================
    
    



//============GDT end=======================
        // map仅占用一个blocksize
        bm = new bitset<4096>;
        bm->reset();

};
