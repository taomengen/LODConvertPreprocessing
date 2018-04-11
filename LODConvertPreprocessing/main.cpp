#include <windows.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <direct.h>

using namespace std;
const wchar_t* iniPath = L".\\Config.ini";

char* W2M(wchar_t *p)
{
	//获取转换所需的目标缓存大小  
	DWORD dBufSize=WideCharToMultiByte(CP_OEMCP, 0, p, -1, NULL,0,NULL, FALSE);  
	//分配目标缓存  
	char *dBuf = new char[dBufSize];  
	memset(dBuf, 0, dBufSize);  
	//转换  
	int nRet=WideCharToMultiByte(CP_OEMCP, 0, p, -1, dBuf, dBufSize, NULL, FALSE);  
	return dBuf;
}

wchar_t MetaDataPathBuff[256];
DWORD dWord1 = GetPrivateProfileString(L"DataPath", //节名
							L"MetaDataPath", //项名
							L"No Path", //没找到此项时的返回值
							MetaDataPathBuff, //目标缓冲区地址
							256, //目标缓冲区长度
							iniPath); //配置文件的准确路径
char* preMetaDataPath = W2M(MetaDataPathBuff);
string MetaDataPath(preMetaDataPath);

wchar_t LODFolderPathBuff[256];
DWORD dWord2 = GetPrivateProfileString(L"DataPath", //节名
							L"LODFolderPath", //项名
							L"No Path", //没找到此项时的返回值
							LODFolderPathBuff, //目标缓冲区地址
							256, //目标缓冲区长度
							iniPath); //配置文件的准确路径
char* preLODFolderPath = W2M(LODFolderPathBuff);
string LODFolderPath(preLODFolderPath);

void CreateFolderAndMoveFile()
{
	cout << MetaDataPath << endl;
	cout << LODFolderPath << endl;
	cout << "解析配置文件成功！" << endl;
	string dirstr = "dir /b /ad " + MetaDataPath + "\\*.*" + " > " + ".\\dir.txt";
	system(dirstr.c_str());
	ifstream in(".\\dir.txt");  
    string line;  
    if(in) // 有该文件 
    {  
        while (getline (in, line)) // line中不包括每行的换行符  
        {
			////////////////////////////////////////////////////生成模型文件夹和纹理文件夹
			string modelPath = MetaDataPath + "\\" + line + "\\model";
			string texturePath = MetaDataPath + "\\" + line + "\\texture";

			string strModel = "md " + modelPath;
			string strTexture = "md " + texturePath;

			system(strModel.c_str());
			system(strTexture.c_str());

			////////////////////////////////////////////////////移动模型和纹理
			string str1 = "move " + MetaDataPath + "\\" + line + "\\*.wrl " +  MetaDataPath + "\\" + line + "\\model";
			system(str1.c_str());
	
			string str2 = "move " + MetaDataPath + "\\" + line + "\\*.dds " +  MetaDataPath + "\\" + line + "\\texture";
			system(str2.c_str());

			/*
			string str3 = "del " + MetaDataPath + "\\" + line + "\\*.wrl";
			system(str3.c_str());

			string str4 = "del " + MetaDataPath + "\\" + line + "\\*.dds";
			system(str4.c_str());
			*/

			////////////////////////////////////////////////////生成LOD数据文件夹
			string LODPath = LODFolderPath + "\\" + line;
			const char *pLODPath=LODPath.c_str();
			mkdir(pLODPath);
			cout << "创建文件夹" << line << endl;	
        }  
    }  
    else // 没有该文件  
    {  
        cout <<"No such file!" << endl;  
    } 
}

int main()
{
	CreateFolderAndMoveFile();
	cout << "LOD转换预处理工作完成，可以进行LOD转换！";
	system("pause");
	return 0;
}