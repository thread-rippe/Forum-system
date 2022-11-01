#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
using namespace std;
int main(){
    char *buf;
    string name, passwd, age, sex, who;
    if((buf = getenv("QUERY_STRING")) != nullptr){
        string temp(buf);
        for(auto& c: temp){
            if(c == '&'){
                c = ' ';
            }
        }
        istringstream input(temp);
        input >> name >> passwd >> age >> sex >> who;
    }
    string way = "html/";
    way += name;
    way += ".html";
    cout << way << endl;
    ofstream out(way);
    if(out.is_open()){
        cout << "成功打开" << endl;
    }else{
        cout << "打开失败" << endl;
        return 0;
    }
    out << "<html>" << endl;
    out << "<head>" << endl;
    out << "<meta charset=utf-8>" << endl;
    out << "<title>个人信息</title>" << endl;
    out << "<style type=\"text/css\">" << endl;
    out << "h1{" << endl;
    out << "font-size:30px;" << endl;
    out << "color:#930;" << endl;
    out << "text-align:center;" << endl;
    out << "}" << endl;
    out << "</style>" << endl;
    out << "</head>" << endl;
    out << "<body>" << endl;
    out << "<h1>个人信息</h1>" << endl;
	out << "<p>" << endl;
    out << "<b>用户名：" + name + "</b><br>" << endl;
    out << "<b>密码：" + passwd + "</b><br>" << endl;
    out << "<b>年龄：" + age + "</b><br>" << endl;
    out << "<b>性别：" + sex + "</b><br>" << endl;
    if(who == "company"){
        out << "<b>您是企业用户。</b><br>" << endl;
    }else{
        out << "<b>您是普通用户。</b><br>" << endl;
    }
    out << "</p>" << endl;
    out << "<a href=\"/\">回到主页面</a>" << endl;
    out << "<br/>" << endl;
    out << "<a href=\"home.html\">返回登陆界面</a>" << endl;
    out << "</form>" << endl;
    out << "</body>" << endl;
    out << "</html>" << endl;
    out.close();
    return 0;
}
