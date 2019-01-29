// Lists of Contents.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int files_Listing(string folder_name, int& files_cnt, fstream& file_op, int op_mode = 0);

bool md_fileoperation(int op, fstream& file_operation, string content = "");



int main()
{
	cout << "Please input absolute path of folder." << endl;
	string path_name;
	cin >> path_name;
	fstream file_op(path_name);
	int files_cnt = 0;
	int nRet = files_Listing(path_name, files_cnt, file_op);//D:\github\spotlight_pic  D:\github\test
	switch (nRet)
	{
	case -2://folder not correct
	{
		cout << "Folder Path is not correctly input." << endl;
		break;
	}
	case -1://file access failed
	{
		cout << "Files involved can not be accessed to correctly." << endl;
		break;
	}
	case 0://normally run
	{
		cout << "Output completed." << endl;
		break;
	}
	default:
		break;
	}
	md_fileoperation(3, file_op, to_string(files_cnt));
	
	system("pause");
	return 0;
}

int files_Listing(string folder_name, int& files_cnt, fstream& file_op, int op_mode)
{
	int nRet =0;
	if (_access(folder_name.c_str(), 0) == -1)
	{
		nRet = -2;
		return nRet;
	}
	//define
	vector<string> _extension1{ "jpg", "png", "pdf", "bmp", "gif", "tif", "Tiff", "psd" };
	vector<string> _extension2{ "mp4", "avi", "mkv", "m2ts", "iso", 
		"rmvb", "RMVB", "fov", "flv", "rm", "RM"};
	const double unit1 = 1024.0f;//KB
	const double unit2 = 1024.0f * 1024.0f;//MB
	const double unit3 = 1024.0f * 1024.0f * 1024.0f;//GB
	double file_size;
	_finddata_t file_info;
	string thumbnail = folder_name;
	string current_path = folder_name + "\\*.*";
	intptr_t handle = _findfirst(current_path.c_str(), &file_info);
	if (-1 == handle)
	{
		nRet = -1;
		return nRet;
	}
	//
	if (!op_mode)
	{
		if (_access("Output", 0) == -1)//new
		{
			::CreateDirectory(L"Output", NULL);
		}
		folder_name.erase(folder_name.begin(), folder_name.begin() +
			folder_name.find_last_of('\\') + 1);
		string File_list = string("Output\\") + folder_name + string(".md");
		file_op.close();
		if (_access(File_list.c_str(), 0) == -1)//create
		{
			file_op.open(File_list, ios::out | ios::app);
			bool operation_sign = md_fileoperation(0, file_op, folder_name);
		}
		else
		{
			file_op.open(File_list, ios::out | ios::app);
			bool operation_sign = md_fileoperation(1, file_op);
		}
	}
	//
	do
	{
		string attribute;
		string unit;
		string content;
		ostringstream buffer;
		string iden = file_info.name;
		if (iden ==  "." || iden == "..")
		{
			continue;
		}
		if (file_info.attrib == _A_SUBDIR)
		{
			attribute = "dir";
			content = "---\n[" + string(file_info.name) + 
				"](" + thumbnail + "\\" + iden + ")\nAttribute: " + attribute + "\n";
			md_fileoperation(2, file_op, content);
			files_Listing(thumbnail + "\\" + iden, files_cnt, file_op, 1);
		}
		else
		{
			attribute = "file";
			file_size = file_info.size;
#pragma region Filesize_iden
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
#pragma endregion
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
				if (find(_extension1.begin(), _extension1.end(),ext_) != _extension1.end()
					&& name_ == "poster." + ext_)
				{
					string thumbnail_ = thumbnail + "\\" + name_;
					content = "<img src = \"" + thumbnail_ + "\" style=\"zoom:35%\" />\n";
					md_fileoperation(2, file_op, content);
				}
				/*content = "\n---";
				md_fileoperation(2, file_op, content);*/
			}
		}
	} while (!_findnext(handle, &file_info));
	md_fileoperation(2, file_op, "---\n");
	_findclose(handle);
	/*time_t nowtime;
	nowtime = time(NULL);
	char currentdate[255];
	strftime(currentdate, sizeof(currentdate),
	"%Y-%m-%d-%H-%M-%S", localtime(&nowtime));
	summary = "<span name = \"" + string(currentdate) + "\"  > </span>\n";
	md_fileoperation(2, file_op, summary);*/
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
		info_data = "# [" + content + "](" + + " File-Lists Output";
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
	case 3:
	{
		string summary = string("* Total Number of Files: ") + content + string("\n------");
		file_operation << summary << endl;
		file_operation << "\n" << endl;
		summary = "[TOC]";
		file_operation << summary << endl;
		file_operation.close();
		break;
	}
	default:
		break;
	}
	return nRet;
}
