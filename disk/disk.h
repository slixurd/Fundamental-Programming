#ifndef DISK_H
#define DISK_H

#include<iostream>
#include<fstream>
#include<ctime>
#include<bitset>
using namespace std;

struct SuperBlock{
/*
 * 超级块
 * 每个块组前都有一个备份
 * 
 */
    int blockSize;//block大小,单位byte
    long unusedBlockCount;//未使用block数目
    long totalBlockCount;//总计block数目
    long unusedInodeCount;//未使用inode数目
    long totalInodeCount;//总计inode数目
    long blockGroupCount;//block组数目
    int blockGroupSize;//block组大小
    int blockPerGroupNum;//每组block数目
    int inodePerGroupNum;//每组inode数目
};

struct GDT{
/*块组描述符
 * Group Descriptor Table
 *
 */
    long blockLoc;
    long inodeLoc;
    long inodeTableLoc;
    int unusedInodeCount;
    int unusedBlockCount;
    int directoryCount;//目录数
};

struct inode{
    int mod;//权限,采用位操作,1,2,4分别代表root,user,guest
    unsigned iCount;//引用次数
    long size;
    bool isDirectory;
    int uid;//拥有者id
    //gid_t gid;//群组id,暂时不做分组
    time_t mtime;//修改时间
    time_t atime;//读取时间
    time_t ctime;//创建时间
    int blocks;//使用了的block个数
    long block[6];
    long redirect1;//间接指针,指向inode number
    long redirect2;
    long redirect3;
};


struct GroupHeader{
    SuperBlock superBlock;
    GDT xGDT;
//头部后面的数据
//    block bitmap;
//    inode bitmap;
//    inode table;
//    data;
};


class FileSystem{
public:
    FileSystem(fstream* _fs){
        fs = _fs;
    
    }

    bool initDisk(int _blockSize);
    void sbPrint(){
        cout<<"diskSize\t\t"<<diskSize<<endl;
        cout<<"blockSize\t\t"<<superBlock->blockSize<<endl;
        cout<<"BlockCount\t\t"<<superBlock->unusedBlockCount<<endl;
        cout<<"BgSize\t\t"<<superBlock->blockGroupSize<<endl;
        cout<<"blockGroupCount\t\t"<<superBlock->blockGroupCount<<endl;
        cout<<"inodeCount\t\t"<<superBlock->totalInodeCount<<endl;
    }
//文件操作列表



//存取步骤
//1.确定是否具有权限
//2.根据inode bitmap找到没有使用的inode号码,写入权限/属性
//3.根据block bitmap找到没有使用的号码,将文件写入block,并将inode的block指向这个号码
//4.更新两个bitmap,更新superBlock
private:
    int currentLocation;//当前在文件中的位置
    int diskSize;
    int bs;
    int bitmapSize;
    GDT* cGDT;
    bitset<4096> *bm;//block可以为1,2,4k,以最大4k设计bitmap
    SuperBlock *superBlock;
    fstream* fs;
    
    bool headerToDisk(){
        if(fs==NULL)return false;
        fs->seekg(0);
        fs->seekp(0);
        //fs->write();
    
    }
};

#endif
