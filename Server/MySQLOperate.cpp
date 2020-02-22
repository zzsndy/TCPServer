#include "mysql.h"
#include "MySQLOperate.h"

#define host "192.168.128.103"
#define user "root"
#define passwd "111111"
#define table "transerver"

MySQLOperate::MySQLOperate()
{
    cout << "ehllo" << endl;
}

MySQLOperate::~MySQLOperate()
{
}

using namespace std;

int main()
{
    const char host[] = "localhost"; //MySQL服务器IP地址；若是本地可填写“localhost”或127.0.0.1
    const char user[] = "root";      //MySQL的用户名
    const char pswd[] = "root";      //密码
    const char table[] = "test";     //数据库名称
    unsigned int port = 3306;        //MySQL服务端口号，默认是3306
    MYSQL myCont;                    //创建MYSQL对象，定义数据库连接句柄
    MYSQL_RES *result;               //查询结果集，存放查询结果
    MYSQL_ROW sql_row;               //存放一行查询结果的字符串数组
    MYSQL_FIELD *fd;
    / 包含字段信息的结构 char column[32][32];
    int res;
    mysql_library_init(0, NULL, NULL); //初始化MySQL库
    mysql_init(&myCont);               //初始化连接处理程序
    if (mysql_real_connect(&myCont, host, user, pswd, table, port, NULL, 0))
    { //通过调用mysql_real_connect()连接到服务器
        cout << "connect succeed!" << endl;
        mysql_query(&myCont, "SET NAMES GBK");               //设置编码格式,否则在cmd下无法显示中文
        res = mysql_query(&myCont, "select * from samples"); //执行查询语句，mysql_query如果查询成功，零；如果出现一个错误，非零。
        if (!res)
        {
            result = mysql_store_result(&myCont); //保存查询到的数据到result
            if (result)
            {
                int i, j;
                cout << "number of result: " << (unsigned long)mysql_num_rows(result) << endl;
                for (i = 0; fd = mysql_fetch_field(result); i++) //获取列名
                {
                    strcpy(column[i], fd->name);
                }
                j = mysql_num_fields(result);
                for (i = 0; i < j; i++)
                {
                    printf("%s\t", column[i]);
                }
                printf("\n");
                while (sql_row = mysql_fetch_row(result)) //获取具体的数据
                {
                    for (i = 0; i < j; i++)
                    {
                        printf("%s\n", sql_row[i]);
                    }
                    printf("\n");
                }
            }
        }
        else
        {
            cout << "query sql failed!" << endl;
        }
    }
    else
    {
        cout << "connect failed!" << endl;
    }
    //注意用完数据库要及时回收资源
    if (result != NULL)
        mysql_free_result(result); //释放结果资源
    mysql_close(&myCont);          //关闭MySQL连接
    mysql_library_end();           //关闭MySQL库
    return 0;
}
————————————————
