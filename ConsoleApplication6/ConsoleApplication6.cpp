#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"
#include "string.h"
#include "io.h"       //关于文件目录的函数        
#define ERROR 0
//#define ElemType Student

struct Student
{
	// 请不要增加、删除或者修改成员变量
	unsigned int	ID;			 // 学号
	char			Name[8];
	char			Subject[50]; // 信息工程 或 光电信息科学与工程
	char			Phone[11];
	int				Sex;		 // 男:0; 女:1
	int				BirthYear;   // 出生年，例如1993

								 // TODO: 完善getAge()函数，使之能实现对Student的成员变量进行赋值
	void init(unsigned int id, const char* name, const char* subject, const char* phone, int sex, int birthyear)

	{
		ID = id;
		strcpy(Name, name);
		strcpy(Phone, phone);
		strcpy(Subject, subject);
		Sex = sex;
		BirthYear = birthyear;
	}

	// TODO: 完善getAge()函数，使之能计算并返回你的岁数
	int getAge()
	{
		return 2018 - BirthYear;
	}

	void print()
	{
		// TODO
		// 功能：在屏幕上打印自我介绍
		// 示例："大家好！我叫孙世贤, 男，光电信息科学与工程专业的学生，学号是2015050152，今年25岁"
		char SexStr[3];
		switch (Sex)
		{
		case 0:
			sprintf(SexStr, "男");
			break;
		case 1:
			sprintf(SexStr, "女");
			break;
		}
		printf("%8s, %3s，%20s，%15d，%5d", Name, SexStr, Subject, ID, getAge());
		//printf("%15d ", ID);   //对于作业3输出女学生的学号可以替换上一句printf语句
	}
};

struct IDInt
{

	unsigned int ID;

	void print()
	{
		printf("%d\n", ID);
	}
};

template <typename ElemType>
struct SqList
{
	//动态分配顺序存储空间
	ElemType *elem;
	int length;       //相当于uselength
	int listsize;     //相当于Maxlenth
	ElemType *p;

	//初始化顺序表 
	void init(int InitLength)
	{
		elem = (ElemType*)malloc(InitLength * sizeof(ElemType));
		if (!elem)
		{
			printf("分配内存失败");
			exit(ERROR);              //超出内存表示范围，退出程序并返回OVERFLOW的值给主调进程 
		}
		else
		{
			printf("分配内存成功\n");
		}
		length = 0;                        //空表长度为0 
		listsize = InitLength;            //初始存储容量 	
	}

	//在位置Index处插入元素e
	//Index的合法值为1<=Index<=length+1
	void insert(int Index, ElemType e)
	{
		if (Index < 1 || Index > length + 1)
			exit(0);
		if (length >= listsize)	     //检查顺序表是否已满 
		{
			elem = (ElemType*)realloc(elem, (listsize + 1) * sizeof(ElemType));  //分配新内存 
			listsize++;
		}
		for (int index = length; index >= Index; index--)
			*(elem + index) = *(elem + index - 1);   //数据后移 
		*(elem + Index - 1) = e;   //插入元素e
		length++;  //表长加1
	}

	//在顺序表中删除index位置上的元素
	//index的合法值为1<=index<=length
	void remove(int Index)
	{
		if ((Index < 1) || (Index > length))
			exit(0);
		else
		{
			for (int index = Index; index < length - 1; index++)
			{
				*(elem + index) = *(elem + index + 1);  //元素左移	
			}
			--length;   //表长减一
		}
	}

	//查找某个元素e，若找到则返回其在顺序表中的位置
	//indexOf子函数是参考课本然后自己将老师的版本改写的
	int indexOf(ElemType e, int(*MatchMethod)(ElemType, ElemType))
	{
		int index = 1;
		p = elem;
		while (index <= length && !(*MatchMethod)(*p++, e))
			++index;
		if (index <= length)
		{
			return index;
		}
		else return 0;
	}

	// 获取顺序表中的某一个元素
	ElemType get(int Index)
	{
		// 判断Index值是否合法，1 <= Index <= UsedLength + 1
		if (Index < 1 || Index >length)
		{
			printf("Invalid index.\n");
			exit(ERROR);
		}
		ElemType e = *(elem + Index - 1);
		return e;   //返回该元素
	}

	// 输出当前的顺序表
	void print()
	{
		printf("\n------顺序表开始------\n");
		for (int index = 1; index <= length; index++)  //index修改 
		{
			printf("[%0.3d]\t", index);
			(elem + index - 1)->print();
			printf("\n");
		}
		printf("------顺序表结束------\n\n");
	}

	// 将数据保存到文件，该函数已经完成，不需要修改
	//修改传值，可以利用str传递文件名字
	void save2File(const char *str)
	{
		char Filename[50];
		sprintf(Filename, "%s.dat", str);
		FILE* fid = fopen(Filename, "w+");
		fwrite(this, sizeof(ElemType), 1, fid);
		fclose(fid);
	}
};


int main()
{
	SqList<Student> sqList;;
	sqList.init(10);
	struct _finddata_t files;
	int File_Handle;
	int i = 0;
	const char* FileFolder = "C:/Users/Administrator/Desktop/dat";
	//将char*改为const char*，其路径为一常量值，不能直接用char*
	char Pattern[255];
	sprintf(Pattern, "%s/*.dat", FileFolder);

	File_Handle = _findfirst(Pattern, &files);
	if (File_Handle == -1)
	{
		printf("error\n");
		return 0;
		//修改return为return 0
	}
	do
	{
		printf("%s \n", files.name);

		char FilePath[255];
		sprintf(FilePath, "%s/%s", FileFolder, files.name);
		FILE* fid = fopen(FilePath, "r");
		Student student;
		fread(&student, sizeof(Student), 1, fid);
		fclose(fid);

		sqList.insert(1, student);

		i++;
	} while (0 == _findnext(File_Handle, &files));
	_findclose(File_Handle);
	printf("Find %d files\n", i);


	//对于所有同学所得的顺序表

	//sqList.save2File("CompleteSqList");
	sqList.print();


	//删除所有姓李的同学
	SqList<Student> SqListWithoutLi;
	Student thisStudent;
	SqListWithoutLi.init(10);
	for (int i = 1; i< sqList.length + 1; i++)
	{
		sqList.get(i);
		Student thisStudent = sqList.get(i);
		if (strncmp(thisStudent.Name, "李", 2) != 0)
		{
			SqListWithoutLi.insert(SqListWithoutLi.length + 1, thisStudent);
		}
	}
	//SqListWithoutLi.save2File("SqListWithoutLi.dat");
	SqListWithoutLi.print();

	//女同学的学号顺序表
	SqList <IDInt>sqListSex;
	Student student2Find;
	sqListSex.init(6);
	IDInt studentid;
	for (int i = 1; i< sqList.length + 1; i++)
	{
		sqList.get(i);
		student2Find = sqList.get(i);
		
		if (student2Find.Sex == 1)
		{
			studentid.ID = student2Find.ID;

			sqListSex.insert(sqListSex.length + 1, studentid);
		}
	}
	//sqListSex.save2File("GirlIDList");
	sqListSex.print();
	system("pause");
}
