// Lists of Contents.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
   
	cout << "Please input absolute path of folder." << endl;
	string path_name;
	cin >> path_name;
	int nRet = files_Listing(path_name);//"D:\\github\\spotlight_pic"
	switch (nRet)	
	{
	case -2://folder not correct
	{
		cout << "Folder Path is not correctly input." << endl;
		break;
	}
	case -1://no files in folder
	{
		cout << "Folder is completely empty." << endl;
		break;
	}
	case 0:////no files in folder
	{
		cout << "Folder is completely empty." << endl;
		break;
	}
	case 1://normally run 
	{
		cout << "Output completed." << endl;
		break;
	}
	default:
		break;
	}
	system("pause");
	return 0;
}

int files_Listing(string folder_name)
{
	int nRet = 1;
	if (_access(folder_name.c_str(), 0) == -1)
	{
		nRet = -2;
		return nRet;
	}
	int files_cnt = 0;
	const double unit1 = 1024.0f;//KB
	const double unit2 = 1024.0f * 1024.0f;//MB
	const double unit3 = 1024.0f * 1024.0f * 1024.0f;//GB
	double file_size;
	_finddata_t file_info;
	fstream file_op;
	string current_path = folder_name + "\\*.*";
	string thumbnail = folder_name;
	intptr_t handle = _findfirst(current_path.c_str(), &file_info);
	if (-1 == handle)
	{
		nRet = -1;
		return nRet;
	}
	if (_access("Output", 0) == -1)//New
	{
		::CreateDirectory(L"Output", NULL);
	}
	folder_name.erase(folder_name.begin(), folder_name.begin() +
		folder_name.find_last_of('\\') + 1);
	string File_list = string("Output\\") + folder_name + string(".md");
	//
	if (_access(File_list.c_str(), 0) == -1)//New
	{
		file_op.open(File_list, ios::out | ios::app);
		bool operation_sign = md_fileoperation(0, file_op, folder_name);
	}
	else
	{
		file_op.open(File_list, ios::out | ios::app);
		bool operation_sign = md_fileoperation(1, file_op);
	}
	//	
	do
	{
		string attribute;
		string unit;
		string content;
		ostringstream buffer;
		if (file_info.attrib == _A_SUBDIR)
			attribute = "dir";
		else
		{
			attribute = "file";
			file_size = file_info.size;
			if (file_size < unit1)
			{
				unit = " Byte";
				buffer << setprecision(0) << fixed << file_size;
			}
			else if (file_size > unit1 && file_size < unit2)
			{
				file_size /= unit1;
				unit = " KB";
				buffer << setprecision(0) << fixed << file_size;
			}
			else if (file_size > unit2 && file_size < unit3)
			{
				file_size /= unit2;
				unit = " MB";
				buffer << setprecision(2) << fixed << file_size;
			}
			else if (file_size > unit3)
			{
				file_size /= unit3;
				unit = " GB";
				buffer << setprecision(1) << fixed << file_size;
			}
			string file_sz = buffer.str();
			if (stoi(file_sz) > 0)
			{
				files_cnt++;
				string ext_ = file_info.name;
				ext_.erase(ext_.begin(), ext_.begin() + ext_.find_last_of('.') + 1);
				string name_ = file_info.name;
				content = to_string(files_cnt) + ". " + name_ + "\nFile Size: " +
					file_sz + unit + "\nFile attribute: " + ext_ + "\n";
				md_fileoperation(2, file_op, content);
				if (ext_ != "md")
				{
					string thumbnail_ = thumbnail + "\\" + name_;
					content = "<img src = \"" + thumbnail_ + "\" style=\"zoom:20%\" />";
					md_fileoperation(2, file_op, content);
				}
				content = "\n---";
				md_fileoperation(2, file_op, content);
			}
		}
	} while (!_findnext(handle, &file_info));
	_findclose(handle);
	string summary = string("* Total Number of Files: ") + to_string(files_cnt) + string("\n------");
	md_fileoperation(2, file_op, summary);
	/*time_t nowtime;
	nowtime = time(NULL);
	char currentdate[255];
	strftime(currentdate, sizeof(currentdate),
	"%Y-%m-%d-%H-%M-%S", localtime(&nowtime));
	summary = "<span name = \"" + string(currentdate) + "\"  > </span>\n";
	md_fileoperation(2, file_op, summary);*/
	file_op << "\n" << endl;
	summary = "[TOC]";
	md_fileoperation(2, file_op, summary);
	file_op.close();
	if (files_cnt == 0)
	{
		nRet = 0;
	}
	return nRet;
}

bool md_fileoperation(int op, fstream& file_operation, string content)
{
	bool nRet = true;
	string info_data;

	time_t nowtime;
	nowtime = time(NULL);
	char currentdate[100];
	strftime(currentdate, sizeof(currentdate),
		"%Y-%m-%d", localtime(&nowtime));

	switch (op)
	{
	case 0://initialize
	{
		info_data = "# " + content + " File-Lists Output";
		file_operation << info_data << endl;
		info_data = "## Created On: " + string(currentdate);
		file_operation << info_data << endl;

		/*info_data = "<a name = \"" + string(currentdate) + "\" / >";
		file_operation << info_data << endl;*/
		break;
	}
	case 1:
	{
		info_data = "## Updated On: " + string(currentdate);
		file_operation << info_data << endl;

		/*info_data = "<a name = \"" + string(currentdate) + "\" / >";
		file_operation << info_data << endl;*/
		break;
	}
	case 2:
	{
		file_operation << content << endl;
		break;
	}
	default:
		break;
	}
	return nRet;
}

