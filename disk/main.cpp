#include "disk.h"

class FileSystem{
    public:
        FileSystem(){
        
        }

        bool initDisk();
//文件操作列表



//存取步骤
//1.确定是否具有权限
//2.根据inode bitmap找到没有使用的inode号码,写入权限/属性
//3.根据block bitmap找到没有使用的号码,将文件写入block,并将inode的block指向这个号码
//4.更新两个bitmap,更新superblock
    private:
        int currentLocation;//当前在文件中的位置



};

//SuperBlock
//空闲的bitmap表
//Block,inode的大小
//基本信息,例如最近一次写入资料时间,修改时间,创建时间
//block,inode总量

//资料存放区:inode table,data block
//metadata:bitmap,superblock
int main(){


}

