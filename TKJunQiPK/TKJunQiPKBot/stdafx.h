// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#define WIN32_LEAN_AND_MEAN  // �� Windows ͷ���ų�����ʹ�õ�����

#ifdef _DEBUG
#pragma comment(lib, "libprotobuf_vs100_D.lib")
#else
#pragma comment(lib, "libprotobuf_vs100_R.lib")
#endif
#include <windows.h>
//#include <stdlib.h>
#include <deque>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
