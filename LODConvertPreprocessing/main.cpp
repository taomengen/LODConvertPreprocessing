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
	//��ȡת�������Ŀ�껺���С  
	DWORD dBufSize=WideCharToMultiByte(CP_OEMCP, 0, p, -1, NULL,0,NULL, FALSE);  
	//����Ŀ�껺��  
	char *dBuf = new char[dBufSize];  
	memset(dBuf, 0, dBufSize);  
	//ת��  
	int nRet=WideCharToMultiByte(CP_OEMCP, 0, p, -1, dBuf, dBufSize, NULL, FALSE);  
	return dBuf;
}

wchar_t MetaDataPathBuff[256];
DWORD dWord1 = GetPrivateProfileString(L"DataPath", //����
							L"MetaDataPath", //����
							L"No Path", //û�ҵ�����ʱ�ķ���ֵ
							MetaDataPathBuff, //Ŀ�껺������ַ
							256, //Ŀ�껺��������
							iniPath); //�����ļ���׼ȷ·��
char* preMetaDataPath = W2M(MetaDataPathBuff);
string MetaDataPath(preMetaDataPath);

wchar_t LODFolderPathBuff[256];
DWORD dWord2 = GetPrivateProfileString(L"DataPath", //����
							L"LODFolderPath", //����
							L"No Path", //û�ҵ�����ʱ�ķ���ֵ
							LODFolderPathBuff, //Ŀ�껺������ַ
							256, //Ŀ�껺��������
							iniPath); //�����ļ���׼ȷ·��
char* preLODFolderPath = W2M(LODFolderPathBuff);
string LODFolderPath(preLODFolderPath);

void CreateFolderAndMoveFile()
{
	cout << MetaDataPath << endl;
	cout << LODFolderPath << endl;
	cout << "���������ļ��ɹ���" << endl;
	string dirstr = "dir /b /ad " + MetaDataPath + "\\*.*" + " > " + ".\\dir.txt";
	system(dirstr.c_str());
	ifstream in(".\\dir.txt");  
    string line;  
    if(in) // �и��ļ� 
    {  
        while (getline (in, line)) // line�в�����ÿ�еĻ��з�  
        {
			////////////////////////////////////////////////////����ģ���ļ��к������ļ���
			string modelPath = MetaDataPath + "\\" + line + "\\model";
			string texturePath = MetaDataPath + "\\" + line + "\\texture";

			string strModel = "md " + modelPath;
			string strTexture = "md " + texturePath;

			system(strModel.c_str());
			system(strTexture.c_str());

			////////////////////////////////////////////////////�ƶ�ģ�ͺ�����
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

			////////////////////////////////////////////////////����LOD�����ļ���
			string LODPath = LODFolderPath + "\\" + line;
			const char *pLODPath=LODPath.c_str();
			mkdir(pLODPath);
			cout << "�����ļ���" << line << endl;	
        }  
    }  
    else // û�и��ļ�  
    {  
        cout <<"No such file!" << endl;  
    } 
}

int main()
{
	CreateFolderAndMoveFile();
	cout << "LODת��Ԥ��������ɣ����Խ���LODת����";
	system("pause");
	return 0;
}